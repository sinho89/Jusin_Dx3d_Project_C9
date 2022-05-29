#include "NavigationMesh.h"
#include "Cell.h"

USING(Engine)

Engine::CNavigationMesh::CNavigationMesh(LPDIRECT3DDEVICE9 pGraphicDev)
: CMesh(pGraphicDev)
, m_dwCurrentIdx(0)
, m_pNextCell(NULL)
{

}

Engine::CNavigationMesh::CNavigationMesh(const CNavigationMesh& rhs)
: CMesh(rhs)
, m_vecCell(rhs.m_vecCell)
, m_dwCurrentIdx(rhs.m_dwCurrentIdx)
, m_dwEndIdx(0)
, m_dwTargetIdx(0)
, m_dwIdx(0)
, m_iCount(0)
, m_pNextCell(NULL)
, m_isFinish(false)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		m_vecCell[i]->AddRef();
	}	
}

Engine::CNavigationMesh::~CNavigationMesh(void)
{

}


void Engine::CNavigationMesh::Set_CurrentCellIndex(const _ulong& dwCurrentIndex)
{
	m_dwCurrentIdx = dwCurrentIndex;
}

_ulong Engine::CNavigationMesh::Get_CurrentCellIndex( _vec3* pPos )
{
	m_dwMaxIdx = m_vecCell.size();

	_vec3 vPoint[3];

	vector<Engine::CCell*>	::iterator iter = m_vecCell.begin();
	vector<Engine::CCell*>::iterator iter_end = m_vecCell.end();

	for(iter; iter!= iter_end; ++iter)
	{
		_bool bCheck[3];
		ZeroMemory(bCheck, sizeof(bool)*3);

		//셀의 세 꼭지점을 가져와...
		for(int i = 0; i < 3 ;++i)
		{
			vPoint[i] = *(*iter)->Get_Point((Engine::CCell::POINT)i);
		}

		//방향을 구하고..
		_vec3 vDirection[3] = 
		{
			vPoint[1] - vPoint[0],
			vPoint[2] - vPoint[1],
			vPoint[0] - vPoint[2],
		};
		
		//세점의 법선벡터를 구해서..
		_vec3 vNormal[3] =
		{
			_vec3(-vDirection[0].z, 0.f, vDirection[0].x),
			_vec3(-vDirection[1].z, 0.f, vDirection[1].x),
			_vec3(-vDirection[2].z, 0.f, vDirection[2].x),
		};

		//단위 벡터로 만들었어...
		for(int i = 0; i < 3; ++i)
			D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

		for(int i = 0; i < 3; ++i)
		{
			//현재 나의 위치와 세 꼭지점간의 방향벡터를구해서..
			D3DXVECTOR3	vDestDir = *pPos - vPoint[i];
			
			//그 꼭지점의 법선벡터와 나의 위치랑 내적을해서 왼쪽인지 오른쪽인지 판단해.
			float fDotResult = D3DXVec3Dot(&vDestDir, &vNormal[i]);

			if(fDotResult > 0.f)
			{
				bCheck[i] = false;				
			}
			else
			{
				bCheck[i] = true;
			}
		}
		//그래서 전부 왼쪽이면 삼각형 안에 들어온것이다...
		if(bCheck[0] && bCheck[1] && bCheck[2])
		{
			float fCurrentY = (*pPos).y;
			float fMaxY = max(max(vPoint[0].y, vPoint[1].y), vPoint[2].y) + 3.f;
			float fMinY = min(min(vPoint[0].y, vPoint[1].y), vPoint[2].y) - 3.f;
			//y값도 판단해야해...
			if(fCurrentY > fMinY && fCurrentY < fMaxY)
			{
				m_dwCurrentIdx = ((CCell*)(*iter))->GetIndex();
				return ((CCell*)(*iter))->GetIndex();
			}
		}
	}

	//위치가 셀 안에 들어있지 않다면
//	Get_CurrentCellIndex(pPos);
	return 0;
}


HRESULT Engine::CNavigationMesh::Ready_NavigationMesh(const _tchar* pFilePath, const _tchar* pFileName)
{
	// 파일입출력을 통해서 셀을 만들어
	m_vecCell.reserve(10000);

	_tchar			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD	dwByte = 0;

	while (true)
	{
		Engine::NAVIINFO*	pInfo = new Engine::NAVIINFO;
		ReadFile(hFile, pInfo, sizeof(Engine::NAVIINFO), &dwByte, NULL);

		if (dwByte == 0)
		{
			Engine::Safe_Delete(pInfo);
			break;
		}

		Add_Cell(&pInfo->vPoint[0],
			&pInfo->vPoint[1],
			&pInfo->vPoint[2],
			pInfo->m_eType);

		delete pInfo;
		pInfo = NULL;
	}

	if(FAILED(Link_Cell()))
		return E_FAIL;

	m_dwMaxIdx = m_vecCell.size();

	return S_OK;
}

void Engine::CNavigationMesh::Render_NaviCell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		m_vecCell[i]->Render_Cell();
	}
}

_vec3 Engine::CNavigationMesh::Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _float fSpeedRate
, _ulong dwIdx, const _float& fTimeDelta)
{
	_vec3		vEndPos = *pTargetPos + *pTargetDir * fSpeedRate * fTimeDelta;

	if (CCell::COMPARE_MOVE == m_vecCell[dwIdx]->Compare(&vEndPos, &dwIdx))
	{
		return vEndPos;	
	}

	else if(CCell::COMPARE_NOMOVE == m_vecCell[dwIdx]->Compare(&vEndPos, &dwIdx))
	{
		if (m_vecCell[dwIdx]->GetType() == TYPE_NORMAL)
		{
			_vec3 vSlidingPos;

			_vec3 vNormal;
			_vec3 vLineDir;
			_vec3 vSliding;
			_vec3 vUp = _vec3(0.f, 1.f, 0.f);
			_vec3 vPointA = *m_vecCell[dwIdx]->Get_Point(CCell::POINT_A);
			_vec3 vPointB = *m_vecCell[dwIdx]->Get_Point(CCell::POINT_B);
			_vec3 vPointC = *m_vecCell[dwIdx]->Get_Point(CCell::POINT_C);

			if (CCell::LINE_AB == m_vecCell[dwIdx]->GetLineType(&vEndPos, &dwIdx))
			{
				vLineDir = vPointB - vPointA;
				D3DXVec3Cross(&vNormal, &vLineDir, &vUp);
				D3DXVec3Normalize(&vNormal, &vNormal);
				_float fCos = D3DXVec3Dot(&vNormal, pTargetDir);

				if (fCos > 0.f)
				{
					vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &vNormal) * vNormal);
				}
				else
				{
					vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &-vNormal) * -vNormal);
				}
			}

			else if (CCell::LINE_BC == m_vecCell[dwIdx]->GetLineType(&vEndPos, &dwIdx))
			{
				vLineDir = vPointC - vPointB;
				D3DXVec3Cross(&vNormal, &vLineDir, &vUp);
				D3DXVec3Normalize(&vNormal, &vNormal);
				_float fCos = D3DXVec3Dot(&vNormal, pTargetDir);

				if (fCos > 0.f)
				{
					vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &vNormal) * vNormal);
				}
				else
				{
					vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &-vNormal) * -vNormal);
				}
			}

			else	//LINE CA
			{
				vLineDir = vPointA - vPointC;
				D3DXVec3Cross(&vNormal, &vLineDir, &vUp);
				D3DXVec3Normalize(&vNormal, &vNormal);
				_float fCos = D3DXVec3Dot(&vNormal, pTargetDir);

				if (fCos > 0.f)
				{
					vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &vNormal) * vNormal);
				}
				else
				{
					vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &-vNormal) * -vNormal);
				}
			}

			//업벡터와 라인벡터의 외적으로 노멀벡터를 구한다.

			D3DXVec3Normalize(&vSliding, &vSliding);

			vSlidingPos = *pTargetPos + vSliding * fSpeedRate * fTimeDelta * 0.5f;


			return vSlidingPos;
		}
		else
			return vEndPos;
	}

	else
	{
		return *pTargetPos;
	}
}

_vec3 Engine::CNavigationMesh::Move_OnNaviMesh_Tool( const _vec3* pTargetPos, const _vec3* pTargetDir, 
													_float fSpeedRate, _ulong dwIdx)
{
	_vec3		vEndPos;

	vEndPos = *pTargetPos + *pTargetDir * fSpeedRate;

	if(CCell::COMPARE_MOVE == m_vecCell[dwIdx]->Compare(&vEndPos, &dwIdx))
	{
		return vEndPos;
	}

	//맵툴...
	//변 끝에 닿았을때....
	else if(CCell::COMPARE_NOMOVE == m_vecCell[dwIdx]->Compare(&vEndPos, &dwIdx))
	{
		//D3DXVECTOR3 slidingVector = 
		//	moveVec - D3DXVec3Dot( &moveVec, &contactNormal) * contactNormal;
		_vec3 vNormal;
		_vec3 vLineDir;
		_vec3  vSliding;
		_vec3 vUp = _vec3(0.f, 1.f, 0.f);
		_vec3 vPointA = *m_vecCell[dwIdx]->Get_Point(CCell::POINT_A);
		_vec3 vPointB = *m_vecCell[dwIdx]->Get_Point(CCell::POINT_B);
		_vec3 vPointC = *m_vecCell[dwIdx]->Get_Point(CCell::POINT_C);


		if(CCell::LINE_AB == m_vecCell[dwIdx]->GetLineType(&vEndPos, &dwIdx))
		{
			vLineDir = vPointB - vPointA;
			D3DXVec3Cross(&vNormal, &vLineDir, &vUp);
			D3DXVec3Normalize(&vNormal, &vNormal);
			_float fCos = D3DXVec3Dot(&vNormal, pTargetDir);

			if(fCos > 0.f)
			{
				vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &vNormal) * vNormal);
			}
			else
			{
				vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &-vNormal) * -vNormal);
			}
		}

		else if(CCell::LINE_BC == m_vecCell[dwIdx]->GetLineType(&vEndPos, &dwIdx))
		{
			vLineDir = vPointC - vPointB;
			D3DXVec3Cross(&vNormal, &vLineDir, &vUp);
			D3DXVec3Normalize(&vNormal, &vNormal);
			_float fCos = D3DXVec3Dot(&vNormal, pTargetDir);

			if(fCos > 0.f)
			{
				vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &vNormal) * vNormal);
			}
			else
			{
				vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &-vNormal) * -vNormal);
			}
		}

		else	//LINE CA
		{
			vLineDir = vPointA - vPointC;
			D3DXVec3Cross(&vNormal, &vLineDir, &vUp);
			D3DXVec3Normalize(&vNormal, &vNormal);
			_float fCos = D3DXVec3Dot(&vNormal, pTargetDir);

			if(fCos > 0.f)
			{
				vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &vNormal) * vNormal);
			}
			else
			{
				vSliding = *pTargetDir - (D3DXVec3Dot(pTargetDir, &-vNormal) * -vNormal);
			}
		}

		//업벡터와 라인벡터의 외적으로 노멀벡터를 구한다.

		D3DXVec3Normalize(&vSliding, &vSliding);

		_vec3 vSlidingPos = *pTargetPos + vSliding * fSpeedRate;
	


		return vSlidingPos;
	}

	else
	{
		return *pTargetPos;
	}

}

HRESULT Engine::CNavigationMesh::Link_Cell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if(i == j)
				continue;		


			if(m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) == NULL
				&& m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A)
				, m_vecCell[i]->Get_Point(CCell::POINT_B), m_vecCell[i]) == true)
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);

				continue;

			}

			if(m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) == NULL
				&& m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B)
				, m_vecCell[i]->Get_Point(CCell::POINT_C), m_vecCell[i]) == true)
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if(m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) == NULL
				&& m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C)
				, m_vecCell[i]->Get_Point(CCell::POINT_A), m_vecCell[i]) == true)
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;

			}

		}		
	}


	return S_OK;
}


void Engine::CNavigationMesh::Add_Cell( const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC, NAVI_TYPE eType)
{
	CCell*		pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), pPointA, pPointB, pPointC, eType);

	++m_dwCurrentIdx;

	m_vecCell.push_back(pCell);

	m_dwMaxIdx = m_vecCell.size();

}

vector<Engine::CCell*>* Engine::CNavigationMesh::GetVECCELL(void)
{
	return &m_vecCell;
}


void Engine::CNavigationMesh::FindPath( _vec3* pStartPos, _vec3* pEndPos )
{

	//내 셀 인덱스 가져오기.
	m_dwEndIdx = Get_CurrentCellIndex(pStartPos);

	m_dwIdx = m_dwEndIdx;
	//목표 셀 인덱스 가져오기.
	m_dwTargetIdx = Get_CurrentCellIndex(pEndPos);

	if (m_dwTargetIdx == 0)
		return;

	if(m_dwEndIdx == m_dwTargetIdx)
	{
		return;
	}

	_vec3 vTargetPos;

	Release_Path();		//경로 다 지우고..


	//최초의 A스타 노드.
	NODE* pParent = NULL;

	pParent = new NODE;
	pParent->bIsClosed = true;
	pParent->fFCost = 0.f;
	pParent->fHCost = 0.f;
	pParent->fGCost = 0.f;
	pParent->dwIdx = m_dwEndIdx;
	pParent->vPos  = *pStartPos;
	pParent->pParent = NULL;

	m_iCount = 0;

	m_OpenMap.insert( make_pair(pParent->fFCost, pParent));

	//경로만들기..
	while(true)
	{
		//내 셀과 인접한 셀들을 구하자...
		CCell* pCell[CCell::NEIGHBOR_END];
		vector<Engine::CCell*>::iterator iter = m_vecCell.begin();
		vector<Engine::CCell*>::iterator iter_end = m_vecCell.end();

		_vec3 vEndCenter;		//시작셀의 중점...

		for(_uint i = 0; i < CCell::NEIGHBOR_END; ++i)
		{
			pCell[i] = NULL;
		}

		for(iter; iter != iter_end; ++iter)
		{
			if((*iter)->GetIndex() == m_dwIdx)
			{
				vEndCenter = (*iter)->GetCellCenter(); //시작셀의중점
				for(_uint i = 0; i < CCell::NEIGHBOR_END; ++i)
				{
					if(NULL != (*iter)->Get_Neighbor(CCell::NEIGHBOR(i)))
					{
						pCell[i] = (*iter)->Get_Neighbor(CCell::NEIGHBOR(i));
					}
				}
			}
			if((*iter)->GetIndex() == m_dwTargetIdx)
			{
				vTargetPos = (*iter)->GetCellCenter();  //목표셀의중점 
			}
		}
		//인접한 셀들 중 비용이 가장 싼 것을 계산한다..


		for(_uint i = 0; i < CCell::NEIGHBOR_END; ++i)
		{
			if(NULL != pCell[i])
			{
				_vec3 vNextCenter = pCell[i]->GetCellCenter();	//인접한 셀의 센터. . . .
				_vec3 vDelta = vTargetPos - vNextCenter;	//인접한 셀과 목표지점간 거리
				_vec3 vFCost = vNextCenter - vEndCenter;	//인접한 센터와 현재 센터간 거리
				_float fGCost = D3DXVec3Length(&vFCost);	
				_float fHCost = D3DXVec3Length(&vDelta);
				_ulong dwIdx = *pCell[i]->Get_Index();
				if(CheckList(dwIdx))
				{
					Add_Node(pParent, fGCost, fHCost, dwIdx, &vNextCenter);
					m_iCount++;
				}
			}
		}

		multimap<float, NODE*>::iterator iter_Open = m_OpenMap.begin();
		multimap<float, NODE*>::iterator iter_Open_end = m_OpenMap.end();

		for(iter_Open; iter_Open != iter_Open_end; iter_Open++)
		{
			if(iter_Open->second->bIsClosed == false)
				break;
		}

		if(iter_Open == iter_Open_end)
		{
			Release_Path();
			return;
		}

		//m_dwIdx = pParent->dwIdx;
		pParent = iter_Open->second;
		pParent->bIsClosed = true;
		m_dwIdx = iter_Open->second->dwIdx;

		if(pParent->dwIdx == m_dwTargetIdx/* || m_iCount > 20*/)
		{
			while(true)
			{
				m_BestList.push_back(pParent->vPos);
				pParent = pParent->pParent;

				if(pParent->dwIdx == m_dwEndIdx)
					break;
			}

			m_BestList.reverse();
			break;
		}

	}

//////////////////////////////////....Astar.../////////////////////

}


void Engine::CNavigationMesh::Add_Node( NODE* pParent, _float fGCost, _float fHCost ,
									   _ulong dwIdx, _vec3* pPos)
{
	multimap<float, NODE*>::iterator iter = m_OpenMap.begin();
	multimap<float, NODE*>::iterator iter_end = m_OpenMap.end();

	//오픈리스트 안에 있으면 리턴.
// 	for(iter; iter != iter_end; iter++)
// 	{
// 		if(iter->second->bIsClosed == true)
// 			continue;
// 
// 		if(iter->second->dwIdx == dwIdx)
// 		{
// // 			if(iter->second->fGCost < fGCost)
// // 				return;
// // 
// // 			iter->second->fGCost = fGCost;
// // 			iter->second->fFCost = iter->second->fGCost + iter->second->fHCost;
// // 			iter->second->pParent = pParent;
// 			return;	
// 		}
// 	}

	NODE* pNode = NULL;
	pNode = new NODE;
	pNode->pParent = pParent;
	pNode->fGCost = fGCost;
	pNode->fHCost = fHCost;
	pNode->fFCost = pNode->fGCost + pNode->fHCost;
	pNode->dwIdx = dwIdx;
	pNode->vPos  = *pPos;

	m_OpenMap.insert( make_pair(pNode->fFCost, pNode) );

}

_bool Engine::CNavigationMesh::CheckList( _ulong dwIdx )
{
	multimap<float, NODE*>::iterator iter = m_OpenMap.begin();
	multimap<float, NODE*>::iterator iter_end = m_OpenMap.end();

	for(iter; iter != iter_end; iter++)
	{
		if(iter->second->dwIdx == dwIdx)
			if(iter->second->bIsClosed)
				return false;
	}
	return true;
}

_vec3 Engine::CNavigationMesh::AStarMove( _vec3* pPos )
{
	if(m_BestList.empty())
	{
		return *pPos;
	}

	//그냥 셀의 중점으로 이동
	_vec3 vPos = *pPos;
	list<_vec3>::iterator iter = m_BestList.begin();
	list<_vec3>::iterator iter_end = m_BestList.end();

	_vec3 vNextPos = *iter;

	m_vAstarDir = vNextPos - vPos;
	_float fDistance = D3DXVec3Length(&m_vAstarDir);

	if(fDistance < 2.f)
	{
		m_BestList.pop_front();
	}


	D3DXVec3Normalize(&m_vAstarDir, &m_vAstarDir);

	*pPos += m_vAstarDir * 0.1f;

	return *pPos;


}

void Engine::CNavigationMesh::Release_Path( void )
{
	multimap<float, Engine::NODE*>::iterator iter = m_OpenMap.begin();
	multimap<float, Engine::NODE*>::iterator iter_end = m_OpenMap.end();

	for(iter; iter != iter_end; iter++)
	{
		delete (*iter).second;
		(*iter).second = NULL;
	}
	m_OpenMap.clear();

	m_BestList.clear();
}

CNavigationMesh* Engine::CNavigationMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CNavigationMesh* pInstance = new CNavigationMesh(pGraphicDev);

	if(FAILED(pInstance->Ready_NavigationMesh(pFilePath, pFileName)))
	{
		MSG_BOX("CNavigationMesh Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CResource* Engine::CNavigationMesh::Clone(void)
{
	return new CNavigationMesh(*this);
}

void Engine::CNavigationMesh::Free(void)
{
	Release_Path();

	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		Engine::Safe_Release(m_vecCell[i]);
	}
	m_vecCell.clear();

	Engine::CMesh::Free();

}

_ulong Engine::CNavigationMesh::Get_MaxCellIndex( void )
{
	m_dwMaxIdx = m_vecCell.size();
	return m_dwMaxIdx;
}

_bool Engine::CNavigationMesh::Jump_Check_NaviSide( const _vec3* pTargetPos, _ulong dwIdx )
{
	//맵툴...
	//변 끝에 닿았을때....
	if (m_vecCell.size() == 0)
	{
		return true;
	}

	if (m_vecCell[dwIdx]->GetType() == TYPE_FALL)
	{
		if (CCell::COMPARE_NOMOVE == m_vecCell[dwIdx]->Compare(pTargetPos, &dwIdx))
		{
			return true;

			if (CCell::LINE_AB == m_vecCell[dwIdx]->GetLineType(pTargetPos, &dwIdx))
			{
				//점프할곳인지 확인...
				if (NULL == m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_AB))
				{
					return true;
				}
			}

			else if (CCell::LINE_BC == m_vecCell[dwIdx]->GetLineType(pTargetPos, &dwIdx))
			{
				////점프할곳인지 확인...
				if (NULL == m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_BC))
				{
					return true;
				}
			}

			else	//LINE CA
			{
				//점프할곳인지 확인...
				if (NULL == m_vecCell[dwIdx]->Get_Neighbor(CCell::NEIGHBOR_CA))
				{
					return true;
				}
			}
		//	return true;
		}
	}


	return false;
}

_vec3* Engine::CNavigationMesh::GetAStarDir( void )
{
	return &m_vAstarDir;
}


