#include "stdafx.h"
#include "HanumanCombo.h"
#include "HanumaAttack01_Trail.h"
#include "Hanuman.h"
#include "ReflectMesh.h"
#include "Wave.h"
#include "ReflectDust.h"
#include "Sprite.h"

CHanumanCombo::CHanumanCombo(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pLeft_0_Trail(NULL)
, m_pLeft_1_Trail(NULL)
, m_pLeft_2_Trail(NULL)
, m_bLeftSwing(false)
, m_bLeftDeleteSwing(false)
, m_pReflectGroundWave(NULL)
, m_bReflectGroundWaveCheck(false)
, m_dReflectSize(0)
, m_pBlueWave(NULL)
, m_pRedWave(NULL)
, m_bDustCheck(false)
, m_bRadialBlur(FALSE)
, m_pHanuman(NULL)
, m_fRadialBlurTime(0.0f)
{
	ZeroMemory(&m_pDust, sizeof(CSprite*) * 20);
	m_bAni = false;

	ZeroMemory(&m_pSlideWave, sizeof(CWave*) * 8);
}

CHanumanCombo::~CHanumanCombo(void)
{
}

void CHanumanCombo::Update_Object(const _float & fTimeDelta)
{
	if (g_bHanumanDead)
		return;

	//Combo0 -> 왼손 준비 동작
	//Combo1 -> 왼손 스윙
	//Combo2 -> 오른손 내려 찍기
	//Combo3 -> 동작 마무리 Roar
	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::COMBO1)
	{
		m_bAni = true;

		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (!m_bLeftSwing) //Trail 발동
		{
			m_bLeftSwing = true;

			m_pLeft_0_Trail->SetAni(true);
			m_pLeft_0_Trail->SettingTrail();

			m_pLeft_1_Trail->SetAni(true);
			m_pLeft_1_Trail->SettingTrail();

			m_pLeft_2_Trail->SetAni(true);
			m_pLeft_2_Trail->SettingTrail();
		}

		if( (0.6f <= dTargetTime) && (!m_bLeftDeleteSwing))
		{
			_vec4 vColor = m_pLeft_0_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pLeft_0_Trail->SetColor(vColor);

			vColor = m_pLeft_0_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pLeft_1_Trail->SetColor(vColor);

			vColor = m_pLeft_0_Trail->GetColor();
			vColor.w -= 0.08f;
			m_pLeft_2_Trail->SetColor(vColor);

			if (m_pLeft_0_Trail->GetColor().w <= 0.0f)
			{
				m_pLeft_0_Trail->SetAni(false);
				m_pLeft_1_Trail->SetAni(false);
				m_pLeft_2_Trail->SetAni(false);

				m_bLeftDeleteSwing = true;
			}
		}
	}
	else
	{
		m_bLeftDeleteSwing = false;
		m_bLeftSwing = false;
	}

	//오른손 내려찍기
	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::COMBO2)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//Reflect GroundWave 발동
		if ((0.59f <= dTargetTime) && (!m_bReflectGroundWaveCheck))
		{
			m_bReflectGroundWaveCheck = true;

			//BlueWave, RedWave Start
			ColorWaveStart();

			ReflectGroundWaveStart(); //ReflectGroundWave_Start

			m_bRadialBlur = TRUE;
		}

		// Dust 발동
		if((0.62f <= dTargetTime) && (!m_bDustCheck))
		{
			m_bDustCheck = true;

			DustStart(); //먼지
			SlideWaveStart();//슬라이드웨이브
		}

	}
	else
	{
		m_bReflectGroundWaveCheck = false;
		m_bDustCheck = false;
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

	//ColorWave 업데이트
	ColorWaveUpdate();

	//ReflectGround업데이트
	ReflectGroundUpdate();

	//Dust 업데이트
	DustUpdate();


	//SlideWave 업데이트
	SlideWaveUpdate();


	//SlideWave Update
	for (int i = 0; i < 8; ++i)
	{
		m_pSlideWave[i]->Update_Object(fTimeDelta);
	}

	//Trail Update
	m_pLeft_0_Trail->Update_Object(fTimeDelta);
	m_pLeft_1_Trail->Update_Object(fTimeDelta);
	m_pLeft_2_Trail->Update_Object(fTimeDelta);

	//ReflectWave
	m_pReflectGroundWave->Update_Object(fTimeDelta);

	//Wave Update
	m_pBlueWave->Update_Object(fTimeDelta);
	m_pRedWave->Update_Object(fTimeDelta);

	//Dust Update
	for (int i = 0; i < 20; ++i)
	{
		m_pDust[i]->Update_Object(fTimeDelta);
	}

}

void CHanumanCombo::Render_Object(void)
{
	if (g_bHanumanDead)
		return;

	if (!m_bAni)
		return;


	//SlideWave
	for (int i = 0; i < 8; ++i)
	{
		m_pSlideWave[i]->Render_Object();
	}


	m_pLeft_0_Trail->Render_Object();
	m_pLeft_1_Trail->Render_Object();
	m_pLeft_2_Trail->Render_Object();

	//ReflectWave
	m_pReflectGroundWave->Render_Object();

	//Wave Render
	m_pBlueWave->Render_Object();
	m_pRedWave->Render_Object();

	//Reflect Render
	for (int i = 0; i < 20; ++i)
	{
		m_pDust[i]->Render_Object();
	}
}

HRESULT CHanumanCombo::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//===============================================================================
	m_pReflectGroundWave = CReflectMesh::Create(m_pGraphicDev, L"ReflectGroundWave");
	m_pReflectGroundWave->SetAni(false);
	m_pReflectGroundWave->SetPattern(CReflectMesh::PATTERN_END);
	m_pReflectGroundWave->SetReflectIndex(0.2f);
	//================================================================================


	//BlueWave===============================================
	m_pBlueWave = CWave::Create(m_pGraphicDev, L"BlueWave");
	m_pBlueWave->SetAni(false);
	m_pBlueWave->SetPattern(CWave::PATTERN_END);
	m_pBlueWave->SetRendererCheck(true);
	m_pBlueWave->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	_vec4 BlueColor = _vec4(0.1f, 0.0f, 1.0f, 1.5f);
	m_pBlueWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)BlueColor;
	m_pBlueWave->GetParticleInfo()->m_iPath = 2;
	m_pBlueWave->GetParticleInfo()->m_iTextureNumber = 0;

	m_pBlueWave->SetBlurCheck(true);
	m_pBlueWave->SetBlurCnt(2);
	m_pBlueWave->SetBlurPass(2);
	m_pBlueWave->SetBlurPower(1.1f);
	m_pBlueWave->SetBlurType(Engine::BLUR_CROSS);
	m_pBlueWave->SetBlurValue(128.0f);
	//===========================================================

	//RedWave====================================================
	m_pRedWave = CWave::Create(m_pGraphicDev, L"RedWave");
	m_pRedWave->SetAni(false);
	m_pRedWave->SetPattern(CWave::PATTERN_END);
	m_pRedWave->SetRendererCheck(true);
	m_pRedWave->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	_vec4 RedColor = _vec4(1.0f, 0.0f, 0.1f, 1.5f);
	m_pRedWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)RedColor;
	m_pRedWave->GetParticleInfo()->m_iPath = 2;
	m_pRedWave->GetParticleInfo()->m_iTextureNumber = 8;

	m_pRedWave->SetBlurCheck(true);
	m_pRedWave->SetBlurCnt(2);
	m_pRedWave->SetBlurPass(2);
	m_pRedWave->SetBlurPower(1.1f);
	m_pRedWave->SetBlurType(Engine::BLUR_CROSS);
	m_pRedWave->SetBlurValue(128.0f);
	//===========================================================


	//SlideWave=================================================
	for (int i = 0; i < 8; ++i)
	{
		m_pSlideWave[i] = CWave::Create(m_pGraphicDev, L"SlideWave");
		m_pSlideWave[i]->SetAni(false);
		m_pSlideWave[i]->SetPattern(CWave::PATTERN_END);
		m_pSlideWave[i]->SetRendererCheck(true);
		m_pSlideWave[i]->SetBilBoardCheck(false);
		m_pSlideWave[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
		_vec4 SlideColor = _vec4(1.0f, 0.5f, 0.0f, 1.5f);
		m_pSlideWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)SlideColor;
		m_pSlideWave[i]->GetParticleInfo()->m_iPath = 2;
		m_pSlideWave[i]->GetParticleInfo()->m_iTextureNumber = 46;
		m_pSlideWave[i]->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);

		m_pSlideWave[i]->SetBlurCheck(true);
		m_pSlideWave[i]->SetBlurCnt(2);
		m_pSlideWave[i]->SetBlurPass(2);
		m_pSlideWave[i]->SetBlurPower(1.1f);
		m_pSlideWave[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pSlideWave[i]->SetBlurValue(128.0f);
		//===========================================================
	}


	//Dust================================================
	for (int i = 0; i < 20; ++i)
	{
		m_pDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanHQDust");
		m_pDust[i]->SetAni(false);
		m_pDust[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
		_vec4 Color = _vec4(0.2f, 0.2f, 0.2f, 2.f);
		m_pDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pDust[i]->SetRenderCheck(true);
		m_pDust[i]->GetTransCom()->Set_Scale(4.f, 4.f, 4.f);

		m_pDust[i]->SetBlurCheck(true);
		m_pDust[i]->SetBlurCnt(2);
		m_pDust[i]->SetBlurPass(3);
		m_pDust[i]->SetBlurPower(1.05f);
		m_pDust[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pDust[i]->SetBlurValue(100.0f);
	}
	//===========================================================




	return S_OK;
}

CHanumanCombo * CHanumanCombo::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHanumanCombo*		pInstance = new CHanumanCombo(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("HanumanCombo Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHanumanCombo::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pLeft_0_Trail);
	Engine::Safe_Release(m_pLeft_1_Trail);
	Engine::Safe_Release(m_pLeft_2_Trail);
	Engine::Safe_Release(m_pReflectGroundWave);
	Engine::Safe_Release(m_pBlueWave);
	Engine::Safe_Release(m_pRedWave);

	for (int i = 0; i < 20; ++i)
	{
		Engine::Safe_Release(m_pDust[i]);
	}

	for (int i = 0; i < 8; ++i)
	{
		Engine::Safe_Release(m_pSlideWave[i]);
	}

	::Safe_Release(m_pHanuman);
}

HRESULT CHanumanCombo::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CHanumanCombo::SkillStart(void)
{
}

void CHanumanCombo::EndAniCheck(void)
{
	if (m_bAni == true)
		m_bAni = false;
}

HRESULT CHanumanCombo::Start_ForScene(void)
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

	const _matrix* pMatWorld = ((CHanuman*)m_pHanuman)->GetTransCom()->Get_WorldMatrixPointer();

	//LeftTrail--------------------------------------------------------------------------------
	const _matrix* pStartMat = m_pTargetMesh->Get_FrameByName("_39");
	const _matrix* pEndpMat = m_pTargetMesh->Get_FrameByName("_40");

	m_pLeft_0_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Combo_0_Left_Trail");
	m_pLeft_0_Trail->SetAni(false);
	m_pLeft_0_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pLeft_0_Trail->SetTargetWorldMat(pMatWorld);

	pStartMat = m_pTargetMesh->Get_FrameByName("_41");
	pEndpMat = m_pTargetMesh->Get_FrameByName("_42");

	m_pLeft_1_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Combo_1_Left_Trail");
	m_pLeft_1_Trail->SetAni(false);
	m_pLeft_1_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pLeft_1_Trail->SetTargetWorldMat(pMatWorld);

	pStartMat = m_pTargetMesh->Get_FrameByName("_43");
	pEndpMat = m_pTargetMesh->Get_FrameByName("_44");

	m_pLeft_2_Trail = CHanumaAttack01_Trail::Create(m_pGraphicDev, L"Hanuman_Combo_2_Left_Trail");
	m_pLeft_2_Trail->SetAni(false);
	m_pLeft_2_Trail->SetTargetMatrix(pStartMat, pEndpMat);
	m_pLeft_2_Trail->SetTargetWorldMat(pMatWorld);
	//-------------------------------------------------------------------------------------
	return S_OK;
}

void CHanumanCombo::ReflectGroundWaveStart(void)
{
	m_pReflectGroundWave->GetTransCom()->Set_Position(&m_pTargetTransform->Get_Now_Position());
	m_pReflectGroundWave->SetAni(true);
	m_pReflectGroundWave->GetTransCom()->Set_Scale(0.2f, 0.05f, 0.2f);

}


//굴절 웨이브 Update
void CHanumanCombo::ReflectGroundUpdate(void)
{
	if (m_pReflectGroundWave->GetAni() == true)
	{
		if (m_dReflectSize >= 30.f)
		{
			m_pReflectGroundWave->SetAni(false);
			m_dReflectSize = 0;
		}
		else
		{
			m_pReflectGroundWave->GetTransCom()->Add_NotYScale(0.2f);
			m_dReflectSize += 0.2;
			_vec3 vPos;
			vPos = m_pReflectGroundWave->GetTransCom()->Get_Now_Position();
			vPos.y -= 0.01f;
			m_pReflectGroundWave->GetTransCom()->Set_Position(&vPos);
		}
	}
}

void CHanumanCombo::ColorWaveStart(void)
{

	//BlueWave======================================================
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	vPos.y += 1.0f;
	m_pBlueWave->SetAni(true);
	m_pBlueWave->GetTransCom()->Set_Position(&vPos);
	m_pBlueWave->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);
	m_pBlueWave->GetTransCom()->Set_Angle(&vAngle);
	_vec4 BlueColor = _vec4(0.1f, 0.0f, 1.0f, 1.5f);
	m_pBlueWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)BlueColor;
	//=============================================================

	//RedWave======================================================
	m_pRedWave->SetAni(true);
	m_pRedWave->GetTransCom()->Set_Position(&vPos);
	m_pRedWave->GetTransCom()->Set_Scale(2.3f, 2.3f, 2.3f);
	m_pRedWave->GetTransCom()->Set_Angle(&vAngle);
	_vec4 RedColor = _vec4(1.0f, 0.0f, 0.1f, 1.5f);
	m_pRedWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)RedColor;
	//==============================================================
}

void CHanumanCombo::ColorWaveUpdate(void)
{
	//파란 웨이브가 빨간 웨이브보다 좀더 빨라야함
	if (m_pBlueWave->GetAni() == true)
	{

		_vec4 vColor = (_vec4)m_pBlueWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
			m_pBlueWave->SetAni(false);
		else
		{
			m_pBlueWave->GetTransCom()->Add_Scale(0.4f);
			vColor.w -= 0.04f;
			m_pBlueWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
	}
	//빨간 웨이브
	if (m_pRedWave->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pRedWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
			m_pRedWave->SetAni(false);
		else
		{
			m_pRedWave->GetTransCom()->Add_Scale(0.38f);
			vColor.w -= 0.04f;
			m_pRedWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
	}
}

void CHanumanCombo::DustStart(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();


	for (int i = 0; i < 20; ++i)
	{
		_vec3 vPosition;

		_float	fPhi;		// 각도 φ

		fPhi = (_float)(i * 18.f);

		fPhi = D3DXToRadian(fPhi);

		m_pDust[i]->SetAni(true);
		vPosition.x = vPos.x + (20.f * cosf(fPhi));
		vPosition.z = vPos.z + (20.f * sinf(fPhi));
		vPosition.y = vPos.y;

		m_pDust[i]->GetTransCom()->Set_Position(&vPosition);
	}
}

void CHanumanCombo::DustUpdate(void)
{
	for (int i = 0; i < 20; ++i)
	{
		if (m_pDust[i]->GetAni() == false)
			continue ;

		_vec3 vPos = m_pDust[i]->GetTransCom()->Get_Now_Position();

		vPos.y += 0.01f;

		m_pDust[i]->GetTransCom()->Set_Position(&vPos);
	}
}


void CHanumanCombo::SlideWaveStart(void)
{
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();

	for (int i = 0; i < 8; ++i)
	{
		m_pSlideWave[i]->SetAni(true);
	
		_vec3 vPosition;

		_float	fPhi;		// 각도 φ

		fPhi = i * 45.f;

		_vec3 vAngle;
		vAngle = _vec3(0.f, 180.f -fPhi, 0.f);

		m_pSlideWave[i]->GetTransCom()->Set_Angle(&vAngle);

		fPhi = D3DXToRadian(fPhi);

		vPosition.x = vPos.x + (10.f * cosf(fPhi));
		vPosition.z = vPos.z + (10.f * sinf(fPhi));
		vPosition.y = vPos.y;


		m_vSlideWaveDir[i] = vPosition - vPos;

		D3DXVec3Normalize(&m_vSlideWaveDir[i], &m_vSlideWaveDir[i]);

		vPosition.y += 5.f;
		m_pSlideWave[i]->GetTransCom()->Set_Position(&vPosition);

		_vec4 SlideColor = _vec4(1.0f, 0.5f, 0.0f, 1.5f);
		m_pSlideWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)SlideColor;

	}
	

}

void CHanumanCombo::SlideWaveUpdate(void)
{
	if (!m_pSlideWave[0]->GetAni())
		return;

	for (int i = 0; i < 8; ++i)
	{
		_vec4 vColor = (_vec4)m_pSlideWave[i]->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pSlideWave[i]->SetAni(false);
		}
		else
		{
			vColor.w -= 0.07f;

			m_pSlideWave[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;


			_vec3 vPos;
			vPos = m_pSlideWave[i]->GetTransCom()->Get_Now_Position();

			vPos += m_vSlideWaveDir[i] * 0.3f;

			m_pSlideWave[i]->GetTransCom()->Set_Position(&vPos);

		}
	}
}