#include "stdafx.h"
#include "Mistic_State.h"

CMistic_State::CMistic_State(_uint iAniIndex)
	: Engine::CAnimationState(iAniIndex)
	, m_pRigidbody(NULL)
	, m_pTransform(NULL)
	, m_pAnimator(NULL)
	, m_pCalculator(NULL)
	, m_pCollider(NULL)
	, m_pSphereCollider(NULL)
	, m_pWeapon(NULL)
	, m_pPlayer(NULL)
	, m_pPlayerInfo(NULL)
{
	ZeroMemory(m_bAction, sizeof(_bool) * CMistic::ACTION_END);
}

CMistic_State::~CMistic_State(void)
{

}

const _bool * CMistic_State::Get_CurAction(void) const
{
	return m_bAction;
}

void CMistic_State::Start_State(void)
{

}

void CMistic_State::Update_State(const _float & fTimeDelta)
{

}

void CMistic_State::End_State(void)
{

}

void CMistic_State::EndCheck(void)
{
	_double dPosition = m_pMesh->Get_TrackPos();

	if (!(*m_pAniInfo)[m_iAniIndex]->tMotionInfo.bIsLoop)
	{
		if (dPosition >= (*m_pAniInfo)[m_iAniIndex]->tMotionInfo.dEndTime)
			CAnimationState::End_Animation();
	}
}

void CMistic_State::Free(void)
{
	Engine::Safe_Release(m_pMesh);
}