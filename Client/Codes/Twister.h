#ifndef Twister_h__
#define Twister_h__

#include "Defines.h"
#include "Effect.h"

class CWind;

class CTwister
	: public Engine::CEffect
{
private:
	explicit CTwister(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CTwister(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT								Ready_Object(void);

private:
	CWind*								m_pWindOne;
	CWind*								m_pWindTwo;
	const	_matrix*					WorldMat;
	_float								m_fTwisterSpeed;


public:
	static					CTwister* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SetAni(_bool bAni = TRUE);

	_bool					GetAni(void);

	void					SetSpeed(_float fSpeed);

	void					AddScaleAlpha(void);

};



#endif