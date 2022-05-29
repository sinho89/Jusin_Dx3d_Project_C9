#ifndef NPC_Weapon_h__
#define NPC_Weapon_h__

#include "Defines.h"
#include "NPC.h"

class CNPC_Weapon
	: public CNPC
{
private:
	explicit CNPC_Weapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CNPC_Weapon(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);
	
private:
	HRESULT Ready_Object(void);

public:
	static CNPC_Weapon* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
	
};
#endif // NPC_Weapon_h__
