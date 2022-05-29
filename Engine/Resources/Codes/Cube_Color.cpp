#include "Cube_Color.h"

USING(Engine)

Engine::CCube_Color::CCube_Color(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CVIBuffer(pGraphicDev)
{

}
Engine::CCube_Color::CCube_Color(const CCube_Color& rhs)
: Engine::CVIBuffer(rhs)
{

}

Engine::CCube_Color::~CCube_Color(void)
{

}

HRESULT Engine::CCube_Color::Ready_Buffer(const _ulong& dwColor, const _matrix* pmatParent)
{
	if(pmatParent == NULL)
		return E_FAIL;

	m_pmatParent = pmatParent;

	m_isCreateVtxBuffer = true;
	m_dwVtxCnt = 8;
	m_dwVtxSize = sizeof(VTXCUBECOL);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEXCOORDSIZE3(0);

	m_isCreateIdxBuffer = true;
	m_dwTriCnt = 12;
	m_dwIdxSize = sizeof(INDEX16) * m_dwTriCnt;
	m_IndexFmt = D3DFMT_INDEX16;

	if(FAILED(Engine::CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	// 할당된 메모리에 접근을 하자.
	VTXCUBECOL*				pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	pVertex[0].dwColor = dwColor;

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	pVertex[1].dwColor = dwColor;

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	pVertex[2].dwColor = dwColor;

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	pVertex[3].dwColor = dwColor;

	pVertex[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	pVertex[4].dwColor = dwColor;

	pVertex[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	pVertex[5].dwColor = dwColor;

	pVertex[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	pVertex[6].dwColor = dwColor;

	pVertex[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);
	pVertex[7].dwColor = dwColor;

	m_pVB->Unlock();

	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._0 = 1; pIndex[0]._1 = 6; pIndex[0]._2 = 2;
	pIndex[1]._0 = 1; pIndex[1]._1 = 5; pIndex[1]._2 = 6;

	// -x									   
	pIndex[2]._0 = 4; pIndex[2]._1 = 0; pIndex[2]._2 = 3;
	pIndex[3]._0 = 4; pIndex[3]._1 = 3; pIndex[3]._2 = 7;

	// +y									   
	pIndex[4]._0 = 4; pIndex[4]._1 = 5; pIndex[4]._2 = 1;
	pIndex[5]._0 = 4; pIndex[5]._1 = 1; pIndex[5]._2 = 0;

	// -y									   
	pIndex[6]._0 = 3; pIndex[6]._1 = 2; pIndex[6]._2 = 6;
	pIndex[7]._0 = 3; pIndex[7]._1 = 6; pIndex[7]._2 = 7;

	// +z									   
	pIndex[8]._0 = 7; pIndex[8]._1 = 6; pIndex[8]._2 = 5;
	pIndex[9]._0 = 7; pIndex[9]._1 = 5; pIndex[9]._2 = 4;

	// -z									   
	pIndex[10]._0 = 0; pIndex[10]._1 = 1; pIndex[10]._2 = 2;
	pIndex[11]._0 = 0; pIndex[11]._1 = 2; pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void Engine::CCube_Color::Render_Buffer(void)
{
	m_pGraphicDev->SetTexture(0, NULL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pmatParent);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CCube_Color* Engine::CCube_Color::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pmatParent, const _ulong& dwColor)
{
	CCube_Color*		pInstance = new CCube_Color(pGraphicDev);

	if(FAILED(pInstance->Ready_Buffer(dwColor, pmatParent)))
	{
		MSG_BOX("CCube_Color Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

CResource* Engine::CCube_Color::Clone(void)
{
	return new CCube_Color(*this);
}

void Engine::CCube_Color::Free(void)
{
	Engine::CVIBuffer::Free();
}