#ifndef IceTwister_h__
#define IceTwister_h__

#include "Defines.h"
#include "Trap.h"

class CIceTwister
	: public CTrap
{
private:
	explicit CIceTwister(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CIceTwister(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT Ready_Object(void);

public:
	static CIceTwister* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
private:
	virtual void Free(void);

};
#endif // IceTwister_h__
