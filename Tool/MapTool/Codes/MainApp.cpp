#include "stdafx.h"
#include "MainApp.h"
#include "Scene_MapTool.h"
#include "Export_Engine.h"
#include "Mouse.h"

//콘솔창쓸때
#define _DEBUG__

#ifdef _DEBUG__
//#include "vld.h"
#endif // _DEBUG__


//전역변수

int g_iMouseX;
int g_iMouseY;
int g_iSkyDrawID;
_vec3 g_vMouse;
_vec3 g_vViewMouse;
_vec3 g_vCamEye;



CMainApp::CMainApp( void )
: m_pGraphicInstance(NULL)
, m_pGraphicDev(NULL)
, m_pManagementInstance(NULL)
, m_pMouseCol(NULL)
, m_dwRenderCnt(0)
, m_fTime(0.f)
{
	m_vMousePos = D3DXVECTOR3(0.f, 0.f, 0.f);
	ZeroMemory(&m_szFPS, sizeof(_tchar) * 128);
	ZeroMemory(&m_szCamEye, sizeof(_tchar) * 128);
}

CMainApp::~CMainApp( void )
{

}

HRESULT CMainApp::Get_GraphicDev(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if(ppGraphicDev == NULL)
		return E_FAIL;
	if(m_pGraphicDev == NULL)
		return E_FAIL;

	*ppGraphicDev = m_pGraphicDev;
	(*ppGraphicDev)->AddRef();

	return S_OK;
}

HRESULT CMainApp::Ready_MainApp( void )
{
#ifdef _DEBUG__
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	AllocConsole(); 
	freopen( "CONOUT$",  "wt", stdout);
	SetConsoleTitleA("Debug");
#endif

	srand(unsigned(time(NULL)));

	if(FAILED(SetUp_DefaultSetting(&m_pGraphicDev)))
		return E_FAIL;

	if(FAILED(Ready_ResourceForStatic(m_pGraphicDev, RESOURCE_END)))
		return E_FAIL;

	if(FAILED(Ready_OpeningScene(m_pGraphicDev, &m_pManagementInstance)))
		return E_FAIL;

	Engine::Safe_Release(m_pGraphicDev);


	return S_OK;
}


void CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	if(m_pManagementInstance == NULL)
		return;

	Engine::SetUp_InputState();

	m_pManagementInstance->Update_Scene(fTimeDelta);

	return;
}


void CMainApp::Render_MainApp(void)
{	
	++m_dwRenderCnt;

	if(m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS:%d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	wsprintf(m_szCamEye, L"CamEye : X %d Y %d Z %d", int(g_vCamEye.x), int(g_vCamEye.y), int(g_vCamEye.z));

	_vec4			vTmp = _vec4(_vec3(0, 0, 0), 1.f);

	m_pGraphicInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	if(m_pManagementInstance == NULL)
		return;

	m_pManagementInstance->Render_Scene(m_pGraphicDev);

	Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(400.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

	Engine::Render_Font(L"Font_Default", m_szCamEye,  &_vec2(10.f, 100.f), D3DXCOLOR(1.f, 0.f, 1.f, 1.f));

	m_pGraphicInstance->Render_End();

}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if(FAILED(Engine::Ready_GraphicDev(g_hWnd, Engine::CGraphicDev::MODE_WIN, WINSIZEX, WINSIZEY, &m_pGraphicInstance)))
		return E_FAIL;

	m_pGraphicInstance->AddRef();

	m_pGraphicInstance->Set_RenderState(D3DRS_LIGHTING, TRUE);	

	*ppGraphicDev = m_pGraphicInstance->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();	

	if(FAILED(Engine::Ready_InputDevice(g_hInst, g_hWnd)))
		return E_FAIL;

	// For.Font
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/SDSwaggerTTF.ttf", L"Font_Default", L"스웨거 TTF", 15, 20, FW_HEAVY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax)
{
	Engine::Reserve_ContainerSize(eMax);

	/*if(FAILED(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcCol", Engine::BUFFER_RCCOL)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CMainApp::Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInstance)
{
	Engine::CScene*				pOpeningScene = NULL;

	if(FAILED(Engine::Create_Management_ForTool(pGraphicDev, ppManagementInstance)))
		return E_FAIL;

	Engine::SetManagerProjectID(Engine::PROJECT_MAP);
	Engine::SetResourceProjectID(Engine::PROJECT_MAP);

	(*ppManagementInstance)->AddRef();

	pOpeningScene = CScene_MapTool::Create(pGraphicDev);
	if(pOpeningScene == NULL)
		return E_FAIL;



	if(FAILED((*ppManagementInstance)->SetUp_CurrentScene(pOpeningScene)))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create( void )
{
	CMainApp* pMainApp = new CMainApp;

	if(FAILED(pMainApp->Ready_MainApp()))
	{
		MSG_BOX("CMainApp Created Failed");
		Engine::Safe_Release(pMainApp);
	}

	return pMainApp;
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

void CMainApp::Free( void )
{
	Engine::Safe_Release(m_pManagementInstance);
	Engine::Safe_Release(m_pGraphicInstance);

	Engine::Release_Utility();
	Engine::Release_Resource();
	Engine::Release_System();

	#ifdef _DEBUG__
	 	FreeConsole();
	#endif
}
