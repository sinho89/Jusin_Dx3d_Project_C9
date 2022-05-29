#include "Triangle_Color.h"

USING(Engine)

Engine::CTriangle_Color::CTriangle_Color(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
{

}

Engine::CTriangle_Color::CTriangle_Color(const CTriangle_Color& rhs)
: Engine::CVIBuffer(rhs)
{

}

Engine::CTriangle_Color::~CTriangle_Color(void)
{

}

HRESULT Engine::CTriangle_Color::Ready_Buffer(void)
{
	m_dwVtxCnt = 3;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL*			pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(0.0f, 0.5f, 0.f);
	pVertex[0].dwColor = D3DXCOLOR(0.0f, 1.f, 0.f, 1.f);

	pVertex[1].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[1].dwColor = D3DXCOLOR(0.0f, 1.f, 0.f, 1.f);

	pVertex[2].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[2].dwColor = D3DXCOLOR(0.0f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	return S_OK;

}

void Engine::CTriangle_Color::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwVtxFVF);

	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}

CTriangle_Color* Engine::CTriangle_Color::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriangle_Color*		pInstance = new CTriangle_Color(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("CTri_Color Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResource* Engine::CTriangle_Color::Clone(void)
{
	return new CTriangle_Color(*this);
}

void Engine::CTriangle_Color::Free(void)
{
	Engine::CVIBuffer::Free();
}