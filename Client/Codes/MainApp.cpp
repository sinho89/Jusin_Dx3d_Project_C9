#include "stdafx.h"
#include "MainApp.h"
#include "Export_Engine.h"
#include "Scene_Logo.h"
#include "Scene_Town.h"
#include "Scene_LoadingForTown.h"
#include "Scene_Stage_Ice.h"
#include "Scene_LoadingForIce.h"
#include "AstarManager.h"
#include "Culling_Manager.h"
#include "NumberingMgr.h"
#include "Fade_Manager.h"
#include "Sound_Manager.h"

CMainApp::CMainApp(void)
: m_pGraphicInstance(NULL)
, m_pGraphicDev(NULL)
, m_pManagementInstance(NULL)
, m_dwRenderCnt(0)
, m_fTime(0.f)
, m_bOneClick(FALSE)

{
	ZeroMemory(&m_szFPS, sizeof(_tchar) * 128);
	//AllocConsole();
	//freopen("CONOUT$", "wt", stdout);
	//SetConsoleTitleA("Debug");
}

CMainApp::~CMainApp(void)
{
	//FreeConsole();
}


HRESULT CMainApp::Ready_MainApp(void)
{
	srand(unsigned(time(NULL)));

	if(FAILED(SetUp_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;

	if(FAILED(Ready_ResourceForStatic(m_pGraphicDev, RESOURCE_END)))
		return E_FAIL;

	if(FAILED(Ready_OpeningScene(m_pGraphicDev, &m_pManagementInstance)))
		return E_FAIL;

	Engine::Safe_Release(m_pGraphicDev);

	//쉐이더 옵션들의 초기화.
	g_ShaderOption_Info.bCACDShadow = FALSE;
	g_ShaderOption_Info.bDebugBuffer = FALSE;
	g_ShaderOption_Info.bDOF = FALSE;
	g_ShaderOption_Info.bExpFog = FALSE;
	g_ShaderOption_Info.bGaussianBlur = FALSE;
	g_ShaderOption_Info.bHDR = FALSE;
	g_ShaderOption_Info.bHeightFog = FALSE;
	g_ShaderOption_Info.bLimLight = FALSE;
	g_ShaderOption_Info.bMotionBlurBuffer = FALSE;
	g_ShaderOption_Info.bShadowBuffer = FALSE;
	g_ShaderOption_Info.bSoftShadow = FALSE;
	g_ShaderOption_Info.bSquareFog = FALSE;
	g_ShaderOption_Info.bSSAO = FALSE;
	g_ShaderOption_Info.fBlurValue = 0.0f;
	g_ShaderOption_Info.fBrightness = 0.0f;

	return S_OK;
}

void CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	if(m_pManagementInstance == NULL)
		return;

	ShowCursor(FALSE);
	Engine::SetUp_InputState();

	m_pManagementInstance->Update_Scene(fTimeDelta);

	// 인풋 장치 소실 잡아 주는 함수.
	Set_Focus();

	//DebugRender
	if (Engine::Get_DIKeyState(DIK_NUMPADMINUS) & 0x80)
	{
		if (TRUE == m_bOneClick)
			return;
		m_bOneClick = TRUE;

		g_bDebugRender = !g_bDebugRender;
	}	
	//Shadow On / Off
	else if (Engine::Get_DIKeyState(DIK_NUMPADSTAR) & 0x80)
	{
		if (TRUE == m_bOneClick)
			return;
		m_bOneClick = TRUE;

		_bool bBuf = Engine::GetShadowBuffer();
		Engine::SetShadowBuffer(!bBuf);
	}
	//MotionBlur On / Off
	else if (Engine::Get_DIKeyState(DIK_F9) & 0x80)
	{
		if (TRUE == m_bOneClick)
			return;
		m_bOneClick = TRUE;

		_bool bBuf = Engine::GetMotionBlurBuffer();
		Engine::SetMotionBlurBuffer(!bBuf);
	}
	//Radial Blur On/Off
	else if (Engine::Get_DIKeyState(DIK_NUMPAD7) & 0x80)
	{
		if (TRUE == m_bOneClick)
			return;
		m_bOneClick = TRUE;

		_bool bBuf = Engine::GetRadialBlur();
		Engine::SetRadialBlur(!bBuf);
	}
	//else if (Engine::Get_DIKeyState(DIK_F5) & 0x80)	//휘도 평균값 ++
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;

	//	Engine::LuminanceAverageVariation(-0.001f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_F6) & 0x80)	//휘도 평균값 --
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;

	//	Engine::LuminanceAverageVariation(0.001f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_Y) & 0x80) // A ++
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::HDR_A_Variation(0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_H) & 0x80) // A --
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::HDR_A_Variation(-0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_U) & 0x80) // B ++
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::HDR_B_Variation(0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_J) & 0x80) // B --
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::HDR_B_Variation(-0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_M) & 0x80) // C ++
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::HDR_C_Variation(0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_N) & 0x80) // C --
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::HDR_C_Variation(-0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_O) & 0x80) // D ++
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::HDR_D_Variation(0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_L) & 0x80) // D --
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::HDR_D_Variation(-0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_M) & 0x80) // Focus ++
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::DOF_Focus_Variation(0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_N) & 0x80) // Focus --
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::DOF_Focus_Variation(-0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_B) & 0x80) // Range ++
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::DOF_Range_Variation(0.01f);
	//}
	//else if (Engine::Get_DIKeyState(DIK_V) & 0x80) // Range --
	//{
	//	if (TRUE == m_bOneClick)
	//		return;
	//	m_bOneClick = TRUE;
	//	Engine::DOF_Range_Variation(-0.01f);
	//}
	else
		m_bOneClick = FALSE;

}

void CMainApp::Render_MainApp(void)
{	
	++m_dwRenderCnt;
	if(m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS:%d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
		Engine::Clear_ColliderGroup();
	}

	_vec4			vTmp = _vec4(_vec3(0, 0, 0), 1.f);

	m_pGraphicInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	if(m_pManagementInstance == NULL)
		return;

	m_pManagementInstance->Render_Scene(m_pGraphicDev);

	//Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(50, 225), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	/*TCHAR	szMouse[128];

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	wsprintf(szMouse, L"Mouse X:%d", ptMouse.x);
	Engine::Render_Font(L"Font_Default", szMouse, &_vec2(50, 325), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	wsprintf(szMouse, L"Mouse Y:%d", ptMouse.y);
	Engine::Render_Font(L"Font_Default", szMouse, &_vec2(50, 350), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));*/

	m_pGraphicInstance->Render_End();

}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if(FAILED(Engine::Ready_GraphicDev(g_hWnd, Engine::CGraphicDev::MODE_FULL, WINSIZEX, WINSIZEY, &m_pGraphicInstance)))
		return E_FAIL;

	m_pGraphicInstance->AddRef();

	m_pGraphicInstance->Set_RenderState(D3DRS_LIGHTING, TRUE);	

	*ppGraphicDev = m_pGraphicInstance->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();	

	if(FAILED(Engine::Ready_InputDevice(g_hInst, g_hWnd)))
		return E_FAIL;

	// For.StaticFont
	if(FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Default", L"NanumGothic", 10, 27, FW_NORMAL)))
		return E_FAIL;
	// For.LoginFont (ID & PassWord)
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Login", L"NanumGothic", 6, 15, FW_NORMAL)))
		return E_FAIL;
	// For.SelectFont (Slot)
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Select_Slot", L"NanumGothic", 10, 27, FW_MEDIUM)))
		return E_FAIL;
	// For.SelectFont (State)
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Select_Level", L"NanumGothic", 10, 26, FW_MEDIUM)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Select_Char", L"NanumGothic", 6, 18, FW_MEDIUM)))
		return E_FAIL;
	// For.CreateFont (State)
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Create_Char", L"NanumGothic", 6, 18, FW_MEDIUM)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Create_State", L"NanumGothic", 6, 18, FW_MEDIUM)))
		return E_FAIL;
	// For.StageFont (MainSlot)
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Main_Slot", L"NanumGothic", 5, 16, FW_MEDIUM)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Main_Data", L"NanumGothic", 5, 16, FW_MEDIUM)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Main_Button", L"NanumGothic", 6, 18, FW_MEDIUM)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Info_Data", L"NanumGothic", 5, 16, FW_MEDIUM)))
		return E_FAIL;
	// For.StageFont (Enter)
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Start_DunGeun", L"NanumGothic", 10, 26, FW_MEDIUM)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Select_DunGeun", L"NanumGothic", 8, 20, FW_MEDIUM)))
		return E_FAIL;

	// Result UI

	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/NanumGothic.ttf", L"Font_Result_Item", L"NanumGothic", 6, 18, FW_MEDIUM)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax)
{
	Engine::Reserve_ContainerSize(eMax);

	// Mouse Sprite

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"CursorEff", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Cursor/CursorEff.dds", 1)))
		return E_FAIL;

	// Ready_Buffer
	if (FAILED(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_CubeTex", Engine::BUFFER_CUBETEX)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_TriTex", Engine::BUFFER_TRITEX)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_Pentagon", Engine::BUFFER_PENTAGON)))
		return E_FAIL;


	if(FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_Trail", Engine::BUFFER_TRAILTEX, 10, 2, 1)))
	{
		MSG_BOX("Trail Buffer Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STATIC, L"Buffer_Trail2", Engine::BUFFER_TRAILTEX, 10, 2, 1)))
	{
		MSG_BOX("Trail Buffer2 Load Failed");
		return E_FAIL;
	}

	// Ready_Texture

	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Collider_Blue", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/Collider/Blue.png", 1)))
		return E_FAIL;

	// Texture for Opening Scene
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Opening_Banner", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Logo/UI_CI.png", 1)))
		return E_FAIL;

	// Texture for Loading Scene
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Cursor", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Cursor/Cursor.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Town_Loading_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/LoadingScene/Town_Loading_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Ice_Loading_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/LoadingScene/Ice_Loading_Base.png", 1)))
		return E_FAIL;
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Fire_Loading_Base", Engine::TEX_NORMAL, L"../Bin/Resource/UI/LoadingScene/Fire_Loading_Base.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Loading_Bar", Engine::TEX_NORMAL, L"../Bin/Resource/UI/LoadingScene/Loading_Bar.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Loading_Char", Engine::TEX_NORMAL, L"../Bin/Resource/UI/LoadingScene/Loading_Char.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Loading_Effect", Engine::TEX_NORMAL, L"../Bin/Resource/UI/LoadingScene/Loading_Effect_Base.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Loading_Gage", Engine::TEX_NORMAL, L"../Bin/Resource/UI/LoadingScene/Loading_Gage.png", 1)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Loading_Effect_Gage", Engine::TEX_NORMAL, L"../Bin/Resource/UI/LoadingScene/Loading_Effect_Gage.png", 1)))
		return E_FAIL;


	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Fade", Engine::TEX_NORMAL, L"../Bin/Resource/UI/Fade/Fade.png", 1)))
		return E_FAIL;

	//======이펙트용 Texture=================================//

	//Particle Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Particle", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/Particle/Particle%d.png", 20)))
	{
		MSG_BOX("Texture Particle Loading Failed");
		return E_FAIL;
	}

	//Sprite Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Sprite", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/Sprite/Sprite%d.dds", 100)))
	{
		MSG_BOX("Texture Sprite Loading Failed");
		return E_FAIL;
	}

	//Trail Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Trail", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/Trail/Trail%d.dds", 32)))
	{
		MSG_BOX("Texture Trail Loading Failed");
		return E_FAIL;
	}

	//Wave Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Wave", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/Wave/Wave%d.dds", 72)))
	{
		MSG_BOX("Texture Wave Loading Failed");
		return E_FAIL;
	}


	//Effect Normal
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_EffectNormal", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/EffectNormal/EffectNormal%d.dds", 3)))
	{
		MSG_BOX("Texture Normal Loading Failed");
		return E_FAIL;
	}

	//Effect Sword
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Sword", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/Sword/Sword%d.dds", 2)))
	{
		MSG_BOX("Texture Normal Loading Failed");
		return E_FAIL;
	}

	//UI Effect
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_UI", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/UIEffect/UIEffect%d.dds", 55)))
	{
		MSG_BOX("Texture UI Loading Failed");
		return E_FAIL;
	}

	//Decal Effect
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Decal", Engine::TEX_NORMAL, L"../Bin/Resource/Effect/Decal/Decal_%d.dds", 4)))
	{
		MSG_BOX("Texture Decal Loading Failed");
		return E_FAIL;
	}


	return S_OK;
}

HRESULT CMainApp::Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInstance)
{
	Engine::CScene*				pOpeningScene = NULL;

	if(FAILED(Engine::Create_Management_ForClient(pGraphicDev, ppManagementInstance)))
		return E_FAIL;

	//Utility에서 현재 프로젝트가 무엇인지 판단하기 위한 것.
	Engine::SetManagerProjectID(Engine::PROJECT_CLIENT);
	//Resource에서 현재 프로젝트가 무엇인지 판단하기 위한 것.
	Engine::SetResourceProjectID(Engine::PROJECT_CLIENT);
	
	(*ppManagementInstance)->AddRef();

	//씬바꾸는곳..
	pOpeningScene = CScene_Logo::Create(pGraphicDev);
	//pOpeningScene = CScene_LoadingForTown::Create(pGraphicDev);
	//((CScene_LoadingForTown*)pOpeningScene)->SaveSlotIndex(2);
	//pOpeningScene = CScene_LoadingForIce::Create(pGraphicDev);
	//((CScene_LoadingForIce*)pOpeningScene)->SaveSlotIndex(2);
	if(pOpeningScene == NULL)
		return E_FAIL;

	if(FAILED((*ppManagementInstance)->SetUp_CurrentScene(pOpeningScene)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Set_Focus(void)
{
	if(g_bFocus)
	{
		if(g_bSetAquire)
		{
			Engine::Set_Acquire();
			g_bSetAquire = false;
		}
	}
}
CMainApp* CMainApp::Create(void)
{
	CMainApp*			pInstance = new CMainApp;

	if(FAILED(pInstance->Ready_MainApp()))
	{
		MSG_BOX("CMainApp Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free(void)
{	
	CCulling_Manager::GetInstance()->StopThread();
	Engine::Safe_Release(m_pManagementInstance);
	Engine::Safe_Release(m_pGraphicInstance);
	Engine::Release_Utility();
	Engine::Release_Resource();
	Engine::Release_System();	
	CCulling_Manager::GetInstance()->DestroyInstance();
	CNumbering_Manager::GetInstance()->DestroyInstance();
	CFade_Manager::GetInstance()->DestroyInstance();
	CSound_Manager::GetInstance()->DestroyInstance();
}


