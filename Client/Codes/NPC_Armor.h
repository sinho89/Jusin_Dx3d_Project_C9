#ifndef NPC_Armor_h__
#define NPC_Armor_h__

#include "Defines.h"
#include "NPC.h"

class CNPC_Armor
	: public CNPC
{
private:
	explicit CNPC_Armor(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CNPC_Armor(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);


private:
	HRESULT Ready_Object(void);

public:
	static CNPC_Armor* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);

};
#endif // NPC_Weapon_h__
