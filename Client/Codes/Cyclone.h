#ifndef Cyclone_h__
#define Cyclone_h__

#include "Defines.h"
#include "Effect.h"



class CCyclone
	: public Engine::CEffect
{
private:
	explicit CCyclone(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CCyclone(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	HRESULT Ready_Object(void);


public:
	static CCyclone* Create(LPDIRECT3DDEVICE9 pGraphicDev
					 , const wstring& wstrName);

	static CCyclone* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	virtual void Free(void);

private:
	_float			m_fCycloneRadius;
	_float			m_fCycloneAngle;


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

	void			  ResetInfo(void);




};

#endif