#include "stdafx.h"
#include "AI_Down_BWarrior.h"
#include "Map_Ice.h"
#include "AI.h"

//클라..

CAI_Down_BWarrior::CAI_Down_BWarrior(void)
	:CAI_Babegazi_Warrior()
	, m_fDownTime(0.f)
	, m_bStand(false)
	, m_iDirection(0)
	, m_bOne(false)
	, m_bDown(false)
{
}

CAI_Down_BWarrior::~CAI_Down_BWarrior(void)
{
}


HRESULT CAI_Down_BWarrior::Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
	Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
	Engine::COLINFO* pColInfo, _bool* pDamaged,
	Engine::CAI* pAI)
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

void CAI_Down_BWarrior::Start_State(void)
{
	_vec3 vAngle = m_pTransform->Get_Now_Angle();
	vAngle.x = 0.f;
	m_pTransform->Set_Angle(&vAngle);
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
	m_bStand = false;
	m_bOne = false;
	m_fDownTime = 0.f;
}


void CAI_Down_BWarrior::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();

	if (!m_bOne)
	{
		m_pAnimator->Set_AnimationSet(DOWN);
		m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
		m_bOne = true;
		m_bDown = true;
	}


	if (!m_bStand)
	{
		m_fDownTime += fTimeDelta;
		if (m_fDownTime > 4.0f)
		{
			m_bStand = true;
			m_fDownTime = 0.f;	
		}
	}

	if (m_bStand)
	{
		m_pAnimator->Set_AnimationSet(STAND_UP);
		m_bStand = false;
		m_bDown = false;
	}
	if (m_bDown)
	{
		if (*m_pDamaged)
		{
			switch (m_pColInfo->eColInfo)
			{
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
	
	if (!m_bDown)
	{
		Collsion_Work();
	}

	ChangeAni_Work();

	ChangeAI_Work();

	//Down.
	if (m_pAnimator->Is_AnimationEnd(STAND_UP, 0.1))
	{
		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
		{
			m_pAI->Set_State(AI_IDLE);
		}
	}





}

void CAI_Down_BWarrior::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

void CAI_Down_BWarrior::Collsion_Work(void)
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

CAI_Down_BWarrior * CAI_Down_BWarrior::Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
	Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
	Engine::COLINFO* pColInfo, _bool* pDamaged,
	Engine::CAI* pAI)
{
	CAI_Down_BWarrior* pInstance = new CAI_Down_BWarrior;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody
	, pCollider, pColInfo, pDamaged, pAI)))
	{
		MSG_BOX("CAI_Down_BWarrior Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Down_BWarrior::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
