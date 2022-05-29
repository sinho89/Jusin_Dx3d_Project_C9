#include "Rect_Color.h"

USING(Engine)

Engine::CRect_Color::CRect_Color(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
{

}
Engine::CRect_Color::CRect_Color(const CRect_Color& rhs)
: Engine::CVIBuffer(rhs)
{

}

Engine::CRect_Color::~CRect_Color(void)
{

}

HRESULT Engine::CRect_Color::Ready_Buffer(void)
{
	m_isCreateVtxBuffer = true;
	m_dwVtxCnt = 6;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	m_isCreateIdxBuffer = false;



	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	// 할당된 메모리에 접근을 하자.
	VTXCOL*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].dwColor = D3DXCOLOR(1.0f, 1.0f, 0.f, 1.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].dwColor = D3DXCOLOR(1.0f, 1.0f, 0.f, 1.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].dwColor = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);


	pVertex[3].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[3].dwColor = D3DXCOLOR(1.0f, 1.0f, 0.f, 1.f);

	pVertex[4].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[4].dwColor = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);

	pVertex[5].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[5].dwColor = D3DXCOLOR(1.0f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();

	return S_OK;
}

void Engine::CRect_Color::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));
	m_pGraphicDev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

CRect_Color* Engine::CRect_Color::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRect_Color*		pInstance = new CRect_Color(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("CRect_Color Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResource* Engine::CRect_Color::Clone(void)
{
	return new CRect_Color(*this);
}

void Engine::CRect_Color::Free(void)
{
	Engine::CVIBuffer::Free();
}