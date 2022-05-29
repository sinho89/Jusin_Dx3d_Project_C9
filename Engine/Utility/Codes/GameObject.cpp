#include "GameObject.h"
#include "Component.h"
#include "Export_Engine.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
								 const OBJECTSTATE& eState, const _bool bIsActive)
: m_pGraphicDev(pGraphicDev)
, m_wstrName(wstrName)
, m_eState(eState)
, m_bIsActive(bIsActive)
, m_bLight(false)
, m_bIsStatic(false)
, m_bLoading(false)
, m_bOnceStart(false)
, m_bOutlineCheck(FALSE)
, m_iBlurPass(0)
, m_iBlurCnt(0)
, m_fBlurPower(0.0f)
, m_fBlurValue(0.0f)
, m_eBlurType(BLUR_END)
, m_bOutlineColor(1.0f, 1.0f, 1.0f)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject(void)
{

}

Engine::_float Engine::CGameObject::Get_ViewZ(void) const
{
	return m_fViewZ;
}

OBJECTSTATE& Engine::CGameObject::GetState(void)
{
	return m_eState;
}

_bool Engine::CGameObject::GetActive(void)
{
	return m_bIsActive;
}


_bool Engine::CGameObject::GetLight(void)
{
	return m_bLight;
}

wstring& Engine::CGameObject::GetName(void)
{
	return m_wstrName;
}

_bool CGameObject::GetStatic(void)
{
	return m_bIsStatic;
}
_bool CGameObject::GetLoading(void)
{
	return m_bLoading;
}
void Engine::CGameObject::SetState(const OBJECTSTATE& eState)
{
	m_eState = eState;
}

void Engine::CGameObject::SetActive(const _bool& bIsActive)
{
	m_bIsActive = bIsActive;
}


HRESULT Engine::CGameObject::Start_ForScene(void)
{
	return S_OK;
}

void Engine::CGameObject::Update_Object(const _float& fTimeDelta)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent[COMID_DYNAMIC].begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent[COMID_DYNAMIC].end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Update_Component(fTimeDelta);
	}
}

void Engine::CGameObject::Render_Object(void)
{

}

void Engine::CGameObject::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
}

void Engine::CGameObject::Compute_ViewZ(const _vec3* pPosition)
{
	_matrix			matView;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3			vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	m_fViewZ = D3DXVec3Length(&(vCamPos- *pPosition));
}

void Engine::CGameObject::Free(void)
{
	for (_int i = 0; i < COMID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CRelease_Pair());
		m_mapComponent[i].clear();
	}
	Engine::Safe_Release(m_pGraphicDev);
}