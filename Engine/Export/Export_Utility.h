#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Defines.h"
#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "UICalculater.h"
#include "Calculator.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Shader.h"
#include "Animator.h"
#include "Rigidbody.h"
#include "Light_Manager.h"
#include "Prototype_Manager.h"
#include "CameraMgr.h"
#include "Target_Manager.h"
#include "Collision_Manager.h"

BEGIN(Engine)

// For.Management Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline CGameObject*			FindWithTag(const _tchar* pObjTag);
inline list<CGameObject*>*	FindList(const _tchar* pObjTag);
inline CGameObject*			Find(const _tchar* pObjKey);
inline CLayer*				FindLayer(WORD eLayerID);
inline const SCENEID		GetSceneID(void);
inline const CScene*		GetScene(void);
inline void					SetSceneID(const SCENEID eSceneID);
inline void					SetManagerProjectID(const PROJECTID eProjectID);
inline _bool				Get_Delete(void);
inline PSRWLOCK				Get_Lock(PSRWLOCK * pLock);
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline void					AddObject(const WORD& wLayerID, const _tchar* pObjectKey, CGameObject* pGameObject);
// General ----------------------------1---------------------------------------------------------------------------------------------------------------------------
inline HRESULT 				Create_Management_ForClient(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);
inline HRESULT 				Create_Management_ForTool(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);
inline HRESULT 				SetUp_CurrentScene(CScene* pCurrentScene);
inline HRESULT				Return_SelectScene(CScene* pCurrentScene);
inline void					SafeRelease(void);

// For.Renderer Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline CRenderer*			Get_Renderer(void);
inline const _float			GetBlurValue(void);
inline const _bool			GetDebugBuffer(void);
inline const _bool			GetShadowBuffer(void);
inline const _bool			GetMotionBlurBuffer(void);
inline const _bool			GetGaussianBlur(void);
inline const MRTID			GetMRTID(void);
inline const _float			GetLuminanceAverage(void);
inline const _bool			GetHDR(void);
inline const _bool			GetSoftShadow(void);
inline const _bool			GetCACDShadow(void);
inline const _uint			GetCACDCount(void);
inline const _bool			GetDOF(void);
inline const _bool			GetSSAO(void);
inline const _bool			GetExpFog(void);
inline const _bool			GetSquareFog(void);
inline const _bool			GetHeightFog(void);
inline const _bool			GetLimLight(void);
inline const _float			GetBrightness(void);
inline const _bool			GetRadialBlur(void);
inline const _float			GetRadialBlurPower(void);
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline void					SetBlurValue(const _float fBlurValue);
inline void					SetDebugBuffer(const _bool bDebugBuf);
inline void					SetShadowBuffer(const _bool bShadowBuf);
inline void					SetMotionBlurBuffer(const _bool bMotionBlurBuffer);
inline void					SetGaussianBlur(const _bool bGaussianBlur);
inline void					SetMRTID(const MRTID eMRT);
inline void					SetLuminanceAverage(const _float fLum_Avg);
inline void					SetHDR(const _bool bHDR);
inline void					SetSoftShadow(const _bool bSoftShadow);
inline void					SetCACDShadow(const _bool bCACDShadow);
inline void					SetCACDCount(const _uint iCACDCnt);
inline void					SetDOF(const _bool bDOF);
inline void					SetSSAO(const _bool bSSAO);
inline void					SetExpFog(const _bool bExpFog);
inline void					SetSquareFog(const _bool bSquareFog);
inline void					SetHeightFog(const _bool bHeightFog);
inline void					SetLimLight(const _bool bLimLight);
inline void					SetBrightness(const _float fBrightnessValue);
inline void					SetRadialBlur(const _bool bRadialBlur);
inline void					SetRadialBlurPower(const _float vPower);
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
inline void					LuminanceAverageVariation(const _float fLum_Avg);
inline void					HDR_A_Variation(const _float fA);
inline void					HDR_B_Variation(const _float fB);
inline void					HDR_C_Variation(const _float fC);
inline void					HDR_D_Variation(const _float fD);
inline void					DOF_Focus_Variation(const _float fFocus);
inline void					DOF_Range_Variation(const _float fRange);

// For.Light_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
//inline D3DLIGHT9*			Get_LightInfo(const _uint& iIndex = 0);
inline 	CLight*				Get_PointLight(const _uint& iIndex = 0);
inline D3DLIGHT9*			Find_LightInfo(const _uint sLightTag, const _short LightType);
inline 	CLight*				Find_Light(const _uint sLightTag, const _short LightType);
inline map<const _uint, CLight*>*	Get_PointLightMap(void);
inline map<const _uint, CLight*>*	Get_SpotLightMap(void);
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline HRESULT				Ready_ScreenBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
inline HRESULT				Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
inline void					LightMap_Clear(void);
inline void					Render_Directional_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex);
inline void					Render_Point_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex);
inline void					Render_Spot_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex);
inline void					Add_Light(CLight* pLight, const _short LightType);

// For.Prototype_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
inline HRESULT				Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*			Clone_Prototype(const _tchar* pProtoTag);

// For.CameraMgr Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline CCamera*				GetCurrentCamera(void);
inline CCamera*				GetCamera(WORD eCameraID);
inline WORD&				GetCurrentCameraID(void);
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline void					Set_Shake(const WORD& eCameraID, const _vec3& vShakeDir, const _float& fPower = 0.2f, const _int& iCnt = 5, const _int& iDelay = 1);
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
inline void					DefineCurrentCamera(WORD eCameraID);
inline HRESULT				ReadyCamera(CCamera* pCamera, WORD eCameraID);

// For.Target_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline CTarget*				Find_Target(const _tchar* pTargetTag);
inline list<CTarget*>*		Find_MRT(const _tchar* pMRTTag);
inline LPDIRECT3DTEXTURE9	GetTargetTexture(const _tchar* pTargetTag);
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline HRESULT				SetUp_OnShader(const _tchar* pTargetTag, LPD3DXEFFECT pEffect, const char* pConstantName);
inline HRESULT				Ready_Target(LPDIRECT3DDEVICE9 pGraphicDev,	const _tchar* pTargetTag, const _uint& iSizeX, const _uint& iSizeY,	D3DFORMAT Format, D3DXCOLOR Color, RENDERTARGETID eRTID);
inline HRESULT				Ready_DebugBuffer(const _tchar* pTargetTag, const _uint& iX, const _uint& iY, const _uint& iSizeX, const _uint& iSizeY);
inline HRESULT				Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
inline HRESULT				Begin_MRT(const _tchar* pMRTTag, _uint iIndex = 0);
inline HRESULT				Begin_NoClear_MRT(const _tchar* pMRTTag, _uint iIndex = 0);
inline HRESULT				End_MRT(const _tchar* pMRTTag, _uint iIndex = 0);
inline HRESULT				Render_DebugBuffer(const _tchar* pMRTTag);
inline HRESULT				MRT_Clear(const _tchar* pMRTTag);


// For.Collision_Manager Instance ---------------------------------------------------------------------------------------------------------------------------------------
// Getter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline _bool IsCol_ForOBB(const _tchar* pTargetTag, const CCollider* pMyCollider, const _vec3* pNextPos);
inline _bool IsCollision(const _tchar* pTargetTag, COLINFO* pColInfo, const CCollider* pMyCollider);
inline _bool IsCapture(const _tchar* pTargetTag, CCollider* pCollider);
// Setter --------------------------------------------------------------------------------------------------------------------------------------------------------
inline void Add_ColliderGroup(COL_TYPE eType, const _tchar* pTag, CCollider* pCollider);
inline void Clear_ColliderGroup(void);
inline void	Clear_All(void);
inline void Delete_Collider(const _tchar* pTag, const CCollider* pMyCollider);
// General -------------------------------------------------------------------------------------------------------------------------------------------------------
inline CBoxCollider* Create_BoxCollider(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _vec3& vScale, const _ulong& dwColor);
inline CSphereCollider* Create_SphereCollider(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _float& fRad);

// For.Release Utility Instance---------------------------------------------------------------------------------------------------------------------------------------
inline void					Release_Utility(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__
