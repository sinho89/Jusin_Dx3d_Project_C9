#include "stdafx.h"
#include "PledgeBlade.h"
#include "Export_Engine.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"
#include "PledgeRing.h"
#include "Sprite.h"
#include "Wave.h"

CPledgeBlade::CPledgeBlade(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pvecAniInfo(NULL)
,m_pTargetMesh(NULL)
,m_pTargetTransform(NULL)
,m_pLeftOne(NULL)
,m_pRightTwo(NULL)
,m_bleftOne(false)
,m_bRightTwo(false)
,m_pLeftThree(NULL)
,m_pRightFour(NULL)
,m_bLeftThree(false)
,m_fLeftOneGage(0.0f)
,m_fRightTwoGage(0.0f)
,m_fLeftThreeGage(0.0f)
,m_fRightFourGage(0.0f)
,m_bRightFour(false)
,m_pLightSprite(NULL)
,m_pWhiteWave(NULL)
,m_pWhiteWaveTwo(NULL)
,m_pWhiteWaveThree(NULL)
,m_pWhiteWaveFour(NULL)
{
	m_bIsStatic = true;
	m_bAni = false;

	for (int i = 0; i < 4; ++i)
		m_pHitBlade[i] = NULL;
}

CPledgeBlade::~CPledgeBlade(void)
{
}

void CPledgeBlade::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayerAction[CWitchBlade::ACTION_PLEDGEBLADE] == true)
	{
		 m_bAni = true;

		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime > 0.6113f && (m_bleftOne == false)) //첫번째 칼날 Update
		{
			m_bleftOne = true;
			LeftOneInit();
			SkillStart(0);
		}

		if (dTargetTime > 1.5f && (m_bRightTwo == false))	//두번째 칼날 Update
		{
			m_bRightTwo = true;
			RightTwoInit();
			SkillStart(1);
		}

		if (dTargetTime > 2.358f && (m_bLeftThree == false)) //세번째 칼날 Update
		{
			m_bLeftThree = true;
			LeftThreeInit();
			SkillStart(2);
		}
		if (dTargetTime > 3.231f && (m_bRightFour == false)) //네번째 칼날 Update
		{
			m_bRightFour = true;
			RightFourInit();
			SkillStart(3);
		}
	}
	else
	{
		m_bleftOne = false;
		m_bRightTwo = false;
		m_bLeftThree = false;
		m_bRightFour = false;
	}

	if (!m_bAni)
		return;

	EndAniCheck();

	LightUpdate();

	WhiteUpdate();


	m_pLightSprite->Update_Object(fTimeDelta);

	m_pWhiteWave->Update_Object(fTimeDelta);
	m_pWhiteWaveTwo->Update_Object(fTimeDelta);
	m_pWhiteWaveThree->Update_Object(fTimeDelta);
	m_pWhiteWaveFour->Update_Object(fTimeDelta);

	for (int i = 0; i < 4; ++i)
		m_pHitBlade[i]->Update_Object(fTimeDelta);

	RightTwoUpdate(fTimeDelta);
	LeftOneUpdate(fTimeDelta);
	LeftThreeUpdate(fTimeDelta);
	RightFourUpdate(fTimeDelta);


}

void CPledgeBlade::Render_Object(void)
{
	m_pLightSprite->Render_Object();
	m_pWhiteWave->Render_Object();
	m_pWhiteWaveTwo->Render_Object();
	m_pWhiteWaveThree->Render_Object();
	m_pWhiteWaveFour->Render_Object();

	for (int i = 0; i < 4; ++i)
		m_pHitBlade[i]->Render_Object();

	m_pRightTwo->Render_Object();
	m_pLeftOne->Render_Object();
	m_pLeftThree->Render_Object();
	m_pRightFour->Render_Object();
}

HRESULT CPledgeBlade::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//LeftOne======================================================//
	_vec4 Color = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pLeftOne = CPledgeRing::Create(m_pGraphicDev, L"LeftOne", 40);
	m_pLeftOne->SetAni(false);
	m_pLeftOne->SetRotSpeed(0.f);
	m_pLeftOne->SetRadius(10.0f, 2.0f);
	m_pLeftOne->SetTrailY(0.0f);
	m_pLeftOne->SetTextureNumber(1);
	m_pLeftOne->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLeftOne->GetParticleInfo()->m_iPath = 3;
	m_pLeftOne->SetRenderCheck(true);
	m_pLeftOne->SetIntervalAngle(10.f);
	m_pLeftOne->SettingTrail();
	_vec3 vAngle = _vec3(0.f, -90.f, -40.f);
	m_pLeftOne->GetTransCom()->Set_Angle(&vAngle);
	m_pLeftOne->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);

	m_pLeftOne->SetEffectBlurCheck(true);
	m_pLeftOne->SetBlurPass(3);
	m_pLeftOne->SetBlurCnt(4);
	m_pLeftOne->SetBlurPower(1.1f);
	m_pLeftOne->SetBlurValue(150.0f);
	m_pLeftOne->SetBlurType(Engine::BLUR_HORIZON);

	//===========================================================//


	//RightTwo=====================================================
	Color = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pRightTwo = CPledgeRing::Create(m_pGraphicDev, L"RightTwo", 40);
	m_pRightTwo->SetAni(false);
	m_pRightTwo->SetRotSpeed(0.f);
	m_pRightTwo->SetRadius(10.0f, 5.0f);
	m_pRightTwo->SetTrailY(0.0f);
	m_pRightTwo->SetTextureNumber(1);
	m_pRightTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRightTwo->GetParticleInfo()->m_iPath = 3;
	m_pRightTwo->SetRenderCheck(true);
	m_pRightTwo->SetIntervalAngle(10.f);
	m_pRightTwo->SettingTrail();
	vAngle = _vec3(0.f, -160.f, 180.f);
	m_pRightTwo->GetTransCom()->Set_Angle(&vAngle);
	m_pRightTwo->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);

	m_pRightTwo->SetEffectBlurCheck(true);
	m_pRightTwo->SetBlurPass(3);
	m_pRightTwo->SetBlurCnt(4);
	m_pRightTwo->SetBlurPower(1.1f);
	m_pRightTwo->SetBlurValue(150.0f);
	m_pRightTwo->SetBlurType(Engine::BLUR_HORIZON);
	//==============================================================

	//LeftThree
	//==============================================================
	Color = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pLeftThree = CPledgeRing::Create(m_pGraphicDev, L"LeftThree", 40);
	m_pLeftThree->SetAni(false);
	m_pLeftThree->SetRotSpeed(0.f);
	m_pLeftThree->SetRadius(10.0f, 2.0f);
	m_pLeftThree->SetTrailY(0.0f);
	m_pLeftThree->SetTextureNumber(1);
	m_pLeftThree->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pLeftThree->GetParticleInfo()->m_iPath = 3;
	m_pLeftThree->SetRenderCheck(true);
	m_pLeftThree->SetIntervalAngle(10.f);
	m_pLeftThree->SettingTrail();
	vAngle = _vec3(0.f, 200.f, 40.f);
	m_pLeftThree->GetTransCom()->Set_Angle(&vAngle);
	m_pLeftThree->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);

	m_pLeftThree->SetEffectBlurCheck(true);
	m_pLeftThree->SetBlurPass(3);
	m_pLeftThree->SetBlurCnt(4);
	m_pLeftThree->SetBlurPower(1.1f);
	m_pLeftThree->SetBlurValue(150.0f);
	m_pLeftThree->SetBlurType(Engine::BLUR_HORIZON);
	//================================================================


	//RightFour=======================================================
	Color = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pRightFour = CPledgeRing::Create(m_pGraphicDev, L"RightFour", 40);
	m_pRightFour->SetAni(false);
	m_pRightFour->SetRotSpeed(0.f);
	m_pRightFour->SetRadius(10.0f, 2.0f);
	m_pRightFour->SetTrailY(0.0f);
	m_pRightFour->SetTextureNumber(1);
	m_pRightFour->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pRightFour->GetParticleInfo()->m_iPath = 3;
	m_pRightFour->SetRenderCheck(true);
	m_pRightFour->SetIntervalAngle(10.f);
	m_pRightFour->SettingTrail();
	vAngle = _vec3(180.f,150.f, -50.f);
	m_pRightFour->GetTransCom()->Set_Angle(&vAngle);
	m_pRightFour->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);

	m_pRightFour->SetEffectBlurCheck(true);
	m_pRightFour->SetBlurPass(3);
	m_pRightFour->SetBlurCnt(4);
	m_pRightFour->SetBlurPower(1.1f);
	m_pRightFour->SetBlurValue(150.0f);
	m_pRightFour->SetBlurType(Engine::BLUR_HORIZON);
	//================================================================


	//Light=============================================================
	m_pLightSprite = CSprite::LoadCreate(m_pGraphicDev, L"PledgeSprite");
	m_pLightSprite->SetAni(false);
	m_pLightSprite->GetParticleInfo()->m_iPath = 3;
	m_pLightSprite->SetRenderCheck(true);
	m_pLightSprite->GetTransCom()->Set_Scale(3.5f, 3.5f, 3.5f);
	Color = _vec4(0.0f, 0.0f, 0.0f, 3.f);
	m_pLightSprite->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	//==================================================================


	//White=============================================================
	m_pWhiteWave = CWave::Create(m_pGraphicDev, L"WhiteCircle");
	m_pWhiteWave->SetAni(false);
	m_pWhiteWave->SetPattern(CWave::PATTERN_END);
	m_pWhiteWave->SetBilBoardCheck(true);
	m_pWhiteWave->SetRendererCheck(true);
	m_pWhiteWave->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	Color = _vec4(1.0f, 1.0f, 1.f, 2.f);
	m_pWhiteWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWhiteWave->GetParticleInfo()->m_iPath = 2;
	m_pWhiteWave->GetParticleInfo()->m_iTextureNumber = 34;

	m_pWhiteWave->SetBlurCheck(true);
	m_pWhiteWave->SetBlurPass(2);
	m_pWhiteWave->SetBlurCnt(4);
	m_pWhiteWave->SetBlurPower(1.3f);
	m_pWhiteWave->SetBlurValue(128.0f);
	m_pWhiteWave->SetBlurType(Engine::BLUR_STAR);
	//==================================================================

	//White2=============================================================
	m_pWhiteWaveTwo = CWave::Create(m_pGraphicDev, L"WhiteCircle");
	m_pWhiteWaveTwo->SetAni(false);
	m_pWhiteWaveTwo->SetPattern(CWave::PATTERN_END);
	m_pWhiteWaveTwo->SetBilBoardCheck(true);
	m_pWhiteWaveTwo->SetRendererCheck(true);
	m_pWhiteWaveTwo->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	Color = _vec4(1.0f, 1.0f, 1.f, 2.f);
	m_pWhiteWaveTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWhiteWaveTwo->GetParticleInfo()->m_iPath = 2;
	m_pWhiteWaveTwo->GetParticleInfo()->m_iTextureNumber = 34;

	m_pWhiteWaveTwo->SetBlurCheck(true);
	m_pWhiteWaveTwo->SetBlurPass(2);
	m_pWhiteWaveTwo->SetBlurCnt(4);
	m_pWhiteWaveTwo->SetBlurPower(1.3f);
	m_pWhiteWaveTwo->SetBlurValue(128.0f);
	m_pWhiteWaveTwo->SetBlurType(Engine::BLUR_STAR);
	//==================================================================

	m_pWhiteWaveThree = CWave::Create(m_pGraphicDev, L"WhiteCircle");
	m_pWhiteWaveThree->SetAni(false);
	m_pWhiteWaveThree->SetPattern(CWave::PATTERN_END);
	m_pWhiteWaveThree->SetBilBoardCheck(true);
	m_pWhiteWaveThree->SetRendererCheck(true);
	m_pWhiteWaveThree->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	Color = _vec4(1.0f, 1.0f, 1.f, 2.f);
	m_pWhiteWaveThree->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWhiteWaveThree->GetParticleInfo()->m_iPath = 2;
	m_pWhiteWaveThree->GetParticleInfo()->m_iTextureNumber = 8;

	m_pWhiteWaveThree->SetBlurCheck(true);
	m_pWhiteWaveThree->SetBlurPass(2);
	m_pWhiteWaveThree->SetBlurCnt(4);
	m_pWhiteWaveThree->SetBlurPower(1.3f);
	m_pWhiteWaveThree->SetBlurValue(128.0f);
	m_pWhiteWaveThree->SetBlurType(Engine::BLUR_STAR);
	//==================================================================

	//Four
	m_pWhiteWaveFour = CWave::Create(m_pGraphicDev, L"WhiteCircle");
	m_pWhiteWaveFour->SetAni(false);
	m_pWhiteWaveFour->SetPattern(CWave::PATTERN_END);
	m_pWhiteWaveFour->SetBilBoardCheck(true);
	m_pWhiteWaveFour->SetRendererCheck(true);
	m_pWhiteWaveFour->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
	Color = _vec4(1.0f, 1.0f, 1.f, 2.f);
	m_pWhiteWaveFour->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pWhiteWaveFour->GetParticleInfo()->m_iPath = 2;
	m_pWhiteWaveFour->GetParticleInfo()->m_iTextureNumber = 8;

	m_pWhiteWaveFour->SetBlurCheck(true);
	m_pWhiteWaveFour->SetBlurPass(2);
	m_pWhiteWaveFour->SetBlurCnt(4);
	m_pWhiteWaveFour->SetBlurPower(1.3f);
	m_pWhiteWaveFour->SetBlurValue(128.0f);
	m_pWhiteWaveFour->SetBlurType(Engine::BLUR_STAR);


	for (int i = 0; i < 4; ++i)
	{
		m_pHitBlade[i] = CWave::Create(m_pGraphicDev, L"WhiteHitEffect");
		m_pHitBlade[i]->SetAni(false);
		m_pHitBlade[i]->SetPattern(CWave::PATTERN_END);
		m_pHitBlade[i]->SetBilBoardCheck(true);
		m_pHitBlade[i]->SetRendererCheck(true);
		m_pHitBlade[i]->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);
		Color = _vec4(1.0f, 1.0f, 1.f, 2.f);
		m_pHitBlade[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pHitBlade[i]->GetParticleInfo()->m_iPath = 2;
		m_pHitBlade[i]->GetParticleInfo()->m_iTextureNumber = 8;

		m_pHitBlade[i]->SetBlurCheck(true);
		m_pHitBlade[i]->SetBlurPass(2);
		m_pHitBlade[i]->SetBlurCnt(4);
		m_pHitBlade[i]->SetBlurPower(1.2f);
		m_pHitBlade[i]->SetBlurValue(128.0f);
		m_pHitBlade[i]->SetBlurType(Engine::BLUR_STAR);
	}

	return S_OK;
}

CPledgeBlade * CPledgeBlade::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CPledgeBlade*		pInstance = new CPledgeBlade(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("PledgeBlade Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPledgeBlade::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pLeftOne);
	Engine::Safe_Release(m_pRightTwo);
	Engine::Safe_Release(m_pLeftThree);
	Engine::Safe_Release(m_pRightFour);
	Engine::Safe_Release(m_pLightSprite);
	Engine::Safe_Release(m_pWhiteWave);
	Engine::Safe_Release(m_pWhiteWaveTwo);
	Engine::Safe_Release(m_pWhiteWaveThree);
	Engine::Safe_Release(m_pWhiteWaveFour);

	for (int i = 0; i < 4; ++i)
	{
		Engine::Safe_Release(m_pHitBlade[i]);
	}

}

HRESULT CPledgeBlade::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CPledgeBlade::SkillStart(int iNumber)
{
	m_pLightSprite->SetAni(true);

	_vec3 vPos = m_pTargetTransform->Get_Now_Position();
	_vec3 Look;

	m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);

	Look *= 8.f;

	vPos += Look;

	vPos.y += 5.f;


	m_pLightSprite->GetTransCom()->Set_Position(&vPos);

	_vec4 Color = _vec4(0.0f, 0.0f, 0.0f, 3.f);
	m_pLightSprite->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;


	switch (iNumber)
	{
		case 0:
		{
			_vec3 vPos = m_pTargetTransform->Get_Now_Position();
			_vec3 Look;
			m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);

			Look *= 7.5f;

			vPos += Look;

			vPos.y += 5.f;

			
			m_pWhiteWave->SetAni(true);
			m_pWhiteWave->GetTransCom()->Set_Position(&vPos);
			Color = _vec4(1.f, 1.f, 1.f, 1.f);
			m_pWhiteWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pWhiteWave->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);

			m_pHitBlade[0]->SetAni(true);
			m_pHitBlade[0]->GetTransCom()->Set_Position(&vPos);
			m_pHitBlade[0]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pHitBlade[0]->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);
		}
		break;

		case 1:
		{
			_vec3 vPos = m_pTargetTransform->Get_Now_Position();
			_vec3 Look;
			m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);

			Look *= 7.5f;

			vPos += Look;

			vPos.y += 5.f;

			m_pWhiteWaveTwo->SetAni(true);
			m_pWhiteWaveTwo->GetTransCom()->Set_Position(&vPos);
			_vec4 Color = _vec4(1.f, 1.f, 1.f, 1.f);
			m_pWhiteWaveTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pWhiteWaveTwo->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);

			m_pHitBlade[1]->SetAni(true);
			m_pHitBlade[1]->GetTransCom()->Set_Position(&vPos);
			m_pHitBlade[1]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pHitBlade[1]->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);
		}
		break;


		case 2:
		{
			_vec3 vPos = m_pTargetTransform->Get_Now_Position();
			_vec3 Look;
			m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);

			Look *= 7.5f;

			vPos += Look;

			vPos.y += 5.f;

			m_pWhiteWaveThree->SetAni(true);
			m_pWhiteWaveThree->GetTransCom()->Set_Position(&vPos);
			_vec4 Color = _vec4(1.f, 1.f, 1.f, 1.f);
			m_pWhiteWaveThree->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pWhiteWaveThree->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);

			m_pHitBlade[2]->SetAni(true);
			m_pHitBlade[2]->GetTransCom()->Set_Position(&vPos);
			m_pHitBlade[2]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pHitBlade[2]->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);
		}
			break;


		case 3:
		{
			_vec3 vPos = m_pTargetTransform->Get_Now_Position();
			_vec3 Look;
			m_pTargetTransform->Get_Infomation(Engine::INFO_LOOK, &Look);

			Look *= 7.5f;

			vPos += Look;

			vPos.y += 5.f;

			m_pWhiteWaveFour->SetAni(true);
			m_pWhiteWaveFour->GetTransCom()->Set_Position(&vPos);
			_vec4 Color = _vec4(1.f, 1.f, 1.f, 1.f);
			m_pWhiteWaveFour->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pWhiteWaveFour->GetTransCom()->Set_Scale(0.75f, 0.75f, 0.75f);

			m_pHitBlade[3]->SetAni(true);
			m_pHitBlade[3]->GetTransCom()->Set_Position(&vPos);
			m_pHitBlade[3]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
			m_pHitBlade[3]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);
		}
			break;
	}
}

void CPledgeBlade::EndAniCheck(void)
{
	if (m_bAni == true && m_pLeftOne->GetAni() == false && m_pRightTwo->GetAni() == false && m_pLeftThree->GetAni() == false
		&& m_pRightFour->GetAni() == false && m_pLightSprite->GetAni() == false && m_pWhiteWave->GetAni() == false &&
		m_pWhiteWaveTwo->GetAni() == false && m_pWhiteWaveThree->GetAni() == false && m_pWhiteWaveFour->GetAni() == false)
	{
		m_bAni = false;
	}

}

HRESULT CPledgeBlade::Start_ForScene(void)
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


	Engine::Safe_Release(pGameObject);
	return S_OK;
}


void CPledgeBlade::LeftOneUpdate(const _float & fTimeDelta)
{
	if (m_pLeftOne->GetAni() == false)
		return;

	m_pLeftOne->Update_Object(fTimeDelta);

	_matrix mMat, LeftOneMat;

	_vec3 vPosition;

	vPosition.x = 0.f;
	vPosition.z = 0.f;
	vPosition.y = 3.f;


	//회전
	m_pLeftOne->GetTransCom()->Rotation(Engine::ROT_Y, 20.f);
	m_fLeftOneGage += 20.f;

	m_pLeftOne->GetTransCom()->Set_Position(&vPosition);
	m_pTargetTransform->Get_WorldMatrix(&mMat);
	m_pLeftOne->GetTransCom()->Get_WorldMatrix(&LeftOneMat);

	LeftOneMat = LeftOneMat * mMat;

	m_pLeftOne->GetTransCom()->Set_WorldMatrix(&LeftOneMat);


	_float fGage = m_pLeftOne->GetGage();

	fGage = max(fGage - 0.2f, 0.f);


	m_pLeftOne->SetGage(fGage);


	if (m_fLeftOneGage >= 300.f)
	{
		m_pLeftOne->SetGage(1.f);
		m_pLeftOne->SetAni(false);
		m_fLeftOneGage = 0.0f;
	}

}

void CPledgeBlade::RightTwoUpdate(const _float & fTimeDelta)
{
	if (m_pRightTwo->GetAni() == false)
		return;

	m_pRightTwo->Update_Object(fTimeDelta);

	_matrix mMat, RightTwoMat;
	
	_vec3 vPosition;

	vPosition.x = 0.f;
	vPosition.z = 0.f;
	vPosition.y = 5.f;


	//회전
	m_pRightTwo->GetTransCom()->Rotation(Engine::ROT_Y, 20.f);
	m_fRightTwoGage += 20.f;


	m_pRightTwo->GetTransCom()->Set_Position(&vPosition);
	m_pTargetTransform->Get_WorldMatrix(&mMat);
	m_pRightTwo->GetTransCom()->Get_WorldMatrix(&RightTwoMat);

	RightTwoMat = RightTwoMat * mMat;

	m_pRightTwo->GetTransCom()->Set_WorldMatrix(&RightTwoMat);


	_float fGage = m_pRightTwo->GetGage();

	fGage = max(fGage - 0.2f, 0.f);


	m_pRightTwo->SetGage(fGage);


	if (m_fRightTwoGage >= 300.f)
	{
		m_pRightTwo->SetGage(1.f);
		m_pRightTwo->SetAni(false);
		m_fRightTwoGage = 0.0f;
	}

}

void CPledgeBlade::LeftThreeUpdate(const _float & fTimeDelta)
{
	if (m_pLeftThree->GetAni() == false)
		return;

	m_pLeftThree->Update_Object(fTimeDelta);

	_matrix mMat, LeftThreeMat;

	_vec3 vPosition;

	vPosition.x = 0.f;
	vPosition.z = 0.f;
	vPosition.y = 3.f;


	//회전
	m_pLeftThree->GetTransCom()->Rotation(Engine::ROT_Y, 20.f);
	m_fLeftThreeGage += 20.f;


	m_pLeftThree->GetTransCom()->Set_Position(&vPosition);
	m_pTargetTransform->Get_WorldMatrix(&mMat);
	m_pLeftThree->GetTransCom()->Get_WorldMatrix(&LeftThreeMat);

	LeftThreeMat = LeftThreeMat * mMat;

	m_pLeftThree->GetTransCom()->Set_WorldMatrix(&LeftThreeMat);


	_float fGage = m_pLeftThree->GetGage();

	fGage = max(fGage - 0.1f, 0.f);


	m_pLeftThree->SetGage(fGage);


	if (m_fLeftThreeGage >= 300.f)
	{
		m_pLeftThree->SetGage(1.f);
		m_pLeftThree->SetAni(false);
		m_fLeftThreeGage = 0.0f;
	}


}

void CPledgeBlade::RightFourUpdate(const _float & fTimeDelta)
{
	if (m_pRightFour->GetAni() == false)
		return;

	m_pRightFour->Update_Object(fTimeDelta);

	_matrix mMat, RightFourMat;

	_vec3 vPosition;

	vPosition.x = 0.f;
	vPosition.z = 0.f;
	vPosition.y = 3.f;


	//회전
	m_pRightFour->GetTransCom()->Rotation(Engine::ROT_Y, -20.f);
	m_fRightFourGage += 20.f;

	m_pRightFour->GetTransCom()->Set_Position(&vPosition);
	m_pTargetTransform->Get_WorldMatrix(&mMat);
	m_pRightFour->GetTransCom()->Get_WorldMatrix(&RightFourMat);

	RightFourMat = RightFourMat * mMat;

	m_pRightFour->GetTransCom()->Set_WorldMatrix(&RightFourMat);


	_float fGage = m_pRightFour->GetGage();

	fGage = max(fGage - 0.1f, 0.f);


	m_pRightFour->SetGage(fGage);


	if (m_fRightFourGage >= 300.f)
	{
		m_pRightFour->SetGage(1.f);
		m_pRightFour->SetAni(false);
		m_fRightFourGage = 0.0f;
	}

}

void CPledgeBlade::LeftOneInit(void)
{
	_vec4 Color = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pLeftOne->SetAni(true);
	m_pLeftOne->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	_vec3 vAngle = _vec3(0.f, -90.f, -40.f);
	m_pLeftOne->GetTransCom()->Set_Angle(&vAngle);
	m_pLeftOne->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
}

void CPledgeBlade::RightTwoInit(void)
{
	_vec4 Color = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pRightTwo->SetAni(true);
	m_pRightTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	_vec3 vAngle = _vec3(0.f, -160.f, 180.f);
	m_pRightTwo->GetTransCom()->Set_Angle(&vAngle);
	m_pRightTwo->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
}

void CPledgeBlade::LeftThreeInit(void)
{
	_vec4 Color = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pLeftThree->SetAni(true);
	m_pLeftThree->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	_vec3 vAngle = _vec3(0.f, 200.f, 40.f);
	m_pLeftThree->GetTransCom()->Set_Angle(&vAngle);
	m_pLeftThree->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);
}

void CPledgeBlade::RightFourInit(void)
{

	_vec4 Color = _vec4(0.2941f, 0.0f, 0.5098f, 2.f);
	m_pRightFour->SetAni(true);
	m_pRightFour->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	_vec3 vAngle = _vec3(180.f, 150.f, -50.f);
	m_pRightFour->GetTransCom()->Set_Angle(&vAngle);
	m_pRightFour->GetTransCom()->Set_Scale(1.2f, 1.2f, 1.2f);


}

void CPledgeBlade::LightUpdate(void)
{
	if (m_pLightSprite->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pLightSprite->GetParticleInfo()->m_dColor;

		Color.w = max(Color.w - 0.1f, 0.f);

		m_pLightSprite->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	}
}

void CPledgeBlade::WhiteUpdate(void)
{
	if (m_pWhiteWave->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pWhiteWave->GetParticleInfo()->m_dColor;

		if (Color.w >= 0.f)
		{
			Color.w = max(Color.w - 0.1f, 0.f);

			m_pWhiteWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

			m_pWhiteWave->GetTransCom()->Add_Scale(0.075f);
		}
		else
		{
			m_pWhiteWave->SetAni(false);
		}
	}


	if (m_pWhiteWaveTwo->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pWhiteWaveTwo->GetParticleInfo()->m_dColor;

		if (Color.w >= 0.f)
		{
			Color.w = max(Color.w - 0.1f, 0.f);

			m_pWhiteWaveTwo->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

			m_pWhiteWaveTwo->GetTransCom()->Add_Scale(0.1f);
		}
		else
		{
			m_pWhiteWaveTwo->SetAni(false);
		}
	}

	if (m_pWhiteWaveThree->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pWhiteWaveThree->GetParticleInfo()->m_dColor;

		if (Color.w >= 0.f)
		{
			Color.w = max(Color.w - 0.05f, 0.f);

			m_pWhiteWaveThree->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

			m_pWhiteWaveThree->GetTransCom()->Add_Scale(0.2f);
		}
		else
		{
			m_pWhiteWaveThree->SetAni(false);
		}
	}

	if (m_pWhiteWaveFour->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pWhiteWaveFour->GetParticleInfo()->m_dColor;

		if (Color.w >= 0.f)
		{
			Color.w = max(Color.w - 0.02f, 0.f);

			m_pWhiteWaveFour->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

			m_pWhiteWaveFour->GetTransCom()->Add_Scale(0.3f);
		}
		else
		{
			m_pWhiteWaveFour->SetAni(false);
		}
	}

	if (m_pHitBlade[0]->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pHitBlade[0]->GetParticleInfo()->m_dColor;

		if (Color.w >= 0.f)
		{
			Color.w = max(Color.w - 0.1f, 0.f);

			m_pHitBlade[0]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

			m_pHitBlade[0]->GetTransCom()->Add_Scale(0.075f);
		}
		else
		{
			m_pHitBlade[0]->SetAni(false);
		}
	}

	if (m_pHitBlade[1]->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pHitBlade[1]->GetParticleInfo()->m_dColor;

		if (Color.w >= 0.f)
		{
			Color.w = max(Color.w - 0.1f, 0.f);

			m_pHitBlade[1]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

			m_pHitBlade[1]->GetTransCom()->Add_Scale(0.075f);
		}
		else
		{
			m_pHitBlade[1]->SetAni(false);
		}
	}

	if (m_pHitBlade[2]->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pHitBlade[2]->GetParticleInfo()->m_dColor;

		if (Color.w >= 0.f)
		{
			Color.w = max(Color.w - 0.1f, 0.f);

			m_pHitBlade[2]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

			m_pHitBlade[2]->GetTransCom()->Add_Scale(0.075f);
		}
		else
		{
			m_pHitBlade[2]->SetAni(false);
		}
	}

	if (m_pHitBlade[3]->GetAni() == true)
	{
		_vec4 Color = (_vec4)m_pHitBlade[3]->GetParticleInfo()->m_dColor;

		if (Color.w >= 0.f)
		{
			Color.w = max(Color.w - 0.1f, 0.f);

			m_pHitBlade[3]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;

			m_pHitBlade[3]->GetTransCom()->Add_Scale(0.075f);
		}
		else
		{
			m_pHitBlade[3]->SetAni(false);
		}
	}
}
