#include "stdafx.h"
#include "AnimationApp.h"

#include "Scene_AniTool.h"
#include "Export_Engine.h"

CAnimationApp::CAnimationApp(void)
: m_pGraphicInstance(NULL)
, m_pGraphicDev(NULL)
, m_pManagementInstance(NULL)
, m_dwRenderCnt(0)
, m_fTime(0.f)
{
	ZeroMemory(&m_szFPS, sizeof(_tchar) * 128);
}

CAnimationApp::~CAnimationApp(void)
{

}

HRESULT CAnimationApp::Get_GraphicDev(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if(ppGraphicDev == NULL)
		return E_FAIL;
	if(m_pGraphicDev == NULL)
		return E_FAIL;

	*ppGraphicDev = m_pGraphicDev;
	(*ppGraphicDev)->AddRef();

	return S_OK;
}

HRESULT CAnimationApp::Ready_AnimationApp(void)
{
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

void CAnimationApp::Update_AnimationApp(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;

	if(m_pManagementInstance == NULL)
		return;

	Engine::SetUp_InputState();

	m_pManagementInstance->Update_Scene(fTimeDelta);
	return;
}

void CAnimationApp::Render_AnimationApp(void)
{
	++m_dwRenderCnt;

	if(m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS:%d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	_vec4			vTmp = _vec4(_vec3(0, 0, 0), 1.f);

	m_pGraphicInstance->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	if(m_pManagementInstance == NULL)
		return;

	m_pManagementInstance->Render_Scene(m_pGraphicDev);

	Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(400.f, 0.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CAnimationApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
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
	if (FAILED(Engine::Ready_Font(m_pGraphicDev, L"../Bin/Resource/Font/SDSwaggerTTF.ttf", L"Font_Default", L"½º¿þ°Å TTF", 15, 20, FW_HEAVY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimationApp::Ready_ResourceForStatic(LPDIRECT3DDEVICE9 pGraphicDev, RESOURCEID eMax)
{
	Engine::Reserve_ContainerSize(eMax);

	/*if(FAILED(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcCol", Engine::BUFFER_RCCOL)))
		return E_FAIL;
	if(FAILED(Engine::Ready_Buffers(pGraphicDev, RESOURCE_STATIC, L"Buffer_RcTex", Engine::BUFFER_RCTEX)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CAnimationApp::Ready_OpeningScene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagementInstance)
{
	Engine::CScene*				pOpeningScene = NULL;

	if(FAILED(Engine::Create_Management_ForTool(pGraphicDev, ppManagementInstance)))
		return E_FAIL;

	Engine::SetManagerProjectID(Engine::PROJECT_ANIMATION);
	Engine::SetResourceProjectID(Engine::PROJECT_ANIMATION);

	(*ppManagementInstance)->AddRef();

	pOpeningScene = CScene_AniTool::Create(pGraphicDev);
	if(pOpeningScene == NULL)
		return E_FAIL;



	if(FAILED((*ppManagementInstance)->SetUp_CurrentScene(pOpeningScene)))
		return E_FAIL;

	return S_OK;
}

CAnimationApp* CAnimationApp::Create(void)
{
	CAnimationApp*			pInstance = new CAnimationApp;

	if(FAILED(pInstance->Ready_AnimationApp()))
	{
		MSG_BOX("CMainApp Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimationApp::Free(void)
{
	Engine::Safe_Release(m_pManagementInstance);
	Engine::Safe_Release(m_pGraphicInstance);

	Engine::Release_Utility();
	Engine::Release_Resource();
	Engine::Release_System();
}

