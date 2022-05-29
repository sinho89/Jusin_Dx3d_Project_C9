#include "stdafx.h"
#include "WitchBlade.h"
#include "Witch_Town.h"
#include "Witch_Dungeon.h"
#include "Witch_Fight.h"
#include "PlayerCamera.h"
#include "Light.h"
#include "Map_Store.h"
#include "ScreenSpaceDecal.h"
#include "WitchBladeWeapon.h"
#include "ObjectName.h"
#include "UI_Skill.h"
#include "Sound.h"
#include "PlayerName.h"


CWitchBlade::CWitchBlade(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: CPlayer(pGraphicDev, wstrName)
//, m_bDecal(FALSE)
{
	
}

CWitchBlade::~CWitchBlade(void)
{

}


const _bool* CWitchBlade::Get_IsSkill(const _uint& iState) const
{
	const CWitch_State* pState = NULL;
	pState = (const CWitch_State*)m_pAnimatorCom->Get_State(iState);
	if (pState == NULL)
		return NULL;

	return pState->Get_CurAction();
}

_bool CWitchBlade::Get_CameraMove(void)
{
	return m_pPlayerCamera->Get_Move();
}

void CWitchBlade::Set_ActionEnd(void)
{
	CWitch_Fight* pState = (CWitch_Fight*)m_pAnimatorCom->Get_State(PLAYER_FIGHT);
	pState->Set_ActionEnd();
}

void CWitchBlade::Set_Skill(const CUI_Skill* pSkill)
{
	if (const_cast<CUI_Skill*>(pSkill)->Get_Skill_Info()->m_eSkillId > SKILL_END)
		return;

	if (m_pAnimatorCom->Get_State() != PLAYER_FIGHT)
		return;

	CWitch_Fight* pState = (CWitch_Fight*)m_pAnimatorCom->Get_CurState();
	pState->Set_Skill(pSkill);
}

void CWitchBlade::Set_Move(void)
{
	CWitch_Fight* pState = (CWitch_Fight*)m_pAnimatorCom->Get_CurState();
	pState->Get_Move(&m_bPlayerMoveState);
}

void CWitchBlade::Compute_Camera(void)
{
	const _vec3* pCamPos = m_pPlayerCamera->Get_Pos();
	Compute_CameraLook(pCamPos);
	m_pPlayerCamera->Set_Move();
}

HRESULT CWitchBlade::Start_ForScene(void)
{
	CPlayer::Start_ForScene();

	if (m_bOnceStart)
	{
		return S_OK;
	}

	m_bOnceStart = true;
	// NameUI	


	CWitchBladeWeapon* pWeaponR = CWitchBladeWeapon::Create(m_pGraphicDev, L"Sword1", m_wstrName.c_str(), L"Sword1R");
	if(pWeaponR == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pWeaponR);

	CWitchBladeWeapon* pWeaponL = CWitchBladeWeapon::Create(m_pGraphicDev, L"SwordL1", m_wstrName.c_str(), L"Sword1L");
	if(pWeaponL == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pWeaponL);

	CWitchBladeWeapon* pWeaponR_In = CWitchBladeWeapon::Create(m_pGraphicDev, L"Sword1", m_wstrName.c_str(), L"Sword1R_In");
	if (pWeaponR_In == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pWeaponR_In);

	CWitchBladeWeapon* pWeaponL_In = CWitchBladeWeapon::Create(m_pGraphicDev, L"SwordL1", m_wstrName.c_str(), L"Sword1L_In");
	if (pWeaponL_In == NULL)
		return E_FAIL;
	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Weapon", pWeaponL_In);
	

	//애니메이션 상태 준비
	Engine::CAnimationState* pState = NULL;

	pState = CWitch_Town::Create(m_pMeshCom, &m_pmapNextMotion[PLAYER_TOWN], this, WITCH_IDLE, pWeaponL_In, pWeaponR_In);
	if (pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(PLAYER_TOWN, pState);

	pState = CWitch_Dungeon::Create(m_pMeshCom, &m_pmapNextMotion[PLAYER_DUNGEON], this, WITCH_IDLE, pWeaponL_In, pWeaponR_In);
	if (pState == NULL)
		return E_FAIL;

	m_pAnimatorCom->Set_State(PLAYER_DUNGEON, pState);

	pState = CWitch_Fight::Create(m_pMeshCom, &m_pmapNextMotion[PLAYER_FIGHT], this, WITCH_ATTACKIDLE, pWeaponL, pWeaponR);
	if (pState == NULL)
		return E_FAIL;

	m_pAnimatorCom->Set_State(PLAYER_FIGHT, pState);

	//애니메이터 세팅
	m_pAnimatorCom->Set_Animator(&m_vecAniInfo);
	m_pAnimatorCom->Set_State(m_iPlayerAniState);
	if (m_iPlayerAniState == PLAYER_FIGHT)
		m_pAnimatorCom->Set_AnimationSet(WITCH_ATTACKIDLE);
	else
		m_pAnimatorCom->Set_AnimationSet(WITCH_IDLE);
	
	//Light Cam
	m_pLightCam = Engine::GetCamera(Engine::CAMERA_LIGHT);
	m_pPlayerCamera = (CPlayerCamera*)Engine::GetCamera(Engine::CAMERA_PLAYER);
	

	//m_bOutlineCheck = TRUE;

	//=============================데칼 테스트
	/*m_pMapStore = static_cast<CMap_Store*>(Engine::Find(L"Map_Store"));
	int iTemp = 0;
	Engine::CStaticMesh** pMesh = m_pMapStore->GetMapMesh(&iTemp);

	for (int i = 0; i < 9; ++i)
		m_pMapMesh[i] = pMesh[i]->Get_Mesh();*/

	//m_pLogicLayer = Engine::FindLayer(Engine::LAYER_GAMELOGIC);

	return S_OK;
}

void CWitchBlade::Update_Object(const _float& fTimeDelta)
{
	CPlayer::Update_Object(fTimeDelta);
	m_pMeshCom->Play_AnimationSet(fTimeDelta);
	// UI_Skill
	Set_Move();

	if(m_pCalculatorCom->Get_IsCol()) //지형충돌
		m_pRigidbodyCom->Set_Gravity(false); //중력을 false
	else
		m_pRigidbodyCom->Set_Gravity(true); //중력을 true

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DYNAMIC, this);

	//Motion Blur============================================================
	if (m_pRendererCom->GetMotionBlurBuffer())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_MOTION_BLUR, this);
		m_pMeshCom->UpdateTimeDelta(fTimeDelta);
	}
	//=======================================================================
	
	//Shadow=================================================================
	if (m_pRendererCom->GetShadowBuffer()	|| 
		m_pRendererCom->GetSoftShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_DYANAMIC_SHADOW, this);
		
		_vec3 EyePos = m_pTransCom->Get_Now_Position() + _vec3(-2.5f, 50.5f, -2.5f);
		//_vec3 EyePos = _vec3(1.0f, 85.0f, 1.0f);
		//EyePos.y = 50.5f;

		m_pLightCam->SetEyePosition(EyePos);
		m_pLightCam->SetAtPosition(m_pPlayerCamera->GetAtPosition());

		m_pLightCam->MakeViewMatrix();
		m_pLightCam->MakeProjOrthoLHMatrix(_float(WINSIZEX / 10.0f), _float(WINSIZEY / 10.0f));
	}
	else if (m_pRendererCom->GetCACDShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_DYANAMIC_SHADOW, this);

		_vec3 EyePos = m_pTransCom->Get_Now_Position() + _vec3(-2.5f, 50.5f, -2.5f);

		m_pLightCam->SetEyePosition(EyePos);
		m_pLightCam->SetAtPosition(m_pPlayerCamera->GetAtPosition());
		m_pLightCam->MakeViewMatrix();

		_float fNear	=	m_pLightCam->GetNear();
		_float fFar		=	m_pLightCam->GetCamFar();
		_matrix matProj;

		D3DXMatrixOrthoLH(&matProj, _float(WINSIZEX / 80.0f), _float(WINSIZEY / 80.0f), fNear, fFar);
		m_pLightCam->Set_CACD_Proj(matProj, 0);
		D3DXMatrixOrthoLH(&matProj, _float(WINSIZEX / 5.0f), _float(WINSIZEY / 5.0f), fNear, fFar);
		m_pLightCam->Set_CACD_Proj(matProj, 1);
		D3DXMatrixOrthoLH(&matProj, _float(WINSIZEX * 2.0f), _float(WINSIZEY * 2.0f), fNear, fFar);
		m_pLightCam->Set_CACD_Proj(matProj, 2);
	}
	//=======================================================================

	//데칼 테스트========
	/*if (Engine::Get_DIKeyState(DIK_F8) & 0x80)
	{
		if (m_bDecal)
			return;

		m_bDecal = TRUE;

		POINT	ptMouse = { WINSIZEX >> 1, WINSIZEY >> 1 };

		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);

		for (int i = 0; i < 9; ++i)
		{
			if (m_pCalculatorCom->Picking_Mesh(ptMouse, WINSIZEX, WINSIZEY, m_pMapMesh[i], m_pMapStore->GetTrans()->Get_WorldMatrixPointer(), &m_fPickPos, &m_vPickDir))
			{
				_vec3 vAngle = static_cast<CPlayerCamera*>(Engine::GetCamera(Engine::CAMERA_PLAYER))->GetCamAngle();

				Engine::CGameObject*	pGameObject = CScreenSpaceDecal::Create(m_pGraphicDev, L"Decal", m_fPickPos, vAngle);
				if (NULL == pGameObject) return;
				m_pLogicLayer->AddObject(L"Decal", pGameObject);

				break;
			}
		}
	}
	else
		m_bDecal = FALSE;*/
	//==================	

	//플레이어 위치이동..
	if (GetAsyncKeyState(VK_NUMPAD5))
	{
		m_pTransCom->Set_Position(&_vec3(0.0f, 5.0f, 0.f));
	}
	if (GetAsyncKeyState(VK_NUMPAD6))
	{
		m_pTransCom->Set_Position(&_vec3(479.f, 124.f, -252.f));
		//m_pTransCom->Set_Position(&_vec3(201.f, -5.f, -425.f));
	}

	m_tPlayerInfo.m_vLook = m_pTransCom->Get_Now_Look();
	m_tPlayerInfo.m_vRight = m_pTransCom->Get_Now_Right();
}

void CWitchBlade::Render_Object(void)
{
	//LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	//if(NULL == pEffect)
	//	return;

	//pEffect->AddRef();

	//_uint iPass = 0;
	//
	//switch (Engine::GetMRTID())
	//{
	//case Engine::MRT_DEFERRED_0:
	//	iPass = 3;
	//	if (FAILED(Set_Shadow_ConstanTable(pEffect)))return;
	//	break;

	//case Engine::MRT_DEFERRED_1:
	//	iPass = 0;
	//	if (FAILED(SetUp_ConstantTable(pEffect)))return;
	//	break;

	//case Engine::MRT_DEFERRED_2:
	//	iPass = 1;
	//	if (FAILED(SetUp_ConstantTable(pEffect)))return;
	//	break;

	//case Engine::MRT_MOTION_BLUR:
	//	iPass = 2;
	//	if (FAILED(SetUp_ConstantTable(pEffect)))return;
	//	break;
	//}
	//
	//pEffect->Begin(NULL, 0);
	//pEffect->BeginPass(iPass);

	//if (2 == iPass) //모션블러 일 때.
	//	m_pMeshCom->Render_BlurMesh(pEffect);
	//else
	//	m_pMeshCom->Render_Mesh(pEffect);

	//pEffect->EndPass();
	//pEffect->End();
	//
	//Engine::Safe_Release(pEffect);
	
	if(m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
	if (m_pSphereColCom)
		m_pSphereColCom->Render_Collider(g_bDebugRender);
}

void CWitchBlade::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	switch (iPassIdx)
	{
	case 0:
		if (FAILED(SetUp_ConstantTable(pEffect)))return;
		break;

	case 1:
		if (FAILED(SetUp_ConstantTable(pEffect)))return;
		break;

	case 2:
		if (FAILED(SetUp_ConstantTable(pEffect)))return;
		break;

	case 3:
		if (FAILED(Set_Shadow_ConstanTable(pEffect)))return;
		break;

	case 4:
		if (FAILED(Set_CACD_Shadow_ConstanTable(pEffect)))return;
		break;

	/*case 5:
		if (m_bOutlineDraw)
		{
			if (FAILED(SetUp_ConstantTable(pEffect)))return;
		}
		else
		{
			return;
		}
		break;*/
	}

	if (2 == iPassIdx) //모션블러 일 때.
		m_pMeshCom->Render_BlurMesh(pEffect);
	else
		m_pMeshCom->Render_Mesh(pEffect);	

	/*if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
	if (m_pSphereColCom)
		m_pSphereColCom->Render_Collider(g_bDebugRender);*/
}

HRESULT CWitchBlade::Ready_Object(SCENE_TYPE eSceneType)
{
	if(FAILED(Add_Component()))
		return E_FAIL;
	if(FAILED(LoadAniInfo(L"WitchBladeAni")))
		return E_FAIL;
	if(FAILED(LoadCollider(L"WitchBladeCollider")))
		return E_FAIL;

	m_eSceneType = eSceneType;
	if(m_eSceneType == SCENE_TOWN)
		m_iPlayerAniState = PLAYER_TOWN;
	if(m_eSceneType == SCENE_DUNGEON)
		m_iPlayerAniState = PLAYER_FIGHT;

	m_pTransCom->Set_OutlineScale(1.0f, 1.0f, 1.0f);

	return S_OK;
}

HRESULT CWitchBlade::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"0", pComponent));

	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_WitchBlade");
	if(pComponent == NULL)
		return E_FAIL;
	m_pMeshCom = (Engine::CDynamicMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	//Renderer
	pComponent = Engine::Get_Renderer();
	if(pComponent == NULL)
		return E_FAIL;
	m_pRendererCom = (Engine::CRenderer*)pComponent;
	m_pRendererCom->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));

	//Shader
	pComponent = Engine::Clone_Prototype(L"Shader_DynamicMesh");
	if(pComponent == NULL)
		return E_FAIL;
	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	//Animator
	pComponent = Engine::CAnimator::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pAnimatorCom = (Engine::CAnimator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"3", pComponent));

	//Calculator
	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	if(pComponent == NULL)
		return E_FAIL;
	m_pCalculatorCom = (Engine::CCalculator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"2", pComponent));
	m_pCalculatorCom->Set_Transform(m_pTransCom);

	//Rigidbody
	pComponent = Engine::CRigidbody::Create(m_pTransCom, m_pCalculatorCom);
	if(pComponent == NULL)
		return E_FAIL;
	m_pRigidbodyCom = (Engine::CRigidbody*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"1", pComponent));
	m_pRigidbodyCom->Set_Gravity(false);

	//SphereCollider
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Player", _vec3(0.f, 0.f, 0.f), 10.f);
	if (pComponent == NULL)
		return E_FAIL;
	m_pSphereColCom = (Engine::CSphereCollider*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"SphereCol", pComponent));

	//Sound
	pComponent = CSound::Create(m_pTransCom);
	if (pComponent == NULL)
		return E_FAIL;
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Sound", pComponent));

	m_pSoundCom->Set_Sound(L"Attack1", L"WitchAttack1.wav");
	m_pSoundCom->Set_Sound(L"Attack2", L"WitchAttack2.wav");
	m_pSoundCom->Set_Sound(L"Attack3", L"WitchAttack3.wav");
	m_pSoundCom->Set_Sound(L"Attack4", L"WitchAttack4.wav");
	m_pSoundCom->Set_Sound(L"Attack5", L"WitchAttack5.wav");
	m_pSoundCom->Set_Sound(L"DarkVoice1", L"DarkPollingA.wav");
	m_pSoundCom->Set_Sound(L"DarkVoice2", L"DarkPollingB.wav");
	m_pSoundCom->Set_Sound(L"DarkWind1", L"WitchDarkPollingWind1.wav");
	m_pSoundCom->Set_Sound(L"DarkWind2", L"WitchDarkPollingWind2.wav");
	m_pSoundCom->Set_Sound(L"Fledge", L"WitchFledge.wav");
	m_pSoundCom->Set_Sound(L"FledgeVoice1", L"WitchFledgeVoice1.wav");
	m_pSoundCom->Set_Sound(L"FledgeVoice2", L"WitchFledgeVoice2.wav");
	m_pSoundCom->Set_Sound(L"FledgeVoice3", L"WitchFledgeVoice3.wav");
	m_pSoundCom->Set_Sound(L"FledgeVoice4", L"WitchFledgeVoice4.wav");
	m_pSoundCom->Set_Sound(L"Hillturn", L"WitchHillturn.wav");
	m_pSoundCom->Set_Sound(L"HillturnSwing", L"WitchHillturnSwing.wav");
	m_pSoundCom->Set_Sound(L"Spiral", L"WitchSpiral.wav");
	m_pSoundCom->Set_Sound(L"SpiralB", L"WitchSpiralB.wav");
	m_pSoundCom->Set_Sound(L"SpiralSwing", L"WitchSpiralSwing.wav");
	m_pSoundCom->Set_Sound(L"Tricky", L"WitchTricky.wav");
	m_pSoundCom->Set_Sound(L"TrickyB", L"WitchTrickyB.wav");
	m_pSoundCom->Set_Sound(L"Violet", L"WitchViolet.wav");
	m_pSoundCom->Set_Sound(L"VioletWind1", L"WitchVioletWind1.wav");
	m_pSoundCom->Set_Sound(L"VioletWind2", L"WitchVioletWind2.wav");
	m_pSoundCom->Set_Sound(L"VioletVoice", L"WitchVioletVoice.wav");
	m_pSoundCom->Set_Sound(L"Sword1", L"WitchSword1.wav");
	m_pSoundCom->Set_Sound(L"Sword2", L"WitchSword2.wav");
	m_pSoundCom->Set_Sound(L"Sword3", L"WitchSword3.wav");
	m_pSoundCom->Set_Sound(L"Sword4", L"WitchSword4.wav");
	m_pSoundCom->Set_Sound(L"Sword5", L"WitchSword5.wav");
	m_pSoundCom->Set_Sound(L"SwordSkill1", L"WitchSwordSkill1.wav");
	m_pSoundCom->Set_Sound(L"SwordSkill2", L"WitchSwordSkill2.wav");
	m_pSoundCom->Set_Sound(L"SwordSkill3", L"WitchSwordSkill3.wav");
	m_pSoundCom->Set_Sound(L"SwordSkill4", L"WitchSwordSkill4.wav");
	m_pSoundCom->Set_Sound(L"Swing1", L"WitchSwing1.wav");
	m_pSoundCom->Set_Sound(L"Swing2", L"WitchSwing2.wav");
	m_pSoundCom->Set_Sound(L"Swing3", L"WitchSwing3.wav");
	m_pSoundCom->Set_Sound(L"Swing4", L"WitchSwing4.wav");
	m_pSoundCom->Set_Sound(L"Swing5", L"WitchSwing5.wav");
	m_pSoundCom->Set_Sound(L"SpinTurnA", L"WitchSpinTurnA.wav");
	m_pSoundCom->Set_Sound(L"SpinTurnB", L"WitchSpinTurnB.wav");
	m_pSoundCom->Set_Sound(L"Blair", L"WitchBlare.wav");
	m_pSoundCom->Set_Sound(L"BlairVoice", L"WitchBlairVoice.wav");
	m_pSoundCom->Set_Sound(L"WalkL", L"WitchWalk2.wav");
	m_pSoundCom->Set_Sound(L"WalkR", L"WitchWalk2.wav");
	m_pSoundCom->Set_Sound(L"SoftRanding", L"WitchSoftRanding.wav");
	m_pSoundCom->Set_Sound(L"Jump", L"WitchJump.wav");
	m_pSoundCom->Set_Sound(L"BlackRose1", L"WitchBlackRoseSwing1.wav");
	m_pSoundCom->Set_Sound(L"BlackRose2", L"WitchBlackRoseSwing2.wav");
	m_pSoundCom->Set_Sound(L"BlackRoseVoice1", L"WitchBlackRoseVoice1.wav");
	m_pSoundCom->Set_Sound(L"BlackRoseVoice2", L"WitchBlackRoseVoice2.wav");
	m_pSoundCom->Set_Sound(L"Viper1", L"WitchViper1.wav");
	m_pSoundCom->Set_Sound(L"Viper2", L"WitchViper2.wav");
	m_pSoundCom->Set_Sound(L"Hit", L"WitchHit.wav");
	m_pSoundCom->Set_Sound(L"ViperVoice1", L"WitchViperVoice1.wav");
	m_pSoundCom->Set_Sound(L"ViperVoice2", L"WitchViperVoice2.wav");
	m_pSoundCom->Set_Sound(L"RiddleKick", L"WitchRiddle.wav");
	m_pSoundCom->Set_Sound(L"RiddleKick2", L"WitchRiddle.wav");
	m_pSoundCom->Set_Sound(L"Damaged1", L"WitchDamaged.wav");
	m_pSoundCom->Set_Sound(L"Damaged2", L"WitchDamaged2.wav");
	m_pSoundCom->Set_Sound(L"SpinningLoop", L"WitchSpinningLoop.wav");
	m_pSoundCom->Set_Sound(L"SpinningMove", L"WitchSpinningMove.wav");
	m_pSoundCom->Set_Sound(L"Disiram", L"WitchDisiram.wav");
	m_pSoundCom->Set_Sound(L"Install", L"WitchInstall.wav");
	m_pSoundCom->Set_Sound(L"LevelUp", L"LevelUp.wav");

	//게이트 소리

	return S_OK;
}

HRESULT CWitchBlade::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	//========================
	//For.MotionBlur
	//이전에 저장했던 것을 쉐이더로 전달
	if (Engine::MRT_MOTION_BLUR == m_pRendererCom->GetMRTID())
	{
		//if(Engine::GetHDR())
		//	Engine::SetUp_OnShader(L"Target_Char_Sampler_Shade_HDR", pEffect, "g_BaseTexture");
		//else
			Engine::SetUp_OnShader(L"Target_Char_Sampler_Shade", pEffect, "g_BaseTexture");
		
		pEffect->SetMatrix("g_matOldWorld", &m_matOldWorld);
		pEffect->SetMatrix("g_matOldView", &m_matOldView);
		pEffect->SetMatrix("g_matOldProj", &m_matOldProj);

		float fTime = m_pMeshCom->GetTimeDelta();

		if (0.07f <= fTime)
		{
			//다음을 위해 지금의 것을 저장한다.
			m_matOldWorld = matWorld;
			m_matOldView = matView;
			m_matOldProj = matProj;	
		}		
	}	
	//========================

	//Engine::SetUp_OnShader(L"Target_Shade", pEffect, "g_Light_Texture");

	pEffect->SetVector("g_vLightDir", &_vec4(1.0f, -1.0f, 1.0f, 0.0f));

	//const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	////지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 70.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);
	
	pEffect->SetBool("g_bBlurCheck", Engine::GetMotionBlurBuffer());
	
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);
	pEffect->SetFloat("g_fColorCheck", 0.0f);
	pEffect->SetFloat("g_fOutlineThickness", 0.00045f);
	
	/*if (m_bOutlineCheck)
	{
		pEffect->SetVector("g_vColor", &_vec4(m_bOutlineColor, 0.0f));
		pEffect->SetFloat("g_bOutlinePower", 2.5f);
	}*/

	pEffect->SetBool("g_bLimLight", true);

	if (Engine::GetLimLight())
	{
		pEffect->SetFloat("g_bOutlinePower", 18.0f);

		if (Engine::SCENE_SELECT == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 1.0f, 1.0f, 0.0f));
		else if (Engine::SCENE_TOWN == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 0.8549f, 0.7254f, 0.0f));
		else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(0.5294f, 0.8078f, 0.9215f, 0.0f));
		else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 0.6274f, 0.4784f, 0.0f));
	}	

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CWitchBlade::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = m_pLightCam->GetMatView();
	matLightProj = m_pLightCam->GetMatProj();

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CWitchBlade::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	pEffect->SetMatrix("g_matLightView", &m_pLightCam->GetMatView());
	
	if(0 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &m_pLightCam->Get_CACD_Proj(0));
	else if(1 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &m_pLightCam->Get_CACD_Proj(1));
	else if(2 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &m_pLightCam->Get_CACD_Proj(2));	

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CWitchBlade* CWitchBlade::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, SCENE_TYPE eSceneType)
{
	CWitchBlade*		pInstance = new CWitchBlade(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object(eSceneType)))
	{
		MSG_BOX("CWitchBlade Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitchBlade::Free(void)
{
	//::Safe_Release(m_pLogicLayer);
	//::Safe_Release(m_pMapStore);

	CPlayer::Free();
}