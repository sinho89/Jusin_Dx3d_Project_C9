#ifndef DarkPollingWave_h__
#define DarkPollingWave_h__

#include "Defines.h"
#include "Effect.h"

class CDarkPollingWave
	: public Engine::CEffect
{
private:
	explicit CDarkPollingWave(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CDarkPollingWave(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT		 Ready_Object(void);

public:
	static CDarkPollingWave* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);


private:
	virtual			  void Free(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);
	void			  SetAni(_bool bAni = TRUE);
	_bool			  GetAni(void);

};



#endif