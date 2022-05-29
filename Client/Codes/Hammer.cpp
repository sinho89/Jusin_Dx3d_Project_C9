#include "stdafx.h"
#include "Hammer.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"

CHammer::CHammer(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pWorldMat(NULL)
, m_pDust_0(NULL)
, m_pDust_1(NULL)
, m_pDust_2(NULL)
, m_pDust_3(NULL)
, m_bDust_0(FALSE)
, m_bDust_1(FALSE)
, m_bDust_2(FALSE)
, m_bDust_3(FALSE)
{
	ZeroMemory(&m_pHammerFireWave, sizeof(CWave*) * 4);
	ZeroMemory(&m_pBigRune, sizeof(CWave*) * 4);
	ZeroMemory(&m_pSmallRune, sizeof(CWave*) * 4);
	ZeroMemory(&m_pBombFrameSprite, sizeof(CSprite*) * 2);

	for (int i = 0; i < 4; ++i)
		m_bHammerFireWave[i] = FALSE;
}

CHammer::~CHammer(void)
{
}

void CHammer::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::ATT_HAMMER)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= 2.592f && (!m_bDust_0))
		{
			m_bDust_0 = TRUE;
			Dust_0_Start();
		}

		if (dTargetTime >= 4.428f && (!m_bDust_1))
		{
			m_bDust_1 = TRUE;
			Dust_1_Start();
		}

		if (dTargetTime >= 6.048f && (!m_bDust_2))
		{
			m_bDust_2 = TRUE;
			Dust_2_Start();
		}

		if (dTargetTime >= 7.992f && (!m_bDust_3))
		{
			m_bDust_3 = TRUE;
			Dust_3_Start();
		}

		//Wave------------------------------------------------------------
		if (dTargetTime >= 2.592f && (!m_bHammerFireWave[0]))
		{
			m_bHammerFireWave[0] = TRUE;
			HammerFireWaveStart(0);
		}

		if (dTargetTime >= 4.4280001f && (!m_bHammerFireWave[1]))
		{
			m_bHammerFireWave[1] = TRUE;
			HammerFireWaveStart(1);
		}

		if (dTargetTime >= 6.1560001f && (!m_bHammerFireWave[2]))
		{
			m_bHammerFireWave[2] = TRUE;
			HammerFireWaveStart(2);
		}

		if (dTargetTime >= 7.9920001f && (!m_bHammerFireWave[3]))
		{
			m_bHammerFireWave[3] = TRUE;
			HammerFireWaveStart(3);
		}
		//----------------------------------------------------------------
		if (dTargetTime >= 2.592f && dTargetTime < 2.6f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 2.6f && dTargetTime < 2.65f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}

		if (dTargetTime >= 4.428f && dTargetTime < 4.5f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 4.5f && dTargetTime < 4.55f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}

		if (dTargetTime >= 6.156f && dTargetTime < 6.2f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 6.2f && dTargetTime < 6.25f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}

		if (dTargetTime >= 7.992f && dTargetTime < 8.05f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 8.05f && dTargetTime < 8.1f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bDust_0 = FALSE;
		m_bDust_1 = FALSE;
		m_bDust_2 = FALSE;
		m_bDust_3 = FALSE;

		for (int i = 0; i < 4; ++i)
			m_bHammerFireWave[i] = FALSE;
	}

	HammerFireWaveUpdate();

	RuneUpdate();

	m_pDust_0->Update_Object(fTimeDelta);
	m_pDust_1->Update_Object(fTimeDelta);
	m_pDust_2->Update_Object(fTimeDelta);
	m_pDust_3->Update_Object(fTimeDelta);

	for (int i = 0; i < 4; ++i)
	{
		m_pHammerFireWave[i]->Update_Object(fTimeDelta);

		m_pBigRune[i]->Update_Object(fTimeDelta);
		m_pSmallRune[i]->Update_Object(fTimeDelta);
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pBombFrameSprite[i]->Update_Object(fTimeDelta);
	}
}

void CHammer::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pDust_0->Render_Object();
	m_pDust_1->Render_Object();
	m_pDust_2->Render_Object();
	m_pDust_3->Render_Object();

	for (int i = 0; i < 4; ++i)
	{
		m_pHammerFireWave[i]->Render_Object();

		m_pBigRune[i]->Render_Object();
		m_pSmallRune[i]->Render_Object();
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pBombFrameSprite[i]->Render_Object();
	}
}

HRESULT CHammer::Ready_Object(void)
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

	for (int i = 0; i < 4; ++i)
	{
		m_pHammerFireWave[i] = CWave::Create(m_pGraphicDev, L"Hammer_Fire_Wave");
		m_pHammerFireWave[i]->SetAni(FALSE);
		m_pHammerFireWave[i]->SetPattern(CWave::PATTERN_END);
		m_pHammerFireWave[i]->SetBilBoardCheck(FALSE);
		m_pHammerFireWave[i]->SetRendererCheck(TRUE);
		m_pHammerFireWave[i]->GetParticleInfo()->m_iTextureNumber = 7;
		m_pHammerFireWave[i]->GetParticleInfo()->m_iPath = 2;
		Color = _vec4(1.0f, -0.3f, -0.9f, 2.0f);
		m_pHammerFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pHammerFireWave[i]->SetBlurCheck(true);
		m_pHammerFireWave[i]->SetBlurPass(2);
		m_pHammerFireWave[i]->SetBlurCnt(2);
		m_pHammerFireWave[i]->SetBlurPower(1.2f);
		m_pHammerFireWave[i]->SetBlurValue(128.0f);
		m_pHammerFireWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);

	for (int i = 0; i < 4; ++i)
	{

		m_pBigRune[i] = CWave::Create(m_pGraphicDev, L"Rune_Wave");
		m_pBigRune[i]->SetAni(false);
		m_pBigRune[i]->SetPattern(CWave::PATTERN_END);
		m_pBigRune[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pBigRune[i]->SetBilBoardCheck(false);
		m_pBigRune[i]->SetRendererCheck(true);
		m_pBigRune[i]->GetParticleInfo()->m_iTextureNumber = 70;
		m_pBigRune[i]->GetParticleInfo()->m_iPath = 0;


		m_pSmallRune[i] = CWave::Create(m_pGraphicDev, L"SmallRune_Wave");
		m_pSmallRune[i]->SetAni(false);
		m_pSmallRune[i]->SetPattern(CWave::PATTERN_END);
		m_pSmallRune[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pSmallRune[i]->SetBilBoardCheck(false);
		m_pSmallRune[i]->SetRendererCheck(true);
		m_pSmallRune[i]->GetParticleInfo()->m_iTextureNumber = 71;
		m_pSmallRune[i]->GetParticleInfo()->m_iPath = 0;
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pBombFrameSprite[i] = CSprite::LoadCreate(m_pGraphicDev, L"VolcanoFrame");
		m_pBombFrameSprite[i]->SetAni(false);
		m_pBombFrameSprite[i]->SetRenderCheck(true);
		m_pBombFrameSprite[i]->GetTransCom()->Set_Scale(8.0f, 18.0f, 8.0f);
		m_pBombFrameSprite[i]->SetBlurCheck(true);
		m_pBombFrameSprite[i]->SetBlurCnt(2);
		m_pBombFrameSprite[i]->SetBlurPass(2);
		m_pBombFrameSprite[i]->SetBlurPower(1.5f);
		m_pBombFrameSprite[i]->SetBlurType(Engine::BLUR_VERTICAL);
		m_pBombFrameSprite[i]->SetBlurValue(100.0f);
	}
	return S_OK;
}

CHammer * CHammer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHammer*		pInstance = new CHammer(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Hammer Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHammer::Free(void)
{
	::Safe_Release(m_pDust_0);
	::Safe_Release(m_pDust_1);
	::Safe_Release(m_pDust_2);
	::Safe_Release(m_pDust_3);

	for (int i = 0; i < 4; ++i)
	{
		::Safe_Release(m_pHammerFireWave[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		Engine::Safe_Release(m_pBigRune[i]);
		Engine::Safe_Release(m_pSmallRune[i]);
	}

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pBombFrameSprite[i]);
	}

	CEffect::Free();
}

HRESULT CHammer::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CHammer::SkillStart(void)
{
}

void CHammer::EndAniCheck(void)
{
}

HRESULT CHammer::Start_ForScene(void)
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
	m_pHammerEnd = m_pTargetMesh->Get_FrameByName("_63");

	::Safe_Release(pGameObject);

	return S_OK;
}

void CHammer::Dust_0_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_0->SetAni(TRUE);
	m_pDust_0->GetTransCom()->Set_Position(&vPos);
}

void CHammer::Dust_1_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_1->SetAni(TRUE);
	m_pDust_1->GetTransCom()->Set_Position(&vPos);
}

void CHammer::Dust_2_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_2->SetAni(TRUE);
	m_pDust_2->GetTransCom()->Set_Position(&vPos);
}

void CHammer::Dust_3_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_3->SetAni(TRUE);
	m_pDust_3->GetTransCom()->Set_Position(&vPos);
}

void CHammer::HammerFireWaveStart(int iIndex)
{
	_vec4 Color = _vec4(1.0f, -0.3f, -0.9f, 2.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);
	
	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y += 0.5f;

	m_pHammerFireWave[iIndex]->SetAni(TRUE);
	m_pHammerFireWave[iIndex]->GetTransCom()->Set_Position(&vPos);
	m_pHammerFireWave[iIndex]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	m_pHammerFireWave[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pHammerFireWave[iIndex]->GetTransCom()->Set_Angle(&vAngle);


	Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	m_pBigRune[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pBigRune[iIndex]->SetAni(true);
	m_pBigRune[iIndex]->GetTransCom()->Set_Position(&vPos);
	m_pBigRune[iIndex]->GetTransCom()->Set_Angle(&vAngle);
	m_pBigRune[iIndex]->GetTransCom()->Set_Scale(5.0f, 5.0f, 5.0f);

	vPos.y += 2.f;
	m_pSmallRune[iIndex]->SetAni(true);
	m_pSmallRune[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pSmallRune[iIndex]->GetTransCom()->Set_Position(&vPos);
	m_pSmallRune[iIndex]->GetTransCom()->Set_Angle(&vAngle);
	m_pSmallRune[iIndex]->GetTransCom()->Set_Scale(4.0f, 4.0f, 4.0f);


	vPos.y += 50.f;
	for (int i = 0; i < 2; ++i)
	{
		m_pBombFrameSprite[i]->SetAni(true);
		m_pBombFrameSprite[i]->GetTransCom()->Set_Position(&vPos);
	}
}

void CHammer::HammerFireWaveUpdate(void)
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_pHammerFireWave[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pHammerFireWave[i]->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pHammerFireWave[i]->SetAni(FALSE);
			}
			else
			{
				vColor.w -= 0.04f;
				m_pHammerFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
				m_pHammerFireWave[i]->GetTransCom()->Add_Scale(0.3f);
			}
		}
	}
}

void CHammer::RuneUpdate(void)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!m_pBigRune[i]->GetAni())
			continue ;

		_vec4 vColor = (_vec4)m_pBigRune[i]->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pBigRune[i]->SetAni(false);
		}
		else
		{
			vColor.w -= 0.002f;
			m_pBigRune[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pBigRune[i]->GetTransCom()->Rotation(Engine::ROT_Y, 2.f);
		}
	}

	for (int i = 0; i < 4; ++i)
	{
		if (!m_pSmallRune[i]->GetAni())
			continue;

		_vec4 vColor = (_vec4)m_pSmallRune[i]->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pSmallRune[i]->SetAni(false);
		}
		else
		{
			vColor.w -= 0.002f;
			m_pSmallRune[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pSmallRune[i]->GetTransCom()->Rotation(Engine::ROT_Y, 2.f);
		}
	}
}
