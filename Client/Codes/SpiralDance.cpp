#include "stdafx.h"
#include "SpiralDance.h"
#include "PlayerCamera.h"
#include "WitchBlade.h"
#include "Wave.h"
#include "CherryBlossom.h"
#include "SprialDance_In_Trail.h"
#include "SprialDance_Out_Trail.h"

CSprialDance::CSprialDance(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pmat_In_L_Sword(NULL)
	, m_pmat_In_R_Sword(NULL)
	, m_pTargetAnimator(NULL)
	, m_pIn_L_Trail(NULL)
	, m_pIn_R_Trail(NULL)
	, m_pOut_L_Trail(NULL)
	, m_pOut_R_Trail(NULL)
	, m_pGroundWave(NULL)
	, m_pCherryBlossom(NULL)
	, m_bTrailStart(FALSE)
	, m_bTrailTimming(FALSE)
	, m_bTrailOffTimming(FALSE)
	, m_bWaveTimming(FALSE)
{
	m_bIsStatic = TRUE;
	m_bAni = FALSE;
}

CSprialDance::~CSprialDance(void)
{
}

HRESULT CSprialDance::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return E_FAIL;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);
	m_pTargetAnimator = ((CWitchBlade*)pGameObject)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();

	//Player TransCom
	Engine::CTransform* pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);
	m_pTargetTransform = pTargetTransform;

	//LEFT Sword Object
	Engine::CGameObject* pTargetObject = Engine::Find(L"Sword1");
	Engine::CTransform* pSwordTransform = (Engine::CTransform*)pTargetObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//LEFT 검의 월드행렬
	m_pmat_In_L_Sword = pSwordTransform->Get_WorldMatrixPointer();

	m_pIn_L_Trail = CSprialDance_In_Trail::Create(m_pGraphicDev, L"LeftSword");
	m_pIn_L_Trail->SetAni(false);
	m_pIn_L_Trail->SetTargetMatrix(m_pmat_In_L_Sword);

	m_pOut_L_Trail = CSprialDance_Out_Trail::Create(m_pGraphicDev, L"Out_LeftSword");
	m_pOut_L_Trail->SetAni(false);
	m_pOut_L_Trail->SetTargetMatrix(m_pmat_In_L_Sword);

	//RIGHT Sword Object
	Engine::CGameObject* p_R_TargetObject = Engine::Find(L"SwordL1");
	Engine::CTransform*  p_R_SwordTransform = (Engine::CTransform*)p_R_TargetObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//RIGHT 검의 월드행렬
	m_pmat_In_R_Sword = p_R_SwordTransform->Get_WorldMatrixPointer();

	//Traill=============================================================================
	m_pIn_R_Trail = CSprialDance_In_Trail::Create(m_pGraphicDev, L"RightSword");
	m_pIn_R_Trail->SetAni(false);
	m_pIn_R_Trail->SetTargetMatrix(m_pmat_In_R_Sword);

	m_pOut_R_Trail = CSprialDance_Out_Trail::Create(m_pGraphicDev, L"Out_RightSword");
	m_pOut_R_Trail->SetAni(false);
	m_pOut_R_Trail->SetTargetMatrix(m_pmat_In_R_Sword);
	//====================================================================================

	Engine::Safe_Release(p_R_TargetObject);
	Engine::Safe_Release(pTargetObject);
	Engine::Safe_Release(pTargetTransform);
	Engine::Safe_Release(pGameObject);

	return S_OK;
}

void CSprialDance::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayerAction[CWitchBlade::ACTION_SPIRALDANCE])
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 0.63f && (!m_bTrailTimming))
		{
			m_bTrailTimming = TRUE;

			if (m_bTrailStart)
			{
				m_bTrailStart = FALSE;

				m_pIn_L_Trail->SettingTrail();
				m_pIn_R_Trail->SettingTrail();
				m_pOut_R_Trail->SettingTrail();
				m_pOut_L_Trail->SettingTrail();
			}

			m_pIn_L_Trail->SetAni(TRUE);
			m_pIn_R_Trail->SetAni(TRUE);
			m_pOut_R_Trail->SetAni(TRUE);
			m_pOut_L_Trail->SetAni(TRUE);
		}

		if (dTargetTime >= 0.83f && (!m_bWaveTimming))
		{
			m_bWaveTimming = TRUE;
			m_pGroundWave->SetAni(TRUE);
			_vec3 vPos = m_pTargetTransform->Get_Now_Position();
			vPos.y += 0.5f;
			m_pGroundWave->GetTransCom()->Set_Position(&vPos);
			m_pGroundWave->GetTransCom()->Set_Scale(0.8f, 0.8f, 0.8f);
			_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
			m_pGroundWave->GetTransCom()->Set_Angle(&vAngle);
			_vec4 Color = _vec4(1.0f, 1.f, 1.0f, 1.2f);
			m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			
		}

		if (dTargetTime >= 1.95f)
		{
			//m_bTrailOffTimming = TRUE;
			
			_vec4 vColor = m_pIn_L_Trail->GetColor();
			vColor.w -= 0.1f;
			m_pIn_L_Trail->SetColor(vColor);

			vColor = m_pIn_R_Trail->GetColor();
			vColor.w -= 0.1f;
			m_pIn_R_Trail->SetColor(vColor);

			vColor = m_pOut_L_Trail->GetColor();
			vColor.w -= 0.1f;
			m_pOut_L_Trail->SetColor(vColor);

			vColor = m_pOut_R_Trail->GetColor();
			vColor.w -= 0.1f;
			m_pOut_R_Trail->SetColor(vColor);

			if (m_pIn_L_Trail->GetColor().w <= 0.0f)
			{
				m_pIn_L_Trail->SetAni(FALSE);
				m_pIn_R_Trail->SetAni(FALSE);
				m_pOut_L_Trail->SetAni(FALSE);
				m_pOut_R_Trail->SetAni(FALSE);
			}
		}

		if (!m_bSkillStart)
		{
			m_bSkillStart = TRUE;
			SkillStart();
		}
	}
	else
	{
		m_bWaveTimming = FALSE;
		m_bTrailTimming = FALSE;
		m_bSkillStart = FALSE;
		m_bTrailStart = TRUE;

		m_pIn_L_Trail->SetAni(FALSE);
		m_pIn_R_Trail->SetAni(FALSE);
		m_pOut_L_Trail->SetAni(FALSE);
		m_pOut_R_Trail->SetAni(FALSE);
	}

	//if (!m_bAni)
	//	return;

	//EndAniCheck();

	GroundWaveUpdate();

	m_pIn_L_Trail->Update_Object(fTimeDelta);
	m_pIn_R_Trail->Update_Object(fTimeDelta);

	m_pOut_L_Trail->Update_Object(fTimeDelta);
	m_pOut_R_Trail->Update_Object(fTimeDelta);

	m_pGroundWave->Update_Object(fTimeDelta);
}

void CSprialDance::Render_Object(void)
{
	//if (!m_bAni)
	//	return;

	m_pIn_L_Trail->Render_Object();
	m_pIn_R_Trail->Render_Object();

	m_pOut_L_Trail->Render_Object();
	m_pOut_R_Trail->Render_Object();

	m_pGroundWave->Render_Object();
}

void CSprialDance::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
}

void CSprialDance::GroundWaveUpdate(void)
{
	if (m_pGroundWave->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pGroundWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pGroundWave->SetAni(false);
		}
		else
		{
			vColor.w -= 0.015f;
			m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

			m_pGroundWave->GetTransCom()->Rotation(Engine::ROT_Y, -30.f);
			m_pGroundWave->GetTransCom()->Add_Scale(0.0025f);

			_vec3 vPos = m_pTargetTransform->Get_Now_Position();
			vPos.y += 0.5f;
			m_pGroundWave->GetTransCom()->Set_Position(&vPos);
		}
	}
}

void CSprialDance::CherryBlossomUpdate(void)
{
	if (m_pCherryBlossom->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pCherryBlossom->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pCherryBlossom->SetAni(false);
		}
		else
		{
			vColor.w -= 0.03f;
			m_pCherryBlossom->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
	}
}

HRESULT CSprialDance::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pGroundWave = CWave::Create(m_pGraphicDev, L"GroundWave");
	m_pGroundWave->SetAni(false);
	m_pGroundWave->SetPattern(CWave::PATTERN_END);
	m_pGroundWave->SetRendererCheck(true);
	m_pGroundWave->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 1.2f);
	m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pGroundWave->GetParticleInfo()->m_iPath = 2;
	m_pGroundWave->GetParticleInfo()->m_iTextureNumber = 9;

	/*m_pCherryBlossom = CCherryBlossom::LoadCreate(m_pGraphicDev, L"SpiralDanceParticle");
	m_pCherryBlossom->SetAni(false);
	m_pCherryBlossom->SetBlurRenderCheck(true);
	_vec4 vCherryColor = _vec4(0.9333f, 0.5098f, 0.9333f, 1.f);
	m_pCherryBlossom->GetParticleInfo()->m_dColor = (D3DXCOLOR)vCherryColor;
	m_pCherryBlossom->SetEqual(true);
	m_pCherryBlossom->GetParticleInfo()->m_iPath = 2;
	m_pCherryBlossom->GetTransCom()->Set_Scale(0.25f, 0.25f, 0.25f);*/

	return S_OK;
}

CSprialDance * CSprialDance::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CSprialDance*		pInstance = new CSprialDance(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("SpinningHeart Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSprialDance::Free(void)
{
	::Safe_Release(m_pIn_L_Trail);
	::Safe_Release(m_pIn_R_Trail);
	::Safe_Release(m_pOut_L_Trail);
	::Safe_Release(m_pOut_R_Trail);
	::Safe_Release(m_pGroundWave);
	::Safe_Release(m_pCherryBlossom);
}

HRESULT CSprialDance::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CSprialDance::SkillStart(void)
{
	/*if (m_bAni)
		return;

	m_bAni = true;*/

	_vec3 vTargetPos;
	vTargetPos = m_pTargetTransform->Get_Now_Position();
	vTargetPos.y += 4.0f;
}

void CSprialDance::EndAniCheck(void)
{
	if (m_bAni == TRUE && 
		m_pIn_L_Trail->GetAni() == FALSE && m_pIn_R_Trail->GetAni() == FALSE &&
		m_pOut_L_Trail->GetAni() == FALSE && m_pOut_R_Trail->GetAni() == FALSE &&
		m_pGroundWave->GetAni() == FALSE)
		m_bAni = FALSE;
}
