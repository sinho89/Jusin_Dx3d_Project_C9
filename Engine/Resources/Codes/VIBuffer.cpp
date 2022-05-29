#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
: CResource(pGraphicDev)
, m_pVB(NULL)
, m_pIB(NULL)
, m_dwVtxCnt(0)
, m_dwVtxSize(0)
, m_dwVtxFVF(0)
{

}

Engine::CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
: CResource(rhs)
, m_pVB(rhs.m_pVB)
, m_pIB(rhs.m_pIB)
, m_dwVtxCnt(rhs.m_dwVtxCnt)
, m_dwVtxSize(rhs.m_dwVtxSize)
, m_dwVtxFVF(rhs.m_dwVtxFVF)
, m_dwIdxSize(rhs.m_dwIdxSize)
, m_dwTriCnt(rhs.m_dwTriCnt)
, m_IndexFmt(rhs.m_IndexFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

void CVIBuffer::Change_BoxColor(const _ulong & dwColor)
{
	VTXCUBECOL*		pVertex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	for (int i = 0; i < 8; ++i)
	{
		pVertex[i].dwColor = dwColor;
	}
	m_pVB->Unlock();
}

HRESULT Engine::CVIBuffer::Ready_Buffer(void)
{
	// CreateVertexBuffer : 정점(Vertex)들을 보관하기위한 메모리공간(Buffer)을 생성하낟.
	if(true == m_isCreateVtxBuffer)
	{
		if(FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
			return E_FAIL;
	}

	if(true == m_isCreateIdxBuffer)
	{
		if(FAILED(m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize, 0, m_IndexFmt, D3DPOOL_MANAGED, &m_pIB, NULL)))
			return E_FAIL;
	}

	return S_OK;
}

void Engine::CVIBuffer::GetVtxInfo( void* pVtxInfo )
{
	void* pOriVtx = NULL;

	m_pVB->Lock(0, 0, (void**)&pOriVtx, 0);
	memcpy(pVtxInfo, pOriVtx, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}


void Engine::CVIBuffer::SetVtxInfo(void* pVtxInfo)
{
	void* pOriVtx = NULL;

	m_pVB->Lock(0, 0, (void**)&pOriVtx, 0);
	memcpy(pOriVtx, pVtxInfo, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::Free(void)
{
	Engine::CResource::Free();

	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
}

