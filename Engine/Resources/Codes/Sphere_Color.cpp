#include "Sphere_Color.h"

USING(Engine)

Engine::CSphere_Color::CSphere_Color(LPDIRECT3DDEVICE9 pGraphicDev)
: CVIBuffer(pGraphicDev)
, m_pMesh(NULL)
, m_pmatParent(NULL)
{

}

Engine::CSphere_Color::~CSphere_Color(void)
{

}

void Engine::CSphere_Color::Render_Buffer(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if(m_pmatParent != NULL)
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pmatParent);
	m_pMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT Engine::CSphere_Color::Ready_Sphere(const _matrix* pmatParent)
{
	if(pmatParent == NULL)
		return E_FAIL;

	m_pmatParent = pmatParent;

	if(FAILED(D3DXCreateSphere(m_pGraphicDev, 1.f, 36, 36, &m_pMesh, NULL)))
		return E_FAIL;

	return S_OK;
}

CResource* Engine::CSphere_Color::Clone(void)
{
	return NULL;
}

CSphere_Color* Engine::CSphere_Color::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pmatParent)
{
	CSphere_Color* pInstance = new CSphere_Color(pGraphicDev);
	if(FAILED(pInstance->Ready_Sphere(pmatParent)))
	{
		MSG_BOX("CSphere_Color Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CSphere_Color::Free(void)
{
	Engine::CVIBuffer::Free();
}
