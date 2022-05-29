#ifndef Export_Resource_h__
#define Export_Resource_h__

#include "Resource_Manager.h"

// Á¤ÀÇ
BEGIN(Engine)

// For.Resource_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline CResource*		Clone_Resource(const _ushort& wContinerIdx, const _tchar* pResourceTag);
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
// General ----------------------------1---------------------------------------------------------------------------------------------------------------------------
inline HRESULT			Reserve_ContainerSize(const _ushort& wSize);
inline HRESULT			Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContinerIdx, const _tchar* pBufferTag, BUFFERID ID, const _ulong& dwCntX = 1, const _ulong& dwCntZ = 1, const _ulong& dwVtxInterval = 1);
inline HRESULT			Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContinerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt = 1);
inline HRESULT			Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName);

inline void				Render_Buffer(const _ushort& wContinerIdx, const _tchar* pBufferTag);
inline void				Reset_Resource(const _ushort& wContinerIdx);
inline void				Delete_Resource(const _ushort& wContinerIdx, const _tchar* pResourceTag);
inline void				SetLoadScale(_float fLoadScale = 0);

inline void				SetResourceProjectID(const PROJECTID eProjectID);
inline const PROJECTID	GetResourceProjectID(void);

inline _float			GetLoadScale(void);
// For.Release System Instance---------------------------------------------------------------------------------------------------------------------------------------
inline void				Release_Resource(void);

#include "Export_Resource.inl"

END



#endif // Export_System_h__
