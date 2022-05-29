#include "stdafx.h"
#include "LFoot.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "Phase2_Trail.h"

CLFoot::CLFoot(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pRightLegTrail(NULL)
	, m_pWorldMat(NULL)
	, m_pRightLegMat(NULL)
	, m_pDust_0(NULL)
	, m_pLegWave(NULL)
	, m_bLegWave(FALSE)
	, m_bLegSwingStart(FALSE)
	, m_bLegSwingEnd(FALSE)
	, m_bDust_0(FALSE)
{
	ZeroMemory(&m_pDust_1, sizeof(CSprite*) * 5);

	for (int i = 0; i < 5; ++i)
		m_bDust_1[i] = FALSE;
}

CLFoot::~CLFoot(void)
{
}

void CLFoot::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::ATT_LFOOT)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//Trail----------------------------------------------
		if (dTargetTime >= 3.9246668f && (!m_bLegSwingStart))
		{
			m_bLegSwingStart = TRUE;
			m_pRightLegTrail->SetAni(TRUE);
			m_pRightLegTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pRightLegTrail->SettingTrail();
		}

		if (dTargetTime >= 4.4660001f && (!m_bLegSwingEnd))
		{
			_vec4 vColor = m_pRightLegTrail->GetColor();
			vColor.w -= 0.095f;
			m_pRightLegTrail->SetColor(vColor);

			if (m_pRightLegTrail->GetColor().w <= 0.0f)
			{
				m_pRightLegTrail->SetAni(FALSE);
				m_bLegSwingEnd = TRUE;
			}
		}
		//--------------------------------------------------------------

		//Dust-----------------------------------------------------------
		if (dTargetTime >= 2.4360001f && (!m_bDust_0))
		{
			m_bDust_0 = TRUE;
			Dust_0_Start();
		}
		
		for (int i = 0; i < 5; ++i)
		{
			if (dTargetTime >= 4.1276668f + (i * 0.0406f) && (!m_bDust_1[i]))
			{
				m_bDust_1[i] = TRUE;
				Dust_1_Start(i);
			}
		}
		//--------------------------------------------------------------

		//Wave-----------------------------------------------------------
		if (dTargetTime >= 2.4360001f && (!m_bLegWave))
		{
			m_bLegWave = TRUE;
			LegWaveStart();
		}
		//----------------------------------------------------------------

		if (dTargetTime >= 2.43415f && dTargetTime < 2.5f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 2.5f && dTargetTime < 2.6f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bLegSwingStart = FALSE;
		m_bLegSwingEnd = FALSE;
		m_bDust_0 = FALSE;

		for (int i = 0; i < 5; ++i)
			m_bDust_1[i] = FALSE;

		m_bLegWave = FALSE;
	}

	LegWaveUpdate();

	m_pRightLegTrail->Update_Object(fTimeDelta);

	m_pDust_0->Update_Object(fTimeDelta);

	for (int i = 0; i < 5; ++i)
		m_pDust_1[i]->Update_Object(fTimeDelta);

	m_pLegWave->Update_Object(fTimeDelta);
}

void CLFoot::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pRightLegTrail->Render_Object();

	m_pDust_0->Render_Object();

	for (int i = 0; i < 5; ++i)
		m_pDust_1[i]->Render_Object();

	m_pLegWave->Render_Object();
}

HRESULT CLFoot::Ready_Object(void)
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

	for (int i = 0; i < 5; ++i)
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

	m_pLegWave = CWave::Create(m_pGraphicDev, L"LFoot_Leg_Wave");
	m_pLegWave->SetAni(FALSE);
	m_pLegWave->SetPattern(CWave::PATTERN_END);
	m_pLegWave->SetBilBoardCheck(FALSE);
	m_pLegWave->SetRendererCheck(TRUE);
	m_pLegWave->GetParticleInfo()->m_iPath = 2;	
	m_pLegWave->GetParticleInfo()->m_iTextureNumber = 3;
	Color = _vec4(0.5450f, 0.0f, 0.0f, 1.0f);
	m_pLegWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLegWave->SetBlurCheck(true);
	m_pLegWave->SetBlurPass(2);
	m_pLegWave->SetBlurCnt(2);
	m_pLegWave->SetBlurPower(1.2f);
	m_pLegWave->SetBlurValue(128.0f);
	m_pLegWave->SetBlurType(Engine::BLUR_CROSS);

	return S_OK;
}

CLFoot * CLFoot::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CLFoot*		pInstance = new CLFoot(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("LFoot Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLFoot::Free(void)
{
	::Safe_Release(m_pRightLegTrail);
	::Safe_Release(m_pDust_0);
	::Safe_Release(m_pLegWave);

	for (int i = 0; i < 5; ++i)
		::Safe_Release(m_pDust_1[i]);

	CEffect::Free();
}

HRESULT CLFoot::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CLFoot::SkillStart(void)
{
}

void CLFoot::EndAniCheck(void)
{
}

HRESULT CLFoot::Start_ForScene(void)
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

	m_pRightLegMat = m_pTargetMesh->Get_FrameByName("_55");

	const _matrix* pHammerStart = m_pTargetMesh->Get_FrameByName("_55");
	const _matrix* pHammerEnd = m_pTargetMesh->Get_FrameByName("_52");

	m_pRightLegTrail = CPhase2_Trail::Create(m_pGraphicDev, L"LFoot_Trail");
	m_pRightLegTrail->SetAni(FALSE);
	m_pRightLegTrail->SetTargetMatrix(pHammerStart, pHammerEnd);
	m_pRightLegTrail->SetTargetWorldMat(m_pWorldMat);
	m_pRightLegTrail->SetTextureNum(23);
	m_pRightLegTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	::Safe_Release(pGameObject);

	return S_OK;
}

void CLFoot::Dust_0_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_0->SetAni(TRUE);
	m_pDust_0->GetTransCom()->Set_Position(&vPos);
}

void CLFoot::Dust_1_Start(int iIndex)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y = m_pBardiel->GetTransCom()->Get_Now_Position().y;

	m_pDust_1[iIndex]->SetAni(TRUE);
	m_pDust_1[iIndex]->GetTransCom()->Set_Position(&vPos);
}

void CLFoot::LegWaveStart(void)
{
	_vec4 Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y += 1.5f;

	m_pLegWave->SetAni(TRUE);
	m_pLegWave->GetTransCom()->Set_Position(&vPos);
	m_pLegWave->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	m_pLegWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLegWave->GetTransCom()->Set_Angle(&vAngle);
}

void CLFoot::LegWaveUpdate(void)
{
	if(m_pLegWave->GetAni())
	{
		_vec4 vColor = (_vec4)m_pLegWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pLegWave->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.01f;
			m_pLegWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pLegWave->GetTransCom()->Add_Scale(0.4f);
		}
	}
}
