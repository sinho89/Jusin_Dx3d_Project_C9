#include "stdafx.h"
#include "ShortHammer.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "Phase2_Trail.h"

CShortHammer::CShortHammer(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pHammerTrail(NULL)
	, m_pWorldMat(NULL)
	, m_pDust_2(NULL)
	, m_pDust_3(NULL)
	, m_pDust_4(NULL)
	, m_bHammerTrail(FALSE)
	, m_bDust_2(FALSE)
	, m_bDust_3(FALSE)
	, m_bDust_4(FALSE)
	, m_bHammerWave_0(FALSE)
	, m_bHammerWave_1(FALSE)
{
	ZeroMemory(&m_pDust_0, sizeof(CSprite*) * 5);
	ZeroMemory(&m_pDust_1, sizeof(CSprite*) * 8);
	ZeroMemory(&m_pHammerWave_0, sizeof(CWave*) * 2);
	ZeroMemory(&m_pHammerWave_1, sizeof(CWave*) * 2);

	for (int i = 0; i < 5; ++i)
		m_bDust_0[i] = FALSE;

	for (int i = 0; i < 8; ++i)
		m_bDust_1[i] = FALSE;	
}

CShortHammer::~CShortHammer(void)
{
}

void CShortHammer::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::ATT_SHORTHAMMER)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//Trail----------------------------------------------
		if (!m_bHammerTrail)
		{
			m_bHammerTrail = TRUE;
			m_pHammerTrail->SetAni(TRUE);
			m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pHammerTrail->SettingTrail();
		}
		//--------------------------------------------------------------

		//Dust-----------------------------------------------------------
		for (int i = 0; i < 5; ++i)
		{
			if (dTargetTime >= 1.188f + (i * 0.0432f) && (!m_bDust_0[i]))
			{
				m_bDust_0[i] = TRUE;
				Dust_0_Start(i);
			}
		}

		for (int i = 0; i < 8; ++i)
		{
			if (dTargetTime >= 2.592f + (i * 0.081f) && (!m_bDust_1[i]))
			{
				m_bDust_1[i] = TRUE;
				Dust_1_Start(i);
			}
		}

		if (dTargetTime >= 4.104f && (!m_bDust_2))
		{
			m_bDust_2 = TRUE;
			Dust_2_Start();
		}

		if (dTargetTime >= 5.616f && (!m_bDust_3))
		{
			m_bDust_3 = TRUE;
			Dust_3_Start();
		}

		if (dTargetTime >= 6.804f && (!m_bDust_4))
		{
			m_bDust_4 = TRUE;
			Dust_4_Start();
		}
		//--------------------------------------------------------------

		//Wave------------------------------------------------------------
		if (dTargetTime >= 4.1040001f && (!m_bHammerWave_0))
		{
			m_bHammerWave_0 = TRUE;
			HammerWave_0_Start();
		}

		if (dTargetTime >= 6.8040001f && (!m_bHammerWave_1))
		{
			m_bHammerWave_1 = TRUE;
			HammerWave_1_Start();
		}
		//----------------------------------------------------------------

		if (dTargetTime >= 4.104001f && dTargetTime < 4.2f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 4.2f && dTargetTime < 4.3f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}

		if (dTargetTime >= 6.8040001f && dTargetTime < 6.9f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 6.9f && dTargetTime < 7.0f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bHammerTrail = FALSE;
		m_pHammerTrail->SetAni(FALSE);

		for (int i = 0; i < 5; ++i)
			m_bDust_0[i] = FALSE;

		for (int i = 0; i < 8; ++i)
			m_bDust_1[i] = FALSE;

		m_bDust_2 = FALSE;
		m_bDust_3 = FALSE;
		m_bDust_4 = FALSE;

		m_bHammerWave_0 = FALSE;
		m_bHammerWave_1 = FALSE;
	}

	HammerWave_0_Update();
	HammerWave_1_Update();

	m_pHammerTrail->Update_Object(fTimeDelta);

	for (int i = 0; i < 5; ++i)
		m_pDust_0[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 8; ++i)
		m_pDust_1[i]->Update_Object(fTimeDelta);

	m_pDust_2->Update_Object(fTimeDelta);
	m_pDust_3->Update_Object(fTimeDelta);
	m_pDust_4->Update_Object(fTimeDelta);

	for (int i = 0; i < 2; ++i)
		m_pHammerWave_0[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 2; ++i)
		m_pHammerWave_1[i]->Update_Object(fTimeDelta);
}

void CShortHammer::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pHammerTrail->Render_Object();

	for (int i = 0; i < 5; ++i)
		m_pDust_0[i]->Render_Object();

	for (int i = 0; i < 8; ++i)
		m_pDust_1[i]->Render_Object();

	m_pDust_2->Render_Object();
	m_pDust_3->Render_Object();
	m_pDust_4->Render_Object();

	for (int i = 0; i < 2; ++i)
		m_pHammerWave_0[i]->Render_Object();

	for (int i = 0; i < 2; ++i)
		m_pHammerWave_1[i]->Render_Object();
}

HRESULT CShortHammer::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);

	for (int i = 0; i < 5; ++i)
	{
		m_pDust_0[i] = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
		m_pDust_0[i]->SetAni(false);		
		m_pDust_0[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pDust_0[i]->GetParticleInfo()->m_iPath = 3;
		m_pDust_0[i]->SetRenderCheck(true);
		m_pDust_0[i]->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
		m_pDust_0[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
		m_pDust_0[i]->SetBlurCheck(true);
		m_pDust_0[i]->SetBlurCnt(2);
		m_pDust_0[i]->SetBlurPass(3);
		m_pDust_0[i]->SetBlurPower(1.1f);
		m_pDust_0[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pDust_0[i]->SetBlurValue(128.0f);
	}

	for (int i = 0; i < 8; ++i)
	{
		m_pDust_1[i] = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
		m_pDust_1[i]->SetAni(false);
		m_pDust_1[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pDust_1[i]->GetParticleInfo()->m_iPath = 3;
		m_pDust_1[i]->SetRenderCheck(true);
		m_pDust_1[i]->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
		m_pDust_1[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
		m_pDust_1[i]->SetBlurCheck(true);
		m_pDust_1[i]->SetBlurCnt(2);
		m_pDust_1[i]->SetBlurPass(3);
		m_pDust_1[i]->SetBlurPower(1.1f);
		m_pDust_1[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pDust_1[i]->SetBlurValue(128.0f);
	}

	m_pDust_2 = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
	m_pDust_2->SetAni(false);
	m_pDust_2->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pDust_2->GetParticleInfo()->m_iPath = 3;
	m_pDust_2->SetRenderCheck(true);
	m_pDust_2->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pDust_2->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
	m_pDust_2->SetBlurCheck(true);
	m_pDust_2->SetBlurCnt(2);
	m_pDust_2->SetBlurPass(3);
	m_pDust_2->SetBlurPower(1.1f);
	m_pDust_2->SetBlurType(Engine::BLUR_CROSS);
	m_pDust_2->SetBlurValue(128.0f);

	m_pDust_3 = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
	m_pDust_3->SetAni(false);
	m_pDust_3->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pDust_3->GetParticleInfo()->m_iPath = 3;
	m_pDust_3->SetRenderCheck(true);
	m_pDust_3->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pDust_3->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
	m_pDust_3->SetBlurCheck(true);
	m_pDust_3->SetBlurCnt(2);
	m_pDust_3->SetBlurPass(3);
	m_pDust_3->SetBlurPower(1.1f);
	m_pDust_3->SetBlurType(Engine::BLUR_CROSS);
	m_pDust_3->SetBlurValue(128.0f);

	m_pDust_4 = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
	m_pDust_4->SetAni(false);
	m_pDust_4->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pDust_4->GetParticleInfo()->m_iPath = 3;
	m_pDust_4->SetRenderCheck(true);
	m_pDust_4->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pDust_4->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
	m_pDust_4->SetBlurCheck(true);
	m_pDust_4->SetBlurCnt(2);
	m_pDust_4->SetBlurPass(3);
	m_pDust_4->SetBlurPower(1.1f);
	m_pDust_4->SetBlurType(Engine::BLUR_CROSS);
	m_pDust_4->SetBlurValue(128.0f);

	for (int i = 0; i < 2; ++i)
	{
		m_pHammerWave_0[i] = CWave::Create(m_pGraphicDev, L"ShortHammer_Wave_First");
		m_pHammerWave_0[i]->SetAni(FALSE);
		m_pHammerWave_0[i]->SetPattern(CWave::PATTERN_END);
		m_pHammerWave_0[i]->SetBilBoardCheck(FALSE);
		m_pHammerWave_0[i]->SetRendererCheck(TRUE);
		m_pHammerWave_0[i]->GetParticleInfo()->m_iPath = 2;		
		m_pHammerWave_0[i]->GetParticleInfo()->m_iTextureNumber = 3;
		Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
		m_pHammerWave_0[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pHammerWave_0[i]->SetBlurCheck(true);
		m_pHammerWave_0[i]->SetBlurPass(2);
		m_pHammerWave_0[i]->SetBlurCnt(2);
		m_pHammerWave_0[i]->SetBlurPower(1.1f);
		m_pHammerWave_0[i]->SetBlurValue(128.0f);
		m_pHammerWave_0[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pHammerWave_1[i] = CWave::Create(m_pGraphicDev, L"ShortHammer_Wave_Second");
		m_pHammerWave_1[i]->SetAni(FALSE);
		m_pHammerWave_1[i]->SetPattern(CWave::PATTERN_END);
		m_pHammerWave_1[i]->SetBilBoardCheck(FALSE);
		m_pHammerWave_1[i]->SetRendererCheck(TRUE);
		m_pHammerWave_1[i]->GetParticleInfo()->m_iPath = 2;
		m_pHammerWave_1[i]->GetParticleInfo()->m_iTextureNumber = 7;
		Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
		m_pHammerWave_1[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pHammerWave_1[i]->SetBlurCheck(true);
		m_pHammerWave_1[i]->SetBlurPass(2);
		m_pHammerWave_1[i]->SetBlurCnt(2);
		m_pHammerWave_1[i]->SetBlurPower(1.2f);
		m_pHammerWave_1[i]->SetBlurValue(128.0f);
		m_pHammerWave_1[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	return S_OK;
}

CShortHammer * CShortHammer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CShortHammer*		pInstance = new CShortHammer(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("ShortHammer Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CShortHammer::Free(void)
{
	::Safe_Release(m_pHammerTrail);

	for (int i = 0; i < 5; ++i)
		::Safe_Release(m_pDust_0[i]);

	for (int i = 0; i < 8; ++i)
		::Safe_Release(m_pDust_1[i]);

	::Safe_Release(m_pDust_2);
	::Safe_Release(m_pDust_3);
	::Safe_Release(m_pDust_4);

	for (int i = 0; i < 2; ++i)
		::Safe_Release(m_pHammerWave_0[i]);

	for (int i = 0; i < 2; ++i)
		::Safe_Release(m_pHammerWave_1[i]);

	CEffect::Free();
}

HRESULT CShortHammer::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CShortHammer::SkillStart(void)
{
}

void CShortHammer::EndAniCheck(void)
{
}

HRESULT CShortHammer::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::Find(L"Mesh_Bardiel");
	if (pGameObject == NULL)
		return E_FAIL;

	m_pBardiel = ((CBardiel*)pGameObject);
	m_pTargetAnimator = ((CBardiel*)pGameObject)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//Hanuman TransCom
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pWorldMat = ((CBardiel*)pGameObject)->GetTransCom()->Get_WorldMatrixPointer();

	m_pLeftLegMat = m_pTargetMesh->Get_FrameByName("_59");
	m_pHammerEnd = m_pTargetMesh->Get_FrameByName("_63");
	const _matrix* pHammerEnd = m_pTargetMesh->Get_FrameByName("_62");

	m_pHammerTrail = CPhase2_Trail::Create(m_pGraphicDev, L"ShortHammer_Trail");
	m_pHammerTrail->SetAni(FALSE);
	m_pHammerTrail->SetTargetMatrix(m_pHammerEnd, pHammerEnd);
	m_pHammerTrail->SetTargetWorldMat(m_pWorldMat);
	m_pHammerTrail->SetTextureNum(23);
	m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	::Safe_Release(pGameObject);

	return S_OK;
}

void CShortHammer::Dust_0_Start(int iIndex)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y = m_pBardiel->GetTransCom()->Get_Now_Position().y;

	m_pDust_0[iIndex]->SetAni(TRUE);
	m_pDust_0[iIndex]->GetTransCom()->Set_Position(&vPos);
}

void CShortHammer::Dust_1_Start(int iIndex)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y = m_pBardiel->GetTransCom()->Get_Now_Position().y;

	m_pDust_1[iIndex]->SetAni(TRUE);
	m_pDust_1[iIndex]->GetTransCom()->Set_Position(&vPos);
}

void CShortHammer::Dust_2_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_2->SetAni(TRUE);
	m_pDust_2->GetTransCom()->Set_Position(&vPos);
}

void CShortHammer::Dust_3_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_3->SetAni(TRUE);
	m_pDust_3->GetTransCom()->Set_Position(&vPos);
}

void CShortHammer::Dust_4_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_4->SetAni(TRUE);
	m_pDust_4->GetTransCom()->Set_Position(&vPos);
}

void CShortHammer::HammerWave_0_Start(void)
{
	_vec4 Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y += 1.5f;

	for (int i = 0; i < 2; ++i)
	{
		m_pHammerWave_0[i]->SetAni(TRUE);
		m_pHammerWave_0[i]->GetTransCom()->Set_Position(&vPos);
		m_pHammerWave_0[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pHammerWave_0[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pHammerWave_0[i]->GetTransCom()->Set_Angle(&vAngle);
	}
}

void CShortHammer::HammerWave_1_Start(void)
{
	_vec4 Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y += 1.5f;

	for (int i = 0; i < 2; ++i)
	{
		m_pHammerWave_1[i]->SetAni(TRUE);
		m_pHammerWave_1[i]->GetTransCom()->Set_Position(&vPos);
		m_pHammerWave_1[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pHammerWave_1[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pHammerWave_1[i]->GetTransCom()->Set_Angle(&vAngle);
	}
}

void CShortHammer::HammerWave_0_Update(void)
{
	for (int i = 0; i < 2; ++i)
	{
		if (m_pHammerWave_0[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pHammerWave_0[i]->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pHammerWave_0[i]->SetAni(FALSE);
			}
			else
			{
				vColor.w -= 0.015f;
				m_pHammerWave_0[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

				if (0 == i)
					m_pHammerWave_0[i]->GetTransCom()->Add_Scale(0.3f);
				else if (1 == i)
					m_pHammerWave_0[i]->GetTransCom()->Add_Scale(0.5f);
			}
		}
	}
}

void CShortHammer::HammerWave_1_Update(void)
{
	for (int i = 0; i < 2; ++i)
	{
		if (m_pHammerWave_1[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pHammerWave_1[i]->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pHammerWave_1[i]->SetAni(FALSE);
			}
			else
			{
				vColor.w -= 0.015f;
				m_pHammerWave_1[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

				if (0 == i)
					m_pHammerWave_1[i]->GetTransCom()->Add_Scale(0.3f);
				else if (1 == i)
					m_pHammerWave_1[i]->GetTransCom()->Add_Scale(0.5f);
			}
		}
	}
}
