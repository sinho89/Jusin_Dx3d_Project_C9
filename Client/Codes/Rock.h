#ifndef Rock_h__
#define Rock_h__


#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CStaticMesh;
}

class CSprite;

class CRock
	: public Engine::CEffect
{
private:
	explicit CRock(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CRock(void);

public:
	virtual void		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);
	virtual	void		Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);


private:
	virtual			    void Free(void);
	HRESULT			    Ready_Object(void);
	HRESULT			    Add_Component(void);
	HRESULT			    SetUp_ConstantTable(LPD3DXEFFECT pEffect);


private:
	Engine::CStaticMesh*				m_pMeshCom;
	Engine::CTransform*					m_pTargetTransform;


public:
	static CRock*			Create(LPDIRECT3DDEVICE9 pGraphicDev
									,const wstring& wstrName);

};


#endif