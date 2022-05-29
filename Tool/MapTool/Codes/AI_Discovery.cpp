#include "stdafx.h"
#include "AI_Discovery.h"
#include "Map_Ice.h"
#include "Map_Fire.h"
#include "NavigationMesh.h"
#include "Cube.h"
#include "Player.h"

//맵툴

CAI_Discovery::CAI_Discovery(void)
	:Engine::CAI_State()
	, m_fAggroRad(0.f)
	, m_bAggro(false)
{
}

CAI_Discovery::~CAI_Discovery(void)
{
}


HRESULT CAI_Discovery::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody)
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

	//if (NULL == Engine::FindList(L"Map_Ice"))
	//	return E_FAIL;

	Engine::CGameObject* pGameObject = NULL;

	if (NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		pGameObject = (CMap_Ice*)(*iter);
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);
		for (_int i = 0; i < iSize; ++i)
		{
			Set_MapMesh(ppMesh[i]->Get_Mesh());
		}
	}

	///////////////////////////////////////////

	if (NULL != Engine::FindList(L"Map_Fire"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Fire")->begin();
		pGameObject = (CMap_Fire*)(*iter);
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);
		for (_int i = 0; i < iSize; ++i)
		{
			Set_MapMesh(ppMesh[i]->Get_Mesh());
		}
	}

	if (NULL == Engine::FindList(L"Player"))
		return E_FAIL;

	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Player")->begin();
	pGameObject = (CPlayer*)(*iter);
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_fAggroRad = 30.f;

	return S_OK;
}

void CAI_Discovery::Start_State(void)
{

}

void CAI_Discovery::Start_State(_vec3* pDir, _float fForce)
{

}

void CAI_Discovery::Update_State(const _float & fTimeDelta)
{
	//InitializeCriticalSection(&m_Crt);

	//m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	m_vTargetPos = m_pTargetTransform->Get_Now_Position();
	m_vPlayerPos = m_pTargetTransform->Get_Now_Position();
	m_vPivotPos = m_pTransform->Get_Now_Position();
	m_vPivotPos.y += 2.f;
	m_vRayDir = m_vTargetPos - m_vPivotPos;
	m_fPlayerDis = D3DXVec3Length(&m_vRayDir);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	if (!m_bAggro)
	{
		if (m_fPlayerDis < m_fAggroRad)
		{
			m_pAnimator->Set_AnimationSet(DISCOVERY);
			LookAtDir(&m_vRayDir, fTimeDelta);
		}

		if (m_pAnimator->Is_AnimationEnd(DISCOVERY))
		{
			m_bAggro = true;
			//m_pAnimator->Set_State(MONSTER_FIGHT);
			//m_pAnimator->Set_AnimationSet(MOVE_RUN);
		}
	}


	//LeaveCriticalSection(Get_CriticalSection());

}

void CAI_Discovery::End_State(void)
{

}

void CAI_Discovery::CubeRayCheck(void)
{
	if (NULL != Engine::FindList(L"Cube"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Cube")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Cube")->end();

		//			_float	fU, fV, fDist;
		_float fDist = 99999.f;
		_float rDist = 99999.f;
		BOOL BHit = false;
		_vec3 vDis;
		for (iter; iter != iter_end; ++iter)
		{
			((CCube*)(*iter))->CheckRayCol(&fDist, &m_vPivotPos, &m_vRayDir, rDist);
		}
		//플레이어와의 거리 비교..
		if (fDist < m_fPlayerDis)
		{
			m_bCubeRayCol = true;
		}
		else
		{
			m_bCubeRayCol = false;
		}
	}
}


CAI_Discovery * CAI_Discovery::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody)
{
	CAI_Discovery* pInstance = new CAI_Discovery;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody)))
	{
		MSG_BOX("CAI_Discovery Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Discovery::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
	//WaitForSingleObject(m_hThread, INFINITE);
	//CloseHandle(m_hThread);
	//DeleteCriticalSection(&m_Crt);
	//Engine::Safe_Release(m_ppMapMesh);
}
