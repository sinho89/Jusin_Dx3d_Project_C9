#include "stdafx.h"
#include "SpinTurnThrust.h"
#include "Export_Engine.h"
#include "WitchBlade.h"
#include "Wave.h"
#include "Sprite.h"
#include "SpinTurnThrustTrail.h"
#include "SpinTurnThrustDust.h"

CSpinTurnThrust::CSpinTurnThrust(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pmatTarget(NULL)
, pPlayerTransform(NULL)
, m_bStartSkillCheck(NULL)
, m_pBlackSpark(NULL)
, m_pTrail(NULL)
, m_bTrailStartCheck(false)
, m_fTrailEndTime(0)
, m_pDust(NULL)
, m_pSphere(NULL)
{
	m_bIsStatic = true;
	m_bAni = false;
}

CSpinTurnThrust::~CSpinTurnThrust(void)
{

}

void CSpinTurnThrust::Update_Object(const _float& fTimeDelta)
{

	if (m_pPlayerAction[CWitchBlade::ACTION_SPINTURNTHRUST] == true)
	{
		if (!m_bStartSkillCheck)
		{
			m_bAni = true;


			m_pSphere->SetAni(true);

			m_pBlackSpark->SetAni(true);

			m_pDust->SetAni(true);

			_vec3 Look;
			_vec3 vTargetPos;

			pPlayerTransform->Get_Infomation(Engine::INFO_LOOK, &Look);
			D3DXVec3Normalize(&Look, &Look);

			vTargetPos = pPlayerTransform->Get_Now_Position();

			Look *= -1.f;
			
			vTargetPos += Look;

			vTargetPos.y += 2.f;

			m_pDust->GetTransCom()->Set_Position(&pPlayerTransform->Get_Now_Position());

			m_bStartSkillCheck = true;
		}

		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();
		
		if (dTargetTime <= dTime) //보라색 섬광 Render
		{
			SkillStart();
		}
		else //트레일 발동
		{
			m_pSphere->SetAni(false);
			m_pBlackSpark->SetAni(false);

			//==보라색 섬광===============================================//
			//_vec3 vScale = _vec3(2.0f, 2.0f, 2.f);
			//m_pSphere->SetScale(vScale);

			//vScale = _vec3(1.0f, 1.0f, 1.f);
			//m_pBlackSpark->SetScale(vScale);
			
			if (!m_bTrailStartCheck)
			{
				m_pTrail->SetAni(true);
				m_pTrail->SettingTrail();
				m_bTrailStartCheck = true;
			}

			if (m_fTrailEndTime <= dTargetTime)
			{
				m_pTrail->SetAni(false);
			}
		}

		if (dTargetTime >= 0.833f && dTargetTime < 0.9916667f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 0.9916667f && dTargetTime < 1.2f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bStartSkillCheck = false;
		m_bTrailStartCheck = false;
		m_pTrail->SetAni(false);
	}



	EndAniCheck();

	if(m_bAni == false)
		return ;


	m_pSphere->Update_Object(fTimeDelta);

	m_pBlackSpark->Update_Object(fTimeDelta);

	m_pTrail->Update_Object(fTimeDelta);
	m_pDust->Update_Object(fTimeDelta);
}

void CSpinTurnThrust::Render_Object(void)
{
	if(m_bAni == false)
		return ;


	m_pSphere->Render_Object();

	m_pBlackSpark->Render_Object();

	m_pTrail->Render_Object();
	m_pDust->Render_Object();
}

HRESULT CSpinTurnThrust::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vScale = _vec3(1.5f, 1.5f, 1.5f);
	D3DXCOLOR color = _vec4(0.0f, 0.0f, 0.0f, 1.25f);

	//==보라색 섬광===============================================//
	m_pSphere = CWave::Create(m_pGraphicDev, L"Sphere");
	m_pSphere->SetAni(false);
	m_pSphere->SetBilBoardCheck(true);
	m_pSphere->SetRendererCheck(true);
	m_pSphere->SetBlurPass(8);
	m_pSphere->GetParticleInfo()->m_dColor = color;
	m_pSphere->GetParticleInfo()->m_iPath = 3;
	m_pSphere->GetParticleInfo()->m_iTextureNumber = 24;
	m_pSphere->SetPattern(CWave::PATTERN_END);
	m_pSphere->SetScale(vScale);
	//============================================================//


	//=검은 스파크===============================================//
	m_pBlackSpark = CSprite::LoadCreate(m_pGraphicDev, L"SpinTurnSpark");
	m_pBlackSpark->SetAni(false);
	m_pBlackSpark->SetBlurCheck(true);
	vScale = _vec3(0.5f, 0.5f, 0.5f);
	D3DXCOLOR vcolor = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pBlackSpark->GetParticleInfo()->m_dColor = vcolor;
	m_pBlackSpark->GetParticleInfo()->m_iPath = 2;
	m_pBlackSpark->SetRenderCheck(true);
	m_pBlackSpark->SetScale(vScale);
	//==========================================================//

	_vec4 vDustcolor = _vec4(1.0f, 1.0f, 1.0f, 1.5f);
	m_pDust = CSpinTurnThrustDust::LoadCreate(m_pGraphicDev, L"SpinTurnDust");
	m_pDust->SetAni(false);
	_vec3 vSDustcale = _vec3(1.3f, 1.3f, 1.3f);
	m_pDust->SetScale(vSDustcale);
	m_pDust->GetParticleInfo()->m_dColor = (D3DXCOLOR)vDustcolor;
	m_pDust->SetBlurPass(0);
	m_pDust->SetBlurCnt(2);
	m_pDust->SetBlurPower(2.0f);
	m_pDust->SetBlurValue(110.0f);
	m_pDust->SetBlurType(Engine::BLUR_CROSS);

	return S_OK;
}

CSpinTurnThrust* CSpinTurnThrust::Create(LPDIRECT3DDEVICE9 pGraphicDev , const wstring& wstrName)
{
	CSpinTurnThrust*   pInstance = new CSpinTurnThrust(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("SpinTurnThrust Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpinTurnThrust::Free(void)
{
	CEffect::Free();


	Engine::Safe_Release(m_pSphere);

	Engine::Safe_Release(m_pBlackSpark);
	Engine::Safe_Release(m_pTrail);
	Engine::Safe_Release(m_pDust);
}

HRESULT CSpinTurnThrust::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CSpinTurnThrust::SkillStart(void)
{
	//==보라색 섬광========================================================//
	_vec3 vStart;
	vStart = _vec3(0.0f, -2.5f, -2.2f);
	D3DXVec3TransformCoord(&vStart, &vStart, m_pmatTarget);


	m_pSphere->GetTransCom()->Set_Position(&vStart);
	//===================================================================

	m_pBlackSpark->GetTransCom()->Set_Position(&vStart);

	/*_vec3 vScale;
	m_pSphere->GetTransCom()->Get_Scale(&vScale);

	if (vScale.x <= 0.8f)
	{
		m_pSphere->GetTransCom()->Add_Scale(0.04f);

		m_pBlackSpark->GetTransCom()->Add_Scale(0.05f);
	}*/
	//============================================================================
}

void CSpinTurnThrust::EndAniCheck(void)
{
	if(m_bAni == true && m_pSphere->GetAni() == false &&
		m_pBlackSpark->GetAni() == false && 
		m_pTrail->GetAni() == false && m_pDust->GetAni() == false)
		m_bAni = false;
}

HRESULT CSpinTurnThrust::Start_ForScene(void)
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

	//Player TransForm
	pPlayerTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//Target Object
	Engine::CGameObject* pTargetObject = Engine::Find(L"SwordL1");

	Engine::CTransform* pTargetTransform = (Engine::CTransform*)pTargetObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);


	//검의 월드행렬
	m_pmatTarget = pTargetTransform->Get_WorldMatrixPointer();


	m_fTrailEndTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.8;

	//Traill=============================================================================
	m_pTrail = CSpinTurnThrustTrail::Create(m_pGraphicDev, L"SpinTurnThrustTrail");
	m_pTrail->SetAni(false);
	m_pTrail->SetTargetMatrix(m_pmatTarget);
	//====================================================================================

	Engine::Safe_Release(pTargetObject);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}
