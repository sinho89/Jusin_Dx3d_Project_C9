#include "Target.h"
#include "Screen_Texture.h"

USING(Engine)

Engine::CTarget::CTarget(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pTargetTexture(NULL)
, m_pTargetSurface(NULL)
, m_pOldSurface(NULL)
, m_pScreenTexture(NULL)
{
	m_pGraphicDev->AddRef();
}

Engine::CTarget::~CTarget(void)
{
}

HRESULT Engine::CTarget::Ready_Target(const _uint & iSizeX, const _uint & iSizeY, D3DFORMAT Format, D3DXCOLOR Color, RENDERTARGETID eRTID)
{
	m_Color = Color;
	m_eRenderTargetID = eRTID;

	//Create Render Target
	if (FAILED(D3DXCreateTexture(m_pGraphicDev, iSizeX, iSizeY, 1,
								D3DUSAGE_RENDERTARGET,
								Format,
								D3DPOOL_DEFAULT,
								&m_pTargetTexture)))
								return E_FAIL;

	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);
	Engine::Safe_Release(m_pTargetTexture);

	return S_OK;
}

HRESULT Engine::CTarget::Ready_DebugBuffer(const _uint & iX, const _uint & iY, const _uint & iSizeX, const _uint & iSizeY)
{
	m_pScreenTexture = CScreen_Texture::Create(m_pGraphicDev, iX, iY, iSizeX, iSizeY);
	if (NULL == m_pScreenTexture)
	{
		MSG_BOX("Target Screen Texture Create Failed!");
		return E_FAIL;
	}

	return S_OK;
}

void Engine::CTarget::Render_DubugBuffer(void)
{
	if (NULL == m_pScreenTexture)
		return;

	m_pGraphicDev->SetTexture(0, m_pTargetTexture);
	m_pScreenTexture->Render_Buffer();
}

void Engine::CTarget::Clear_RenderTarget(void)
{
	m_pGraphicDev->GetRenderTarget(0, &m_pOldSurface);
	m_pGraphicDev->SetRenderTarget(0, m_pTargetSurface);

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_Color, 1.0f, 0);

	m_pGraphicDev->SetRenderTarget(0, m_pOldSurface);
	Engine::Safe_Release(m_pOldSurface);
}

void Engine::CTarget::SetUp_OnGraphicDev(const _ulong & dwIndex)
{
	m_pGraphicDev->GetRenderTarget(dwIndex, &m_pOldSurface);
	m_pGraphicDev->SetRenderTarget(dwIndex, m_pTargetSurface);
}

void Engine::CTarget::Release_OnGraphicDev(const _ulong & dwIndex)
{
	m_pGraphicDev->SetRenderTarget(dwIndex, m_pOldSurface);
	Engine::Safe_Release(m_pOldSurface);
}

HRESULT Engine::CTarget::SetUp_OnShader(LPD3DXEFFECT pEffect, const char * pConstantName)
{
	if (NULL == pEffect) return E_FAIL;

	pEffect->SetTexture(pConstantName, m_pTargetTexture);

	/*if(!strcmp("g_ShadowTexture", pConstantName))
	{
	D3DXSaveTextureToFile(L"../../SaveTargetTexture.bmp", D3DXIFF_BMP, m_pTargetTexture, NULL);
	}*/

	return S_OK;
}

Engine::CTarget * Engine::CTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint & iSizeX, const _uint & iSizeY, D3DFORMAT Format, D3DXCOLOR Color, RENDERTARGETID eRTID)
{
	CTarget*	pInstance = new CTarget(pGraphicDev);

	if (FAILED(pInstance->Ready_Target(iSizeX, iSizeY, Format, Color, eRTID)))
	{
		MSG_BOX("CTarget Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CTarget::Free(void)
{
	Engine::Safe_Release(m_pScreenTexture);
	Engine::Safe_Release(m_pOldSurface);

	Engine::Safe_Release(m_pTargetTexture);
	Engine::Safe_Release(m_pGraphicDev);
}
