#include "Cell.h"
#include "Line.h"
#include "Rect_Texture.h"

USING(Engine)

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pD3DXLine(NULL)
, m_pGraphicDev(pGraphicDev)
, m_dwIndex(0)
{
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
	ZeroMemory(m_bMovePoint, sizeof(bool) * POINT_END);
	ZeroMemory(m_bChoosedPoint, sizeof(bool) * POINT_END);
	ZeroMemory(m_fArrivalCost, sizeof(float) * LINE_END);
	m_pGraphicDev->AddRef();
}

Engine::CCell::~CCell(void)
{

}

const _vec3* Engine::CCell::Get_Point(POINT eType) const
{
	return &m_vPoint[eType];
}

CCell* Engine::CCell::Get_Neighbor(NEIGHBOR eType) const
{
	if(NULL == m_pNeighbor[eType])
		return  NULL;

	return m_pNeighbor[eType];
}

const _ulong* Engine::CCell::Get_Index(void)
{
	return &m_dwIndex;
}

void Engine::CCell::Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor)
{
	m_pNeighbor[eType] = pNeighbor;
}

HRESULT Engine::CCell::Ready_Cell(const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, NAVI_TYPE eType)
{
	m_dwIndex = dwIndex;
	m_eType   = eType;
	m_eFirstType = m_eType;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_vLineCenter[LINE_AB] = (m_vPoint[POINT_A] + m_vPoint[POINT_B])/2.f;
	m_vLineCenter[LINE_BC] = (m_vPoint[POINT_B] + m_vPoint[POINT_C])/2.f;
	m_vLineCenter[LINE_CA] = (m_vPoint[POINT_C] + m_vPoint[POINT_A])/2.f;

	m_vCellCenter = (m_vPoint[POINT_A] + m_vPoint[POINT_B] + m_vPoint[POINT_C])/3.f;

	_vec3 vDist;
	vDist = m_vLineCenter[LINE_AB] - m_vCellCenter;
	m_fArrivalCost[LINE_AB] = D3DXVec3Length(&vDist);
	vDist = m_vLineCenter[LINE_BC] - m_vCellCenter;
	m_fArrivalCost[LINE_BC] = D3DXVec3Length(&vDist);
	vDist = m_vLineCenter[LINE_CA] - m_vCellCenter;
	m_fArrivalCost[LINE_CA] = D3DXVec3Length(&vDist);

	m_pLine[LINE_AB] = CLine::Create(&_vec2(m_vPoint[POINT_A].x,m_vPoint[POINT_A].z)
		, &_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));
	m_pLine[LINE_BC] = CLine::Create(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z)
		, &_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));
	m_pLine[LINE_CA] = CLine::Create(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z)
		, &_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));

	//m_pIndexTex = CRect_Texture::Create(m_pGraphicDev);
	if (FAILED(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine)))
		return E_FAIL;

#ifdef _DEBUG


#endif

	return S_OK;
}

_bool Engine::CCell::Compare_Point(const _vec3* pPointA, const _vec3* pPointB, CCell* pCell)
{
	if(m_vPoint[POINT_A] == *pPointA)
	{
		if(m_vPoint[POINT_B] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if(m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if(m_vPoint[POINT_B] == *pPointA)
	{
		if(m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if(m_vPoint[POINT_C] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if(m_vPoint[POINT_C] == *pPointA)
	{
		if(m_vPoint[POINT_A] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
		if(m_vPoint[POINT_B] == *pPointB)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	return false;

}

CCell::COMPARE Engine::CCell::Compare(const _vec3* pEndPos, _ulong* pCellIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		//라인의 왼쪽에 있을때(삼각형 안에 있을때..)
		if(CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if(NULL == m_pNeighbor[i])
				return CCell::COMPARE_NOMOVE;
			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_Index();
				return CCell::COMPARE_MOVE;
			}
		}
		//if (CLine::COMPARE_RIGHT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		//{
		//	return CCell::COMPARE_NOMOVE;
		//}
	}
	return CCell::COMPARE_MOVE;	
}

void Engine::CCell::Render_Cell()
{
	_vec3			vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix			matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if(vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

#ifdef _DEBUG

	m_pD3DXLine->SetWidth(3.f);

	m_pGraphicDev->EndScene();

	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();

	_matrix				matTmp;

	if (m_eType == TYPE_FALL)
	{
		m_pD3DXLine->DrawTransform(vPoint, 4,
			D3DXMatrixIdentity(&matTmp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}
	else if (m_eType == TYPE_NORMAL)
	{
		m_pD3DXLine->DrawTransform(vPoint, 4,
			D3DXMatrixIdentity(&matTmp), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
	}
	else
	{
		m_pD3DXLine->DrawTransform(vPoint, 4,
			D3DXMatrixIdentity(&matTmp), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	}

	m_pD3DXLine->End();

#endif


//	m_pIndexTex->Render_Buffer();

}


CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev,
							 const _ulong& dwIndex,
							 const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC,
							 NAVI_TYPE eType)
{
	CCell* pInstance = new CCell(pGraphicDev);

	if(FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC, eType)))
	{
		MSG_BOX("CCell Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CCell::Free(void)
{
	for(_uint i = 0; i < LINE_END; i++)
		Engine::Safe_Release(m_pLine[i]);
	Engine::Safe_Release(m_pGraphicDev);	
	Engine::Safe_Release(m_pD3DXLine);
}

void Engine::CCell::SetMovePoint( D3DXVECTOR3* vPoint )
{
	if(m_bMovePoint[POINT_A])
	{
		m_vPoint[POINT_A] = *vPoint;
		m_bChoosedPoint[POINT_A] = true;
	}
	else if(m_bMovePoint[POINT_B])
	{
		m_vPoint[POINT_B] = *vPoint;
		m_bChoosedPoint[POINT_B] = true;
	}
	else if(m_bMovePoint[POINT_C])
	{
		m_vPoint[POINT_C] = *vPoint;
		m_bChoosedPoint[POINT_C] = true;
	}

	m_pLine[LINE_AB]->Ready_Line(&_vec2(m_vPoint[POINT_A].x,m_vPoint[POINT_A].z)
		, &_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z));
	m_pLine[LINE_BC]->Ready_Line(&_vec2(m_vPoint[POINT_B].x, m_vPoint[POINT_B].z)
		, &_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z));
	m_pLine[LINE_CA]->Ready_Line(&_vec2(m_vPoint[POINT_C].x, m_vPoint[POINT_C].z)
		, &_vec2(m_vPoint[POINT_A].x, m_vPoint[POINT_A].z));
}

void Engine::CCell::ChoicePoint( void )
{
	if(m_bChoosedPoint[POINT_A])
	{
		if(GetAsyncKeyState(VK_UP))
		{
			m_vPoint[POINT_A].y += 0.01f;
		}
		if(GetAsyncKeyState(VK_DOWN))
		{
			m_vPoint[POINT_A].y -= 0.01f;
		}
	}


	if(m_bChoosedPoint[POINT_B])
	{
		if(GetAsyncKeyState(VK_UP))
		{
			m_vPoint[POINT_B].y += 0.01f;
		}
		if(GetAsyncKeyState(VK_DOWN))
		{
			m_vPoint[POINT_B].y -= 0.01f;
		}
	}


	if(m_bChoosedPoint[POINT_C])
	{
		if(GetAsyncKeyState(VK_UP))
		{
			m_vPoint[POINT_C].y += 0.01f;
		}
		if(GetAsyncKeyState(VK_DOWN))
		{
			m_vPoint[POINT_C].y -= 0.01f;
		}
	}
}

void Engine::CCell::SetType( NAVI_TYPE eType )
{
	m_eType = eType;
}

void Engine::CCell::SetFirstType( void )
{
	m_eType = m_eFirstType;
}

Engine::NAVI_TYPE Engine::CCell::GetType( void )
{
	return m_eType;
}

Engine::NAVIINFO* Engine::CCell::GetNaviInfo( void )
{
	for(int i = 0; i < 3; ++i)
	{
		m_tInfo.vPoint[i] = m_vPoint[i];
	}
	
	m_tInfo.m_eType = m_eFirstType;

	return &m_tInfo;
}

_ulong Engine::CCell::GetIndex( void )
{
	return m_dwIndex;
}

_vec3 Engine::CCell::GetCellCenter( void )
{
	return m_vCellCenter;
}

_float* Engine::CCell::GetArrivalCost( void )
{
	return m_fArrivalCost;
}

_vec3* Engine::CCell::GetLineCenter( void )
{
	return m_vLineCenter;
}

Engine::CCell::LINE Engine::CCell::GetLineType( const _vec3* pEndPos, _ulong* pCellIndex )
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		//라인의 왼쪽에 있을때(삼각형 안에 있을때..)
		if(CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if(NULL == m_pNeighbor[i])
				return CCell::LINE(i);

		}
	}
	return CCell::LINE_END;	
}




