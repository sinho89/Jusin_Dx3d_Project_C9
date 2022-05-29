#include "AI.h"
#include "AI_State.h"

Engine::CAI::CAI(void)
: m_iState(0)
, m_bAIStop(false)
{
}

Engine::CAI::~CAI(void)
{
}

void Engine::CAI::Set_LookAtDir(_bool bCheck)
{
	MAPSTATE::iterator iter = m_mapState.begin();
	MAPSTATE::iterator iter_end = m_mapState.end();

	for (iter; iter != iter_end; ++iter)
	{
		iter->second->Set_LookAtDir(bCheck);
	}
}

void Engine::CAI::Set_State(_uint iIndex)
{

	MAPSTATE::iterator iter = m_mapState.find(m_iState);

	if (iter == m_mapState.end())
		return;

	//기존 State의 EndState를 호출
	iter->second->End_State();

	//새로운 State의 StartState를 호출
	m_iState = iIndex;

	iter = m_mapState.find(m_iState);

	iter->second->Start_State();

}

void Engine::CAI::Set_State(_uint iIndex, CAI_State * pState)
{
	if (pState == NULL)
		return;

	m_mapState.insert(MAPSTATE::value_type(iIndex, pState));
}

void Engine::CAI::Set_AI_Stop(void)
{
	m_bAIStop = true;
}

void Engine::CAI::Set_AI_Update(void)
{
	m_bAIStop = false;
}

void Engine::CAI::Set_StartState(void)
{
	MAPSTATE::iterator iter = m_mapState.begin();
	MAPSTATE::iterator iter_end = m_mapState.end();

	for (iter; iter != iter_end; ++iter)
	{
		iter->second->Start_State();
	}
}

void Engine::CAI::Set_StartPos(_uint iIndex, _vec3 * pPos)
{
	MAPSTATE::iterator iter = m_mapState.find(iIndex);

	if (iter == m_mapState.end())
		return;

	iter->second->Set_StartPos(pPos);
}

Engine::CAI_State * Engine::CAI::Get_State(const _uint & iState)
{
	MAPSTATE::iterator iter = m_mapState.find(iState);
	if (iter == m_mapState.end())
		return NULL;

	return iter->second;
}

int Engine::CAI::Get_CurState(void)
{
	return m_iState;
}

void Engine::CAI::Update_Component(const _float & fTimeDelta)
{
	if (!m_bAIStop)
	{
		MAPSTATE::iterator iter = m_mapState.find(m_iState);

		if (iter == m_mapState.end())
			return;

		iter->second->Update_State(fTimeDelta);
	}

}

HRESULT Engine::CAI::Ready_AI(void)
{
	
	return S_OK;
}

Engine::CAI * Engine::CAI::Create(void)
{
	CAI*		pInstance = new CAI;

	if (FAILED(pInstance->Ready_AI()))
	{
		MSG_BOX("CAI Createtd Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CAI::Free(void)
{
	for_each(m_mapState.begin(), m_mapState.end(), CRelease_Pair());
	m_mapState.clear();
}
