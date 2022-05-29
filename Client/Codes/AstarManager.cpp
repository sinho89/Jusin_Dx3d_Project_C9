#include "stdafx.h"
#include "AStarManager.h"
#include "Cell.h"

IMPLEMENT_SINGLETON(CAStarManager)

CAStarManager::CAStarManager(void)
	:m_pvecCell(NULL)
	, m_dwStartIndex(0)
	, m_dwEndIndex(0)
	, m_dwIndex(0)
	, m_iCount(0)
	, m_iLoop(0)
	, m_bWait(false)
	,m_hThread(NULL)
	,m_pBestList(NULL)

{
	ZeroMemory(&m_Crt, sizeof(CRITICAL_SECTION));
}

CAStarManager::~CAStarManager(void)
{

}

unsigned int APIENTRY CAStarManager::Thread_Main(void* pArg)
{
	CAStarManager* pAStarMgr = (CAStarManager*)pArg;

	_uint		iFlag = 0;
	EnterCriticalSection(pAStarMgr->Get_CriticalSection());

	pAStarMgr->FindPath();
	pAStarMgr->m_bWait = false;

	LeaveCriticalSection(pAStarMgr->Get_CriticalSection());

	pAStarMgr->Complete_Thread();
	return iFlag;
}

void CAStarManager::Complete_Thread(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}

void CAStarManager::Ready_AStarMgr(vector<Engine::CCell*>* pVecCell
	, list<_vec3>* pBestList
	, _vec3* pStartPos
	, _vec3* pEndPos
	, _ulong dwStartIndex
	, _ulong dwEndIndex)
{
	if(m_bWait)
		return;


	//if (dwEndIndex == 0)
	//{
	//	m_bWait = false;
	//	return;
	//}

	//if (dwEndIndex == dwStartIndex)
	//{
	//	m_bWait = false;
	//	return;
	//}					
	m_pvecCell = NULL;
	m_pvecCell = pVecCell;
	m_pBestList = NULL;
	m_pBestList = pBestList;
	m_vStartPos = *pStartPos;
	m_vEndPos = *pEndPos;

	m_dwStartIndex = Get_CurrentIndex(&m_vStartPos);
	m_dwEndIndex = Get_CurrentIndex(&m_vEndPos);

	//m_dwStartIndex	= dwStartIndex;
	//m_dwEndIndex	= dwEndIndex;
	m_iCount = 0;
	//	m_pBestList->clear();
	m_iLoop = 1;
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);
	//	m_bWait = false;

}

_ulong CAStarManager::Get_CurrentIndex(_vec3 * pPos)
{
	_vec3 vPoint[3];

	vector<Engine::CCell*>	::iterator iter = (*m_pvecCell).begin();
	vector<Engine::CCell*>::iterator iter_end = (*m_pvecCell).end();

	for (iter; iter != iter_end; ++iter)
	{
		_bool bCheck[3];
		ZeroMemory(bCheck, sizeof(bool) * 3);

		//���� �� �������� ������...
		for (int i = 0; i < 3; ++i)
		{
			vPoint[i] = *(*iter)->Get_Point((Engine::CCell::POINT)i);
		}

		//������ ���ϰ�..
		_vec3 vDirection[3] =
		{
			vPoint[1] - vPoint[0],
			vPoint[2] - vPoint[1],
			vPoint[0] - vPoint[2],
		};

		//������ �������͸� ���ؼ�..
		_vec3 vNormal[3] =
		{
			_vec3(-vDirection[0].z, 0.f, vDirection[0].x),
			_vec3(-vDirection[1].z, 0.f, vDirection[1].x),
			_vec3(-vDirection[2].z, 0.f, vDirection[2].x),
		};

		//���� ���ͷ� �������...
		for (int i = 0; i < 3; ++i)
			D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

		for (int i = 0; i < 3; ++i)
		{
			//���� ���� ��ġ�� �� ���������� ���⺤�͸����ؼ�..
			D3DXVECTOR3	vDestDir = *pPos - vPoint[i];

			//�� �������� �������Ϳ� ���� ��ġ�� �������ؼ� �������� ���������� �Ǵ���.
			float fDotResult = D3DXVec3Dot(&vDestDir, &vNormal[i]);

			if (fDotResult > 0.f)
			{
				bCheck[i] = false;
			}
			else
			{
				bCheck[i] = true;
			}
		}
		//�׷��� ���� �����̸� �ﰢ�� �ȿ� ���°��̴�...
		if (bCheck[0] && bCheck[1] && bCheck[2])
		{
			float fCurrentY = (*pPos).y;
			float fMaxY = max(max(vPoint[0].y, vPoint[1].y), vPoint[2].y) + 3.f;
			float fMinY = min(min(vPoint[0].y, vPoint[1].y), vPoint[2].y) - 3.f;
			//y���� �Ǵ��ؾ���...
			if (fCurrentY > fMinY && fCurrentY < fMaxY)
			{
				//m_dwCurrentIdx = ((CCell*)(*iter))->GetIndex();
				return ((Engine::CCell*)(*iter))->GetIndex();
			}
		}
	}

	//��ġ�� �� �ȿ� ������� �ʴٸ�
	//	Get_CurrentCellIndex(pPos);
	return 0;
}

void CAStarManager::FindPath(void)
{
	//��Ƽ������ �ѹ��� �ѳ� ���� ����
	m_bWait = true;

	if (m_dwStartIndex == 0)
	{
		m_bWait = false;
		return;
	}

	if (m_dwStartIndex == m_dwEndIndex)
	{
		m_bWait = false;
		return;
	}

	m_dwIndex = m_dwStartIndex;

	_vec3 vTargetPos;

	Release_Path();		//��� �� �����..


						//������ A��Ÿ ���.
	Engine::NODE* pParent = NULL;

	pParent = new Engine::NODE;
	pParent->bIsClosed = true;
	pParent->fFCost = 0.f;
	pParent->fHCost = 0.f;
	pParent->fGCost = 0.f;
	pParent->dwIdx = m_dwStartIndex;
	pParent->vPos = m_vStartPos;
	pParent->pParent = NULL;
	++m_iCount;

	m_OpenMap.insert(make_pair(pParent->fFCost, pParent));

	//��θ����..
	while (true)
	{
		if (m_iLoop == 0)
			break;
		//�� ���� ������ ������ ������...
		Engine::CCell* pCell[Engine::CCell::NEIGHBOR_END];
		vector<Engine::CCell*>::iterator iter = (*m_pvecCell).begin();
		vector<Engine::CCell*>::iterator iter_end = (*m_pvecCell).end();

		_vec3 vEndCenter;		//���ۼ��� ����...

		for (_uint i = 0; i < Engine::CCell::NEIGHBOR_END; ++i)
		{
			pCell[i] = NULL;
		}

		for (iter; iter != iter_end; ++iter)
		{
			if ((*iter)->GetIndex() == m_dwIndex)
			{
				vEndCenter = (*iter)->GetCellCenter(); //���ۼ�������
				for (_uint i = 0; i < Engine::CCell::NEIGHBOR_END; ++i)
				{
					if (NULL != (*iter)->Get_Neighbor(Engine::CCell::NEIGHBOR(i)))
					{
						pCell[i] = (*iter)->Get_Neighbor(Engine::CCell::NEIGHBOR(i));
					}
				}
			}
			if ((*iter)->GetIndex() == m_dwEndIndex)
			{
				vTargetPos = (*iter)->GetCellCenter();  //��ǥ�������� 
			}
		}
		//������ ���� �� ����� ���� �� ���� ����Ѵ�..


		for (_uint i = 0; i < Engine::CCell::NEIGHBOR_END; ++i)
		{
			if (NULL != pCell[i])
			{
				_vec3 vNextCenter = pCell[i]->GetCellCenter();	//������ ���� ����. . . .
				_vec3 vDelta = vTargetPos - vNextCenter;	//������ ���� ��ǥ������ �Ÿ�
				_vec3 vFCost = vNextCenter - vEndCenter;	//������ ���Ϳ� ���� ���Ͱ� �Ÿ�
				_float fGCost = D3DXVec3Length(&vFCost);
				_float fHCost = D3DXVec3Length(&vDelta);
				_ulong dwIdx = *pCell[i]->Get_Index();
				if (CheckList(dwIdx))
				{
					Add_Node(pParent, fGCost, fHCost, dwIdx, &vNextCenter);
					++m_iCount;
				}
			}
		}

		multimap<float, Engine::NODE*>::iterator iter_Open = m_OpenMap.begin();
		multimap<float, Engine::NODE*>::iterator iter_Open_end = m_OpenMap.end();

		for (iter_Open; iter_Open != iter_Open_end; iter_Open++)
		{
			if (iter_Open->second->bIsClosed == false)
				break;
		}

		if (iter_Open == iter_Open_end)
		{
			Release_Path();
			m_bWait = false;
			return;
		}

		//m_dwIdx = pParent->dwIdx;
		pParent = iter_Open->second;
		pParent->bIsClosed = true;
		m_dwIndex = iter_Open->second->dwIdx;

		if (pParent->dwIdx == m_dwEndIndex || m_iCount > 20)
		{
			while (true)
			{
				(*m_pBestList).push_back(pParent->vPos);
				pParent = pParent->pParent;

				if (pParent->dwIdx == m_dwStartIndex)
					break;
			}

			(*m_pBestList).reverse();
			m_bWait = false;

			break;
		}

	}
	m_bWait = false;

	//////////////////////////////////....Astar.../////////////////////

}

void CAStarManager::Release_Path(void)
{
	multimap<float, Engine::NODE*>::iterator iter = m_OpenMap.begin();
	multimap<float, Engine::NODE*>::iterator iter_end = m_OpenMap.end();

	for (iter; iter != iter_end; iter++)
	{
		delete (*iter).second;
		(*iter).second = NULL;
	}
	m_OpenMap.clear();

	if (m_pBestList != NULL)
		m_pBestList->clear();
}

void CAStarManager::Add_Node(Engine::NODE * pParent, _float fGCost, _float fHCost, _ulong dwIndex, _vec3 * pPos)
{
	multimap<float, Engine::NODE*>::iterator iter = m_OpenMap.begin();
	multimap<float, Engine::NODE*>::iterator iter_end = m_OpenMap.end();

	//���¸���Ʈ �ȿ� ������ ����.
	//for (iter; iter != iter_end; iter++)
	//{
	//	if (iter->second->bIsClosed == true)
	//		continue;

	//	if (iter->second->dwIdx == dwIndex)
	//	{
	//		if (iter->second->fGCost < fGCost)
	//			return;

	//		iter->second->fGCost = fGCost;
	//		iter->second->fFCost = iter->second->fGCost + iter->second->fHCost;
	//		iter->second->pParent = pParent;
	//		return;
	//	}
	//}

	Engine::NODE* pNode = NULL;
	pNode = new Engine::NODE;
	pNode->pParent = pParent;
	pNode->fGCost = fGCost;
	pNode->fHCost = fHCost;
	pNode->fFCost = pNode->fGCost + pNode->fHCost;
	pNode->dwIdx = dwIndex;
	pNode->vPos = *pPos;

	m_OpenMap.insert(make_pair(pNode->fFCost, pNode));
}

_bool CAStarManager::CheckList(_ulong dwIndex)
{
	multimap<float, Engine::NODE*>::iterator iter = m_OpenMap.begin();
	multimap<float, Engine::NODE*>::iterator iter_end = m_OpenMap.end();

	for (iter; iter != iter_end; iter++)
	{
		if (iter->second->dwIdx == dwIndex)
			if (iter->second->bIsClosed)
				return false;
	}
	return true;
}

void CAStarManager::StopThread(void)
{
	m_iLoop = 0;
	//WaitForSingleObject(m_hThread, INFINITE);
}

void CAStarManager::Free(void)
{
	/*Release_Path();*/
	//WaitForSingleObject(m_hThread, INFINITE);
	multimap<float, Engine::NODE*>::iterator iter = m_OpenMap.begin();
	multimap<float, Engine::NODE*>::iterator iter_end = m_OpenMap.end();

	for (iter; iter != iter_end; iter++)
	{
		delete (*iter).second;
		(*iter).second = NULL;
	}
	m_OpenMap.clear();

	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}
