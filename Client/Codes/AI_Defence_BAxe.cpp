#include "stdafx.h"
#include "AI_Defence_BAxe.h"
#include "Map_Ice.h"
#include "NavigationMesh.h"
#include "Cube.h"
#include "AstarManager.h"
#include "AI.h"
#include "Sound.h"

//클라..

CAI_Defence_BAxe::CAI_Defence_BAxe(void)
	:CAI_Babegazi_Axe()
	, m_fDefenceTime(0.f)
	, m_bAttack(false)
	, m_bOne(false)
	, m_bDefence(false)
	, m_bAttackCol(false)
	, m_iDirection(0)
	, m_dwIdx(0)
{
}

CAI_Defence_BAxe::~CAI_Defence_BAxe(void)
{
}


HRESULT CAI_Defence_BAxe::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
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


	pGameObject = Engine::FindWithTag(L"Player");
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	Engine::Safe_Release(pGameObject);

	m_fMonsterSpeed = 10.f;

	m_bRayTime = true;



	return S_OK;
}

void CAI_Defence_BAxe::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_bLookAtDir = true;
	m_bOne = false;
	m_bDamaged = false;

	if (JUMP_C == m_pAnimator->Get_AnimationIndex())
	{
		m_pAnimator->Set_AnimationSet(BATTLE_IDLE);
	}
	if (JUMP_B == m_pAnimator->Get_AnimationIndex())
	{
		m_pAnimator->Set_AnimationSet(JUMP_C);
	}

	if (JUMP_A == m_pAnimator->Get_AnimationIndex())
	{
		m_pAnimator->Set_AnimationSet(JUMP_C);
	}

}

void CAI_Defence_BAxe::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;


	Set_Ray();


	LookAtDir(&m_vRayDir, fTimeDelta);	

	if (!m_bOne)
	{
		if (m_pAnimator->Set_AnimationSet(DEFENCE))
		{
			m_bOne = true;
			m_bDefence = true;
		}
	}
	////////////////////////////////////
	if (m_pAnimator->Get_AnimationIndex() == DEFENCE_SWORD)
	{
		m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
		m_bLookAtDir = false;

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"Defence_Sword");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dEndTime * 0.2,
			(*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dEndTime * 0.3))
		{
			m_pSound->MyPlaySound(L"Weapon_3");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dEndTime * 0.2,
			(*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[DEFENCE_SWORD]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAttackCol)
			{
				m_bAttackCol = true;
				m_pSphereCollider->SetScale(5.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.5f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 3.f;
				int iRand = rand() % 200;
				tColInfo.iDamage = 200 + iRand;
				tColInfo.vDir = m_vLook;
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pSphereCollider);
			}
			m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vLook, 15.f, m_dwIdx, fTimeDelta));
		}
		else
		{
			if (m_bAttackCol)
			{
				m_bAttackCol = false;
				Engine::Delete_Collider(L"Monster", m_pSphereCollider);
			}
		}
	}
	////////////////////////////////////////
	if (!m_bDamaged)
	{
		if (!m_bAttack)
		{
			m_fDefenceTime += fTimeDelta;
			if (m_fDefenceTime > 5.0f)
			{
				m_bLookAtDir = false;
				m_bAttack = true;
			}

		}
	}

	if (m_bAttack)
	{
		m_pAnimator->Set_AnimationSet(DEFENCE_SWORD);
		m_bAttack = false;
		m_bLookAtDir = false;
		m_bDefence = false;
		m_fDefenceTime = 0.f;
	}

	if (DEFENCE_SWORD == m_pAnimator->Get_AnimationIndex())
	{
		m_bLookAtDir = false;
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

	ChangeAI_Work();

	ChangeAni_Work();

	if (m_pAnimator->Is_AnimationEnd(DEFENCE_SWORD, 0.1))
	{
		if (m_pAnimator->Set_AnimationSet(DOWN))
		{
			m_pAI->Set_State(AI_DOWN);
		}
	}

}

void CAI_Defence_BAxe::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_bAttackCol = false;
}

void CAI_Defence_BAxe::Collsion_Work(void)
{
	if (*m_pDamaged)
	{

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
			if (!m_bDefence)
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

				switch (m_pColInfo->eColInfo)
				{
				case Engine::COL_BASE:

					break;
				case Engine::COL_PUSH:

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
			else      //m_bDefence == true
			{

				switch (m_pColInfo->eColInfo)
				{
				case Engine::COL_BASE:

					break;
				case Engine::COL_PUSH:

					break;
				case Engine::COL_SMASH:
					m_bDamaged = true;
					m_bDefence = false;
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

					break;
				case Engine::COL_SPINA:

					break;
				case Engine::COL_SPINB:

					break;
				case Engine::COL_STAMP:
					m_bDamaged = true;
					m_bDefence = false;
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
					m_bDamaged = true;
					m_bDefence = false;
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
	
}

CAI_Defence_BAxe *CAI_Defence_BAxe::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
{
	CAI_Defence_BAxe* pInstance = new CAI_Defence_BAxe;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI, pSound)))
	{
		MSG_BOX("CAI_Defence_BAxe Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Defence_BAxe::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
