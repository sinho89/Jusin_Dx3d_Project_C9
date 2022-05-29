#include "stdafx.h"
#include "AI_Trace.h"
#include "Map_Ice.h"
#include "Map_Fire.h"
#include "NavigationMesh.h"
#include "Cube.h"
#include "Player.h"

//맵툴

CAI_Trace::CAI_Trace(void)
	:Engine::CAI_State()
{
}

CAI_Trace::~CAI_Trace(void)
{
}

unsigned int APIENTRY CAI_Trace::Thread_Main(void* pArg)
{
	CAI_Trace* pTrace = (CAI_Trace*)pArg;

	_uint		iFlag = 0;
	EnterCriticalSection(pTrace->Get_CriticalSection());

	//pTrace->Update_State();

	LeaveCriticalSection(pTrace->Get_CriticalSection());

	return iFlag;
}

HRESULT CAI_Trace::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
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

	m_fMonsterSpeed = 10.f;
	return S_OK;
}

void CAI_Trace::Start_State(void)
{

}

void CAI_Trace::Start_State(_vec3 * pDir, _float fForce)
{

}

void CAI_Trace::Update_State(const _float & fTimeDelta)
{
	//InitializeCriticalSection(&m_Crt);

	//m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	//if (m_pAnimator->Is_AnimationEnd(JUMP_C, 0.2f))
	//{
	//	m_pAnimator->Set_AnimationSet(BATTLE_IDLE);
	//}

	m_vTargetPos = m_pTargetTransform->Get_Now_Position();
	m_vPlayerPos = m_pTargetTransform->Get_Now_Position();
	m_vPivotPos = m_pTransform->Get_Now_Position();
	m_vPivotPos.y += 2.f;
	m_vRayDir = m_vTargetPos - m_vPivotPos;
	m_fPlayerDis = D3DXVec3Length(&m_vRayDir);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	PlayerRayCheck();
	CubeRayCheck();
	
	if (!m_bJump && !m_bJumpMove && !m_bStop)
	{
		TraceMove(fTimeDelta);
	}

	if (m_bJump && !m_bJumpMove)
	{
		//Set_Force로 점프를하고
		//m_pRigidbody->Set_Force(0.7f);
		m_pCalculator->Set_Jump(true);
		m_pRigidbody->Set_Jump(0.7f);

		m_pAnimator->Set_AnimationSet(JUMP_A);
		m_bJumpMove = true;
	}

	else if (m_bJump && m_bJumpMove)
	{
		//레이방향으로 이동...
		_vec3 vPos = *m_pTransform->Get_Infomation(Engine::INFO_POSITION, &vPos);
		D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
		_vec3 vEndPos = vPos + m_vRayDir * fTimeDelta * m_fMonsterSpeed;
		m_pTransform->Set_Position(&vEndPos);

		if (!m_pCalculator->Get_IsCol())
		{
			if (m_pAnimator->Is_AnimationEnd(JUMP_A, 0.1f))
			{
				m_pAnimator->Set_AnimationSet(JUMP_B);
			}
			if (m_pAnimator->Is_AnimationEnd(JUMP_B, 0.1f))
			{
				m_pAnimator->Set_AnimationSet(JUMP_B);
			}
		}

		if (m_pCalculator->Get_IsCol())
		{
			if (m_pAnimator->Get_AnimationIndex() == JUMP_A)
			{
				m_pAnimator->Set_AnimationSet(JUMP_C);
			}
			if (m_pAnimator->Get_AnimationIndex() == JUMP_B)
			{
				m_pAnimator->Set_AnimationSet(JUMP_C);
			}
			if (m_pAnimator->Get_AnimationIndex() == MOVE_RUN)
			{
				m_pAnimator->Set_AnimationSet(JUMP_C);
			}

			m_bJump = false;
			m_bJumpMove = false;
			m_bStop = true;
		}

		//if (m_pAnimator->Is_AnimationEnd(JUMP_A, 0.1))
		//{
		//	if (!m_pCalculator->Get_IsCol())
		//	{
		//		m_pAnimator->Set_AnimationSet(JUMP_B);
		//	}
		//	if (m_pCalculator->Get_IsCol())
		//	{
		//		m_pAnimator->Set_AnimationSet(JUMP_C);
		//		m_bJump = false;
		//		m_bJumpMove = false;
		//		m_bStop = true;
		//	}
		//}
		//if (m_pAnimator->Is_AnimationEnd(JUMP_B, 0.1))
		//{
		//	if (!m_pCalculator->Get_IsCol())
		//	{
		//		m_pAnimator->Set_AnimationSet(JUMP_B);
		//	}
		//	if (m_pCalculator->Get_IsCol())
		//	{
		//		m_pAnimator->Set_AnimationSet(JUMP_C);
		//		m_bJump = false;
		//		m_bJumpMove = false;
		//		m_bStop = true;
		//	}
		//}

		//if (m_pAnimator->Is_AnimationEnd(MOVE_RUN, 0.1))
		//{
		//	if (!m_pCalculator->Get_IsCol())
		//	{
		//		m_pAnimator->Set_AnimationSet(JUMP_B);
		//	}
		//	if (m_pCalculator->Get_IsCol())
		//	{
		//		m_pAnimator->Set_AnimationSet(JUMP_C);
		//		m_bJump = false;
		//		m_bJumpMove = false;
		//		m_bStop = true;
		//	}
		//}

	}

	if (m_bStop)
	{
		m_pAnimator->Set_AnimationSet(MOVE_RUN);
		m_fStopTime += fTimeDelta;
		if (m_fStopTime > 1.5f)
		{
			m_fStopTime = 0.f;
			m_bStop = false;
		}
	}


	//LeaveCriticalSection(Get_CriticalSection());

}

void CAI_Trace::End_State(void)
{
	
}

void CAI_Trace::CubeRayCheck(void)
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

void CAI_Trace::TraceMove(const _float& fTimeDelta)
{
	_vec3 vPos;
	m_pTransform->Get_Infomation(Engine::INFO_POSITION, &vPos);
	_vec3 vEndPos;
	//레이가 플레이어와의 직선거리보다 길면...(장애물X)
	if (m_fRayDis > m_fPlayerDis)
	{
		if (!m_bCubeRayCol)
		{
			m_bRayMove = true;
			m_bAStarMove = false;
			vEndPos = vPos + m_vRayDir * fTimeDelta * m_fMonsterSpeed;
			m_pTransform->Set_Position(&vEndPos);
			_ulong dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&vPos);

			if (!m_bJump && m_pCalculator->Get_IsCol())
			{
				m_pAnimator->Set_AnimationSet(MOVE_RUN);
				//여기서 m_bJump를 트루 반환하게 되면..
				m_bJump = m_pNavigationMesh->Jump_Check_NaviSide(&vEndPos, dwIdx);
				if (m_bJump)
					m_pAnimator->Set_AnimationSet(JUMP_A);
			}
		}
		else
		{
			//A스타 이동....
			m_bAStarMove = true;
			m_bRayMove = false;
			m_pNavigationMesh->FindPath(&vPos, &m_vPlayerPos);
			m_pTransform->Set_Position(&m_pNavigationMesh->AStarMove(&vPos));
			m_pAnimator->Set_AnimationSet(MOVE_RUN);
		}

	}


	//레이가 플레이어와의 직선거리보다 짧으면..(장애물O)
	else
	{
		//짧더라도 장애물이 "바닥"일 경우!
		if (m_vTargetPos.y < vPos.y + 1.f)
		{
			if (!m_bCubeRayCol)
			{
				m_pAnimator->Set_AnimationSet(MOVE_RUN);
				m_bRayMove = true;
				m_bAStarMove = false;

				vEndPos = vPos + m_vRayDir * fTimeDelta * m_fMonsterSpeed;
				m_pTransform->Set_Position(&vEndPos);
				_ulong dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&vPos);
				//cout << "바닥 인덱스 : " << dwIdx << endl;
				//m_pTransCom->Set_Position(&m_pNaviMeshCom->Move_OnNaviMesh_Tool(&vPos, &m_vRayDir, 0.1f, dwIdx));

				if (!m_bJump && m_pCalculator->Get_IsCol())
				{
					m_bJump = m_pNavigationMesh->Jump_Check_NaviSide(&vEndPos, dwIdx);
					if (m_bJump)
						m_pAnimator->Set_AnimationSet(JUMP_A);
				}
			}
			else
			{
				m_bRayMove = false;
				m_bAStarMove = true;
				//A스타 이동....
				m_pNavigationMesh->FindPath(&vPos, &m_vPlayerPos);
				m_pTransform->Set_Position(&m_pNavigationMesh->AStarMove(&vPos));
				m_pAnimator->Set_AnimationSet(MOVE_RUN);
			}
		}
		//장애물이 바닥이 아니고 벽이라면...
		else
		{
			m_bRayMove = false;
			m_bAStarMove = true;
			//A스타 이동....
			m_pNavigationMesh->FindPath(&vPos, &m_vPlayerPos);
			m_pTransform->Set_Position(&m_pNavigationMesh->AStarMove(&vPos));
			m_pAnimator->Set_AnimationSet(BATTLE_IDLE);
		}
	}
	if (m_bRayMove)
	{
		LookAtDir(&m_vRayDir, fTimeDelta);

	}
	else
	{
		LookAtDir(m_pNavigationMesh->GetAStarDir(), fTimeDelta);
	}
}

CAI_Trace * CAI_Trace::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody)
{
	CAI_Trace* pInstance = new CAI_Trace;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody)))
	{
		MSG_BOX("CAI_Trace Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Trace::Free(void)
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
