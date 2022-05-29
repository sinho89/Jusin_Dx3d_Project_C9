// For.Resource_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
CResource* Engine::Clone_Resource(const _ushort& wContinerIdx, const _tchar* pResourceTag)
{
	return CResource_Manager::GetInstance()->Clone_Resource(wContinerIdx, pResourceTag);
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
// General ----------------------------1---------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Reserve_ContainerSize(const _ushort& wSize)
{
	return CResource_Manager::GetInstance()->Reserve_ContainerSize(wSize);
}
void Engine::SetLoadScale(_float fLoadScale /*= 0*/)
{
	CResource_Manager::GetInstance()->SetLoadScale(fLoadScale);
}
_float Engine::GetLoadScale(void)
{
	return CResource_Manager::GetInstance()->GetLoadScale();
}
HRESULT Engine::Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContinerIdx, const _tchar* pBufferTag, BUFFERID ID, const _ulong& dwCntX/* = 1*/, const _ulong& dwCntZ/* = 1*/, const _ulong& dwVtxInterval/* = 1*/)
{
	return CResource_Manager::GetInstance()->Ready_Buffers(pGraphicDev, wContinerIdx, pBufferTag, ID, dwCntX, dwCntZ, dwVtxInterval);
}

HRESULT Engine::Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContinerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt)
{
	return CResource_Manager::GetInstance()->Ready_Textures(pGraphicDev, wContinerIdx, pTextureTag, eType, pPath, iCnt);
}
HRESULT Engine::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName)
{
	return CResource_Manager::GetInstance()->Ready_Mesh(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}
void Engine::Render_Buffer(const _ushort& wContinerIdx, const _tchar* pBufferTag)
{
	CResource_Manager::GetInstance()->Render_Buffer(wContinerIdx, pBufferTag);
}

void Engine::Reset_Resource(const _ushort& wContinerIdx)
{
	CResource_Manager::GetInstance()->Reset_Resource(wContinerIdx);
}

void Engine::Delete_Resource(const _ushort& wContinerIdx, const _tchar* pResourceTag)
{
	CResource_Manager::GetInstance()->Delete_Resource(wContinerIdx, pResourceTag);
}
inline void	SetResourceProjectID(const PROJECTID eProjectID)
{
	CResource_Manager::GetInstance()->SetResourceProjectID(eProjectID);
}
inline const PROJECTID	GetResourceProjectID(void)
{
	return CResource_Manager::GetInstance()->GetResourceProjectID();
}

// For.Release System Instance---------------------------------------------------------------------------------------------------------------------------------------
void Engine::Release_Resource(void)
{
	CResource_Manager::GetInstance()->DestroyInstance();
}