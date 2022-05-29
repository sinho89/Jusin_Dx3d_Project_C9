#include "Timer_Manager.h"
#include "Timer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimer_Manager)

Engine::CTimer_Manager::CTimer_Manager(void)
{

}

Engine::CTimer_Manager::~CTimer_Manager(void)
{

}

Engine::_float Engine::CTimer_Manager::Get_TimeDelta(const _tchar* pTimerTag) const
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if(pTimer == NULL)
		return 0.f;

	return pTimer->Get_TimeDelta();	
}

void Engine::CTimer_Manager::SetUp_TimeDelta(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if(pTimer == NULL)
		return;

	pTimer->SetUp_TimeDelta();
}

HRESULT Engine::CTimer_Manager::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer*	pTimer = Find_Timer(pTimerTag);

	if(pTimer != NULL)
		return E_FAIL;

	pTimer = CTimer::Create();
	if(pTimer == NULL)
		return E_FAIL;

	m_mapTimers.insert(MAPTIMER::value_type(pTimerTag, pTimer));

	return S_OK;
}

CTimer* Engine::CTimer_Manager::Find_Timer(const _tchar* pTimerTag) const
{
	MAPTIMER::const_iterator	iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), CTag_Finder(pTimerTag));

	if(iter == m_mapTimers.end())
		return NULL;

	return iter->second;
}

void Engine::CTimer_Manager::Free(void)
{
	for_each(m_mapTimers.begin(), m_mapTimers.end(), CRelease_Pair());
	m_mapTimers.clear();
}

