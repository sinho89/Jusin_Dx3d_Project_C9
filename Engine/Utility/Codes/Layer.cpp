#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

USING(Engine)

Engine::CLayer::CLayer(void)
: m_bDelete(false)
, m_psrwLock(NULL)
{
	//InitializeSRWLock(m_psrwLock);
}

Engine::CLayer::~CLayer(void)
{

}

CGameObject* Engine::CLayer::FindWithTag(const _tchar* pObjKey)
{
	MAPOBJLIST::iterator iter = m_mapObjlist.find(pObjKey);
	if(iter != m_mapObjlist.end())
	{
		if(!iter->second.empty())
			return iter->second.front();
	}
	return NULL;
}

CGameObject* Engine::CLayer::Find(const _tchar* pObjKey)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for(iter; iter != iter_end; iter++)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for(iterList; iterList != iterList_end; iterList++)
		{
			wstring wstrName = (*iterList)->GetName();
			if(!lstrcmp(pObjKey, wstrName.c_str()))
				return (*iterList);
		}
	}

	return NULL;
}

list<CGameObject*>* Engine::CLayer::FindList(const _tchar* pObjKey)
{
	MAPOBJLIST::iterator iter = m_mapObjlist.find(pObjKey);
	if(iter != m_mapObjlist.end())
	{
		if(!iter->second.empty())
			return &iter->second;
	}
	return NULL;
}

map<wstring, list<CGameObject*>>* CLayer::Get_MapObject(void)
{
	return &m_mapObjlist;
}

_bool CLayer::Get_Delete(void)
{
	return m_bDelete;
}

PSRWLOCK CLayer::Get_Lock(PSRWLOCK * pLock)
{
	if (pLock == NULL)
		return NULL;

	*pLock = m_psrwLock;

	return m_psrwLock;
}

HRESULT Engine::CLayer::AddObject(const _tchar* pObjectKey, CGameObject* pGameObject)
{
	if(pGameObject == NULL)
		return E_FAIL;

	MAPOBJLIST::iterator iter = m_mapObjlist.find(pObjectKey);
	if(iter == m_mapObjlist.end())
		m_mapObjlist[pObjectKey] = OBJECTLIST();

	m_mapObjlist[pObjectKey].push_back(pGameObject);

	return S_OK;
}


HRESULT Engine::CLayer::Start_ForScene(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();	
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for(iter; iter != iter_end; iter++)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for(iterList; iterList != iterList_end; iterList++)
		{
			if(FAILED((*iterList)->Start_ForScene()))
				return E_FAIL;
		}
	}
	return S_OK;
}

void Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for(iter; iter != iter_end; iter++)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for(iterList; iterList != iterList_end;)
		{
			
			if((*iterList)->GetState() == STATE_DESTROY)
			{
				//AcquireSRWLockExclusive(m_psrwLock);
				m_bDelete = true;
				Safe_Release(*iterList);
				iterList = iter->second.erase(iterList);
				//ReleaseSRWLockExclusive(m_psrwLock);
			}
			else
			{
				m_bDelete = false;
				if((*iterList)->GetActive())
					(*iterList)->Update_Object(fTimeDelta);

				iterList++;
			}
		}
	}

}

void Engine::CLayer::Render_Layer(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for(iter; iter != iter_end; iter++)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for(iterList; iterList != iterList_end; iterList++)
		{
			if((*iterList)->GetActive())
				(*iterList)->Render_Object();
		}
	}
}

void Engine::CLayer::SafeRelease(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for(iter; iter != iter_end; iter++)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for(iterList; iterList != iterList_end;)
		{
			if((*iterList)->GetState() == STATE_DESTROY)
			{
				Safe_Release(*iterList);
				iterList = iter->second.erase(iterList);
			}
			else
				iterList++;
		}
	}
}

CLayer* Engine::CLayer::Create(void)
{
	return new CLayer;
}

void Engine::CLayer::Free(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for(iter; iter != iter_end; iter++)
	{
		for_each(iter->second.begin(), iter->second.end(), CRelease_Single());
		iter->second.clear();
	}

	m_mapObjlist.clear();
}

