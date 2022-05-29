#include "stdafx.h"
#include "AI_Death_Hanuman.h"
#include "Map_Ice.h"
#include "AI.h"

//클라..

CAI_Death_Hanuman::CAI_Death_Hanuman(void)
	:CAI_Hanuman()
	, m_fDownTime(0.f)
	, m_bStand(false)
	, m_iDirection(0)
	, m_bOne(false)
	, m_bDown(false)
{
}

CAI_Death_Hanuman::~CAI_Death_Hanuman(void)
{
}


HRESULT CAI_Death_Hanuman::Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
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

void CAI_Death_Hanuman::Start_State(void)
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


void CAI_Death_Hanuman::Update_State(const _float & fTimeDelta)
{

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	m_pAnimator->Set_AnimationSet(BATTLE_IDLE);


}

void CAI_Death_Hanuman::End_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

CAI_Death_Hanuman * CAI_Death_Hanuman::Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
	Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
	Engine::COLINFO* pColInfo, _bool* pDamaged,
	Engine::CAI* pAI)
{
	CAI_Death_Hanuman* pInstance = new CAI_Death_Hanuman;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody
	, pCollider, pColInfo, pDamaged, pAI)))
	{
		MSG_BOX("CAI_Death_Hanuman Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Death_Hanuman::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
