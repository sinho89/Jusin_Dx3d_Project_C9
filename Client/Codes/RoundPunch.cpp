#include "stdafx.h"
#include "RoundPunch.h"
#include "PlayerCamera.h"
#include "Wave.h"
#include "Sprite.h"
#include "Hanuman.h"

CRoundPunch::CRoundPunch(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pWorldMat(NULL)
	, m_pLeftHandMat(NULL)
	, m_pRightHandMat(NULL)
	, m_pLeftFootMat(NULL)
	, m_pRightFootMat(NULL)
	, m_pLeftShoulderMat(NULL)
	, m_pRightShoulderMat(NULL)
	, m_pBackMat(NULL)
	, m_pBackDownMat(NULL)
	, m_plowerBodyMat(NULL)
	, m_pFaceMat(NULL)
	, m_pHanuman(NULL)
	, m_bHanumanWhitePower(FALSE)
	, m_bEffectPowerUpDown(FALSE)
	, m_bSkillStart(FALSE)
	, m_bRoundDust_A(FALSE)
	, m_bRoundC1_End(FALSE)
{
	ZeroMemory(&m_pRoundDust_A, sizeof(CSprite*));
	ZeroMemory(&m_pRoundDust_B, sizeof(CSprite*) * 4);
	ZeroMemory(&m_pRoundDust_C, sizeof(CSprite*) * 7);
	ZeroMemory(&m_pRoundC1_Start, sizeof(CSprite*) * 12);
	ZeroMemory(&m_pRoundC1_Ing, sizeof(CSprite*) * 3);
	ZeroMemory(&m_pRoundC1_End, sizeof(CSprite*));

	for (int i = 0; i < 4; ++i)
		m_bRoundDust_B[i] = FALSE;

	for (int i = 0; i < 7; ++i)
		m_bRoundDust_C[i] = FALSE;

	for (int i = 0; i < 12; ++i)
		m_bRoundC1_Ing[i] = FALSE;

	for (int i = 0; i < 3; ++i)
		m_bRoundC1_Ing[i] = FALSE;
}

CRoundPunch::~CRoundPunch(void)
{
}

void CRoundPunch::Update_Object(const _float & fTimeDelta)
{
	if (g_bHanumanDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ROUNDPUNCH_A)
	{
		m_bHanumanWhitePower = TRUE;
		m_bEffectPowerUpDown = TRUE;

		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 2.058f && (!m_bRoundDust_A))
		{
			m_bRoundDust_A = TRUE;
			RoundDust_A_Start();
			m_pRoundDust_A->SetAni(TRUE);
		}
	}
	else
	{
		m_bRoundDust_A = FALSE;
	}

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ROUNDPUNCH_B)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();
		
		if (dTargetTime >= 0.00f && (!m_bRoundDust_B[0]))
		{
			m_bRoundDust_B[0] = TRUE;
			RoundDust_B_Start(0);
			m_pRoundDust_B[0]->SetAni(TRUE);
		}

		if (dTargetTime >= 0.084f && (!m_bRoundDust_B[1]))
		{
			m_bRoundDust_B[1] = TRUE;
			RoundDust_B_Start(1);
			m_pRoundDust_B[1]->SetAni(TRUE);
		}

		if (dTargetTime >= 0.686f && (!m_bRoundDust_B[2]))
		{
			m_bRoundDust_B[2] = TRUE;
			RoundDust_B_Start(2);
			m_pRoundDust_B[2]->SetAni(TRUE);
		}

		if (dTargetTime >= 0.812f && (!m_bRoundDust_B[3]))
		{
			m_bRoundDust_B[3] = TRUE;
			RoundDust_B_Start(3);
			m_pRoundDust_B[3]->SetAni(TRUE);
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
			m_bRoundDust_B[i] = FALSE;
	}

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ROUNDPUNCH_C)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();
		
		if (dTargetTime >= 0.2f && (!m_bRoundDust_C[0]))
		{
			m_bRoundDust_C[0] = TRUE;
			RoundDust_C_Start(0);
			m_pRoundDust_C[0]->SetAni(TRUE);
		}

		if (dTargetTime >= 0.8f && (!m_bRoundDust_C[1]))
		{
			m_bRoundDust_C[1] = TRUE;
			RoundDust_C_Start(1);
			m_pRoundDust_C[1]->SetAni(TRUE);
		}

		if (dTargetTime >= 0.866f && (!m_bRoundDust_C[2]))
		{
			m_bRoundDust_C[2] = TRUE;
			RoundDust_C_Start(2);
			m_pRoundDust_C[2]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.0f && (!m_bRoundDust_C[3]))
		{
			m_bRoundDust_C[3] = TRUE;
			RoundDust_C_Start(3);
			m_pRoundDust_C[3]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.166f && (!m_bRoundDust_C[4]))
		{
			m_bRoundDust_C[4] = TRUE;
			RoundDust_C_Start(4);
			m_pRoundDust_C[4]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.166f && (!m_bRoundDust_C[5]))
		{
			m_bRoundDust_C[5] = TRUE;
			RoundDust_C_Start(5);
			m_pRoundDust_C[5]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.83f && (!m_bRoundDust_C[6]))
		{
			m_bRoundDust_C[6] = TRUE;
			RoundDust_C_Start(6);
			m_pRoundDust_C[6]->SetAni(TRUE);
		}
	}
	else
	{
		for (int i = 0; i < 6; ++i)
			m_bRoundDust_C[i] = FALSE;
	}

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ROUNDPUNCH_C1_START)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 0.76f && (!m_bRoundC1_Start[0]))
		{
			m_bRoundC1_Start[0] = TRUE;
			RoundC1_Start_Start(0);
			m_pRoundC1_Start[0]->SetAni(TRUE);
		}

		if (dTargetTime >= 0.82f && (!m_bRoundC1_Start[1]))
		{
			m_bRoundC1_Start[1] = TRUE;
			RoundC1_Start_Start(1);
			m_pRoundC1_Start[1]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.013f && (!m_pRoundC1_Start[2]))
		{
			m_bRoundC1_Start[2] = TRUE;
			RoundC1_Start_Start(2);
			m_pRoundC1_Start[2]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.076f && (!m_bRoundC1_Start[3]))
		{
			m_bRoundC1_Start[3] = TRUE;
			RoundC1_Start_Start(3);
			m_pRoundC1_Start[3]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.076f && (!m_bRoundC1_Start[4]))
		{
			m_bRoundC1_Start[4] = TRUE;
			RoundC1_Start_Start(4);
			m_pRoundC1_Start[4]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.076f && (!m_bRoundC1_Start[5]))
		{
			m_bRoundC1_Start[5] = TRUE;
			RoundC1_Start_Start(5);
			m_pRoundC1_Start[5]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.6466f && (!m_bRoundC1_Start[6]))
		{
			m_bRoundC1_Start[6] = TRUE;
			RoundC1_Start_Start(6);
			m_pRoundC1_Start[6]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.6466f && (!m_bRoundC1_Start[7]))
		{
			m_bRoundC1_Start[7] = TRUE;
			RoundC1_Start_Start(7);
			m_pRoundC1_Start[7]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.6466f && (!m_bRoundC1_Start[8]))
		{
			m_bRoundC1_Start[8] = TRUE;
			RoundC1_Start_Start(8);
			m_pRoundC1_Start[8]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.6466f && (!m_bRoundC1_Start[9]))
		{
			m_bRoundC1_Start[9] = TRUE;
			RoundC1_Start_Start(9);
			m_pRoundC1_Start[9]->SetAni(TRUE);
		}

		if (dTargetTime >= 3.673f && (!m_bRoundC1_Start[10]))
		{
			m_bRoundC1_Start[10] = TRUE;
			RoundC1_Start_Start(10);
			m_pRoundC1_Start[10]->SetAni(TRUE);
		}

		if (dTargetTime >= 5.06f && (!m_bRoundC1_Start[11]))
		{
			m_bRoundC1_Start[11] = TRUE;
			RoundC1_Start_Start(11);
			m_pRoundC1_Start[11]->SetAni(TRUE);
		}
	}
	else
	{
		for (int i = 0; i < 12; ++i)
			m_bRoundC1_Start[i] = FALSE;
	}

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ROUNDPUNCH_C1_ING)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 0.00f && (!m_bRoundC1_Ing[0]))
		{
			m_bRoundC1_Ing[0] = TRUE;
			RoundC1_Start_Start(0);
			m_pRoundC1_Ing[0]->SetAni(TRUE);
		}

		if (dTargetTime >= 1.84f && (!m_bRoundC1_Ing[1]))
		{
			m_bRoundC1_Ing[1] = TRUE;
			RoundC1_Start_Start(1);
			m_pRoundC1_Ing[1]->SetAni(TRUE);
		}

		if (dTargetTime >= 3.763f && (!m_bRoundC1_Ing[2]))
		{
			m_bRoundC1_Ing[2] = TRUE;
			RoundC1_Start_Start(2);
			m_pRoundC1_Ing[2]->SetAni(TRUE);
		}
	}
	else
	{
		for (int i = 0; i < 3; ++i)
			m_bRoundC1_Ing[i] = FALSE;
	}

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ROUNDPUNCH_C1_END)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 0.709f && (!m_bRoundC1_End))
		{
			m_bRoundC1_End = TRUE;
			RoundC1_End_Start();
			m_pRoundC1_End->SetAni(TRUE);

			m_bEffectPowerUpDown = FALSE;
		}
	}
	else
	{
		m_bRoundC1_End = FALSE;

		m_bHanumanWhitePower = FALSE;
		m_bEffectPowerUpDown = FALSE;
	}

	if (m_bHanumanWhitePower)
	{
		if (m_bEffectPowerUpDown)	//UP
		{
			m_pHanuman->PlusBlurPower(0.2f);
		}
		else//Down
		{
			m_pHanuman->PlusBlurPower(-0.2f);
		}
	}
	else
	{
		m_pHanuman->SetBlurPower(1.7f);
	}


	m_pRoundDust_A->Update_Object(fTimeDelta);

	for (int i = 0; i < 4; ++i)
		m_pRoundDust_B[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 7; ++i)
		m_pRoundDust_C[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 12; ++i)
		m_pRoundC1_Start[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 3; ++i)
		m_pRoundC1_Ing[i]->Update_Object(fTimeDelta);

	m_pRoundC1_End->Update_Object(fTimeDelta);
}

void CRoundPunch::Render_Object(void)
{
	if (g_bHanumanDead)
		return;

	m_pRoundDust_A->Render_Object();

	for (int i = 0; i < 4; ++i)
		m_pRoundDust_B[i]->Render_Object();

	for (int i = 0; i < 7; ++i)
		m_pRoundDust_C[i]->Render_Object();

	for (int i = 0; i < 12; ++i)
		m_pRoundC1_Start[i]->Render_Object();

	for (int i = 0; i < 3; ++i)
		m_pRoundC1_Ing[i]->Render_Object();

	m_pRoundC1_End->Render_Object();
}

HRESULT CRoundPunch::Ready_Object(void)
{
	m_pRoundDust_A = CSprite::LoadCreate(m_pGraphicDev, L"HanumanDust");
	m_pRoundDust_A->SetAni(false);
	m_pRoundDust_A->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
	m_pRoundDust_A->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRoundDust_A->GetParticleInfo()->m_iPath = 3;
	m_pRoundDust_A->SetRenderCheck(true);

	m_pRoundDust_A->SetBlurCheck(true);
	m_pRoundDust_A->SetBlurCnt(2);
	m_pRoundDust_A->SetBlurPass(3);
	m_pRoundDust_A->SetBlurPower(1.05f);
	m_pRoundDust_A->SetBlurType(Engine::BLUR_CROSS);
	m_pRoundDust_A->SetBlurValue(200.0f);

	for (int i = 0; i < 4; ++i)
	{
		m_pRoundDust_B[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanDust");
		m_pRoundDust_B[i]->SetAni(false);
		m_pRoundDust_B[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pRoundDust_B[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pRoundDust_B[i]->GetParticleInfo()->m_iPath = 3;
		m_pRoundDust_B[i]->SetRenderCheck(true);

		m_pRoundDust_B[i]->SetBlurCheck(true);
		m_pRoundDust_B[i]->SetBlurCnt(2);
		m_pRoundDust_B[i]->SetBlurPass(3);
		m_pRoundDust_B[i]->SetBlurPower(1.05f);
		m_pRoundDust_B[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pRoundDust_B[i]->SetBlurValue(200.0f);
	}

	for (int i = 0; i < 7; ++i)
	{
		m_pRoundDust_C[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanDust");
		m_pRoundDust_C[i]->SetAni(false);
		m_pRoundDust_C[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pRoundDust_C[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pRoundDust_C[i]->GetParticleInfo()->m_iPath = 3;
		m_pRoundDust_C[i]->SetRenderCheck(true);

		m_pRoundDust_C[i]->SetBlurCheck(true);
		m_pRoundDust_C[i]->SetBlurCnt(2);
		m_pRoundDust_C[i]->SetBlurPass(3);
		m_pRoundDust_C[i]->SetBlurPower(1.05f);
		m_pRoundDust_C[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pRoundDust_C[i]->SetBlurValue(200.0f);
	}

	for (int i = 0; i < 12; ++i)
	{
		m_pRoundC1_Start[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanDust");
		m_pRoundC1_Start[i]->SetAni(false);
		m_pRoundC1_Start[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pRoundC1_Start[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pRoundC1_Start[i]->GetParticleInfo()->m_iPath = 3;
		m_pRoundC1_Start[i]->SetRenderCheck(true);

		m_pRoundC1_Start[i]->SetBlurCheck(true);
		m_pRoundC1_Start[i]->SetBlurCnt(2);
		m_pRoundC1_Start[i]->SetBlurPass(3);
		m_pRoundC1_Start[i]->SetBlurPower(1.05f);
		m_pRoundC1_Start[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pRoundC1_Start[i]->SetBlurValue(200.0f);
	}

	for (int i = 0; i < 3; ++i)
	{
		m_pRoundC1_Ing[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanDust");
		m_pRoundC1_Ing[i]->SetAni(false);
		m_pRoundC1_Ing[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pRoundC1_Ing[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pRoundC1_Ing[i]->GetParticleInfo()->m_iPath = 3;
		m_pRoundC1_Ing[i]->SetRenderCheck(true);

		m_pRoundC1_Ing[i]->SetBlurCheck(true);
		m_pRoundC1_Ing[i]->SetBlurCnt(2);
		m_pRoundC1_Ing[i]->SetBlurPass(3);
		m_pRoundC1_Ing[i]->SetBlurPower(1.05f);
		m_pRoundC1_Ing[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pRoundC1_Ing[i]->SetBlurValue(200.0f);
	}

	m_pRoundC1_End = CSprite::LoadCreate(m_pGraphicDev, L"HanumanDust");
	m_pRoundC1_End->SetAni(false);
	m_pRoundC1_End->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	m_pRoundC1_End->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRoundC1_End->GetParticleInfo()->m_iPath = 3;
	m_pRoundC1_End->SetRenderCheck(true);

	m_pRoundC1_End->SetBlurCheck(true);
	m_pRoundC1_End->SetBlurCnt(2);
	m_pRoundC1_End->SetBlurPass(3);
	m_pRoundC1_End->SetBlurPower(1.05f);
	m_pRoundC1_End->SetBlurType(Engine::BLUR_CROSS);
	m_pRoundC1_End->SetBlurValue(200.0f);

	return S_OK;
}

CRoundPunch * CRoundPunch::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CRoundPunch*		pInstance = new CRoundPunch(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("RoundPunch Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CRoundPunch::Free(void)
{
	::Safe_Release(m_pRoundDust_A);

	for (int i = 0; i < 4; ++i)
		::Safe_Release(m_pRoundDust_B[i]);

	for (int i = 0; i < 7; ++i)
		::Safe_Release(m_pRoundDust_C[i]);

	for (int i = 0; i < 12; ++i)
		::Safe_Release(m_pRoundC1_Start[i]);

	for (int i = 0; i < 3; ++i)
		::Safe_Release(m_pRoundC1_Ing[i]);

	::Safe_Release(m_pRoundC1_End);
	::Safe_Release(m_pHanuman);

	CEffect::Free();
}

HRESULT CRoundPunch::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CRoundPunch::SkillStart(void)
{
}

void CRoundPunch::EndAniCheck(void)
{
}

HRESULT CRoundPunch::Start_ForScene(void)
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

	m_pWorldMat = ((CHanuman*)m_pHanuman)->GetTransCom()->Get_WorldMatrixPointer();

	m_pLeftHandMat = m_pTargetMesh->Get_FrameByName("_36");
	m_pRightHandMat = m_pTargetMesh->Get_FrameByName("_18");
	m_pLeftFootMat = m_pTargetMesh->Get_FrameByName("_54");
	m_pRightFootMat = m_pTargetMesh->Get_FrameByName("_50");
	m_pLeftShoulderMat = m_pTargetMesh->Get_FrameByName("_46");
	m_pRightShoulderMat = m_pTargetMesh->Get_FrameByName("_16");
	m_pBackMat = m_pTargetMesh->Get_FrameByName("_6");
	m_pBackDownMat = m_pTargetMesh->Get_FrameByName("_5");
	m_plowerBodyMat = m_pTargetMesh->Get_FrameByName("_47");
	m_pFaceMat = m_pTargetMesh->Get_FrameByName("_13");

	return S_OK;
}

void CRoundPunch::RoundDust_A_Start(void)
{
	_vec3 vDustPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightHandMat);
	D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

	m_pRoundDust_A->GetTransCom()->Set_Position(&vDustPos);
	m_pRoundDust_A->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
}

void CRoundPunch::RoundDust_A_Update(void)
{
	if (m_pRoundDust_A->GetAni())
	{
		_vec3 vPos = m_pRoundDust_A->GetTransCom()->Get_Now_Position();
		vPos.y += 0.2f;
		vPos -= (m_pRoundDust_A->GetTransCom()->Get_Now_Look() * 1.5f);
		m_pRoundDust_A->GetTransCom()->Set_Position(&vPos);
	}
}

void CRoundPunch::RoundDust_B_Start(_uint iNum)
{
	_vec3 vDustPos = _vec3(0.0f, 0.0f, 0.0f);

	if (0 == iNum)	//¿Þ¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pLeftHandMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_B[0]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_B[0]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
	else if (1 == iNum)	//¿Þ¹ß
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pLeftFootMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_B[1]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_B[1]->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	}
	else if (2 == iNum)	//¿À¸¥¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightHandMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_B[2]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_B[2]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
	else if (3 == iNum)	//¿À¸¥¹ß
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightFootMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_B[3]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_B[3]->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	}
}

void CRoundPunch::RoundDust_B_Update(void)
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_pRoundDust_B[i]->GetAni())
		{
			_vec3 vPos = m_pRoundDust_A->GetTransCom()->Get_Now_Position();
			vPos.y += 0.2f;
			vPos -= (m_pRoundDust_A->GetTransCom()->Get_Now_Look() * 1.5f);
			m_pRoundDust_A->GetTransCom()->Set_Position(&vPos);
		}
	}
}

void CRoundPunch::RoundDust_C_Start(_uint iNum)
{
	_vec3 vDustPos = _vec3(0.0f, 0.0f, 0.0f);

	if (0 == iNum)	//¿Þ¹ß
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pLeftFootMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_C[0]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_C[0]->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	}
	else if (1 == iNum)	//¿À¸¥¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightHandMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_C[1]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_C[1]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
	else if (2 == iNum)	//¿À¸¥¾î±ú
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightShoulderMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_C[2]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_C[2]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
	}
	else if (3 == iNum)	//µî
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pBackMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_C[3]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_C[3]->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	}
	else if (4 == iNum)	//µî ¾Æ·¡
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pBackDownMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_C[4]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_C[4]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
	}
	else if (5 == iNum)	//¿Þ ¾î±ú
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pLeftShoulderMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_C[5]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_C[5]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
	}
	else if (6 == iNum)	//¿À¸¥¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightHandMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundDust_C[6]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundDust_C[6]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
}

void CRoundPunch::RoundDust_C_Update(void)
{
	for (int i = 0; i < 7; ++i)
	{
		if (m_pRoundDust_C[i]->GetAni())
		{
			_vec3 vPos = m_pRoundDust_A->GetTransCom()->Get_Now_Position();
			vPos.y += 0.2f;
			vPos -= (m_pRoundDust_A->GetTransCom()->Get_Now_Look() * 1.5f);
			m_pRoundDust_A->GetTransCom()->Set_Position(&vPos);
		}
	}
}

void CRoundPunch::RoundC1_Start_Start(_uint iNum)
{
	_vec3 vDustPos = _vec3(0.0f, 0.0f, 0.0f);

	if (0 == iNum)	//¿À¸¥¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightHandMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[0]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[0]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
	else if (1 == iNum)	//¿À¸¥¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightHandMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[1]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[1]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
	else if (2 == iNum)	//ÇÏÃ¼
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_plowerBodyMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[2]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[2]->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	}
	else if (3 == iNum)	//µî
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pBackMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[3]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[3]->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	}
	else if (4 == iNum)	//¿À¸¥ ¾î±ú
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightShoulderMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[4]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[4]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
	}
	else if (5 == iNum)	//¿Þ ¾î±ú
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pLeftShoulderMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[5]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[5]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
	}
	else if (6 == iNum)	//ÇÏÃ¼
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_plowerBodyMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[6]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[6]->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	}
	else if (7 == iNum)	//µî
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pBackMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[7]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[7]->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	}
	else if (8 == iNum)	//¿À¸¥¾î±ú
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightShoulderMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[8]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[8]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
	}
	else if (9 == iNum)	//¿Þ ¾î±ú
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pLeftShoulderMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[9]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[9]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
	}
	else if (10 == iNum)	//¿À¸¥¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pRightHandMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[10]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[10]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
	else if (11 == iNum)	//¿Þ¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pLeftHandMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		m_pRoundC1_Start[11]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Start[11]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
}

void CRoundPunch::RoundC1_Start_Update(void)
{
	for (int i = 0; i < 12; ++i)
	{
		if (m_pRoundC1_Start[i]->GetAni())
		{
			_vec3 vPos = m_pRoundDust_A->GetTransCom()->Get_Now_Position();
			vPos.y += 0.2f;
			vPos -= (m_pRoundDust_A->GetTransCom()->Get_Now_Look() * 1.5f);
			m_pRoundDust_A->GetTransCom()->Set_Position(&vPos);
		}
	}
}

void CRoundPunch::RoundC1_Ing_Start(_uint iNum)
{
	_vec3 vDustPos = _vec3(0.0f, 0.0f, 0.0f);

	if (0 == iNum)	//¿À¸¥¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pFaceMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		vDustPos.y += 1.0f;

		m_pRoundC1_Ing[0]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Ing[0]->GetTransCom()->Set_Scale(0.25f, 0.25f, 0.25f);
	}
	else if (1 == iNum)	//¿À¸¥¼Õ
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pFaceMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		vDustPos.y += 1.0f;

		m_pRoundC1_Ing[1]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Ing[1]->GetTransCom()->Set_Scale(0.25f, 0.25f, 0.25f);
	}
	else if (2 == iNum)	//ÇÏÃ¼
	{
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pFaceMat);
		D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

		vDustPos.y += 1.0f;

		m_pRoundC1_Ing[2]->GetTransCom()->Set_Position(&vDustPos);
		m_pRoundC1_Ing[2]->GetTransCom()->Set_Scale(0.25f, 0.25f, 0.25f);
	}
}

void CRoundPunch::RoundC1_Ing_Update(void)
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_pRoundC1_Ing[i]->GetAni())
		{
			_vec3 vPos = m_pRoundDust_A->GetTransCom()->Get_Now_Position();
			vPos.y += 0.2f;
			vPos -= (m_pRoundDust_A->GetTransCom()->Get_Now_Look() * 1.5f);
			m_pRoundDust_A->GetTransCom()->Set_Position(&vPos);
		}
	}
}

void CRoundPunch::RoundC1_End_Start(void)
{
	_vec3 vDustPos = _vec3(0.0f, 0.0f, 0.0f); 

	D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pLeftHandMat);
	D3DXVec3TransformCoord(&vDustPos, &vDustPos, m_pWorldMat);

	m_pRoundDust_A->GetTransCom()->Set_Position(&vDustPos);
	m_pRoundDust_A->GetTransCom()->Set_Scale(1.4f, 1.4f, 1.4f);
}

void CRoundPunch::RoundC1_End_Update(void)
{
	if (m_pRoundDust_A->GetAni())
	{
		_vec3 vPos = m_pRoundDust_A->GetTransCom()->Get_Now_Position();
		vPos.y += 0.2f;
		vPos -= (m_pRoundDust_A->GetTransCom()->Get_Now_Look() * 1.5f);
		m_pRoundDust_A->GetTransCom()->Set_Position(&vPos);
	}
}

