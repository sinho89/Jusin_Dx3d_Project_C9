#include "stdafx.h"
#include "UICamera.h"
#include "Export_Engine.h"

CUICamera::CUICamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CCamera(pGraphicDev, wstrName)
{

}

CUICamera::~CUICamera(void)
{

}

HRESULT CUICamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_vEye		=	*pEye;
	m_vAt		=	*pAt;

	m_fFovy		=	D3DXToRadian(60.0f);
	m_fAspect	=	_float(WINSIZEX) / _float(WINSIZEY);
	m_fNear		=	0.1f;
	m_fFar		=	100.0f;

	Engine::CCamera::Ready_Object();

	return S_OK;
}

void CUICamera::Update_Object(const _float& fTimeDelta)
{
	
}


CUICamera* CUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CUICamera*	pCamera = new CUICamera(pGraphicDev, wstrName);

	if(FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CUICamera::Free(void)
{
	Engine::CCamera::Free();
}

