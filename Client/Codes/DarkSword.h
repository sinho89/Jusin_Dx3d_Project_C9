#ifndef DarkSword_h__
#define DarkSword_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CStaticMesh;
}

class CDarkSword
	:public Engine::CEffect
{
private:
	explicit CDarkSword(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CDarkSword(void);

public:
	virtual void		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);
	virtual	void		Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);


private:
	virtual			  void Free(void);
	HRESULT			  Ready_Object(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	virtual			  HRESULT		Start_ForScene(void);
	void			  Setting(void);

private:
	Engine::CStaticMesh*				m_pMeshCom;
	Engine::CTransform*					m_pTargetTransform;

public:
	static CDarkSword* Create(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);


public:
	void			  SetAni(_bool bAni = TRUE);
	_bool			  GetAni(void);



};


#endif
