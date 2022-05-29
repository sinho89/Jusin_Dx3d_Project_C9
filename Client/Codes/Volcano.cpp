#include "stdafx.h"
#include "Volcano.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Wave.h"
#include "Sprite.h"
#include "Phase2_Trail.h"

CVolcano::CVolcano(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pvecAniInfo(NULL)
	, m_pTargetMesh(NULL)
	, m_pTargetTransform(NULL)
	, m_pTargetAnimator(NULL)
	, m_pHammerTrail(NULL)
	, m_pWorldMat(NULL)
	, m_pHammerEnd(NULL)
	, m_pRightLeg(NULL)	
	, m_pDust_1(NULL)
	, m_pDust_2(NULL)
	, m_bDust_1(FALSE)
	, m_bDust_2(FALSE)
	, m_bFirstSwing_Start(FALSE)
	, m_bFirstSwing_End(FALSE)
	, m_bSecondSwing_Start(FALSE)
	, m_bSecondSwing_End(FALSE)
	, m_bHammerWave(FALSE)
	, m_dDust_0Timer(0)
{
	ZeroMemory(&m_pDust_0, sizeof(CSprite*) * 5);
	ZeroMemory(&m_pHammerWave, sizeof(CWave*) * 3);
	ZeroMemory(&m_pBombFrameSprite, sizeof(CSprite*) * 2);
}

CVolcano::~CVolcano(void)
{
}

void CVolcano::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::ATT_VOLCANO)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//Trail----------------------------------------------
		if (dTargetTime >= 0.3383333f && (!m_bFirstSwing_Start))
		{
			m_bFirstSwing_Start = TRUE;
			m_pHammerTrail->SetAni(TRUE);
			m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pHammerTrail->SettingTrail();
		}

		if (dTargetTime >= 1.8270001f && (!m_bFirstSwing_End))
		{
			_vec4 vColor = m_pHammerTrail->GetColor();
			vColor.w -= 0.095f;
			m_pHammerTrail->SetColor(vColor);

			if (m_pHammerTrail->GetColor().w <= 0.0f)
			{
				m_pHammerTrail->SetAni(FALSE);
				m_bFirstSwing_End = TRUE;
			}
		}

		if (dTargetTime >= 3.1126668f && (!m_bSecondSwing_Start))
		{
			m_bSecondSwing_Start = TRUE;
			m_pHammerTrail->SetAni(TRUE);
			m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));
			m_pHammerTrail->SettingTrail();
		}

		if (dTargetTime >= 3.8570001f && (!m_bSecondSwing_End))
		{
			_vec4 vColor = m_pHammerTrail->GetColor();
			vColor.w -= 0.095f;
			m_pHammerTrail->SetColor(vColor);

			if (m_pHammerTrail->GetColor().w <= 0.0f)
			{
				m_pHammerTrail->SetAni(FALSE);
				m_bSecondSwing_End = TRUE;
			}
		}
		//--------------------------------------------------------------

		//Dust-----------------------------------------------------------
		if((dTargetTime >= 0.5413334f) && (dTargetTime  <= 1.08f))
		{
			m_dDust_0Timer += fTimeDelta;

			if (m_dDust_0Timer >= 0.06f)
			{
				m_dDust_0Timer = 0;

				for (int i = 0; i < 5; ++i)
				{
					//망치 휘두를 때
					if (m_pDust_0[i]->GetAni())
						continue;

					Dust_0_Start(i);
					break;
				}
			}
		}
		//==============================================
		

		if (dTargetTime >= 3.5186668f && (!m_bDust_1))
		{
			m_bDust_1 = TRUE;
			Dust_1_Start();
		}

		if (dTargetTime >= 6.0223335f && (!m_bDust_2))
		{
			m_bDust_2 = TRUE;
			Dust_2_Start();
		}
		//----------------------------------------------------------------

		//Wave------------------------------------------------------------
		if (dTargetTime >= 3.5186668f && (!m_bHammerWave))
		{
			m_bHammerWave = TRUE;
			HammerWaveStart();
		}
		//----------------------------------------------------------------

		if (dTargetTime >= 3.518668f && dTargetTime < 3.6f)
		{
			Engine::SetRadialBlurPower(-0.2f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 3.6f && dTargetTime < 3.7f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}
	}
	else
	{
		m_bFirstSwing_Start = FALSE;
		m_bFirstSwing_End = FALSE;
		m_bSecondSwing_Start = FALSE;
		m_bSecondSwing_End = FALSE;

		m_bDust_1 = FALSE;
		m_bDust_2 = FALSE;

		m_bHammerWave = FALSE;

	}
	//Wave Update
	HammerWaveUpdate();

	Dust_0_Update();

	m_pHammerTrail->Update_Object(fTimeDelta);

	for (int i = 0; i < 5; ++i)
		m_pDust_0[i]->Update_Object(fTimeDelta);

	m_pDust_1->Update_Object(fTimeDelta);
	m_pDust_2->Update_Object(fTimeDelta);

	//충격파
	for (int i = 0; i < 3; ++i)
	{
		m_pHammerWave[i]->Update_Object(fTimeDelta);
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pBombFrameSprite[i]->Update_Object(fTimeDelta);
	}
}

void CVolcano::Render_Object(void)
{
	if (g_bBardielDead)
		return;

	m_pHammerTrail->Render_Object();

	for (int i = 0; i < 5; ++i)
		m_pDust_0[i]->Render_Object();

	m_pDust_1->Render_Object();
	m_pDust_2->Render_Object();


	//충격파
	for (int i = 0; i < 3; ++i)
	{
		m_pHammerWave[i]->Render_Object();
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pBombFrameSprite[i]->Render_Object();
	}
}

HRESULT CVolcano::Ready_Object(void)
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
		m_pDust_0[i]->SetBlurCheck(true);
		m_pDust_0[i]->SetBlurCnt(2);
		m_pDust_0[i]->SetBlurPass(3);
		m_pDust_0[i]->SetBlurPower(1.1f);
		m_pDust_0[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pDust_0[i]->SetBlurValue(128.0f);
	}

	//망치 찍었을때===========================================================
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
	//========================================================================

	//오른발 끝============================================================
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
	//=======================================================================

	//충격파 Wave=============================================================
	for (int i = 0; i < 3; ++i)
	{
		m_pHammerWave[i] = CWave::Create(m_pGraphicDev, L"Volcano_Hammer_Wave");
		m_pHammerWave[i]->SetAni(false);
		m_pHammerWave[i]->SetPattern(CWave::PATTERN_END);
		m_pHammerWave[i]->SetBilBoardCheck(false);
		m_pHammerWave[i]->SetRendererCheck(true);
		m_pHammerWave[i]->GetParticleInfo()->m_iPath = 3;

		if (0 == i)
			m_pHammerWave[i]->GetParticleInfo()->m_iTextureNumber = 3;
		else if (1 == i)
			m_pHammerWave[i]->GetParticleInfo()->m_iTextureNumber = 3;
		else if (2 == i)
			m_pHammerWave[i]->GetParticleInfo()->m_iTextureNumber = 7;

		D3DXCOLOR vColor = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
		m_pHammerWave[i]->GetParticleInfo()->m_dColor = vColor;
		m_pHammerWave[i]->SetBlurCheck(true);
		m_pHammerWave[i]->SetBlurPass(2);
		m_pHammerWave[i]->SetBlurCnt(2);
		m_pHammerWave[i]->SetBlurPower(1.5f);
		m_pHammerWave[i]->SetBlurValue(100.0f);
		m_pHammerWave[i]->SetBlurType(Engine::BLUR_CROSS);
	}
	//=========================================================================
	for (int i = 0; i < 2; ++i)
	{
		m_pBombFrameSprite[i] = CSprite::LoadCreate(m_pGraphicDev, L"VolcanoFrame");
		m_pBombFrameSprite[i]->SetAni(false);
		m_pBombFrameSprite[i]->SetRenderCheck(true);
		m_pBombFrameSprite[i]->GetTransCom()->Set_Scale(8.0f, 20.0f, 8.0f);
		m_pBombFrameSprite[i]->SetBlurCheck(true);
		m_pBombFrameSprite[i]->SetBlurCnt(2);
		m_pBombFrameSprite[i]->SetBlurPass(2);
		m_pBombFrameSprite[i]->SetBlurPower(1.5f);
		m_pBombFrameSprite[i]->SetBlurType(Engine::BLUR_VERTICAL);
		m_pBombFrameSprite[i]->SetBlurValue(100.0f);
	}
	return S_OK;
}

CVolcano * CVolcano::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CVolcano*		pInstance = new CVolcano(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Volcano Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CVolcano::Free(void)
{
	::Safe_Release(m_pHammerTrail);

	::Safe_Release(m_pDust_1);
	::Safe_Release(m_pDust_2);

	for (int i = 0; i < 5; ++i)
		::Safe_Release(m_pDust_0[i]);

	for (int i = 0; i < 3; ++i)
	{
		::Safe_Release(m_pHammerWave[i]);
	}

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pBombFrameSprite[i]);
	}

	CEffect::Free();
}

HRESULT CVolcano::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CVolcano::SkillStart(void)
{
}

void CVolcano::EndAniCheck(void)
{
}

HRESULT CVolcano::Start_ForScene(void)
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

	m_pRightLeg = m_pTargetMesh->Get_FrameByName("_55");
	m_pHammerEnd = m_pTargetMesh->Get_FrameByName("_63");
	const _matrix* pHammerEnd = m_pTargetMesh->Get_FrameByName("_62");

	m_pHammerTrail = CPhase2_Trail::Create(m_pGraphicDev, L"Volcano_Trail");
	m_pHammerTrail->SetAni(FALSE);
	m_pHammerTrail->SetTargetMatrix(m_pHammerEnd, pHammerEnd);
	m_pHammerTrail->SetTargetWorldMat(m_pWorldMat);
	m_pHammerTrail->SetTextureNum(23);
	m_pHammerTrail->SetColor(_vec4(0.5450f, 0.0f, 0.0f, 1.0f));

	::Safe_Release(pGameObject);

	return S_OK;
}

void CVolcano::Dust_0_Start(_int iIndex)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y = m_pBardiel->GetTransCom()->Get_Now_Position().y;

	m_pDust_0[iIndex]->SetAni(TRUE);
	m_pDust_0[iIndex]->GetTransCom()->Set_Position(&vPos);
	m_pDust_0[iIndex]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
}

void CVolcano::Dust_1_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_1->SetAni(TRUE);
	m_pDust_1->GetTransCom()->Set_Position(&vPos);
}

void CVolcano::Dust_2_Start(void)
{
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pRightLeg);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	m_pDust_2->SetAni(TRUE);
	m_pDust_2->GetTransCom()->Set_Position(&vPos);
}

void CVolcano::HammerWaveStart(void)
{
	_vec4 Color = _vec4(1.0f, -0.3f, -0.9f, 1.0f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec3 vPos = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vPos, &vPos, m_pHammerEnd);
	D3DXVec3TransformCoord(&vPos, &vPos, m_pWorldMat);

	vPos.y += 3.f;

	for (int i = 0; i < 3; ++i)
	{
		m_pHammerWave[i]->SetAni(true);
		m_pHammerWave[i]->GetTransCom()->Set_Position(&vPos);
		m_pHammerWave[i]->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
		m_pHammerWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pHammerWave[i]->GetTransCom()->Set_Angle(&vAngle);
	}


	vPos.y += 50.f;
	for (int i = 0; i < 2; ++i)
	{
		m_pBombFrameSprite[i]->SetAni(true);
		m_pBombFrameSprite[i]->GetTransCom()->Set_Position(&vPos);
	}

}

void CVolcano::HammerWaveUpdate(void)
{
	for (int i = 0; i < 3; ++i)
	{
		if(m_pHammerWave[i]->GetAni())
		{
			_vec4 vColor = (_vec4)m_pHammerWave[i]->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pHammerWave[i]->SetAni(false);
			}
			else
			{
				vColor.w -= 0.03f;
				m_pHammerWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

				if(0 == i)
					m_pHammerWave[i]->GetTransCom()->Add_Scale(0.6f);
				else if(1 == i)
					m_pHammerWave[i]->GetTransCom()->Add_Scale(0.3f);
				else if(2 == i)
					m_pHammerWave[i]->GetTransCom()->Add_Scale(0.2f);
			}
		}
	}
}

void CVolcano::Dust_0_Update(void)
{
	for (int i = 0; i < 5; ++i)
	{
		if (!m_pDust_0[i]->GetAni())
			continue;

		m_pDust_0[i]->GetTransCom()->Add_Scale(0.1f);
	}
}
