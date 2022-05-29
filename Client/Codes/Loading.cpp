#include "stdafx.h"
#include "Loading.h"
#include "Export_Engine.h"

#include "Sound_Manager.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_isFinish(false)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szloadingString, sizeof(_tchar) * 128);
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_LoadingID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForLogo(void)
{
	CSound_Manager::GetInstance()->Ready_SoundManager();

	// Texture for Login Scene
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Login_BackGround", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Logoback.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Login_Top_Sever", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Sever_base.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Login_Bottom_Sever", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Sever_base_01.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Login_AMD", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_AMD.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Login_Banner", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/GP_UI_CI.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Login_Window", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Panel_001.png", 1)))
		return E_FAIL;

	/*if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Login_NotCol_Button", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Normal_Button_NC.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Login_Col_Button", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Normal_Button_C.png", 1)))
		return E_FAIL;*/

	// Texture for Select Scene

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Select_Side_Bar", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Char_Option_Basic.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Select_Char_Back_F", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Select_Char_Back_F.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Select_Char_Back_H", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Select_Char_Back_H.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Select_Char_Back_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Select_Char_Back_S.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Select_Char_Back_W", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Select_Char_Back_W.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Select_Char_Back_M", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Select_Char_Back_M.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Select_Char_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Select_Char_Button_NC.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Select_Char_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Select_Char_Button_C.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Select_Char_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Select_Char_Button_Click.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Game_Start_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/GameStart_NC.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Game_Start_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/GameStart_C.png", 1)))
		return E_FAIL;

	// Texture for Create Scene
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Create_Fighter", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_pk.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Create_Hunter", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_ph.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Create_Shaman", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_pm.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Create_WitchBlade", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_pw.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Create_Mistic", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_ps.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Create_Info", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Class_Info_Panel.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_Pentagon", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_ClassDiagram.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_F_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_F_NC.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_F_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_F_C.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_F_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_F_S.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_H_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_H_NC.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_H_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_H_C.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_H_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_H_S.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_S_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_S_NC.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_S_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_S_C.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_S_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_S_S.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_W_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_W_NC.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_W_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_W_C.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_W_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_W_S.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_M_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_M_NC.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_M_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_M_C.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Button_M_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Class_Button_M_S.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_BackGround_F", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_pk.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_BackGround_H", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_ph.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_BackGround_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_pm.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_BackGround_W", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_pw.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_BackGround_M", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Character_BG_ps.png", 1)))
		return E_FAIL;


	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Texture_F", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Char_pk.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Texture_H", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Char_ph.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Texture_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Char_pm.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Texture_W", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Char_pw.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_Texture_M", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_Loing_Char_ps.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Class_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Create_Class_ToolTip.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Create_Input_ID", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Create_Input_ID.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_NorMal_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Normal_Button_NC.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_NorMal_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Normal_Button_C.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Texture_NorMal_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Normal_Button_Click.png", 1)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_WitchBlade", Engine::MESH_DYNAMIC, L"../Bin/Resource/Mesh/Dynamic/Select_Character/WitchBlade/", L"WitchBlade.X")))
	{
		MSG_BOX("Mesh_WitchBlade Loading Failed");
		return E_FAIL;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Fighter", Engine::MESH_DYNAMIC, L"../Bin/Resource/Mesh/Dynamic/Select_Character/Fighter/", L"Fighter.X")))
	{
		MSG_BOX("Mesh_Fighter Loading Failed");
		return E_FAIL;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Hunter", Engine::MESH_DYNAMIC, L"../Bin/Resource/Mesh/Dynamic/Select_Character/Hunter/", L"Hunter.X")))
	{
		MSG_BOX("Mesh_Hunter Loading Failed");
		return E_FAIL;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Shaman", Engine::MESH_DYNAMIC, L"../Bin/Resource/Mesh/Dynamic/Select_Character/Shaman/", L"Shaman.X")))
	{
		MSG_BOX("Mesh_Shaman Loading Failed");
		return E_FAIL;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Mistic", Engine::MESH_DYNAMIC, L"../Bin/Resource/Mesh/Dynamic/Select_Character/Mistic/", L"Mistic.X")))
	{
		MSG_BOX("Mesh_Mistic Loading Failed");
		return E_FAIL;
	}


	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Floor_Fighter", Engine::MESH_STATIC, L"../Bin/Resource/Mesh/Static/Selcetion_Stage/", L"PK_Stage.X")))
	{
		MSG_BOX("Mesh_Mistic Loading Failed");
		return E_FAIL;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Floor_Hunter", Engine::MESH_STATIC, L"../Bin/Resource/Mesh/Static/Selcetion_Stage/", L"PH_Stage.X")))
	{
		MSG_BOX("Mesh_Mistic Loading Failed");
		return E_FAIL;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Floor_Shaman", Engine::MESH_STATIC,  L"../Bin/Resource/Mesh/Static/Selcetion_Stage/", L"PM_Stage.X")))
	{
		MSG_BOX("Mesh_Mistic Loading Failed");
		return E_FAIL;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Floor_WitchBlade", Engine::MESH_STATIC,  L"../Bin/Resource/Mesh/Static/Selcetion_Stage/", L"PW_Stage.X")))
	{
		MSG_BOX("Mesh_Mistic Loading Failed");
		return E_FAIL;
	}
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGOSTATIC, L"Mesh_Floor_Mistic", Engine::MESH_STATIC,  L"../Bin/Resource/Mesh/Static/Selcetion_Stage/", L"PS_Stage.X")))
	{
		MSG_BOX("Mesh_Mistic Loading Failed");
		return E_FAIL;
	}
	m_isFinish = true;

	return 0;

}

_uint CLoading::Loading_ForTown(void)
{
	// For.Meshes Loading

	Engine::Reset_Resource(RESOURCE_MESH_TOWN);

	// 상점
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_BigHall",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_BigHall.X")))
	{
		MSG_BOX("Mesh Map Store_BigHall Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_FrontHall",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_FrontHall.X")))
	{
		MSG_BOX("Mesh Map Store_FrontHall Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_FrontHall_Front",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_FrontHall_Front.X")))
	{
		MSG_BOX("Mesh Map Store_FrontHall_Front Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_FrontHall_Right",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_FrontHall_Right.X")))
	{
		MSG_BOX("Mesh Map Store_FrontHall_Right Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_LeftHall",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_LeftHall.X")))
	{
		MSG_BOX("Mesh Map Store_LeftHall Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_LeftHall_Front",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_LeftHall_Front.X")))
	{
		MSG_BOX("Mesh Map Store_LeftHall_Front Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_LeftHall_Right",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_LeftHall_Right.X")))
	{
		MSG_BOX("Mesh Map Store_LeftHall_Right Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_Stairs",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_Stairs.X")))
	{
		MSG_BOX("Mesh Map Store_Stairs Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_Map_Store_Stairs_front",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_Stairs_front.X")))
	{
		MSG_BOX("Mesh Map Store_Stairs_front Load Failed");
		return E_FAIL;
	}

	/*if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
	RESOURCE_STAGE,
	L"Mesh_Map_Store_roof",
	Engine::MESH_STATIC,
	L"../Bin/Resource/Mesh/Static/Store_divide/",
	L"Store_roof.X")))
	{
	MSG_BOX("Mesh Map Store_Stairs_front Load Failed");
	return E_FAIL;
	}
	*/
	////아이스맵

	//if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
	//	RESOURCE_STAGE,
	//	L"Mesh_Map_Ice",
	//	Engine::MESH_STATIC,
	//	L"../Bin/Resource/Mesh/Static/SoulValley/",
	//	L"SoulValley.X")))
	//{
	//	MSG_BOX("Mesh Map Ice Load Failed");
	//	return E_FAIL;
	//}

	//마을 오브젝트...

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_BookShelf",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/BookShelf/",
		L"BookShelf.X")))
	{
		MSG_BOX("Mesh BookShelf Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_PaperBook",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/PaperBook/",
		L"PaperBook.X")))
	{
		MSG_BOX("Mesh PaperBook Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreArmor",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreArmor/",
		L"StoreArmor.X")))
	{
		MSG_BOX("Mesh StoreArmor Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreBow",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreBow/",
		L"StoreBow.X")))
	{
		MSG_BOX("Mesh Bow Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreDagger",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreDagger/",
		L"StoreDagger.X")))
	{
		MSG_BOX("Mesh StoreDagger Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StorePotion",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StorePotion/",
		L"StorePotion.X")))
	{
		MSG_BOX("Mesh StorePotion Load Failed");
		return E_FAIL;
	}


	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreSword",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreSword/",
		L"StoreSword.X")))
	{
		MSG_BOX("Mesh StoreSword Load Failed");
		return E_FAIL;
	}


	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreShield",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreShield/",
		L"StoreShield.X")))
	{
		MSG_BOX("Mesh StoreShield Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreBook",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreBook/",
		L"StoreBook.X")))
	{
		MSG_BOX("Mesh StoreBook Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreTable",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreTable/",
		L"StoreTable.X")))
	{
		MSG_BOX("Mesh StoreTable Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreTable2",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreTable2/",
		L"StoreTable2.X")))
	{
		MSG_BOX("Mesh StoreTable Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreBoard",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreBoard/",
		L"StoreBoard.X")))
	{
		MSG_BOX("Mesh StoreBoard Load Failed");
		return E_FAIL;
	}


	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreBoots",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreBoots/",
		L"StoreBoots.X")))
	{
		MSG_BOX("Mesh StoreBoots Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreCandle",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreCandle/",
		L"StoreCandle.X")))
	{
		MSG_BOX("Mesh StoreCandle Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_StoreSpear",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/StoreSpear/",
		L"StoreSpear.X")))
	{
		MSG_BOX("Mesh StoreSpear Load Failed");
		return E_FAIL;
	}

	//NPC..
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_NPC_Weapon",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/NPC/Alegon/",
		L"Alegon.X")))
	{
		MSG_BOX("Mesh_NPC_Weapon Loading Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_NPC_Armor",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/NPC/Eyine/",
		L"Eyine.X")))
	{
		MSG_BOX("Mesh_NPC_Armor Loading Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_NPC_Book",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/NPC/Aremoni/",
		L"Aremoni.X")))
	{
		MSG_BOX("Mesh_NPC_Potion Loading Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_TOWN,
		L"Mesh_NPC_Potion",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/NPC/Ruisa/",
		L"Ruisa.X")))
	{
		MSG_BOX("Mesh_NPC_Book Loading Failed");
		return E_FAIL;
	}

	if (!g_bEnterToLoadingTown)
	{
		//플레이어..

		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_MESH, L"Mesh_WitchBlade", Engine::MESH_DYNAMIC, L"../Bin/Resource/Mesh/Dynamic/Player_WitchBlade/", L"WitchBlade.X")))
		{
			MSG_BOX("Mesh_Player Loading Failed");
			return E_FAIL;
		}
		//Player 무기 : Sword1
		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_Sword1",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/WitchBladeSword/",
			L"Sword1.X")))
		{
			MSG_BOX("Mesh_Sword1 Loading Failed");
			return E_FAIL;
		}

		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_SwordL1",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/WitchBladeSword/",
			L"SwordL1.X")))
		{
			MSG_BOX("Mesh_Sword1 Loading Failed");
			return E_FAIL;
		}

		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_UI_Sword1",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/UI_WB_Sword/",
			L"UI_Sword1.X")))
		{
			MSG_BOX("Mesh_Sword1 Loading Failed");
			return E_FAIL;
		}

		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_UI_SwordL1",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/UI_WB_Sword/",
			L"UI_SwordL1.X")))
		{
			MSG_BOX("Mesh_Sword1 Loading Failed");
			return E_FAIL;
		}

		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_Reflect",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/Reflect/",
			L"Reflect.X")))
		{
			MSG_BOX("Reflect Loading Failed");
			return E_FAIL;
		}

		//DarkPollingGrave Swoard
		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_DarkSword",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/DarkSword/GreatSword_Aidan/",
			L"GreatSword_Aidan.X")))
		{
			MSG_BOX("DarkPollingSword Loading Failed");
			return E_FAIL;
		}

		//Ice Missile
		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_Icicle1",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/Ice_Missile_1/",
			L"Ice_Missile_1.X")))
		{
			MSG_BOX("Ice_Missile1 Loading Failed");
			return E_FAIL;
		}



		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_Icicle2",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/Ice_Missile_2/",
			L"Ice_Missile_2.X")))
		{
			MSG_BOX("Ice_Missile2 Loading Failed");
			return E_FAIL;
		}



		//Bardiel FireBall
		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_FireBall",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/FireBall/",
			L"FireBall.X")))
		{
			MSG_BOX("FireBall Loading Failed");
			return E_FAIL;
		}




		if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
			RESOURCE_MESH,
			L"Mesh_SpinningBullet",
			Engine::MESH_STATIC,
			L"../Bin/Resource/Mesh/Static/Arrow/",
			L"Arrow.X")))
		{
			MSG_BOX("Mesh SpinningBullet Load Failed");
			return E_FAIL;
		}

		g_bEnterToLoadingTown = true;
	}
	

	// UI Resource
		
	Engine::Reset_Resource(RESOURCE_STAGESTATIC);

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Texture_Login_BackGround", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/Logoback.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Random_NomalMap", Engine::TEX_NORMAL, L"../Bin/Resource/Random_Normal.dds", 1)))
		return E_FAIL;



	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Slot_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/SlotBox.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Exp_Gage_Bace", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Exp_Gage_Bace.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Exp_Gage", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Exp_Gage.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_PVP_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_PVP_Button_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_PVP_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_PVP_Button_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_PVP_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_PVP_Button_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Inven_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Inven_Button_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Inven_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Inven_Button_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Inven_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Inven_Button_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_CashShop_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_CashShop_Button_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_CashShop_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_CashShop_Button_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_CashShop_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_CashShop_Button_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Slot_Up_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Slot_Up_Button_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Slot_Up_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Slot_Up_Button_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Slot_Up_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Slot_Up_Button_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Slot_Down_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Slot_Down_Button_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Slot_Down_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Slot_Down_Button_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Slot_Down_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Slot_Down_Button_Click.png", 1)))
		return E_FAIL;


	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Tired_Gage_Left", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Tired_Gage_Left.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Main_Tired_Gage_Right", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Main/Main_Tired_Gage_Right.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_P", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_P.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_P_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_P_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_P_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_P_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Skill", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Skill.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Skill_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Skill_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Skill_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Skill_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Guild", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Guild.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Guild_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Guild_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Guild_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Guild_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Help", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Help.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Help_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Help_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Help_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Help_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Home", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Home.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Home_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Home_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Home_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Home_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_J", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_J.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_J_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_J_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_J_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_J_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_O", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_O.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_O_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_O_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_O_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_O_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Quest", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Quest.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Quest_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Quest_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_Quest_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_Quest_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_System", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_System.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_System_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_System_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Option_System_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Option/Option_System_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Window_Char_Info", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Window_Char_Info.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Player_State_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/Player_State_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shaman_State_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/Shaman_State_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Class_Icon", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/Class_Icon.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shaman_Icon", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/Shaman_Icon.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Level", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/Level.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"State_Bar", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/State_Bar.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"HP_Bar", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/HP_Gage.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MP_Bar", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/MP_Gage.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Class_Mark", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/Class_Mark.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Class_W_0", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/Class_W_0.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Class_M_1", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/Class_M_1.png", 1)))
		return E_FAIL;

	// MiniMap

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniNpc", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniNpc.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMonster", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMonster.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniBoss", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniBoss.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_ZoomIn_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_ZoomIn_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_ZoomIn_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_ZoomIn_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_ZoomIn_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_ZoomIn_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_ZoomOut_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_ZoomOut_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_ZoomOut_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_ZoomOut_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_ZoomOut_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_ZoomOut_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Cancle_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Cancle_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Cancle_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Cancle_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Cancle_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Cancle_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Window_Inven_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Window_Inven_Base.png", 1)))
		return E_FAIL;


	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Helmet_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Helmet_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shorder_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Shorder_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Armor_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Armor_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Leg_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Leg_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Glove_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Glove_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Boot_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Boot_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SkillStone_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/SkillStone_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"R_Weapon_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/R_Weapon_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Amulet_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Amulet_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Belt_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Belt_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Earing_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Earing_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"L_Weapon_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/L_Weapon_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Arrow_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Arrow_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Ring1_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Ring1_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Ring2_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Ring2_Space.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Armlet_Space", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Armlet_Space.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Model_Turn_Right_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Model_Turn_Right_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Model_Turn_Right_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Model_Turn_Right_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Model_Turn_Right_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Model_Turn_Right_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Model_Turn_Left_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Model_Turn_Left_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Model_Turn_Left_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Model_Turn_Left_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Model_Turn_Left_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Model_Turn_Left_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Sheet_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Sheet_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Sheet_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Sheet_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Sheet_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Sheet_S.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Scroll_Bar_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Scroll_Bar_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Scroll_Bar_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Scroll_Bar_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Scroll_Down_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Scroll_Down_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Scroll_Down_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Scroll_Down_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Scroll_Up_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Scroll_Up_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Scroll_Up_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Scroll_Up_NC.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Array_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Array_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Array_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Array_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Array_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Array_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Delete_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Delete_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Delete_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Delete_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Delete_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Delete_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Gold_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Gold_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Gold_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Gold_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Inven_Gold_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Inven_Gold_Click.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Weapon_Blue", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Blue_Weapon.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Weapon_Red", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Red_Weapon.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shaman_Blue_Weapon", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Shaman_Blue_Weapon.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shaman_Red_Weapon", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Shaman_Red_Weapon.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Helmet_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Helmet_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Armor_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Armor_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Leg_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Leg_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Glove_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Glove_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Boots_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Boots_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shorder_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Shorder_Normal.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Helmet", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/S_Helmet.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Armor", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/S_Armor.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Leg", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/S_Leg.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Glove", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/S_Glove.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Boots", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/S_Boots.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Shorder", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/S_Shorder.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Amulet_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Amulet_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Armlet_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Armlet_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Belt_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Belt_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Earing_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Earing_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Ring_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Ring1_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Ring2_Normal", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Ring2_Normal.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hp_Potion", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Hp_Potion.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Mp_Potion", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/Mp_Potion.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"LevelUp_Potion", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Item/LevelUp_Potion.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Weapon_Blue_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Weapon_Blue_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Weapon_Red_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Weapon_Red_ToolTip.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Weapon_S_Blue_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Weapon_S_Blue_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Weapon_S_Red_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Weapon_S_Red_ToolTip.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Helmet_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Helmet_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Armor_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Armor_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shorder_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Shorder_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Leg_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Leg_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Glove_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Glove_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Boots_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Boots_ToolTip.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Helmet_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/S_Helmet_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Armor_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/S_Armor_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Shorder_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/S_Shorder_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Leg_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/S_Leg_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Glove_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/S_Glove_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"S_Boots_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/S_Boots_ToolTip.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Amulet_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Amulet_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Armlet_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Armlet_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Belt_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Belt_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Earing_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Earing_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Ring1_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Ring1_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Ring2_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Ring2_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"HP_Potion_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/HP_Potion_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MP_Potion_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/MP_Potion_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Level_Potion_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Level_Potion_ToolTip.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Rush_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Rush_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"FlyDance_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/FlyDance_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"PledgeBlade_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/PledgeBlade_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BlackRose_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/BlackRose_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SpinTurn_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/SpinTurn_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Violet_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Violet_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Blare_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Blare_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Cross_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Cross_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Spirer_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Spirer_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DarkPolling_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/DarkPolling_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Dash_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Dash_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"RiddleKick_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/RiddleKick_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"EvadeAtt_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/EvadeAtt_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"TechDown_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/TechDown_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"UpAtt_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/UpAtt_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Viper_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Viper_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Spining_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Spining_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Tricky_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Tricky_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Planit_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Planit_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Plora_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Plora_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SoftLanding_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/SoftLanding_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Siluet_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Siluet_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"UrbanBreak_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/UrbanBreak_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"HillTurn_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/HillTurn_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MaxHpUp_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/MaxHpUp_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MaxMpUp_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/MaxMpUp_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Critical_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Critical_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DualAtt_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/DualAtt_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BloodAgil_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/BloodAgil_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DragonDance_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/DragonDance_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MoveCannon_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/MoveCannon_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"GeilShot_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/GeilShot_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Escape_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Escape_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"HoldShot_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/HoldShot_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ShellBuster_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/ShellBuster_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"JumpKick_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/JumpKick_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DownKick_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/DownKick_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Gravity_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/Gravity_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"FireM_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/FireM_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"UpperKick_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/UpperKick_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ShotJump_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/ShotJump_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"LockOn_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/LockOn_ToolTip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SpinCrash_ToolTip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/ToolTip/SpinCrash_ToolTip.png", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Window_Skill_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Window_Skill_Base_Action.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Window_Skill_Base_Comend1", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Window_Skill_Base_Comend1.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Window_Skill_Base_Comend2", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Window_Skill_Base_Comend2.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Window_Skill_Base_Passive", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Window_Skill_Base_Passive.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Skill_Page_Back_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Skill_Page_Back_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Skill_Page_Back_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Skill_Page_Back_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Skill_Page_Back_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Skill_Page_Back_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Skill_Page_Next_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Skill_Page_Next_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Skill_Page_Next_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Skill_Page_Next_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Skill_Page_Next_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Skill_Page_Next_Click.png", 1)))
		return E_FAIL;


	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"M_Skill_Base_Action", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/M_Skill_Base_Action.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"M_Skill_Base_Comend", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/M_Skill_Base_Comend.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"M_Skill_Base_Passive", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/M_Skill_Base_Passive.png", 1)))
		return E_FAIL;
	// Action Skill

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Rush", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/Rush.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"FlyDance", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/FlyDance.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"FladgeBlade", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/FladgeBlade.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SpinTurn", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/SpinTurn.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Violet", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/Violet.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"CrossBlits", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/CrossBlits.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SpirerDance", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/SpirerDance.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DarkPolling", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/DarkPolling.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BlackRose", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/BlackRose.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DragonDance", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/DragonDance.png", 1)))
		return E_FAIL;

	// command skill

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Dash", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/Dash.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"LiddleKick", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/LiddleKick.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"EvadeAtt", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/EvadeAtt.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"TechDown", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/TechDown.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BlareSiluet", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/BlareSiluet.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"UpAtt", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/UpAtt.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ViferKick", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/ViferKick.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SpiningHeart", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/SpiningHeart.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"TrickyRabbit", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/TrickyRabbit.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"PlanitDance", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/PlanitDance.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Flora", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/Flora.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Softlanding", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/Softlanding.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SiluetDance", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/SiluetDance.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"HillTurn", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/HillTurn.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"UrbanBreak", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/UrbanBreak.png", 1)))
		return E_FAIL;

	// Passive skill
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MaxHpUp", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/MaxHpUp.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MaxMpUp", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/MaxMpUp.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"CriUp", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/CriUp.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DualAtt", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/DualAtt.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DualBlade", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/DualBlade.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BloodAgil", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/BloodAgil.png", 1)))
		return E_FAIL;

	// M Skill
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DownKick", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/DownKick.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"JumpDownKick", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/JumpDownKick.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SpinCrash", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/SpinCrash.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Escape", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/Escape.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"HoldShot", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/HoldShot.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"UpperKick", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/UpperKick.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"FireM", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/FireM.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"GeilShot", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/GeilShot.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Gravity", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/Gravity.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"LockOn", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/LockOn.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MovingCanon", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/MovingCanon.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ShellBuster", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/ShellBuster.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ShotJump", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Skill/ShotJump.dds", 1)))
		return E_FAIL;

	// Partition UI
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Item_Partition_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Button_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Button_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Button_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Up_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Up_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Up_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Up_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Up_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Up_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Down_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Down_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Down_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Down_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Down_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Down_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Max_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Max_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Max_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Max_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Partition_Max_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Partition/Partition_Max_Click.png", 1)))
		return E_FAIL;


	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Window_Option_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Window_Option_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"System_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/System_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"System_CB_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/System_CB_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"System_CB_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/System_CB_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"System_CB_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/System_CB_S.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Slide_Button_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Slide_Button_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Slide_Button_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Slide_Button_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Slide_Button_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Slide_Button_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shop_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Shop_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Buy_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Buy_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Sell_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/Sell_Base.png", 1)))
		return E_FAIL;

	//Decal text
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"UI_CI", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/GP_UI_CI.png", 1)))
		return E_FAIL;


	//Texture Minimap
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Texture_MiniMap_Circle", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Texture_MiniMap_Alpha", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/Alpha.bmp", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Texture_MiniMap_Player", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/MiniMap/MiniMap_Player.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_Town", Engine::TEX_NORMAL, L"../Bin/Resource/UI/MiniMap/TownMiniMap.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_Ice", Engine::TEX_NORMAL, L"../Bin/Resource/UI/MiniMap/Stage1MiniMap.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"MiniMap_Fire", Engine::TEX_NORMAL, L"../Bin/Resource/UI/MiniMap/Stage2MiniMap.png", 1)))
		return E_FAIL;



	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"COMBO_FONT", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Numbering/Combo_Char.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"COMBO_EFFECT", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Numbering/Combo_Effect.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"COMBO_NUM", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Numbering/Combo_Num%d.png", 10)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DAM_NUM", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Numbering/Dam_Num%d.png", 10)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Critical", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Numbering/Critical.png", 1)))
		return E_FAIL;

	// Boss UI
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BossBar", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Boss/BossBar.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BossBar2", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Boss/BossBar2.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BossEffect", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Boss/BossEffect.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BossBarEffect", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Boss/BossBarEffect.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BossGage", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Boss/BossGage.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BossTex", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Boss/BossTex.png", 1)))
		return E_FAIL;

	//Result UI

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Best", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Best.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Bonus", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Bonus.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Bonus_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Bonus_S.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Excellent", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Excellent.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"GameResult", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/GameResult.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Gold", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Gold.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Kill", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Kill.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Mento", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Mento.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Nanip", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Nanip.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"PC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/PC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Rank_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Rank_S.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ResultBase", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/ResultBase2.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ReturnStage", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/ReturnStage2.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Skill", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Skill.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"StageClear", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/StageClear.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Style", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Style.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Technic", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Technic.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Total", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Total.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Good", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Good.png", 1)))
		return E_FAIL;

	//Result Number

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ResultNum", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Result%d.png", 12)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"ResultItemBack", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/ResultItemBack.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SelectItemChar", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/SelectItemChar.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SelectItemBox", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/SelectItemBox.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SelectItemLock_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/SelectItemLock_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SelectItemLock_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/SelectItemLock_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"NotSelectItemX", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/NotSelectItemX.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"GetItemWindow", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/GetItemWindow.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"GetItemWindowNS", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/GetItemWindowNS.png", 1)))
		return E_FAIL;

	// Shop Sprite

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Shop_Col", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/Shop_Col.png", 1)))
		return E_FAIL;

	// Player Level Up

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"LevelUp", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/LevelUp.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"LevelUp_Back", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/LevelUp_Back.png", 1)))
		return E_FAIL;

	// UpJuk

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Upjuk_Up", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Upjuk_Up.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Upjuk_Down", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Upjuk_Down.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Upjuk_Center", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Upjuk_Center.png", 1)))
		return E_FAIL;

	// Action Boss
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Action_Back", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Action_Back.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Action_Blood", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Action_Blood.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Action_Sub_Bardiel", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Action_Sub_Bardiel.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Action_Sub_Hanuman", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Result/Action_Sub_Hanuman.png", 1)))
		return E_FAIL;

	// Effect Hit
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"HitEffect", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/HitEffect.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"HitDir", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/HitDir.png", 1)))
		return E_FAIL;


	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Line", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Line.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Bim", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Bim.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Pell", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Pell.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Lee", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Lee.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Sung", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Sung.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Sang", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Sang.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Jum", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Jum.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Ji", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Ji.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Goo", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Goo.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_So", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_So.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Ul", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Ul.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Vell", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Vell.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Ha", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Ha.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Noo", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Noo.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Man", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Man.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Eu", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Eu.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Dong", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Dong.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Gool", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Gool.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Bol", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Bol.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_S.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Lan", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Lan.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_D", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_D.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Ba", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Ba.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_R", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_R.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Di", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Di.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_El", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_El.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Ju", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Ju.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Fade_Doon", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/FadeStage/Fade_Doon.png", 1)))
		return E_FAIL;

	//World UI
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Monster_Bar", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/Monster_Bar.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Monster_Gage", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/Monster_Gage.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Monster_Gage2", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/Monster_Gage2.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"M_War_Name", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/M_War_Name.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"M_Bow_Name", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/M_Bow_Name.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"M_Axe_Name", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/M_Axe_Name.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"M_Noll_Name", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/M_Noll_Name.png", 1)))
		return E_FAIL;
	//if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"GuildMark", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/PlayerState/GuildMark.png", 1)))
	//	return E_FAIL;

	//NPC Name
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Weapon_NPC_Name", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/Weapon_NPC_Name.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Armor_NPC_Name", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/Armor_NPC_Name.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Acc_NPC_Name", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/Acc_NPC_Name.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Potion_NPC_Name", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/World/Potion_NPC_Name.png", 1)))
		return E_FAIL;

	Engine::Reset_Resource(RESOURCE_TOWN);
	//Town UI
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Shadow", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Base2.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Select_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Select_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Select_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Select_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Select_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Select_S.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"C9_3th_Stage_SoulValley_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/C9_3th_Stage_SoulValley_NC.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"C9_3th_Stage_SoulValley_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/C9_3th_Stage_SoulValley_C.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"C9_3th_Stage_SoulValley_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/C9_3th_Stage_SoulValley_S.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"C9_2th_Stage_VolslandUT_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/C9_2th_Stage_VolslandUT_NC.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"C9_2th_Stage_VolslandUT_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/C9_2th_Stage_VolslandUT_C.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"C9_2th_Stage_VolslandUT_S", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/C9_2th_Stage_VolslandUT_S.dds", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Start_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Start_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Start_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Start_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Start_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Start_Click.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Back_NC", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Back_NC.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Back_C", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Back_C.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGESTATIC, L"DunGeon_Back_Click", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Stage/Window/DunGeon_Back_Click.png", 1)))
		return E_FAIL;
	
	// For. Buffers Loading
	lstrcpy(m_szloadingString, L"Buffer Loading...");

	m_isFinish = true;

	lstrcpy(m_szloadingString, L"Loading Complete...");

	return 0;
}

_uint CLoading::Loading_ForDungeonFire(void)
{
	Engine::Reset_Resource(RESOURCE_MESH_FIRE);

	//오브젝트..
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Torch2",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Torch2/",
		L"Torch2.X")))
	{
		MSG_BOX("Mesh Torch2  Load Failed");
		return E_FAIL;
	}

	//맵
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_0",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_0.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_1",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_1.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_2",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_2.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_3",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_3.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_4",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_4.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_5",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_5.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_6",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_6.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_7",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_7.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_8",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_8.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_9",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_9.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_10",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_10.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_11",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_11.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_12",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_12.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_13",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_13.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_14",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_14.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_15",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_15.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_16",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_16.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_17",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_17.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_18",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_18.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_19",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_19.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_20",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_20.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_21",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_21.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_22",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_22.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_23",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_23.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_24",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_24.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_25",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_25.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_26",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_26.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_27",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_27.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_28",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_28.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_29",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_29.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_30",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_30.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_31",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_31.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Map_Fire_32",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Volsland_UT/",
		L"Volsland_UT_32.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	//if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
	//	RESOURCE_MESH_FIRE,
	//	L"Mesh_Map_Fire_33",
	//	Engine::MESH_STATIC,
	//	L"../Bin/Resource/Mesh/Static/Volsland_UT/",
	//	L"Volsland_UT_33.X")))
	//{
	//	MSG_BOX("Mesh Map Fire Load Failed");
	//	return E_FAIL;
	//}

	//if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
	//	RESOURCE_MESH_FIRE,
	//	L"Mesh_Map_Fire_34",
	//	Engine::MESH_STATIC,
	//	L"../Bin/Resource/Mesh/Static/Volsland_UT/",
	//	L"Volsland_UT_34.X")))
	//{
	//	MSG_BOX("Mesh Map Fire Load Failed");
	//	return E_FAIL;
	//}

	//오브젝트..
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Bardiel_Sealed",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Bardiel_Sealed/",
		L"Bardiel_Sealed.X")))
	{
		MSG_BOX("Mesh_Bardiel_Sealed Load Failed");
		return E_FAIL;
	}

	//몬스터..
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_FIRE,
		L"Mesh_Bardiel",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/BardielBoss/",
		L"Bardiel.X")))
	{
		MSG_BOX("Mesh_Bardiel  Load Failed");
		return E_FAIL;
	}

	//Bardiel Blur Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_MESH_FIRE, L"Bardiel_Body_Blur_Effect", Engine::TEX_NORMAL, L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13601_ef.dds", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_MESH_FIRE, L"Bardiel_Shield_Blur_Effect", Engine::TEX_NORMAL, L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13602_ef.dds", 1)))
		return E_FAIL;

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_MESH_FIRE, L"Bardiel_HandGun_Blur_Effect", Engine::TEX_NORMAL, L"../Bin/Resource/Mesh/Dynamic/BardielBoss/m13603_ef.dds", 1)))
		return E_FAIL;
	//=====================


	m_isFinish = true;

	lstrcpy(m_szloadingString, L"Loading Complete...");

	return 0;
}

_uint CLoading::Loading_ForDungeonIce(void)
{

	//아이스맵

	Engine::Reset_Resource(RESOURCE_MESH_ICE);

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_0",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_0.X")))
	{
		MSG_BOX("Mesh Map Ice 0 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_1",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_1.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_2",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_2.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_3",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_3.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_4",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_4.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_5",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_5.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_6",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_6.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_7",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_7.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_8",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_8.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_9",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_9.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_10",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_10.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_11",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_11.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_12",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_12.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_13",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_13.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_14",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_14.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_15",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_15.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_16",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_16.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_17",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_17.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}


	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_18",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_18.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_19",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_19.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_20",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_20.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_21",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_21.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_22",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_22.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_23",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_23.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_24",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_24.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_25",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_25.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_26",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_26.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_27",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_27.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_28",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_28.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_29",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_29.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_30",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_30.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_31",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_31.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_32",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_32.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_33",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_33.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_34",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_34.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_35",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_35.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_36",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_36.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_37",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_37.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_38",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_38.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_39",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_39.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_40",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_40.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_41",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_41.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_42",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_42.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_43",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_43.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_44",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_44.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_45",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_45.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_46",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_46.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_47",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_47.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_48",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_48.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_49",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_49.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_50",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_50.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_51",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_51.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_52",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_52.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_53",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_53.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_54",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_54.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_55",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_55.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_56",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_56.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_57",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_57.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_58",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_58.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_59",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_59.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_60",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_60.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_61",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_61.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_62",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_62.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_63",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_63.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_64",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_64.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_65",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_65.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_66",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_66.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_67",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_67.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Map_Ice_68",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_68.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	//if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
	//	RESOURCE_STAGE,
	//	L"Mesh_Map_Ice_04",
	//	Engine::MESH_STATIC,
	//	L"../Bin/Resource/Mesh/Static/SoulValley/SoulValley_04/",
	//	L"SoulValley_04.X")))
	//{
	//	MSG_BOX("Mesh Map Ice 04 Load Failed");
	//	return E_FAIL;
	//}

	//if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
	//	RESOURCE_STAGE,
	//	L"Mesh_Map_Ice_05",
	//	Engine::MESH_STATIC,
	//	L"../Bin/Resource/Mesh/Static/SoulValley/SoulValley_05/",
	//	L"SoulValley_05.X")))
	//{
	//	MSG_BOX("Mesh Map Ice 01 Load Failed");
	//	return E_FAIL;
	//}

	//오브젝트..
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Torch1",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Torch1/",
		L"Torch1.X")))
	{
		MSG_BOX("Mesh Torch 01 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Arrow",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Arrow/",
		L"Arrow.X")))
	{
		MSG_BOX("Mesh Arrow Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Thrown_Arrow",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Thrown_Arrow/",
		L"Thrown_Arrow.X")))
	{
		MSG_BOX("Mesh Thrown_Arrow Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Cristal_1",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Cristal_1/",
		L"Cristal_1.X")))
	{
		MSG_BOX("Mesh_Cristal_1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Cristal_2",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Cristal_2/",
		L"Cristal_2.X")))
	{
		MSG_BOX("Mesh_Cristal_2 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Ice_Rock_1",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Ice_Rock_1/",
		L"Ice_Rock_1.X")))
	{
		MSG_BOX("Mesh_Cristal_2 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Lamp_Bone_A",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Lamp_Bone_A/",
		L"Lamp_Bone_A.X")))
	{
		MSG_BOX("Lamp_Bone_A Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Lamp_Bone_B",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Lamp_Bone_B/",
		L"Lamp_Bone_B.X")))
	{
		MSG_BOX("Lamp_Bone_A Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Lamp_Bone_C",
		Engine::MESH_STATIC,
		L"../Bin/Resource/Mesh/Static/Lamp_Bone_C/",
		L"Lamp_Bone_C.X")))
	{
		MSG_BOX("Lamp_Bone_A Load Failed");
		return E_FAIL;
	}


	//다이나믹 메쉬...

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Babegazi_Warrior",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/Monster/Babegazi_Warrior/",
		L"Babegazi_Warrior.X")))
	{
		MSG_BOX("Mesh Babegazi Warrior  Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Babegazi_Axe",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/Monster/Babegazi_Axe/",
		L"Babegazi_Axe.X")))
	{
		MSG_BOX("Mesh Babegazi Axe  Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Babegazi_Bow",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/Monster/Babegazi_Bow/",
		L"Babegazi_Bow.X")))
	{
		MSG_BOX("Mesh Babegazi Bow  Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Knole_Commander",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/",
		L"Knole_Commander.X")))
	{
		MSG_BOX("Mesh Knole_Commander Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Hanuman",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/HanumanBoss/",
		L"Hanuman.X")))
	{
		MSG_BOX("Mesh_Hanuman Load Failed");
		return E_FAIL;
	}

	//Hanuman Blur Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_MESH_ICE, L"HANUMAN_Blur_Effect", Engine::TEX_NORMAL, L"../Bin/Resource/Mesh/Dynamic/HanumanBoss/m10001_ef.dds", 1)))
		return E_FAIL;
	//=====================


	//함정..
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_SnowBall",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/Trap/SnowBall/",
		L"SnowBall.X")))
	{
		MSG_BOX("Mesh_SnowBall Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_IceTwister",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/Trap/IceTwister/",
		L"IceTwister.X")))
	{
		MSG_BOX("Mesh_IceTwister Load Failed");
		return E_FAIL;
	}
	
	//문
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Gate_A",
		Engine::MESH_DYNAMIC,
		L"../Bin/Resource/Mesh/Dynamic/Gate/",
		L"Gate_A.X")))
	{
		MSG_BOX("Gate_A Load Failed");
		return E_FAIL;
	}

	//네비메쉬
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_MESH_ICE,
		L"Mesh_Navigation"
		, Engine::MESH_NAVIGATION
		, L"../Bin/Data/Stage_Ice/"
		, L"Ice_Navy.dat")))
	{
		MSG_BOX("NaviMesh Loading Failed");
		return E_FAIL;
	}


	m_isFinish = true;

	lstrcpy(m_szloadingString, L"Loading Complete...");

	return 0;
}

unsigned int APIENTRY CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint		iFlag = 0;

	EnterCriticalSection(pLoading->Get_CriticalSection());

	switch(pLoading->Get_LoadingID())
	{
	case LOADING_LOGO:
		iFlag = pLoading->Loading_ForLogo();
		break;
	case LOADING_TOWN:
		iFlag = pLoading->Loading_ForTown();
		break;
	case LOADING_DUNGEON_FIRE:
		iFlag = pLoading->Loading_ForDungeonFire();
		break;
	case LOADING_DUNGEON_ICE:
		iFlag = pLoading->Loading_ForDungeonIce();
		break;
	}	

	LeaveCriticalSection(pLoading->Get_CriticalSection());

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if(FAILED(pInstance->Ready_Loading(eLoadingID)))
	{
		MSG_BOX("CLoading Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);	
	DeleteCriticalSection(&m_Crt);

	::Safe_Release(m_pGraphicDev);
}

