#include "stdafx.h"
#include "AI_Idle_Hanuman.h"
#include "Map_Ice.h"
#include "AI.h"

//클라..

CAI_Idle_Hanuman::CAI_Idle_Hanuman(void)
	:CAI_Hanuman()
	, m_bOne(false)
	, m_bIdleTime(false)
	, m_fIdleTime(0.f)
{
}

CAI_Idle_Hanuman::~CAI_Idle_Hanuman(void)
{
}


HRESULT CAI_Idle_Hanuman::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
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

void CAI_Idle_Hanuman::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);

	m_pAnimator->Set_AnimationSet(BATTLE_IDLE);

	m_bDamaged = false;
	m_bIdleTime = false;
	m_fIdleTime = 0.f;
}

void CAI_Idle_Hanuman::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();


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
		if (m_iPatternCount == 0)
		{
			if (m_pAnimator->Set_AnimationSet(ATTACK1))
			{
				m_pAI->Set_State(AI_ATTACK1);
				++m_iPatternCount;
				return;
			}
		}
		else if (m_iPatternCount == 1)
		{
			if (m_pAnimator->Set_AnimationSet(ATTACK2))
			{
				m_pAI->Set_State(AI_ATTACK2);
				++m_iPatternCount;
				return;
			}
		}
		else if (m_iPatternCount == 2)
		{
			if (m_pAnimator->Set_AnimationSet(ROUNDPUNCH_A))
			{
				m_pAI->Set_State(AI_ROUNDPUNCH);
				++m_iPatternCount;
				return;
			}
		}
		else if (m_iPatternCount == 3)
		{
			if (m_pAnimator->Set_AnimationSet(ROAR))
			{
				m_pAI->Set_State(AI_ROAR);
				++m_iPatternCount;
				return;
			}
		}
		else if (m_iPatternCount == 4)
		{
			if (m_pAnimator->Set_AnimationSet(CRAZYFOOT))
			{
				m_pAI->Set_State(AI_CRAZYFOOT);
				++m_iPatternCount;
				return;
			}
		}
		else if (m_iPatternCount == 5)
		{
			if (m_pAnimator->Set_AnimationSet(BREATH_0))
			{
				m_pAI->Set_State(AI_BREATH);
				++m_iPatternCount;
				return;
			}
		}
		else if (m_iPatternCount == 6)
		{
			if (m_pAnimator->Set_AnimationSet(COMBO0))
			{
				m_pAI->Set_State(AI_COMBO);
				++m_iPatternCount;
				return;
			}
		}
		else
		{
			int iRand = rand() % 7;
			if (iRand == 0)
			{
				if (m_pAnimator->Set_AnimationSet(ATTACK1))
				{
					m_pAI->Set_State(AI_ATTACK1);
				}
			}
			if (iRand == 1)
			{
				if (m_pAnimator->Set_AnimationSet(ATTACK2))
				{
					m_pAI->Set_State(AI_ATTACK2);
				}
			}
			if (iRand == 2)
			{
				if (m_pAnimator->Set_AnimationSet(ROUNDPUNCH_A))
				{
					m_pAI->Set_State(AI_ROUNDPUNCH);
				}
			}
			if (iRand == 3)
			{
				if (m_pAnimator->Set_AnimationSet(ROAR))
				{
					m_pAI->Set_State(AI_ROAR);
				}
			}
			if (iRand == 4)
			{
				if (m_pAnimator->Set_AnimationSet(COMBO0))
				{
					m_pAI->Set_State(AI_COMBO);
				}
			}
			if (iRand == 5)
			{
				if (m_pAnimator->Set_AnimationSet(CRAZYFOOT))
				{
					m_pAI->Set_State(AI_CRAZYFOOT);
				}
			}
			if (iRand == 6)
			{
				if (m_pAnimator->Set_AnimationSet(BREATH_0))
				{
					m_pAI->Set_State(AI_BREATH);
				}
			}
		}

	}


	if (!m_bIdleTime)
	{
		TurnWork(fTimeDelta);
	}

	if (m_fPlayerDis > 20.f)
	{
		m_pAI->Set_State(AI_TRACE);
	}

	////////////////////////////////

	Collsion_Work();

//	ChangeAni_Work();

	//AI 바뀌는곳..
	ChangeAI_Work();


}

void CAI_Idle_Hanuman::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

void CAI_Idle_Hanuman::TurnWork(const _float& fTimeDelta)
{
	if (m_bLookAtDir)
	{
		_vec3 vLook;
		m_pTransform->Get_Infomation(Engine::INFO_LOOK, &vLook);

		//float fAngle = acosf(D3DXVec3Dot(&m_vRayDir, &vLook));

		_vec3 vUp = _vec3(0.f, 1.f, 0.f);
		_vec3 vRight;

		D3DXVec3Cross(&vRight, &vUp, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		float fCos = D3DXVec3Dot(&vRight, &m_vRayDir);
		float fRealCos = D3DXVec3Dot(&vLook, &m_vRayDir);
		float fAngle = acosf(fRealCos);

		fAngle = D3DXToDegree(fAngle);

		if (fAngle > 180.f)
			fAngle = 360.f - fAngle;

		if (fCos > 0.2f)
		{
			m_pTransform->Rotation(Engine::ROT_Y, 80.f * fTimeDelta);
			m_pAnimator->Set_AnimationSet(ROLL_L);
		}
		else if (fCos < -0.2f)
		{
			m_pTransform->Rotation(Engine::ROT_Y, -80.f * fTimeDelta);
			m_pAnimator->Set_AnimationSet(ROLL_R);
		}
		else
		{
			m_pAnimator->Set_AnimationSet(BATTLE_IDLE);
		}

		//else
		//m_pTransform->Set_Angle(&m_vRayDir);

	}
}

void CAI_Idle_Hanuman::Collsion_Work(void)
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
			m_pAnimator->Set_AnimationSet(DAMAGE);
			break;
		case Engine::COL_FLY:

			break;
		case Engine::COL_SPINA:

			break;
		case Engine::COL_SPINB:
			m_bCol[Engine::COL_SPINB] = true;
			m_pAnimator->Set_AnimationSet(DAMAGE);

			break;
		case Engine::COL_STAMP:
			m_pAnimator->Set_AnimationSet(DAMAGE);

			break;
		case Engine::COL_STUN:
			m_pAnimator->Set_AnimationSet(STUN);
		case Engine::COL_CAPTURE:
			break;
		}
	}
}



CAI_Idle_Hanuman * CAI_Idle_Hanuman::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI)
{
	CAI_Idle_Hanuman* pInstance = new CAI_Idle_Hanuman;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator,
		pRigidbody, pCollider, pColInfo, pDamaged, pAI)))
	{
		MSG_BOX("CAI_Idle Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Idle_Hanuman::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
