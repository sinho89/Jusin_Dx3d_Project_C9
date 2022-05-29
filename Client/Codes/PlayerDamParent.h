#ifndef PlayerDamParent_h__
#define PlayerDamParent_h__


#include "UI.h"

namespace Engine
{
	class CGameObject;
}

class CPlayerDamParent : public CUI
{
private:
	explicit CPlayerDamParent(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CPlayerDamParent(void);

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
	typedef list<CUI*>	LISTPLAYERDAM;

public:
	void SetDamageNumber(_uint iDamageNumber, _bool bCritical);
public:
	void SetParent(Engine::CGameObject* pParent) { m_pParentObject = pParent; }
public:
	static CPlayerDamParent* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pParentObject, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // PlayerDamParent_h__
