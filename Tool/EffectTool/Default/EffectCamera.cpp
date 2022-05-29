#include "stdafx.h"
#include "EffectCamera.h"
#include "Export_Engine.h"


CEffectCamera::CEffectCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CCamera(pGraphicDev, wstrName)
, m_fCameraSpeed(0.0f)
{

}

CEffectCamera::~CEffectCamera(void)
{

}

void CEffectCamera::Update_Object(const _float& fTimeDelta)
{
	KeyCheck(fTimeDelta);
	MouseMove();
	Engine::CCamera::Update_Object(fTimeDelta);
}

HRESULT CEffectCamera::Ready_Camera(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, _float fFovy, _float fAspect, _float fNear, _float fFar)
{
	m_fCameraSpeed = 100.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovy = fFovy;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	if(FAILED(Engine::CCamera::Ready_Object()))
		return E_FAIL;

	return S_OK;
}

void CEffectCamera::KeyCheck(const _float& fTimeDelta)
{
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	if(Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fCameraSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if(Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[2][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fCameraSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if(Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fCameraSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if(Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3			vLook;
		memcpy(&vLook, &matWorld.m[0][0], sizeof(_vec3));

		_vec3			vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fCameraSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}
}

void CEffectCamera::MouseMove(void)
{
	_long	dwMouseMove = 0;
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB))
	{
		if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
		{
			_vec3			vRight;
			memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

			_vec3			vLook = m_vAt - m_vEye;	


			_matrix			matRot;
			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.0f));		

			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}

		if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
		{
			_vec3			vAxis = _vec3(0.f, 1.f, 0.f);

			_vec3			vLook = m_vAt - m_vEye;

			_matrix			matRot;

			D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(dwMouseMove / 10.0f));		

			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}
	}
}

CEffectCamera* CEffectCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, _float fFovy /*= D3DXToRadian(60.0f)*/, _float fAspect /*= _float(WINSIZEX) / WINSIZEY*/, _float fNear /*= 0.1f*/, _float fFar /*= 1000.f*/)
{
	CEffectCamera*			pInstance = new CEffectCamera(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Camera(pEye, pAt, pUp, fFovy, fAspect, fNear, fFar)))
	{
		MSG_BOX("CCamera_AniTool Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffectCamera::Free(void)
{
	Engine::CCamera::Free();
}

D3DXMATRIX CEffectCamera::GetView(void)
{
	return m_matView;
}

