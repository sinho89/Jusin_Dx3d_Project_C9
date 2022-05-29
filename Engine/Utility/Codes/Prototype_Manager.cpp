#include "Prototype_Manager.h"
#include "Component.h"

USING(Engine)
IMPLEMENT_SINGLETON(CPrototype_Manager)

Engine::CPrototype_Manager::CPrototype_Manager(void)
{

}

Engine::CPrototype_Manager::~CPrototype_Manager(void)
{

}

HRESULT Engine::CPrototype_Manager::Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	CComponent*		pPrototype = Find_Prototype(pProtoTag);
	if(pPrototype != NULL)
		return E_FAIL;

	m_mapPrototype.insert(MAPPROTOTYPE::value_type(pProtoTag, pInstance));

	return S_OK;
}

CComponent* Engine::CPrototype_Manager::Clone_Prototype(const _tchar* pProtoTag)
{
	CComponent*		pPrototype = Find_Prototype(pProtoTag);
	if(pPrototype == NULL)
		return NULL;

	return pPrototype->Clone();
}

CComponent* Engine::CPrototype_Manager::Find_Prototype(const _tchar* pProtoTag)
{
	MAPPROTOTYPE::iterator	iter = find_if(m_mapPrototype.begin(), m_mapPrototype.end(), CTag_Finder(pProtoTag));
	if(iter == m_mapPrototype.end())
		return NULL;

	return iter->second;
}

void Engine::CPrototype_Manager::Free(void)
{
	for_each(m_mapPrototype.begin(), m_mapPrototype.end(), CRelease_Pair());
	m_mapPrototype.clear();
}

