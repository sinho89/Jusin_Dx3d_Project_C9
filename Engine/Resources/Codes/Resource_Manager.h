#ifndef Resource_Manager_h__
#define Resource_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

#include "DynamicMesh.h"
#include "StaticMesh.h"
#include "Sphere_Color.h"

BEGIN(Engine)

class CResource;

class ENGINE_DLL CResource_Manager 
	: public CBase
{
public:
	DECLARE_SINGLETON(CResource_Manager)
private:
	explicit CResource_Manager(void);
	virtual ~CResource_Manager(void);
public:
	CResource* Clone_Resource(const _ushort& wContinerIdx, const _tchar* pResourceTag);
public:
	HRESULT Reserve_ContainerSize(const _ushort& wSize);
	HRESULT Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContinerIdx, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCntX = 1, const _ulong& dwCntZ = 1, const _ulong& dwVtxInterval = 1);
	HRESULT Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContinerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt = 1);
	HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName);	

	void Render_Buffer(const _ushort& wContinerIdx, const _tchar* pBufferTag);

	void Reset_Resource(const _ushort& wContinerIdx);
	void Delete_Resource(const _ushort& wContinerIdx, const _tchar* pResourceTag);

public:
	void SetLoadScale(_float fLoadScale = 0);

public:
	_float GetLoadScale(void);

private:
	map<wstring, CResource*>*			m_pmapResource;
	typedef map<wstring, CResource*>	MAPRESOURCE;

private:
	PROJECTID								m_eProjectID;
	_ushort									m_MaxContainerSize;
	_float									m_fLoadScale;

private:
	CResource* Find_Resources(const _ushort& wContinerIdx, const _tchar* pResourceTag);

public:
	const PROJECTID							GetResourceProjectID(void) const { return m_eProjectID; }
	void									SetResourceProjectID(const PROJECTID eProjectID) { m_eProjectID = eProjectID; }

public:
	virtual void Free(void);
};


END

#endif // Resource_Manager_h__
