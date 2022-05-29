#include "stdafx.h"
#include "Penalty.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "PenaltyTrail.h"

CPenalty::CPenalty(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pLeftHand_Trail(NULL)
	, m_pRightHand_Trail(NULL)
	, m_pWorldMat(NULL)
	, m_pRightHandFingerMat(NULL)
	, m_pLeftHandFingerMat(NULL)
	, m_pRightElbowMat(NULL)
	, m_pLeftElbowMat(NULL)
	, m_pRightLastStepDust(NULL)
	, m_pWave_0(NULL)
	, m_pWave_1(NULL)
	, m_pWave_2(NULL)
	, m_pBardiel(NULL)
	, m_bRightHand_Trail_End(FALSE)
	, m_bRightHand_Trail(FALSE)
	, m_bLeftHand_Trail_End(FALSE)
	, m_bLeftHand_Trail(FALSE)
	, m_bSmash_End(FALSE)
	, m_bSmash(FALSE)
	, m_bLastDustStep(FALSE)
	, m_bLegDust_End(FALSE)
	, m_bLegDust_Tow_End(FALSE)
	, m_bSmashWave(FALSE)
	, m_vBulletPos(0.0f, 0.0f, 0.0f)
	, m_bBulletSet(FALSE)
{
	m_bAni = false;

	ZeroMemory(&m_pLeftLegDust, sizeof(CSprite*) * 10);
	ZeroMemory(&m_pRightLegDust, sizeof(CSprite*) * 10);
	ZeroMemory(&m_pLeftLegDustTwo, sizeof(CSprite*) * 5);
	ZeroMemory(&m_pRightLegDustTwo, sizeof(CSprite*) * 5);
	ZeroMemory(&m_pShotGunFireWave, sizeof(CWave*) * 30);

	for (int i = 0; i < 10; ++i)
		m_bLegDust[i] = FALSE;

	for (int i = 0; i < 5; ++i)
		m_bLegDust_Two[i] = FALSE;

	for (int i = 0; i < 30; ++i)
		m_bShotGunFireWave[i] = FALSE;
}

CPenalty::~CPenalty(void)
{
}

void CPenalty::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::CHANGEPHASE2)
	{
		m_bAni = false;
	}

	if(m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::PENALTY)
	{
		m_bAni = true;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//=============Trail==============================

		if (dTargetTime >= 2.170333f && (!m_bRightHand_Trail))
		{
			m_bRightHand_Trail = TRUE;
			m_pRightHand_Trail->SetAni(TRUE);
			m_pRightHand_Trail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pRightHand_Trail->SettingTrail();
		}

		if (dTargetTime >= 3.0639999f && (!m_bRightHand_Trail_End))
		{
			m_bRightHand_Trail_End = TRUE;
			m_pRightHand_Trail->SetAni(FALSE);
		}

		if (dTargetTime >= 4.085333f && (!m_bLeftHand_Trail))
		{
			m_bLeftHand_Trail = TRUE;
			m_pLeftHand_Trail->SetAni(TRUE);
			m_pLeftHand_Trail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pLeftHand_Trail->SettingTrail();
		}

		if (dTargetTime >= 4.8513332f && (!m_bLeftHand_Trail_End))
		{
			m_bLeftHand_Trail_End = TRUE;
			m_pLeftHand_Trail->SetAni(FALSE);
		}

		if (dTargetTime >= 7.9153332f && (!m_bSmash))
		{
			m_bSmash = TRUE;
			m_pRightHand_Trail->SetAni(TRUE);
			m_pRightHand_Trail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pRightHand_Trail->SettingTrail();

			m_pLeftHand_Trail->SetAni(TRUE);
			m_pLeftHand_Trail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pLeftHand_Trail->SettingTrail();
		}

		if (dTargetTime >= 8.298332f && (!m_bSmash_End))
		{
			m_bSmash_End = TRUE;
			m_pRightHand_Trail->SetAni(FALSE);
			m_pLeftHand_Trail->SetAni(FALSE);
		}

		//======================Dust===========================
		for (int i = 0; i < 10; ++i)
		{
			if (dTargetTime >= 2.425666f + (0.0638333f * i) && (!m_bLegDust[i]))
			{
				m_bLegDust[i] = TRUE;
				RightLeft_Leg_Dust_Start(i);
			}						
		}

		for (int i = 0; i < 5; ++i)
		{
			if (dTargetTime >= 4.085333f + (0.1021332f * i) && (!m_bLegDust_Two[i]))
			{
				m_bLegDust_Two[i] = TRUE;
				RightLeft_Leg_Dust_Two_Start(i);
			}
		}

		if (dTargetTime >= 11.617666f && (!m_bLastDustStep))
		{
			m_bLastDustStep = TRUE;
			RightLastStep_Dust_Start();
		}

		//========================Wave=========================
		if (dTargetTime >= 8.170665f && (!m_bSmashWave))
		{
			m_bSmashWave = TRUE;
			SmashWaveStart();
		}

		//===========================ShotGunFire================
		if (dTargetTime >= 10.70f && (!m_bBulletSet))
		{
			m_bBulletSet = TRUE;
			BulletPosStart();
		}

		for (int i = 0; i < 30; ++i)
		{
			if (dTargetTime >= 10.70f + (i * 0.1f) && (!m_bShotGunFireWave[i]))
			{
				m_bShotGunFireWave[i] = TRUE;
				ShotGunFireWaveStart(i);
			}
		}

		if (dTargetTime >= 8.336485f && dTargetTime < 8.4f)
		{
			Engine::SetRadialBlurPower(-0.2f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 8.4f && dTargetTime < 8.45f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bRightHand_Trail = FALSE;
		m_bRightHand_Trail_End = FALSE;
		m_bLeftHand_Trail = FALSE;
		m_bLeftHand_Trail_End = FALSE;
		m_bSmash = FALSE;
		m_bSmash_End = FALSE;
		m_bLastDustStep = FALSE;
		m_bLegDust_End = FALSE;
		m_bLegDust_Tow_End = FALSE;
		m_bSmashWave = FALSE;
		m_bBulletSet = FALSE;

		for (int i = 0; i < 10; ++i)
			m_bLegDust[i] = FALSE;

		for (int i = 0; i < 5; ++i)
			m_bLegDust_Two[i] = FALSE;

		for (int i = 0; i < 30; ++i)
			m_bShotGunFireWave[i] = FALSE;
	}


	if (!m_bAni)
		return;

	SmashwaveUpdate();
	BulletPosUpdate();
	ShotGunFireWaveUpdate();

	m_pLeftHand_Trail->Update_Object(fTimeDelta);
	m_pRightHand_Trail->Update_Object(fTimeDelta);
	m_pRightLastStepDust->Update_Object(fTimeDelta);

	m_pWave_0->Update_Object(fTimeDelta);
	m_pWave_1->Update_Object(fTimeDelta);
	m_pWave_2->Update_Object(fTimeDelta);

	for (int i = 0; i < 10; ++i)
		m_pLeftLegDust[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 10; ++i)
		m_pRightLegDust[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 5; ++i)
		m_pLeftLegDustTwo[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 5; ++i)
		m_pRightLegDustTwo[i]->Update_Object(fTimeDelta);

	for (int i = 0; i < 30; ++i)
		m_pShotGunFireWave[i]->Update_Object(fTimeDelta);
}

void CPenalty::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	if (!m_bAni)
		return;

	m_pLeftHand_Trail->Render_Object();
	m_pRightHand_Trail->Render_Object();

	m_pRightLastStepDust->Render_Object();

	m_pWave_0->Render_Object();
	m_pWave_1->Render_Object();
	m_pWave_2->Render_Object();
	
	for (int i = 0; i < 10; ++i)
		m_pLeftLegDust[i]->Render_Object();

	for (int i = 0; i < 10; ++i)
		m_pRightLegDust[i]->Render_Object();

	for (int i = 0; i < 5; ++i)
		m_pLeftLegDustTwo[i]->Render_Object();

	for (int i = 0; i < 5; ++i)
		m_pRightLegDustTwo[i]->Render_Object();

	for (int i = 0; i < 30; ++i)
		m_pShotGunFireWave[i]->Render_Object();
}

HRESULT CPenalty::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pRightLastStepDust = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
	m_pRightLastStepDust->SetAni(FALSE);
	_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
	m_pRightLastStepDust->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRightLastStepDust->GetParticleInfo()->m_iPath = 3;
	m_pRightLastStepDust->SetRenderCheck(TRUE);
	m_pRightLastStepDust->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pRightLastStepDust->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 25.0f;
	m_pRightLastStepDust->GetParticleInfo()->m_bBillBoard = TRUE;
	m_pRightLastStepDust->SetBlurCheck(TRUE);
	m_pRightLastStepDust->SetBlurCnt(2);
	m_pRightLastStepDust->SetBlurPass(3);
	m_pRightLastStepDust->SetBlurPower(1.1f);
	m_pRightLastStepDust->SetBlurType(Engine::BLUR_CROSS);
	m_pRightLastStepDust->SetBlurValue(128.0f);

	for (int i = 0; i < 10; ++i)
	{
		m_pLeftLegDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
		m_pLeftLegDust[i]->SetAni(FALSE);
		Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
		m_pLeftLegDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pLeftLegDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pLeftLegDust[i]->SetRenderCheck(TRUE);
		m_pLeftLegDust[i]->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
		m_pLeftLegDust[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 25.0f;
		m_pLeftLegDust[i]->GetParticleInfo()->m_bBillBoard = TRUE;
		m_pLeftLegDust[i]->GetParticleInfo()->m_bRoop = FALSE;
		m_pLeftLegDust[i]->SetBlurCheck(TRUE);
		m_pLeftLegDust[i]->SetBlurCnt(2);
		m_pLeftLegDust[i]->SetBlurPass(3);
		m_pLeftLegDust[i]->SetBlurPower(1.1f);
		m_pLeftLegDust[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pLeftLegDust[i]->SetBlurValue(128.0f);
	}

	for (int i = 0; i < 10; ++i)
	{
		m_pRightLegDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
		m_pRightLegDust[i]->SetAni(FALSE);
		Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
		m_pRightLegDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pRightLegDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pRightLegDust[i]->SetRenderCheck(TRUE);
		m_pRightLegDust[i]->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
		m_pRightLegDust[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 25.0f;
		m_pRightLegDust[i]->GetParticleInfo()->m_bBillBoard = TRUE;
		m_pRightLegDust[i]->GetParticleInfo()->m_bRoop = FALSE;
		m_pRightLegDust[i]->SetBlurCheck(TRUE);
		m_pRightLegDust[i]->SetBlurCnt(2);
		m_pRightLegDust[i]->SetBlurPass(3);
		m_pRightLegDust[i]->SetBlurPower(1.1f);
		m_pRightLegDust[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pRightLegDust[i]->SetBlurValue(128.0f);
	}

	for (int i = 0; i < 5; ++i)
	{
		m_pLeftLegDustTwo[i] = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
		m_pLeftLegDustTwo[i]->SetAni(FALSE);
		Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
		m_pLeftLegDustTwo[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pLeftLegDustTwo[i]->GetParticleInfo()->m_iPath = 3;
		m_pLeftLegDustTwo[i]->SetRenderCheck(TRUE);
		m_pLeftLegDustTwo[i]->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
		m_pLeftLegDustTwo[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 25.0f;
		m_pLeftLegDustTwo[i]->GetParticleInfo()->m_bBillBoard = TRUE;
		m_pLeftLegDustTwo[i]->GetParticleInfo()->m_bRoop = FALSE;
		m_pLeftLegDustTwo[i]->SetBlurCheck(TRUE);
		m_pLeftLegDustTwo[i]->SetBlurCnt(2);
		m_pLeftLegDustTwo[i]->SetBlurPass(3);
		m_pLeftLegDustTwo[i]->SetBlurPower(1.1f);
		m_pLeftLegDustTwo[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pLeftLegDustTwo[i]->SetBlurValue(128.0f);
	}

	for (int i = 0; i < 5; ++i)
	{
		m_pRightLegDustTwo[i] = CSprite::LoadCreate(m_pGraphicDev, L"PenaltyActionDust");
		m_pRightLegDustTwo[i]->SetAni(FALSE);
		Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
		m_pRightLegDustTwo[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pRightLegDustTwo[i]->GetParticleInfo()->m_iPath = 3;
		m_pRightLegDustTwo[i]->SetRenderCheck(TRUE);
		m_pRightLegDustTwo[i]->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
		m_pRightLegDustTwo[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 25.0f;
		m_pRightLegDustTwo[i]->GetParticleInfo()->m_bBillBoard = TRUE;
		m_pRightLegDustTwo[i]->GetParticleInfo()->m_bRoop = FALSE;
		m_pRightLegDustTwo[i]->SetBlurCheck(TRUE);
		m_pRightLegDustTwo[i]->SetBlurCnt(2);
		m_pRightLegDustTwo[i]->SetBlurPass(3);
		m_pRightLegDustTwo[i]->SetBlurPower(1.1f);
		m_pRightLegDustTwo[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pRightLegDustTwo[i]->SetBlurValue(128.0f);
	}

	m_pWave_0 = CWave::Create(m_pGraphicDev, L"Penaly_Smash_Wave_0");
	m_pWave_0->SetAni(FALSE);
	m_pWave_0->SetPattern(CWave::PATTERN_END);
	m_pWave_0->SetBilBoardCheck(FALSE);
	m_pWave_0->SetRendererCheck(TRUE);
	m_pWave_0->GetParticleInfo()->m_iPath = 2;
	m_pWave_0->GetParticleInfo()->m_iTextureNumber = 3;
	Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	m_pWave_0->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWave_0->SetBlurCheck(true);
	m_pWave_0->SetBlurPass(2);
	m_pWave_0->SetBlurCnt(2);
	m_pWave_0->SetBlurPower(1.1f);
	m_pWave_0->SetBlurValue(128.0f);
	m_pWave_0->SetBlurType(Engine::BLUR_CROSS);

	m_pWave_1 = CWave::Create(m_pGraphicDev, L"Penaly_Smash_Wave_1");
	m_pWave_1->SetAni(FALSE);
	m_pWave_1->SetPattern(CWave::PATTERN_END);
	m_pWave_1->SetBilBoardCheck(FALSE);
	m_pWave_1->SetRendererCheck(TRUE);
	m_pWave_1->GetParticleInfo()->m_iPath = 2;
	m_pWave_1->GetParticleInfo()->m_iTextureNumber = 35;
	Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	m_pWave_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWave_1->SetBlurCheck(true);
	m_pWave_1->SetBlurPass(2);
	m_pWave_1->SetBlurCnt(2);
	m_pWave_1->SetBlurPower(1.1f);
	m_pWave_1->SetBlurValue(128.0f);
	m_pWave_1->SetBlurType(Engine::BLUR_CROSS);

	m_pWave_2 = CWave::Create(m_pGraphicDev, L"Penaly_Smash_Wave_2");
	m_pWave_2->SetAni(FALSE);
	m_pWave_2->SetPattern(CWave::PATTERN_END);
	m_pWave_2->SetBilBoardCheck(FALSE);
	m_pWave_2->SetRendererCheck(TRUE);
	m_pWave_2->GetParticleInfo()->m_iPath = 2;
	m_pWave_2->GetParticleInfo()->m_iTextureNumber = 4;
	Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	m_pWave_2->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWave_2->SetBlurCheck(true);
	m_pWave_2->SetBlurPass(2);
	m_pWave_2->SetBlurCnt(2);
	m_pWave_2->SetBlurPower(1.1f);
	m_pWave_2->SetBlurValue(128.0f);
	m_pWave_2->SetBlurType(Engine::BLUR_CROSS);

	for (int i = 0; i < 30; ++i)
	{
		m_pShotGunFireWave[i] = CWave::Create(m_pGraphicDev, L"Penaly_ShotGunFire_Wave_2");
		m_pShotGunFireWave[i]->SetAni(FALSE);
		m_pShotGunFireWave[i]->SetPattern(CWave::PATTERN_END);
		m_pShotGunFireWave[i]->SetBilBoardCheck(FALSE);
		m_pShotGunFireWave[i]->SetRendererCheck(TRUE);
		m_pShotGunFireWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pShotGunFireWave[i]->GetParticleInfo()->m_bBillBoard = TRUE;

		if (0 <= i && i <= 4)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 0;
		else if (5 <= i && i <= 9)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 8;
		else if (10 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 2;
		else if (11 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 5;
		else if (12 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 6;
		else if (13 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 12;
		else if (14 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 20;
		else if (15 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 8;
		else if (16 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 21;
		else if (17 <= i || i <= 20)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 27;
		else if (21 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 43;
		else if (22 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 51;
		else if (23 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 52;
		else if (24 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 56;
		else if (25 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 57;
		else if (26 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 62;
		else if (27 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 64;
		else if (28 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 41;
		else if (29 == i)
			m_pShotGunFireWave[i]->GetParticleInfo()->m_iTextureNumber = 36;

		Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
		m_pShotGunFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pShotGunFireWave[i]->SetBlurCheck(true);
		m_pShotGunFireWave[i]->SetBlurPass(2);
		m_pShotGunFireWave[i]->SetBlurCnt(2);
		m_pShotGunFireWave[i]->SetBlurPower(1.2f);
		m_pShotGunFireWave[i]->SetBlurValue(128.0f);
		m_pShotGunFireWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}

	return S_OK;
}

CPenalty * CPenalty::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CPenalty*		pInstance = new CPenalty(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Penalty Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPenalty::Free(void)
{
	::Safe_Release(m_pRightHand_Trail);
	::Safe_Release(m_pLeftHand_Trail);
	::Safe_Release(m_pRightLastStepDust);

	::Safe_Release(m_pWave_0);
	::Safe_Release(m_pWave_1);
	::Safe_Release(m_pWave_2);


	for (int i = 0; i < 10; ++i)
		::Safe_Release(m_pLeftLegDust[i]);

	for (int i = 0; i < 10; ++i)
		::Safe_Release(m_pRightLegDust[i]);

	for (int i = 0; i < 5; ++i)
		::Safe_Release(m_pLeftLegDustTwo[i]);

	for (int i = 0; i < 5; ++i)
		::Safe_Release(m_pRightLegDustTwo[i]);

	for (int i = 0; i < 30; ++i)
		::Safe_Release(m_pShotGunFireWave[i]);

	CEffect::Free();
}

HRESULT CPenalty::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CPenalty::SkillStart(void)
{
}

void CPenalty::EndAniCheck(void)
{
}

HRESULT CPenalty::Start_ForScene(void)
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
	m_pRightHandFingerMat = m_pTargetMesh->Get_FrameByName("_25");
	m_pLeftHandFingerMat = m_pTargetMesh->Get_FrameByName("_41");
	m_pRightElbowMat = m_pTargetMesh->Get_FrameByName("_19");
	m_pLeftElbowMat = m_pTargetMesh->Get_FrameByName("_35");
	m_pLeftLegMat = m_pTargetMesh->Get_FrameByName("_59");
	m_pRightLegMat = m_pTargetMesh->Get_FrameByName("_55");

	m_pLeftHand_Trail = CPenaltyTrail::Create(m_pGraphicDev, L"Penalty_RightHand");
	m_pLeftHand_Trail->SetAni(FALSE);
	m_pLeftHand_Trail->SetTargetMatrix(m_pLeftHandFingerMat, m_pLeftElbowMat);
	m_pLeftHand_Trail->SetTargetWorldMat(m_pWorldMat);
	m_pLeftHand_Trail->SetTextureNum(3);
	m_pLeftHand_Trail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	m_pRightHand_Trail = CPenaltyTrail::Create(m_pGraphicDev, L"Penalty_LeftHand");
	m_pRightHand_Trail->SetAni(FALSE);
	m_pRightHand_Trail->SetTargetMatrix(m_pRightHandFingerMat, m_pRightElbowMat);
	m_pRightHand_Trail->SetTargetWorldMat(m_pWorldMat);
	m_pRightHand_Trail->SetTextureNum(3);
	m_pRightHand_Trail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	::Safe_Release(pGameObject);

	return S_OK;
}

void CPenalty::RightLeft_Leg_Dust_Start(int iIndex)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	//vPos.y += 53.0f;

	m_pLeftLegDust[iIndex]->SetAni(TRUE);
	m_pLeftLegDust[iIndex]->GetTransCom()->Set_Position(&vPos);

	vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	//vPos.y += 53.0f;

	m_pRightLegDust[iIndex]->SetAni(TRUE);
	m_pRightLegDust[iIndex]->GetTransCom()->Set_Position(&vPos);
}

void CPenalty::RightLeft_Leg_Dust_Update(void)
{
}

void CPenalty::RightLeft_Leg_Dust_Two_Start(int iIndex)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	//vPos.y += 53.0f;

	m_pLeftLegDustTwo[iIndex]->SetAni(TRUE);
	m_pLeftLegDustTwo[iIndex]->GetTransCom()->Set_Position(&vPos);

	vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	//vPos.y += 53.0f;

	m_pRightLegDustTwo[iIndex]->SetAni(TRUE);
	m_pRightLegDustTwo[iIndex]->GetTransCom()->Set_Position(&vPos);
}

void CPenalty::RightLastStep_Dust_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightLegMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	//vPos.y += 53.0f;

	m_pRightLastStepDust->SetAni(TRUE);
	m_pRightLastStepDust->GetTransCom()->Set_Position(&vPos);
}

void CPenalty::SmashWaveStart(void)
{
	_vec4 Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftHandFingerMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y -= 1.5f;

	m_pWave_0->SetAni(TRUE);
	m_pWave_0->GetTransCom()->Set_Position(&vPos);
	m_pWave_0->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pWave_0->GetTransCom()->Set_Angle(&vAngle);
	m_pWave_0->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	
	m_pWave_1->SetAni(TRUE);
	m_pWave_1->GetTransCom()->Set_Position(&vPos);
	m_pWave_1->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pWave_1->GetTransCom()->Set_Angle(&vAngle);
	m_pWave_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

	m_pWave_2->SetAni(TRUE);
	m_pWave_2->GetTransCom()->Set_Position(&vPos);
	m_pWave_2->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	m_pWave_2->GetTransCom()->Set_Angle(&vAngle);
	m_pWave_2->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
}

void CPenalty::SmashwaveUpdate(void)
{
	_vec4 vColor;

	if (m_pWave_0->GetAni())
	{
		vColor = (_vec4)m_pWave_0->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pWave_0->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.02f;
			m_pWave_0->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pWave_0->GetTransCom()->Add_Scale(0.2f);
		}
	}

	if (m_pWave_1->GetAni())
	{
		vColor = (_vec4)m_pWave_1->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pWave_1->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.02f;
			m_pWave_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pWave_1->GetTransCom()->Add_Scale(0.4f);
		}
	}

	if (m_pWave_2->GetAni())
	{
		vColor = (_vec4)m_pWave_2->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pWave_2->SetAni(FALSE);
		}
		else
		{
			vColor.w -= 0.02f;
			m_pWave_2->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pWave_2->GetTransCom()->Add_Scale(0.6f);
		}
	}
}

void CPenalty::ShotGunFireWaveStart(int iIndex)
{
	_vec4 Color = _vec4(1.0f, -0.3f, -0.9f, 1.f);

	_vec3 vWaveLook = _vec3(0.0f, 0.0f, 1.0f);
	_float Dot = D3DXVec3Dot(&vWaveLook, &m_pBardiel->GetTransCom()->Get_Now_Look());
	_float Radian = (_float)acos(Dot);
	Radian = D3DXToDegree(Radian);

	if (D3DXVec3Dot(&_vec3(-1.0f, 0.0f, 0.0f), &m_pBardiel->GetTransCom()->Get_Now_Look()) > 0.0f)
		Radian *= -1.0f;

	_vec3 Angle = _vec3(0.0f, Radian, 0.0f);

	m_pShotGunFireWave[iIndex]->SetAni(TRUE);
	m_pShotGunFireWave[iIndex]->GetTransCom()->Set_Position(&m_vBulletPos);
	m_pShotGunFireWave[iIndex]->GetTransCom()->Set_Angle(&Angle);
	m_pShotGunFireWave[iIndex]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	m_pShotGunFireWave[iIndex]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
}

void CPenalty::ShotGunFireWaveUpdate(void)
{
	for (int i = 0; i < 30; ++i)
	{
		if (m_pShotGunFireWave[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pShotGunFireWave[i]->GetParticleInfo()->m_dColor;
			if (vColor.w < 0.f)
			{
				m_pShotGunFireWave[i]->SetAni(false);
			}
			else
			{
				vColor.w -= 0.008f;
				m_pShotGunFireWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

				/*_vec3 vPos = m_pShotGunFireWave[i]->GetTransCom()->Get_Now_Position();
				_vec3 vDir = m_pBardiel->GetTransCom()->Get_Now_Look();

				vPos += (vDir * 1.005f);
				m_pShotGunFireWave[i]->GetTransCom()->Set_Position(&vPos);*/
				m_pShotGunFireWave[i]->GetTransCom()->Add_Scale(0.8f);
			}
		}
	}
}

void CPenalty::BulletPosStart(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pLeftHandFingerMat);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_vBulletPos = vPos;
}

void CPenalty::BulletPosUpdate(void)
{
	m_vBulletPos += (m_pBardiel->GetTransCom()->Get_Now_Look() * 6.0f);
}

