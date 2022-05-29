#include "Camera.h"

USING(Engine)

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_vEye(0.0f, 0.0f, 0.0f)
, m_vUp(0.0f, 1.0f, 0.0f)
, m_vAt(0.0f, 0.0f, 0.0f)
, m_fFovy(0.0f)
, m_fAspect(0.0f)
, m_fNear(0.0f)
, m_fFar(0.0f)
, m_iShakeCnt(0)
, m_bShake(false)
, m_iShakeMaxCnt(0)
, m_fShakePower(0.f)
, m_vShakeDir(0.f, 0.f, 0.f)
, m_iShakeDelay(0)
, m_fGap(0.0f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_bIsStatic = true;
}

Engine::CCamera::~CCamera(void)
{

}

HRESULT Engine::CCamera::Ready_Object(void)
{
	// For.View Matrix
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	// For.Projection Matrix
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovy, m_fAspect, m_fNear, m_fFar);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

void Engine::CCamera::Update_Object(const _float& fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCamera::MakeViewMatrix(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
}

void CCamera::MakeProjMatrix(float fWinSizeX, float fWinSizeY)
{
	m_fAspect = fWinSizeX / fWinSizeY;
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovy, m_fAspect, m_fNear, m_fFar);
}

void CCamera::MakeProjOrthoLHMatrix(float fWinSizeX, float fWinSizeY, float fZn, float fZf)
{
	if (0 == fZn)
		fZn = m_fNear;

	if (0 == fZf)
		fZf = m_fFar;

	D3DXMatrixOrthoLH(&m_matProj, fWinSizeX, fWinSizeY, fZn, fZf);
}

void CCamera::Set_Shake(const _vec3 & vShakeDir, const _float & fPower, const _int & iCnt, const _int& iDelay)
{
	m_iShakeCnt = 0;
	m_bShake = true;
	m_vShakeDir = vShakeDir;
	m_fShakePower = fPower;
	m_iShakeMaxCnt = iCnt;
	m_iShakeDelay = iDelay;
}

void CCamera::CameraShake(void)
{
	if (m_iShakeCnt > m_iShakeMaxCnt)
	{
		m_bShake = false;
		m_iShakeCnt = 0;
	}

	if (m_bShake)
	{
		_float fDecelValue = 1.f - (_float(m_iShakeCnt) / _float(m_iShakeMaxCnt));

		if ((m_iShakeCnt % (2 * m_iShakeDelay)) == 0)
		{
			m_vEye = m_vEye + (m_vShakeDir * m_fShakePower * fDecelValue);
		}

		if ((m_iShakeCnt % (2 * m_iShakeDelay)) == 1)
		{
			m_vEye = m_vEye - (m_vShakeDir * m_fShakePower * fDecelValue);
		}

		m_iShakeCnt++;
	}
}

void Engine::CCamera::Free(void)
{
	Engine::CGameObject::Free();
}