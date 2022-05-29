#ifndef HierarchyLoader_h__
#define HierarchyLoader_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CHierarchyLoader 
	: public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath);
	virtual ~CHierarchyLoader(void);
public:
	STDMETHOD(Ready_Loader)(void);
public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree); 
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	const _tchar*					m_pPath;
	PROJECTID						m_eProjectID;
private:
	void Allocate_Name(char** ppName, const char* pFrameName);
public:
	static CHierarchyLoader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath);
public:
	_ulong Release(void);
};



END

#endif // HierarchyLoader_h__
