#ifndef NPC_Book_h__
#define NPC_Book_h__

#include "Defines.h"
#include "NPC.h"

class CNPC_Book
	: public CNPC
{
private:
	explicit CNPC_Book(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CNPC_Book(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT Ready_Object(void);

public:
	static CNPC_Book* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);

};
#endif // NPC_Weapon_h__
