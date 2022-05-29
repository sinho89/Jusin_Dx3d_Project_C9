#ifndef Target_Manager_h__
#define Target_Manager_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CShader;
class CTarget;
class ENGINE_DLL CTarget_Manager
	: public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)

private:
	explicit			CTarget_Manager(void);
	virtual				~CTarget_Manager(void);

public:
	HRESULT				Ready_Target(LPDIRECT3DDEVICE9 pGraphicDev,
									const _tchar* pTargetTag,
									const _uint& iSizeX,
									const _uint& iSizeY,
									D3DFORMAT Format,
									D3DXCOLOR Color,
									RENDERTARGETID eRTID);

	// 렌더타겟들을	화면에 출력해보기위한 버퍼
	HRESULT				Ready_DebugBuffer(const _tchar* pTargetTag,
											const _uint& iX,
											const _uint& iY,
											const _uint& iSizeX,
											const _uint& iSizeY);

	HRESULT				Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
	HRESULT				Begin_MRT(const _tchar* pMRTTag, _uint iIndex = 0);
	HRESULT				Begin_NoClear_MRT(const _tchar* pMRTTag, _uint iIndex = 0);
	HRESULT				End_MRT(const _tchar* pMRTTag, _uint iIndex = 0);
	HRESULT				MRT_Clear(const _tchar* pMRTTag);
	HRESULT				Render_DebugBuffer(const _tchar* pMRTTag);

	HRESULT				SetUp_OnShader(const _tchar* pTargetTag, LPD3DXEFFECT pEffect, const char* pConstantName);

private:
	map<const _tchar*, CTarget*>				m_mapTarget;
	typedef map<const _tchar*, CTarget*>		MAPTARGET;

private:
	map<const _tchar*, list<CTarget*>>			m_mapMRT;
	typedef map<const _tchar*, list<CTarget*>>	MAPMRT;

public:
	CTarget*			Find_Target(const _tchar* pTargetTag);
	list<CTarget*>*		Find_MRT(const _tchar* pMRTTag);
	LPDIRECT3DTEXTURE9	GetTargetTexture(const _tchar* pTargetTag);

private:
	virtual void		Free(void);
};

END

#endif // Taget_Manager_h__