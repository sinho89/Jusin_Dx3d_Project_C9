#ifndef ActionStartObj_h__
#define ActionStartObj_h__

#include "GameObject.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
	class CCollider;
	class CRenderer;
}

class CActionStartObj
	: public Engine::CGameObject
{
private:
	explicit CActionStartObj(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CActionStartObj(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);
private:
	Engine::CTransform* m_pTransCom;
	Engine::CCollider*	m_pColliderCom;
	Engine::CRenderer*  m_pRendererCom;
private:
	_float				m_fStartDelay;
	_bool				m_bOnceAdd;
	_float				m_fAlpha;
private:
	HRESULT Ready_Object(const _vec3& vPos);
	HRESULT Add_Component(void);

public:
	static CActionStartObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _vec3& vPos);
private:
	virtual void Free(void);
};

#endif // IceActionStartObj_h__
