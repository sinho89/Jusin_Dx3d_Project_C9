#include "stdafx.h"
#include "Scene_Town.h"
#include "Export_Engine.h"

#include "WorldCamera.h"
#include "Map_Store.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"
#include "Environment.h"
#include "NPC_Weapon.h"
#include "NPC_Armor.h"
#include "NPC_Potion.h"
#include "NPC_Book.h"
#include "Camera.h"
#include "MapCamera.h"
#include "MiniMap.h"
#include "RiddleKick.h"
#include "RiseAttack.h"
#include "SpinTurnThrust.h"
#include "BlaireSilhouette.h"
#include "SoftRanding.h"
#include "UICamera.h"
#include "LightCamera.h"
#include "Light.h"

//// UI ////
#include "Cursor.h"
#include "Main_Base.h"
#include "Main_Pvp_Button.h"
#include "Main_Inven_Button.h"
#include "Main_CashShop_Button.h"
#include "Slot_Up_Button.h"
#include "Slot_Down_Button.h"
#include "Tired_Gage.h"
#include "Option_Base.h"
#include "Option_P.h"
#include "Option_Skill.h"
#include "Option_Quest.h"
#include "Option_Guild.h"
#include "Option_Home.h"
#include "Option_System.h"
#include "Option_Help.h"
#include "Option_J.h"
#include "Option_O.h"
#include "Window_Info.h"
#include "Window_Cancle.h"
#include "State_Base.h"
#include "Class_Texture.h"
#include "Level_Base.h"
#include "State_Bar.h"
#include "State_Gage.h"
#include "Class_Mark_Base.h"
#include "Class_Mark.h"
#include "Minimap_Base.h"
#include "MiniMap_ZoomIn.h"
#include "MiniMap_ZoomOut.h"
#include "Inven_Base.h"
#include "Helmet_Space.h"
#include "Armor_Space.h"
#include "Shorder_Space.h"
#include "Leg_Space.h"
#include "Glove_Space.h"
#include "Boots_Space.h"
#include "SkillStone_Space.h"
#include "R_Weapon_Space.h"
#include "Earing_Space.h"
#include "Amulet_Space.h"
#include "Belt_Space.h"
#include "Skill_Base.h"
#include "Armlet_Space.h"
#include "Ring1_Space.h"
#include "Ring2_Space.h"
#include "Arrow_Space.h"
#include "L_Weapon_Space.h"
#include "Model_Turn_Right.h"
#include "Model_Turn_Left.h"
#include "Inven_Sheet.h"
#include "Inven_Block.h"
//==Item==//

#include "UI_Weapon.h"
#include "UI_Helmet.h"
#include "UI_Armor.h"
#include "UI_Leg.h"
#include "UI_Glove.h"
#include "UI_Boots.h"
#include "UI_Shorder.h"
#include "UI_Amulet.h"
#include "UI_Armlet.h"
#include "UI_Belt.h"
#include "UI_Earing.h"
#include "UI_Ring.h"

#include "ToolTip.h"
#include "Skill_Sheet.h"
#include "Page_Back_Button.h"
#include "Page_Next_Button.h"
#include "Skill_Block.h"
#include "UI_Rush.h"
#include "UI_FlyDance.h"
#include "UI_FladgeBlade.h"
#include "UI_SpinTurn.h"
#include "UI_Violet.h"
#include "UI_CrossBrits.h"
#include "UI_SpirerDance.h"
#include "UI_DarkPolling.h"
#include "Main_Block.h"
#include "Main_Font.h"
#include "UI_Potion.h"
#include "UI_BlackRose.h"
#include "UI_Dash.h"
#include "UI_LiddleKick.h"
#include "UI_TechDown.h"
#include "UI_EvadeAtt.h"
#include "UI_BlareSiluet.h"
#include "UI_UpAtt.h"
#include "UI_ViperKick.h"
#include "UI_SpiningHeart.h"
#include "UI_TrickyRabbit.h"
#include "UI_PlanitDance.h"
#include "UI_SoftRanding.h"
#include "UI_Plora.h"
#include "UI_SiluetDance.h"
#include "UI_UrbanBreak.h"
#include "UI_HillTurn.h"
#include "UI_MaxHpUp.h"
#include "UI_MaxMpUp.h"
#include "UI_CriticalUp.h"
#include "UI_DualBlade.h"
#include "UI_BloodAgil.h"
#include "Partition_Base.h"
#include "Partition_Up_Button.h"
#include "Partition_Down_Button.h"
#include "Partition_Max_Button.h"
#include "Partition_Button.h"
#include "Inven_Delete.h"
#include "Inven_Array.h"
#include "Skill_ToolTip.h"
#include "Skill_Avi.h"
#include "Wear_ToolTip.h"
#include "Inven_Space.h"
#include "ESC_Base.h"
#include "ESC_Button.h"
#include "System_Base.h"
#include "Button_Exit.h"
#include "Button_LevelUp.h"
#include "Button_Reset.h"
#include "Button_Gold.h"
#include "Button_Item.h"
#include "Button_HDR.h"
#include "Button_SSAO.h"
#include "Button_DOF.h"
#include "Button_MB.h"
#include "Button_LF.h"
#include "Button_Fog.h"
#include "Button_Shadow.h"
#include "Debug_Check.h"
#include "FPS_Check.h"
#include "Mouse_Check.h"
#include "LB_Check.h"

#include "Dungeon_Base.h"
#include "Dungeon_Shadow.h"
#include "Dungeon_Select.h"
#include "Dungeon_Texture.h"
#include "Dungeon_Start.h"
#include "Dungeon_Back.h"
#include "Shop_Cancle.h"
#include "Inven_Scroll_Button.h"
#include "Shop_Block.h"
#include "Shop_Sheet.h"
#include "Shop_ToolTip.h"
#include "UI_DragonDance.h"
#include "Buy_Base.h"
#include "Buy_Button.h"
#include "Sell_Base.h"
#include "Sell_Button.h"
#include "Compare_ToolTip.h"
#include "Combo.h"
#include "ComboEffect.h"
#include "ComboNum.h"
#include "NumberingMgr.h"
#include "Scene_Select.h"
#include "Scene_Login.h"
#include "Scene_Logo.h"
#include "..\Engine\Utility\Codes\CameraMgr.h"
#include "UI_SpinCrash.h"
#include "UI_LockOn.h"
#include "UI_ShotJump.h"
#include "UI_UpperKick.h"
#include "UI_FireM.h"
#include "UI_Gravity.h"
#include "UI_JumpDownKick.h"
#include "UI_HoldShot.h"
#include "UI_GeilShot.h"
#include "UI_DownKick.h"
#include "UI_ShellBuster.h"
#include "UI_Escape.h"
#include "UI_MovingCannon.h"
#include "Mistic.h"
#include "Inven_Char_Model.h"
#include "Model_WB_Weapon.h"
#include "UI_Fade_Line.h"

//==Effect==//
#include "ReflectMesh.h"
#include "FlyDance.h"
#include "Sprite.h"
#include "Portal.h"
#include "Fire.h"
#include "DarkPollingGrave.h"
#include "ViperKneeKick.h"
#include "SpinTurnThrust.h"
#include "Slide_Bright.h"
#include "Slide_Bloom.h"
#include "Slide_BackSound.h"
#include "Slide_EffectSound.h"
#include "Shop_Base.h"
#include "Scene_LoadingForIce.h"
#include "HillTurn.h"
#include "DragonDance.h"
#include "VioletCross.h"
#include "DarkSword.h"
#include "CherryBlossom.h"
#include "SpinningHeart.h"
#include "TrickyRabbit.h"
#include "SpiralDance.h"
#include "BlackRose.h"

//cullingManager
#include "Culling_Manager.h"
#include "UI_Fade_Title.h"
#include "Scene_LoadingForFire.h"
#include "PledgeBlade.h"
#include "UI_Fade_Sub.h"
#include "ResultBase.h"
#include "Fade_Manager.h"
#include "Fade.h"
#include "ResultItemLogo.h"
#include "ResultItemChar.h"
#include "ResultItemBox.h"
#include "Exp_Base.h"
#include "Exp_Gage.h"
#include "Upjuk_Ud.h"
#include "Boss_Base.h"
#include "PlayerHit.h"
#include "HitDir.h"
#include "CoolTimeMgr.h"

CScene_Town::CScene_Town(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_eCameraID(Engine::CAMERA_END)
	, m_iLightCount(0)
	, m_bTestKeyCheck(false)
	, m_bFadeUI(false)
	, m_fFadeTime(0.0f)
	, m_bFadeTest(false)
	, m_iSceneSkip(0)
	, m_bOneClick(FALSE)
{
	Engine::SetSceneID(Engine::SCENE_TOWN);
	g_iPointLightIndex = 0;
}

CScene_Town::~CScene_Town(void)
{

}

HRESULT CScene_Town::Ready_Scene(void)
{
	if (!g_bEnterToTown)
		m_fFadeNum = 6.f;
	else
		m_fFadeNum = 2.f;

	Engine::SetDebugBuffer(g_ShaderOption_Info.bDebugBuffer);
	Engine::SetShadowBuffer(g_ShaderOption_Info.bShadowBuffer);
	Engine::SetSoftShadow(g_ShaderOption_Info.bSoftShadow);
	Engine::SetCACDShadow(g_ShaderOption_Info.bCACDShadow);
	Engine::SetMotionBlurBuffer(g_ShaderOption_Info.bMotionBlurBuffer);
	Engine::SetGaussianBlur(g_ShaderOption_Info.bGaussianBlur);
	Engine::SetHDR(g_ShaderOption_Info.bHDR);
	Engine::SetDOF(g_ShaderOption_Info.bDOF);
	Engine::SetSSAO(g_ShaderOption_Info.bSSAO);
	Engine::SetExpFog(g_ShaderOption_Info.bExpFog);
	Engine::SetSquareFog(g_ShaderOption_Info.bSquareFog);
	Engine::SetHeightFog(g_ShaderOption_Info.bHeightFog);
	Engine::SetBlurValue(g_ShaderOption_Info.fBlurValue);
	Engine::SetBrightness(g_ShaderOption_Info.fBrightness);
	Engine::SetLimLight(g_ShaderOption_Info.bLimLight);

	if (FAILED(Engine::CScene::Ready_Scene()))	return E_FAIL;

	if (FAILED(SetUp_DefaultSetting()))			return E_FAIL;

	if (FAILED(Ready_Resource()))				return E_FAIL;

	if (FAILED(Ready_Prototype()))				return E_FAIL;

	if (FAILED(Ready_Layer_Environment()))		return E_FAIL;

	if (FAILED(Ready_Layer_GameLogic()))		return E_FAIL;

	if (FAILED(Ready_Layer_Weapon()))			return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))			return E_FAIL;

	if (FAILED(Ready_LightInfo()))				return E_FAIL;

	if (FAILED(Ready_Layer_UI()))				return E_FAIL;

	return S_OK;
}

HRESULT CScene_Town::Start_ForScene(void)
{
	if (g_bEnterToTown == true)
	{
		/*CPlayer* pPlayer = (CPlayer*)Engine::FindWithTag(L"Player");
		if (pPlayer == NULL)
		return E_FAIL;

		((Engine::CTransform*)pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Set_Position(&_vec3(0.f, 2.f, 0.f));
		pPlayer->Set_SceneType(SCENE_TOWN);*/

		Engine::CCamera* pCamera = NULL;

		pCamera = Engine::GetCamera(Engine::CAMERA_PLAYER);
		((CPlayerCamera*)pCamera)->Reset_CashingUI();
		((CPlayerCamera*)pCamera)->Reset_Scene_Change();

		//Engine::Safe_Release(pPlayer);
	}
	
	Ready_GameObject();
	
	Engine::CScene::Start_ForScene();	

	//마을 횃불 설치
	Ready_GameEffect();

	//마을 횃불에 점광원 설치
	Ready_EffectLight();

	// UI
	//SetWindowUI();

	//static_cast<CWorldCamera*>(Engine::GetCamera(Engine::CAMERA_WORLD))->Start_ForScene();
	//if(g_bEnterToTown == false)
	static_cast<CPlayerCamera*>(Engine::GetCamera(Engine::CAMERA_PLAYER))->Start_ForScene();
	//static_cast<CMapCamera*>(Engine::GetCamera(Engine::CAMERA_MINIMAP))->Start_ForScene();

	CPlayer* pPlayer = (CPlayer*)Engine::FindWithTag(L"Player");
	if (pPlayer == NULL)
		return E_FAIL;
	((Engine::CTransform*)pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Set_Position(&_vec3(-32.f, 0.f, 0.f));
	pPlayer->Set_SceneType(SCENE_TOWN);

	Engine::Safe_Release(pPlayer);

	if (!g_bEnterToTown)
		g_bEnterToTown = true;

	CCulling_Manager::GetInstance()->Ready_Culling(m_mapLayer[Engine::LAYER_GAMELOGIC]->Get_MapObject(), 30.f);

	CFade_Manager::GetInstance()->Ready_Fade();
	Engine::CGameObject* pGameObject = CFade::Create(m_pGraphicDev, L"Fade");
	Engine::AddObject(Engine::LAYER_UI, L"UI_Fade", pGameObject);
	CFade_Manager::GetInstance()->AddFade((CUI*)pGameObject, CFade_Manager::FADE_CLEAR_GETITEM);

	return S_OK;
}

void CScene_Town::Update_Scene(const _float& fTimeDelta)
{
	//잴위에 있어야됨
	CCulling_Manager::GetInstance()->Start_Culling();
	m_fFadeTime += fTimeDelta;

	if (m_fFadeTime >= m_fFadeNum)
	{
		m_fFadeTime = 0.f;
		if (!m_bFadeUI)
		{
			m_bFadeUI = true;

			Engine::CGameObject* pGameObject = CUI_Fade_Line::Create(m_pGraphicDev, L"Fade_Line");
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Bim", FADE_BIM);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Pell", FADE_PELL);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Lee", FADE_LEE);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Title::Create(m_pGraphicDev, L"Fade_Sung", FADE_SUNG);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Sang", FADE_SANG);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Jum", FADE_JUM);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Ji", FADE_JI);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
			pGameObject = CUI_Fade_Sub::Create(m_pGraphicDev, L"Fade_Goo", FADE_GOO);
			Engine::AddObject(Engine::LAYER_UI, L"UI", pGameObject);
		}
	}
	KeyCheck(fTimeDelta);
	//OverlapProtectWindowUI();

	static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);

	Engine::CScene::Update_Scene(fTimeDelta);

	/*switch (m_eCameraID)
	{
	case Engine::CAMERA_WORLD:
		static_cast<CWorldCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
		break;

	case Engine::CAMERA_PLAYER:
		static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
		break;

		case Engine::CAMERA_MINIMAP:
		static_cast<CMapCamera*>(Engine::GetCurrentCamera())->Update_Object(fTimeDelta);
		break;
	}*/

	if (Engine::Get_DIKeyState(DIK_F4) & 0x80)
	{
		if (m_bFadeTest)
			return;
		m_bFadeTest = true;
	}
	else
		m_bFadeTest = false;

	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		Engine::CScene*	pScene = NULL;

		pScene = CScene_LoadingForIce::Create(m_pGraphicDev);

		if (NULL == pScene)
			return;
		if (FAILED(Engine::SetUp_CurrentScene(pScene)))
			return;
		return;
	}
	else if (m_iSceneSkip == 1)
	{
		Engine::CScene*	pScene = NULL;

		pScene = CScene_LoadingForIce::Create(m_pGraphicDev);

		if (NULL == pScene)
			return;
		if (FAILED(Engine::SetUp_CurrentScene(pScene)))
			return;
		return;
	}
	else if (m_iSceneSkip == 2)
	{
		Engine::CScene*	pScene = NULL;

		pScene = CScene_LoadingForFire::Create(m_pGraphicDev);

		if (NULL == pScene)
			return;
		if (FAILED(Engine::SetUp_CurrentScene(pScene)))
			return;
		return;
	}

	else if (GetKeyState(VK_NUMPAD5) & 0x8000)
	{
		Engine::CScene*	pScene = NULL;

		pScene = CScene_LoadingForFire::Create(m_pGraphicDev);

		if (NULL == pScene)
			return;
		if (FAILED(Engine::SetUp_CurrentScene(pScene)))
			return;
		return;
	}
	//Deferred DebugRender
	/*else if (Engine::Get_DIKeyState(DIK_NUMPADSLASH) & 0x80)
	{
		if (TRUE == m_bOneClick)
			return;
		m_bOneClick = TRUE;
		
		_bool bBuf = Engine::GetDebugBuffer();
		Engine::SetDebugBuffer(!bBuf);
	}
	else
		m_bOneClick = FALSE;*/
}

void CScene_Town::Render_Scene(void)
{

}

HRESULT CScene_Town::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CScene_Town::Ready_LightInfo(void)
{
	Engine::LightMap_Clear();

	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	if (NULL == Engine::Find_LightInfo(m_iLightCount, D3DLIGHT_DIRECTIONAL))
	{
		LightInfo.Type = D3DLIGHT_DIRECTIONAL;
		LightInfo.Diffuse = D3DXCOLOR(1.0f * 0.06f, 1.0f * 0.06f, 1.0f * 0.06f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
		LightInfo.Direction = _vec3(1.0f, -1.0f, 1.0f);

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
		m_iLightCount++;

		return S_OK;
	}

	Engine::Find_LightInfo(m_iLightCount, D3DLIGHT_DIRECTIONAL)->Type = D3DLIGHT_DIRECTIONAL;
	Engine::Find_LightInfo(m_iLightCount, D3DLIGHT_DIRECTIONAL)->Diffuse = D3DXCOLOR(1.0f * 0.06f, 1.0f * 0.06f, 1.0f * 0.06f, 1.0f); //D3DXCOLOR(1.0 * 0.06f, 0.6274f * 0.06f, 0.4784f * 0.06f, 1.0f);
	Engine::Find_LightInfo(m_iLightCount, D3DLIGHT_DIRECTIONAL)->Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	Engine::Find_LightInfo(m_iLightCount, D3DLIGHT_DIRECTIONAL)->Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	Engine::Find_LightInfo(m_iLightCount, D3DLIGHT_DIRECTIONAL)->Direction = _vec3(1.0f, -1.0f, -1.0f);

	return S_OK;
}

HRESULT CScene_Town::Ready_Layer_Environment(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	Engine::CGameObject*	pGameObject = NULL;

	//Map 맵바꾸는곳..
	pGameObject = CMap_Store::Create(m_pGraphicDev, L"Map_Store");
	if (NULL == pGameObject) return E_FAIL;
	pLayer->AddObject(L"Map", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_ENVIRONMENT, pLayer));

	return S_OK;
}

HRESULT CScene_Town::Ready_Layer_GameLogic(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	if (g_bEnterToTown == true)
	{
		Engine::CGameObject*	pGameObject = NULL;
		////Portal
		pGameObject = CPortal::Create(m_pGraphicDev, L"Portal");
		if (NULL == pGameObject) return E_FAIL;

		_vec3 vPos = _vec3(-32.f, 30.f, 120.f);
		((CPortal*)pGameObject)->SetPortalPosition(&vPos);
		((CPortal*)pGameObject)->SkillStart();
		pLayer->AddObject(L"SoftEffect", pGameObject);

		m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));

		return S_OK;
	}

	Engine::CGameObject*	pGameObject = NULL;
	//Player
	if (m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CWitchBlade::Create(m_pGraphicDev, L"Player", SCENE_TOWN);
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"Player", pGameObject);
	}
	else
	{
		pGameObject = CMistic::Create(m_pGraphicDev, L"Player", SCENE_TOWN);
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"Player", pGameObject);
	}

	if (g_bEnterToTown == false)
	{
		LoadPlayerInfo((CPlayer*)pGameObject);
	}

	CPlayer* pPlayer = (CPlayer*)pGameObject;
	//========================SkillEffect============================//

	if (pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		//RiddleKick
		pGameObject = CRiddleKick::Create(m_pGraphicDev, L"RiddleKick");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//FlyDance
		pGameObject = CFlyDance::Create(m_pGraphicDev, L"FlyDance");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//SpinTurnThrust
		pGameObject = CSpinTurnThrust::Create(m_pGraphicDev, L"SpinTurnThrust");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//BlaireSilhouette
		pGameObject = CBlaireSilhouette::Create(m_pGraphicDev, L"BlaireSilhouette");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//RiseAttack
		pGameObject = CRiseAttack::Create(m_pGraphicDev, L"RiseAttack");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//SoftRanding
		pGameObject = CSoftRanding::Create(m_pGraphicDev, L"SoftRanding");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//DardPollingGrave
		pGameObject = CDarkPollingGrave::Create(m_pGraphicDev, L"DarkPollingGrave");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//ViperKneeKick
		pGameObject = CViperKneekick::Create(m_pGraphicDev, L"ViperKneeKick");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);


		//HillTurn
		pGameObject = CHillTurn::Create(m_pGraphicDev, L"HillTurn");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//DragonDance
		pGameObject = CDragonDance::Create(m_pGraphicDev, L"DragonDance");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//VioletCross
		pGameObject = CVioletCross::Create(m_pGraphicDev, L"VioletCross");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);


		//PledgeBlade
		pGameObject = CPledgeBlade::Create(m_pGraphicDev, L"PledgeBlade");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);


		//SpinningHeart
		pGameObject = CSpinningHeart::Create(m_pGraphicDev, L"SpinningHeart");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//TrickyRabbit
		pGameObject = CTrickyRabbit::Create(m_pGraphicDev, L"TrickyRabbit");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		//SpiralDance
		pGameObject = CSprialDance::Create(m_pGraphicDev, L"SprialDance");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);

		pGameObject = CBlackRose::Create(m_pGraphicDev, L"BlackRose");
		if (NULL == pGameObject) return E_FAIL;
		pLayer->AddObject(L"SoftEffect", pGameObject);
	}


	//Portal
	pGameObject = CPortal::Create(m_pGraphicDev, L"Portal");
	if (NULL == pGameObject) return E_FAIL;
	_vec3 vPos = _vec3(-32.f, 30.f, 120.f);
	((CPortal*)pGameObject)->SetPortalPosition(&vPos);
	((CPortal*)pGameObject)->SkillStart();
	pLayer->AddObject(L"SoftEffect", pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CScene_Town::Ready_Layer_Weapon(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_WEAPON, pLayer));
	return S_OK;
}

HRESULT CScene_Town::Ready_Layer_Camera(void)
{

	Engine::DefineCurrentCamera(Engine::CAMERA_PLAYER);
	m_eCameraID = Engine::CAMERA_PLAYER;

	return S_OK;
}

HRESULT CScene_Town::Ready_Layer_UI(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	if (g_bEnterToTown == true)
	{

		m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));

		return S_OK;
	}

	Engine::CGameObject*	pGameObject = NULL;

	// UI

	//For. MiniMap===================================================
	pGameObject = CMiniMap::Create(m_pGraphicDev, L"MiniMap");
	if (NULL == pGameObject)
		goto except;

	if (FAILED(pLayer->AddObject(L"UI_MiniMap", pGameObject)))
		goto except;
	//================================================================



	pGameObject = CShop_Base::Create(m_pGraphicDev, L"Shop_Base_Weapon", SHOP_WEAPON);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pWeaponShop = ((CUI*)pGameObject);

	pGameObject = CShop_Base::Create(m_pGraphicDev, L"Shop_Base_Armor", SHOP_ARMOR);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pArmorShop = ((CUI*)pGameObject);

	pGameObject = CShop_Base::Create(m_pGraphicDev, L"Shop_Base_Acc", SHOP_ACC);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pAccShop = ((CUI*)pGameObject);

	pGameObject = CShop_Base::Create(m_pGraphicDev, L"Shop_Base_Potion", SHOP_POTION);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pPotionShop = ((CUI*)pGameObject);

	pGameObject = CShop_Sheet::Create(m_pGraphicDev, L"Shop_Sheet_Weapon_Buy", pWeaponShop, SHOP_WEAPON, 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pBuyWeapon = ((CUI*)pGameObject);

	pGameObject = CShop_Sheet::Create(m_pGraphicDev, L"Shop_Sheet_Weapon_Sell", pWeaponShop, SHOP_WEAPON, 1);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pSellWeapon = ((CUI*)pGameObject);

	for (float fY = -50.f; fY > -400; fY -= 56.f)
	{
		for (float fX = 14.f; fX > -250; fX -= 54.f)
		{
			pGameObject = CShop_Block::Create(m_pGraphicDev, L"Shop_Block_Buy_Weapon", pBuyWeapon, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CShop_Sheet*)pBuyWeapon)->AddVecBlock(0, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}
	for (float fY = -50.f; fY > -400; fY -= 56.f)
	{
		for (float fX = 214.f; fX > -50; fX -= 54.f)
		{
			pGameObject = CShop_Block::Create(m_pGraphicDev, L"Shop_Block_Sell_Weapon", pSellWeapon, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CShop_Sheet*)pSellWeapon)->AddVecBlock(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}

	pGameObject = CShop_Sheet::Create(m_pGraphicDev, L"Shop_Sheet_Armor_Buy", pArmorShop, SHOP_ARMOR, 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pBuyArmor = ((CUI*)pGameObject);

	pGameObject = CShop_Sheet::Create(m_pGraphicDev, L"Shop_Sheet_Armor_Sell", pArmorShop, SHOP_ARMOR, 1);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pSellArmor = ((CUI*)pGameObject);

	for (float fY = -50.f; fY > -400; fY -= 56.f)
	{
		for (float fX = 14.f; fX > -250; fX -= 54.f)
		{
			pGameObject = CShop_Block::Create(m_pGraphicDev, L"Shop_Block_Buy_Armor", pBuyArmor, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CShop_Sheet*)pBuyArmor)->AddVecBlock(0, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}
	for (float fY = -50.f; fY > -400; fY -= 56.f)
	{
		for (float fX = 214.f; fX > -50; fX -= 54.f)
		{
			pGameObject = CShop_Block::Create(m_pGraphicDev, L"Shop_Block_Sell_Armor", pSellArmor, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CShop_Sheet*)pSellArmor)->AddVecBlock(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}

	pGameObject = CShop_Sheet::Create(m_pGraphicDev, L"Shop_Sheet_Acc_Buy", pAccShop, SHOP_ACC, 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pBuyAcc = ((CUI*)pGameObject);

	pGameObject = CShop_Sheet::Create(m_pGraphicDev, L"Shop_Sheet_Acc_Sell", pAccShop, SHOP_ACC, 1);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pSellAcc = ((CUI*)pGameObject);

	for (float fY = -50.f; fY > -400; fY -= 56.f)
	{
		for (float fX = 14.f; fX > -250; fX -= 54.f)
		{
			pGameObject = CShop_Block::Create(m_pGraphicDev, L"Shop_Block_Buy_Acc", pBuyAcc, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CShop_Sheet*)pBuyAcc)->AddVecBlock(0, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}
	for (float fY = -50.f; fY > -400; fY -= 56.f)
	{
		for (float fX = 214.f; fX > -50; fX -= 54.f)
		{
			pGameObject = CShop_Block::Create(m_pGraphicDev, L"Shop_Block_Sell_Acc", pSellAcc, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CShop_Sheet*)pSellAcc)->AddVecBlock(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}

	pGameObject = CShop_Sheet::Create(m_pGraphicDev, L"Shop_Sheet_Potion_Buy", pPotionShop, SHOP_POTION, 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pBuyPotion = ((CUI*)pGameObject);

	pGameObject = CShop_Sheet::Create(m_pGraphicDev, L"Shop_Sheet_Potion_Sell", pPotionShop, SHOP_POTION, 1);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pSellPotion = ((CUI*)pGameObject);

	for (float fY = -50.f; fY > -400; fY -= 56.f)
	{
		for (float fX = 14.f; fX > -250; fX -= 54.f)
		{
			pGameObject = CShop_Block::Create(m_pGraphicDev, L"Shop_Block_Buy_Potion", pBuyPotion, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CShop_Sheet*)pBuyPotion)->AddVecBlock(0, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}
	for (float fY = -50.f; fY > -400; fY -= 56.f)
	{
		for (float fX = 214.f; fX > -50; fX -= 54.f)
		{
			pGameObject = CShop_Block::Create(m_pGraphicDev, L"Shop_Block_Sell_Potion", pSellPotion, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CShop_Sheet*)pSellPotion)->AddVecBlock(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}


	pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_Blue", pBuyWeapon, WEAPON_BLUE, false);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyWeapon)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_RED", pBuyWeapon, WEAPON_RED, false);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyWeapon)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_S_Blue", pBuyWeapon, WEAPON_S_BLUE, false);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyWeapon)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_S_RED", pBuyWeapon, WEAPON_S_RED, false);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyWeapon)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Helmet::Create(m_pGraphicDev, L"Helmet_Normal", pBuyArmor, HELMET_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_Normal", pBuyArmor, ARMOR_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_Normal", pBuyArmor, LEG_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Glove::Create(m_pGraphicDev, L"Glove_Normal", pBuyArmor, GLOVE_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Boots::Create(m_pGraphicDev, L"Boots_Normal", pBuyArmor, BOOTS_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Shorder::Create(m_pGraphicDev, L"Shorder_Normal", pBuyArmor, SHORDER_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);


	pGameObject = CUI_Helmet::Create(m_pGraphicDev, L"Helmet_S_Normal", pBuyArmor, S_HELMET);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_S_Normal", pBuyArmor, S_ARMOR);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_S_Normal", pBuyArmor, S_LEG);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Glove::Create(m_pGraphicDev, L"Glove_S_Normal", pBuyArmor, S_GLOVE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Boots::Create(m_pGraphicDev, L"Boots_S_Normal", pBuyArmor, S_BOOTS);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Shorder::Create(m_pGraphicDev, L"Shorder_S_Normal", pBuyArmor, S_SHORDER);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyArmor)->AddShop(((CUI_Item*)pGameObject), 0);


	pGameObject = CUI_Amulet::Create(m_pGraphicDev, L"Amulet_Normal", pBuyAcc, AMULET_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyAcc)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Armlet::Create(m_pGraphicDev, L"Armlet_Normal", pBuyAcc, ARMLET_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyAcc)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Belt::Create(m_pGraphicDev, L"Belt_Normal", pBuyAcc, BELT_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyAcc)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Earing::Create(m_pGraphicDev, L"Earing_Normal", pBuyAcc, EARING_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyAcc)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Ring::Create(m_pGraphicDev, L"Ring_Normal", pBuyAcc, RING_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyAcc)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Ring::Create(m_pGraphicDev, L"Ring2_Normal", pBuyAcc, RING_NORMAL2);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyAcc)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_HP", pBuyPotion, POTION_HP);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyPotion)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_HP", pBuyPotion, POTION_HP);
	if (NULL == pGameObject)
		goto except;
	((CUI_Potion*)pGameObject)->Get_Item_Info()->m_iCount = 10;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Block*)(*((CShop_Sheet*)pBuyPotion)->GetVecBlock())[5])->SetItem(((CUI_Potion*)pGameObject));

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_HP", pBuyPotion, POTION_HP);
	if (NULL == pGameObject)
		goto except;
	((CUI_Potion*)pGameObject)->Get_Item_Info()->m_iCount = 100;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Block*)(*((CShop_Sheet*)pBuyPotion)->GetVecBlock())[10])->SetItem(((CUI_Potion*)pGameObject));

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_MP", pBuyPotion, POTION_MP);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyPotion)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_MP", pBuyPotion, POTION_MP);
	if (NULL == pGameObject)
		goto except;
	((CUI_Potion*)pGameObject)->Get_Item_Info()->m_iCount = 10;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Block*)(*((CShop_Sheet*)pBuyPotion)->GetVecBlock())[6])->SetItem(((CUI_Potion*)pGameObject));

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_MP", pBuyPotion, POTION_MP);
	if (NULL == pGameObject)
		goto except;
	((CUI_Potion*)pGameObject)->Get_Item_Info()->m_iCount = 100;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Block*)(*((CShop_Sheet*)pBuyPotion)->GetVecBlock())[11])->SetItem(((CUI_Potion*)pGameObject));

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_Level", pBuyPotion, POTION_LEVEL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Sheet*)pBuyPotion)->AddShop(((CUI_Item*)pGameObject), 0);

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_Level", pBuyPotion, POTION_LEVEL);
	if (NULL == pGameObject)
		goto except;
	((CUI_Potion*)pGameObject)->Get_Item_Info()->m_iCount = 10;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Block*)(*((CShop_Sheet*)pBuyPotion)->GetVecBlock())[7])->SetItem(((CUI_Potion*)pGameObject));

	pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_Level", pBuyPotion, POTION_LEVEL);
	if (NULL == pGameObject)
		goto except;
	((CUI_Potion*)pGameObject)->Get_Item_Info()->m_iCount = 100;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CShop_Block*)(*((CShop_Sheet*)pBuyPotion)->GetVecBlock())[12])->SetItem(((CUI_Potion*)pGameObject));

	pGameObject = CShop_Cancle::Create(m_pGraphicDev, L"Shop_Cancle_Weapon", pWeaponShop);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	pGameObject = CShop_Cancle::Create(m_pGraphicDev, L"Shop_Cancle_Armor", pArmorShop);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	pGameObject = CShop_Cancle::Create(m_pGraphicDev, L"Shop_Cancle_Acc", pAccShop);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	pGameObject = CShop_Cancle::Create(m_pGraphicDev, L"Shop_Cancle_Potion", pPotionShop);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CBuy_Base::Create(m_pGraphicDev, L"Buy_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pBuy = ((CUI*)pGameObject);

	pGameObject = CBuy_Button::Create(m_pGraphicDev, L"Buy_Ok_Button", pBuy, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CBuy_Button::Create(m_pGraphicDev, L"Buy_Cancle_Button", pBuy, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CSell_Base::Create(m_pGraphicDev, L"Sell_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI* pSell = ((CUI*)pGameObject);

	pGameObject = CSell_Button::Create(m_pGraphicDev, L"Sell_Ok_Button", pSell, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CSell_Button::Create(m_pGraphicDev, L"Sell_Cancle_Button", pSell, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Base::Create(m_pGraphicDev, L"Dungeon_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	CUI* pDunParent = (CUI*)pGameObject;

	pGameObject = CDunGeon_Texture::Create(m_pGraphicDev, L"Dungeon_Texture_Ice", pDunParent, _vec2(400.f, 400.f), true, DUNGEON_ICE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Texture::Create(m_pGraphicDev, L"Dungeon_Texture_Fire", pDunParent, _vec2(1200.f, 1000.f), true, DUNGEON_FIRE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Shadow::Create(m_pGraphicDev, L"Dungeon_Shadow", pDunParent);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Select::Create(m_pGraphicDev, L"Dungeon_Select_Ice", pDunParent, _vec2(1450.f, 225.f), true, DUNGEON_ICE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Select::Create(m_pGraphicDev, L"Dungeon_Select_Fire", pDunParent, _vec2(1450.f, 300.f), true, DUNGEON_FIRE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Select::Create(m_pGraphicDev, L"Dungeon_Select4", pDunParent, _vec2(1450.f, 375.f), false, DUNGEON_NONE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Select::Create(m_pGraphicDev, L"Dungeon_Select5", pDunParent, _vec2(1450.f, 450.f), false, DUNGEON_NONE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Select::Create(m_pGraphicDev, L"Dungeon_Select6", pDunParent, _vec2(1450.f, 525.f), false, DUNGEON_NONE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Select::Create(m_pGraphicDev, L"Dungeon_Select7", pDunParent, _vec2(1450.f, 600.f), false, DUNGEON_NONE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Select::Create(m_pGraphicDev, L"Dungeon_Select3", pDunParent, _vec2(1450.f, 675.f), false, DUNGEON_NONE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Start::Create(m_pGraphicDev, L"Dungeon_Start", pDunParent, _vec2(1450.f, 775.f), this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CDunGeon_Back::Create(m_pGraphicDev, L"Dungeon_Back", pDunParent, _vec2(1450.f, 850.f), this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Enter", pGameObject)))
		goto except;

	pGameObject = CMain_Base::Create(m_pGraphicDev, L"Main_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	CUI* pMainBase = (CUI*)pGameObject;

	int iIndex = 1;

	for (_float fX = 231.f; fX > -250; fX -= 51.35f)
	{
		pGameObject = CMain_Block::Create(m_pGraphicDev, L"Main_Block_1", pMainBase, _vec2(fX, 10));
		if (NULL == pGameObject)
			goto except;
		((CMain_Base*)pMainBase)->AddVecBlock(iIndex, ((CUI*)pGameObject));
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CMain_Font::Create(m_pGraphicDev, L"Main_Font_1", (CUI*)pGameObject, _vec2(fX, 10));
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		++iIndex;
	}

	for (_float fX = 231.f; fX > -250; fX -= 51.35f)
	{
		pGameObject = CMain_Block::Create(m_pGraphicDev, L"Main_Block_1", pMainBase, _vec2(fX, 10));
		if (NULL == pGameObject)
			goto except;
		((CMain_Base*)pMainBase)->AddVecBlock(iIndex, ((CUI*)pGameObject));
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CMain_Font::Create(m_pGraphicDev, L"Main_Font_1", (CUI*)pGameObject, _vec2(fX, 10));
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		++iIndex;
	}

	for (_float fX = 231.f; fX > -250; fX -= 51.35f)
	{
		pGameObject = CMain_Block::Create(m_pGraphicDev, L"Main_Block_1", pMainBase, _vec2(fX, 10));
		if (NULL == pGameObject)
			goto except;
		((CMain_Base*)pMainBase)->AddVecBlock(iIndex, ((CUI*)pGameObject));
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CMain_Font::Create(m_pGraphicDev, L"Main_Font_1", (CUI*)pGameObject, _vec2(fX, 10));
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		++iIndex;
	}

	for (_float fX = 179.65f; fX > -200.f; fX -= 51.35f)
	{
		pGameObject = CMain_Block::Create(m_pGraphicDev, L"Main_Block_1", pMainBase, _vec2(fX, 61.35f));
		if (NULL == pGameObject)
			goto except;
		((CMain_Base*)pMainBase)->AddVecBlock(iIndex, ((CUI*)pGameObject));
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CMain_Font::Create(m_pGraphicDev, L"Main_Font_1", (CUI*)pGameObject, _vec2(fX, 10));
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		++iIndex;
	}


	pGameObject = CMain_Pvp_Button::Create(m_pGraphicDev, L"Main_PVP_Button");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CMain_Inven_Button::Create(m_pGraphicDev, L"Main_Inven_Button");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CMain_CashShop_Button::Create(m_pGraphicDev, L"Main_CashShop_Button");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CSlot_Up_Button::Create(m_pGraphicDev, L"Slot_Up_Button");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CSlot_Down_Button::Create(m_pGraphicDev, L"Slot_Down_Button");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CTired_Gage::Create(m_pGraphicDev, L"Tired_Gage", 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CTired_Gage::Create(m_pGraphicDev, L"Tired_Gage", 1);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;


	pGameObject = CExp_Gage::Create(m_pGraphicDev, L"Exp_Gage");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CExp_Base::Create(m_pGraphicDev, L"Exp_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;


	pGameObject = COption_Base::Create(m_pGraphicDev, L"Option_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_P::Create(m_pGraphicDev, L"Option_P");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_Skill::Create(m_pGraphicDev, L"Option_Skill");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_Quest::Create(m_pGraphicDev, L"Option_Quest");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_System::Create(m_pGraphicDev, L"Option_System");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_Help::Create(m_pGraphicDev, L"Option_Help");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_Home::Create(m_pGraphicDev, L"Option_Home");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_J::Create(m_pGraphicDev, L"Option_J");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_O::Create(m_pGraphicDev, L"Option_O");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = COption_Guild::Create(m_pGraphicDev, L"Option_Guild");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Option", pGameObject)))
		goto except;

	pGameObject = CWindow_Info::Create(m_pGraphicDev, L"Window_Info");		/////////////////////////////////////////////// Window 창 UI
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CWindow_Cancle::Create(m_pGraphicDev, L"Window_Info_Cancle", ((CUI*)pGameObject));
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CInven_Base::Create(m_pGraphicDev, L"Inven_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI*	pUI = ((CUI*)pGameObject);

	pGameObject = CInven_Scroll_Button::Create(m_pGraphicDev, L"Inven_Scroll_Up", pUI, true);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CInven_Scroll_Button::Create(m_pGraphicDev, L"Inven_Scroll_Down", pUI, false);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	pGameObject = CInven_Delete::Create(m_pGraphicDev, L"Inven_Delete", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CInven_Array::Create(m_pGraphicDev, L"Inven_Array", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	////////////////////////////////////////////////////////////////////////////////////////////////// 인벤 모델 직업별로 나누기 /////////////////////////////////////////////////////
	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CInven_Char_Model::Create(m_pGraphicDev, L"Inven_Model", pUI, false);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

	}
	else
	{
		pGameObject = CInven_Char_Model::Create(m_pGraphicDev, L"Inven_Model", pUI, true);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	pGameObject = CModel_Turn_Right::Create(m_pGraphicDev, L"Model_Turn_Right", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CModel_Turn_Left::Create(m_pGraphicDev, L"Model_Turn_Left", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CWindow_Cancle::Create(m_pGraphicDev, L"Window_Inven_Cancle", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CHelmet_Space::Create(m_pGraphicDev, L"Helmet_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CArmor_Space::Create(m_pGraphicDev, L"Armor_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CShorder_Space::Create(m_pGraphicDev, L"Shorder_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CLeg_Space::Create(m_pGraphicDev, L"Leg_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CGlove_Space::Create(m_pGraphicDev, L"Glove_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CBoots_Space::Create(m_pGraphicDev, L"Boots_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CSkillStone_Space::Create(m_pGraphicDev, L"SkillStone_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CR_Weapon_Space::Create(m_pGraphicDev, L"R_Weapon_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CEaring_Space::Create(m_pGraphicDev, L"Earing_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CAmulet_Space::Create(m_pGraphicDev, L"Amulet_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CBelt_Space::Create(m_pGraphicDev, L"Belt_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CArmlet_Space::Create(m_pGraphicDev, L"Armlet_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CRing1_Space::Create(m_pGraphicDev, L"Ring1_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CRing2_Space::Create(m_pGraphicDev, L"Ring2_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CArrow_Space::Create(m_pGraphicDev, L"Arrow_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CL_Weapon_Space::Create(m_pGraphicDev, L"L_Weapon_Space", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	((CInven_Base*)pUI)->AddVecSpace(((CInven_Space*)((CUI*)pGameObject)));

	pGameObject = CInven_Sheet::Create(m_pGraphicDev, L"Inven_Sheet_1", pUI, 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI*	pSheet1 = ((CUI*)pGameObject);	////////////////////// 인벤 블럭////////////////////////////////////

	for (float fY = -44.f; fY > -300; fY -= 50.f)
	{
		for (float fX = 23.5; fX > -250; fX -= 49.5f)
		{
			pGameObject = CInven_Block::Create(m_pGraphicDev, L"Inven_Block_1", pSheet1, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CInven_Sheet*)pSheet1)->AddVecBlock(0, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}

	pGameObject = CInven_Sheet::Create(m_pGraphicDev, L"Inven_Sheet_2", pUI, 1);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI*	pSheet2 = ((CUI*)pGameObject);	////////////////////// 인벤 블럭////////////////////////////////////

	for (float fY = -44.f; fY > -200; fY -= 50.f)
	{
		for (float fX = 123.5f; fX > -150; fX -= 49.5f)
		{
			pGameObject = CInven_Block::Create(m_pGraphicDev, L"Inven_Block_2", pSheet2, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CInven_Sheet*)pSheet2)->AddVecBlock(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}

	pGameObject = CInven_Sheet::Create(m_pGraphicDev, L"Inven_Sheet_3", pUI, 2);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	CUI*	pSheet3 = ((CUI*)pGameObject);	////////////////////// 인벤 블럭////////////////////////////////////

	for (float fY = -44.f; fY > -200; fY -= 50.f)
	{
		for (float fX = 223.5; fX > -50; fX -= 49.5f)
		{
			pGameObject = CInven_Block::Create(m_pGraphicDev, L"Inven_Block_3", pSheet3, _vec2(fX, fY));
			if (NULL == pGameObject)
				goto except;
			((CInven_Sheet*)pSheet3)->AddVecBlock(2, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////// 직업별 아이템 생성 ///////////////////////////
	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_Blue", pSheet1, WEAPON_BLUE, false);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_RED", pSheet1, WEAPON_RED, false);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Helmet::Create(m_pGraphicDev, L"Helmet_Normal", pSheet1, HELMET_NORMAL);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_Normal", pSheet1, ARMOR_NORMAL);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_Normal", pSheet1, LEG_NORMAL);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Glove::Create(m_pGraphicDev, L"Glove_Normal", pSheet1, GLOVE_NORMAL);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Boots::Create(m_pGraphicDev, L"Boots_Normal", pSheet1, BOOTS_NORMAL);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Shorder::Create(m_pGraphicDev, L"Shorder_Normal", pSheet1, SHORDER_NORMAL);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
	}
	else
	{
		pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_S_Blue", pSheet1, WEAPON_S_BLUE, false);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_S_RED", pSheet1, WEAPON_S_RED, false);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Helmet::Create(m_pGraphicDev, L"Helmet_S_Normal", pSheet1, S_HELMET);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_S_Normal", pSheet1, S_ARMOR);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_S_Normal", pSheet1, S_LEG);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Glove::Create(m_pGraphicDev, L"Glove_S_Normal", pSheet1, S_GLOVE);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Boots::Create(m_pGraphicDev, L"Boots_SNormal", pSheet1, S_BOOTS);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

		pGameObject = CUI_Shorder::Create(m_pGraphicDev, L"Shorder_Normal", pSheet1, S_SHORDER);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
	}
	pGameObject = CUI_Amulet::Create(m_pGraphicDev, L"Amulet_Normal", pSheet1, AMULET_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

	pGameObject = CUI_Armlet::Create(m_pGraphicDev, L"Armlet_Normal", pSheet1, ARMLET_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

	pGameObject = CUI_Belt::Create(m_pGraphicDev, L"Belt_Normal", pSheet1, BELT_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

	pGameObject = CUI_Earing::Create(m_pGraphicDev, L"Earing_Normal", pSheet1, EARING_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

	pGameObject = CUI_Ring::Create(m_pGraphicDev, L"Ring_Normal", pSheet1, RING_NORMAL);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));

	pGameObject = CUI_Ring::Create(m_pGraphicDev, L"Ring2_Normal", pSheet1, RING_NORMAL2);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));


	for (int i = 0; i < 20; ++i)
	{
		pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_HP", pSheet3, POTION_HP);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet3)->AddInven(((CUI_Item*)pGameObject));
	}

	for (int i = 0; i < 20; ++i)
	{
		pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_MP", pSheet3, POTION_MP);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet3)->AddInven(((CUI_Item*)pGameObject));
	}

	for (int i = 0; i < 20; ++i)
	{
		pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_Level", pSheet3, POTION_LEVEL);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CInven_Sheet*)pSheet3)->AddInven(((CUI_Item*)pGameObject));
	}


	////////////////////////////////////////////////////////////////////////////////////////////// 직업별 툴팁 /////////////////////////////////////////////////////////////////////////////

	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CToolTip::Create(m_pGraphicDev, L"ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CWear_ToolTip::Create(m_pGraphicDev, L"Wear_ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CCompare_ToolTip::Create(m_pGraphicDev, L"Compare_ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CShop_ToolTip::Create(m_pGraphicDev, L"Shop_ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CSkill_ToolTip::Create(m_pGraphicDev, L"Skill_ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CSkill_Avi::Create(m_pGraphicDev, L"Skill_Avi", true);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
	}
	else
	{
		pGameObject = CToolTip::Create(m_pGraphicDev, L"ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CWear_ToolTip::Create(m_pGraphicDev, L"Wear_ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CCompare_ToolTip::Create(m_pGraphicDev, L"Compare_ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CShop_ToolTip::Create(m_pGraphicDev, L"Shop_ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CSkill_ToolTip::Create(m_pGraphicDev, L"Skill_ToolTip");
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pGameObject = CSkill_Avi::Create(m_pGraphicDev, L"Skill_Avi", true);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
	}

	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CSkill_Base::Create(m_pGraphicDev, L"Skill_Base", true);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pUI = ((CUI*)pGameObject);
	}
	else
	{
		pGameObject = CSkill_Base::Create(m_pGraphicDev, L"Skill_Base", false);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pUI = ((CUI*)pGameObject);

	}


	pGameObject = CSkill_Sheet::Create(m_pGraphicDev, L"Skill_Sheet_1", pUI, 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pSheet1 = ((CUI*)pGameObject);	////////////////////// 스킬 블럭////////////////////////////////////

	pGameObject = CPage_Back_Button::Create(m_pGraphicDev, L"Page_Back_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CPage_Next_Button::Create(m_pGraphicDev, L"Page_Next_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	for (_float fY = -57.5; fY > -350; fY -= 65)
	{
		pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_1", pSheet1, _vec2(40, fY));
		if (NULL == pGameObject)
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSheet(0, ((CUI*)pGameObject));
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
	}

	for (_float fY = -57.5; fY > -350; fY -= 65)
	{
		pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_1", pSheet1, _vec2(-160, fY));
		if (NULL == pGameObject)
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSheet(0, ((CUI*)pGameObject));
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////// //////////////////////////////// 액션 스킬 //////////////////////////////////
	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CUI_Rush::Create(m_pGraphicDev, L"UI_Rush", pSheet1, SKILL_RUSH, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 0, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_FlyDance::Create(m_pGraphicDev, L"UI_FlyDance", pSheet1, SKILL_FLYDANCE, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 4, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_FladgeBlade::Create(m_pGraphicDev, L"UI_FladgeBlade", pSheet1, SKILL_FLEDGE_BLADE, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 1, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_SpinTurn::Create(m_pGraphicDev, L"UI_SpinTurn", pSheet1, SKILL_STUNPIN, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 5, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_Violet::Create(m_pGraphicDev, L"UI_Violet", pSheet1, SKILL_VIOLET, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 2, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_CrossBrits::Create(m_pGraphicDev, L"UI_CrossBrits", pSheet1, SKILL_CROSS, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 6, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_SpirerDance::Create(m_pGraphicDev, L"UI_SpirerDance", pSheet1, SKILL_SPIRALDANCE, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 3, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_DarkPolling::Create(m_pGraphicDev, L"UI_DarkPolling", pSheet1, SKILL_DARKPOLLING, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 7, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_BlackRose::Create(m_pGraphicDev, L"UI_BlackRose", pSheet1, SKILL_BLACKROSE, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 8, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_DragonDance::Create(m_pGraphicDev, L"UI_DragonDance", pSheet1, SKILL_DRAGONDANCE, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 9, ((CUI_Skill*)pGameObject));
	}
	else
	{
		pGameObject = CUI_Rush::Create(m_pGraphicDev, L"UI_Rush", pSheet1, SKILL_RUSH, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 0, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_LockOn::Create(m_pGraphicDev, L"UI_LockOn", pSheet1, SKILL_LOCKON, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 5, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_ShotJump::Create(m_pGraphicDev, L"UI_ShotJump", pSheet1, SKILL_SHOTJUMP, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 1, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_UpperKick::Create(m_pGraphicDev, L"UI_UpperKick", pSheet1, SKILL_UPPERKICK, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 6, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_FireM::Create(m_pGraphicDev, L"UI_FireM", pSheet1, SKILL_FIREM, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 2, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_Gravity::Create(m_pGraphicDev, L"UI_Gravity", pSheet1, SKILL_GRAVITY, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 7, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_SpinCrash::Create(m_pGraphicDev, L"UI_SpinCrash", pSheet1, SKILL_SPINCRASH, SKILL_TYPE_ACTION);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet1)->AddSkill(0, 3, ((CUI_Skill*)pGameObject));

	}


	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CSkill_Sheet::Create(m_pGraphicDev, L"Skill_Sheet_2", pUI, 1);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pSheet2 = ((CUI*)pGameObject);

		iIndex = 0;
		for (_float fY = -57.5; fY > -400; fY -= 65)
		{
			pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_2", pSheet2, _vec2(170, fY), iIndex);
			if (NULL == pGameObject)
				goto except;
			((CSkill_Sheet*)pSheet2)->AddSheet(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;

			++iIndex;
		}

		for (_float fY = -57.5; fY > -400; fY -= 65)
		{
			pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_2", pSheet2, _vec2(-30, fY), iIndex);
			if (NULL == pGameObject)
				goto except;
			((CSkill_Sheet*)pSheet2)->AddSheet(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;

			++iIndex;
		}

		for (_float fY = -57.5; fY > -400; fY -= 65)
		{
			pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_2", pSheet2, _vec2(170, fY), iIndex);
			if (NULL == pGameObject)
				goto except;
			((CSkill_Sheet*)pSheet2)->AddSheet(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;

			++iIndex;
		}

		for (_float fY = -57.5; fY > -400; fY -= 65)
		{
			pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_2", pSheet2, _vec2(-30, fY), iIndex);
			if (NULL == pGameObject)
				goto except;
			((CSkill_Sheet*)pSheet2)->AddSheet(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;

			++iIndex;
		}
	}
	else
	{
		pGameObject = CSkill_Sheet::Create(m_pGraphicDev, L"Skill_Sheet_2", pUI, 1, true);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;

		pSheet2 = ((CUI*)pGameObject);

		iIndex = 0;
		for (_float fY = -57.5; fY > -400; fY -= 65)
		{
			pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_2", pSheet2, _vec2(170, fY), iIndex);
			if (NULL == pGameObject)
				goto except;
			((CSkill_Sheet*)pSheet2)->AddSheet(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;

			++iIndex;
		}

		for (_float fY = -57.5; fY > -400; fY -= 65)
		{
			pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_2", pSheet2, _vec2(-30, fY), iIndex);
			if (NULL == pGameObject)
				goto except;
			((CSkill_Sheet*)pSheet2)->AddSheet(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;

			++iIndex;
		}

		for (_float fY = -57.5; fY > -400; fY -= 65)
		{
			pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_2", pSheet2, _vec2(170, fY), iIndex);
			if (NULL == pGameObject)
				goto except;
			((CSkill_Sheet*)pSheet2)->AddSheet(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;

			++iIndex;
		}

		for (_float fY = -57.5; fY > -400; fY -= 65)
		{
			pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_2", pSheet2, _vec2(-30, fY), iIndex);
			if (NULL == pGameObject)
				goto except;
			((CSkill_Sheet*)pSheet2)->AddSheet(1, ((CUI*)pGameObject));
			if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
				goto except;

			++iIndex;
		}
	}

	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CUI_Dash::Create(m_pGraphicDev, L"UI_Dash", pSheet2, SKILL_DASH, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 0, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_LiddleKick::Create(m_pGraphicDev, L"UI_LiddleKick", pSheet2, SKILL_LIDDLEKICK, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 1, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_EvadeAtt::Create(m_pGraphicDev, L"UI_EvadeAtt", pSheet2, SKILL_EVADEATT, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 2, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_TechDown::Create(m_pGraphicDev, L"UI_TechDown", pSheet2, SKILL_TECHDOWN, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 3, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_BlareSiluet::Create(m_pGraphicDev, L"UI_BlareSiluet", pSheet2, SKILL_BLARE, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 4, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_UpAtt::Create(m_pGraphicDev, L"UI_UpAtt", pSheet2, SKILL_UPATT, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 5, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_ViperKick::Create(m_pGraphicDev, L"UI_ViperKick", pSheet2, SKILL_VIFERKICK, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 6, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_SpiningHeart::Create(m_pGraphicDev, L"UI_SpiningHeart", pSheet2, SKILL_SPININGHEART, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 7, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_TrickyRabbit::Create(m_pGraphicDev, L"UI_TrickyRabbit", pSheet2, SKILL_TRICKEY, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 8, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_PlanitDance::Create(m_pGraphicDev, L"UI_Planit", pSheet2, SKILL_FLENITDANCE, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 9, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_Plora::Create(m_pGraphicDev, L"UI_Plora", pSheet2, SKILL_PLORA, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 10, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_SoftRanding::Create(m_pGraphicDev, L"UI_SoftRanding", pSheet2, SKILL_SOFTLANDING, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 11, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_SiluetDance::Create(m_pGraphicDev, L"UI_Siluet", pSheet2, SKILL_SILUETDANCE, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 12, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_HillTurn::Create(m_pGraphicDev, L"UI_HillTurn", pSheet2, SKILL_HILLTURN, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 18, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_UrbanBreak::Create(m_pGraphicDev, L"UI_UrbanBreak", pSheet2, SKILL_URBANBREAT, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 13, ((CUI_Skill*)pGameObject));
	}
	else
	{
		pGameObject = CUI_Dash::Create(m_pGraphicDev, L"UI_Dash", pSheet2, SKILL_DASH, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 0, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_TechDown::Create(m_pGraphicDev, L"UI_TechDown", pSheet2, SKILL_TECHDOWN, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 3, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_UpAtt::Create(m_pGraphicDev, L"UI_UpAtt", pSheet2, SKILL_UPATT, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 8, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_JumpDownKick::Create(m_pGraphicDev, L"UI_JumpDownKick", pSheet2, SKILL_JUMPDOWNKICK, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 1, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_Holdshot::Create(m_pGraphicDev, L"UI_HoldShot", pSheet2, SKILL_HOLDSHOT, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 2, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_GeilShot::Create(m_pGraphicDev, L"UI_GeilShot", pSheet2, SKILL_GEILSHOT, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 4, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_DownKick::Create(m_pGraphicDev, L"UI_DownKick", pSheet2, SKILL_DOWNKICK, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 6, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_ShellBuster::Create(m_pGraphicDev, L"UI_ShellBuster", pSheet2, SKILL_SHELLBUSTER, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 7, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_Escape::Create(m_pGraphicDev, L"UI_Escape", pSheet2, SKILL_ESCAPE, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 9, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_MovingCannon::Create(m_pGraphicDev, L"UI_MovingCannon", pSheet2, SKILL_MOVINGCANNON, SKILL_TYPE_COMMAND);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet2)->AddSkill(1, 10, ((CUI_Skill*)pGameObject));
	}

	pGameObject = CSkill_Sheet::Create(m_pGraphicDev, L"Skill_Sheet_3", pUI, 2);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pSheet3 = ((CUI*)pGameObject);

	for (_float fY = -57.5; fY > -200; fY -= 65)
	{
		pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_3", pSheet3, _vec2(300, fY));
		if (NULL == pGameObject)
			goto except;
		((CSkill_Sheet*)pSheet3)->AddSheet(2, ((CUI*)pGameObject));
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
	}

	for (_float fY = -57.5; fY > -150; fY -= 65)
	{
		pGameObject = CSkill_Block::Create(m_pGraphicDev, L"Skill_Block_3", pSheet3, _vec2(100, fY));
		if (NULL == pGameObject)
			goto except;
		((CSkill_Sheet*)pSheet3)->AddSheet(2, ((CUI*)pGameObject));
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
	}

	pGameObject = CUI_MaxHpUp::Create(m_pGraphicDev, L"UI_MaxHpUp", pSheet3, SKILL_MAXHP, SKILL_TYPE_PASSIVE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CSkill_Sheet*)pSheet3)->AddSkill(2, 0, ((CUI_Skill*)pGameObject));

	pGameObject = CUI_MaxMpUp::Create(m_pGraphicDev, L"UI_MaxMpUp", pSheet3, SKILL_MAXMP, SKILL_TYPE_PASSIVE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CSkill_Sheet*)pSheet3)->AddSkill(2, 3, ((CUI_Skill*)pGameObject));

	pGameObject = CUI_CriticalUp::Create(m_pGraphicDev, L"UI_CriUp", pSheet3, SKILL_CRITICAL, SKILL_TYPE_PASSIVE);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	((CSkill_Sheet*)pSheet3)->AddSkill(2, 1, ((CUI_Skill*)pGameObject));
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 패시브 스킬 ///////////////////////////////////////////
	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		pGameObject = CUI_DualBlade::Create(m_pGraphicDev, L"UI_DualBlade", pSheet3, SKILL_ATTMASTER, SKILL_TYPE_PASSIVE);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet3)->AddSkill(2, 4, ((CUI_Skill*)pGameObject));

		pGameObject = CUI_BloodAgil::Create(m_pGraphicDev, L"UI_BloodAgil", pSheet3, SKILL_BLOODAGIL, SKILL_TYPE_PASSIVE);
		if (NULL == pGameObject)
			goto except;
		if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
			goto except;
		((CSkill_Sheet*)pSheet3)->AddSkill(2, 2, ((CUI_Skill*)pGameObject));
	}
	else
	{

	}

	pGameObject = CWindow_Cancle::Create(m_pGraphicDev, L"Window_Skill_Cancle", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CPartition_Base::Create(m_pGraphicDev, L"Partition_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pUI = ((CUI*)pGameObject);

	pGameObject = CPartition_Up_Button::Create(m_pGraphicDev, L"Partition_Up_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CPartition_Down_Button::Create(m_pGraphicDev, L"Partition_Down_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CPartition_Max_Button::Create(m_pGraphicDev, L"Partition_Max_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CPartition_Button::Create(m_pGraphicDev, L"Partition_Max_Button", pUI, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CPartition_Button::Create(m_pGraphicDev, L"Partition_Max_Button", pUI, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CWindow_Cancle::Create(m_pGraphicDev, L"Window_Skill_Cancle", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CESC_Base::Create(m_pGraphicDev, L"Esc_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pUI = ((CUI*)pGameObject);

	pGameObject = CESC_Button::Create(m_pGraphicDev, L"System_Button", pUI, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CESC_Button::Create(m_pGraphicDev, L"Exit_Button", pUI, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CWindow_Cancle::Create(m_pGraphicDev, L"Window_Skill_Cancle", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CSystem_Base::Create(m_pGraphicDev, L"System_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pUI = ((CUI*)pGameObject);

	pGameObject = CButton_Exit::Create(m_pGraphicDev, L"System_Exit_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_LevelUp::Create(m_pGraphicDev, L"System_LevelUp_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Reset::Create(m_pGraphicDev, L"System_Reset_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Gold::Create(m_pGraphicDev, L"System_Gold_Button", pUI);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Item::Create(m_pGraphicDev, L"System_Item_Button", pUI, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_HDR::Create(m_pGraphicDev, L"System_HDR_On_Button", pUI, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_HDR::Create(m_pGraphicDev, L"System_HDR_Off_Button", pUI, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_SSAO::Create(m_pGraphicDev, L"System_SSAO_On_Button", pUI, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_SSAO::Create(m_pGraphicDev, L"System_SSAO_Off_Button", pUI, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_DOF::Create(m_pGraphicDev, L"System_DOF_On_Button", pUI, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_DOF::Create(m_pGraphicDev, L"System_DOF_Off_Button", pUI, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_MB::Create(m_pGraphicDev, L"System_MB_On_Button", pUI, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_MB::Create(m_pGraphicDev, L"System_MB_Off_Button", pUI, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_LF::Create(m_pGraphicDev, L"System_LF_On_Button", pUI, true, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_LF::Create(m_pGraphicDev, L"System_LF_Off_Button", pUI, false, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Fog::Create(m_pGraphicDev, L"System_Fog_Default_Button", pUI, 0, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Fog::Create(m_pGraphicDev, L"System_Fog_Index_Button", pUI, 1, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Fog::Create(m_pGraphicDev, L"System_Fog_Pow_Index_Button", pUI, 2, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Fog::Create(m_pGraphicDev, L"System_Fog_Height_Button", pUI, 3, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Shadow::Create(m_pGraphicDev, L"System_Shadow_Default_Button", pUI, 0, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Shadow::Create(m_pGraphicDev, L"System_Shadow_Normal_Button", pUI, 1, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Shadow::Create(m_pGraphicDev, L"System_Shadow_Cas_Button", pUI, 2, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CButton_Shadow::Create(m_pGraphicDev, L"System_Shadow_Soft_Button", pUI, 3, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CDebug_Check::Create(m_pGraphicDev, L"Debug_Check", pUI, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;


	pGameObject = CSlide_Bright::Create(m_pGraphicDev, L"Slide_Bright", pUI, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CSlide_Bloom::Create(m_pGraphicDev, L"Slide_Bloom", pUI, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CSlide_BackSound::Create(m_pGraphicDev, L"Slide_BackSound", pUI, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;

	pGameObject = CSlide_EffectSound::Create(m_pGraphicDev, L"Slide_EffectSound", pUI, this);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI_Window", pGameObject)))
		goto except;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	pGameObject = CHitDir::Create(m_pGraphicDev, L"HitDir");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CPlayerHit::Create(m_pGraphicDev, L"PlayerHit", m_pPlayer->GetPlayerInfo()->m_eClassID);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CState_Base::Create(m_pGraphicDev, L"State_Base", m_pPlayer->GetPlayerInfo()->m_eClassID);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CLevel_Base::Create(m_pGraphicDev, L"Level_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CState_Bar::Create(m_pGraphicDev, L"State_Hp_Bar", 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CState_Bar::Create(m_pGraphicDev, L"State_Mp_Bar", 1);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CState_Gage::Create(m_pGraphicDev, L"HP_Gage", 0);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CState_Gage::Create(m_pGraphicDev, L"MP_Gage", 1);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CClass_Texture::Create(m_pGraphicDev, L"Class_Texture", m_pPlayer->GetPlayerInfo()->m_eClassID);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CClass_Mark_Base::Create(m_pGraphicDev, L"Class_Mark_Base");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CClass_Mark::Create(m_pGraphicDev, L"Class_Mark", m_pPlayer->GetPlayerInfo()->m_eClassID);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;


	pGameObject = CMinimap_Base::Create(m_pGraphicDev, L"Minimap_Base", L"빔 펠리");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CMiniMap_ZoomIn::Create(m_pGraphicDev, L"Minimap_ZoomIn");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CMiniMap_ZoomOut::Create(m_pGraphicDev, L"Minimap_ZoomOut");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CCombo::Create(m_pGraphicDev, L"Combo");
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	Engine::CGameObject* pCombo = pGameObject;

	pGameObject = CComboEffect::Create(m_pGraphicDev, L"ComboEffect", pCombo);
	if (NULL == pGameObject)
		goto except;
	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	/// Commend CoolTime



	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));

	return S_OK;


except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CScene_Town::Ready_Resource(void)
{
	return S_OK;
}

HRESULT CScene_Town::Ready_Prototype(void)
{
	return S_OK;
}

void CScene_Town::SaveSlotIndex(_uint iSaveIndex)
{
	m_iSlotIndex = iSaveIndex;
}

void CScene_Town::LoadPlayerInfo(CPlayer* pPlayer)
{
	_tchar		szPath[128] = L"";
	wsprintf(szPath, L"../Bin/Data/PlayerInfo/Player%d.dat", m_iSlotIndex);

	DWORD	dwByte = 0;

	HANDLE hFile = CreateFile(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int iIndex = 0;

	PLAYER_INFO MyPlayerInfo;
	memset(&MyPlayerInfo, NULL, sizeof(MyPlayerInfo));

	while (true)
	{
		PLAYER_INFO* pPlayerInfo = new PLAYER_INFO;

		if (!ReadFile(hFile, pPlayerInfo, sizeof(PLAYER_INFO), &dwByte, NULL))
		{
			delete pPlayerInfo;
			pPlayerInfo = NULL;

			CloseHandle(hFile);
			return;
		}

		if (!iIndex)
		{
			memcpy(&MyPlayerInfo, pPlayerInfo, sizeof(PLAYER_INFO));

			delete pPlayerInfo;
			pPlayerInfo = NULL;
		}
		if (dwByte == 0)
		{
			delete pPlayerInfo;
			pPlayerInfo = NULL;
			break;
		}

		++iIndex;
	}

	CloseHandle(hFile);

	m_pPlayer = pPlayer;
	m_pPlayer->SetPlayerInfo(&MyPlayerInfo);
}

void CScene_Town::SavePlayerInfo(void)
{
	_tchar		szPath[128];
	wsprintf(szPath, L"../Bin/Data/PlayerInfo/Player%d.dat", m_iSlotIndex);

	DWORD	dwByte = 0;
	HANDLE	hFile = CreateFile(szPath, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	CPlayer* pPlayer = static_cast<CWitchBlade*>(Engine::Find(L"Player"));

	WriteFile(hFile, pPlayer->GetPlayerInfo(), sizeof(PLAYER_INFO), &dwByte, NULL);

	CloseHandle(hFile);

	Engine::Safe_Release(pPlayer);
}

void CScene_Town::SkipScene(const _float& fTimeDelta)
{

}

void CScene_Town::KeyCheck(const _float& fTimeDelta)
{
	/*if(Engine::Get_DIKeyState(DIK_NUMPAD1)	& 0x80)
	{
	m_eCameraID = Engine::CAMERA_WORLD;
	Engine::DefineCurrentCamera(m_eCameraID);
	}*/

	if (Engine::Get_DIKeyState(DIK_NUMPAD2) & 0x80)
	{
		m_eCameraID = Engine::CAMERA_PLAYER;
		Engine::DefineCurrentCamera(m_eCameraID);
	}

	if (Engine::Get_DIKeyState(DIK_NUMPAD3) & 0x80)
	{
		m_eCameraID = Engine::CAMERA_MINIMAP;
		Engine::DefineCurrentCamera(m_eCameraID);
	}

	if (Engine::Get_DIKeyState(DIK_NUMPADPLUS) & 0x80)
	{
		CPlayer* pPlayer = static_cast<CWitchBlade*>(Engine::Find(L"Player"));

		PLAYER_INFO* pPlayerInfo = pPlayer->GetPlayerInfo();
		pPlayerInfo->m_iLevel += 1;

		Engine::Safe_Release(pPlayer);
	}
}


CScene_Town* CScene_Town::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Town*			pInstance = new CScene_Town(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Town Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CScene_Town* CScene_Town::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iIndex, CLASSID eClassID)
{
	CScene_Town*			pInstance = new CScene_Town(pGraphicDev);

	pInstance->SaveSlotIndex(iIndex);
	pInstance->SetClassID(eClassID);

	if (FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Town Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

HRESULT CScene_Town::Ready_GameObject(void)
{
	Engine::CGameObject*	pGameObject = NULL;

	DWORD	dwByte = 0;
	HANDLE hFile = CreateFile(L"../Bin/Data/Stage_Town/Town_Obj.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (true)
	{
		OBJ_INFO*	pInfo = new OBJ_INFO;

		ReadFile(hFile, pInfo, sizeof(OBJ_INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			delete pInfo;
			break;
		}

		wstring szTemp;

		szTemp = (wchar_t*)pInfo->m_szTag;

		if (szTemp == L"NPC")
		{
			wstring szName;

			szName = (wchar_t*)pInfo->m_szName;

			if (szName == L"Mesh_NPC_Weapon")
			{
				pGameObject = CNPC_Weapon::Create(m_pGraphicDev, pInfo->m_szName);

				((CNPC*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, pInfo->m_szName, pGameObject);
			}

			if (szName == L"Mesh_NPC_Armor")
			{
				pGameObject = CNPC_Armor::Create(m_pGraphicDev, pInfo->m_szName);

				((CNPC*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, pInfo->m_szName, pGameObject);
			}

			if (szName == L"Mesh_NPC_Potion")
			{
				pGameObject = CNPC_Potion::Create(m_pGraphicDev, pInfo->m_szName);

				((CNPC*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, pInfo->m_szName, pGameObject);
			}

			if (szName == L"Mesh_NPC_Book")
			{
				pGameObject = CNPC_Book::Create(m_pGraphicDev, pInfo->m_szName);

				((CNPC*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(pInfo->m_eLayerType, pInfo->m_szName, pGameObject);
			}

		}

		else if (szTemp == L"Environment")
		{
			pGameObject = CEnvironment::Create(m_pGraphicDev, pInfo->m_szName);

			((CEnvironment*)pGameObject)->SetObjectInfo(&pInfo->m_vPos, &pInfo->m_vScale, &pInfo->m_vAngle);

			Engine::AddObject(pInfo->m_eLayerType, pInfo->m_szTag, pGameObject);
		}


		delete pInfo;
		pInfo = NULL;
	}

	CloseHandle(hFile);

	return S_OK;

}

//마을 횃불 이펙트 설치
HRESULT CScene_Town::Ready_GameEffect(void)
{

	Engine::CGameObject*	pRedFireObject = NULL;
	Engine::CGameObject*	pBlueFireObject = NULL;

	DWORD	dwByte = 0;
	HANDLE hFile = CreateFile(L"../Bin/Data/EffectData/StoreFire.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	pRedFireObject = CFire::LoadCreate(m_pGraphicDev, L"RedFire");

	pBlueFireObject = CFire::LoadCreate(m_pGraphicDev, L"BlueFire");



	while (true)
	{
		OBJ_INFO	pInfo;

		ZeroMemory(&pInfo, sizeof(OBJ_INFO));

		ReadFile(hFile, &pInfo, sizeof(OBJ_INFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			break;
		}

		wstring szTemp;

		szTemp = (wchar_t*)pInfo.m_szTag;

		if (szTemp == L"Effect")
		{
			wstring szName;

			szName = (wchar_t*)pInfo.m_szName;


			if (szName == L"RedFire")
			{
				((CFire*)pRedFireObject)->AddInitSetPart(&pInfo.m_vPos);
			}
			else if (szName == L"BlueFire")
			{
				((CFire*)pBlueFireObject)->AddInitSetPart(&pInfo.m_vPos);
			}

		}
	}

	((CFire*)pRedFireObject)->SetInitAnimation();
	((CFire*)pBlueFireObject)->SetInitAnimation();


	Engine::AddObject(Engine::LAYER_ENVIRONMENT, L"Effect_RedFire", pRedFireObject);
	Engine::AddObject(Engine::LAYER_ENVIRONMENT, L"Effect_BlueFire", pBlueFireObject);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CScene_Town::Ready_EffectLight(void)
{
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	CFire* pRedFire = static_cast<CFire*>(Engine::FindWithTag(L"Effect_RedFire"));
	//CFire* pBlueFire = static_cast<CFire*>(Engine::FindWithTag(L"Effect_BlueFire"));

	vector<_vec3>* RedFireVec = pRedFire->GetVecPos();
	//vector<_vec3>* BlueFireVec = pBlueFire->GetVecPos();


	for (size_t i = 0; i<RedFireVec->size(); ++i)
	{
		LightInfo.Type = D3DLIGHT_POINT;
		LightInfo.Range = 15.5f;

		LightInfo.Position = (*RedFireVec)[i] + _vec3(0.0f, 2.0f, 0.0f);

		LightInfo.Diffuse = D3DXCOLOR(1.0f, 0.2705f, 0.0f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.0001f, 0.0001f, 0.0001f, 1.0f);

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
		++m_iLightCount;
	}

	/*for (size_t i = 0; i<BlueFireVec->size(); ++i)
	{
		LightInfo.Type = D3DLIGHT_POINT;
		LightInfo.Range = 14.5f;

		LightInfo.Position = (*BlueFireVec)[i] + _vec3(0.0f, 2.0f, 0.0f);

		LightInfo.Diffuse = D3DXCOLOR(0.09f, 0.698f, 0.666f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(0.09f, 0.698f, 0.666f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

		LightInfo.Attenuation0 = 0.0f;
		LightInfo.Attenuation1 = 0.8f;
		LightInfo.Attenuation2 = 0.0f;

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
		++m_iLightCount;
	}*/
	
	//포션 상점
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 20.0f;

	LightInfo.Position = _vec3(25.0f, 17.0f, -46.0f);

	LightInfo.Diffuse = D3DXCOLOR(0.09f, 0.698f, 0.666f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.0001f, 0.0001f, 0.0001f, 1.0f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//장신구 상점
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 22.0f;

	LightInfo.Position = _vec3(85.0f, 17.0f, 6.0f);

	LightInfo.Diffuse = D3DXCOLOR(1.0f, 0.2705f, 0.0f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.0001f, 0.0001f, 0.0001f, 1.0f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//무기 상점
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 22.0f;

	LightInfo.Position = _vec3(-86.0f, 17.0f, -51.0f);

	LightInfo.Diffuse = D3DXCOLOR(1.0f, 0.2705f, 0.0f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.0001f, 0.0001f, 0.0001f, 1.0f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	//방어구 상점
	LightInfo.Type = D3DLIGHT_POINT;
	LightInfo.Range = 22.0f;

	LightInfo.Position = _vec3(-32.0f, 17.0f, -113.0f);

	LightInfo.Diffuse = D3DXCOLOR(1.0f, 0.2705f, 0.0f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.0001f, 0.0001f, 0.0001f, 1.0f);

	if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, m_iLightCount)))	return E_FAIL;
	++m_iLightCount;

	Engine::Safe_Release(pRedFire);
	//Engine::Safe_Release(pBlueFire);

	return S_OK;
}



void CScene_Town::Free(void)
{
	//SavePlayerInfo();
	CCulling_Manager::GetInstance()->StopThread();
	Engine::Clear_All();
	Engine::CScene::Free();
}

