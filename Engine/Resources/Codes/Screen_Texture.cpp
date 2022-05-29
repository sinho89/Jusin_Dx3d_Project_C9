#include "Screen_Texture.h"

USING(Engine)

Engine::CScreen_Texture::CScreen_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
{

}

Engine::CScreen_Texture::CScreen_Texture(const CScreen_Texture& rhs)
: Engine::CVIBuffer(rhs)
{

}

Engine::CScreen_Texture::~CScreen_Texture(void)
{

}

HRESULT Engine::CScreen_Texture::Ready_Buffer(const _uint& iX, const _uint& iY,	const _uint& iSizeX, const _uint& iSizeY)
{	
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXSCREEN);
	m_dwVtxFVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

	m_isCreateVtxBuffer = true;
	m_isCreateIdxBuffer = false;

	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXSCREEN*	pVertex = NULL;

	float fX		=	float(iX);
	float fY		=	float(iY);
	float fSizeX	=	float(iSizeX);
	float fSizeY	=	float(iSizeY);

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition	=	_vec4(fX - 0.5f,			fY - 0.5f,			0.0f, 1.0f);
	pVertex[0].vTexUV		=	_vec2(0.0f, 0.0f);

	pVertex[1].vPosition	=	_vec4(fX + fSizeX + 0.5f,	fY - 0.5f,			0.0f, 1.0f);
	pVertex[1].vTexUV		=	_vec2(1.0f, 0.0f);

	pVertex[2].vPosition	=	_vec4(fX + fSizeX + 0.5f,	fY + fSizeY + 0.5f,	0.0f, 1.0f);
	pVertex[2].vTexUV		=	_vec2(1.0f, 1.0f);

	pVertex[3].vPosition	=	_vec4(fX - 0.5f,			fY + fSizeY + 0.5f,	0.0f, 1.0f);
	pVertex[3].vTexUV		=	_vec2(0.0f, 1.0f);

	m_pVB->Unlock();

	if(FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2, 0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB, 0)))
		return E_FAIL;

	INDEX16*	pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0	=	0;
	pIndex[0]._1	=	1;
	pIndex[0]._2	=	2;

	pIndex[1]._0	=	0;
	pIndex[1]._1	=	2;
	pIndex[1]._2	=	3;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CScreen_Texture::Render_Buffer(void)
{	
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

CScreen_Texture* Engine::CScreen_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev,
												 const _uint& iX, 
												 const _uint& iY, 
												 const _uint& iSizeX, 
												 const _uint& iSizeY)
{
	CScreen_Texture*		pInstance = new CScreen_Texture(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer(iX, iY, iSizeX, iSizeY)))
	{
		MSG_BOX("CGrid_Col Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResource* Engine::CScreen_Texture::Clone(void)
{
	return new CScreen_Texture(*this);
}

void Engine::CScreen_Texture::Free(void)
{
	Engine::CVIBuffer::Free();
}

