#include "stdafx.h"
#include "AI_Trace_Hanuman.h"
#include "Map_Ice.h"
#include "NavigationMesh.h"
#include "AstarManager.h"
#include "AI.h"

//클라..

CAI_Trace_Hanuman::CAI_Trace_Hanuman(void)
	:CAI_Hanuman()
	, m_fCompareTime(0.f)
	, m_bCompareDis(false)
	, m_dwIdx(0)
{
}

CAI_Trace_Hanuman::~CAI_Trace_Hanuman(void)
{
}


HRESULT CAI_Trace_Hanuman::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI)
{
	if (pTransform == NULL || pAnimator == NULL || pNaviMesh == NULL)
		return E_FAIL;

	m_pTransform = pTransform;
	m_pTransform->AddRef();

	m_pAnimator = pAnimator;
	m_pAnimator->AddRef();

	m_pNavigationMesh = pNaviMesh;

	m_pCalculator = pCalculator;
	m_pCalculator->AddRef();

	m_pRigidbody = pRigidbody;
	m_pRigidbody->AddRef();

	m_pCollider = pCollider;

	m_pColInfo = pColInfo;

	m_pDamaged = pDamaged;

	m_pAI = pAI;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
	if (pGameObject == NULL)
		return E_FAIL;

	if (dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);
		//레이를 위한 맵 메쉬 세팅...
		for (_int i = 0; i < iSize; ++i)
		{
			Set_MapMesh(ppMesh[i]->Get_Mesh());
		}
	}
	Engine::Safe_Release(pGameObject);


	pGameObject = Engine::FindWithTag(L"Player");
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	Engine::Safe_Release(pGameObject);

	m_fMonsterSpeed = 20.f;

	m_bRayTime = true;



	return S_OK;
}

void CAI_Trace_Hanuman::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_pCubeList = Engine::FindList(L"Cube");
	m_bCompareDis = true;
	m_bDamaged = false;
	m_fCompareTime = 0.f;

}

void CAI_Trace_Hanuman::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();

	m_pAnimator->Set_AnimationSet(MOVE_RUN);

	m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
	m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vRayDir, m_fMonsterSpeed, m_dwIdx, fTimeDelta));
	m_vPos = m_pTransform->Get_Now_Position();

	LookAtDir(&m_vRayDir, fTimeDelta);

	if (m_fPlayerDis < 15.f)
	{
		int iRand = rand() % 4;

		if (iRand == 0)
		{
			if (m_pAnimator->Set_AnimationSet(ATTACK1))
			{
				m_pAI->Set_State(AI_ATTACK1);
			}
		}
		if (iRand == 1)
		{
			if (m_pAnimator->Set_AnimationSet(COMBO0))
			{
				m_pAI->Set_State(AI_COMBO);
			}
		}
		if (iRand == 2)
		{
			if (m_pAnimator->Set_AnimationSet(ATTACK2))
			{
				m_pAI->Set_State(AI_ATTACK2);
			}
		}
		if (iRand == 3)
		{
			if (m_pAnimator->Set_AnimationSet(ROAR))
			{
				m_pAI->Set_State(AI_ROAR);
			}
		}
	}

	PlayerRayCheck();

	ChangeAni_Work();

	//AI바뀌는곳.
	ChangeAI_Work();

}

void CAI_Trace_Hanuman::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

//void CAI_Trace_Hanuman::CubeRayCheck(void)
//{
//	if (NULL != Engine::FindList(L"Cube"))
//	{
//		//	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Cube")->begin();
//			//list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Cube")->end();
//		list<Engine::CGameObject*>::iterator iter = m_pCubeList->begin();
//		list<Engine::CGameObject*>::iterator iter_end = m_pCubeList->end();
//		//			_float	fU, fV, fDist;
//		_float fDist = 99999.f;
//		_float rDist = 99999.f;
//		BOOL BHit = false;
//		_vec3 vDis;
//		for (iter; iter != iter_end; ++iter)
//		{
//			((CCube*)(*iter))->CheckRayCol(&fDist, &m_vPivotPos, &m_vRayDir, rDist);
//		}
//		//플레이어와의 거리 비교..
//		if (fDist < m_fPlayerDis)
//		{
//			m_bCubeRayCol = true;
//		}
//		else
//		{
//			m_bCubeRayCol = false;
//		}
//	}
//}

void CAI_Trace_Hanuman::TraceMove(const _float& fTimeDelta)
{


}

_vec3 CAI_Trace_Hanuman::AStarMove(_vec3* pPos, const _float &fTimeDelta)
{
	if (m_BestList.empty())
	{
		return *pPos;
	}

	//그냥 셀의 중점으로 이동
	_vec3 vPos = *pPos;
	list<_vec3>::iterator iter = m_BestList.begin();
	list<_vec3>::iterator iter_end = m_BestList.end();

	_vec3 vNextPos = *iter;

	m_vAStarDir = vNextPos - vPos;
	_float fDistance = D3DXVec3Length(&m_vAStarDir);

	if (fDistance < 2.f)
	{
		m_BestList.pop_front();
	}


	D3DXVec3Normalize(&m_vAStarDir, &m_vAStarDir);

	*pPos += m_vAStarDir * fTimeDelta * m_fMonsterSpeed;

	return *pPos;
}

void CAI_Trace_Hanuman::Collsion_Work(void)
{
	if (*m_pDamaged)
	{
		switch (m_pColInfo->eColInfo)
		{
		case Engine::COL_BASE:

			break;
		case Engine::COL_PUSH:

			break;
		case Engine::COL_SMASH:
			//	m_pAnimator->Set_AnimationSet(DAMAGE);
			break;
		case Engine::COL_FLY:

			break;
		case Engine::COL_SPINA:

			break;
		case Engine::COL_SPINB:
			m_bCol[Engine::COL_SPINB] = true;
			//	m_pAnimator->Set_AnimationSet(DAMAGE);

			break;
		case Engine::COL_STAMP:
			//	m_pAnimator->Set_AnimationSet(DAMAGE);

			break;
		case Engine::COL_STUN:
			m_pAnimator->Set_AnimationSet(STUN);
		case Engine::COL_CAPTURE:
			break;
		}
	}

}


CAI_Trace_Hanuman * CAI_Trace_Hanuman::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI)
{
	CAI_Trace_Hanuman* pInstance = new CAI_Trace_Hanuman;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI)))
	{
		MSG_BOX("CAI_Trace_Hanuman Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Trace_Hanuman::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
