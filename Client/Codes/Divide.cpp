#include "stdafx.h"
#include "Divide.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "Phase2_Trail.h"

CDivide::CDivide(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pHammerTrail(NULL)
	, m_pLeftLegTrail(NULL)
	, m_pRightLegTrail(NULL)
	, m_pWorldMat(NULL)
	, m_pLandingWave(NULL)
	, m_bLandingWave(FALSE)
	, m_bHammerSwingStart(FALSE)
	, m_bHammerSwingEnd(FALSE)
	, m_bLeftLegSwingStart(FALSE)
	, m_bLeftLegSwingEnd(FALSE)
	, m_bRightLegSwingStart(FALSE)
	, m_bRightLegSwingEnd(FALSE)
	, m_pDust_0(NULL)
	, m_pDust_1(NULL)
	, m_pDust_2(NULL)
	, m_bDust_0(FALSE)
	, m_bDust_1(FALSE)
	, m_bDust_2(FALSE)
{ 
}

CDivide::~CDivide(void)
{
}

void CDivide::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::ATT_DIVIDE)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//Trail----------------------------------------------
		if (dTargetTime >= 0.759333f && (!m_bHammerSwingStart))
		{
			m_bHammerSwingStart = TRUE;
			m_pHammerTrail->SetAni(TRUE);
			m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pHammerTrail->SettingTrail();
		}

		if (dTargetTime >= 2.444f && (!m_bHammerSwingEnd))
		{
			_vec4 vColor = m_pHammerTrail->GetColor();
			vColor.w -= 0.095f;
			m_pHammerTrail->SetColor(vColor);

			if (m_pHammerTrail->GetColor().w <= 0.0f)
			{
				m_pHammerTrail->SetAni(FALSE);
				m_bHammerSwingEnd = TRUE;
			}
		}

		if (dTargetTime >= 1.206f && (!m_bLeftLegSwingStart))
		{
			m_bLeftLegSwingStart = TRUE;
			m_pLeftLegTrail->SetAni(TRUE);
			m_pLeftLegTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pLeftLegTrail->SettingTrail();
		}

		if (dTargetTime >= 1.652667f && (!m_bLeftLegSwingEnd))
		{
			_vec4 vColor = m_pLeftLegTrail->GetColor();
			vColor.w -= 0.095f;
			m_pLeftLegTrail->SetColor(vColor);

			if (m_pLeftLegTrail->GetColor().w <= 0.0f)
			{
				m_pLeftLegTrail->SetAni(FALSE);
				m_bLeftLegSwingEnd = TRUE;
			}
		}

		if (dTargetTime >= 0.759333f && (!m_bRightLegSwingStart))
		{
			m_bRightLegSwingStart = TRUE;
			m_pRightLegTrail->SetAni(TRUE);
			m_pRightLegTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pRightLegTrail->SettingTrail();
		}

		if (dTargetTime >= 1.652667f && (!m_bRightLegSwingEnd))
		{
			_vec4 vColor = m_pRightLegTrail->GetColor();
			vColor.w -= 0.095f;
			m_pRightLegTrail->SetColor(vColor);

			if (m_pRightLegTrail->GetColor().w <= 0.0f)
			{
				m_pRightLegTrail->SetAni(FALSE);
				m_bRightLegSwingEnd = TRUE;
			}
		}
		//--------------------------------------------------------------

		//Dust------------------------------------------------------------
		if (dTargetTime >= 1.742f && (!m_bDust_0))
		{
			m_bDust_0 = TRUE;
			Dust_0_Start();
		}

		if (dTargetTime >= 1.9206667f && (!m_bDust_1))
		{
			m_bDust_1 = TRUE;
			Dust_1_Start();
		}

		if (dTargetTime >= 2.01f && (!m_bDust_2))
		{
			m_bDust_2 = TRUE;
			Dust_2_Start();
		}
		//----------------------------------------------------------------

		//Wave------------------------------------------------------------
		if (dTargetTime >= 2.01f && (!m_bLandingWave))
		{
			m_bLandingWave = TRUE;
			LandingWaveStart();
		}
		//----------------------------------------------------------------

		if (dTargetTime >= 1.072f && dTargetTime < 1.1f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 1.1f && dTargetTime < 1.15f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bHammerSwingStart		= FALSE;
		m_bHammerSwingEnd		= FALSE;
		m_bLeftLegSwingStart	= FALSE;
		m_bLeftLegSwingEnd		= FALSE;
		m_bRightLegSwingStart	= FALSE;
		m_bRightLegSwingEnd		= FALSE;
		m_bLandingWave			= FALSE;

		m_bDust_0 = FALSE;
		m_bDust_1 = FALSE;
		m_bDust_2 = FALSE;
	}

	LandingWaveUpdate();

	m_pHammerTrail->Update_Object(fTimeDelta);
	m_pRightLegTrail->Update_Object(fTimeDelta);
	m_pLeftLegTrail->Update_Object(fTimeDelta);

	m_pDust_0->Update_Object(fTimeDelta);
	m_pDust_1->Update_Object(fTimeDelta);
	m_pDust_2->Update_Object(fTimeDelta);

	m_pLandingWave->Update_Object(fTimeDelta);
}

void CDivide::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pHammerTrail->Render_Object();
	m_pRightLegTrail->Render_Object();
	m_pLeftLegTrail->Render_Object();

	m_pDust_0->Render_Object();
	m_pDust_1->Render_Object();
	m_pDust_2->Render_Object();
	
	m_pLandingWave->Render_Object();
}

HRESULT CDivide::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);

	m_pDust_0 = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
	m_pDust_0->SetAni(false);
	m_pDust_0->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pDust_0->GetParticleInfo()->m_iPath = 3;
	m_pDust_0->SetRenderCheck(true);
	m_pDust_0->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pDust_0->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
	m_pDust_0->SetBlurCheck(true);
	m_pDust_0->SetBlurCnt(2);
	m_pDust_0->SetBlurPass(3);
	m_pDust_0->SetBlurPower(1.1f);
	m_pDust_0->SetBlurType(Engine::BLUR_CROSS);
	m_pDust_0->SetBlurValue(128.0f);

	m_pDust_1 = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
	m_pDust_1->SetAni(false);
	m_pDust_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pDust_1->GetParticleInfo()->m_iPath = 3;
	m_pDust_1->SetRenderCheck(true);
	m_pDust_1->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pDust_1->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 20.0f;
	m_pDust_1->SetBlurCheck(true);
	m_pDust_1->SetBlurCnt(2);
	m_pDust_1->SetBlurPass(3);
	m_pDust_1->SetBlurPower(1.1f);
	m_pDust_1->SetBlurType(Engine::BLUR_CROSS);
	m_pDust_1->SetBlurValue(128.0f);

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

	m_pLandingWave = CWave::Create(m_pGraphicDev, L"Divide_Landing_Wave");
	m_pLandingWave->SetAni(FALSE);
	m_pLandingWave->SetPattern(CWave::PATTERN_END);
	m_pLandingWave->SetBilBoardCheck(FALSE);
	m_pLandingWave->SetRendererCheck(TRUE);
	m_pLandingWave->GetParticleInfo()->m_iPath = 2;
	m_pLandingWave->GetParticleInfo()->m_iTextureNumber = 7;
	Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	m_pLandingWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;;
	m_pLandingWave->SetBlurCheck(true);
	m_pLandingWave->SetBlurPass(2);
	m_pLandingWave->SetBlurCnt(2);
	m_pLandingWave->SetBlurPower(1.2f);
	m_pLandingWave->SetBlurValue(128.0f);
	m_pLandingWave->SetBlurType(Engine::BLUR_CROSS);

	return S_OK;
}

CDivide * CDivide::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CDivide*		pInstance = new CDivide(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Divide Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDivide::Free(void)
{
	::Safe_Release(m_pHammerTrail);
	::Safe_Release(m_pLeftLegTrail);
	::Safe_Release(m_pRightLegTrail);

	::Safe_Release(m_pDust_0);
	::Safe_Release(m_pDust_1);
	::Safe_Release(m_pDust_2);

	::Safe_Release(m_pLandingWave);

	CEffect::Free();
}

HRESULT CDivide::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CDivide::SkillStart(void)
{
}

void CDivide::EndAniCheck(void)
{
}

HRESULT CDivide::Start_ForScene(void)
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
	m_pRightLegMat = m_pTargetMesh->Get_FrameByName("_55");
	m_pLeftLegMat = m_pTargetMesh->Get_FrameByName("_59");

	const _matrix* pStart = m_pTargetMesh->Get_FrameByName("_63");
	const _matrix* pEnd = m_pTargetMesh->Get_FrameByName("_62");

	m_pHammerTrail = CPhase2_Trail::Create(m_pGraphicDev, L"Divide_Trail");
	m_pHammerTrail->SetAni(FALSE);
	m_pHammerTrail->SetTargetMatrix(pStart, pEnd);
	m_pHammerTrail->SetTargetWorldMat(m_pWorldMat);
	m_pHammerTrail->SetTextureNum(23);
	m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
	
	pStart = m_pTargetMesh->Get_FrameByName("_59");
	pEnd = m_pTargetMesh->Get_FrameByName("_56");

	m_pLeftLegTrail = CPhase2_Trail::Create(m_pGraphicDev, L"Divide_LeftLeg_Trail");
	m_pLeftLegTrail->SetAni(FALSE);
	m_pLeftLegTrail->SetTargetMatrix(pStart, pEnd);
	m_pLeftLegTrail->SetTargetWorldMat(m_pWorldMat);
	m_pLeftLegTrail->SetTextureNum(23);
	m_pLeftLegTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	pStart = m_pTargetMesh->Get_FrameByName("_55");
	pEnd = m_pTargetMesh->Get_FrameByName("_52");

	m_pRightLegTrail = CPhase2_Trail::Create(m_pGraphicDev, L"Divide_RightLeg_Trail");
	m_pRightLegTrail->SetAni(FALSE);
	m_pRightLegTrail->SetTargetMatrix(pStart, pEnd);
	m_pRightLegTrail->SetTargetWorldMat(m_pWorldMat);
	m_pRightLegTrail->SetTextureNum(23);
	m_pRightLegTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	::Safe_Release(pGameObject);

	return S_OK;
}

void CDivide::Dust_0_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_0->SetAni(TRUE);
	m_pDust_0->GetTransCom()->Set_Position(&vPos);
}

void CDivide::Dust_1_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_1->SetAni(TRUE);
	m_pDust_1->GetTransCom()->Set_Position(&vPos);
}

void CDivide::Dust_2_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEndMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_2->SetAni(TRUE);
	m_pDust_2->GetTransCom()->Set_Position(&vPos);
}

void CDivide::LandingWaveStart(void)
{
	_vec4 Color = _vec4(1.0f, 0.1f, 0.0f, 1.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEndMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y += 2.0f;

	m_pLandingWave->SetAni(TRUE);
	m_pLandingWave->GetTransCom()->Set_Position(&vPos);
	m_pLandingWave->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	m_pLandingWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLandingWave->GetTransCom()->Set_Angle(&vAngle);
}

void CDivide::LandingWaveUpdate(void)
{
	if (m_pLandingWave->GetAni())
	{
		_vec4 vColor = (_vec4)m_pLandingWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pLandingWave->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.01f;
			m_pLandingWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;					
			m_pLandingWave->GetTransCom()->Add_Scale(0.4f);			
		}
	}
}
