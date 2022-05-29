#include "stdafx.h"
#include "DarkPollingGrave.h"
#include "Export_Engine.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"
#include "DarkSword.h"
#include "Wave.h"
#include "Sprite.h"

CDarkPollingGrave::CDarkPollingGrave(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pPlayerAction(NULL)
, m_pWave(NULL)
, m_bSkillCheck(false)
, m_pBlueFlowerWave(NULL)
, m_pBlackLine(NULL)
, m_bBlueFlowerCheck(false)
, m_pRuneOutLine(NULL)
, m_pMainRune(NULL)
, m_pRedCircle(NULL)
, m_bDarkSwordCheck(false)
, m_bSwordTargetCheck(false)
, m_pvecAniInfo(NULL)
, m_bCenterSwordCheck(false)
, m_bCenterShoot(false)
, m_bDarkPollingSwordEnd(false)
, m_bSwordPositionCheck(false)
{
	m_bIsStatic = true;
	m_bAni = false;

	ZeroMemory(&m_pWhiteLine, sizeof(CWave*) * 2);

	ZeroMemory(&m_pDarkSword, sizeof(CDarkSword*) * 20);
	ZeroMemory(&m_pDarkOutLineSword, sizeof(CDarkSword*) * 20);
	ZeroMemory(&m_pDarkMiddleLineSword, sizeof(CDarkSword*) * 20);
	ZeroMemory(&m_pCenterSword, sizeof(CDarkSword*) * 20);
}

CDarkPollingGrave::~CDarkPollingGrave(void)
{
}

void CDarkPollingGrave::Update_Object(const _float & fTimeDelta)
{

	if (m_pPlayerAction[CWitchBlade::ACTION_DARKFALLINGGRAVE] == true)
	{
		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->pMotionEventInfo[EVENT_EFFECT].dStartTime;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (!m_bSkillCheck) //초기 스킬 시작 판단 변수
		{
			SkillStart(); //초기 발동되는 이펙트 세팅 함수
			m_bSkillCheck = true;
		}

		if (dTargetTime > 2.75f) //Effect 종료 시간이 되었을 경우
		{
			m_bDarkPollingSwordEnd = true;
		}

		if (dTargetTime >= 2.04f && dTargetTime < 2.21f)
		{
			Engine::SetRadialBlurPower(-0.1f);
			Engine::SetRadialBlur(TRUE);
		}

		if (dTargetTime >= 2.21f && dTargetTime < 2.26f)
		{
			Engine::SetRadialBlurPower(-0.25f);
			Engine::SetRadialBlur(FALSE);
		}


		if (dTargetTime >= 1.10f) //검이펙트 발동
		{
			m_bCenterSwordCheck = true;

			if (!m_bSwordPositionCheck)
			{
				for (int i = 0; i < 20; ++i)
				{

					//로컬상의 좌표를 잡아준다.
					//============================
					_float	fPhi;		// 각도 φ
					fPhi = 18.f * i;
					// 라디안으로 변
					fPhi = D3DXToRadian(fPhi);

					//가장 안쪽 검(Center Sword)
					m_vCenterSwordPosition[i].x = 5 * cosf(fPhi);
					m_vCenterSwordPosition[i].z = 3.f + 5 * sinf(fPhi);
					m_vCenterSwordPosition[i].y = 25.0f + rand() % 10;

					//Inside Sword
					m_vInsideSwordPosition[i].x = 8 * cosf(fPhi);
					m_vInsideSwordPosition[i].z = 3.f + 8 * sinf(fPhi);
					m_vInsideSwordPosition[i].y = 30.0f + rand() % 10;

					//Middle Sword
					m_vMiddleSwordPosition[i].x = 11 * cosf(fPhi);
					m_vMiddleSwordPosition[i].z = 3.f + 12 * sinf(fPhi);
					m_vMiddleSwordPosition[i].y = 35.0f + rand() % 10;

					//Out Sword
					m_vOutSwordPosition[i].x = 14 * cosf(fPhi);
					m_vOutSwordPosition[i].z = 3.f + 15 * sinf(fPhi);
					m_vOutSwordPosition[i].y = 40.0f + rand() % 10;


					m_pCenterSword[i]->GetTransCom()->Set_Scale(0.175f, 0.05f, 0.05f);
					m_pDarkSword[i]->GetTransCom()->Set_Scale(0.175f, 0.05f, 0.05f);
					m_pDarkOutLineSword[i]->GetTransCom()->Set_Scale(0.175f, 0.05f, 0.05f);
					m_pDarkMiddleLineSword[i]->GetTransCom()->Set_Scale(0.175f, 0.05f, 0.05f);


					m_fCenterSwordNumber[i] = rand() % 3 * 0.001f + 0.001f;
					m_pInsideSwordNumber[i] = rand() % 3 * 0.001f + 0.001f;
					m_pMiddleSwordNumber[i] = rand() % 3 * 0.001f + 0.001f;
					m_fOutSwordNumber[i]	= rand() % 3 * 0.001f + 0.001f;
				}

				m_bSwordPositionCheck = true;
			}

		}

	}
	else
	{
		m_bSkillCheck = false;
		m_bBlueFlowerCheck = false;
		m_bSwordPositionCheck = false;
	}


	if (!m_bAni)
		return;

	EndAniCheck();

	//Black Wave Update
	BlackWaveUpdate();

	//WhiteLine Update
	WhiteLineUpdate();

	//BlackLine Update
	BlackLineUpdate();

	//BlueFlower
	BlueFlowerUpdate();
	RuneOutLineUpdate();

	
	m_pRuneOutLine->Update_Object(fTimeDelta);
	m_pMainRune->Update_Object(fTimeDelta);
	m_pRedCircle->Update_Object(fTimeDelta);
	m_pBlackLine->Update_Object(fTimeDelta);
	m_pBlueFlowerWave->Update_Object(fTimeDelta);

	for (int i = 0; i < 2; ++i)
	{
		m_pWhiteLine[i]->Update_Object(fTimeDelta);
	}

	m_pWave->Update_Object(fTimeDelta);

	//가장 안쪽 검 메시 이펙트 Update
	CenterSwordUpdate(fTimeDelta);


	//Inside 검 메시 이펙트 Update
	InsideSwordUpdate(fTimeDelta);

	//Middle 검 메시 이펙트 Update
	MiddleSwordUpdate(fTimeDelta);

	//Out 검 메시 이펙트 Update
	OutSwordUpdate(fTimeDelta);
}

void CDarkPollingGrave::Render_Object(void)
{
	if (!m_bAni)
		return;

	m_pRuneOutLine->Render_Object();
	m_pMainRune->Render_Object();

	m_pRedCircle->Render_Object();

	m_pBlackLine->Render_Object();
	m_pBlueFlowerWave->Render_Object();

	for (int i = 0; i < 2; ++i)
	{
		m_pWhiteLine[i]->Render_Object();
	}

	m_pWave->Render_Object();

	for (int i = 0; i < 20; ++i)
	{
		m_pDarkSword[i]->Render_Object();
		m_pDarkOutLineSword[i]->Render_Object();
		m_pDarkMiddleLineSword[i]->Render_Object();
		m_pCenterSword[i]->Render_Object();
	}

}

HRESULT CDarkPollingGrave::Ready_Object(void)
{
	//준비동작 시 퍼져나가는 검은 Wave 생성
	//검은 Wave 생성========================================================
	m_pWave = CWave::Create(m_pGraphicDev, L"BlackWave");
	_vec4 color = _vec4(0.f, 0.f, 0.f, 2.f);
	m_pWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)color;
	m_pWave->GetParticleInfo()->m_iPath = 3;
	m_pWave->GetParticleInfo()->m_iTextureNumber = 21;
	m_pWave->SetPattern(CWave::PATTERN_END);
	m_pWave->SetBilBoardCheck(true);
	m_pWave->SetRendererCheck(true);
	m_pWave->SetAni(false);
	//Blur===================================================================
	m_pWave->SetBlurCheck(true);
	m_pWave->SetBlurCnt(1);
	m_pWave->SetBlurPass(3);
	m_pWave->SetBlurPower(1.01f);
	m_pWave->SetBlurValue(400.0f);
	m_pWave->SetBlurType(Engine::BLUR_STAR);
	//=======================================================================

	//White Line=============================================================
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 2.0f);

	for (int i = 0; i < 2; ++i)
	{
		m_pWhiteLine[i] = CWave::Create(m_pGraphicDev, L"WhiteLine");
		m_pWhiteLine[i]->SetAni(false);
		m_pWhiteLine[i]->SetPattern(CWave::PATTERN_END);
		m_pWhiteLine[i]->SetRendererCheck(true);
		m_pWhiteLine[i]->GetTransCom()->Set_Scale((i * 1.f) + 1.f, (i * 1.f) + 1.f, (i * 1.f) + 1.f);
		m_pWhiteLine[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pWhiteLine[i]->GetParticleInfo()->m_iPath = 2;
		m_pWhiteLine[i]->GetParticleInfo()->m_iTextureNumber = 7;

		m_pWhiteLine[i]->SetBlurCheck(true);
		m_pWhiteLine[i]->SetBlurCnt(1);
		m_pWhiteLine[i]->SetBlurPass(2);
		m_pWhiteLine[i]->SetBlurPower(1.1f);
		m_pWhiteLine[i]->SetBlurValue(300.0f);
		m_pWhiteLine[i]->SetBlurType(Engine::BLUR_STAR);
	}
	//=======================================================================


	//=======================================================================
	_vec4 BlackColor = _vec4(0.0f, 0.0f, 0.0f, 3.0f);
	//BlackLine
	m_pBlackLine = CWave::Create(m_pGraphicDev, L"BlackLine");
	m_pBlackLine->SetAni(false);
	m_pBlackLine->SetPattern(CWave::PATTERN_END);
	m_pBlackLine->SetRendererCheck(true);
	m_pBlackLine->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pBlackLine->GetParticleInfo()->m_dColor = (D3DXCOLOR)BlackColor;
	m_pBlackLine->GetParticleInfo()->m_iPath = 3;
	m_pBlackLine->GetParticleInfo()->m_iTextureNumber = 20;
	//=======================================================================
	/*m_pBlackLine->SetBlurCheck(true);
	m_pBlackLine->SetBlurCnt(1);
	m_pBlackLine->SetBlurPass(3);
	m_pBlackLine->SetBlurPower(1.01f);
	m_pBlackLine->SetBlurValue(500.0f);
	m_pBlackLine->SetBlurType(Engine::BLUR_STAR);*/
	//=======================================================================




	//BlueFlower 생성======================================================
	m_pBlueFlowerWave = CWave::Create(m_pGraphicDev, L"BlueFlower");
	color = _vec4(-1.f, -0.2f, 1.f, 3.f);
	m_pBlueFlowerWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)color;
	m_pBlueFlowerWave->GetParticleInfo()->m_iPath = 2;
	m_pBlueFlowerWave->GetParticleInfo()->m_iTextureNumber = 22;
	m_pBlueFlowerWave->SetDecrement(0.05f, 0.05f); //알파값 , 스케일값 감소량 설정
	m_pBlueFlowerWave->SetPattern(CWave::PATTERN_END);
	m_pBlueFlowerWave->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	m_pBlueFlowerWave->SetAni(false);
	m_pBlueFlowerWave->SetRendererCheck(true);

	/*m_pBlueFlowerWave->SetBlurCheck(true);
	m_pBlueFlowerWave->SetBlurCnt(1);
	m_pBlueFlowerWave->SetBlurPass(2);
	m_pBlueFlowerWave->SetBlurPower(1.01f);
	m_pBlueFlowerWave->SetBlurValue(500.0f);
	m_pBlueFlowerWave->SetBlurType(Engine::BLUR_STAR);*/
	//=====================================================================

	//RuneOutLine==========================================================
	m_pRuneOutLine = CWave::Create(m_pGraphicDev, L"RuneOutLine");
	color = _vec4(0.0f, 0.0f, 0.0f, 2.0f);
	m_pRuneOutLine->GetParticleInfo()->m_dColor = (D3DXCOLOR)color;
	m_pRuneOutLine->GetParticleInfo()->m_iPath = 3;
	m_pRuneOutLine->GetParticleInfo()->m_iTextureNumber = 31;
	m_pRuneOutLine->SetPattern(CWave::PATTERN_END);
	m_pRuneOutLine->GetTransCom()->Set_Scale(3.f, 3.f, 3.f);
	m_pRuneOutLine->SetAni(false);
	m_pRuneOutLine->SetRendererCheck(true);
	//=====================================================================

	//MainRune=============================================================
	m_pMainRune = CWave::Create(m_pGraphicDev, L"MainRune");
	color = _vec4(0.0f, 0.0f, 0.0f, 3.0f);
	m_pMainRune->GetParticleInfo()->m_dColor = (D3DXCOLOR)color;
	m_pMainRune->GetParticleInfo()->m_iPath = 3;
	m_pMainRune->GetParticleInfo()->m_iTextureNumber = 32;
	m_pMainRune->SetPattern(CWave::PATTERN_END);
	m_pMainRune->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);
	m_pMainRune->SetAni(false);
	m_pMainRune->SetRendererCheck(true);
	//=====================================================================

	//RedCircle===========================================================
	m_pRedCircle = CWave::Create(m_pGraphicDev, L"RedCircle");
	color = _vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_pRedCircle->GetParticleInfo()->m_dColor = (D3DXCOLOR)color;
	m_pRedCircle->GetParticleInfo()->m_iPath = 2;
	m_pRedCircle->GetParticleInfo()->m_iTextureNumber = 33;
	m_pRedCircle->SetPattern(CWave::PATTERN_END);
	m_pRedCircle->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);
	m_pRedCircle->SetAni(false);
	m_pRedCircle->SetRendererCheck(true);

	m_pRedCircle->SetBlurCheck(true);
	m_pRedCircle->SetBlurCnt(1);
	m_pRedCircle->SetBlurPass(2);
	m_pRedCircle->SetBlurPower(1.01f);
	m_pRedCircle->SetBlurValue(500.0f);
	m_pRedCircle->SetBlurType(Engine::BLUR_STAR);
	//=====================================================================

	for (int i = 0; i < 20; ++i)
	{
		m_pDarkSword[i] = CDarkSword::Create(m_pGraphicDev, L"DarkSword");
		m_pDarkSword[i]->SetAni(false);

		m_pDarkOutLineSword[i] = CDarkSword::Create(m_pGraphicDev, L"DarkSwordOutLine");
		m_pDarkOutLineSword[i]->SetAni(false);

		m_pDarkMiddleLineSword[i] = CDarkSword::Create(m_pGraphicDev, L"DardSwordMiddleLine");
		m_pDarkMiddleLineSword[i]->SetAni(false);

		m_pCenterSword[i] = CDarkSword::Create(m_pGraphicDev, L"DarkCenterLine");
		m_pCenterSword[i]->SetAni(false);
	}

	return S_OK;
}

CDarkPollingGrave* CDarkPollingGrave::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CDarkPollingGrave*		pInstance = new CDarkPollingGrave(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("DarkPollingGrave Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDarkPollingGrave::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pWave);
	Engine::Safe_Release(m_pBlueFlowerWave);

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pWhiteLine[i]);
	}

	Engine::Safe_Release(m_pBlackLine);
	Engine::Safe_Release(m_pRuneOutLine);
	Engine::Safe_Release(m_pMainRune);
	Engine::Safe_Release(m_pRedCircle);

	for (int i = 0; i < 20; ++i)
	{
		Engine::Safe_Release(m_pDarkSword[i]);
		Engine::Safe_Release(m_pDarkOutLineSword[i]);
		Engine::Safe_Release(m_pDarkMiddleLineSword[i]);
		Engine::Safe_Release(m_pCenterSword[i]);
	}

}

HRESULT CDarkPollingGrave::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CDarkPollingGrave::SkillStart(void)
{
	if (m_bAni)
		return;

	//초기에 퍼져나가는 검은 웨이브
	//BlackWave==================================================
	_vec3 Pos = m_pTargetTransform->Get_Now_Position();
	Pos.y += 2.f;
	m_pWave->GetTransCom()->Set_Position(&Pos);
	m_pWave->SetAni(true);
	_vec3 Scale = _vec3(1.0f, 1.0f, 1.0f);
	m_pWave->SetScale(Scale);
	_vec4 color = _vec4(0.f, 0.f, 0.f, 2.f);
	m_pWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)color;
	//============================================================


	//초기에 퍼져나가는 흰색 웨이브
	//WhiteWave===================================================
	_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 2.0f);

	for (int i = 0; i < 2; ++i)
	{
		_vec3 vWhitePos = m_pTargetTransform->Get_Now_Position();
		vWhitePos.y += 0.5f;
		m_pWhiteLine[i]->SetAni(true);
		m_pWhiteLine[i]->GetTransCom()->Set_Position(&vWhitePos);
		m_pWhiteLine[i]->GetTransCom()->Set_Scale((i * 1.f) + 1.f, (i * 1.f) + 1.f, (i * 1.f) + 1.f);
		m_pWhiteLine[i]->GetTransCom()->Set_Angle(&vAngle);
		m_pWhiteLine[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	}
	//============================================================



	//BlackLine===================================================
	_vec4 BlackColor = _vec4(0.0f, 0.0f, 0.0f, 3.0f);
	_vec3 vBlackPos = m_pTargetTransform->Get_Now_Position();
	vBlackPos.y += 0.5f;
	m_pBlackLine->SetAni(true);
	m_pBlackLine->GetTransCom()->Set_Position(&vBlackPos);
	m_pBlackLine->GetTransCom()->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pBlackLine->GetTransCom()->Set_Angle(&vAngle);
	m_pBlackLine->GetParticleInfo()->m_dColor = (D3DXCOLOR)BlackColor;
	//============================================================




	//BlueFlower==================================================
	_vec3 Rotate = _vec3(90.f, 0.0f, 0.0f);
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos.y += 0.6f;

	Scale = _vec3(2.5f, 2.5f, 2.5f);
	color = _vec4(-1.f, -0.2f, 1.f, 3.f);

	m_pBlueFlowerWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)color;
	m_pBlueFlowerWave->SetScale(Scale);
	m_pBlueFlowerWave->GetTransCom()->Set_Position(&vPos);
	m_pBlueFlowerWave->GetTransCom()->Set_Angle(&Rotate);;
	m_pBlueFlowerWave->SetAni(true);
	m_pBlueFlowerWave->GetParticleInfo()->m_iPath = 2;
	//============================================================

	m_bAni = true;
}

void CDarkPollingGrave::EndAniCheck(void)
{
	if (m_bAni == true && m_pWave->GetAni() == false && m_pBlueFlowerWave->GetAni() == false && m_pWhiteLine[0]->GetAni() == false 
		&& m_pWhiteLine[1]->GetAni() == false && m_pBlackLine->GetAni() == false && m_pRuneOutLine->GetAni() == false && m_pRedCircle->GetAni() == false)
		m_bAni = false;
}

HRESULT CDarkPollingGrave::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");
	if (pGameObject == NULL)
		return E_FAIL;

	//Player의 행동정보를 가져온다.
	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();



	Engine::Safe_Release(pGameObject);
	return S_OK;

}


//초기에 퍼져나가는 검은 이펙트 Update
void CDarkPollingGrave::BlackWaveUpdate(void)
{
	if (m_pWave->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pWave->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pWave->SetAni(false);
		}
		else
		{
			vColor.w -= 0.2f;
			m_pWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pWave->GetTransCom()->Add_Scale(0.2f);
		}

	}
}

//스킬사용 시  초기에 퍼져나가는 흰색 아우라 
void CDarkPollingGrave::WhiteLineUpdate(void)
{
	if (m_pWhiteLine[0]->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pWhiteLine[0]->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pWhiteLine[0]->SetAni(false);
		}
		else
		{
			vColor.w -= 0.08f;
			m_pWhiteLine[0]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pWhiteLine[0]->GetTransCom()->Add_Scale(0.1f);
		}

	}


	if (m_pWhiteLine[1]->GetAni() == true)
	{
		_vec4 vColor = (_vec4)m_pWhiteLine[1]->GetParticleInfo()->m_dColor;

		if (vColor.w <= 0.f)
		{
			m_pWhiteLine[1]->SetAni(false);
		}
		else
		{
			vColor.w -= 0.08f;
			m_pWhiteLine[1]->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pWhiteLine[1]->GetTransCom()->Add_Scale(0.1f);
		}
	}
}

//스킬 초기에 생겨나는 검은색 원 웨이브
void CDarkPollingGrave::BlackLineUpdate(void)
{
	if (m_pBlackLine->GetAni() == true)
	{
		_vec3 vScale;
		m_pBlackLine->GetTransCom()->Get_Scale(&vScale);

		if (vScale.x <= 2.f)
		{
			m_pBlackLine->GetTransCom()->Add_Scale(0.05f);
		}
		else
		{
			_vec4 color = _vec4(0.f, 0.f, 0.f, 3.f);
			m_pBlueFlowerWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)color;
			m_pBlueFlowerWave->GetParticleInfo()->m_iPath = 3;
			m_bBlueFlowerCheck = true;
		}
	}

}

//BlueFlower 꽃모양 웨이브 Update
void CDarkPollingGrave::BlueFlowerUpdate(void)
{
	if (m_pBlueFlowerWave->GetAni() == true && (m_bBlueFlowerCheck == true))
	{
		_vec3 vScale;
		m_pBlueFlowerWave->GetTransCom()->Get_Scale(&vScale);

		if (vScale.x <= 0.0f)
		{
			m_bBlueFlowerCheck = false;
			m_pBlackLine->SetAni(false);
			m_pBlueFlowerWave->SetAni(false);
			RuneOutLine();
		}
		else
		{
			m_pBlueFlowerWave->GetTransCom()->Add_Scale(-0.03f);
		}
	}
}

void CDarkPollingGrave::RuneOutLine(void)
{

	//RuneBlackLine===============================================
	_vec3 Look;
	m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);
	D3DXVec3Normalize(&Look, &Look);
	Look *= 3.0f;

	_vec3 Rotate = _vec3(90.f, 0.f, 0.f);
	_vec4 Color = _vec4(0.0f, 0.0f, 0.0f, 2.0f);
	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	vPos += Look;
	vPos.y += 0.5f;
	m_pRuneOutLine->SetAni(true);
	m_pRuneOutLine->GetTransCom()->Set_Position(&vPos);
	m_pRuneOutLine->GetTransCom()->Set_Scale(4.5f, 4.5f, 4.5f);
	m_pRuneOutLine->GetTransCom()->Set_Angle(&Rotate);
	m_pRuneOutLine->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	//============================================================


	//MainRune======================================================
	m_pMainRune->SetAni(true);
	m_pMainRune->GetTransCom()->Set_Position(&vPos);
	m_pMainRune->GetTransCom()->Set_Scale(2.5f, 2.5f, 2.5f);
	m_pMainRune->GetTransCom()->Set_Angle(&Rotate);
	Color = _vec4(0.0f, 0.0f, 0.0f, 3.0f);
	m_pMainRune->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

	//===============================================================

	//Red Circle====================================================
	m_pRedCircle->SetAni(true);
	m_pRedCircle->GetTransCom()->Set_Position(&vPos);
	m_pRedCircle->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);

	m_pRedCircle->GetTransCom()->Set_Angle(&Rotate);
	Color = _vec4(1.0f, -0.2f, -0.2f, 3.0f);
	m_pRedCircle->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	//==============================================================

	//DarkSword=======================================================
	m_bDarkSwordCheck = true;
	m_bCenterShoot = true;
	//==================================================================

}

void CDarkPollingGrave::RuneOutLineUpdate(void)
{
	if (m_pRuneOutLine->GetAni() == true)
	{
		m_pRuneOutLine->GetTransCom()->Rotation(Engine::ROT_Y, 1.f);

		if (m_bDarkPollingSwordEnd)
		{
			_vec4 vColor = (_vec4)m_pRuneOutLine->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pRuneOutLine->SetAni(false);
			}
			else
			{
				vColor.w -= 0.04f;

				m_pRuneOutLine->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			}
		}
	}

	if (m_pMainRune->GetAni() == true)
	{
		m_pMainRune->GetTransCom()->Rotation(Engine::ROT_Y, -0.5f);

		if (m_bDarkPollingSwordEnd)
		{
			_vec4 vColor = (_vec4)m_pMainRune->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pMainRune->SetAni(false);
			}
			else
			{
				vColor.w -= 0.04f;

				m_pMainRune->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			}
		}
	}


	if (m_pRedCircle->GetAni() == true)
	{
		if (m_bDarkPollingSwordEnd)
		{
			_vec4 vColor = (_vec4)m_pRedCircle->GetParticleInfo()->m_dColor;

			if (vColor.w <= 0.f)
			{
				m_pRedCircle->SetAni(false);
				m_bCenterSwordCheck = false;
				m_bCenterShoot = false;
				m_bDarkPollingSwordEnd = false;
				m_bDarkSwordCheck = false;
				m_bSwordTargetCheck = false;
				m_pBlackLine->SetAni(false);
				m_bSkillCheck = false;
				m_bBlueFlowerCheck = false;
			}
			else
			{
				vColor.w -= 0.03f;
				m_pRedCircle->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			}

		}
	}

}


//CenterSword Update
void CDarkPollingGrave::CenterSwordUpdate(const _float& fTimeDelta)
{
	if (!m_bCenterSwordCheck) //이펙트 발동이 아직 false이면
		return;

	for (int i = 0; i < 20; ++i)
	{
		_vec3 vPosition;
		_vec3 Look;

		_matrix vAxisMatrix;
	
		//아직 검에 꽂히는 순간이 아닌경우 -> Player를 계속 쫒아다닌다.
		//========================================================================
		if (!m_bCenterShoot)
		{
			m_pCenterSword[i]->SetAni(true);

			m_pCenterSword[i]->GetTransCom()->Set_Position(&m_vCenterSwordPosition[i]);
			_vec3 vAngle = _vec3(90.f, 0.f, -90.f);
			m_pCenterSword[i]->GetTransCom()->Set_Angle(&vAngle);

			m_pTargetTransform->Get_WorldMatrix(&CenterMatrix); 
		}
		//=====================================================================

		m_pCenterSword[i]->Update_Object(fTimeDelta);

		m_pCenterSword[i]->GetTransCom()->Get_WorldMatrix(&CenterResultMatrix);

		CenterResultMatrix = CenterResultMatrix *  CenterMatrix;

		//최종 결과 matrix를 검의 월드 행렬에 넣어준다.
		m_pCenterSword[i]->GetTransCom()->Set_WorldMatrix(&CenterResultMatrix);



		if (m_bCenterShoot)//검이 땅에 꽂히는 순간
		{
			_vec3 vPos = m_pCenterSword[i]->GetTransCom()->Get_Now_Position();

			_vec3 vGoal = _vec3(vPos.x, 10.0f, vPos.z);

			_vec3 vDir = vGoal - vPos;

			_float fDis = D3DXVec3Length(&vDir);

			D3DXVec3Normalize(&vDir, &vDir);

			if (fDis > 1.f)//두점 사이의 거리가 1.f보다 크다면 계속 이동
			{
				vPos += vDir * (((rand() % 10) * 0.3f) + 1.3f);

				m_pCenterSword[i]->GetTransCom()->Set_Position(&vPos);
			}
			else //두점 사이의 거리가 1.f보다 작다면
			{
				if(m_bDarkPollingSwordEnd) //종료 변수가 True이면
				{
					_vec3 vScale;
					m_pCenterSword[i]->GetTransCom()->Get_Scale(&vScale);


					if (vScale.z > 0.f)
						vScale.z -= m_fCenterSwordNumber[i];
					else
						vScale.z = 0.f;

					if (vScale.y > 0.f)
						vScale.y -= m_fCenterSwordNumber[i];
					else
						vScale.y = 0.0f;

					m_pCenterSword[i]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);

					if (vScale.x <= 0.f && vScale.y <= 0.f)
						m_pCenterSword[i]->SetAni(false);

				}
			}
		}
	}

}

//InsideSword Update
void CDarkPollingGrave::InsideSwordUpdate(const _float & fTimeDelta)
{
	if (!m_bCenterSwordCheck) //이펙트 발동이 아직 false이면
		return;

	for (int i = 0; i < 20; ++i)
	{
		_vec3 vPosition;
		_vec3 Look;

		//아직 검에 꽂히는 순간이 아닌경우 -> Player를 계속 쫒아다닌다.
		//========================================================================
		if (!m_bCenterShoot)
		{
			m_pDarkSword[i]->SetAni(true);

	
			m_pDarkSword[i]->GetTransCom()->Set_Position(&m_vInsideSwordPosition[i]);
			_vec3 vAngle = _vec3(90.f, 0.f, -90.f);
			m_pDarkSword[i]->GetTransCom()->Set_Angle(&vAngle);

			m_pTargetTransform->Get_WorldMatrix(&CenterMatrix);
		}
		//=========================================================================

		m_pDarkSword[i]->Update_Object(fTimeDelta);

		m_pDarkSword[i]->GetTransCom()->Get_WorldMatrix(&InsideResultMatrix);

		InsideResultMatrix = InsideResultMatrix *  CenterMatrix;

		//최종 결과 matrix를 검의 월드 행렬에 넣어준다.
		m_pDarkSword[i]->GetTransCom()->Set_WorldMatrix(&InsideResultMatrix);

		if (m_bCenterShoot)//검이 땅에 꽂히는 순간
		{
			_vec3 vPos = m_pDarkSword[i]->GetTransCom()->Get_Now_Position();

			_vec3 vGoal = _vec3(vPos.x, 10.0f, vPos.z);

			_vec3 vDir = vGoal - vPos;

			_float fDis = D3DXVec3Length(&vDir);

			D3DXVec3Normalize(&vDir, &vDir);


			if (fDis > 1.f)//두점 사이의 거리가 1.f보다 크다면 계속 이동
			{
				vPos += vDir * (((rand() % 10) * 0.3f) + 1.3f);

				m_pDarkSword[i]->GetTransCom()->Set_Position(&vPos);
			}
			else //두점 사이의 거리가 1.f보다 작다면
			{
				if (m_bDarkPollingSwordEnd) //종료 변수가 True이면
				{
					_vec3 vScale;
					m_pDarkSword[i]->GetTransCom()->Get_Scale(&vScale);

					if (vScale.z > 0.f)
						vScale.z -= m_pInsideSwordNumber[i];
					else
						vScale.z = 0.f;

					if (vScale.y > 0.f)
						vScale.y -= m_pInsideSwordNumber[i];
					else
						vScale.y = 0.0f;

					m_pDarkSword[i]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);

					if (vScale.x <= 0.f && vScale.y <= 0.f)
						m_pDarkSword[i]->SetAni(false);
				}
			}

		}

	}

}

//MiddleSword Update
void CDarkPollingGrave::MiddleSwordUpdate(const _float & fTimeDelta)
{
	if (!m_bCenterSwordCheck) //이펙트 발동이 아직 false이면
		return;

	for (int i = 0; i < 20; ++i)
	{
		_vec3 vPosition;
		_vec3 Look;

		//아직 검에 꽂히는 순간이 아닌경우 -> Player를 계속 쫒아다닌다.
		//========================================================================
		if (!m_bCenterShoot)
		{
			m_pDarkMiddleLineSword[i]->SetAni(true);


			m_pDarkMiddleLineSword[i]->GetTransCom()->Set_Position(&m_vMiddleSwordPosition[i]);
			_vec3 vAngle = _vec3(90.f, 0.f, -90.f);
			m_pDarkMiddleLineSword[i]->GetTransCom()->Set_Angle(&vAngle);

			m_pTargetTransform->Get_WorldMatrix(&CenterMatrix);
		}
		//=========================================================================

		m_pDarkMiddleLineSword[i]->Update_Object(fTimeDelta);

		m_pDarkMiddleLineSword[i]->GetTransCom()->Get_WorldMatrix(&MiddleResultMatrix);

		MiddleResultMatrix = MiddleResultMatrix *  CenterMatrix;

		//최종 결과 matrix를 검의 월드 행렬에 넣어준다.
		m_pDarkMiddleLineSword[i]->GetTransCom()->Set_WorldMatrix(&MiddleResultMatrix);

		if (m_bCenterShoot)//검이 땅에 꽂히는 순간
		{
			_vec3 vPos = m_pDarkMiddleLineSword[i]->GetTransCom()->Get_Now_Position();

			_vec3 vGoal = _vec3(vPos.x, 7.0f, vPos.z);

			_vec3 vDir = vGoal - vPos;

			_float fDis = D3DXVec3Length(&vDir);

			D3DXVec3Normalize(&vDir, &vDir);


			if (fDis > 1.f)//두점 사이의 거리가 1.f보다 크다면 계속 이동
			{
				vPos += vDir * (((rand() % 10) * 0.3f) + 1.3f);

				m_pDarkMiddleLineSword[i]->GetTransCom()->Set_Position(&vPos);
			}
			else //두점 사이의 거리가 1.f보다 작다면
			{
				if (m_bDarkPollingSwordEnd) //종료 변수가 True이면
				{
					_vec3 vScale;
					m_pDarkMiddleLineSword[i]->GetTransCom()->Get_Scale(&vScale);

					if (vScale.z > 0.f)
						vScale.z -= m_pMiddleSwordNumber[i];
					else
						vScale.z = 0.f;

					if (vScale.y > 0.f)
						vScale.y -= m_pMiddleSwordNumber[i];
					else
						vScale.y = 0.0f;

					m_pDarkMiddleLineSword[i]->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);

					if (vScale.x <= 0.f && vScale.y <= 0.f)
						m_pDarkMiddleLineSword[i]->SetAni(false);
				}
			}

		}

	}

}


//OutSword
void CDarkPollingGrave::OutSwordUpdate(const _float & fTimeDelta)
{
	if (!m_bCenterSwordCheck) //이펙트 발동이 아직 false이면
		return;

	for (int i = 0; i < 20; ++i)
	{
		_vec3 vPosition;
		_vec3 Look;

		//아직 검에 꽂히는 순간이 아닌경우 -> Player를 계속 쫒아다닌다.
		//========================================================================
		if (!m_bCenterShoot)
		{
			m_pDarkOutLineSword[i]->SetAni(true);

			m_pDarkOutLineSword[i]->GetTransCom()->Set_Position(&m_vOutSwordPosition[i]);
			_vec3 vAngle = _vec3(90.f, 0.f, -90.f);
			m_pDarkOutLineSword[i]->GetTransCom()->Set_Angle(&vAngle);
			m_pTargetTransform->Get_WorldMatrix(&CenterMatrix);
		}
		//=========================================================================

		m_pDarkOutLineSword[i]->Update_Object(fTimeDelta);

		m_pDarkOutLineSword[i]->GetTransCom()->Get_WorldMatrix(&OutResultMatrix);

		OutResultMatrix = OutResultMatrix *  CenterMatrix;

		//최종 결과 matrix를 검의 월드 행렬에 넣어준다.
		m_pDarkOutLineSword[i]->GetTransCom()->Set_WorldMatrix(&OutResultMatrix);

		if (m_bCenterShoot)//검이 땅에 꽂히는 순간
		{
			_vec3 vPos = m_pDarkOutLineSword[i]->GetTransCom()->Get_Now_Position();

			_vec3 vGoal = _vec3(vPos.x, 8.0f, vPos.z);

			_vec3 vDir = vGoal - vPos;

			_float fDis = D3DXVec3Length(&vDir);

			D3DXVec3Normalize(&vDir, &vDir);


			if (fDis > 1.f)//두점 사이의 거리가 1.f보다 크다면 계속 이동
			{
				vPos += vDir * (((rand() % 10) * 0.3f) + 1.3f);

				m_pDarkOutLineSword[i]->GetTransCom()->Set_Position(&vPos);
			}
			else //두점 사이의 거리가 1.f보다 작다면
			{
				if (m_bDarkPollingSwordEnd) //종료 변수가 True이면
				{
					_vec3 vScale;
					m_pDarkOutLineSword[i]->GetTransCom()->Get_Scale(&vScale);

					if (vScale.z > 0.f)
						vScale.z -= m_fOutSwordNumber[i];
					else
						vScale.z = 0.f;

					if (vScale.y > 0.f)
						vScale.y -= m_fOutSwordNumber[i];
					else
						vScale.y = 0.0f;

					m_pDarkOutLineSword[i]->GetTransCom()->Set_Scale(vScale.x, vScale.y,vScale.z);

					if (vScale.x <= 0.f && vScale.y <= 0.f)
						m_pDarkOutLineSword[i]->SetAni(false);
				}
			}

		}

	}
}