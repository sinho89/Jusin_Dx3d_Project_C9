#include "stdafx.h"
#include "LightCamera.h"
#include "Export_Engine.h"

CLightCamera::CLightCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: Engine::CCamera(pGraphicDev, wstrName)
{
}

CLightCamera::~CLightCamera(void)
{
}

HRESULT CLightCamera::InitCamera(const D3DXVECTOR3 * pEye, const D3DXVECTOR3 * pAt)
{
	m_vEye = *pEye;
	m_vAt = *pAt;

	m_fFovy = D3DXToRadian(60.0f);
	m_fAspect = _float(WINSIZEX) / _float(WINSIZEY);
	m_fNear = 3.0f;
	m_fFar = 150.0f;

	// For.View Matrix
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	// For.Projection Matrix
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovy, m_fAspect, m_fNear, m_fFar);
	//D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), m_fNear, m_fFar);

	return S_OK;
}

void CLightCamera::Update_Object(const _float & fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
}

CLightCamera * CLightCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, const D3DXVECTOR3 * pEye, const D3DXVECTOR3 * pAt)
{
	CLightCamera*	pCamera = new CLightCamera(pGraphicDev, wstrName);

	if (FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CLightCamera::Free(void)
{
	Engine::CCamera::Free();
}
