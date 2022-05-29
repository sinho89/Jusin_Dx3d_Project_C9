#ifndef FlyDanceTrail_h__
#define FlyDanceTrail_h__

#include "Defines.h"
#include "Effect.h"

class CFlyDanceTrail
	: public Engine::CEffect
{
private:
	explicit CFlyDanceTrail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CFlyDanceTrail(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);


private:
	HRESULT		 Ready_Object(void);

private:
	_bool								m_bBilBoardCheck;		//ºôº¸µå Ã¼Å©
	D3DXVECTOR4							m_vTrailColor;
	Engine::CTransform*					pTransform;
	_float								m_fGage;
	Engine::CTexture*		m_pNormalTextureCom;

public:
	static CFlyDanceTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);



private:
	virtual			  void Free(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	virtual			  HRESULT		Start_ForScene(void);

public:
	void			  SetPart(_int nIdx);
	void			  Update_RePart(void);
	void			  SetAni(_bool bAni = TRUE);
	_bool			  GetAni(void);
	void			  SetColor(_vec4 vColor);
	_vec4			  GetColor(void);

	void			  SetGage(_float fGage);

	_float			  GetGage(void);

	void			  SetMinusGage(_float fGage);
};

#endif
