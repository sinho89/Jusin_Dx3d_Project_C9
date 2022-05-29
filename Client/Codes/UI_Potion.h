/*!
 * \file UI_Potion.h
 * \date 2016/12/02 15:07
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

#ifndef UI_Potion_h__
#define UI_Potion_h__


#include "UI_Item.h"
class CUI;
class CUI_Potion : public CUI_Item
{
protected:
	explicit CUI_Potion(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_Potion(void);

public:
	HRESULT Ready_Object(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void SetId(POTION_ID Id);
	void SetInfo(POTION_ID Id);

private:
	POTION_ID		m_ePotionID;
	CUI*			m_pMainBase;

private:
	HRESULT Add_Component(void);
public:
	static CUI_Potion* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CUI_Potion* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, POTION_ID eID, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // UI_Potion_h__