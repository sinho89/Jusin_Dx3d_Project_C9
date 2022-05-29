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

#ifndef UI_Leg_h__
#define UI_Leg_h__

#include "UI_Item.h"

class CUI_Leg : public CUI_Item
{
protected:
	explicit CUI_Leg(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_Leg(void);

public:
	HRESULT Ready_Object(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void SetId(LEG_ID Id);
	void SetInfo(LEG_ID Id);
	void SetFade(_bool bFade) { m_bFade = bFade; }


private:
	LEG_ID		m_eLegID;

private:
	HRESULT Add_Component(void);
public:
	static CUI_Leg* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CUI_Leg* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, LEG_ID eID, _bool bFade = false, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // UI_Helmet_h__