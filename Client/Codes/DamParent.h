#ifndef DamParent_h__
#define DamParent_h__

#include "UI.h"

namespace Engine
{
	class CGameObject;
}

class CDamParent : public CUI
{
private:
	explicit CDamParent(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CDamParent(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);

private:
	Engine::CGameObject* m_pParentObject;
private:
	list<CUI*>			m_listDam;
	typedef	list<CUI*>	LISTDAM;

public:
	void SetDamageNumber(_uint iDamageNumber, _bool bCritical);
public:
	void SetParent(Engine::CGameObject* pParent) { m_pParentObject = pParent; }
public:
	static CDamParent* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pParentObject, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // DamParent_h__
