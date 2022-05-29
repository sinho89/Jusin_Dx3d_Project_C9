#include "stdafx.h"
#include "AI_Combat2_Bardiel.h"
#include "Map_Fire.h"
#include "NavigationMesh.h"
#include "AstarManager.h"
#include "AI.h"

//클라..

CAI_Combat2_Bardiel::CAI_Combat2_Bardiel(void)
	:CAI_Bardiel()
	, m_fChangeTime(0.f)
	, m_bChange(false)
	, m_bLRMove(false)
	, m_iPatternCount(0)
{
}

CAI_Combat2_Bardiel::~CAI_Combat2_Bardiel(void)
{
}


HRESULT CAI_Combat2_Bardiel::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI)
{
	if (pTransform == NULL || pAnimator == NULL )
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

	if (dynamic_cast<CMap_Fire*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);
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

void CAI_Combat2_Bardiel::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);

	m_iDirection = rand() % 2;
	//m_pCubeList = Engine::FindList(L"Cube");
	m_bDamaged = false;
	m_bChange = false;
	m_fChangeTime = 0.f;
	m_bLRMove = false;

}

void CAI_Combat2_Bardiel::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL)
		return;

	Set_Ray();

	//PlayerRayCheck();

	LookAtDir(&m_vRayDir, fTimeDelta);

	if (!m_bChange)
	{
		m_fChangeTime += fTimeDelta;
		if (m_fChangeTime > 2.f)
		{
			m_bChange = true;
			m_fChangeTime = 0.f;
		}
	}

	if (!m_bDamaged)
	{
		if (m_fPlayerDis >= 30.f)
		{
			m_pAnimator->Set_AnimationSet(RUN_WEAPON);
			_vec3 vEnd = m_vPos + m_vLook * fTimeDelta * m_fMonsterSpeed;
			m_pTransform->Set_Position(&vEnd);
		}
		if (m_fPlayerDis > 25.f && m_fPlayerDis < 30.f)
		{
			m_pAnimator->Set_AnimationSet(WALK_F_2);
			_vec3 vEnd = m_vPos + m_vLook * fTimeDelta * m_fMonsterSpeed * 0.5f;
			m_pTransform->Set_Position(&vEnd);
		}


		if (m_bChange)
		{
			if (m_fPlayerDis < 30.f)
			{
				//패턴 고정시키기===========
				m_iPatternCount = rand() % 8;
				//commit할떄 주석 걸도록 하자.
				if (m_iPatternCount == 0)
				{
					if (m_pAnimator->Set_AnimationSet(ATT_WHIRLWIND))
					{
						m_pAI->Set_State(AI_WHIRLWIND);
						++m_iPatternCount;
						return;
					}
				}
				else if (m_iPatternCount == 1)
				{
					if (m_pAnimator->Set_AnimationSet(ATT_VOLCANO))
					{
						m_pAI->Set_State(AI_VOLCANO);
						++m_iPatternCount;
						return;
					}
				}
				else if (m_iPatternCount == 2)
				{
					if (m_pAnimator->Set_AnimationSet(ATT_SHORTHAMMER))
					{
						m_pAI->Set_State(AI_SHORTHAMMER);
						++m_iPatternCount;
						return;
					}
				}
				else if (m_iPatternCount == 3)
				{
					if (m_pAnimator->Set_AnimationSet(ATT_LFOOT))
					{
						m_pAI->Set_State(AI_LFOOT);
						++m_iPatternCount;
						return;
					}
				}
				else if (m_iPatternCount == 4)
				{
					if (m_pAnimator->Set_AnimationSet(ATT_HAMMER))
					{
						m_pAI->Set_State(AI_HAMMER);
						++m_iPatternCount;
						return;
					}
				}
				else if (m_iPatternCount == 5)
				{
					if (m_pAnimator->Set_AnimationSet(ATT_FIRE_WALL))
					{
						m_pAI->Set_State(AI_FIREWALL);
						++m_iPatternCount;
						return;
					}
				}
				else if (m_iPatternCount == 6)
				{
					if (m_pAnimator->Set_AnimationSet(ATT_DIVIDE))
					{
						m_pAI->Set_State(AI_DIVIDE);
						++m_iPatternCount;
						return;
					}
				}
				else if (m_iPatternCount == 7)
				{
					if (m_pAnimator->Set_AnimationSet(ATT_CLEAN))
					{
						m_pAI->Set_State(AI_CLEAN);
						++m_iPatternCount;
						return;
					}
				}
				else
				{
					int iRand = rand() % 8;
					if (iRand == 0)
					{
						if (m_pAnimator->Set_AnimationSet(ATT_WHIRLWIND))
						{
							m_pAI->Set_State(AI_WHIRLWIND);
							++m_iPatternCount;
							return;
						}
					}
					else if (iRand == 1)
					{
						if (m_pAnimator->Set_AnimationSet(ATT_VOLCANO))
						{
							m_pAI->Set_State(AI_VOLCANO);
							++m_iPatternCount;
							return;
						}
					}
					else if (iRand == 2)
					{
						if (m_pAnimator->Set_AnimationSet(ATT_SHORTHAMMER))
						{
							m_pAI->Set_State(AI_SHORTHAMMER);
							++m_iPatternCount;
							return;
						}
					}
					else if (iRand == 3)
					{
						if (m_pAnimator->Set_AnimationSet(ATT_LFOOT))
						{
							m_pAI->Set_State(AI_LFOOT);
							++m_iPatternCount;
							return;
						}
					}
					else if (iRand == 4)
					{
						if (m_pAnimator->Set_AnimationSet(ATT_HAMMER))
						{
							m_pAI->Set_State(AI_HAMMER);
							++m_iPatternCount;
							return;
						}
					}
					else if (iRand == 5)
					{
						if (m_pAnimator->Set_AnimationSet(ATT_FIRE_WALL))
						{
							m_pAI->Set_State(AI_FIREWALL);
							++m_iPatternCount;
							return;
						}
					}
					else if (iRand == 6)
					{
						if (m_pAnimator->Set_AnimationSet(ATT_DIVIDE))
						{
							m_pAI->Set_State(AI_DIVIDE);
							++m_iPatternCount;
							return;
						}
					}
					else if (iRand == 7)
					{
						if (m_pAnimator->Set_AnimationSet(ATT_CLEAN))
						{
							m_pAI->Set_State(AI_CLEAN);
							++m_iPatternCount;
							return;
						}
					}
				}
			}
		}

		PlayerRayCheck();
		//CubeRayCheck();

		//Collsion_Work();

		ChangeAni_Work();

		//AI바뀌는곳.
		ChangeAI_Work();

	}
}

void CAI_Combat2_Bardiel::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

//void CAI_Combat2_Bardiel::CubeRayCheck(void)
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

void CAI_Combat2_Bardiel::TraceMove(const _float& fTimeDelta)
{

	//	if (m_bRayMove)
		//{
		//	m_pAnimator->Set_AnimationSet(MOVE_RUN);

		//	m_vPos = m_vPos + m_vLook * fTimeDelta * m_fMonsterSpeed;
		//	m_pTransform->Set_Position(&m_vPos);

		//	if (!m_bJump && m_pCalculator->Get_IsCol())
		//	{
		//		m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
		//		m_bJump = m_pNavigationMesh->Jump_Check_NaviSide(&m_vPos, m_dwIdx);
		//	}

		//	//LookAtDir(&m_vRayDir, fTimeDelta);

		//}
		//else // (m_bAStarMove)
		//{

		//	m_pAnimator->Set_AnimationSet(MOVE_RUN);
		//	m_pTransform->Set_Position(&AStarMove(&m_vPos, fTimeDelta));

		//	LookAtDir(&m_vAStarDir, fTimeDelta);
		//}
}

void CAI_Combat2_Bardiel::Collsion_Work(void)
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
			//	m_pAnimator->Set_AnimationSet(STUN);
		case Engine::COL_CAPTURE:
			break;
		}
	}

}


CAI_Combat2_Bardiel * CAI_Combat2_Bardiel::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI)
{
	CAI_Combat2_Bardiel* pInstance = new CAI_Combat2_Bardiel;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI)))
	{
		MSG_BOX("CAI_Combat2_Bardiel Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Combat2_Bardiel::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
