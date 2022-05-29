#ifndef SpinningBullet_h__
#define SpinningBullet_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CStaticMesh;
}

class CSpinningBullet
	:public Engine::CEffect
{
private:
	explicit CSpinningBullet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSpinningBullet(void);

public:
	virtual void		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);
	virtual	void		Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);


private:
	virtual			  void Free(void);
	HRESULT			  Ready_Object(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


private:
	Engine::CStaticMesh*				m_pMeshCom;
	Engine::CCollider*					m_pColliderCom;


public:
	static CSpinningBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);


public:
	void			  SetAni(_bool bAni = TRUE);
	_bool			  GetAni(void);

};


#endif
