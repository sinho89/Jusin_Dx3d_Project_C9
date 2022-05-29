#include "stdafx.h"
#include "HillTurn.h"
#include "Export_Engine.h"
#include "WitchBlade.h"
#include "Wave.h"
#include "CherryBlossom.h"


CHillTurn::CHillTurn(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: CEffect(pGraphicDev, wstrName)
,m_pvecAniInfo(NULL)
,m_pTargetMesh(NULL)
,m_pOutLine(NULL)
,m_pTargetTransform(NULL)
,m_pCherryBlossom(NULL)
,m_bSkillStart(false)
,m_pInLine(false)
{
	m_bIsStatic = true;
	m_bAni = false;
}

CHillTurn::~CHillTurn(void)
{
}

void CHillTurn::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayerAction[CWitchBlade::ACTION_HILLTURN] == true)
	{
		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLJUMPKICK]->pMotionEventInfo[EVENT_EFFECT].dStartTime;

		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if ( (dTargetTime >= dTime) && (!m_bSkillStart))
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

	OutLineUpdate();

	InLineUpdate();

	CherryBlossomUpdate();

	m_pOutLine->Update_Object(fTimeDelta);
	m_pInLine->Update_Object(fTimeDelta);
	m_pCherryBlossom->Update_Object(fTimeDelta);
}

void CHillTurn::Render_Object(void)
{
	if (!m_bAni)
		return;

	m_pOutLine->Render_Object();
	m_pInLine->Render_Object();
	m_pCherryBlossom->Render_Object();

}

HRESULT CHillTurn::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	//OutLine============================================//
	m_pOutLine = CWave::Create(m_pGraphicDev, L"OutLine");
	m_pOutLine->SetAni(false);
	m_pOutLine->SetBilBoardCheck(true);
	D3DXCOLOR Color = _vec4(1.f, 1.f, 1.f, 0.8f);
	m_pOutLine->GetParticleInfo()->m_dColor = Color;
	m_pOutLine->GetParticleInfo()->m_iPath = 2;
	m_pOutLine->GetParticleInfo()->m_iTextureNumber = 25;

	_vec3 vScale = _vec3(1.f, 1.f, 1.f); //크기조절
	m_pOutLine->SetOriginScale(vScale);
	m_pOutLine->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	m_pOutLine->SetRendererCheck(true);
	m_pOutLine->SetPattern(CWave::PATTERN_END);
	m_pOutLine->SetBlurCheck(true);
	m_pOutLine->SetBlurPass(2);
	m_pOutLine->SetBlurCnt(4);
	m_pOutLine->SetBlurPower(1.15f);
	m_pOutLine->SetBlurType(Engine::BLUR_STAR);
	m_pOutLine->SetBlurValue(128.0f);
	//===================================================//


	//InLine=============================================//
	m_pInLine = CWave::Create(m_pGraphicDev, L"InLine");
	m_pInLine->SetAni(false);
	m_pInLine->SetBilBoardCheck(true);
	D3DXCOLOR vColor = _vec4(1.f, 1.f, 1.f, 0.8f);
	m_pInLine->GetParticleInfo()->m_dColor = vColor;
	m_pInLine->GetParticleInfo()->m_iPath = 2;
	m_pInLine->GetParticleInfo()->m_iTextureNumber = 25;

	vScale = _vec3(1.f, 1.f, 1.f); //크기조절
	m_pInLine->SetOriginScale(vScale);
	m_pInLine->GetTransCom()->Set_Scale(0.6f, 0.6f, 0.6f);
	m_pInLine->SetRendererCheck(true);
	m_pInLine->SetPattern(CWave::PATTERN_END);
	m_pInLine->SetBlurCheck(true);
	m_pInLine->SetBlurPass(2);
	m_pInLine->SetBlurCnt(4);
	m_pInLine->SetBlurPower(1.15f);
	m_pInLine->SetBlurType(Engine::BLUR_STAR);
	m_pInLine->SetBlurValue(128.0f);
	//===================================================//


	//CherryBlossom======================================//

	m_pCherryBlossom = CCherryBlossom::LoadCreate(m_pGraphicDev, L"SpinningCherry");
	m_pCherryBlossom->SetAni(false);
	m_pCherryBlossom->SetBlurRenderCheck(true);
	_vec4 vCherryColor = _vec4(0.5f, 0.f, 0.f, 1.f);
	m_pCherryBlossom->GetParticleInfo()->m_dColor = (D3DXCOLOR)vCherryColor;
	m_pCherryBlossom->SetEqual(true);
	m_pCherryBlossom->GetParticleInfo()->m_iPath = 2;
	m_pCherryBlossom->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);

	//==================================================//


	return S_OK;
}

CHillTurn * CHillTurn::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHillTurn*		pInstance = new CHillTurn(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("HillTurn Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHillTurn::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pOutLine);
	Engine::Safe_Release(m_pInLine);
	Engine::Safe_Release(m_pCherryBlossom);
}

HRESULT CHillTurn::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CHillTurn::SkillStart(void)
{
	if (m_bAni)
		return;

	m_bAni = true;

	_vec3 vTargetPos;
	vTargetPos = m_pTargetTransform->Get_Now_Position();

	//HillTurn===============================================
	vTargetPos.y += 4.0f;

	m_vTargetPos = vTargetPos;

	m_pOutLine->SetAni(true);
	m_pOutLine->GetTransCom()->Set_Position(&vTargetPos);
	m_pOutLine->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);

	D3DXCOLOR Color = _vec4(1.f, 1.f, 1.f, 0.8f);
	m_pOutLine->GetParticleInfo()->m_dColor = Color;
	//=======================================================

	//pInLine->SkillStart==============================
	m_pInLine->SetAni(true);
	m_pInLine->GetTransCom()->Set_Position(&m_vTargetPos);
	m_pInLine->GetTransCom()->Set_Scale(0.6f, 0.6f, 0.6f);

	D3DXCOLOR inColor = _vec4(1.f, 1.f, 1.f, 0.8f);
	m_pInLine->GetParticleInfo()->m_dColor = inColor;
	//===============================================

	//CherryBlossom==========================================
	m_pCherryBlossom->SetAni(true);
	m_pCherryBlossom->GetTransCom()->Set_Position(&m_vTargetPos);
	_vec4 vCherryColor = _vec4(0.5f, 0.f, 0.f, 1.f);
	m_pCherryBlossom->GetParticleInfo()->m_dColor = (D3DXCOLOR)vCherryColor;
	//=======================================================
}

void CHillTurn::EndAniCheck(void)
{
	if (m_bAni == true && m_pOutLine->GetAni() == false && m_pInLine->GetAni() == false && m_pCherryBlossom->GetAni() == false)
		m_bAni = false;
}

HRESULT CHillTurn::Start_ForScene(void)
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


	//Player TransCom
	Engine::CTransform* pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pTargetTransform = pTargetTransform;


	Engine::Safe_Release(pTargetTransform);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CHillTurn::OutLineUpdate(void)
{
	if (m_pOutLine->GetAni() == false)
		return;

		_vec4 vColor =  (_vec4)m_pOutLine->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.0f)
		{
			m_pOutLine->SetAni(false);
		}
		else
		{
			//===========================================================
			m_pOutLine->GetTransCom()->Add_Scale(0.06f);
			vColor.w -= 0.04f;
			m_pOutLine->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			//===========================================================
		}
}

void CHillTurn::InLineUpdate(void)
{
	if (m_pInLine->GetAni() == false)
		return;

	_vec4 vColor = (_vec4)m_pInLine->GetParticleInfo()->m_dColor;


	if (vColor.w <= 0.0f)
	{
		m_pInLine->SetAni(false);
	}
	else
	{
		m_pInLine->GetTransCom()->Add_Scale(0.15f);
		vColor.w -= 0.04f;
		m_pInLine->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	}

}

void CHillTurn::CherryBlossomUpdate(void)
{
	if(m_pCherryBlossom->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pCherryBlossom->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pCherryBlossom->SetAni(false);
		}
		else
		{
			vColor.w -= 0.005f;
			m_pCherryBlossom->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
	}
}
