#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class CGameObject;
class CScreen_Texture;
class CTexture;
class ENGINE_DLL CRenderer 
	: public CComponent
{
public:
	DECLARE_SINGLETON(CRenderer);

private:
	explicit								CRenderer(void);
	virtual									~CRenderer(void);

public:
	void									Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject);
	void									Add_WindowUI(WORD wSortNum, CGameObject* pGameObject);
	void									Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Clear_RenderGroup(void);

private:
	list<CGameObject*>						m_RenderGroup[RENDER_END];
	typedef list<CGameObject*>				RENDERGROUP;

	multimap<WORD, CGameObject*>			m_WindowUI;
	typedef multimap<WORD, CGameObject*>	WINDOWUIMAP;

private:
	CScreen_Texture*						m_pScreenTexture;
	CScreen_Texture*						m_pScreent_Quad_Texture;

	CTexture*								m_pTexRandNormal;

	MRTID									m_eMRTID;
	PROJECTID								m_eProjectID;

	_float									m_fBlurValue;
	_float									m_fBrightnessValue;
	
	_bool									m_bDebugBuffer;		//디버그 버퍼를 On / Off
	
	_bool									m_bShadowBuffer;	//그림자 On / Off
	_bool									m_bSoftShadow;		//소프트 그림자 On / Off
	_bool									m_bCACDShadow;		//캐스케이드 그림자 On / Off
	_uint									m_iCACD_Cnt;

	_bool									m_bMotionBlurBuffer;//모션블러 On / Off

	_bool									m_bGaussianBlur;	//블러 On / Off
	_bool									m_bGaussianClear;

	_bool									m_bHDR;				//HDR On / Off
	_float									m_fLum_Average;		//휘도 평균값.

	_bool									m_bDOF;				//DOF On / Off
	_float									m_fDOF_Focus;
	_float									m_fDOF_Range;

	_bool									m_bSSAO;			//SSAO On / Off
	_bool									m_bSSAO_Clear;
	_bool									m_bRandNormal;

	_bool									m_bExpFog;
	_bool									m_bSquareFog;
	_bool									m_bHeightFog;

	_bool									m_bLimLight;		//LimLight On / Off

	_bool									m_bRadialBlur;		//RadialBlur On / Off
	_float									m_fRadialBlur_Width;//RadialBlur Power;
	
	//HDR Flimit 
	_float									m_fA;
	_float									m_fB;
	_float									m_fC;
	_float									m_fD;
	
private:
	void									Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_ShadowMap(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Dynamic(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Static(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Decal(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_NoneAlpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Alpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Cursor(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Window(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void									Render_Deferred(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_MotionBlur(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_LightAcc(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Outline(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Shadow(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_CACD_ShadowMap(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_SSAO(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Blend(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_DOF(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_Final(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_RadialBlur(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_PostEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	void									Render_MiniMap(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	bool									Render_Blur_Work(LPD3DXEFFECT pEffect, 
															_int iCnt, 
															const _tchar* pTargetTag, 
															_float fBlurPower, 
															_float fBlurValue,
															BLURTYPE eBlurType = BLUR_HORIZON);

	bool									Render_Blur_Depth_Work(LPD3DXEFFECT pEffect,
																	_int iCnt,
																	const _tchar* pTargetTag,
																	_float fBlurPower,
																	_float fBlurValue,
																	BLURTYPE eBlurType = BLUR_HORIZON);
public:
	const MRTID								GetMRTID(void) const { return m_eMRTID; }
	void									SetMRTID(const MRTID eMRT) { m_eMRTID = eMRT; }

	const _float							GetBlurValue(void) const { return m_fBlurValue; }
	void									SetBlurValue(const _float fBlurValue) { m_fBlurValue = fBlurValue; }

	const _float							GetBrightness(void) const { return m_fBrightnessValue; }
	void									SetBrightness(const _float fBrightnessValue) { m_fBrightnessValue = fBrightnessValue; }

	const _bool								GetDebugBuffer(void) const { return m_bDebugBuffer; }
	void									SetDebugBuffer(const _bool bDebugBuf) { m_bDebugBuffer = bDebugBuf; }

	const _bool								GetShadowBuffer(void) const { return m_bShadowBuffer; }
	void									SetShadowBuffer(const _bool bShadowBuf) { m_bShadowBuffer = bShadowBuf; }

	const _bool								GetSoftShadow(void)	const { return m_bSoftShadow; }
	void									SetSoftShadow(const _bool bSoftShadow) { m_bSoftShadow = bSoftShadow; }

	const _bool								GetCACDShadow(void)	const { return m_bCACDShadow; }
	void									SetCACDShadow(const _bool bCACDShadow) { m_bCACDShadow = bCACDShadow; }

	const _uint								GetCACDCount(void) const { return m_iCACD_Cnt; }
	void									SetCACDCount(const _uint iCACDCnt) { m_iCACD_Cnt = iCACDCnt; }

	const _bool								GetMotionBlurBuffer(void) const { return m_bMotionBlurBuffer; }
	void									SetMotionBlurBuffer(const _bool bMotionBlurBuffer) { m_bMotionBlurBuffer = bMotionBlurBuffer; }

	const _bool								GetGaussianBlur(void) const { return m_bGaussianBlur; }
	void									SetGaussianBlur(const _bool bGaussianBlur) { m_bGaussianBlur = bGaussianBlur; }

	const _bool								GetHDR(void) const { return m_bHDR; }
	void									SetHDR(const _bool bHDR) { m_bHDR = bHDR; }

	const _bool								GetLimLight(void) const { return m_bLimLight; }
	void									SetLimLight(const _bool bLimLight) { m_bLimLight = bLimLight; }

	const _bool								GetDOF(void) const { return m_bDOF; }
	void									SetDOF(const _bool bDOF) { m_bDOF = bDOF; }
	void									DOF_Focus_Variation(const _float fFocus) { m_fDOF_Focus += fFocus; }
	void									DOF_Range_Variation(const _float fRange) { m_fDOF_Range += fRange; }

	const _bool								GetSSAO(void) const { return m_bSSAO; }
	void									SetSSAO(const _bool bSSAO) { m_bSSAO = bSSAO; }

	const _bool								GetExpFog(void) const { return m_bExpFog; }
	void									SetExpFog(const _bool bExpFog) { m_bExpFog = bExpFog; }

	const _bool								GetSquareFog(void) const { return m_bSquareFog; }
	void									SetSquareFog(const _bool bSquareFog) { m_bSquareFog = bSquareFog; }

	const _bool								GetHeightFog(void) const { return m_bHeightFog; }
	void									SetHeightFog(const _bool bHeightFog) { m_bHeightFog = bHeightFog; }

	const _bool								GetRadialBlur(void) const { return m_bRadialBlur; }
	void									SetRadialBlur(const _bool bRadialBlur) { m_bRadialBlur = bRadialBlur; }

	const _float							GetRadialBlurPower(void) const { return m_fRadialBlur_Width; }
	void									SetRadialBlurPower(const _float vPower) { m_fRadialBlur_Width = vPower; }

	const _float							GetLuminanceAverage(void) const { return m_fLum_Average; }
	void									SetLuminanceAverage(const _float fLum_Avg) { m_fLum_Average = fLum_Avg; }
	void									LuminanceAverageVariation(const _float fLum_Avg) { m_fLum_Average += fLum_Avg; }

	void									HDR_A_Variation(const _float fA) { m_fA += fA; }
	void									HDR_B_Variation(const _float fB) { m_fB += fB; }
	void									HDR_C_Variation(const _float fC) { m_fC += fC; }
	void									HDR_D_Variation(const _float fD) { m_fD += fD; }

	const PROJECTID							GetProjectID(void) const { return m_eProjectID; }
	void									SetProjectID(const PROJECTID eProjectID) { m_eProjectID = eProjectID; }

public:
	HRESULT									CreateScreenTexture(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void							Free(void);
};

END

#endif // Renderer_h__
