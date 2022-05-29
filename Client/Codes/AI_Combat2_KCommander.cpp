#include "stdafx.h"
#include "AI_Combat2_KCommander.h"
#include "Map_Ice.h"
#include "NavigationMesh.h"
#include "Cube.h"
#include "AstarManager.h"
#include "AI.h"

//클라..

CAI_Combat2_KCommander::CAI_Combat2_KCommander(void)
	:CAI_Knole_Commander()
	, m_fChangeTime(0.f)
	, m_bChange(false)
	, m_bLRMove(false)
	, m_bDamaged(false)
	, m_iDirection(0)
	, m_dwIdx(0)
{
}

CAI_Combat2_KCommander::~CAI_Combat2_KCommander(void)
{
}


HRESULT CAI_Combat2_KCommander::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon
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

	m_pWeapon = pWeapon;

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

void CAI_Combat2_KCommander::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_iDirection = rand() % 2;

	m_bLookAtDir = true;
	m_bDamaged = false;
	m_bChange = false;
	m_fChangeTime = 0.f;

}

void CAI_Combat2_KCommander::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;


	Set_Ray();

	//PlayerRayCheck();

	LookAtDir(&m_vRayDir, fTimeDelta);

	if (!m_bChange)
	{
		m_fChangeTime += fTimeDelta;
		if (m_fChangeTime > 3.f)
		{
			m_bChange = true;
			m_fChangeTime = 0.f;
		}
	}

	if (!m_bDamaged)
	{
		if (m_bChange)
		{
			int i = rand()%2;
			if (i == 0)
			{
				if (m_pAnimator->Set_AnimationSet(ATTACK_1))
				{
					m_pAI->Set_State(AI_ATTACK1);
					return;
				}
			}
			else if (i == 1)
			{
				if (m_pAnimator->Set_AnimationSet(ATTACK_2))
				{
					m_pAI->Set_State(AI_ATTACK2);
					return;
				}
			}
			else
			{
				m_pAI->Set_State(AI_COMBAT2);
				return;
			}
		}
		if (m_fPlayerDis > 20.f)
		{
			if (m_pAnimator->Set_AnimationSet(WEAPON_OUT))
			{
				//*m_pWeapon = false;
				m_pAI->Set_State(AI_TRACE);
				return;
			}
		}

		if (!m_bLRMove)
		{
			if (m_fPlayerDis > 10.f)
			{
				m_pAnimator->Set_AnimationSet(WALK_F);
				if (m_pAnimator->Get_AnimationIndex() == WALK_F)
				{
					_vec3 vEnd = m_vPos + m_vRayDir * fTimeDelta * m_fMonsterSpeed;
					m_pTransform->Set_Position(&vEnd);
				}
			}
			else
				m_bLRMove = true;
		}


		if (m_bLRMove)
		{
			//떨림보정
			if (m_iDirection == 0)
			{
				m_pAnimator->Set_AnimationSet(WALK_L);
				if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[WALK_L]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WALK_L]->tMotionInfo.dEndTime * 0.0,
					(*m_pvecAniInfo)[WALK_L]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WALK_L]->tMotionInfo.dEndTime * 0.9))
				{
					_vec3 vRight = m_pTransform->Get_Now_Right();
					_vec3 vEnd = m_vPos + vRight * fTimeDelta * 10.f;
					m_pTransform->Set_Position(&vEnd);
				}
			}
			else if (m_iDirection == 1)
			{
				m_pAnimator->Set_AnimationSet(WALK_R);
				if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[WALK_R]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WALK_R]->tMotionInfo.dEndTime * 0.0,
					(*m_pvecAniInfo)[WALK_R]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WALK_R]->tMotionInfo.dEndTime * 0.9))
				{
					_vec3 vRight = m_pTransform->Get_Now_Right();
					_vec3 vEnd = m_vPos - vRight * fTimeDelta * 10.f;
					m_pTransform->Set_Position(&vEnd);
				}
			}

			if (m_fPlayerDis > 15.f /*|| m_fPlayerDis < 7.f*/)
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
		//찍기 와이값..돌리기
		_vec3 vAngle = m_pTransform->Get_Now_Angle();
		vAngle.x = 0.f;
		m_pTransform->Set_Angle(&vAngle);
	}
	////////////////////////////////

	Collsion_Work();

	ChangeAni_Work();
	
	ChangeAI_Work();


}

void CAI_Combat2_KCommander::End_State(void)
{
	//찍기 와이값..돌리기
	_vec3 vAngle = m_pTransform->Get_Now_Angle();
	vAngle.x = 0.f;
	m_pTransform->Set_Angle(&vAngle);
}

void CAI_Combat2_KCommander::Collsion_Work(void)
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
				m_pAnimator->Set_AnimationSet(DAM_B);
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
							m_pAnimator->Set_AnimationSet(DAM_F);
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
							m_pAnimator->Set_AnimationSet(DAM_F);
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
							m_pAnimator->Set_AnimationSet(DAM_F);
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
							m_pAnimator->Set_AnimationSet(DAM_F);
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
				m_pAnimator->Set_AnimationSet(DAM_B);
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

CAI_Combat2_KCommander * CAI_Combat2_KCommander::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon
	, Engine::CAI* pAI)
{
	CAI_Combat2_KCommander* pInstance = new CAI_Combat2_KCommander;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pWeapon, pAI)))
	{
		MSG_BOX("CAI_Combat2_KCommander Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Combat2_KCommander::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
