#include "stdafx.h"
#include "FlyDance.h"
#include "Export_Engine.h"
#include "Trail_Texture.h"
#include "WitchBlade.h"
#include "ReflectMesh.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"
#include "FlyDanceTrail.h"
#include "Sprite.h"
#include "Wave.h"


CFlyDance::CFlyDance(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, pTransform(NULL)
, m_pReflect(NULL)
, m_pTrail(NULL)
, m_bReflectStart(false)
, m_pFlash(NULL)
, pTargetSwordTransform(NULL)
{
	m_fYUp = 0.f;
	m_bIsStatic = true;
	m_bAni = false;

	ZeroMemory(&m_pDust, sizeof(CSprite*) * 2);
}

CFlyDance::~CFlyDance(void)
{
}

void CFlyDance::Update_Object(const _float & fTimeDelta)
{

	if (m_pPlayerAction[CWitchBlade::ACTION_FLYDANCE] == true)
	{
		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLFLING]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();


		if ( (dTargetTime >= m_fTrailStartTime ) ) //Trail이 꺼져있는상태
		{
			SkillStart(0); // Trail 설정
		}
		
		if((dTargetTime >= dTime) && (!m_bReflectStart))
		{
			SkillStart(1);
			m_bReflectStart = true;
		}
	}

	//Trail의 알파값 Update====
	if (m_pTrail->GetAni())
	{
		if (m_pTrail->GetGage() <= 0)
		{
			_vec4 vColor = m_pTrail->GetColor();

			vColor.w = max(vColor.w - 0.03f, 0.f);

			m_pTrail->SetColor(vColor);

			if (vColor.w <= 0)
			{
				m_pTrail->SetAni(false);
				m_bReflectStart = false;
				m_fYUp = 0.f;

				_vec4 vColor = _vec4(1.0f, 0.0f, 0.0f, 1.0f);//_vec4(0.9333f, 0.5098f, 0.9333f, 1.0f);

				m_pTrail->SetColor(vColor);
				m_pTrail->SetGage(1.0f);
			}
		}
	}

	if (!m_bAni)
		return;

	EndAniCheck();

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Update_Object(fTimeDelta);
	}

	m_pTrail->Update_Object(fTimeDelta);
	m_pReflect->Update_Object(fTimeDelta);
	m_pFlash->Update_Object(fTimeDelta);
}

void CFlyDance::Render_Object(void)
{
	if (!m_bAni)
		return;
		
	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Render_Object();
	}

	m_pTrail->Render_Object();
	m_pReflect->Render_Object();
	m_pFlash->Render_Object();
}

HRESULT CFlyDance::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//객체 생성
	//=====Reflect=======================================
	m_pReflect = CReflectMesh::Create(m_pGraphicDev, L"Reflect");
	m_pReflect->SetAni(false);
	m_pReflect->SetReflectIndex(0.02f);
	m_pReflect->SetPattern(CReflectMesh::PATTERN_THREE);
	m_pReflect->GetTransCom()->Set_Scale(0.05f, 0.1f, 0.05f);
	//====================================================

	//===Trail======================================================//
	m_pTrail = CFlyDanceTrail::Create(m_pGraphicDev, L"FlyDanceTrail");
	_vec3 vScale = _vec3(3.25f, 2.25f, 3.25f);
	m_pTrail->SetScale(vScale);
	m_pTrail->SetAni(false);
	m_pTrail->SetEffectBlurCheck(true);
	//===============================================================

	//===Dust============================================================
	_vec4 DustColor = _vec4(0.5f, 0.5f, 0.5f, 2.0f);

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"Dust");
		m_pDust[i]->SetAni(false);
		m_pDust[i]->SetRenderCheck(true);
		m_pDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)DustColor;
		m_pDust[i]->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);
		m_pDust[i]->GetParticleInfo()->m_iPath = 3;
		//m_pDust[i]->SetEffectBlurCheck(TRUE);
		//m_pDust[i]->SetBlurPass(3);
		//m_pDust[i]->SetBlurCnt(2);
		//m_pDust[i]->SetBlurPower(1.5f);
		//m_pDust[i]->SetBlurValue(128.0f);
		//m_pDust[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	//==Flash=======================================================
	m_pFlash = CWave::Create(m_pGraphicDev, L"Flash");
	m_pFlash->SetAni(false);
	m_pFlash->SetRendererCheck(false);
	/*m_pFlash->SetBlurCheck(true);
	m_pFlash->SetBlurCnt(4);
	m_pFlash->SetBlurPass(0);
	m_pFlash->SetBlurPower(1.1f);
	m_pFlash->SetBlurValue(128.0f);
	m_pFlash->SetBlurType(Engine::BLUR_CROSS);*/

	return S_OK;
}

CFlyDance * CFlyDance::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CFlyDance*		pInstance = new CFlyDance(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("FlyDance Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CFlyDance::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pReflect);
	Engine::Safe_Release(m_pTrail);
	Engine::Safe_Release(m_pFlash);

	for (int i = 0; i < 2; ++i)
		Engine::Safe_Release(m_pDust[i]);
}

HRESULT CFlyDance::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CFlyDance::SkillStart(_int iNumber)
{
	m_bAni = true;

	if (iNumber == 0)		// Trail
	{
		_vec3 Pos = pTransform->Get_Now_Position();
		_vec3 Look;

		pTransform->Get_Infomation(Engine::INFO_LOOK, &Look);

		D3DXVec3Normalize(&Look, &Look);

		_vec3 vLook = _vec3(-1.f, 0.0f, 0.0f);

		_float Dot = D3DXVec3Dot(&vLook, &Look);

		_float Radian = (_float)acos(Dot);

		Radian = D3DXToDegree(Radian);

		_vec3 vDir;

		D3DXVec3Cross(&vDir, &vLook, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));


		if (D3DXVec3Dot(&vDir, &Look) > 0) //왼쪽
		{
			Radian -= 5.f;
			Radian *= -1.f;
		}
		else //오른쪽
		{
			Radian += 5.f;
			Radian *= 1.f;
		}

		_vec3 Angle = _vec3(-30.f, Radian, 0.0f);

		m_pTrail->GetTransCom()->Set_Angle(&Angle);


		m_fYUp += 0.05f;

		Look  *= 2.0f;
		Pos   += Look;
		Pos.y += 7.0f + m_fYUp;


		m_pTrail->GetTransCom()->Set_Position(&Pos);
		m_pTrail->SetAni(true);

		m_pTrail->SetMinusGage(0.085f);
	}
	else if (iNumber == 1) 
	{
		//Reflect
		m_pReflect->GetTransCom()->Set_Position(&pTransform->Get_Now_Position());
		m_pReflect->SetAni(true);
		m_pReflect->GetTransCom()->Set_Scale(0.2f, 0.1f, 0.2f);

		//Dust
		for (int i = 0; i < 2; ++i)
		{
			_vec3 Pos;
			Pos = pTransform->Get_Now_Position();
			Pos.y += i * 0.5f + 0.3f;
			m_pDust[i]->SetAni(true);
			m_pDust[i]->GetTransCom()->Set_Position(&Pos);
		}

		//Flash
		D3DXCOLOR color = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
		m_pFlash->GetParticleInfo()->m_dColor = color;
		m_pFlash->GetParticleInfo()->m_iPath = 0;
		m_pFlash->GetParticleInfo()->m_iTextureNumber = 19;
		m_pFlash->SetDecrement(0.05f, 0.05f); //알파값 , 스케일값 감소량 설정
		m_pFlash->SetPattern(CWave::PATTERN_FOUR);
		m_pFlash->SetRotateResult(10.f);

		//왼손검
		_vec3 Local = _vec3(0.2f, -3.0f, -2.0f);
		D3DXVec3TransformCoord(&Local, &Local, pTargetSwordTransform->Get_WorldMatrixPointer());


		//_vec3 Pos = pTransform->Get_Now_Position();
		//Pos.y += 2.f;
		m_pFlash->GetTransCom()->Set_Position(&Local);

		m_pFlash->SetBilBoardCheck(true);
		m_pFlash->SetAni(true);
		_vec3 vScale = _vec3(1.5f, 1.5f, 1.5f);
		m_pFlash->SetScale(vScale);
	}

}

void CFlyDance::EndAniCheck(void)
{
	if (m_bAni == true && m_pReflect->GetAni() == false && m_pTrail->GetAni() == false && 
		m_pDust[0]->GetAni() == false && m_pDust[1]->GetAni() == false && m_pFlash->GetAni() == false)
		m_bAni = false;
}

HRESULT CFlyDance::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return E_FAIL;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);


	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();

	//Player TransCom
	Engine::CTransform* pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	pTransform = pTargetTransform;


	m_fTrailStartTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dEndTime * 0.4;


	m_fTrailEndTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dEndTime * 0.48;




	Engine::CGameObject* pTargetObject = Engine::Find(L"Sword1");
	if (pTargetObject == NULL)
		return E_FAIL;

	pTargetSwordTransform = (Engine::CTransform*)pTargetObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);
	if (pTargetSwordTransform == NULL)
		return E_FAIL;


	Engine::Safe_Release(pTargetTransform);
	Engine::Safe_Release(pGameObject);
	Engine::Safe_Release(pTargetObject);
	return S_OK;
}
