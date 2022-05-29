#ifndef HanumanEye_h__
#define HanumanEye_h__

#include "Defines.h"
#include "Effect.h"

class CHanumanEye
	: public Engine::CEffect
{
private:
	explicit CHanumanEye(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHanumanEye(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT		 Ready_Object(void);


private:
	const _matrix*	  m_pmatTarget; //하누만월드행렬
	_bool			  m_bRenderCheck;
	_bool			  m_bBlurCheck;

public:
	static CHanumanEye* Create(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);

private:
	virtual void Free(void);


	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);


	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni = TRUE);
	_bool			GetAni(void);

	static			CHanumanEye* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual			HRESULT		Start_ForScene(void);



private:
	HRESULT		    LoadReady_Object(void);


};




#endif