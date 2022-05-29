#ifndef Target_h__
#define Target_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CScreen_Texture;
class ENGINE_DLL CTarget
	: public CBase
{
private:
	explicit				CTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual					~CTarget(void);

public:
	HRESULT					Ready_Target(const _uint& iSizeX,	//타겟 생성
										const _uint& iSizeY,
										D3DFORMAT Format,
										D3DXCOLOR Color,
										RENDERTARGETID eRTID);

	HRESULT					Ready_DebugBuffer(const _uint& iX,
											const _uint& iY,
											const _uint& iSizeX,
											const _uint& iSizeY);	//디버그를 위한 디퍼드 화면 버퍼 생성

	void					Render_DubugBuffer(void);	//디퍼드 화면 버퍼에 그림을 그린다.
	void					Clear_RenderTarget(void);	//해당 타겟을 클리어 한다.(각 타겟에 원하는 색깔로 클리어 하기 위함이다.)
	void					SetUp_OnGraphicDev(const _ulong& dwIndex);	//백버퍼를 보관하고 렌더타깃을 장치에 올린다
	void					Release_OnGraphicDev(const _ulong& dwIndex);	//보관한 백버퍼를 다시 장치에 올린다.
	HRESULT					SetUp_OnShader(LPD3DXEFFECT pEffect, const char* pConstantName);

	//Clear 함수는 장치에 올라온 모든 타겟들의 색을 초기화 한다.
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	LPDIRECT3DTEXTURE9		m_pTargetTexture;	//렌더 타겟 텍스쳐
	LPDIRECT3DSURFACE9		m_pTargetSurface;	//렌더 타겟 서피스

	LPDIRECT3DSURFACE9		m_pOldSurface;		//기존 백 버퍼.

	D3DXCOLOR				m_Color;			//렌더 타겟의 클리어 컬러.

	RENDERTARGETID			m_eRenderTargetID;	//렌더 타겟 아이디.

private:
	CScreen_Texture*		m_pScreenTexture;	//그릴 곳.

public:
	static CTarget*			Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _uint& iSizeX,
									const _uint& iSizeY,
									D3DFORMAT Format,
									D3DXCOLOR Color,
									RENDERTARGETID eRTID);

public:
	LPDIRECT3DTEXTURE9		GetTargetTexture(void) { return m_pTargetTexture; }
	RENDERTARGETID&			GetRenderTargetID(void) { return m_eRenderTargetID; }

private:
	virtual void			Free(void);
};


END

#endif