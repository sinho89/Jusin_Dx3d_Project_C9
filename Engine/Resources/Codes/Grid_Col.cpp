#include "Grid_Col.h"

USING(Engine)

Engine::CGrid_Col::CGrid_Col(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
, m_pPosition(NULL)
, m_isClone(false)
{

}

Engine::CGrid_Col::CGrid_Col(const CGrid_Col& rhs)
: Engine::CVIBuffer(rhs)
, m_pPosition(rhs.m_pPosition)
, m_fh(rhs.m_fh)
, m_ih(rhs.m_ih)
, m_isClone(true)
{

}

Engine::CGrid_Col::~CGrid_Col(void)
{

}

void Engine::CGrid_Col::Copy_Indices(Engine::INDEX32* pIndices, const _ulong& dwTriCnt)
{
	Engine::INDEX32*		pOriIndices = NULL;

	m_pIB->Lock(0, 0, (void**)&pOriIndices, 0);

	memcpy(pOriIndices, pIndices, sizeof(Engine::INDEX32) * dwTriCnt);	

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

HRESULT Engine::CGrid_Col::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInterval)
{
	m_isCreateVtxBuffer = true;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_pPosition = new _vec3[m_dwVtxCnt];

	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	m_isCreateIdxBuffer = true;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwIdxSize = sizeof(INDEX32) * m_dwTriCnt;
	m_IndexFmt = D3DFMT_INDEX32;

	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	_ulong			dwByte = 0;

	// 할당된 메모리에 접근을 하자.
	VTXCOL*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong		dwIndex = 0;

	for	(_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition = _vec3(((float)(j * dwVtxInterval)), 0.0f, ((float)(i * dwVtxInterval)) );
			m_pPosition[dwIndex] = pVertex[dwIndex].vPosition;
			pVertex[dwIndex].dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	INDEX32*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong		dwTriIdx = 0;

	for	(_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;
			++dwTriIdx;

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			++dwTriIdx;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();


	return S_OK;

}

void Engine::CGrid_Col::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CGrid_Col* Engine::CGrid_Col::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInterval)
{
	CGrid_Col*		pInstance = new CGrid_Col(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxInterval)))
	{
		MSG_BOX("CGrid_Col Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;

}

CResource* Engine::CGrid_Col::Clone(void)
{
	return new CGrid_Col(*this);
}

void Engine::CGrid_Col::Free(void)
{
	Engine::CVIBuffer::Free();

	if(false == m_isClone)
	{
		Engine::Safe_Delete_Array(m_pPosition);
	}
}

