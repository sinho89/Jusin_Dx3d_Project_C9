#include "Resource_Manager.h"
#include "Rect_Color.h"
#include "Rect_Texture.h"
#include "Triangle_Color.h"
#include "Triangle_Texture.h"
#include "Cube_Color.h"
#include "Cube_Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NavigationMesh.h"
#include "Texture.h"
#include "Grid_Col.h"
#include "Pentagon_Texture.h"
#include "Trail_Texture.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResource_Manager)

CResource_Manager::CResource_Manager(void)
: m_pmapResource(NULL)
, m_fLoadScale(0.f)
{

}
CResource_Manager::~CResource_Manager(void)
{

}

CResource* CResource_Manager::Clone_Resource(const _ushort& wContinerIdx, const _tchar* pResourceTag)
{
	if(m_pmapResource == NULL)
	{
		MSG_BOX("Resource Continer Empty");
		return NULL;
	}

	CResource*	pPrototype = Find_Resources(wContinerIdx, pResourceTag);

	if (pPrototype == NULL)
	{
		MSG_BOX("Find Resource Failed");
		return NULL;	
	}

	return pPrototype->Clone();
}

HRESULT CResource_Manager::Reserve_ContainerSize(const _ushort& wSize)
{
	if(m_pmapResource != NULL)
		return E_FAIL;

	m_pmapResource = new MAPRESOURCE[wSize];

	m_MaxContainerSize = wSize;

	return S_OK;	
}

HRESULT CResource_Manager::Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContinerIdx, 
										 const _tchar* pBufferTag, BUFFERID eID, 
										 const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxInterval)
{
	if(m_pmapResource == NULL)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResource*	pResource = Find_Resources(wContinerIdx, pBufferTag);

	if(pResource != NULL)
	{
		MSG_BOX("Ready_Buffers Failed");
		return E_FAIL;
	}

	switch(eID)
	{
	case BUFFER_TRICOL:
		pResource = CTriangle_Color::Create(pGraphicDev);
		break;
	case BUFFER_TRITEX:
		pResource = CTriangle_Texture::Create(pGraphicDev);
		break;
	case BUFFER_RCCOL:
		pResource = CRect_Color::Create(pGraphicDev);
		break;
	case BUFFER_RCTEX:
		pResource = CRect_Texture::Create(pGraphicDev);
		break;
	case BUFFER_CUBECOL:
		pResource = NULL;
		break;
	case BUFFER_CUBETEX:
		pResource = CCube_Texture::Create(pGraphicDev);
		break;
	case BUFFER_GRIDCOL:
		pResource = CGrid_Col::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxInterval);
		break;
	case BUFFER_PENTAGON:
		pResource = CPentagon_Texture::Create(pGraphicDev);
		break;
	case BUFFER_TRAILTEX:
		pResource = CTrail_Texture::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxInterval);
		break;
	}

	if(pResource == NULL)
		return E_FAIL;

	m_pmapResource[wContinerIdx].insert(MAPRESOURCE::value_type(pBufferTag, pResource));

	m_fLoadScale += 1.f;

	return S_OK;
}

HRESULT CResource_Manager::Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContinerIdx, 
										  const _tchar* pTextureTag, TEXTURETYPE eType, 
										  const _tchar* pPath, const _uint& iCnt)
{
	if(m_pmapResource == NULL)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResource*	pResource = Find_Resources(wContinerIdx, pTextureTag);

	if(pResource != NULL)
	{
		MSG_BOX("Ready_Textures Failed");
		return E_FAIL;
	}

	pResource = CTexture::Create(pGraphicDev, pPath, eType, iCnt);
	if(pResource == NULL)
		return E_FAIL;

	m_pmapResource[wContinerIdx].insert(MAPRESOURCE::value_type(pTextureTag, pResource));	

	m_fLoadScale += 1.f;

	return S_OK;
}

HRESULT CResource_Manager::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, 
									  const _tchar* pMeshTag, MESHTYPE eType, 
									  const _tchar* pFilePath, const _tchar* pFileName)
{
	if(m_pmapResource == NULL)
	{
		MSG_BOX("Resource Container not Reserved");
		return E_FAIL;
	}

	CResource*	pResource = Find_Resources(wContainerIdx, pMeshTag);

	if(pResource != NULL)
	{
		MSG_BOX("Ready_Mesh Failed");
		return E_FAIL;
	}

	switch(eType)
	{
	case MESH_STATIC:
		pResource = CStaticMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;
	case MESH_DYNAMIC:
		pResource = CDynamicMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;
	case MESH_NAVIGATION:
		pResource = CNavigationMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;
	}	

	if(pResource == NULL)
	{
		MSG_BOX("Ready_Mesh Failed");
		return E_FAIL;
	}

	m_pmapResource[wContainerIdx].insert(MAPRESOURCE::value_type(pMeshTag, pResource));	

	m_fLoadScale += 1.f;

	return S_OK;
}


CResource* CResource_Manager::Find_Resources(const _ushort& wContinerIdx, const _tchar* pResourceTag)
{
	MAPRESOURCE::iterator	iter = m_pmapResource[wContinerIdx].find(pResourceTag);

	if(iter == m_pmapResource[wContinerIdx].end())
		return NULL;

	return iter->second;
}

void CResource_Manager::Render_Buffer(const _ushort& wContinerIdx, const _tchar* pBufferTag)
{
	CResource*	pResource = Find_Resources(wContinerIdx, pBufferTag);

	if(pResource == NULL)
		return;

	((CVIBuffer*)pResource)->Render_Buffer();
}

void Engine::CResource_Manager::SetLoadScale(_float fLoadScale /*= 0*/)
{
	m_fLoadScale = fLoadScale;
}

_float Engine::CResource_Manager::GetLoadScale(void)
{
	return m_fLoadScale;
}

void Engine::CResource_Manager::Reset_Resource(const _ushort& wContinerIdx)
{
	for_each(m_pmapResource[wContinerIdx].begin(), m_pmapResource[wContinerIdx].end(), CRelease_Pair());
	m_pmapResource[wContinerIdx].clear();
}

void Engine::CResource_Manager::Delete_Resource(const _ushort& wContinerIdx, const _tchar* pResourceTag)
{
	if(m_pmapResource == NULL)
		return;

	MAPRESOURCE::iterator	iter = m_pmapResource[wContinerIdx].find(pResourceTag);

	if(iter == m_pmapResource[wContinerIdx].end())
		return;

	Safe_Release(iter->second);
	m_pmapResource[wContinerIdx].erase(iter);
}

void CResource_Manager::Free(void)
{
	for (_ushort i = 0; i < m_MaxContainerSize; ++i)
	{
		for_each(m_pmapResource[i].begin(),m_pmapResource[i].end(), CRelease_Pair());
		m_pmapResource[i].clear();
	}

	Engine::Safe_Delete_Array(m_pmapResource);
}