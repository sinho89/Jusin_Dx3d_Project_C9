#include "stdafx.h"
#include "Clean.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "Phase2_Trail.h"

CClean::CClean(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pHammerTrail(NULL)
	, m_pWorldMat(NULL)
	, m_pHammerEndMat(NULL)
	, m_pDust_2(NULL)
	, m_bDust_2(FALSE)
	, m_bHammerTrail(FALSE)
	, m_bHammerWave(FALSE)
{
	ZeroMemory(&m_pDust_0, sizeof(CSprite*) * 6);
	ZeroMemory(&m_pDust_1, sizeof(CSprite*) * 6);
	ZeroMemory(&m_pHammerWave, sizeof(CWave*) * 3);

	for (int i = 0; i < 6; ++i)
		m_bDust_0[i] = FALSE;

	for (int i = 0; i < 6; ++i)
		m_bDust_1[i] = FALSE;
}

CClean::~CClean(void)
{
}

void CClean::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::ATT_CLEAN)
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
		for (int i = 0; i < 6; ++i)
		{
			if (dTargetTime >= 2.008f + (i * 0.0697f) && (!m_bDust_0[i]))
			{
				m_bDust_0[i] = TRUE;
				Dust_0_Start(i);
			}
		}

		for (int i = 0; i < 6; ++i)
		{
			if (dTargetTime >= 3.765f + (i * 0.055777f) && (!m_bDust_1[i]))
			{
				m_bDust_1[i] = TRUE;
				Dust_1_Start(i);
			}
		}

		if (dTargetTime >= 5.9403334f && (!m_bDust_2))
		{
			m_bDust_2 = TRUE;
			Dust_2_Start();
		}
		//--------------------------------------------------------------
		//Wave------------------------------------------------------------
		if (dTargetTime >= 5.9403334f && (!m_bHammerWave))
		{
			m_bHammerWave = TRUE;
			HammerWaveStart();
		}
		//----------------------------------------------------------------

		if (dTargetTime >= 5.940334f && dTargetTime < 6.0f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 6.0f && dTargetTime < 6.05f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bHammerTrail = FALSE;
		m_pHammerTrail->SetAni(FALSE);

		for (int i = 0; i < 6; ++i)
			m_bDust_0[i] = FALSE;

		for (int i = 0; i < 6; ++i)
			m_bDust_1[i] = FALSE;

		m_bDust_2 = FALSE;
		m_bHammerWave = FALSE;
	}

	HammerWaveUpdate();

	m_pHammerTrail->Update_Object(fTimeDelta);

	for (int i = 0; i < 6; ++i)
		m_pDust_0[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 6; ++i)
		m_pDust_1[i]->Update_Object(fTimeDelta);

	m_pDust_2->Update_Object(fTimeDelta);

	for (int i = 0; i < 3; ++i)
		m_pHammerWave[i]->Update_Object(fTimeDelta);
}

void CClean::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pHammerTrail->Render_Object();

	for (int i = 0; i < 6; ++i)
		m_pDust_0[i]->Render_Object();

	for (int i = 0; i < 6; ++i)
		m_pDust_1[i]->Render_Object();

	m_pDust_2->Render_Object();

	for (int i = 0; i < 3; ++i)
		m_pHammerWave[i]->Render_Object();
}

HRESULT CClean::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);

	for (int i = 0; i < 6; ++i)
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

	for (int i = 0; i < 6; ++i)
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

	for (int i = 0; i < 3; ++i)
	{
		m_pHammerWave[i] = CWave::Create(m_pGraphicDev, L"Clean_Hammer_Wave");
		m_pHammerWave[i]->SetAni(FALSE);
		m_pHammerWave[i]->SetPattern(CWave::PATTERN_END);
		m_pHammerWave[i]->SetBilBoardCheck(FALSE);
		m_pHammerWave[i]->SetRendererCheck(TRUE);
		m_pHammerWave[i]->GetParticleInfo()->m_iPath = 2;

		if (0 == i)
			m_pHammerWave[i]->GetParticleInfo()->m_iTextureNumber = 1;
		else if (1 == i)
			m_pHammerWave[i]->GetParticleInfo()->m_iTextureNumber = 3;
		else if (2 == i)
			m_pHammerWave[i]->GetParticleInfo()->m_iTextureNumber = 7;

		D3DXCOLOR vColor = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
		m_pHammerWave[i]->GetParticleInfo()->m_dColor = vColor;
		m_pHammerWave[i]->SetBlurCheck(true);
		m_pHammerWave[i]->SetBlurPass(2);
		m_pHammerWave[i]->SetBlurCnt(2);
		m_pHammerWave[i]->SetBlurPower(1.2f);
		m_pHammerWave[i]->SetBlurValue(128.0f);
		m_pHammerWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	return S_OK;
}

CClean * CClean::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CClean*		pInstance = new CClean(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Clean Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CClean::Free(void)
{
	::Safe_Release(m_pHammerTrail);

	for (int i = 0; i < 6; ++i)
		::Safe_Release(m_pDust_0[i]);

	for (int i = 0; i < 6; ++i)
		::Safe_Release(m_pDust_1[i]);

	::Safe_Release(m_pDust_2);

	for (int i = 0; i < 3; ++i)
		::Safe_Release(m_pHammerWave[i]);

	CEffect::Free();
}

HRESULT CClean::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CClean::SkillStart(void)
{
}

void CClean::EndAniCheck(void)
{
}

HRESULT CClean::Start_ForScene(void)
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

	m_pHammerEndMat = m_pTargetMesh->Get_FrameByName("_63");
	const _matrix* pHammerEnd = m_pTargetMesh->Get_FrameByName("_62");

	m_pHammerTrail = CPhase2_Trail::Create(m_pGraphicDev, L"Clean_Trail");
	m_pHammerTrail->SetAni(FALSE);
	m_pHammerTrail->SetTargetMatrix(m_pHammerEndMat, pHammerEnd);
	m_pHammerTrail->SetTargetWorldMat(m_pWorldMat);
	m_pHammerTrail->SetTextureNum(23);
	m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	::Safe_Release(pGameObject);

	return S_OK;
}

void CClean::Dust_0_Start(int iIndex)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEndMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y = m_pBardiel->GetTransCom()->Get_Now_Position().y;

	m_pDust_0[iIndex]->SetAni(TRUE);
	m_pDust_0[iIndex]->GetTransCom()->Set_Position(&vPos);
}

void CClean::Dust_1_Start(int iIndex)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEndMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y = m_pBardiel->GetTransCom()->Get_Now_Position().y;

	m_pDust_1[iIndex]->SetAni(TRUE);
	m_pDust_1[iIndex]->GetTransCom()->Set_Position(&vPos);
}

void CClean::Dust_2_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEndMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_2->SetAni(TRUE);
	m_pDust_2->GetTransCom()->Set_Position(&vPos);
}

void CClean::HammerWaveStart(void)
{
	_vec4 Color = _vec4(1.0f, 0.1f, 0.0f, 1.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEndMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y += 1.9f;

	for (int i = 0; i < 3; ++i)
	{
		m_pHammerWave[i]->SetAni(TRUE);
		m_pHammerWave[i]->GetTransCom()->Set_Position(&vPos);
		m_pHammerWave[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pHammerWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pHammerWave[i]->GetTransCom()->Set_Angle(&vAngle);
	}
}

void CClean::HammerWaveUpdate(void)
{
	for (int i = 0; i < 3; ++i)
	{
		if (m_pHammerWave[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pHammerWave[i]->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pHammerWave[i]->SetAni(FALSE);
			}
			else
			{
				vColor.w -= 0.03f;
				m_pHammerWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

				if (0 == i)
					m_pHammerWave[i]->GetTransCom()->Add_Scale(0.7f);
				else if (1 == i)
					m_pHammerWave[i]->GetTransCom()->Add_Scale(0.4f);
				else if (2 == i)
					m_pHammerWave[i]->GetTransCom()->Add_Scale(0.2f);
			}
		}
	}
}
