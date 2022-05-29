#include "stdafx.h"
#include "MapCamera.h"
#include "Engine_Defines.h"
#include "WitchBlade.h"

CMapCamera::CMapCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CCamera(pGraphicDev, wstrName)
, m_pPlayer(NULL)
//, m_fGap(0.0f)
, m_vCamDir(0.0f, 1.0f, 0.0f)
{

}

CMapCamera::~CMapCamera(void)
{

}

HRESULT CMapCamera::InitCamera(void)
{
	m_pPlayer = static_cast<CWitchBlade*>(Engine::Find(L"Player"));
	Engine::Safe_Release(m_pPlayer);
	if(NULL == m_pPlayer)	return E_FAIL;

	if (Engine::GetSceneID() == Engine::SCENE_TOWN)
	{
		m_fGap = 150.0f;
	}
	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
	{
		m_fGap = 150.0f;
	}
	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
	{
		m_fGap = 300.0f;
	}

	_vec3	vPlayerPos = _vec3(0.0f, 0.0f, 0.0f);
	Engine::CTransform* pTrans = static_cast<Engine::CTransform*>(m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC));
	if(NULL == pTrans) return E_FAIL;

	pTrans->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);

	m_vEye	=	vPlayerPos + (m_vCamDir * m_fGap);
	m_vAt	=	vPlayerPos;

	m_fFovy		=	D3DXToRadian(60.0f);
	m_fAspect	=	_float(WINSIZEX) / _float(WINSIZEY);
	m_fNear		=	1.0f;
	m_fFar		=	900.0f;

	Engine::CCamera::Ready_Object();

	Engine::Safe_Release(pTrans);

	return S_OK;
}

HRESULT CMapCamera::Start_ForScene(void)
{
	if(FAILED(InitCamera()))
		return E_FAIL;

	return S_OK;
}

void CMapCamera::Update_Object(const _float& fTimeDelta)
{
	//FixMouse();

	_vec3	vPlayerPos = _vec3(0.0f, 0.0f, 0.0f);
	Engine::CTransform* pTrans = static_cast<Engine::CTransform*>(m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC));
	if(NULL == pTrans) return;

	pTrans->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);

	_float fYGap = abs(m_fGap - vPlayerPos.y);

	m_vEye	=	vPlayerPos + (m_vCamDir * fYGap) + _vec3(0.0f, 0.0f, -1.0f);
	m_vAt	=	vPlayerPos;

	Engine::Safe_Release(pTrans);

	Engine::CCamera::MakeViewMatrix();
}

void CMapCamera::FixMouse(void)
{
	POINT	ptMouse = { WINSIZEX >> 1, WINSIZEY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

CMapCamera* CMapCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CMapCamera*	pCamera = new CMapCamera(pGraphicDev, wstrName);

	return pCamera;
}

void CMapCamera::Free(void)
{

	Engine::CCamera::Free();
}

