#include "stdafx.h"
#include "AI_Discovery_BBow.h"
#include "Map_Ice.h"
#include "NavigationMesh.h"
#include "Cube.h"
#include "Player.h"
#include "AI.h"
#include "Sound.h"

//클라

CAI_Discovery_BBow::CAI_Discovery_BBow(void)
	:CAI_Babegazi_Bow()
	, m_fAggroRad(0.f)
	, m_bAggro(false)
{
}

CAI_Discovery_BBow::~CAI_Discovery_BBow(void)
{
}

HRESULT CAI_Discovery_BBow::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
{
	if (pTransform == NULL || pAnimator == NULL)
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

	m_pSound = pSound;

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

	if (NULL == Engine::FindList(L"Player"))
		return E_FAIL;

	pGameObject = Engine::FindWithTag(L"Player");
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	Engine::Safe_Release(pGameObject);
	m_fAggroRad = 30.f;

	return S_OK;
}

void CAI_Discovery_BBow::Start_State(void)
{
	m_bAggro = false;
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

void CAI_Discovery_BBow::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();
	LookAtDir(&m_vRayDir, fTimeDelta);

	if (!m_bAggro)
	{
		m_pSound->MyPlaySound(L"Discovery");
		m_pAnimator->Set_AnimationSet(DISCOVERY);
		m_pAnimator->Set_State(MONSTER_FIGHT);
		
		m_bAggro = true;
	}

	Collsion_Work();

	if (m_pAnimator->Is_AnimationEnd(DAM_B, 0.1) || m_pAnimator->Is_AnimationEnd(DAM_F, 0.1)
		|| m_pAnimator->Is_AnimationEnd(DAM_R, 0.1) || m_pAnimator->Is_AnimationEnd(DAM_L, 0.1))
	{
		m_pAI->Set_State(AI_IDLE);
		m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
		m_bCol[Engine::COL_PUSH] = false;
		m_bCol[Engine::COL_BASE] = false;
	}

	if (m_pAnimator->Is_AnimationEnd(DAM_UPPER_A, 0.3))
	{
		m_pAnimator->Set_AnimationSet(DAM_UPPER_B);
	}

	if (m_pAnimator->Is_AnimationEnd(SPIN_A, 0.1))
	{

		m_pAnimator->Set_AnimationSet(SPIN_B);
	}

	if (m_pAnimator->Get_AnimationIndex() == DAM_UPPER_B)
	{
		//밑으로 찍었을때.....한번위로 점프시킴..
		if (m_bCol[Engine::COL_SPINB] && m_pCalculator->Get_IsCol())
		{
			m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
			if (m_pColInfo->fJumpForce != 0.f)
			{
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_pColInfo->fJumpForce);
				//찍기 끔..
				m_bCol[Engine::COL_SPINB] = false;
				m_bSpinB = true;
				//m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
			}
			else
			{
				m_bCol[Engine::COL_SPINB] = false;
				//	m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
				m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
			}
		}
		else if (!m_bCol[Engine::COL_SPINB] && m_pCalculator->Get_IsCol())
		{
			//m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
			m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
			m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == SPIN_B)
	{
		if (m_pCalculator->Get_IsCol())
		{
			m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
		}
	}

	if (m_pAnimator->Is_AnimationEnd(DAM_HARD, 0.1))
	{
		m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
		m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
		//m_pAI->Set_State(AI_IDLE);
		m_bCol[Engine::COL_SMASH] = false;
	}

	if (m_pAnimator->Is_AnimationEnd(STUN, 0.1))
	{
		m_pAnimator->Set_AnimationSet(STAND_DOWN);
		m_bCol[Engine::COL_STUN] = false;
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
	
	ChangeAI_Work();

	//Discovery..
	if (m_pAnimator->Is_AnimationEnd(DISCOVERY, 0.2))
	{
		if (m_fPlayerDis > 10.f)
		{
			m_pAnimator->Set_State(MONSTER_FIGHT);
			m_pAI->Set_State(AI_TRACE);
		}

		else
		{
			m_pAnimator->Set_State(MONSTER_FIGHT);
			m_pAI->Set_State(AI_COMBAT);
		}
	}



}

void CAI_Discovery_BBow::End_State(void)
{

}


void CAI_Discovery_BBow::Collsion_Work(void)
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
				m_pAnimator->Set_AnimationSet(DAM_UPPER_B);
				m_bCol[Engine::COL_SPINB] = true;
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



CAI_Discovery_BBow * CAI_Discovery_BBow::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
{
	CAI_Discovery_BBow* pInstance = new CAI_Discovery_BBow;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI, pSound)))
	{
		MSG_BOX("CAI_Discovery_BBow Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Discovery_BBow::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
