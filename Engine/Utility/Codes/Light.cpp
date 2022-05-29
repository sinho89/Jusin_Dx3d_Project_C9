#include "Light.h"
#include "Screen_Texture.h"
#include "CameraMgr.h"
#include "Camera.h"
#include "Renderer.h"
USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_iIndex(0)
, m_bLightRender(TRUE)
{
	ZeroMemory(&m_LightInfo, sizeof(D3DLIGHT9));

	m_pGraphicDev->AddRef();

	m_pRenderer = CRenderer::GetInstance();
}

Engine::CLight::~CLight(void)
{

}

void CLight::Set_LightPos(const _vec3 * pPos)
{
	if (pPos == NULL)
		return;
	m_LightInfo.Position = *pPos;
}

void CLight::Set_LightDiffuse(const D3DXCOLOR & tDiffuse)
{
	m_LightInfo.Diffuse = tDiffuse;
}

void CLight::Set_LightRange(const _float & fRange)
{
	m_LightInfo.Range = fRange;
}

D3DLIGHT9* Engine::CLight::Get_LightInfo()
{
	return &m_LightInfo;
}

const _matrix CLight::Get_matLightView(void) const
{
	return m_matLightView;
}

void CLight::Set_matLightView(const _matrix matView)
{
	m_matLightView = matView;
}

const _matrix CLight::Get_matLightProj(void) const
{
	return m_matLightProj;
}

void CLight::Set_matLightProj(const _matrix matProj)
{
	m_matLightProj = matProj;
}

//void CLight::Render(LPD3DXEFFECT pEffect)
//{
//	_uint iPassIdx = 0;
//
//	if (D3DLIGHT_DIRECTIONAL == m_LightInfo.Type)
//	{
//		if (m_pRenderer->GetShadowBuffer())
//			iPassIdx = 0;	//그림자를 그린다.
//		else
//			iPassIdx = 1;	//그림자를 그리지 않는다.
//
//		pEffect->SetVector("g_vLightDir", &_vec4(m_LightInfo.Direction, 0.0f));		
//	}
//	else if (D3DLIGHT_POINT == m_LightInfo.Type)
//	{
//		iPassIdx = 2;
//		pEffect->SetVector("g_vLightPos", &_vec4(m_LightInfo.Position, 0.0f));
//		pEffect->SetFloat("g_fRange", m_LightInfo.Range);
//	}
//	else if (D3DLIGHT_SPOT == m_LightInfo.Type)
//	{
//		iPassIdx = 3;
//	}
//
//	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_LightInfo.Diffuse);
//	pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_LightInfo.Ambient);
//
//	pEffect->SetVector("g_vMtrlDiffuse", &_vec4(1.0f, 1.0f, 1.0f, 1.0f));
//	pEffect->SetVector("g_vMtrlAmbient", &_vec4(1.0f, 1.0f, 1.0f, 1.0f));
//
//	_matrix		matView, matProj;
//	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
//	D3DXMatrixInverse(&matView, NULL, &matView);
//
//	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
//	D3DXMatrixInverse(&matProj, NULL, &matProj);
//
//	pEffect->SetMatrix("g_matViewInv", &matView);
//	pEffect->SetMatrix("g_matProjInv", &matProj);	
//
//	pEffect->SetMatrix("g_matLightView", &m_matLightView);
//	pEffect->SetMatrix("g_matLightProj", &m_matLightProj);
//
//	pEffect->BeginPass(iPassIdx);
//
//	m_pScreenTexture->Render_Buffer();
//
//	pEffect->EndPass();
//}

HRESULT Engine::CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	memcpy(&m_LightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	m_pGraphicDev->SetLight(iIndex, pLightInfo);
	m_pGraphicDev->LightEnable(iIndex, TRUE);

	/*D3DVIEWPORT9			ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	m_pScreenTexture = CScreen_Texture::Create(m_pGraphicDev, 0, 0, ViewPort.Width, ViewPort.Height);*/

	return S_OK;
}

CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight* pInstance = new CLight(pGraphicDev);

	if(FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
	{
		MSG_BOX("CLight Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CLight::Free(void)
{
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);
	//Engine::Safe_Release(m_pScreenTexture);
	Engine::Safe_Release(m_pGraphicDev);	
}

