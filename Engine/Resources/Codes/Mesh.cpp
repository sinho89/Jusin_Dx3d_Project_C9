#include "Mesh.h"

USING(Engine)

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CResource(pGraphicDev)
, m_bClone(false)
{
	
}

CMesh::CMesh(const CMesh& rhs)
: Engine::CResource(rhs)
, m_bClone(true)
{

}

Engine::CMesh::~CMesh(void)
{

}


void Engine::CMesh::Free(void)
{
	Engine::CResource::Free();
}

