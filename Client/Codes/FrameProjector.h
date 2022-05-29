#ifndef FrameProjector_h__
#define FrameProjector_h__

#include "Defines.h"
#include "Effect.h"

class CFrameProjector
	: public Engine::CEffect
{
private:
	explicit CFrameProjector(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CFrameProjector(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT		 Ready_Object(void);


public:
	static CFrameProjector* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);

private:
	virtual void Free(void);

	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);

public:
	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni = TRUE);
	_bool			GetAni(void);

	static			CFrameProjector* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	HRESULT		    LoadReady_Object(void);


private:
	_bool			  m_bBlurCheck;
	_double			  m_dTime;
	_vec3			  m_vTargetDir;
	_vec3			  m_vTargetPos;

public:
	void			  SetTargetDir(_vec3	m_pDir);
	void			  SetTargetPos(_vec3    m_vPos);

};


#endif