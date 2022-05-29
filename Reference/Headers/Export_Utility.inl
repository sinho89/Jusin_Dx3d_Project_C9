#include "Export_Utility.h"


// For.Management Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline Engine::CGameObject* Engine::FindWithTag(const _tchar* pObjTag)
{
	return Engine::CManagement::GetInstance()->FindWithTag(pObjTag);
}
inline list<Engine::CGameObject*>* Engine::FindList(const _tchar* pObjTag)
{
	return Engine::CManagement::GetInstance()->FindList(pObjTag);
}
inline Engine::CGameObject* Engine::Find(const _tchar* pObjKey)
{
	return Engine::CManagement::GetInstance()->Find(pObjKey);
}
inline Engine::CLayer* FindLayer(WORD eLayerID)
{
	return Engine::CManagement::GetInstance()->FindLayer(eLayerID);
}
inline const Engine::SCENEID	GetSceneID(void)
{
	return Engine::CManagement::GetInstance()->GetSceneID();
}
inline const CScene*		GetScene(void)
{
	return Engine::CManagement::GetInstance()->GetScene();
}
inline void	SetSceneID(const Engine::SCENEID eSceneID)
{
	Engine::CManagement::GetInstance()->SetSceneID(eSceneID);
}
inline void	SetManagerProjectID(const Engine::PROJECTID eProjectID)
{
	return Engine::CManagement::GetInstance()->SetManagerProjectID(eProjectID);
}
_bool Engine::Get_Delete(void)
{
	return Engine::CManagement::GetInstance()->Get_Delete();
}
PSRWLOCK	Engine::Get_Lock(PSRWLOCK * pLock)
{
	return Engine::CManagement::GetInstance()->Get_Lock(pLock);
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
void AddObject(const WORD& wLayerID, const _tchar* pObjectKey, CGameObject* pGameObject)
{
	Engine::CManagement::GetInstance()->AddObject(wLayerID, pObjectKey, pGameObject);
}

// General ----------------------------1---------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Create_Management_ForClient(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if(NULL == pManagement)
		return E_FAIL;

	if(FAILED(pManagement->Ready_Management_ForClient(pGraphicDev)))
		return E_FAIL;

	*ppManagement = pManagement;

	return S_OK;	
}

HRESULT Engine::Create_Management_ForTool(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if(NULL == pManagement)
		return E_FAIL;

	if(FAILED(pManagement->Ready_Management_ForTool(pGraphicDev)))
		return E_FAIL;

	*ppManagement = pManagement;

	return S_OK;	
}

HRESULT Engine::SetUp_CurrentScene(CScene* pCurrentScene)
{
	return CManagement::GetInstance()->SetUp_CurrentScene(pCurrentScene);
}
HRESULT	Engine::Return_SelectScene(CScene* pCurrentScene)
{
	return CManagement::GetInstance()->Return_SelectScene(pCurrentScene);
}
void Engine::SafeRelease(void)
{
	Engine::CManagement::GetInstance()->SafeRelease();
}

// For.Renderer Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
CRenderer* Engine::Get_Renderer(void)
{
	return CRenderer::GetInstance();
}
const _float GetBlurValue(void)
{
	return CRenderer::GetInstance()->GetBlurValue();
}
const _bool GetDebugBuffer(void)
{
	return CRenderer::GetInstance()->GetDebugBuffer();
}
const _bool GetShadowBuffer(void)
{
	return CRenderer::GetInstance()->GetShadowBuffer();
}
const _bool GetMotionBlurBuffer(void)
{
	return CRenderer::GetInstance()->GetMotionBlurBuffer();
}
const _bool	GetGaussianBlur(void)
{
	return CRenderer::GetInstance()->GetGaussianBlur();
}
const Engine::MRTID GetMRTID(void)
{
	return CRenderer::GetInstance()->GetMRTID();
}
const _float GetLuminanceAverage(void)
{
	return CRenderer::GetInstance()->GetLuminanceAverage();
}
const _bool	GetHDR(void)
{
	return CRenderer::GetInstance()->GetHDR();
}
const _bool GetSoftShadow(void)
{
	return CRenderer::GetInstance()->GetSoftShadow();
}
const _bool GetCACDShadow(void)
{
	return CRenderer::GetInstance()->GetCACDShadow();
}
const _uint GetCACDCount(void)
{
	return CRenderer::GetInstance()->GetCACDCount();
}
const _bool	GetDOF(void)
{
	return CRenderer::GetInstance()->GetDOF();
}
const _bool	GetSSAO(void)
{
	return CRenderer::GetInstance()->GetSSAO();
}
const _bool GetExpFog(void)
{		    
	return CRenderer::GetInstance()->GetExpFog();
}		    
const _bool GetSquareFog(void)
{		    
	return CRenderer::GetInstance()->GetSquareFog();
}		    
const _bool GetHeightFog(void)
{
	return CRenderer::GetInstance()->GetHeightFog();
}
const _bool	GetLimLight(void)
{
	return CRenderer::GetInstance()->GetLimLight();
}
const _float GetBrightness(void)
{
	return CRenderer::GetInstance()->GetBrightness();
}
const _bool GetRadialBlur(void)
{
	return CRenderer::GetInstance()->GetRadialBlur();
}
const _float GetRadialBlurPower(void)
{
	return CRenderer::GetInstance()->GetRadialBlurPower();
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
void SetDebugBuffer(const _bool bDebugBuf)
{
	CRenderer::GetInstance()->SetDebugBuffer(bDebugBuf);
}
void SetBlurValue(const _float fBlurValue)
{
	CRenderer::GetInstance()->SetBlurValue(fBlurValue);
}
void SetShadowBuffer(const _bool bShadowBuf)
{
	CRenderer::GetInstance()->SetShadowBuffer(bShadowBuf);
}
void SetMotionBlurBuffer(const _bool bMotionBlurBuffer)
{
	CRenderer::GetInstance()->SetMotionBlurBuffer(bMotionBlurBuffer);
}
void SetGaussianBlur(const _bool bGaussianBlur)
{
	CRenderer::GetInstance()->SetGaussianBlur(bGaussianBlur);
}
void SetMRTID(const Engine::MRTID eMRT)
{
	CRenderer::GetInstance()->SetMRTID(eMRT);
}
void SetLuminanceAverage(const _float fLum_Avg)
{
	CRenderer::GetInstance()->SetLuminanceAverage(fLum_Avg);
}
void SetHDR(const _bool bHDR)
{
	CRenderer::GetInstance()->SetHDR(bHDR);
}
void SetSoftShadow(const _bool bSoftShadow)
{
	CRenderer::GetInstance()->SetSoftShadow(bSoftShadow);
}
void SetCACDShadow(const _bool bCACDShadow)
{
	CRenderer::GetInstance()->SetCACDShadow(bCACDShadow);
}
void SetCACDCount(const _uint iCACDCnt)
{
	CRenderer::GetInstance()->SetCACDCount(iCACDCnt);
}
void SetDOF(const _bool bDOF)
{
	CRenderer::GetInstance()->SetDOF(bDOF);
}
void SetSSAO(const _bool bSSAO)
{
	CRenderer::GetInstance()->SetSSAO(bSSAO);
}
void SetLimLight(const _bool bLimLight)
{
	CRenderer::GetInstance()->SetLimLight(bLimLight);
}
void SetBrightness(const _float fBrightnessValue)
{
	CRenderer::GetInstance()->SetBrightness(fBrightnessValue);
}
void SetRadialBlur(const _bool bRadialBlur)
{
	CRenderer::GetInstance()->SetRadialBlur(bRadialBlur);
}
void SetRadialBlurPower(const _float vPower)
{
	CRenderer::GetInstance()->SetRadialBlurPower(vPower);
}
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
void LuminanceAverageVariation(const _float fLum_Avg)
{
	CRenderer::GetInstance()->LuminanceAverageVariation(fLum_Avg);
}
void HDR_A_Variation(const _float fA)
{
	CRenderer::GetInstance()->HDR_A_Variation(fA);
}
void HDR_B_Variation(const _float fB)
{
	CRenderer::GetInstance()->HDR_B_Variation(fB);
}
void HDR_C_Variation(const _float fC)
{
	CRenderer::GetInstance()->HDR_C_Variation(fC);
}
void HDR_D_Variation(const _float fD)
{
	CRenderer::GetInstance()->HDR_D_Variation(fD);
}
void DOF_Focus_Variation(const _float fFocus)
{	 
	CRenderer::GetInstance()->DOF_Focus_Variation(fFocus);
}	 
void DOF_Range_Variation(const _float fRange)
{
	CRenderer::GetInstance()->DOF_Range_Variation(fRange);
}
void SetExpFog(const _bool bExpFog)
{
	CRenderer::GetInstance()->SetExpFog(bExpFog);
}
void SetSquareFog(const _bool bSquareFog)
{
	CRenderer::GetInstance()->SetSquareFog(bSquareFog);
}
void SetHeightFog(const _bool bHeightFog)
{
	CRenderer::GetInstance()->SetHeightFog(bHeightFog);
}

// For.Light_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
//D3DLIGHT9* Engine::Get_LightInfo(const _uint& iIndex)
//{
//	return CLight_Manager::GetInstance()->Get_LightInfo(iIndex);
//}
//
Engine::CLight* Engine::Get_PointLight(const _uint& iIndex)
{
	return CLight_Manager::GetInstance()->Get_PointLight(iIndex);
}
D3DLIGHT9*	Engine::Find_LightInfo(const _uint sLightTag, const _short LightType)
{
	return CLight_Manager::GetInstance()->Find_LightInfo(sLightTag, LightType);
}
Engine::CLight*	Engine::Find_Light(const _uint sLightTag, const _short LightType)
{
	return CLight_Manager::GetInstance()->Find_Light(sLightTag, LightType);
}
map<const _uint, Engine::CLight*>* Engine::Get_PointLightMap(void)
{
	return CLight_Manager::GetInstance()->Get_PointLightMap();
}
map<const _uint, Engine::CLight*>* Engine::Get_SpotLightMap(void)
{
	return CLight_Manager::GetInstance()->Get_SpotLightMap();
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Ready_ScreenBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return CLight_Manager::GetInstance()->Ready_ScreenBuffer(pGraphicDev);
}
HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLight_Manager::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
void LightMap_Clear(void)
{
	CLight_Manager::GetInstance()->LightMap_Clear();
}
void Render_Directional_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex)
{
	CLight_Manager::GetInstance()->Render_Directional_Light(pGraphicDev, pEffect, iIndex);
}
void Render_Point_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex)
{
	CLight_Manager::GetInstance()->Render_Point_Light(pGraphicDev, pEffect, iIndex);
}
void Render_Spot_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex)
{
	CLight_Manager::GetInstance()->Render_Spot_Light(pGraphicDev, pEffect, iIndex);
}
void Add_Light(CLight* pLight, const _short LightType)
{
	CLight_Manager::GetInstance()->Add_Light(pLight, LightType);
}
// For.Prototype_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	return CPrototype_Manager::GetInstance()->Ready_Prototype(pProtoTag, pInstance);
}
Engine::CComponent* Engine::Clone_Prototype(const _tchar* pProtoTag)
{
	return CPrototype_Manager::GetInstance()->Clone_Prototype(pProtoTag);
}

// For.CameraMgr Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline Engine::CCamera* GetCurrentCamera(void)
{
	return CCameraMgr::GetInstance()->GetCurrentCamera();
}
inline Engine::CCamera* GetCamera(WORD eCameraID)
{
	return CCameraMgr::GetInstance()->GetCamera(eCameraID);
}
inline WORD& GetCurrentCameraID(void)
{
	return CCameraMgr::GetInstance()->GetCurrentCameraID();
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::Set_Shake(const WORD& eCameraID, const _vec3& vShakeDir, const _float& fPower, const _int& iCnt, const _int& iDelay)
{
	Engine::CCameraMgr::GetInstance()->Set_Shake(eCameraID, vShakeDir, fPower, iCnt, iDelay);
}
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
void DefineCurrentCamera(WORD eCameraID)
{
	return CCameraMgr::GetInstance()->DefineCurrentCamera(eCameraID);
}
HRESULT ReadyCamera(CCamera* pCamera, WORD eCameraID)
{
	return CCameraMgr::GetInstance()->ReadyCamera(pCamera, eCameraID);
}

// For.Target_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CTarget* Find_Target(const _tchar* pTargetTag)
{
	return CTarget_Manager::GetInstance()->Find_Target(pTargetTag);
}
list<Engine::CTarget*>* Find_MRT(const _tchar* pMRTTag)
{
	return CTarget_Manager::GetInstance()->Find_MRT(pMRTTag);
}
LPDIRECT3DTEXTURE9	GetTargetTexture(const _tchar* pTargetTag)
{
	return CTarget_Manager::GetInstance()->GetTargetTexture(pTargetTag);
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT	SetUp_OnShader(const _tchar* pTargetTag, LPD3DXEFFECT pEffect, const char* pConstantName)
{
	return CTarget_Manager::GetInstance()->SetUp_OnShader(pTargetTag, pEffect, pConstantName);
}
HRESULT	Ready_Target(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pTargetTag, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, D3DXCOLOR Color, RENDERTARGETID eRTID)
{
	return CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, pTargetTag, iSizeX, iSizeY, Format, Color, eRTID);
}
HRESULT	Ready_DebugBuffer(const _tchar* pTargetTag, const _uint& iX, const _uint& iY, const _uint& iSizeX, const _uint& iSizeY)
{
	return CTarget_Manager::GetInstance()->Ready_DebugBuffer(pTargetTag, iX, iY, iSizeX, iSizeY);
}
HRESULT	Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	return CTarget_Manager::GetInstance()->Ready_MRT(pMRTTag, pTargetTag);
}
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT	Begin_MRT(const _tchar* pMRTTag, _uint iIndex)
{
	return CTarget_Manager::GetInstance()->Begin_MRT(pMRTTag, iIndex);
}
HRESULT	Begin_NoClear_MRT(const _tchar* pMRTTag, _uint iIndex)
{
	return CTarget_Manager::GetInstance()->Begin_NoClear_MRT(pMRTTag, iIndex);
}
HRESULT	End_MRT(const _tchar* pMRTTag, _uint iIndex)
{
	return CTarget_Manager::GetInstance()->End_MRT(pMRTTag, iIndex);
}
HRESULT	Render_DebugBuffer(const _tchar* pMRTTag)
{
	return CTarget_Manager::GetInstance()->Render_DebugBuffer(pMRTTag);
}
HRESULT	MRT_Clear(const _tchar* pMRTTag)
{
	return CTarget_Manager::GetInstance()->MRT_Clear(pMRTTag);
}

// For.Collision_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
_bool Engine::IsCol_ForOBB(const _tchar* pTargetTag, const CCollider* pMyCollider, const _vec3* pNextPos)
{
	return Engine::CCollision_Manager::GetInstance()->IsCol_ForOBB(pTargetTag, pMyCollider, pNextPos);
}
_bool Engine::IsCollision(const _tchar* pTargetTag, COLINFO* pColInfo, const CCollider* pMyCollider)
{
	return Engine::CCollision_Manager::GetInstance()->IsCollision(pTargetTag, pColInfo, pMyCollider);
}
_bool Engine::IsCapture(const _tchar* pTargetTag, CCollider* pCollider)
{
	return Engine::CCollision_Manager::GetInstance()->IsCapture(pTargetTag, pCollider);
}
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::Add_ColliderGroup(COL_TYPE eType, const _tchar* pTag, CCollider* pCollider)
{
	Engine::CCollision_Manager::GetInstance()->Add_ColliderGroup(eType, pTag, pCollider);
}
void Engine::Clear_ColliderGroup(void)
{
	Engine::CCollision_Manager::GetInstance()->Clear_ColliderGroup();
}
void	Engine::Clear_All(void)
{
	Engine::CCollision_Manager::GetInstance()->Clear_All();
}
void Engine::Delete_Collider(const _tchar* pTag, const CCollider* pMyCollider)
{
	Engine::CCollision_Manager::GetInstance()->Delete_Collider(pTag, pMyCollider);
}
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
CBoxCollider* Engine::Create_BoxCollider(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _vec3& vScale, const _ulong& dwColor)
{
	return Engine::CCollision_Manager::GetInstance()->Create_BoxCollider(pGraphicDev, pParentTransform, wstrTag, vPos, vScale, dwColor);
}
CSphereCollider* Engine::Create_SphereCollider(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _float& fRad)
{
	return Engine::CCollision_Manager::GetInstance()->Create_SphereCollider(pGraphicDev, pParentTransform, wstrTag, vPos, fRad);
}

// For.Release Utility Instance---------------------------------------------------------------------------------------------------------------------------------------
void Engine::Release_Utility(void)
{
	Engine::CCollision_Manager::GetInstance()->DestroyInstance();
	Engine::CCameraMgr::GetInstance()->DestroyInstance();
	Engine::CPrototype_Manager::GetInstance()->DestroyInstance();
	Engine::CLight_Manager::GetInstance()->DestroyInstance();
	Engine::CRenderer::GetInstance()->DestroyInstance();
	Engine::CManagement::GetInstance()->DestroyInstance();
}