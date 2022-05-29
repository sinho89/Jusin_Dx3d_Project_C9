#ifndef SpinTurnThrustDust_h__
#define SpinTurnThrustDust_h__

#include "Defines.h"
#include "GameObject.h"
#include "Effect.h"

class CSpinTurnThrustDust
	: public Engine::CEffect
{
private:
	explicit CSpinTurnThrustDust(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSpinTurnThrustDust(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);


private:
	float			m_fOriginAlpha;
	float			m_fAlphaDecrement;


private:
	virtual			  void Free(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);



public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);


	static				CSpinTurnThrustDust* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev
											, const wstring& wstrName);


	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni = TRUE);

	_bool			GetAni(void);


	void			SetAlphaDecrement(_float fAlpha);

	void			SetTargetLook(_vec3 Look);


private:
	HRESULT		    LoadReady_Object(void);


};
#endif