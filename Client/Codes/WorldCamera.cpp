#include "stdafx.h"
#include "WorldCamera.h"
#include "Export_Engine.h"

CWorldCamera::CWorldCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CCamera(pGraphicDev, wstrName)
, m_bClick(FALSE)
, m_bMouseFix(FALSE)
{

}

CWorldCamera::~CWorldCamera(void)
{

}

HRESULT CWorldCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCamSpeed	=	50.0f;

	m_vEye		=	*pEye;
	m_vAt		=	*pAt;

	m_fFovy		=	D3DXToRadian(60.0f);
	m_fAspect	=	_float(WINSIZEX) / _float(WINSIZEY);
	m_fNear		=	0.1f;
	m_fFar		=	1000.0f;
	
	Engine::CCamera::Ready_Object();

	return S_OK;
}

void CWorldCamera::Update_Object(const _float& fTimeDelta)
{	
	KeyCheck(fTimeDelta);

	if(m_bMouseFix)
	{
		FixMouse();
		MouseMove();
	}

	Engine::CCamera::Update_Object(fTimeDelta);		
}

void CWorldCamera::KeyCheck(const _float& fTimeDelta)
{
	if(Engine::Get_DIKeyState(DIK_NUMPAD4)		& 0x80)
	{
		_vec3	vRight;
		_matrix	matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fCamSpeed * fTimeDelta;
		m_vAt  -= vRight * m_fCamSpeed * fTimeDelta;
	}

	if(Engine::Get_DIKeyState(DIK_NUMPAD6)	& 0x80)
	{
		_vec3	vRight;
		_matrix	matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fCamSpeed * fTimeDelta;
		m_vAt  += vRight * m_fCamSpeed * fTimeDelta;
	}

	if(Engine::Get_DIKeyState(DIK_NUMPAD8)		& 0x80)
	{
		_vec3	vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye	+=	vLook * m_fCamSpeed * fTimeDelta;
		m_vAt	+=	vLook * m_fCamSpeed * fTimeDelta;
	}

	if(Engine::Get_DIKeyState(DIK_NUMPAD5)		& 0x80)
	{
		_vec3	vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye	-=	vLook * m_fCamSpeed * fTimeDelta;
		m_vAt	-=-	vLook * m_fCamSpeed * fTimeDelta;
	}

	if(Engine::Get_DIKeyState(DIK_NUMPAD7)		& 0x80)
	{
		if(m_bClick)
			return;

		m_bClick = TRUE;

		if(m_bMouseFix)	m_bMouseFix = FALSE;
		else			m_bMouseFix = TRUE;
	}
	else
		m_bClick = FALSE;

	if(m_bMouseFix == FALSE)
		return;
}

CWorldCamera* CWorldCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CWorldCamera*	pCamera = new CWorldCamera(pGraphicDev, wstrName);

	if(FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CWorldCamera::FixMouse(void)
{
	POINT	ptMouse = { WINSIZEX >> 1, WINSIZEY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CWorldCamera::MouseMove(void)
{
	_int iDistance = 0;

	if(iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
	{
		_matrix	matAxis;
		D3DXMatrixRotationAxis(&matAxis, &_vec3(0.0f, 1.0f, 0.0f), D3DXToRadian(iDistance / 30.0f));

		_vec3 vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
	{
		_vec3	vRight;
		_matrix	matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);

		_matrix matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance / 30.0f));

		_vec3	vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}
}

void CWorldCamera::Free(void)
{
	Engine::CCamera::Free();
}

