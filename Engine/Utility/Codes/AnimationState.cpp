#include "AnimationState.h"

#include "DynamicMesh.h"

USING(Engine)

Engine::CAnimationState::CAnimationState(_uint iAniIndex)
: m_pMesh(NULL)
, m_pAniInfo(NULL)
, m_pmapNextMotion(NULL)
, m_iAniIndex(iAniIndex)
, m_bFirstSetting(true)
{
	
}

Engine::CAnimationState::~CAnimationState(void)
{

}

_bool Engine::CAnimationState::IsPlaying_ThisSection(const _double & dStartTime, const _double & dEndTime)
{
	return m_pMesh->IsPlaying_ThisSection(dStartTime, dEndTime);
}

_bool Engine::CAnimationState::Is_AnimationEnd(const _uint& iIndex, const _double dMagicNumber)
{
	if(m_iAniIndex != iIndex)
		return false;

	return m_pMesh->is_AnimationSetEnd(dMagicNumber);
}

_uint Engine::CAnimationState::Get_AnimationIndex(void)
{
	return m_iAniIndex;
}

void Engine::CAnimationState::Pause_Animation(const _bool & bIsPlay)
{
	m_pMesh->Pause_Animation(bIsPlay);
}

_bool Engine::CAnimationState::Set_AnimationSet(const _uint& iState, const _uint& iIndex)
{
	MAPNANIMATOR::iterator iter = m_pmapNextMotion->find(m_iAniIndex);
	if (iter == m_pmapNextMotion->end())
		return false;

	ANIMATORLIST::iterator iter_list = iter->second.begin();
	ANIMATORLIST::iterator iter_list_end = iter->second.end();

	for (iter_list; iter_list != iter_list_end; iter_list++)
	{
		if ((*iter_list)->iNextIndex == iIndex)
			break;
	}

	if (iter_list == iter_list_end)
		return false;

	if (!(*m_pAniInfo)[iIndex]->pUse[iState])
		return false;

	_double dMagicNumber = max(((*m_pAniInfo)[m_iAniIndex]->tMotionInfo.dEndTime - (*iter_list)->dCancelTime), 0.0);

	if (!m_pMesh->is_AnimationSetEnd(dMagicNumber))
		return false;

	if (m_pMesh->Set_AnimationSet(iIndex, &(*m_pAniInfo)[iIndex]->tMotionInfo, (*iter_list)->dMagicNumber))
	{
		m_iAniIndex = iIndex;
		return true;
	}
	else
		return false;
}

void CAnimationState::Set_TrackSpeed(const _float & fSpeed)
{
	m_pMesh->Set_TrackSpeed(fSpeed);
}

void Engine::CAnimationState::Set_Animator(vector<ANIINFO*>* pAniInfo)
{
	m_pAniInfo = pAniInfo;
}

void Engine::CAnimationState::End_Animation(void)
{
	MAPNANIMATOR::iterator iter = m_pmapNextMotion->find(m_iAniIndex);
	if(iter == m_pmapNextMotion->end())
		return;

	if(iter->second.empty())
		return;

	m_iAniIndex = iter->second.front()->iNextIndex;
	m_pMesh->Set_AnimationSet(m_iAniIndex, &(*m_pAniInfo)[m_iAniIndex]->tMotionInfo, iter->second.front()->dMagicNumber);
}

