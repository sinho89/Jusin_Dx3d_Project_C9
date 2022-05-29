#ifndef CherryBlossom_h__
#define CherryBlossom_h__

#include "Defines.h"
#include "Effect.h"


class CCherryBlossom
	: public Engine::CEffect
{
private:
	explicit CCherryBlossom(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CCherryBlossom(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	HRESULT Ready_Object(void);


public:
	static CCherryBlossom* Create(LPDIRECT3DDEVICE9 pGraphicDev
								  , const wstring& wstrName);

	static CCherryBlossom* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	virtual void Free(void);


private:
	_bool					m_bEqual;


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

	void			  SetEqual(_bool bEqual);

};


#endif