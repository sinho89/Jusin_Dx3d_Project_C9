#include "stdafx.h"
#include "Summon.h"
#include "Bardiel.h"
#include "Export_Engine.h"
#include "Curtain.h"
#include "Sprite.h"
#include "SummonParticle.h"
#include "Twister.h"
#include "Wave.h"
#include "Sound.h"

CSummon::CSummon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pLeftHandBoneMat(NULL)
, m_pRightHandBoneMat(NULL)
, m_pCurtain(NULL)
, m_bCurtainCheck(false)
, m_bHandParticleCheck(false)
, m_pLeftSprite(NULL)
, m_pRightSprite(NULL)
, m_bTwoHandFaceCheck(false)
, m_iSpriteCheck(0)
, m_bSpriteOff(false)
, m_bWakeUpCheck(false)
, m_bSpriteScaleMinus(false)
, m_bReturnHandSprite(false)
, m_bLastScaleMinus(false)
, m_bCutainCheck(false)
, m_pLightParticle(NULL)
, m_pLeftParticle(NULL)
, m_pTwister(NULL)
, m_bWakeUpTwistCheck(false)
, m_bDestroyTwist(false)
, m_pGroundDecal(NULL)
, m_bWaveState(false)
, m_bDecalCheck(false)
, m_fEffectPower(0.0f)
{
	m_bAni = true;

	ZeroMemory(&m_pGroundWave, sizeof(CWave*) * 3);
}

CSummon::~CSummon(void)
{
}

HRESULT CSummon::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::Find(L"Mesh_Bardiel");
	if (pGameObject == NULL)
		return E_FAIL;

	//사운드
	m_pSoundCom->Set_Sound(L"SpinCircle", L"Bardiel_CameraIntro_SpinCircle.wav");
	m_pSoundCom->Set_Sound(L"Curtain", L"Bardiel_CameraIntro_Curtain.wav");
	m_pSoundCom->Set_Sound(L"Ball", L"Bardiel_CameraIntro_Ball.wav");

	m_pTargetAnimator = ((CBardiel*)pGameObject)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//Hanuman TransCom
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//20 오른손바닥
	m_pRightHandBoneMat = m_pTargetMesh->Get_FrameByName("_20");

	//36 왼손바닥
	m_pLeftHandBoneMat = m_pTargetMesh->Get_FrameByName("_36");

	Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CSummon::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (!m_bAni)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::WAKEUP)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//WakeUp----------------------------
		if (0.75f <= dTargetTime && 1.235f > dTargetTime)
		{
			m_fEffectPower += 0.05f;
			if (m_fEffectPower > 5.5f)
				m_fEffectPower = 5.5f;

			Engine::SetRadialBlurPower(m_fEffectPower);
			Engine::SetRadialBlur(TRUE);
		}

		if (1.235f <= dTargetTime && 3.2200001f > dTargetTime)
		{
			m_fEffectPower -= 0.05f;
			if (m_fEffectPower < 0.0f)
				m_fEffectPower = 0.0f;

			Engine::SetRadialBlurPower(m_fEffectPower);
		}

		if (3.2200001f <= dTargetTime && 3.3f > dTargetTime)
		{
			Engine::SetRadialBlur(FALSE);
			m_fEffectPower = 1.7f;
		}
		//----------------------------------		

		if ((11.f <= dTargetTime) && (!m_bWakeUpTwistCheck)) //바르디엘 일어나는 순간
		{
			m_bAni = true;

			m_bWakeUpTwistCheck = true;

			//Twist
			m_pTwister->SetAni(true);
			_vec3 vPos = m_pTargetTransform->Get_Now_Position();
			m_pTwister->GetTransCom()->Set_Position(&vPos);

			//GroundDecal======================================================
			_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
			vPos.y += 0.2f;
			m_pGroundDecal->SetAni(true);
			m_pGroundDecal->GetTransCom()->Set_Position(&vPos);
			m_pGroundDecal->GetTransCom()->Set_Angle(&vAngle);
			m_pGroundDecal->GetTransCom()->Set_Scale(20.f, 20.f, 20.f);
			_vec4 vColor = _vec4(0.0f, 0.0f, 0.0f, 1.0f);
			m_pGroundDecal->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;


			vPos.y -= 0.2f;

			//Ground Wave
			for (int i = 0; i < 3; ++i)
			{
				m_pGroundWave[i]->SetAni(true);
				m_pGroundWave[i]->GetTransCom()->Set_Position(&vPos);
				m_pGroundWave[i]->GetTransCom()->Set_Angle(&vAngle);
				m_pGroundWave[i]->GetTransCom()->Set_Scale(i * 5.f + 5.f, i * 5.f + 5.f, i * 5.f + 5.f);

				_vec4 Color = _vec4(0.7f, 0.5f, 0.04f, 1.f);

				m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			}
			//================================================================
		}

	}
	else
	{
		m_bWakeUpTwistCheck = false;
	}

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::SUMMON)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();
		//4.0
		if ((4.f <= dTargetTime) && (!m_bCurtainCheck)) //바르디엘 빨간 커텐 생성
		{
			m_pSoundCom->MyPlaySound(L"Curtain", false, false);
			m_bCurtainCheck = true;
			RedCurtainStart();
		}

		//두손으로 얼굴을 감싸는 상태
		if ((1.11f <= dTargetTime) && (!m_bTwoHandFaceCheck))
		{
			m_pSoundCom->MyPlaySound(L"Ball");
			m_iSpriteCheck = 1;
			m_bTwoHandFaceCheck = true;
			HandParticleStart();
		}

		//한번리셋
		if ((3.60f <= dTargetTime) && (!m_bSpriteOff))
		{
			m_bSpriteOff = true;
			m_pLeftSprite->SetAni(false);
			m_pRightSprite->SetAni(false);

		}

		//양손에 생기는 구체형 파티클
		if ((4.1f <= dTargetTime) && (!m_bHandParticleCheck))
		{
			
			m_iSpriteCheck = 2;
			m_bHandParticleCheck = true;
			HandParticleStart();

			//파티클 생성
			m_pLeftParticle->SetRadius(60.f);
			m_pLightParticle->SetRadius(60.f);
			m_pLightParticle->SetAni(true);
			m_pLeftParticle->SetAni(true);
		}

		//위로 끌어올리는 동작
		if ((6.9f <= dTargetTime) && (!m_bWakeUpCheck))
		{
			m_bWakeUpCheck = true;
			m_iSpriteCheck = 3;
			HandParticleStart();

			m_pLightParticle->SetAni(false);
			m_pLeftParticle->SetAni(false);
		}

		//위로 끌어올리면서 스케일 줄인다.
		if ((10.1f <= dTargetTime) && (!m_bSpriteScaleMinus))
		{
			_vec3 vScale, vRightScale;
			m_pLeftSprite->GetTransCom()->Get_Scale(&vScale);

			if (vScale.x >= 0.0f)
			{
				m_pLeftSprite->GetTransCom()->Add_Scale(-0.05f);
				m_pRightSprite->GetTransCom()->Add_Scale(-0.05f);
			}
			else
			{
				m_bSpriteScaleMinus = true;
			}
		}

		//다시 팔뻗는 동작
		if ((11.4f <= dTargetTime) && (!m_bReturnHandSprite))
		{
			//Twist================================================
			m_pTwister->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
			m_pTwister->SetSpeed(15.f);
			//=====================================================

			m_bReturnHandSprite = true;

			m_iSpriteCheck = 2;
			HandParticleStart();

			m_pLeftSprite->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
			m_pRightSprite->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);

			m_pLeftParticle->SetRadius(70.f);
			m_pLightParticle->SetRadius(70.f);

			m_pLightParticle->SetAni(true);
			m_pLeftParticle->SetAni(true);
		}

		//다시 소멸 두팔을 크로스
		if ((15.0f <= dTargetTime) && (!m_bLastScaleMinus))
		{
			m_bCutainCheck = true;
			m_bWaveState = true;
			m_bDestroyTwist = true;
			m_bDecalCheck = true;

			_vec3 vScale, vRightScale;
			m_pLeftSprite->GetTransCom()->Get_Scale(&vScale);

			if (vScale.x >= 0.0f)
			{
				m_pLeftSprite->GetTransCom()->Add_Scale(-0.05f);
				m_pRightSprite->GetTransCom()->Add_Scale(-0.05f);
			}
			else
			{
				m_bLastScaleMinus = true;
				m_pLeftSprite->SetAni(false);
				m_pRightSprite->SetAni(false);
			}
			m_pLightParticle->SetAni(false);
			m_pLeftParticle->SetAni(false);
		}

		/*if (18.3f <= dTargetTime && 18.7f > dTargetTime)
		{
			m_fEffectPower -= 0.001f;
			if (m_fEffectPower < -0.2f)
				m_fEffectPower = -0.2f;

			Engine::SetRadialBlurPower(m_fEffectPower);
			Engine::SetRadialBlur(TRUE);
		}

		if (18.7f <= dTargetTime && 19.1f > dTargetTime)
		{
			m_fEffectPower += 0.001f;
			if (m_fEffectPower > 0.0f)
				m_fEffectPower = 0.0f;

			Engine::SetRadialBlurPower(m_fEffectPower);
		}

		if (19.4f <= dTargetTime && 19.5f > dTargetTime)
		{
			Engine::SetRadialBlur(FALSE);
			Engine::SetRadialBlurPower(-0.25f);
		}*/

		if (21.0f <= dTargetTime)
		{
			m_bAni = false;
		}
	}
	else
	{
		m_bDecalCheck = false;
		m_bDestroyTwist = false;
		m_bCutainCheck = false;
		m_bLastScaleMinus = false;
		m_bReturnHandSprite = false;
		m_bWakeUpCheck = false;
		m_bSpriteOff = false;
		m_iSpriteCheck = 0;
		m_bHandParticleCheck = false;
		m_bCurtainCheck = false;
		m_bTwoHandFaceCheck = false;
		m_bSpriteScaleMinus = false;
		m_bWaveState = false;
	}

	RedCurtainUpdate();

	//HandSprite Update
	HandParticleUpdate();

	TwistUpdate();


	WaveUpdate();


	DecalUpdate();


	m_pGroundDecal->Update_Object(fTimeDelta);
	m_pLightParticle->Update_Object(fTimeDelta);
	m_pLeftParticle->Update_Object(fTimeDelta);

	m_pLeftSprite->Update_Object(fTimeDelta);
	m_pRightSprite->Update_Object(fTimeDelta);

	m_pCurtain->Update_Object(fTimeDelta);


	m_pTwister->Update_Object(fTimeDelta);

	if (m_pTwister->GetAni())
	{
		m_pSoundCom->MyPlaySound(L"SpinCircle", true, false);
	}
	
	for (int i = 0; i < 3; ++i)
	{
		m_pGroundWave[i]->Update_Object(fTimeDelta);
	}
}

void CSummon::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	if (!m_bAni)
		return;

	m_pLightParticle->Render_Object();
	m_pLeftParticle->Render_Object();

	m_pLeftSprite->Render_Object();
	m_pRightSprite->Render_Object();

	m_pCurtain->Render_Object();

	m_pTwister->Render_Object();

	m_pGroundDecal->Render_Object();


	for (int i = 0; i < 3; ++i)
	{
		m_pGroundWave[i]->Render_Object();
	}
}

HRESULT CSummon::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//RedCurtain
	m_pCurtain = CCurtain::LoadCreate(m_pGraphicDev, L"BardielCurtain");
	m_pCurtain->SetAni(false);
	m_pCurtain->SetEffectBlurCheck(TRUE);
	m_pCurtain->SetBlurPass(2);
	m_pCurtain->SetBlurCnt(2);
	m_pCurtain->SetBlurPower(1.1f);
	m_pCurtain->SetBlurValue(128.0f);
	m_pCurtain->SetBlurType(Engine::BLUR_HORIZON);
	m_pCurtain->SetAni(FALSE);


	//Hand Sprite
	m_pLeftSprite = CSprite::LoadCreate(m_pGraphicDev, L"BardielBall");
	m_pLeftSprite->SetAni(false);
	m_pLeftSprite->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
	m_pLeftSprite->SetBlurCheck(false);

	m_pRightSprite = CSprite::LoadCreate(m_pGraphicDev, L"BardielBall");
	m_pRightSprite->SetAni(false);
	m_pRightSprite->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
	m_pRightSprite->SetBlurCheck(false);



	//Particle
	m_pLightParticle = CSummonParticle::Create(m_pGraphicDev, L"LightParticle", 300);
	m_pLightParticle->SetAni(false);
	m_pLightParticle->SetGoalResult(0.1f);


	m_pLeftParticle = CSummonParticle::Create(m_pGraphicDev, L"LeftParticle", 300);
	m_pLeftParticle->SetAni(false);
	m_pLeftParticle->SetGoalResult(0.1f);


	m_pTwister = CTwister::Create(m_pGraphicDev, L"Twister");
	m_pTwister->SetAni(false);

	//Wave=======================================================
	m_pGroundDecal = CWave::Create(m_pGraphicDev, L"GroundDecal");
	m_pGroundDecal->SetAni(false);
	m_pGroundDecal->SetPattern(CWave::PATTERN_END);
	m_pGroundDecal->SetRendererCheck(true);
	m_pGroundDecal->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	m_pGroundDecal->GetParticleInfo()->m_iPath = 2;
	m_pGroundDecal->GetParticleInfo()->m_iTextureNumber = 58;

	m_pGroundDecal->SetEffectBlurCheck(TRUE);
	m_pGroundDecal->SetBlurPass(2);
	m_pGroundDecal->SetBlurCnt(2);
	m_pGroundDecal->SetBlurPower(1.5f);
	m_pGroundDecal->SetBlurValue(128.0f);
	m_pGroundDecal->SetBlurType(Engine::BLUR_CROSS);
	//==========================================================


	for (int i = 0; i < 3; ++i)
	{
		m_pGroundWave[i] = CWave::Create(m_pGraphicDev, L"GroundWave");
		m_pGroundWave[i]->SetAni(false);
		m_pGroundWave[i]->SetPattern(CWave::PATTERN_END);
		m_pGroundWave[i]->SetRendererCheck(true);
		m_pGroundWave[i]->GetTransCom()->Set_Scale(i * 5.f + 5.f, i * 5.f + 5.f, i * 5.f + 5.f);
		m_pGroundWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pGroundWave[i]->GetParticleInfo()->m_iTextureNumber = 7;

		m_pGroundWave[i]->SetEffectBlurCheck(TRUE);
		m_pGroundWave[i]->SetBlurPass(2);
		m_pGroundWave[i]->SetBlurCnt(2);
		m_pGroundWave[i]->SetBlurPower(1.3f);
		m_pGroundWave[i]->SetBlurValue(80.0f);
		m_pGroundWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	return S_OK;
}

CSummon * CSummon::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CSummon*		pInstance = new CSummon(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Summon Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSummon::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pCurtain);

	Engine::Safe_Release(m_pLeftSprite);
	Engine::Safe_Release(m_pRightSprite);
	Engine::Safe_Release(m_pLightParticle);
	Engine::Safe_Release(m_pLeftParticle);
	Engine::Safe_Release(m_pTwister);
	Engine::Safe_Release(m_pGroundDecal);

	for (int i = 0; i < 3; ++i)
	{
		Engine::Safe_Release(m_pGroundWave[i]);
	}
}

HRESULT CSummon::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Sound
	pComponent = CSound::Create(m_pTransCom);
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Sound", pComponent));

	return S_OK;
}

void CSummon::SkillStart(void)
{
}

void CSummon::EndAniCheck(void)
{
}

void CSummon::RedCurtainStart(void)
{
	m_pCurtain->SetAni(true);

	_vec3 vBardielPos = m_pTargetTransform->Get_Now_Position();

	vBardielPos.y += 90.f;

	m_pCurtain->GetTransCom()->Set_Position(&vBardielPos);
	m_pCurtain->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);

	_vec4 vColor = (_vec4)m_pCurtain->GetParticleInfo()->m_dColor;

	vColor.w = 0.0f;

	m_pCurtain->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
}

//빨간 장막
void CSummon::RedCurtainUpdate(void)
{
	if (!m_pCurtain->GetAni())
		return;

	_vec4 vColor = (_vec4)m_pCurtain->GetParticleInfo()->m_dColor;


	if (!m_bCutainCheck) //빨간장막 생성
	{
		if (vColor.w <= 1.f)
		{
			vColor.w += 0.07f;

			m_pCurtain->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
	}
	else //빨간장막 해제
	{
		if (vColor.w >= 0.f)
		{
			vColor.w -= 0.07f;

			m_pCurtain->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
		else
		{
			m_bCutainCheck = false;
			m_pCurtain->SetAni(false);
			m_pSoundCom->MyStopSound(L"Curtain");
		}

	}
}

void CSummon::HandParticleStart(void)
{
	m_pRightSprite->SetAni(true);
	m_pLeftSprite->SetAni(true);
}

void CSummon::HandParticleUpdate(void)
{

	_matrix vBardielMat;


	m_pTargetTransform->Get_WorldMatrix(&vBardielMat);


	if (m_pRightSprite->GetAni() == true)
	{
		_vec3 vLocalPos;
		memcpy(&vLocalPos, &m_pRightHandBoneMat->m[3][0], sizeof(_vec3));

		if (m_iSpriteCheck == 1) //두손으로 머리를 움켜주는 동작
		{
			vLocalPos.z -= 1.f;
			vLocalPos.y += 3.f;
			vLocalPos.x -= 3.f;
		}
		else if (m_iSpriteCheck == 2) //팔을쭉 펴는 동작
		{
			vLocalPos.x += 7.f;
			vLocalPos.y -= 3.f;

			if (m_pLightParticle->GetAni())
			{
				_vec3 ParticleLocal = vLocalPos;
				D3DXVec3TransformCoord(&ParticleLocal, &ParticleLocal, &vBardielMat);

				m_pLightParticle->GetTransCom()->Set_Position(&ParticleLocal);
			}
		}
		else if (m_iSpriteCheck == 3) //팔을 위로 든다
		{
			vLocalPos.z += 5.f;
			vLocalPos.y += 1.f; //팔위치에서 Y축으로 높이 올라감 +
			vLocalPos.x += 5.f;
		}

		D3DXVec3TransformCoord(&vLocalPos, &vLocalPos, &vBardielMat);

		m_pRightSprite->GetTransCom()->Set_Position(&vLocalPos);
	}

	if (m_pLeftSprite->GetAni() == true)
	{
		_vec3 vLocalPos;

		memcpy(&vLocalPos, &m_pLeftHandBoneMat->m[3][0], sizeof(_vec3));

		if (m_iSpriteCheck == 1) //두손으로 머리를 움켜주는 동작
		{

			vLocalPos.z -= 1.f;
			vLocalPos.y += 3.f;
			vLocalPos.x += 3.f;

		}
		else if (m_iSpriteCheck == 2) //팔을쭉 펴는 동작
		{

			vLocalPos.x -= 7.f;
			vLocalPos.y -= 3.f;


			if (m_pLightParticle->GetAni())
			{
				_vec3 ParticleLocal = vLocalPos;
				D3DXVec3TransformCoord(&ParticleLocal, &ParticleLocal, &vBardielMat);

				m_pLeftParticle->GetTransCom()->Set_Position(&ParticleLocal);
			}

		}
		else if (m_iSpriteCheck == 3) //팔을 위로 든다
		{
			vLocalPos.z += 5.f;
			vLocalPos.y += 1.f;
			vLocalPos.x -= 5.f;
		}

		D3DXVec3TransformCoord(&vLocalPos, &vLocalPos, &vBardielMat);

		m_pLeftSprite->GetTransCom()->Set_Position(&vLocalPos);
	}
}

void CSummon::TwistUpdate(void)
{
	if ((!m_pTwister->GetAni()) || (!m_bDestroyTwist))
		return;

	m_pSoundCom->MyStopSound(L"SpinCircle");
	m_pTwister->AddScaleAlpha();
	
}

void CSummon::WaveUpdate(void)
{
	for (int i = 0; i < 3; ++i)
	{
		_vec3 vPos = m_pTargetTransform->Get_Now_Position();
		vPos.y += 1.3f;

		if (!m_pGroundWave[i]->GetAni())
			continue;

		_vec4 vColor = (_vec4)m_pGroundWave[i]->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			if (m_bWaveState)
			{
				m_pGroundWave[i]->SetAni(false);
			}
			else
			{
				m_pGroundWave[i]->GetTransCom()->Set_Position(&vPos);
				m_pGroundWave[i]->GetTransCom()->Set_Scale(i * 5.f + 5.f, i * 5.f + 5.f, i * 5.f + 5.f);
				_vec4 Color = _vec4(0.7f, 0.5f, 0.04f, 1.f);
				m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			}
		}
		else
		{
			vColor.w -= 0.02f;
			m_pGroundWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pGroundWave[i]->GetTransCom()->Add_Scale(0.4f);
		}
	}
}

void CSummon::DecalUpdate(void)
{
	if (!m_pGroundDecal->GetAni() || m_bDecalCheck == false)
		return;

	_vec4 vColor = (_vec4)m_pGroundDecal->GetParticleInfo()->m_dColor;

	if (vColor.w <= 0.f)
	{
		m_pGroundDecal->SetAni(false);
	}
	else
	{
		vColor.w -= 0.02f;
		m_pGroundDecal->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	}
}
