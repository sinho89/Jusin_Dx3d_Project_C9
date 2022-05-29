#include "stdafx.h"
#include "AI_Idle_BWarrior.h"
#include "Map_Ice.h"
#include "AI.h"

//클라..

CAI_Idle_BWarrior::CAI_Idle_BWarrior(void)
	:CAI_Babegazi_Warrior()
	, m_bOne(false)
	, m_bIdleTime(false)
	, m_fIdleTime(0.f)
{
}

CAI_Idle_BWarrior::~CAI_Idle_BWarrior(void)
{
}


HRESULT CAI_Idle_BWarrior::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
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

	m_fMonsterSpeed = 10.f;

	m_bRayTime = true;



	return S_OK;
}

void CAI_Idle_BWarrior::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	//if (JUMP_C == m_pAnimator->Get_AnimationIndex())
	//{
	//	m_pAnimator->Set_AnimationSet(BATTLE_IDLE);
	//}
	//if (JUMP_B == m_pAnimator->Get_AnimationIndex())
	//{
	//	m_pAnimator->Set_AnimationSet(JUMP_C);
	//}

	//if (JUMP_A == m_pAnimator->Get_AnimationIndex())
	//{
	//	m_pAnimator->Set_AnimationSet(JUMP_C);
	//}

	//m_pAnimator->Set_AnimationSet(BATTLE_IDLE);

	m_bDamaged = false;
	m_bIdleTime = false;
	m_fIdleTime = 0.f;
}

void CAI_Idle_BWarrior::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();
	LookAtDir(&m_vRayDir, fTimeDelta);

	if (!m_bDamaged)
	{
		if (!m_bIdleTime)
		{
			m_fIdleTime += fTimeDelta;
			if (m_fIdleTime > 1.5f)
			{
				m_bIdleTime = true;
				m_fIdleTime = 0.f;
			}
		}

		if (m_bIdleTime)
		{
				if (m_fPlayerDis > 20.f)
				{
					m_pAI->Set_State(AI_TRACE);
				}

				else
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

	//AI 바뀌는곳..
	ChangeAI_Work();


}

void CAI_Idle_BWarrior::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

void CAI_Idle_BWarrior::Collsion_Work(void)
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
		m_fIdleTime = 0.f;
		m_bIdleTime = false;
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
				//m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
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
				m_bCol[Engine::COL_SPINB] = true;
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
		else  //공중콤보가 아닌 경우..
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
				if (m_pAnimator->Get_AnimationIndex() == DAM_UPPER_B)
				{
					m_pAnimator->Set_AnimationSet(DAM_UPPER_B);
				}
				//m_bCol[Engine::COL_PUSH] = true;
				else
				{
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

CAI_Idle_BWarrior * CAI_Idle_BWarrior::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI)
{
	CAI_Idle_BWarrior* pInstance = new CAI_Idle_BWarrior;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator,
		pRigidbody, pCollider, pColInfo, pDamaged, pAI)))
	{
		MSG_BOX("CAI_Idle Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Idle_BWarrior::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
