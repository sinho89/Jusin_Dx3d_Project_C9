#include "stdafx.h"
#include "HanumaAttack01.h"
#include "Hanuman.h"
#include "Wave.h"
#include "HanumaAttack01_Trail.h"

CHanumaAttack01::CHanumaAttack01(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pLeft_0_Trail(NULL)
	, m_pLeft_1_Trail(NULL)
	, m_pLeft_2_Trail(NULL)
	, m_pRight_0_Trail(NULL)
	, m_pRight_1_Trail(NULL)
	, m_pRight_2_Trail(NULL)
	, m_pHanuman(NULL)
	, m_bLeftSwing(FALSE)
	, m_bLeftDeleteSwing(FALSE)
	, m_bRightSwing(FALSE)
	, m_bRightDeleteSwing(FALSE)
{
	m_bAni = false;
}

CHanumaAttack01::~CHanumaAttack01(void)
{
}

void CHanumaAttack01::Update_Object(const _float & fTimeDelta)
{
	if (g_bHanumanDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ATTACK1)
	{
		m_bAni = true;

		//_double dTime = (*m_pvecAniInfo)[CHanuman::HANUMAN::ATTACK1]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();
		
		if (dTargetTime >= 0.692f && (!m_bLeftSwing))
		{
			m_bLeftSwing = TRUE;

			m_pLeft_0_Trail->SetAni(TRUE);
			m_pLeft_0_Trail->SettingTrail();

			m_pLeft_1_Trail->SetAni(TRUE);
			m_pLeft_1_Trail->SettingTrail();

			m_pLeft_2_Trail->SetAni(TRUE);
			m_pLeft_2_Trail->SettingTrail();
		}

		if (dTargetTime >= 1.3f && (!m_bLeftDeleteSwing))
		{
			_vec4 vColor = m_pLeft_0_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pLeft_0_Trail->SetColor(vColor);

			vColor = m_pLeft_0_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pLeft_1_Trail->SetColor(vColor);

			vColor = m_pLeft_0_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pLeft_2_Trail->SetColor(vColor);

			if (m_pLeft_0_Trail->GetColor().w <= 0.0f)
			{
				m_pLeft_0_Trail->SetAni(FALSE);
				m_pLeft_1_Trail->SetAni(FALSE);
				m_pLeft_2_Trail->SetAni(FALSE);

				m_bLeftDeleteSwing = TRUE;
			}
		}

		if (dTargetTime >= 3.11f && (!m_bRightSwing))
		{
			m_bRightSwing = TRUE;

			m_pRight_0_Trail->SetAni(TRUE);
			m_pRight_0_Trail->SettingTrail();

			m_pRight_1_Trail->SetAni(TRUE);
			m_pRight_1_Trail->SettingTrail();

			m_pRight_2_Trail->SetAni(TRUE);
			m_pRight_2_Trail->SettingTrail();
		}

		if (dTargetTime >= 3.72f && (!m_bRightDeleteSwing))
		{
			_vec4 vColor = m_pRight_0_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pRight_0_Trail->SetColor(vColor);

			vColor = m_pRight_1_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pRight_1_Trail->SetColor(vColor);

			vColor = m_pRight_2_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pRight_2_Trail->SetColor(vColor);

			if (m_pRight_0_Trail->GetColor().w <= 0.0f)
			{
				m_pRight_0_Trail->SetAni(FALSE);
				m_pRight_1_Trail->SetAni(FALSE);
				m_pRight_2_Trail->SetAni(FALSE);

				m_bRightDeleteSwing = TRUE;				
			}
		}
	}
	else
	{
		m_bLeftSwing = FALSE;
		m_bLeftDeleteSwing = FALSE;
		m_bRightSwing = FALSE;
		m_bRightDeleteSwing = FALSE;
		m_bAni = FALSE;
	}

	if (!m_bAni)
		return;

	//EndAniCheck();

	m_pLeft_0_Trail->Update_Object(fTimeDelta);
	m_pLeft_1_Trail->Update_Object(fTimeDelta);
	m_pLeft_2_Trail->Update_Object(fTimeDelta);

	m_pRight_0_Trail->Update_Object(fTimeDelta);
	m_pRight_1_Trail->Update_Object(fTimeDelta);
	m_pRight_2_Trail->Update_Object(fTimeDelta);
}

void CHanumaAttack01::Render_Object(void)
{
	if (g_bHanumanDead)
		return;

	if (!m_bAni)
		return;

	m_pLeft_0_Trail->Render_Object();
	m_pLeft_1_Trail->Render_Object();
	m_pLeft_2_Trail->Render_Object();

	m_pRight_0_Trail->Render_Object();
	m_pRight_1_Trail->Render_Object();
	m_pRight_2_Trail->Render_Object();
}

HRESULT CHanumaAttack01::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

CHanumaAttack01 * CHanumaAttack01::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHanumaAttack01*		pInstance = new CHanumaAttack01(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("HanumaAttack01 Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHanumaAttack01::Free(void)
{
	::Safe_Release(m_pLeft_0_Trail);
	::Safe_Release(m_pLeft_1_Trail);
	::Safe_Release(m_pLeft_2_Trail);

	::Safe_Release(m_pRight_0_Trail);
	::Safe_Release(m_pRight_1_Trail);
	::Safe_Release(m_pRight_2_Trail);

	::Safe_Release(m_pHanuman);

	CEffect::Free();
}

HRESULT CHanumaAttack01::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CHanumaAttack01::SkillStart(void)
{
}

void CHanumaAttack01::EndAniCheck(void)
{
	if (m_bAni == true)
		m_bAni = false;
}

HRESULT CHanumaAttack01::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	m_pHanuman = Engine::Find(L"Mesh_Hanuman");
	if (m_pHanuman == NULL)
		return E_FAIL;
	
	m_pTargetAnimator = ((CHanuman*)m_pHanuman)->GetAnimatorInfo();
	
	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)m_pHanuman->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//Hanuman TransCom
	m_pTargetTransform = (Engine::CTransform*)m_pHanuman->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	const _matrix* pMatWorld = ((CHanuman*)m_pHanuman)->GetTransCom()->Get_WorldMatrixPointer();

	//Left--------------------------------------------------------------------------------
	const _matrix* pStartMat	=	m_pTargetMesh->Get_FrameByName("_39");
	const _matrix* pEndpMat		=	m_pTargetMesh->Get_FrameByName("_40");

	m_pLeft_0_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Att01_0_Left_Trail");
	m_pLeft_0_Trail->SetAni(false);
	m_pLeft_0_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pLeft_0_Trail->SetTargetWorldMat(pMatWorld);

	pStartMat = m_pTargetMesh->Get_FrameByName("_41");
	pEndpMat = m_pTargetMesh->Get_FrameByName("_42");

	m_pLeft_1_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Att01_1_Left_Trail");
	m_pLeft_1_Trail->SetAni(false);
	m_pLeft_1_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pLeft_1_Trail->SetTargetWorldMat(pMatWorld);

	pStartMat = m_pTargetMesh->Get_FrameByName("_43");
	pEndpMat = m_pTargetMesh->Get_FrameByName("_44");

	m_pLeft_2_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Att01_2_Left_Trail");
	m_pLeft_2_Trail->SetAni(false);
	m_pLeft_2_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pLeft_2_Trail->SetTargetWorldMat(pMatWorld);
	//---------------------------------------------------------------------------------------

	//Right----------------------------------------------------------------------------------
	pStartMat = m_pTargetMesh->Get_FrameByName("_21");
	pEndpMat = m_pTargetMesh->Get_FrameByName("_22");

	m_pRight_0_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Att01_0_Right_Trail");
	m_pRight_0_Trail->SetAni(false);
	m_pRight_0_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pRight_0_Trail->SetTargetWorldMat(pMatWorld);

	pStartMat = m_pTargetMesh->Get_FrameByName("_23");
	pEndpMat = m_pTargetMesh->Get_FrameByName("_24");

	m_pRight_1_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Att01_1_Right_Trail");
	m_pRight_1_Trail->SetAni(false);
	m_pRight_1_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pRight_1_Trail->SetTargetWorldMat(pMatWorld);

	pStartMat = m_pTargetMesh->Get_FrameByName("_25");
	pEndpMat = m_pTargetMesh->Get_FrameByName("_26");

	m_pRight_2_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Att01_2_Right_Trail");
	m_pRight_2_Trail->SetAni(false);
	m_pRight_2_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pRight_2_Trail->SetTargetWorldMat(pMatWorld);
	//----------------------------------------------------------------------------------------

	return S_OK;
}
