#include "stdafx.h"
#include "VioletCross.h"
#include "PlayerCamera.h"
#include "WitchBlade.h"
#include "Wave.h"
#include "Sprite.h"

CVioletCross::CVioletCross(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pRune(NULL)
, m_pRuneOutLine(NULL)
, m_bSkillStart(false)
, m_pGroundWave(NULL)
, m_pSword1Transform(NULL)
, m_pSwordL1Transform(NULL)
, m_bPuppleSphereCheck(NULL)
, m_pSwordStar(NULL)
, m_pSwordAround_1(NULL)
, m_pSwordAround_2(NULL)
, m_pAfterGlow(NULL)
, m_pHitWave(NULL)
, m_bSwordTimming(FALSE)
{
	m_bIsStatic = true;
	m_bAni = false;

	ZeroMemory(&m_pWhiteCircle, sizeof(CWave*) * 3);
	ZeroMemory(&m_pGroundDustWave, sizeof(CWave*) * 3);
	ZeroMemory(&m_pSwordSphere, sizeof(CSprite*) * 2);
	ZeroMemory(&m_pSwordWave, sizeof(CWave*) * 2);
}

CVioletCross::~CVioletCross(void)
{
}

void CVioletCross::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayerAction[CWitchBlade::ACTION_VIOLETCROSS] == true)
	{
		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if ((dTargetTime >= 0.672f) && (!m_bSwordTimming))
		{
			m_bSwordTimming = TRUE;

			m_pSwordStar->SetAni(true);
			m_pSwordAround_1->SetAni(true);
			m_pSwordAround_2->SetAni(true);
			m_pAfterGlow->SetAni(true);
			m_pHitWave->SetAni(true);
			for (int i = 0; i < 2; ++i)
				m_pSwordWave[i]->SetAni(true);
		}

		if ( (dTargetTime >= dTime - 0.1f) && (!m_bPuppleSphereCheck))
		{
			m_bPuppleSphereCheck = true;

			for (int i = 0; i < 2; ++i)
			{
				m_pSwordSphere[i]->SetAni(true);
			}
		}

		if (!m_bSkillStart)
		{
			SkillStart();
			m_bSkillStart = true;
		}
	}
	else
	{
		m_bSkillStart = false;
		m_bPuppleSphereCheck = false;
		m_bSwordTimming = FALSE;
	}

	if (!m_bAni)
		return;

	EndAniCheck();

	RuneUpdate();
	GroundUpdate();
	PuppleSphereUpdate();

	
	m_pGroundWave->Update_Object(fTimeDelta);
	for (int i = 0; i < 3; ++i)
	{
	
		m_pGroundDustWave[i]->Update_Object(fTimeDelta);
		m_pWhiteCircle[i]->Update_Object(fTimeDelta);
	}

	m_pRune->Update_Object(fTimeDelta);
	m_pRuneOutLine->Update_Object(fTimeDelta);

	m_pSwordStar->Update_Object(fTimeDelta);
	m_pSwordAround_1->Update_Object(fTimeDelta);
	m_pSwordAround_2->Update_Object(fTimeDelta);
	m_pAfterGlow->Update_Object(fTimeDelta);
	m_pHitWave->Update_Object(fTimeDelta);

	for (int i = 0; i < 2; ++i)
	{
		m_pSwordWave[i]->Update_Object(fTimeDelta);
	}

	for(int i = 0; i < 2; ++i)
	{
		m_pSwordSphere[i]->Update_Object(fTimeDelta);
	}
}

void CVioletCross::Render_Object(void)
{
	if (!m_bAni)
		return;


	m_pGroundWave->Render_Object();
	for (int i = 0; i < 3; ++i)
	{
		m_pGroundDustWave[i]->Render_Object();
		m_pWhiteCircle[i]->Render_Object();
	}

	m_pRune->Render_Object();
	m_pRuneOutLine->Render_Object();

	m_pSwordStar->Render_Object();
	m_pSwordAround_1->Render_Object();
	m_pSwordAround_2->Render_Object();
	m_pAfterGlow->Render_Object();
	m_pHitWave->Render_Object();

	for (int i = 0; i < 2; ++i)
	{
		m_pSwordWave[i]->Render_Object();
	}

	for (int i = 0; i < 2; ++i)
	{
		m_pSwordSphere[i]->Render_Object();
	}
}

HRESULT CVioletCross::Ready_Object(void)
{
	//Rune(마법진)===============================================
	m_pRune = CWave::Create(m_pGraphicDev, L"Rune");
	m_pRune->SetAni(false);
	m_pRune->SetPattern(CWave::PATTERN_END);
	m_pRune->SetBilBoardCheck(true);

	m_pRune->SetRendererCheck(true);
	m_pRune->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	_vec4 Color = _vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_pRune->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRune->GetParticleInfo()->m_iPath = 3;
	m_pRune->GetParticleInfo()->m_iTextureNumber = 29;
	//===========================================================


	//RuneOutLine(마법진)======================================
	m_pRuneOutLine = CWave::Create(m_pGraphicDev, L"RuneOutLine");
	m_pRuneOutLine->SetAni(false);
	m_pRuneOutLine->SetPattern(CWave::PATTERN_END);
	m_pRuneOutLine->SetBilBoardCheck(true);
	m_pRuneOutLine->SetRendererCheck(true);
	m_pRuneOutLine->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	Color = _vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_pRuneOutLine->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRuneOutLine->GetParticleInfo()->m_iPath = 3;
	m_pRuneOutLine->GetParticleInfo()->m_iTextureNumber = 30;
	m_pRuneOutLine->SetZRotateCheck(true);
	m_pRuneOutLine->SetRotateSpeed(10.0f);
	//===========================================================


	//WhiteCircle================================================
	for (int i = 0; i < 3; ++i)
	{
		m_pWhiteCircle[i] = CWave::Create(m_pGraphicDev, L"WhiteLine");
		m_pWhiteCircle[i]->SetAni(false);
		m_pWhiteCircle[i]->SetPattern(CWave::PATTERN_END);
		m_pWhiteCircle[i]->SetBilBoardCheck(true);
		m_pWhiteCircle[i]->SetRendererCheck(true);
		m_pWhiteCircle[i]->GetTransCom()->Set_Scale((i * 1.f) + 1.f, (i * 1.f) + 1.f, (i * 1.f) + 1.f);
		Color = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pWhiteCircle[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pWhiteCircle[i]->GetParticleInfo()->m_iPath = 3;
		m_pWhiteCircle[i]->GetParticleInfo()->m_iTextureNumber = 25;
		m_pWhiteCircle[i]->SetZRotateCheck(false);
		m_pWhiteCircle[i]->SetRotateSpeed(0.0f);
	}
	//===========================================================

	//SwordStart==============(앞에서 터지는거)
	m_pSwordStar = CWave::Create(m_pGraphicDev, L"SwordStar");
	m_pSwordStar->SetAni(false);
	m_pSwordStar->SetPattern(CWave::PATTERN_END);
	m_pSwordStar->SetBilBoardCheck(true);
	m_pSwordStar->SetRendererCheck(true);
	m_pSwordStar->GetTransCom()->Set_Scale(1.3f, 1.3f, 1.3f);
	Color = _vec4(0.5019f, 0.0f, 0.0f, 1.0f);
	m_pSwordStar->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pSwordStar->GetParticleInfo()->m_iPath = 2;
	m_pSwordStar->GetParticleInfo()->m_iTextureNumber = 39;
	m_pSwordStar->SetBlurCheck(true);
	m_pSwordStar->SetBlurCnt(3);
	m_pSwordStar->SetBlurPass(2);
	m_pSwordStar->SetBlurPower(1.1f);
	m_pSwordStar->SetBlurType(Engine::BLUR_STAR);
	m_pSwordStar->SetBlurValue(200.0f);
	//========================

	//SwordAround_1================(앞에 터지는거)
	m_pSwordAround_1 = CWave::Create(m_pGraphicDev, L"SwordAround_1");
	m_pSwordAround_1->SetAni(false);
	m_pSwordAround_1->SetPattern(CWave::PATTERN_END);
	m_pSwordAround_1->SetBilBoardCheck(true);
	m_pSwordAround_1->SetRendererCheck(true);
	m_pSwordAround_1->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	Color = _vec4(0.0980f, 0.0980f, 0.4392f, 1.0f);
	m_pSwordAround_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pSwordAround_1->GetParticleInfo()->m_iPath = 2;
	m_pSwordAround_1->GetParticleInfo()->m_iTextureNumber = 44;
	m_pSwordAround_1->SetBlurCheck(true);
	m_pSwordAround_1->SetBlurCnt(3);
	m_pSwordAround_1->SetBlurPass(2);
	m_pSwordAround_1->SetBlurPower(1.1f);
	m_pSwordAround_1->SetBlurType(Engine::BLUR_HORIZON);
	m_pSwordAround_1->SetBlurValue(200.0f);
	//================================

	//SwordAround_2================앞에 터지는거
	m_pSwordAround_2 = CWave::Create(m_pGraphicDev, L"SwordAround_2");
	m_pSwordAround_2->SetAni(false);
	m_pSwordAround_2->SetPattern(CWave::PATTERN_END);
	m_pSwordAround_2->SetBilBoardCheck(true);
	m_pSwordAround_2->SetRendererCheck(true);
	m_pSwordAround_2->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	Color = _vec4(0.5411f, 0.1686f, 0.8862f, 1.0f);
	m_pSwordAround_2->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pSwordAround_2->GetParticleInfo()->m_iPath = 2;
	m_pSwordAround_2->GetParticleInfo()->m_iTextureNumber = 45;
	m_pSwordAround_2->SetBlurCheck(true);
	m_pSwordAround_2->SetBlurCnt(3);
	m_pSwordAround_2->SetBlurPass(2);
	m_pSwordAround_2->SetBlurPower(1.1f);
	m_pSwordAround_2->SetBlurType(Engine::BLUR_VERTICAL);
	m_pSwordAround_2->SetBlurValue(200.0f);
	//===================================================================

	//AfterGlow=========================================================
	m_pAfterGlow = CWave::Create(m_pGraphicDev, L"VioletCrossAfterGlow");
	m_pAfterGlow->SetAni(false);
	m_pAfterGlow->SetPattern(CWave::PATTERN_END);
	m_pAfterGlow->SetBilBoardCheck(true);
	m_pAfterGlow->SetRendererCheck(true);
	m_pAfterGlow->GetTransCom()->Set_Scale(2.0f, 2.0f, 2.0f);
	Color = _vec4(0.9333f, 0.5098f, 0.9333f, 1.0f);
	m_pAfterGlow->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pAfterGlow->GetParticleInfo()->m_iPath = 2;
	m_pAfterGlow->GetParticleInfo()->m_iTextureNumber = 40;
	//==================================================================

	//================================================================
	m_pHitWave = CWave::Create(m_pGraphicDev, L"VioletCrossHitWave");
	m_pHitWave->SetAni(false);
	m_pHitWave->SetPattern(CWave::PATTERN_END);
	m_pHitWave->SetBilBoardCheck(true);
	m_pHitWave->SetRendererCheck(true);
	m_pHitWave->GetParticleInfo()->m_iPath = 2;
	m_pHitWave->GetParticleInfo()->m_iTextureNumber = 43;
	//==================================================================


	//Ground Wave================================================
	m_pGroundWave = CWave::Create(m_pGraphicDev, L"GroundWave");
	m_pGroundWave->SetAni(false);
	m_pGroundWave->SetPattern(CWave::PATTERN_END);
	m_pGroundWave->SetRendererCheck(true);
	m_pGroundWave->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	Color = _vec4(1.0f, 1.0f, 1.0f, 1.5f);
	m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pGroundWave->GetParticleInfo()->m_iPath = 2;
	m_pGroundWave->GetParticleInfo()->m_iTextureNumber = 10;
	//===========================================================


	//GroundDustWave=============================================
	for (int i = 0; i < 3; ++i)
	{
		m_pGroundDustWave[i] = CWave::Create(m_pGraphicDev, L"GroundDustWave");
		m_pGroundDustWave[i]->SetAni(false);
		m_pGroundDustWave[i]->SetPattern(CWave::PATTERN_END);
		m_pGroundDustWave[i]->SetRendererCheck(true);
		m_pGroundDustWave[i]->GetTransCom()->Set_Scale((i * 1.f) + 1.5f, (i * 1.f) + 1.5f, (i * 1.f) + 1.5f);
		Color = _vec4(1.0f, 1.0f, 1.0f, 1.5f);
		m_pGroundDustWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pGroundDustWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pGroundDustWave[i]->GetParticleInfo()->m_iTextureNumber = 23;
	}
	//===========================================================

	//SwordSphere================================================
	for (int i = 0; i < 2; ++i)
	{
		m_pSwordSphere[i] = CSprite::LoadCreate(m_pGraphicDev, L"VioletCross");
		m_pSwordSphere[i]->SetAni(false);
		m_pSwordSphere[i]->GetParticleInfo()->m_iPath = 2;
		m_pSwordSphere[i]->SetRenderCheck(true);
		m_pSwordSphere[i]->GetTransCom()->Set_Scale(0.7f, 0.7f, 0.7f);
		Color = _vec4(0.0f, 0.0f, 0.0f, 1.f);
		m_pSwordSphere[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pSwordSphere[i]->SetBlurCheck(true);
		m_pSwordSphere[i]->SetBlurCnt(2);
		m_pSwordSphere[i]->SetBlurPass(2);
		m_pSwordSphere[i]->SetBlurPower(1.1f);
		m_pSwordSphere[i]->SetBlurType(Engine::BLUR_HORIZON);
		m_pSwordSphere[i]->SetBlurValue(200.0f);
	}
	//===========================================================

	for (int i = 0; i < 2; ++i)
	{
		m_pSwordWave[i] = CWave::Create(m_pGraphicDev, L"SwordWave");
		m_pSwordWave[i]->SetAni(false);
		m_pSwordWave[i]->SetBilBoardCheck(true);
		m_pSwordWave[i]->SetPattern(CWave::PATTERN_END);
		m_pSwordWave[i]->SetRendererCheck(true);
		m_pSwordWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pSwordWave[i]->GetParticleInfo()->m_iTextureNumber = 42;
	}

	return S_OK;
}

CVioletCross * CVioletCross::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CVioletCross*		pInstance = new CVioletCross(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("VioletCross Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CVioletCross::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pRune);
	Engine::Safe_Release(m_pRuneOutLine);

	for (int i = 0; i < 3; ++i)
	{
		Engine::Safe_Release(m_pWhiteCircle[i]);
		Engine::Safe_Release(m_pGroundDustWave[i]);
	}

	Engine::Safe_Release(m_pGroundWave);

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pSwordSphere[i]);
	}

	for (int i = 0; i < 2; ++i)
	{
		::Safe_Release(m_pSwordWave[i]);
	}

	::Safe_Release(m_pSwordAround_1);
	::Safe_Release(m_pSwordAround_2);
	::Safe_Release(m_pSwordStar);
	::Safe_Release(m_pAfterGlow);

	Engine::Safe_Release(m_pHitWave);
}

HRESULT CVioletCross::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CVioletCross::RuneUpdate(void)
{
	if (m_pRune->GetAni() == true)
	{
		_vec3 vRuneScale;
		m_pRune->GetTransCom()->Get_Scale(&vRuneScale);

		if (vRuneScale.x <= 0.0f)
		{
			m_pRune->SetAni(false);
		}
		else
		{
			m_pRune->GetTransCom()->Add_Scale(-0.05f);
		}

	}

	if (m_pRuneOutLine->GetAni() == true)
	{
		_vec3 vRuneOutLineScale;
		m_pRuneOutLine->GetTransCom()->Get_Scale(&vRuneOutLineScale);

		if (vRuneOutLineScale.x <= 0.0f)
		{
			m_pRuneOutLine->SetAni(false);
		}
		else
		{
			m_pRuneOutLine->GetTransCom()->Add_Scale(-0.05f);
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if(m_pWhiteCircle[i]->GetAni() == true)
		{
			_vec3 vWhiteScale;
			m_pWhiteCircle[i]->GetTransCom()->Get_Scale(&vWhiteScale);

			if (vWhiteScale.x <= 0.0f)
			{
				m_pWhiteCircle[i]->SetAni(false);
			}
			else
			{
				m_pWhiteCircle[i]->GetTransCom()->Add_Scale(-0.15f);
			}

		}
	}


	if (m_pSwordStar->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pSwordStar->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pSwordStar->SetAni(false);
		}
		else
		{
			vColor.w -= 0.042f;
			m_pSwordStar->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

			m_pSwordStar->GetTransCom()->Add_Scale(0.005f);
		}
	}

	if (m_pSwordAround_1->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pSwordAround_1->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pSwordAround_1->SetAni(false);
		}
		else
		{
			vColor.w -= 0.05f;
			m_pSwordAround_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

			m_pSwordAround_1->GetTransCom()->Add_Scale(0.01f);
		}
	}

	if (m_pSwordAround_2->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pSwordAround_2->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pSwordAround_2->SetAni(false);
		}
		else
		{
			vColor.w -= 0.05f;
			m_pSwordAround_2->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

			m_pSwordAround_2->GetTransCom()->Add_Scale(0.01f);
		}
	}
	
	if (m_pAfterGlow->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pSwordAround_2->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pAfterGlow->SetAni(false);
		}
		else
		{
			vColor.w -= 0.07f;
			m_pAfterGlow->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
	}

	if (m_pHitWave->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pHitWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pHitWave->SetAni(false);
		}
		else
		{
			vColor.w -= 0.05f;
			m_pHitWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

			m_pHitWave->GetTransCom()->Add_Scale(0.6f);
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		if (m_pSwordWave[i]->GetAni() == true)
		{
			_vec4 vColor = (_vec4)m_pSwordWave[i]->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pSwordWave[i]->SetAni(false);
			}
			else
			{
				vColor.w -= 0.02f;
				m_pSwordWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

				m_pSwordWave[i]->GetTransCom()->Add_Scale(0.01f);
			}
		}
	}	
}

void CVioletCross::GroundUpdate(void)
{
	if (m_pGroundWave->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pGroundWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pGroundWave->SetAni(false);
		}
		else
		{
			vColor.w -= 0.015f;
			m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

			m_pGroundWave->GetTransCom()->Rotation(Engine::ROT_Y, -10.f);
			m_pGroundWave->GetTransCom()->Add_Scale(0.08f);
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if (m_pGroundDustWave[i]->GetAni() == true)
		{
			_vec4 vColor = (_vec4)m_pGroundDustWave[i]->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pGroundDustWave[i]->SetAni(false);
			}
			else
			{
				vColor.w -= 0.015f;
				m_pGroundDustWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
				m_pGroundDustWave[i]->GetTransCom()->Add_Scale(0.08f);
			}

		}
	}


}

void CVioletCross::PuppleSphereUpdate(void)
{
	if(m_pSwordSphere[0]->GetAni() == true)
	{
		_vec3 vStart;//왼손

		vStart = _vec3(0.2f, -3.0f, -2.0f);

		
		_matrix  WorldMatrix;
		m_pSword1Transform->Get_WorldMatrix(&WorldMatrix);

		D3DXVec3TransformCoord(&vStart, &vStart,& WorldMatrix);

		m_pSwordSphere[0]->GetTransCom()->Set_Position(&vStart);

	}

	if (m_pSwordSphere[1]->GetAni() == true)
	{
		_vec3 vStart;//오른손

		vStart = _vec3(0.0f, -2.5f, -2.2f);

		_matrix  WorldMatrix;
		m_pSwordL1Transform->Get_WorldMatrix(&WorldMatrix);

		D3DXVec3TransformCoord(&vStart, &vStart, &WorldMatrix);

		m_pSwordSphere[1]->GetTransCom()->Set_Position(&vStart);
	}
}

void CVioletCross::SkillStart(void)
{
	if (m_bAni)
		return;

	m_bAni = true;


	//Rune(마법진)=================================================
	m_pRune->SetAni(true);
	_vec3 vTargetPos = m_pTargetTransform->Get_Now_Position();
	vTargetPos.y += 5.f;
	m_pRune->GetTransCom()->Set_Position(&vTargetPos);
	m_pRune->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	//============================================================


	//RuneOutLine(마법진)========================================
	m_pRuneOutLine->SetAni(true);
	m_pRuneOutLine->GetTransCom()->Set_Position(&vTargetPos);
	m_pRuneOutLine->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	//===========================================================


	//WhiteCircle==================================================
	for (int i = 0; i < 3; ++i)
	{
		m_pWhiteCircle[i]->SetAni(true);
		m_pWhiteCircle[i]->GetTransCom()->Set_Position(&vTargetPos);
		m_pWhiteCircle[i]->GetTransCom()->Set_Scale((i * 1.f) + 1.f, (i * 1.f) + 1.f, (i * 1.f) + 1.f);
	}
	//===========================================================
		
	//SwordStart==============0.672	
	_vec3 vCrossPos = vTargetPos + (m_pTargetTransform->Get_Now_Look() * 2.0f);
	m_pSwordStar->GetTransCom()->Set_Position(&vCrossPos);
	m_pSwordStar->GetTransCom()->Set_Scale(1.4f, 1.4f, 1.4f);
	_vec4 SwordStarColor = _vec4(0.5019f, 0.0f, 0.0f, 1.0f);
	m_pSwordStar->GetParticleInfo()->m_dColor = (D3DXCOLOR)SwordStarColor;

	
	m_pSwordAround_1->GetTransCom()->Set_Position(&vCrossPos);
	m_pSwordAround_1->GetTransCom()->Set_Scale(2.25f, 2.25f, 2.25f);
	SwordStarColor = _vec4(0.0980f, 0.0980f, 0.4392f, 1.0f);
	m_pSwordAround_1->GetParticleInfo()->m_dColor = (D3DXCOLOR)SwordStarColor;

	
	m_pSwordAround_2->GetTransCom()->Set_Position(&vCrossPos);
	m_pSwordAround_2->GetTransCom()->Set_Scale(2.25f, 2.25f, 2.25f);
	SwordStarColor = _vec4(0.5411f, 0.1686f, 0.8862f, 1.0f);
	m_pSwordAround_2->GetParticleInfo()->m_dColor = (D3DXCOLOR)SwordStarColor;
	//========================

	//AfterGlow==============	
	vCrossPos = vTargetPos + (m_pTargetTransform->Get_Now_Look() * 1.5f);
	m_pAfterGlow->GetTransCom()->Set_Position(&vCrossPos);
	m_pAfterGlow->GetTransCom()->Set_Scale(1.5f, 2.0f, 1.5f);
	SwordStarColor = _vec4(1.0f, 0.0f, 1.0f, 1.0f);
	m_pAfterGlow->GetParticleInfo()->m_dColor = (D3DXCOLOR)SwordStarColor;
	//========================
		
	m_pHitWave->GetTransCom()->Set_Position(&vCrossPos);
	m_pHitWave->GetTransCom()->Set_Scale(1.0f, 1.0f, 1.0f);
	SwordStarColor = _vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_pHitWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)SwordStarColor;

	_vec3 vLeft;
	D3DXVec3Cross(&vLeft, &m_pTargetTransform->Get_Now_Look(), &_vec3(0.0f, 1.0f, 0.0f));

	for (int i = 0; i < 2; ++i)
	{		
		if (0 == i)
		{
			vCrossPos = vTargetPos + (m_pTargetTransform->Get_Now_Look() * 2.5f) + (vLeft * 2.0f);
			m_pSwordWave[i]->GetTransCom()->Set_Angle(&_vec3(0.0f, 0.0f, 45.0f));
		}			
		else if (1 == i)
		{
			vCrossPos = vTargetPos + (m_pTargetTransform->Get_Now_Look() * 2.5f) - (vLeft * 2.0f);
			m_pSwordWave[i]->GetTransCom()->Set_Angle(&_vec3(0.0f, 0.0f, -45.0f));
		}
		
		m_pSwordWave[i]->GetTransCom()->Set_Position(&vCrossPos);

		m_pSwordWave[i]->GetTransCom()->Set_Scale(0.0005f, 3.5f, 1.0f);

		SwordStarColor = _vec4(0.0f, 0.0f, 1.0f, 1.0f);
		m_pSwordWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)SwordStarColor;
	}


	//Ground Wave===============================================
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 1.0f;
	m_pGroundWave->SetAni(true);
	m_pGroundWave->GetTransCom()->Set_Position(&vPos);
	m_pGroundWave->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	m_pGroundWave->GetTransCom()->Set_Angle(&vAngle);
	_vec4 Color = _vec4(1.0f, 1.f, 1.0f, 1.5f);
	m_pGroundWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	//==========================================================


	//Ground Dust Wave==========================================
	for (int i = 0; i < 3; ++i)
	{
		m_pGroundDustWave[i]->SetAni(true);
		m_pGroundDustWave[i]->GetTransCom()->Set_Position(&vPos);
		m_pGroundDustWave[i]->GetTransCom()->Set_Scale( (i * 1.f) + 1.5f, (i * 1.f) + 1.5f, (i * 1.f) + 1.5f);
		_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
		m_pGroundDustWave[i]->GetTransCom()->Set_Angle(&vAngle);
		Color = _vec4(1.0f, 1.0f, 1.0f, 1.5f);
		m_pGroundDustWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pGroundDustWave[i]->GetTransCom()->Rotation(Engine::ROT_Y, i * 10.f);
	}
	//==========================================================	
}

void CVioletCross::EndAniCheck(void)
{
	if (m_bAni == true && m_pRune->GetAni() == false && m_pRuneOutLine->GetAni() == false && m_pWhiteCircle[0]->GetAni() == false 
		&& m_pWhiteCircle[1]->GetAni() == false && m_pWhiteCircle[2]->GetAni() == false && m_pGroundWave->GetAni() == false
		&& m_pGroundDustWave[0]->GetAni() == false && m_pGroundDustWave[1]->GetAni() == false && m_pGroundDustWave[2]->GetAni() == false
		&& m_pSwordSphere[0]->GetAni() == false && m_pSwordSphere[1]->GetAni() == false
		&& m_pSwordStar->GetAni() == false && m_pSwordAround_1->GetAni() == false && m_pSwordAround_2->GetAni() == false && m_pAfterGlow->GetAni() == false
		&& m_pSwordWave[0]->GetAni() == false && m_pSwordWave[1]->GetAni() == false
		&& m_pHitWave->GetAni() == false)
		m_bAni = false;
}

HRESULT CVioletCross::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return E_FAIL;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();

	//Player TransCom
	Engine::CTransform* pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pTargetTransform = pTargetTransform;


	Engine::CGameObject*  pSwordObject = Engine::Find(L"Sword1");
	if (pSwordObject == NULL)
		return E_FAIL;

	m_pSword1Transform = (Engine::CTransform*)pSwordObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);



	Engine::CGameObject*  pSwordLObject = Engine::Find(L"SwordL1");
	if (pSwordLObject == NULL)
		return E_FAIL;

	m_pSwordL1Transform = (Engine::CTransform*)pSwordLObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);




	Engine::Safe_Release(pSwordLObject);
	Engine::Safe_Release(pSwordObject);
	Engine::Safe_Release(pTargetTransform);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}
