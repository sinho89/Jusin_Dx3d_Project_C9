#include "stdafx.h"
#include "Camera_MapTool.h"
#include "Player.h"

#include "Export_Engine.h"

CCamera_MapTool::CCamera_MapTool(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CCamera(pGraphicDev, wstrName)
, m_fCameraSpeed(0.f)
, m_vCamDir(0.0f, 0.0f, -1.0f)
, m_fEpslnY(0.0f)
, m_fGap(0.0f)
, m_bIsMove(true)
, m_bPush(false) 
, m_bMouseAngle(false)
, m_bMouseFix(false)
{
	m_eCameraMode = MODE_NORMAL;
}

CCamera_MapTool::~CCamera_MapTool(void)
{

}

void CCamera_MapTool::Update_Object(const _float& fTimeDelta)
{
	if(m_eCameraMode == MODE_NORMAL)
	{
		KeyCheck(fTimeDelta);
	}

	if(m_bMouseFix && m_eCameraMode == MODE_NORMAL)
	{
		FixMouse();
		MouseMove();
	}

	if(m_eCameraMode == MODE_DEBUG)
	{
		DebugMouseMove();

		_vec3	vPlayerPos = _vec3(0.0f, 0.0f, 0.0f);

		if(NULL != Engine::FindList(L"Player"))
		{
			list<Engine::CGameObject*>::iterator  iter = Engine::FindList(L"Player")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Player")->end();
			for(iter; iter != iter_end;iter++)
			{
				((CPlayer*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);
			}
		}

		m_vEye	=	vPlayerPos + (m_vCamDir * m_fGap) + _vec3(0.0f, m_fEpslnY, 0.0f);
		m_vAt	=	vPlayerPos + _vec3(0.0f, m_fEpslnY, 0.0f);	
	}

	Engine::CCamera::Update_Object(fTimeDelta);
}

HRESULT CCamera_MapTool::Ready_Camera(const _vec3* pEye, const _vec3* pAt, 
									  const _vec3* pUp, _float fFovy, _float fAspect, _float fNear, _float fFar)
{
	m_fCameraSpeed = 50.f;

	m_fEpslnY	=	7.0f;
	m_fGap		=	30.0f;

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

void CCamera_MapTool::KeyCheck(const _float& fTimeDelta)
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

	if(Engine::Get_DIKeyState(DIK_Q)		& 0x80)
	{
		if(m_bMouseAngle)
			return;

		m_bMouseAngle = TRUE;

		if(m_bMouseFix)	m_bMouseFix = FALSE;
		else			m_bMouseFix = TRUE;
	}
	else
		m_bMouseAngle = FALSE;

	if(m_bMouseFix == FALSE)
		return;
}

void CCamera_MapTool::MouseMove(void)
{
	_long	dwMouseMove = 0;
	_matrix			matWorld;
	D3DXMatrixInverse(&matWorld, NULL, &m_matView);

	/*if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))*/
	//if(m_bMouseAngle)
	{
		if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
		{
			_vec3			vRight;
			memcpy(&vRight, &matWorld.m[0][0], sizeof(_vec3));

			_vec3			vLook = m_vAt - m_vEye;	


			_matrix			matRot;
			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 5.0f));		

			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}

		if(dwMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
		{
			_vec3			vAxis = _vec3(0.f, 1.f, 0.f);

			_vec3			vLook = m_vAt - m_vEye;

			_matrix			matRot;

			D3DXMatrixRotationAxis(&matRot, &vAxis, D3DXToRadian(dwMouseMove / 5.0f));		

			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}
	}
}

CCamera_MapTool* CCamera_MapTool::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
										 const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, 
										 _float fFovy /*= D3DXToRadian(60.0f)*/, _float fAspect /*= _float(WINSIZEX) / WINSIZEY*/, 
										 _float fNear /*= 0.1f*/, _float fFar /*= 1000.f*/)
{
	CCamera_MapTool*			pInstance = new CCamera_MapTool(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Camera(pEye, pAt, pUp, fFovy, fAspect, fNear, 100000)))
	{
		MSG_BOX("CCamera_AniTool Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_MapTool::Free(void)
{
	Engine::CCamera::Free();
}

_vec3 CCamera_MapTool::GetCamEye( void )
{
	return m_vEye;
}

_vec3 CCamera_MapTool::GetCamAt( void )
{
	return m_vAt;
}

_matrix CCamera_MapTool::GetProj( void )
{
	return m_matProj;
}

_matrix CCamera_MapTool::GetView( void )
{
	return m_matView;
}

void CCamera_MapTool::SetCamSpeed( int iSpeed )
{
	m_fCameraSpeed = (float)iSpeed;
}

void CCamera_MapTool::SetCamEye(_vec3 vPos)
{
	m_vEye = vPos;
}
void CCamera_MapTool::FixMouse( void )
{
	POINT	ptMouse = { (WINSIZEX + 550) >> 1 , (WINSIZEY + 100) >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CCamera_MapTool::SetCamMode( eCAMERA_MODE eMode )
{
	m_eCameraMode = eMode;
}

void CCamera_MapTool::DebugMouseMove( void )
{
	_int iDistance = 0;

	if(iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
	{
		_matrix	matAxis;
		D3DXMatrixRotationAxis(&matAxis, &_vec3(0.0f, 1.0f, 0.0f), D3DXToRadian(iDistance / 10.0f));
		D3DXVec3TransformNormal(&m_vCamDir, &m_vCamDir, &matAxis);
	}

	if(iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
	{
		_vec3	vRight;
		_matrix	matCam;

		D3DXMatrixInverse(&matCam, NULL, &m_matView);
		memcpy(&vRight, &matCam.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);

		_matrix matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance / 10.0f));

		D3DXVec3TransformNormal(&m_vCamDir, &m_vCamDir, &matAxis);
	}

	if(iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Z))
	{
		if (iDistance < 0)
			m_fGap +=10;
		else
			m_fGap -=10;			
	}
}

