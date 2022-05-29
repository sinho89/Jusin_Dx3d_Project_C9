#include "stdafx.h"
#include "Witch_State.h"

#include "Sound.h"


CWitch_State::CWitch_State(_uint iAniIndex)
: Engine::CAnimationState(iAniIndex)
, m_pRigidbody(NULL)
, m_pTransform(NULL)
, m_pAnimator(NULL)
, m_pCalculator(NULL)
, m_pCollider(NULL)
, m_pSphereCollider(NULL)
, m_pSound(NULL)
, m_pLeftSword(NULL)
, m_pRightSword(NULL)
, m_pPlayer(NULL)
, m_pPlayerInfo(NULL)
{
	ZeroMemory(m_bAction, sizeof(_bool) * CWitchBlade::ACTION_END);
}

CWitch_State::~CWitch_State(void)
{

}

const _bool * CWitch_State::Get_CurAction(void) const
{
	return m_bAction;
}

void CWitch_State::Start_State(void)
{

}

void CWitch_State::Update_State(const _float & fTimeDelta)
{

}

void CWitch_State::End_State(void)
{

}

void CWitch_State::EndCheck(void)
{
	_double dPosition = m_pMesh->Get_TrackPos();

	if (!(*m_pAniInfo)[m_iAniIndex]->tMotionInfo.bIsLoop)
	{
		if (dPosition >= (*m_pAniInfo)[m_iAniIndex]->tMotionInfo.dEndTime)
			CAnimationState::End_Animation();
	}
}

void CWitch_State::Free(void)
{
	Engine::Safe_Release(m_pMesh);
}