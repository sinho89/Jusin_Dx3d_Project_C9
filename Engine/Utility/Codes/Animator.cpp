#include "Animator.h"

#include "AnimationState.h"

USING(Engine)

Engine::CAnimator::CAnimator(void)
: m_iState(0)
{

}

Engine::CAnimator::~CAnimator(void)
{

}

_bool Engine::CAnimator::Is_AnimationEnd(const _uint& iIndex, const _double dMagicNumber)
{
	MAPSTATE::iterator iter = m_mapState.find(m_iState);
	if(iter == m_mapState.end())
		return false;

	return iter->second->Is_AnimationEnd(iIndex, dMagicNumber);
}

_uint Engine::CAnimator::Get_AnimationIndex(void)
{
	MAPSTATE::iterator iter = m_mapState.find(m_iState);
	if(iter == m_mapState.end())
		return 0;
	return iter->second->Get_AnimationIndex();
}

_bool CAnimator::IsPlaying_ThisSection(const _double & dStartTime, const _double & dEndTime)
{
	MAPSTATE::iterator iter = m_mapState.find(m_iState);
	if (iter == m_mapState.end())
		return false;
	return iter->second->IsPlaying_ThisSection(dStartTime, dEndTime);
}

void CAnimator::Pause_Animation(const _bool & bIsPlay)
{
	MAPSTATE::iterator iter = m_mapState.find(m_iState);
	if (iter == m_mapState.end())
		return;
	iter->second->Pause_Animation(bIsPlay);
}

const CAnimationState * CAnimator::Get_CurState(void) 
{
	MAPSTATE::iterator iter = m_mapState.find(m_iState);
	if (iter == m_mapState.end())
		return NULL;
	return iter->second;
}

const CAnimationState * CAnimator::Get_State(const _uint & iState)
{
	MAPSTATE::iterator iter = m_mapState.find(iState);
	if (iter == m_mapState.end())
		return NULL;
	return iter->second;
}

_uint CAnimator::Get_State(void)
{
	return m_iState;
}

void CAnimator::Reset_State(void)
{
	for_each(m_mapState.begin(), m_mapState.end(), CRelease_Pair());
	m_mapState.clear();
}

_bool Engine::CAnimator::Set_AnimationSet(const _uint& iIndex)
{
	MAPSTATE::iterator iter = m_mapState.find(m_iState);
	if(iter == m_mapState.end())
		return false;

	return iter->second->Set_AnimationSet(m_iState, iIndex);
}

void CAnimator::Set_AnimationSet(const _uint & iState, const _uint iIndex)
{
	MAPSTATE::iterator iter = m_mapState.find(iState);
	if (iter == m_mapState.end())
		return;
	iter->second->Set_AnimationSet(iState, iIndex);
}

void CAnimator::Set_TrackSpeed(const _float & fSpeed)
{
	MAPSTATE::iterator iter = m_mapState.find(m_iState);
	if (iter == m_mapState.end())
		return;

	iter->second->Set_TrackSpeed(fSpeed);
}

void Engine::CAnimator::Set_Animator(vector<ANIINFO*>* pAniInfo)
{
	if(pAniInfo == NULL)
		return;

	MAPSTATE::iterator iter = m_mapState.begin();
	MAPSTATE::iterator iter_end = m_mapState.end();

	for(iter; iter != iter_end; iter++)
	{
		iter->second->Set_Animator(pAniInfo);
	}
}

void Engine::CAnimator::Set_State(const _uint& iIndex, CAnimationState* pAnimationState)
{
	if(pAnimationState == NULL)
		return;

	m_mapState.insert(MAPSTATE::value_type(iIndex, pAnimationState));
}

void Engine::CAnimator::Set_State(const _uint& iIndex)
{
	m_mapState[m_iState]->End_State();
	m_iState = iIndex;	
	m_mapState[m_iState]->Start_State();
}

void Engine::CAnimator::Update_Component(const _float& fTimeDelta)
{
	MAPSTATE::iterator iter = m_mapState.find(m_iState);
	
	if(iter == m_mapState.end())
		return;

	iter->second->Update_State(fTimeDelta);
}

HRESULT Engine::CAnimator::Ready_Animator(void)
{
	return S_OK;
}

CAnimator* Engine::CAnimator::Create(void)
{
	CAnimator*		pInstance = new CAnimator;

	if(FAILED(pInstance->Ready_Animator()))
	{
		MSG_BOX("CAnimator Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CAnimator::Free(void)
{
	for_each(m_mapState.begin(), m_mapState.end(), CRelease_Pair());
	m_mapState.clear();
}

