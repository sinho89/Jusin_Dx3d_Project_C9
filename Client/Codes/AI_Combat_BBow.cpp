#include "stdafx.h"
#include "AI_Combat_BBow.h"
#include "Map_Ice.h"
#include "NavigationMesh.h"
#include "Cube.h"
#include "AstarManager.h"
#include "AI.h"

//Ŭ��..

CAI_Combat_BBow::CAI_Combat_BBow(void)
	:CAI_Babegazi_Bow()
	, m_fChangeTime(0.f)
	, m_fEscapeTime(0.f)
	, m_bChange(false)
	, m_bLRMove(false)
	, m_bDamaged(false)
	, m_bEscape(false)
	, m_iDirection(0)
	, m_dwIdx(0)
{
}

CAI_Combat_BBow::~CAI_Combat_BBow(void)
{
}


HRESULT CAI_Combat_BBow::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
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
		//���̸� ���� �� �޽� ����...
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

void CAI_Combat_BBow::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_iDirection = rand() % 2;

	m_bLookAtDir = true;
	m_bDamaged = false;
	m_bChange = false;
	m_bEscape = false;
	m_fChangeTime = 0.f;
	m_fEscapeTime = 0.f;

}

void CAI_Combat_BBow::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();

	if (m_pCalculator->Wall_Collision_Check(&m_vRayDir))
	{
		if(m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
			m_pAI->Set_State(AI_IDLE);
	}

	if (m_pCalculator->Wall_Collision_Check(&-m_vRayDir))
	{
		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
			m_pAI->Set_State(AI_IDLE);
	}

	if (!m_bEscape)
	{
		LookAtDir(&m_vRayDir, fTimeDelta);
	}

	if (!m_bChange && !m_bEscape)
	{
		m_fChangeTime += fTimeDelta;
		if (m_fChangeTime > 4.f)
		{
			m_bChange = true;
			m_fChangeTime = 0.f;
		}
	}

	if (!m_bDamaged && m_bEscape)
	{
		m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
		m_fChangeTime = 0.f;
		LookAtRoamDir(&m_vEscapeDir, fTimeDelta);
		m_fEscapeTime += fTimeDelta;
		if (m_fEscapeTime > 3.f)
		{
			m_bEscape = false;
			m_fEscapeTime = 0.f;
			if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
			{
				m_pAI->Set_State(AI_IDLE);
			}
		}
		m_pAnimator->Set_AnimationSet(MOVE_RUN);
		m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vEscapeDir, 10.f, m_dwIdx, fTimeDelta));
	}

	if (!m_bDamaged && !m_bEscape)
	{
		if (m_bChange)
		{
			int i = 3; // rand() % 6;
			if (i >= 0 && i<= 2 )
			{
				m_pAI->Set_State(AI_ATTACK);
				return;
			}
			else if (i == 3)
			{
				m_pAI->Set_State(AI_ATTACK);
				return;
			}
			else if (i == 4)
			{
				if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
				{
					m_pAI->Set_State(AI_IDLE);
					return;
				}
			}
			else
			{
				m_pAI->Set_State(AI_COMBAT);
				return;
			}
		}
		if (m_fPlayerDis > 32.f)
		{
			m_pAI->Set_State(AI_TRACE);
			return;
		}

		if (!m_bLRMove)
		{
			m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
			if (m_fPlayerDis > 30.f)
			{
				m_pAnimator->Set_AnimationSet(MOVE_RUN);
				m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vLook, m_fMonsterSpeed, m_dwIdx, fTimeDelta));
			}
			else if (m_fPlayerDis < 15.f && m_fPlayerDis > 10.f)
			{
				m_pAnimator->Set_AnimationSet(WALK_B);
				m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vLook, m_fMonsterSpeed, m_dwIdx, fTimeDelta));

			}
			else if (m_fPlayerDis <= 10.f)
			{
				m_bEscape = true;
				m_vEscapeDir = -m_vLook;
				//m_pAnimator->Set_AnimationSet(MOVE_RUN);
				//_vec3 vEnd = m_vPos - m_vRayDir * fTimeDelta * 10.f;
				//m_pTransform->Set_Position(&vEnd);
			}
			else
				m_bLRMove = true;
		}


		if (m_bLRMove)
		{
			m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
			//��������
			if (m_iDirection == 0)
			{
				m_pAnimator->Set_AnimationSet(WALK_L);
				_vec3 vRight = m_pTransform->Get_Now_Right();
				m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &vRight, 3.f, m_dwIdx, fTimeDelta));
			}
			else if (m_iDirection == 1)
			{
				m_pAnimator->Set_AnimationSet(WALK_R);
				_vec3 vRight = m_pTransform->Get_Now_Right();
				m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &vRight, 3.f, m_dwIdx, fTimeDelta));
			}

			if (m_fPlayerDis > 31.f || m_fPlayerDis < 14.f)
				m_bLRMove = false;
		}
	}

	///////////////////////////////
	if (m_bSpinB)
	{
		m_pTransform->Rotation(Engine::ROT_X, -700.f * fTimeDelta);
	}
	else
	{
		//��� ���̰�..������
		_vec3 vAngle = m_pTransform->Get_Now_Angle();
		vAngle.x = 0.f;
		m_pTransform->Set_Angle(&vAngle);
	}
	////////////////////////////////

	Collsion_Work();

	ChangeAni_Work();
	
	ChangeAI_Work();


}

void CAI_Combat_BBow::End_State(void)
{
	//��� ���̰�..������
	_vec3 vAngle = m_pTransform->Get_Now_Angle();
	vAngle.x = 0.f;
	m_pTransform->Set_Angle(&vAngle);
}

void CAI_Combat_BBow::Collsion_Work(void)
{
	if (*m_pDamaged)
	{
		m_bEscape = false;
		m_fEscapeTime = 0.f;
		//���ĵ�����¿��� ������ ������ �� ����ó��
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
		//�����޺��߿��´´ٸ�..
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
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_PUSH:
				m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_SMASH:
				m_pAnimator->Set_AnimationSet(DAM_HARD);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
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
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
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
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
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
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				break;
			case Engine::COL_STAMP:
				m_pAnimator->Set_AnimationSet(DAM_H);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				m_bSpinB = false;
				m_bCol[SPIN_B] = false;
				break;
			case Engine::COL_STUN:
				m_pAnimator->Set_AnimationSet(STUN);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
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
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
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
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				break;
			case Engine::COL_SMASH:
				m_pAnimator->Set_AnimationSet(DAM_HARD);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				break;
			case Engine::COL_FLY:
				m_pAnimator->Set_AnimationSet(DAM_UPPER_A);
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_pColInfo->fJumpForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				break;
			case Engine::COL_SPINA:
				m_pAnimator->Set_AnimationSet(SPIN_A);
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_pColInfo->fJumpForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				break;
			case Engine::COL_SPINB:
				m_pAnimator->Set_AnimationSet(DAM_UPPER_B);
				m_bCol[Engine::COL_SPINB] = true;
				m_pRigidbody->Set_RevisionGravity(m_pColInfo->fStampForce);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				break;
			case Engine::COL_STAMP:
				m_pAnimator->Set_AnimationSet(DAM_H);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				break;
			case Engine::COL_STUN:
				m_pAnimator->Set_AnimationSet(STUN);
				if (m_pColInfo->eColDir == Engine::DIR_LOOK)
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, m_pColInfo->fForce); //x z �� ����
				}
				else
				{
					m_pRigidbody->Set_AccMotion(&m_pColInfo->vDest, m_pColInfo->fForce); //x z �� ����
				}
				break;
			case Engine::COL_CAPTURE:
				m_pAnimator->Set_AnimationSet(CAPTURE);
				break;
			}
		}

	}
}

CAI_Combat_BBow * CAI_Combat_BBow::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI)
{
	CAI_Combat_BBow* pInstance = new CAI_Combat_BBow;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI)))
	{
		MSG_BOX("CAI_Combat_BBow Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Combat_BBow::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
