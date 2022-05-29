#include "SphereMesh.h"

USING(Engine)

Engine::CSphereMesh::CSphereMesh(LPDIRECT3DDEVICE9 pGraphicDev)
: CMesh(pGraphicDev)
, m_pMesh(NULL)
{
}

CSphereMesh::CSphereMesh(const CSphereMesh & rhs)
: CMesh(rhs)
, m_pMesh(rhs.m_pMesh)
{
}

Engine::CSphereMesh::~CSphereMesh(void)
{
}

void Engine::CSphereMesh::GetMesh(LPD3DXMESH * ppMesh)
{
	*ppMesh = m_pMesh;
}

void Engine::CSphereMesh::Render_Mesh(void)
{
	m_pMesh->DrawSubset(0);
}

HRESULT Engine::CSphereMesh::Ready_Mesh(void)
{
	if (FAILED(D3DXCreateSphere(m_pGraphicDev, 1.f, 36, 36, &m_pMesh, NULL)))
		return E_FAIL;

	return S_OK;
}

CResource * Engine::CSphereMesh::Clone(void)
{
	return new CSphereMesh(*this);
}

CSphereMesh * Engine::CSphereMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSphereMesh*		pInstance = new CSphereMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Mesh()))
	{
		MSG_BOX("CSphereMesh Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CSphereMesh::Free(void)
{
	Engine::CMesh::Free();
}
