#include "Frame_Manager.h"
#include "Frame.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrame_Manager)

Engine::CFrame_Manager::CFrame_Manager(void)
{

}

Engine::CFrame_Manager::~CFrame_Manager(void)
{

}

Engine::_bool Engine::CFrame_Manager::isPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	CFrame*	pFrame = Find_Frame(pFrameTag);

	if(pFrame == NULL)
		return false;

	return pFrame->isPermit_Call(fTimeDelta);
}

HRESULT Engine::CFrame_Manager::Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	CFrame*	pFrame = Find_Frame(pFrameTag);

	if(pFrame != NULL)
		return E_FAIL;

	pFrame = CFrame::Create(fCallLimit);
	if(pFrame == NULL)
		return E_FAIL;

	m_mapFrames.insert(MAPFRAMES::value_type(pFrameTag, pFrame));

	return S_OK;	
}

CFrame* Engine::CFrame_Manager::Find_Frame(const _tchar* pFrameTag)
{
	MAPFRAMES::iterator	iter = find_if(m_mapFrames.begin(), m_mapFrames.end(), CTag_Finder(pFrameTag));

	if(iter == m_mapFrames.end())
		return NULL;

	return iter->second;
}

void Engine::CFrame_Manager::Free(void)
{
	for_each(m_mapFrames.begin(), m_mapFrames.end(), CRelease_Pair());
	m_mapFrames.clear();
}
