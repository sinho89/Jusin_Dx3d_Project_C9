#include "Triangle_Texture.h"

USING(Engine)

Engine::CTriangle_Texture::CTriangle_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
{

}
Engine::CTriangle_Texture::CTriangle_Texture(const CTriangle_Texture& rhs)
: Engine::CVIBuffer(rhs)
{

}

Engine::CTriangle_Texture::~CTriangle_Texture(void)
{

}

HRESULT Engine::CTriangle_Texture::Ready_Buffer(void)
{
	return S_OK;
}


void Engine::CTriangle_Texture::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CTriangle_Texture* Engine::CTriangle_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriangle_Texture*		pInstance = new CTriangle_Texture(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer()))
	{
		MSG_BOX("CRect_Texture Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResource* Engine::CTriangle_Texture::Clone(void)
{
	return new CTriangle_Texture(*this);
}

void Engine::CTriangle_Texture::Free(void)
{
	Engine::CVIBuffer::Free();



}

