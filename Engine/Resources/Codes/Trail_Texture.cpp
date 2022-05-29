#include "Trail_Texture.h"

USING(Engine)


Engine::CTrail_Texture::CTrail_Texture(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
, m_pPosition(NULL)
, m_isClone(false)
{

}

Engine::CTrail_Texture::CTrail_Texture(const CTrail_Texture& rhs)
: Engine::CVIBuffer(rhs)
, m_pPosition(rhs.m_pPosition)
, m_fh(rhs.m_fh)
, m_ih(rhs.m_ih)
, m_isClone(true)
{

}

Engine::CTrail_Texture::~CTrail_Texture(void)
{

}

void Engine::CTrail_Texture::Copy_Indices(Engine::INDEX32* pIndices, const _ulong& dwTriCnt)
{
	Engine::INDEX32*		pOriIndices = NULL;

	m_pIB->Lock(0, 0, (void**)&pOriIndices, 0);

	memcpy(pOriIndices, pIndices, sizeof(Engine::INDEX32) * dwTriCnt);	

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

HRESULT Engine::CTrail_Texture::Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInterval)
{
	m_isCreateVtxBuffer = true;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_pPosition = new _vec3[m_dwVtxCnt];

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	m_isCreateIdxBuffer = true;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwIdxSize = sizeof(INDEX32) * m_dwTriCnt;
	m_IndexFmt = D3DFMT_INDEX32;

	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	_ulong			dwByte = 0;

	// 할당된 메모리에 접근을 하자.
	VTXTEX*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong		dwIndex = 0;



	for	(_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			pVertex[dwIndex].vPosition = _vec3( 0.0f, 0.0f, 0.0f);
			m_pPosition[dwIndex] = pVertex[dwIndex].vPosition;
			pVertex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			pVertex[dwIndex].vTexUV = _vec2(_float(i) / (dwCntZ - 1.f), _float(j) / (dwCntX - 1.f));
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

			_vec3		vDest, vSour, vNormal;
			vDest = pVertex[pIndex[dwTriIdx]._1].vPosition - pVertex[pIndex[dwTriIdx]._0].vPosition;
			vSour = pVertex[pIndex[dwTriIdx]._2].vPosition - pVertex[pIndex[dwTriIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			++dwTriIdx;

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;
			vDest = pVertex[pIndex[dwTriIdx]._1].vPosition - pVertex[pIndex[dwTriIdx]._0].vPosition;
			vSour = pVertex[pIndex[dwTriIdx]._2].vPosition - pVertex[pIndex[dwTriIdx]._1].vPosition;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);
			pVertex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVertex[pIndex[dwTriIdx]._2].vNormal += vNormal;
			++dwTriIdx;
		}
	}

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)	
		D3DXVec3Normalize(&pVertex[i].vNormal, &pVertex[i].vNormal);


	m_pVB->Unlock();
	m_pIB->Unlock();


	return S_OK;
}

void Engine::CTrail_Texture::Render_Buffer(void)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

CTrail_Texture* Engine::CTrail_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInterval)
{
	CTrail_Texture*		pInstance = new CTrail_Texture(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxInterval)))
	{
		MSG_BOX("CTrail_Texture Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResource* Engine::CTrail_Texture::Clone(void)
{
	return new CTrail_Texture(*this);
}

void Engine::CTrail_Texture::Free(void)
{
	Engine::CVIBuffer::Free();

	if(false == m_isClone)
	{
		Engine::Safe_Delete_Array(m_pPosition);
	}
}

