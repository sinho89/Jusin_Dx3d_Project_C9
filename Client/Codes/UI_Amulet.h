/*!
 * \file UI_Helmet.h
 * \date 2016/11/28 10:07
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

#ifndef UI_Amulet_h__
#define UI_Amulet_h__

#include "UI_Item.h"

class CUI_Amulet : public CUI_Item
{
protected:
	explicit CUI_Amulet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_Amulet(void);

public:
	HRESULT Ready_Object(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void SetId(AMULET_ID Id);
	void SetInfo(AMULET_ID Id);

private:
	AMULET_ID		m_eAmuletID;

private:
	HRESULT Add_Component(void);
public:
	static CUI_Amulet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CUI_Amulet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, AMULET_ID eID, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // UI_Helmet_h__