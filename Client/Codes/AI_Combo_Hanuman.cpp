#include "stdafx.h"
#include "AI_Combo_Hanuman.h"
#include "Map_Ice.h"
#include "NavigationMesh.h"
#include "AstarManager.h"
#include "AI.h"
#include "Sound.h"

//클라..

CAI_Combo_Hanuman::CAI_Combo_Hanuman(void)
	:CAI_Hanuman()
	, m_fCompareTime(0.f)
	, m_bCompareDis(false)
	, m_bAttackCol1(false)
	, m_bAttackCol2(false)
{
}

CAI_Combo_Hanuman::~CAI_Combo_Hanuman(void)
{
}


HRESULT CAI_Combo_Hanuman::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
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

void CAI_Combo_Hanuman::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_pCubeList = Engine::FindList(L"Cube");
	m_bCompareDis = true;
	m_bDamaged = false;
	m_fCompareTime = 0.f;
}

void CAI_Combo_Hanuman::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();


	//LookAtDir(&m_vRayDir, fTimeDelta);

	if (m_pAnimator->Get_AnimationIndex() == COMBO0)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[COMBO0]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO0]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[COMBO0]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO0]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"Combo_Voice");
		}
		if (m_pAnimator->Set_AnimationSet(COMBO1))
		{

		}

	}

	if (m_pAnimator->Get_AnimationIndex() == COMBO1)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[COMBO1]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO1]->tMotionInfo.dEndTime * 0.2,
			(*m_pvecAniInfo)[COMBO1]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO1]->tMotionInfo.dEndTime * 0.3))
		{
			m_pSound->MyPlaySound(L"Combo_Attack_1");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[COMBO1]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO1]->tMotionInfo.dEndTime * 0.6,
			(*m_pvecAniInfo)[COMBO1]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO1]->tMotionInfo.dEndTime * 0.9))
		{
			if (!m_bAttackCol1)
			{
				m_bAttackCol1 = true;
				m_pSphereCollider->SetScale(10.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 5.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 3.f;
				tColInfo.vDir = m_vLook;
				int iRand = rand() % 300;
				tColInfo.iDamage = 500 + iRand;
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAttackCol1)
			{
				m_bAttackCol1 = false;
				Engine::Delete_Collider(L"Monster", m_pSphereCollider);
			}
		}


		if (m_pAnimator->Set_AnimationSet(COMBO2))
		{
	
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == COMBO2)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[COMBO2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO2]->tMotionInfo.dEndTime * 0.5,
			(*m_pvecAniInfo)[COMBO2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO2]->tMotionInfo.dEndTime * 0.9))
		{
			m_pSound->MyPlaySound(L"Combo_Attack_2");
			if (!m_bAttackCol2)
			{
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 6);
				m_bAttackCol2 = true;
				m_pSphereCollider->SetScale(17.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 8.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 3.f;
				tColInfo.vDir = m_vLook;
				int iRand = rand() % 300;
				tColInfo.iDamage = 600 + iRand;
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAttackCol2)
			{
				m_bAttackCol2 = false;
				Engine::Delete_Collider(L"Monster", m_pSphereCollider);
			}
		}

		if (m_pAnimator->Set_AnimationSet(COMBO2_END))
		{

		}
	}

	if (m_pAnimator->Get_AnimationIndex() == COMBO2_END)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[COMBO2_END]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO2_END]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[COMBO2_END]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[COMBO2_END]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"Combo_Attack_3");
		}
		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
		{
			m_pAI->Set_State(AI_IDLE);
		}
	}

	Collsion_Work();

	ChangeAni_Work();

	//AI바뀌는곳.
	ChangeAI_Work();

}

void CAI_Combo_Hanuman::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

void CAI_Combo_Hanuman::Collsion_Work(void)
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


CAI_Combo_Hanuman * CAI_Combo_Hanuman::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
{
	CAI_Combo_Hanuman* pInstance = new CAI_Combo_Hanuman;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI, pSound)))
	{
		MSG_BOX("CAI_Combo_Hanuman Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Combo_Hanuman::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
