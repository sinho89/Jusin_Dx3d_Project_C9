#include "stdafx.h"
#include "AI_RoundPunch_Hanuman.h"
#include "Map_Ice.h"
#include "NavigationMesh.h"
#include "AstarManager.h"
#include "AI.h"
#include "Sound.h"

//클라..

CAI_RoundPunch_Hanuman::CAI_RoundPunch_Hanuman(void)
	:CAI_Hanuman()
	, m_fCompareTime(0.f)
	, m_bCompareDis(false)
	, m_bAttackCol(false)
{
}

CAI_RoundPunch_Hanuman::~CAI_RoundPunch_Hanuman(void)
{
}


HRESULT CAI_RoundPunch_Hanuman::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
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

	m_fMonsterSpeed = 20.f;

	m_bRayTime = true;

	return S_OK;
}

void CAI_RoundPunch_Hanuman::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_pCubeList = Engine::FindList(L"Cube");
	m_bCompareDis = true;
	m_bDamaged = false;
	m_fCompareTime = 0.f;
	m_bWallCol = false;
}

void CAI_RoundPunch_Hanuman::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();

	if (m_pCalculator->Wall_Collision_Check(&m_vRayDir))
	{
		m_bWallCol = true;
	}

	if (m_pCalculator->Wall_Collision_Check(&-m_vRayDir))
	{
		m_bWallCol = true;
	}

	//LookAtDir(&m_vRayDir, fTimeDelta);

	if (m_pAnimator->Get_AnimationIndex() == ROUNDPUNCH_A)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_A]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_A]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[ROUNDPUNCH_A]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_A]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"RoundPunch_Voice");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_A]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_A]->tMotionInfo.dEndTime * 0.7,
			(*m_pvecAniInfo)[ROUNDPUNCH_A]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_A]->tMotionInfo.dEndTime * 0.8))
		{
			m_pSound->MyPlaySound(L"RoundPunch_Attack_1");
		}

		if (m_pAnimator->Set_AnimationSet(ROUNDPUNCH_B))
		{

		}

	}

	if (m_pAnimator->Get_AnimationIndex() == ROUNDPUNCH_B)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_B]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_B]->tMotionInfo.dEndTime * 0.2,
			(*m_pvecAniInfo)[ROUNDPUNCH_B]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_B]->tMotionInfo.dEndTime * 0.3))
		{
			m_pSound->MyPlaySound(L"RoundPunch_Attack_2");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_B]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_B]->tMotionInfo.dEndTime * 0.1,
			(*m_pvecAniInfo)[ROUNDPUNCH_B]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_B]->tMotionInfo.dEndTime * 0.9))
		{
			if (!m_bAttackCol)
			{
				m_bAttackCol = true;
				m_pSphereCollider->SetScale(15.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 2.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 1.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				int iRand = rand() % 300;
				tColInfo.iDamage = 500 + iRand;
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAttackCol)
			{
				m_bAttackCol = false;
				Engine::Delete_Collider(L"Monster", m_pSphereCollider);
			}
		}

		int i = rand() % 2;
		if (i == 0)
		{
			if (m_pAnimator->Set_AnimationSet(ROUNDPUNCH_C))
			{

			}
		}
		if (i == 1)
		{
			if (m_pAnimator->Set_AnimationSet(ROUNDPUNCH_C1_START))
			{

			}
		}
		if (!m_bWallCol)
		{
			m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
			m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vLook, m_fMonsterSpeed, m_dwIdx, fTimeDelta));
			m_vPos = m_pTransform->Get_Now_Position();
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == ROUNDPUNCH_C)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_C]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[ROUNDPUNCH_C]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"RoundPunch_Fall_1");
			m_pSound->MyPlaySound(L"RoundPunch_Attack_3");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_C]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[ROUNDPUNCH_C]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C]->tMotionInfo.dEndTime * 0.5))
		{
			if (!m_bWallCol)
			{
				m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vLook, m_fMonsterSpeed, m_dwIdx, fTimeDelta));
				m_vPos = m_pTransform->Get_Now_Position();
			}
		}

		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
		{
			m_pAI->Set_State(AI_IDLE);
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == ROUNDPUNCH_C1_START)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"RoundPunch_Attack_3");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dEndTime * 0.3))
		{
			if (!m_bWallCol)
			{
				m_pTransform->Set_Position(&m_pNavigationMesh->Move_OnNaviMesh(&m_vPos, &m_vLook, m_fMonsterSpeed, m_dwIdx, fTimeDelta));
				m_vPos = m_pTransform->Get_Now_Position();
			}
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dEndTime * 0.1,
			(*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C1_START]->tMotionInfo.dEndTime * 0.2))
		{
			m_pSound->MyPlaySound(L"RoundPunch_Fall_1");
		}

		if (m_pAnimator->Set_AnimationSet(ROUNDPUNCH_C1_ING))
		{

		}
	}

	if (m_pAnimator->Get_AnimationIndex() == ROUNDPUNCH_C1_ING)
	{
		if (m_pAnimator->Set_AnimationSet(ROUNDPUNCH_C1_END))
		{

		}
	}
	
	if (m_pAnimator->Get_AnimationIndex() == ROUNDPUNCH_C1_END)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROUNDPUNCH_C1_END]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C1_END]->tMotionInfo.dEndTime * 0.2,
			(*m_pvecAniInfo)[ROUNDPUNCH_C1_END]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROUNDPUNCH_C1_END]->tMotionInfo.dEndTime * 0.3))
		{
			m_pSound->MyPlaySound(L"Floor_Hit");
		}

		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
		{
			m_pAI->Set_State(AI_IDLE);
		}
	}

	//PlayerRayCheck();
	//CubeRayCheck();

	Collsion_Work();

	ChangeAni_Work();

	//AI바뀌는곳.
	ChangeAI_Work();

}

void CAI_RoundPunch_Hanuman::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

void CAI_RoundPunch_Hanuman::Collsion_Work(void)
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


CAI_RoundPunch_Hanuman * CAI_RoundPunch_Hanuman::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
{
	CAI_RoundPunch_Hanuman* pInstance = new CAI_RoundPunch_Hanuman;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI, pSound)))
	{
		MSG_BOX("CAI_RoundPunch_Hanuman Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_RoundPunch_Hanuman::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
