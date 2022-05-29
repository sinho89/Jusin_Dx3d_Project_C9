#include "stdafx.h"
#include "AI_Hanuman.h"
#include "Map_Ice.h"
#include "AI.h"
#include "Cube.h"
#include "NavigationMesh.h"

//클라..

CAI_Hanuman::CAI_Hanuman(void)
	:Engine::CAI_State()
	,m_iPatternCount(0)
{
}

CAI_Hanuman::~CAI_Hanuman(void)
{
}

void CAI_Hanuman::Start_State(void)
{
}

void CAI_Hanuman::Update_State(const _float & fTimeDelta)
{
}

void CAI_Hanuman::End_State(void)
{
}

void CAI_Hanuman::Set_Ray(void)
{
	m_vTargetPos = m_pTargetTransform->Get_Now_Position();
	m_vPlayerPos = m_pTargetTransform->Get_Now_Position();
	m_vPivotPos = m_pTransform->Get_Now_Position();
	m_vPos = m_pTransform->Get_Now_Position();
	m_vPivotPos.y += 2.f;
	m_vRayDir = m_vTargetPos - m_vPivotPos;
	m_fPlayerDis = D3DXVec3Length(&m_vRayDir);
	m_dwIdx = m_pNavigationMesh->Get_CurrentCellIndex(&m_vPos);
	m_vLook = m_pTransform->Get_Now_Look();
	//D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void CAI_Hanuman::ChangeAni_Work(void)
{

	//if (m_pAnimator->Is_AnimationEnd(SPIN_A, 0.1))
	//{

	//	m_pAnimator->Set_AnimationSet(SPIN_B);
	//}

	//if (m_pAnimator->Get_AnimationIndex() == DAM_UPPER_B)
	//{
	//	//밑으로 찍었을때.....한번위로 점프시킴..
	//	if (m_bCol[Engine::COL_SPINB] && m_pCalculator->Get_IsCol())
	//	{
	//		Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.f, 1);
	//		m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
	//		if (m_pColInfo->fJumpForce != 0.f)
	//		{
	//			m_pCalculator->Set_Jump(true);
	//			m_pRigidbody->Set_Jump(m_pColInfo->fJumpForce);
	//			//찍기 끔..
	//			m_bCol[Engine::COL_SPINB] = false;
	//			m_bSpinB = true;
	//		}
	//		else
	//		{
	//			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.f, 1);
	//			m_bCol[Engine::COL_SPINB] = false;
	//			m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
	//			m_bSpinB = false;
	//		}
	//	}
	//	else if (!m_bCol[Engine::COL_SPINB] && m_pCalculator->Get_IsCol())
	//	{
	//		Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.f, 1);
	//		m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
	//		m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
	//	}
	//}

	//if (m_pAnimator->Get_AnimationIndex() == SPIN_B)
	//{
	//	if (m_pCalculator->Get_IsCol())
	//	{
	//		Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.f, 1);
	//		m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
	//		m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
	//	}
	//}

	//if (m_pAnimator->Is_AnimationEnd(DAM_HARD, 0.1))
	//{
	//	Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.f, 1);
	//	m_pAnimator->Set_AnimationSet(DAM_UPPER_C);
	//	m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
	//}

	//if (m_pAnimator->Is_AnimationEnd(STUN, 0.1))
	//{
	//	m_pAnimator->Set_AnimationSet(STAND_DOWN);
	//}

	//if (JUMP_B == m_pAnimator->Get_AnimationIndex())
	//{
	//	if (m_pCalculator->Get_IsCol())
	//	{
	//		m_pAnimator->Set_AnimationSet(JUMP_C);
	//	}
	//}
}

void CAI_Hanuman::ChangeAI_Work(void)
{
	////AI 바뀌는곳..
	if (m_pAnimator->Is_AnimationEnd(STUN, 0.1))
	{
		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
		{
			m_pAI->Set_State(AI_IDLE);
		}
	}

	if (m_pAnimator->Is_AnimationEnd(DAMAGE, 0.1))
	{
		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
		{
			m_pAI->Set_State(AI_IDLE);
		}
	}

	//if (m_pAnimator->Is_AnimationEnd(DAM_UPPER_C, 0.1))
	//{
	//	if (m_pCalculator->Get_IsCol())
	//	{
	//		if (!m_bSpinB)
	//		{
	//			m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
	//			m_pAI->Set_State(AI_DOWN);
	//		}
	//		else
	//		{
	//			m_pAnimator->Set_AnimationSet(DAM_UPPER_B);
	//			m_bSpinB = false;
	//		}
	//	}
	//}

	//if (m_pAnimator->Is_AnimationEnd(DAM_H, 0.1))
	//{
	//	m_pRigidbody->Set_AccMotion(&m_pColInfo->vDir, 0.f);
	//	m_pAI->Set_State(AI_IDLE);
	//}

	//if (m_pAnimator->Is_AnimationEnd(STAND_DOWN, 0.1))
	//{
	//	m_pAI->Set_State(AI_DOWN);
	//	m_bCol[Engine::COL_STAMP] = false;
	//}
}

void CAI_Hanuman::Free(void)
{

}

void CAI_Hanuman::CubeRayCheck(void)
{
	if (NULL != Engine::FindList(L"Cube"))
	{
		//	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Cube")->begin();
		//list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Cube")->end();
		list<Engine::CGameObject*>::iterator iter = m_pCubeList->begin();
		list<Engine::CGameObject*>::iterator iter_end = m_pCubeList->end();
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
