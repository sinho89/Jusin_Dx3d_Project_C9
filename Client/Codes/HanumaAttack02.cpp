#include "stdafx.h"
#include "HanumaAttack02.h"
#include "PlayerCamera.h"
#include "Wave.h"
#include "Sprite.h"
#include "Hanuman.h"

CHanumanAttack02::CHanumanAttack02(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pAttackWave_0(NULL)
	, m_pAttackWave_1(NULL)
	, m_pWorldMat(NULL)
	, m_pLeftHandMat(NULL)
	, m_pRightHandMat(NULL)
	, m_pHanuman(NULL)
	, m_bSkillStart(FALSE)
	, m_bAttackWave(FALSE)
	, m_bRadialBlur(FALSE)
	, m_fRadialBlurTime(0.0f)
{
	m_bAni = false;

	ZeroMemory(&m_pAttackDust, sizeof(CSprite*) * 10);

	for (int i = 0; i < 5; ++i)
		m_bAttackDust[i] = FALSE;
}

CHanumanAttack02::~CHanumanAttack02(void)
{
}

void CHanumanAttack02::Update_Object(const _float & fTimeDelta)
{
	if (g_bHanumanDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::ATTACK2)
	{
		m_bAni = TRUE;

		//_double dTime = (*m_pvecAniInfo)[CHanuman::HANUMAN::ATTACK1]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 2.96f && (!m_bAttackWave))
		{
			m_bAttackWave = TRUE;

			m_pAttackWave_0->SetAni(TRUE);
			m_pAttackWave_1->SetAni(TRUE);

			m_bRadialBlur = TRUE;
		}

		if (dTargetTime >= 2.00f && (!m_bAttackDust[0]))
		{
			m_bAttackDust[0] = TRUE;

			AttackDustStart(0);

			m_pAttackDust[0]->SetAni(TRUE);
			m_pAttackDust[1]->SetAni(TRUE);			
		}

		if (dTargetTime >= 2.10f && (!m_bAttackDust[1]))
		{
			m_bAttackDust[1] = TRUE;

			AttackDustStart(1);

			m_pAttackDust[2]->SetAni(TRUE);
			m_pAttackDust[3]->SetAni(TRUE);			
		}

		if (dTargetTime >= 2.83f && (!m_bAttackDust[2]))
		{
			m_bAttackDust[2] = TRUE;

			AttackDustStart(2);

			m_pAttackDust[4]->SetAni(TRUE);
			m_pAttackDust[5]->SetAni(TRUE);			
		}

		if (dTargetTime >= 2.86f && (!m_bAttackDust[3]))
		{
			m_bAttackDust[3] = TRUE;

			AttackDustStart(3);

			m_pAttackDust[6]->SetAni(TRUE);
			m_pAttackDust[7]->SetAni(TRUE);			
		}

		if (dTargetTime >= 2.89f && (!m_bAttackDust[4]))
		{
			m_bAttackDust[4] = TRUE;

			AttackDustStart(4);

			m_pAttackDust[8]->SetAni(TRUE);
			m_pAttackDust[9]->SetAni(TRUE);			
		}

		if (!m_bSkillStart)
		{
			SkillStart();
			m_bSkillStart = TRUE;
		}
	}
	else
	{
		m_bAni = FALSE;
		m_bSkillStart = FALSE;
		m_bAttackWave = FALSE;
		
		for (int i = 0; i < 5; ++i)
			m_bAttackDust[i] = FALSE;
	}

	if (m_bRadialBlur)
	{
		Engine::SetRadialBlur(TRUE);
		Engine::SetRadialBlurPower(-0.15f);

		m_fRadialBlurTime += fTimeDelta;
		if (m_fRadialBlurTime >= 0.2f)
		{
			m_fRadialBlurTime = 0.0f;
			Engine::SetRadialBlur(FALSE);
			m_bRadialBlur = FALSE;
		}
	}

	if (!m_bAni)
		return;

	AttackWaveUpdate();
	AttackDustUpdate();
	
	m_pAttackWave_0->Update_Object(fTimeDelta);
	m_pAttackWave_1->Update_Object(fTimeDelta);

	for (int i = 0; i < 10; ++i)
	{
		m_pAttackDust[i]->Update_Object(fTimeDelta);
	}
}

void CHanumanAttack02::Render_Object(void)
{
	if (g_bHanumanDead)
		return;

	if (!m_bAni)
		return;

	m_pAttackWave_0->Render_Object();
	m_pAttackWave_1->Render_Object();

	for (int i = 0; i < 10; ++i)
	{
		m_pAttackDust[i]->Render_Object();
	}
}

HRESULT CHanumanAttack02::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pAttackWave_0 = CWave::Create(m_pGraphicDev, L"AttackWave_0");
	m_pAttackWave_0->SetAni(false);
	m_pAttackWave_0->SetPattern(CWave::PATTERN_END);
	m_pAttackWave_0->SetBilBoardCheck(false);
	m_pAttackWave_0->SetRendererCheck(true);
	m_pAttackWave_0->GetParticleInfo()->m_iPath = 2;
	m_pAttackWave_0->GetParticleInfo()->m_iTextureNumber = 3;

	m_pAttackWave_0->SetBlurCheck(true);
	m_pAttackWave_0->SetBlurPass(2);
	m_pAttackWave_0->SetBlurCnt(2);
	m_pAttackWave_0->SetBlurPower(1.2f);
	m_pAttackWave_0->SetBlurValue(128.0f);
	m_pAttackWave_0->SetBlurType(Engine::BLUR_X);


	m_pAttackWave_1 = CWave::Create(m_pGraphicDev, L"AttackWave_1");
	m_pAttackWave_1->SetAni(false);
	m_pAttackWave_1->SetPattern(CWave::PATTERN_END);
	m_pAttackWave_1->SetBilBoardCheck(false);
	m_pAttackWave_1->SetRendererCheck(true);
	m_pAttackWave_1->GetParticleInfo()->m_iPath = 2;
	m_pAttackWave_1->GetParticleInfo()->m_iTextureNumber = 3;

	m_pAttackWave_1->SetBlurCheck(true);
	m_pAttackWave_1->SetBlurPass(2);
	m_pAttackWave_1->SetBlurCnt(2);
	m_pAttackWave_1->SetBlurPower(1.2f);
	m_pAttackWave_1->SetBlurValue(128.0f);
	m_pAttackWave_1->SetBlurType(Engine::BLUR_X);

	for (int i = 0; i < 10; ++i)
	{
		m_pAttackDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanDust");
		m_pAttackDust[i]->SetAni(false);
		m_pAttackDust[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
		m_pAttackDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pAttackDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pAttackDust[i]->SetRenderCheck(true);

		m_pAttackDust[i]->SetBlurCheck(true);
		m_pAttackDust[i]->SetBlurCnt(2);
		m_pAttackDust[i]->SetBlurPass(3);
		m_pAttackDust[i]->SetBlurPower(1.05f);
		m_pAttackDust[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pAttackDust[i]->SetBlurValue(200.0f);
	}

	return S_OK;
}

CHanumanAttack02 * CHanumanAttack02::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHanumanAttack02*		pInstance = new CHanumanAttack02(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("HanumaAttack02 Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHanumanAttack02::Free(void)
{
	::Safe_Release(m_pAttackWave_0);
	::Safe_Release(m_pAttackWave_1);

	for (int i = 0; i < 10; ++i)
		::Safe_Release(m_pAttackDust[i]);

	::Safe_Release(m_pHanuman);

	CEffect::Free();
}

HRESULT CHanumanAttack02::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CHanumanAttack02::SkillStart(void)
{
	AttackWaveStart();
}

void CHanumanAttack02::EndAniCheck(void)
{
}

HRESULT CHanumanAttack02::Start_ForScene(void)
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

	m_pLeftHandMat	= m_pTargetMesh->Get_FrameByName("_36");
	m_pRightHandMat = m_pTargetMesh->Get_FrameByName("_18");

	return S_OK;
}

void CHanumanAttack02::AttackWaveStart(void)
{
	_vec3 vLook = m_pTargetTransform->Get_Now_Look();
	_vec3 vLeft;

	D3DXVec3Cross(&vLeft, &vLook, &_vec3(0.0f, 1.0f, 0.0f));

	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 1.5f;
	vPos += (vLeft * 3.0f);

	m_pAttackWave_0->SetAni(FALSE);
	m_pAttackWave_0->GetTransCom()->Set_Position(&vPos);
	m_pAttackWave_0->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	_vec4 Color = _vec4(1.0f, 0.8431f, 0.0f, 1.5f);
	m_pAttackWave_0->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	m_pAttackWave_0->GetTransCom()->Set_Angle(&vAngle);

	vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 1.5f;
	vPos -= (vLeft * 3.0f);

	m_pAttackWave_1->SetAni(FALSE);
	m_pAttackWave_1->GetTransCom()->Set_Position(&vPos);
	m_pAttackWave_1->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	m_pAttackWave_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pAttackWave_1->GetTransCom()->Set_Angle(&vAngle);
}

void CHanumanAttack02::AttackWaveUpdate(void)
{
	if (m_pAttackWave_0->GetAni())
	{
		_vec4 vColor = (_vec4)m_pAttackWave_0->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pAttackWave_0->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.08f;
			m_pAttackWave_0->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pAttackWave_0->GetTransCom()->Add_Scale(0.3f);
		}
	}

	if (m_pAttackWave_1->GetAni())
	{
		_vec4 vColor = (_vec4)m_pAttackWave_1->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pAttackWave_1->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.08f;
			m_pAttackWave_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pAttackWave_1->GetTransCom()->Add_Scale(0.3f);
		}
	}
}

void CHanumanAttack02::AttackDustStart(_uint iNum)
{
	_vec3 vLeftDustPos = _vec3(0.0f, 0.0f, 0.0f);
	_vec3 vRightDustPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vLeftDustPos, &vLeftDustPos, m_pLeftHandMat);
	D3DXVec3TransformCoord(&vLeftDustPos, &vLeftDustPos, m_pWorldMat);

	D3DXVec3TransformCoord(&vRightDustPos, &vRightDustPos, m_pRightHandMat);
	D3DXVec3TransformCoord(&vRightDustPos, &vRightDustPos, m_pWorldMat);

	if (0 == iNum)
	{
		m_pAttackDust[0]->GetTransCom()->Set_Position(&vLeftDustPos);
		m_pAttackDust[0]->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);

		m_pAttackDust[1]->GetTransCom()->Set_Position(&vRightDustPos);
		m_pAttackDust[1]->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);
	}
	else if (1 == iNum)
	{
		m_pAttackDust[2]->GetTransCom()->Set_Position(&vLeftDustPos);
		m_pAttackDust[2]->GetTransCom()->Set_Scale(1.1f, 1.1f, 1.1f);

		m_pAttackDust[3]->GetTransCom()->Set_Position(&vRightDustPos);
		m_pAttackDust[3]->GetTransCom()->Set_Scale(1.1f, 1.1f, 1.1f);
	}
	else if (2 == iNum)
	{
		m_pAttackDust[4]->GetTransCom()->Set_Position(&vLeftDustPos);
		m_pAttackDust[4]->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);

		m_pAttackDust[5]->GetTransCom()->Set_Position(&vRightDustPos);
		m_pAttackDust[5]->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);
	}
	else if (3 == iNum)
	{
		m_pAttackDust[6]->GetTransCom()->Set_Position(&vLeftDustPos);
		m_pAttackDust[6]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);

		m_pAttackDust[7]->GetTransCom()->Set_Position(&vRightDustPos);
		m_pAttackDust[7]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	}
	else if (4 == iNum)
	{
		m_pAttackDust[8]->GetTransCom()->Set_Position(&vLeftDustPos);
		m_pAttackDust[8]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);

		m_pAttackDust[9]->GetTransCom()->Set_Position(&vRightDustPos);
		m_pAttackDust[9]->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
	}	
}

void CHanumanAttack02::AttackDustUpdate(void)
{
	for (int i = 0; i < 10; ++i)
	{
		if (m_pAttackDust[i]->GetAni())
		{
			_vec3 vPos = m_pAttackDust[i]->GetTransCom()->Get_Now_Position();
			vPos.y += 0.2f;
			//vPos -= (m_pAttackDust[i]->GetTransCom()->Get_Now_Look() * 1.5f);
			m_pAttackDust[i]->GetTransCom()->Set_Position(&vPos);
		}
	}
}
