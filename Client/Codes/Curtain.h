#ifndef Curtain_h__
#define Curtain_h__



#include "Defines.h"
#include "Effect.h"

class CCurtain
	: public Engine::CEffect
{
private:
	explicit CCurtain(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CCurtain(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT Ready_Object(void);


public:
	static CCurtain* Create(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);

	static CCurtain* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);


private:
	virtual void Free(void);

private:
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

	HRESULT		      LoadReady_Object(void);


public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(void);
	void			  Update_RePart(void);

	void			  SetAni(_bool bAni = TRUE);//애니메이션 활성화 / 비활성화

	_bool			  GetAni(void);


private:
	_bool				m_bYBillBoard;


public:
	void				SetYBillBoard(_bool bYBillBoard);

};






#endif