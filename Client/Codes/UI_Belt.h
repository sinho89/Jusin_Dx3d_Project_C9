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

#ifndef UI_Belt_h__
#define UI_Belt_h__

#include "UI_Item.h"

class CUI_Belt : public CUI_Item
{
protected:
	explicit CUI_Belt(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_Belt(void);

public:
	HRESULT Ready_Object(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void SetId(BELT_ID Id);
	void SetInfo(BELT_ID Id);

private:
	BELT_ID		m_eBeltID;

private:
	HRESULT Add_Component(void);
public:
	static CUI_Belt* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CUI_Belt* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, BELT_ID eID, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // UI_Helmet_h__