#include "stdafx.h"
#include "AI_Trace_BWarrior.h"
#include "Map_Ice.h"
#include "NavigationMesh.h"
#include "AstarManager.h"
#include "AI.h"

//클라..

CAI_Trace_BWarrior::CAI_Trace_BWarrior(void)
	:CAI_Babegazi_Warrior()
	, m_fCompareTime(0.f)
	, m_bCompareDis(false)
	, m_dwIdx(0)
{
}

CAI_Trace_BWarrior::~CAI_Trace_BWarrior(void)
{
}


HRESULT CAI_Trace_BWarrior::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
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

	m_fMonsterSpeed = 15.f;

	m_bRayTime = true;



	return S_OK;
}

void CAI_Trace_BWarrior::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_pCubeList = Engine::FindList(L"Cube");
	m_bCompareDis = true;
	m_bDamaged = false;
	m_fCompareTime = 0.f;
	m_bStop = false;
}

void CAI_Trace_BWarrior::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();

	//AStar와 레이무빙 비교 텀..
	if (!m_bCompareDis)
	{
		m_fCompareTime += fTimeDelta;

		if (m_fCompareTime > 1.5f)
		{
			m_bCompareDis = true;
			m_fCompareTime = 0.f;
		}
	}

	if (m_bStop)
	{
		m_pAnimator->Set_AnimationSet(BATTLE_IDLE);
		m_fStopTime += fTimeDelta;
		if (m_fStopTime > 1.5f)
		{
			m_fStopTime = 0.f;
			m_bStop = false;
		}
	}

	PlayerRayCheck();
	CubeRayCheck();


	if (!m_bDamaged)
	{
		if (m_bCompareDis)
		{
			if (m_fRayDis > m_fPlayerDis)
			{
				if (!m_bCubeRayCol)
				{
					m_bRayMove = true;
					m_bAStarMove = false;
				}
				else
				{
					//A스타 이동....
					m_bAStarMove = true;
					m_bRayMove = false;
					_ulong dwStartIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
					_ulong dwEndIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vTargetPos);
					CAStarManager::GetInstance()->Ready_AStarMgr(m_pNavigationMesh->GetVECCELL(), &m_BestList, &m_vPos, &m_vPlayerPos, dwStartIdx, dwEndIdx);
				//	m_pAnimator->Set_AnimationSet(MOVE_RUN);
				}
			}

			else
			{
				//짧더라도 장애물이 "바닥"일 경우!
				if (m_vTargetPos.y < m_vPos.y + 1.f)
				{
					if (!m_bCubeRayCol)
					{
					//	m_pAnimator->Set_AnimationSet(MOVE_RUN);
						m_bRayMove = true;
						m_bAStarMove = false;
					}
					else //m_bCubeRayCol == true
					{
						m_bRayMove = false;
						m_bAStarMove = true;
						//A스타 이동....
						_ulong dwStartIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
						_ulong dwEndIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vTargetPos);
						CAStarManager::GetInstance()->Ready_AStarMgr(m_pNavigationMesh->GetVECCELL(), &m_BestList, &m_vPos, &m_vPlayerPos, dwStartIdx, dwEndIdx);
					}
				}
				//장애물이 바닥이 아니고 벽이라면...
				else
				{
					m_bRayMove = false;
					m_bAStarMove = true;
					//A스타 이동....
					_ulong dwStartIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
					_ulong dwEndIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vTargetPos);
					CAStarManager::GetInstance()->Ready_AStarMgr(m_pNavigationMesh->GetVECCELL(), &m_BestList, &m_vPos, &m_vPlayerPos, dwStartIdx, dwEndIdx);
				//	m_pAnimator->Set_AnimationSet(MOVE_RUN);
				}
			}

			m_bCompareDis = false;
		}

		if (!m_bJump && !m_bJumpMove && !m_bStop)
		{
			TraceMove(fTimeDelta);
		}



		if (m_bJump && !m_bJumpMove)
		{
			if (m_pAnimator->Set_AnimationSet(JUMP_A))
			{
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(0.7f);
				m_bJumpMove = true;
			}
		}

		else if (m_bJump && m_bJumpMove)
		{
			//레이방향으로 이동...
			_vec3 vPos = *m_pTransform->Get_Infomation(Engine::INFO_POSITION, &vPos);
			D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
			_vec3 vEndPos = vPos + m_vRayDir * fTimeDelta * m_fMonsterSpeed * 2.f;
			m_pTransform->Set_Position(&vEndPos);

			if (!m_pCalculator->Get_IsCol())
			{
				if (m_pAnimator->Is_AnimationEnd(JUMP_A, 0.1f))
				{
					m_pAnimator->Set_AnimationSet(JUMP_B);
				}
				if (m_pAnimator->Is_AnimationEnd(JUMP_B, 0.1f))
				{
					m_pAnimator->Set_AnimationSet(JUMP_B);
				}
			}

			if (m_pCalculator->Get_IsCol())
			{
				if (m_pAnimator->Get_AnimationIndex() == JUMP_A)
				{
					m_pAnimator->Set_AnimationSet(JUMP_C);
				}
				if (m_pAnimator->Get_AnimationIndex() == JUMP_B)
				{
					m_pAnimator->Set_AnimationSet(JUMP_C);
				}
				if (m_pAnimator->Get_AnimationIndex() == MOVE_RUN)
				{
					m_pAnimator->Set_AnimationSet(JUMP_C);
				}

				m_bJump = false;
				m_bJumpMove = false;
				m_bStop = true;
			}
		}

		if (m_fPlayerDis < 14.f)
		{
			if (!m_bJump && !m_bJumpMove)
			{
				m_pAI->Set_State(AI_COMBAT);
			}
		}
	}

	///////////////////////////////
	if (m_bSpinB)
	{
		m_pTransform->Rotation(Engine::ROT_X, -700.f * fTimeDelta);
	}
	else
	{
		//찍기 와이값..돌리기
		_vec3 vAngle = m_pTransform->Get_Now_Angle();
		vAngle.x = 0.f;
		m_pTransform->Set_Angle(&vAngle);
	}
	////////////////////////////////

	Collsion_Work();

	ChangeAni_Work();

	//AI바뀌는곳.
	ChangeAI_Work();

}

void CAI_Trace_BWarrior::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

//void CAI_Trace_BWarrior::CubeRayCheck(void)
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

void CAI_Trace_BWarrior::TraceMove(const _float& fTimeDelta)
{

	if (m_bRayMove)
	{
		m_pAnimator->Set_AnimationSet(MOVE_RUN);
		m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);

		if (!m_bJump && m_pCalculator->Get_IsCol())
		{
			m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vRayDir, m_fMonsterSpeed, m_dwIdx, fTimeDelta));
			m_vPos = m_pTransform->Get_Now_Position();
			m_bJump = m_pNavigationMesh->Jump_Check_NaviSide(&m_vPos, m_dwIdx);

		}

		LookAtDir(&m_vRayDir, fTimeDelta);
	}
	else // (m_bAStarMove)
	{
		m_pAnimator->Set_AnimationSet(MOVE_RUN);
		m_pTransform->Set_Position(&AStarMove(&m_vPos, fTimeDelta));

		LookAtDir(&m_vAStarDir, fTimeDelta);
	}
}

_vec3 CAI_Trace_BWarrior::AStarMove(_vec3* pPos, const _float &fTimeDelta)
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

void CAI_Trace_BWarrior::Collsion_Work(void)
{

	if (*m_pDamaged)
	{
		//스탠드업상태에서 밟으면 빠지는 것 예외처리
		if (!m_pCalculator->Get_IsCol())
		{
			if (m_pAnimator->Get_AnimationIndex() == DAM_UPPER_A
				|| m_pAnimator->Get_AnimationIndex() == DAM_UPPER_B
				|| m_pAnimator->Get_AnimationIndex() == DAM_UPPER_C)
			{
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(0.3f);
			}
		}
		m_bDamaged = true;
		//공중콤보중에맞는다면..
		if (m_pAnimator->Get_AnimationIndex() == DAM_UPPER_A
			|| m_pAnimator->Get_AnimationIndex() == DAM_UPPER_B
			|| m_pAnimator->Get_AnimationIndex() == DAM_UPPER_C)
		{
			switch (m_pColInfo->eColInfo)
			{
			case Engine::COL_BASE:
				m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_PUSH:
				m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_SMASH:
				m_pAnimator->Set_AnimationSet(DAM_HARD);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_FLY:
				m_pAnimator->Set_AnimationSet(DAM_UPPER_A);
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_pColInfo->fJumpForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_SPINA:
				m_pAnimator->Set_AnimationSet(SPIN_A);
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_pColInfo->fJumpForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_SPINB:
				m_bCol[Engine::COL_SPINB] = true;
				m_pAnimator->Set_AnimationSet(DAM_UPPER_B);
				m_pRigidbody->Set_RevisionGravity(m_pColInfo->fStampForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_STAMP:
				m_pAnimator->Set_AnimationSet(DAM_H);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_STUN:
				m_pAnimator->Set_AnimationSet(STUN);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_CAPTURE:
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				m_pAnimator->Set_AnimationSet(CAPTURE);
				break;
			}
		}
		else
		{
			switch (m_pColInfo->eColInfo)
			{
			case Engine::COL_BASE:
				if (m_pColInfo->vDir.x > 0)
				{
					if (m_pColInfo->vDir.x == max(m_pColInfo->vDir.x, abs(m_pColInfo->vDir.z)))
					{
						m_pAnimator->Set_AnimationSet(DAM_R);
					}
					else if (m_pColInfo->vDir.z == max(m_pColInfo->vDir.x, abs(m_pColInfo->vDir.z)))
					{

						if (m_pColInfo->vDir.z > 0)
						{
							m_pAnimator->Set_AnimationSet(DAM_F);
						}
						else
						{
							m_pAnimator->Set_AnimationSet(DAM_B);
						}
					}
				}

				else
				{
					if (m_pColInfo->vDir.x == max(abs(m_pColInfo->vDir.x), abs(m_pColInfo->vDir.z)))
					{
						m_pAnimator->Set_AnimationSet(DAM_L);
					}
					else
					{
						if (m_pColInfo->vDir.z > 0)
						{
							m_pAnimator->Set_AnimationSet(DAM_F);
						}
						else
						{
							m_pAnimator->Set_AnimationSet(DAM_B);
						}
					}
				}
				m_pAnimator->Set_AnimationSet(DAM_F);
				//m_bCol[Engine::COL_BASE] = true;
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_PUSH:
				//m_bCol[Engine::COL_PUSH] = true;
				if (m_pColInfo->vDir.x > 0)
				{
					if (m_pColInfo->vDir.x == max(m_pColInfo->vDir.x, abs(m_pColInfo->vDir.z)))
					{
						m_pAnimator->Set_AnimationSet(DAM_R);
					}
					else if (m_pColInfo->vDir.z == max(m_pColInfo->vDir.x, abs(m_pColInfo->vDir.z)))
					{

						if (m_pColInfo->vDir.z > 0)
						{
							m_pAnimator->Set_AnimationSet(DAM_F);
						}
						else
						{
							m_pAnimator->Set_AnimationSet(DAM_B);
						}
					}
				}

				else
				{
					if (m_pColInfo->vDir.x == max(abs(m_pColInfo->vDir.x), abs(m_pColInfo->vDir.z)))
					{
						m_pAnimator->Set_AnimationSet(DAM_L);
					}
					else
					{
						if (m_pColInfo->vDir.z > 0)
						{
							m_pAnimator->Set_AnimationSet(DAM_F);
						}
						else
						{
							m_pAnimator->Set_AnimationSet(DAM_B);
						}
					}
				}
				m_pAnimator->Set_AnimationSet(DAM_F);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_SMASH:
				m_pAnimator->Set_AnimationSet(DAM_HARD);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_FLY:
				m_pAnimator->Set_AnimationSet(DAM_UPPER_A);
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_pColInfo->fJumpForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_SPINA:
				m_pAnimator->Set_AnimationSet(SPIN_A);
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_pColInfo->fJumpForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_SPINB:
				m_bCol[Engine::COL_SPINB] = true;
				m_pAnimator->Set_AnimationSet(DAM_UPPER_B);
				m_pRigidbody->Set_RevisionGravity(m_pColInfo->fStampForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_STAMP:
				m_pAnimator->Set_AnimationSet(DAM_H);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_STUN:
				m_pAnimator->Set_AnimationSet(STUN);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z 축 방향
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z 축 방향
				}
				break;
			case Engine::COL_CAPTURE:
				m_pAnimator->Set_AnimationSet(CAPTURE);
				break;
			}
		}
	
	}

}


CAI_Trace_BWarrior * CAI_Trace_BWarrior::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI)
{
	CAI_Trace_BWarrior* pInstance = new CAI_Trace_BWarrior;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI)))
	{
		MSG_BOX("CAI_Trace_BWarrior Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Trace_BWarrior::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
