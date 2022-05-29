#include "Resource.h"

USING(Engine)

Engine::CResource::CResource(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CResource::CResource(const CResource& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CResource::~CResource(void)
{

}

void Engine::CResource::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
}

