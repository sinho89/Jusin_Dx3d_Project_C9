#include "stdafx.h"
#include "AI_Lava1_Bardiel.h"
#include "Map_Fire.h"
#include "NavigationMesh.h"
#include "AstarManager.h"
#include "AI.h"
#include "Sound.h"

//클라..

CAI_Lava1_Bardiel::CAI_Lava1_Bardiel(void)
	:CAI_Bardiel()
	,m_bAttackCol1(false)
	,m_bAttackCol2(false)
	,m_bAttackCol3(false)
{
}

CAI_Lava1_Bardiel::~CAI_Lava1_Bardiel(void)
{
}


HRESULT CAI_Lava1_Bardiel::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
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

	m_pSound = pSound;

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

void CAI_Lava1_Bardiel::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);

	//m_pCubeList = Engine::FindList(L"Cube");
	m_bDamaged = false;

}

void CAI_Lava1_Bardiel::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL)
		return;

	Set_Ray();

	if (m_pAnimator->Get_AnimationIndex() == LAVA)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[LAVA]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[LAVA]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[LAVA]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[LAVA]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"Bardiel_Lava_Voice");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[LAVA]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[LAVA]->tMotionInfo.dEndTime * 0.4,
			(*m_pvecAniInfo)[LAVA]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[LAVA]->tMotionInfo.dEndTime * 0.5))
		{
			m_pSound->MyPlaySound(L"Bardiel_Lava_Fire_2");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[LAVA]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[LAVA]->tMotionInfo.dEndTime * 0.4,
			(*m_pvecAniInfo)[LAVA]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[LAVA]->tMotionInfo.dEndTime * 0.8))
		{
			m_pSound->MyPlaySound(L"Bardiel_Lava_Fire");
			if (!m_bAttackCol1)
			{
				m_bAttackCol1 = true;
				m_pSphereCollider->SetScale(30.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
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

		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE_1))
		{
			m_pAI->Set_State(AI_COMBAT);
		}
	}



	PlayerRayCheck();
	//CubeRayCheck();

	//Collsion_Work();

	ChangeAni_Work();

		//AI바뀌는곳.
	ChangeAI_Work();
	
}

void CAI_Lava1_Bardiel::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}


void CAI_Lava1_Bardiel::Collsion_Work(void)
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


CAI_Lava1_Bardiel * CAI_Lava1_Bardiel::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
{
	CAI_Lava1_Bardiel* pInstance = new CAI_Lava1_Bardiel;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI, pSound)))
	{
		MSG_BOX("CAI_Lava1_Bardiel Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Lava1_Bardiel::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
