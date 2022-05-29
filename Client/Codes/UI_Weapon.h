/*!
 * \file UI_Weapon.h
 * \date 2016/11/27 19:32
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef UI_Weapon_h__
#define UI_Weapon_h__


#include "UI_Item.h"

class CUI_Weapon : public CUI_Item
{
protected:
	explicit CUI_Weapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_Weapon(void);

public:
	HRESULT Ready_Object(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void SetId(WEAPON_ID Id);
	void SetInfo(WEAPON_ID Id);
	void SetFade(_bool bFade) { m_bFade = bFade; }

private:
	WEAPON_ID		m_eWeaponID;

private:
	HRESULT Add_Component(void);
public:
	static CUI_Weapon* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CUI_Weapon* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, WEAPON_ID eID, _bool bFade = false, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // UI_Weapon_h__