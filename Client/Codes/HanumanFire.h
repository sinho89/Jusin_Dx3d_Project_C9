#ifndef HanumanFire_h__
#define HanumanFire_h__

#include "Defines.h"
#include "Effect.h"

class  CHanumanFire
	: public Engine::CEffect

{
private:
	explicit CHanumanFire(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHanumanFire(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT		 Ready_Object(void);

private:
	_bool			  m_bRenderCheck;
	_bool			  m_bBlurCheck;
	_int			  m_iPattern;

	const _matrix*	  m_pmatTarget; //하누만월드행렬

private:
	const _matrix*	  m_pRightHandFire;
	const _matrix*	  m_pLeftHandFire;
	const _matrix*    m_pRightShoulder;
	const _matrix*	  m_pLeftShoulder;



public:
	static CHanumanFire* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);

private:
	virtual void Free(void);


	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);
	void			  SetRenderCheck(_bool bRenderCheck);
	void			  SetBlurCheck(_bool bCheck) { m_bBlurCheck = bCheck; }
	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni = TRUE);
	_bool			GetAni(void);

	static			CHanumanFire* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual			HRESULT		Start_ForScene(void);

	void			SetPattern(_int iPattern);

private:
	HRESULT		    LoadReady_Object(void);

};


#endif