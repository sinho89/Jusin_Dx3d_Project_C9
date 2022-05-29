#include "stdafx.h"
#include "ViperKneeKick.h"
#include "Export_Engine.h"
#include "WitchBlade.h"
#include "Sprite.h"
#include "Wave.h"
#include "Reflect.h"

CViperKneekick::CViperKneekick(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_bSkillStart(false)
, m_pReflect(NULL)
, m_pRainBow(NULL)
{
	m_bIsStatic = true;
	m_bAni = false;

	ZeroMemory(&m_pLightWave, sizeof(CWave*) * 2);
}

CViperKneekick::~CViperKneekick(void)
{
}

void CViperKneekick::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayerAction[CWitchBlade::ACTION_VIPERKNEEKICK] == true) //바이퍼 니킥 동작일때
	{
		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->pMotionEventInfo[EVENT_EFFECT].dStartTime; //지정시간위치
		_double dTargetTime = m_pTargetMesh->Get_TrackPos(); //Target의 애니메이션 시간

		if ((dTargetTime >= dTime) && (!m_bSkillStart)) //발동
		{
			m_bSkillStart = true;
			SkillStart();
		}
	}
	else
	{
		m_bSkillStart = false;
	}


	if (!m_bAni)
		return;

	EndAniCheck();

	ReflectUpdateCheck();//Reflect Update Check
	RainBowUpdate();


	m_pReflect->Update_Object(fTimeDelta);

	for (int i = 0; i < 2; ++i)
	{
		m_pLightWave[i]->Update_Object(fTimeDelta);
	}

	m_pDust->Update_Object(fTimeDelta);
	m_pRainBow->Update_Object(fTimeDelta);
}

void CViperKneekick::Render_Object(void)
{
	if (!m_pDust)
		return;

	m_pReflect->Render_Object();

	for (int i = 0; i < 2; ++i)
	{
		m_pLightWave[i]->Render_Object();
	}
	m_pDust->Render_Object();
	m_pRainBow->Render_Object();
}

HRESULT CViperKneekick::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//Dust===========================================================//
	m_pDust = CSprite::LoadCreate(m_pGraphicDev, L"ViperKneeKickDust");
	m_pDust->SetAni(false);
	m_pDust->GetTransCom()->Set_Scale(0.3f, 0.3f, 0.3f);
	D3DXCOLOR color = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 0.6f);
	m_pDust->GetParticleInfo()->m_dColor = color;
	m_pDust->GetParticleInfo()->m_iPath = 3;
	//===============================================================//

	//LightWave======================================================//
	for (int i = 0; i < 2; ++i)
	{
		m_pLightWave[i] = CWave::Create(m_pGraphicDev, L"LightWave");
		m_pLightWave[i]->SetAni(false);
		m_pLightWave[i]->SetBilBoardCheck(true);
		D3DXCOLOR WaveColor = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pLightWave[i]->GetParticleInfo()->m_dColor = WaveColor;
		m_pLightWave[i]->GetParticleInfo()->m_iPath = 2;

		if (i == 0)
		{
			m_pLightWave[i]->GetParticleInfo()->m_iTextureNumber = 25;

			_vec3 vScale = _vec3(0.5f, 0.5f, 0.5f); //크기조절
			m_pLightWave[i]->SetOriginScale(vScale);
			m_pLightWave[i]->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
		}
		else
		{
			m_pLightWave[i]->GetParticleInfo()->m_iTextureNumber = 27;

			_vec3 vScale = _vec3(1.f, 1.f, 1.f); //크기조절
			m_pLightWave[i]->SetOriginScale(vScale);
			m_pLightWave[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
		}

		m_pLightWave[i]->SetDecrement(0.05f, 0.05f); //알파값 , 스케일값 감소량 설정
		m_pLightWave[i]->SetPattern(CWave::PATTERN_NINE);
		m_pLightWave[i]->SetRendererCheck(true);
		m_pLightWave[i]->SetBlurCheck(true);
		m_pLightWave[i]->SetBlurPass(2);
		m_pLightWave[i]->SetBlurCnt(4);
		m_pLightWave[i]->SetBlurPower(1.1f);
		m_pLightWave[i]->SetBlurType(Engine::BLUR_STAR);
		m_pLightWave[i]->SetBlurValue(150.0f);
	}
	//===============================================================//
	//RainBow=========================================================
	m_pRainBow = CWave::Create(m_pGraphicDev, L"RainBowWave");
	m_pRainBow->SetAni(false);
	m_pRainBow->SetBilBoardCheck(true);
	D3DXCOLOR WaveColor = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pRainBow->GetParticleInfo()->m_dColor = WaveColor;
	m_pRainBow->GetParticleInfo()->m_iPath = 0;
	m_pRainBow->GetParticleInfo()->m_iTextureNumber = 67;
	m_pRainBow->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	//================================================================

	//Reflect========================================================//
	m_pReflect = CReflect::Create(m_pGraphicDev, L"ViperReflect");
	m_pReflect->SetAni(false);
	m_pReflect->SetTextureNumber(26);
	m_pReflect->SetNormalTextureNumber(0);
	m_pReflect->SetReflectIndex(0.02f);
	m_pReflect->SetShaderPath(7);
	m_pReflect->GetTransCom()->Set_Scale(0.8f, 0.8f, 0.8f);
	m_pReflect->GetParticleInfo()->m_bBillBoard = true;
	D3DXCOLOR vColor = _vec4(0.0, 0.0f, 0.0f, 1.0f);
	m_pReflect->GetParticleInfo()->m_dColor = vColor;
	//================================================================//

	return S_OK;
}

void CViperKneekick::ReflectUpdateCheck(void)
{
	//Reflect Update
	if (m_pReflect->GetAni() == true)
	{
		if (m_pReflect->GetReflectionIndex() <= 0.0f)
		{
			m_pReflect->SetAni(false);
		}
		else
		{
			m_pReflect->GetTransCom()->Add_Scale(0.3f);
			m_pReflect->MinusReflectionIndex(0.0003f);
		}
	}
}

CViperKneekick * CViperKneekick::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CViperKneekick*		pInstance = new CViperKneekick(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("ViperKneeKick Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CViperKneekick::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pDust);

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pLightWave[i]);
	}

	Engine::Safe_Release(m_pReflect);
	Engine::Safe_Release(m_pRainBow);
}

HRESULT CViperKneekick::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CViperKneekick::SkillStart(void)
{
	if(m_bAni)
	   return;

	m_bAni = true;

	//Dust============================================//
	m_pDust->SetAni(true);
	_vec3 vTargetPos;
	vTargetPos = m_pTargetTransform->Get_Now_Position();
	vTargetPos.y += 1.0f;
	m_pDust->GetTransCom()->Set_Position(&vTargetPos);
	//===============================================//


	vTargetPos.y += 4.0f;
	//LightWave========================================
	for (int i = 0; i < 2; ++i)
	{
		m_pLightWave[i]->SetAni(true);
		m_pLightWave[i]->GetTransCom()->Set_Position(&vTargetPos);

		D3DXCOLOR WaveColor = _vec4(1.0f, 1.0f, 1.0f, 1.f);
		m_pLightWave[i]->GetParticleInfo()->m_dColor = WaveColor;

		if(i == 0)
			m_pLightWave[i]->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
		else
			m_pLightWave[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	}
	//=================================================

	m_pReflect->SetAni(true);
	m_pReflect->GetTransCom()->Set_Position(&vTargetPos);
	m_pReflect->GetTransCom()->Set_Scale(0.8f, 0.8f, 0.8f);
	m_pReflect->SetReflectIndex(0.02f);

	//============================================================

	m_pRainBow->SetAni(true);
	m_pRainBow->GetTransCom()->Set_Position(&vTargetPos);
	m_pRainBow->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	D3DXCOLOR WaveColor = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pRainBow->GetParticleInfo()->m_dColor = WaveColor;
}

void CViperKneekick::EndAniCheck(void)
{
	if (m_bAni == true && m_pDust->GetAni() == false && m_pLightWave[0]->GetAni() == false 
		&& m_pLightWave[1]->GetAni() == false && m_pReflect->GetAni() == false && m_pRainBow->GetAni() == false)
		m_bAni = false;
}

HRESULT CViperKneekick::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return S_OK;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();


	//Target TransCom
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);



	Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CViperKneekick::RainBowUpdate(void)
{
	if (!m_pRainBow->GetAni())
		return;

	_vec4 vColor = (_vec4)m_pRainBow->GetParticleInfo()->m_dColor;

	if (vColor.w <= 0.f)
	{
		m_pRainBow->SetAni(false);
	}
	else
	{
		vColor.w -= 0.06f;
		m_pRainBow->GetTransCom()->Add_Scale(0.2f);
		m_pRainBow->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	}

}
