#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Export_Engine.h"

USING(Engine)


Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_bSkipScene(false)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}

CGameObject* Engine::CScene::FindWithTag(const _tchar* pObjKey)
{
	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	CGameObject* pGameObject = NULL;

	for(iter; iter != iter_end; iter++)
	{
		pGameObject = iter->second->FindWithTag(pObjKey);

		if(pGameObject != NULL)
			return pGameObject;
	}

	return NULL;
}

CGameObject* Engine::CScene::Find(const _tchar* pObjKey)
{
	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	CGameObject* pGameObject = NULL;

	for(iter; iter != iter_end; iter++)
	{
		pGameObject = iter->second->Find(pObjKey);

		if(pGameObject != NULL)
			return pGameObject;
	}

	return NULL;
}

list<CGameObject*>* Engine::CScene::FindList(const _tchar* pObjKey)
{
	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	list<CGameObject*>* pObjList = NULL;

	for(iter; iter != iter_end; iter++)
	{
		pObjList = iter->second->FindList(pObjKey);

		if(pObjList != NULL)
			return pObjList;
	}

	return NULL;
}

Engine::CLayer* Engine::CScene::FindLayer(WORD eLayerID)
{
	return m_mapLayer.find(eLayerID)->second;
}

map<WORD, CLayer*>* Engine::CScene::FindLayerMap()
{
	return &m_mapLayer;
}

_bool CScene::Get_Delete(void)
{
	return FindLayer(Engine::LAYER_GAMELOGIC)->Get_Delete();
}

PSRWLOCK CScene::Get_Lock(PSRWLOCK * pLock)
{
	return FindLayer(Engine::LAYER_GAMELOGIC)->Get_Lock(pLock);
}

HRESULT CScene::Set_Object(CScene * pCurrentScene)
{
	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	for (iter; iter != iter_end; iter++)
	{
		map<wstring, list<CGameObject*>>* pmapObject = iter->second->Get_MapObject();

		map<wstring, list<CGameObject*>>::iterator iter_map = pmapObject->begin();
		map<wstring, list<CGameObject*>>::iterator iter_map_end = pmapObject->end();

		for (iter_map; iter_map != iter_map_end; iter_map++)
		{
			list<CGameObject*>::iterator iter_list = iter_map->second.begin();
			list<CGameObject*>::iterator iter_list_end = iter_map->second.end();

			for (iter_list; iter_list != iter_list_end; iter_list++)
			{
				if ((*iter_list)->GetStatic())
				{
					(*iter_list)->AddRef();
					pCurrentScene->AddObject(iter->first, iter_map->first.c_str(), (*iter_list));
				}
			}
		}
	}

	return S_OK;
}



void Engine::CScene::AddObject(const WORD& wLayerID, const _tchar* pObjectKey, CGameObject* pGameObject)
{
	MAPLAYER::iterator iter = m_mapLayer.find(wLayerID);

	if(iter == m_mapLayer.end())
		return;

	iter->second->AddObject(pObjectKey, pGameObject);
}


HRESULT Engine::CScene::Start_ForScene(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (iter; iter != iter_end; iter++)
	{
		if(FAILED(iter->second->Start_ForScene()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT Engine::CScene::Ready_Scene(void)
{
	return S_OK;
}

void Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();


	for (iter; iter != iter_end; iter++)
	{
		if (GetSceneID() == SCENE_LOADING)
		{
			if (iter->first != LAYER_UI)
				continue;
			
			iter->second->Update_Layer(fTimeDelta);
		}
		else
			iter->second->Update_Layer(fTimeDelta);

	}	
}

void Engine::CScene::SafeRelease(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();


	for (iter; iter != iter_end; iter++)
	{
		iter->second->SafeRelease();
	}	
}

void Engine::CScene::Free(void)
{
  	for_each(m_mapLayer.begin(), m_mapLayer.end(), CRelease_Pair());
	m_mapLayer.clear();

	Engine::Safe_Release(m_pGraphicDev);
}


