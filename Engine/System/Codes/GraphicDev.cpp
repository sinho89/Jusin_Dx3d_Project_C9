#include "GraphicDev.h"
USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDev)

Engine::CGraphicDev::CGraphicDev(void)
: m_pSDK(NULL)
, m_pGraphicDev(NULL)
{

}

Engine::CGraphicDev::~CGraphicDev(void)
{

}

LPDIRECT3DDEVICE9 Engine::CGraphicDev::Get_GraphicDev(void) const
{	
	return m_pGraphicDev;
}

HRESULT Engine::CGraphicDev::Get_GraphicDev(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if(m_pGraphicDev == NULL)
		return E_FAIL;

	*ppGraphicDev = m_pGraphicDev;
	m_pGraphicDev->AddRef();

	return S_OK;
}

void Engine::CGraphicDev::Set_RenderState(D3DRENDERSTATETYPE eType, _ulong dwFlag)
{
	m_pGraphicDev->SetRenderState(eType, dwFlag);
}

HRESULT Engine::CGraphicDev::Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, Engine::CGraphicDev** ppGraphicDev)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pSDK == NULL)
		return E_FAIL;

	D3DCAPS9			DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// 장치의 성능정보를 DeviceCaps에 얻어온다.
	if(FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
		return E_FAIL;

	// 얻어온 정보를 바탕으로 특정기능을 지원하는지 조사한ㄷ.ㅏ
	_ulong			BehaviorFlag = 0;
	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		BehaviorFlag  |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		BehaviorFlag  |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	BehaviorFlag |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS			Present_Parameters;
	ZeroMemory(&Present_Parameters, sizeof(D3DPRESENT_PARAMETERS));

	Present_Parameters.BackBufferWidth = iSizeX;
	Present_Parameters.BackBufferHeight = iSizeY;
	Present_Parameters.BackBufferFormat = D3DFMT_A8R8G8B8;
	Present_Parameters.BackBufferCount = 1;

	Present_Parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	Present_Parameters.MultiSampleQuality = 0;

	Present_Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Present_Parameters.hDeviceWindow = hWnd;

	Present_Parameters.Windowed = eMode;
	Present_Parameters.EnableAutoDepthStencil = TRUE;
	Present_Parameters.AutoDepthStencilFormat = D3DFMT_D24S8;

	Present_Parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	Present_Parameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if(FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, BehaviorFlag, &Present_Parameters, &m_pGraphicDev)))
		return E_FAIL;

	*ppGraphicDev = this;

	return S_OK;
}

void Engine::CGraphicDev::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, Color, 1.f, 0);
	m_pGraphicDev->BeginScene();	
}

void Engine::CGraphicDev::Render_End(void)
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);
}

void Engine::CGraphicDev::Free(void)
{
	_ulong		dwRefCnt = 0;

	if(dwRefCnt = Engine::Safe_Release(m_pGraphicDev))
	{
		for(_ulong i=0; i<dwRefCnt; ++i)
			Engine::Safe_Release(m_pGraphicDev);
	}

	if(dwRefCnt = Engine::Safe_Release(m_pSDK))
	{
		for(_ulong i=0; i<dwRefCnt; ++i)
			Engine::Safe_Release(m_pSDK);
	}

	if(dwRefCnt = Engine::Safe_Release(m_pGraphicDev))
		MSG_BOX("m_pGraphicDev Release Failed");

	if(dwRefCnt = Engine::Safe_Release(m_pSDK))
		MSG_BOX("m_pSDK Release Failed");
}