/*!
 * \file Shop_Sheet.h
 * \date 2016/12/20 14:37
 *
 * \author ShinHo
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Shop_Sheet_h__
#define Shop_Sheet_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CUI_Item;
class CShop_ToolTip;
class CShop_Sheet : public CUI
{
private:
	explicit CShop_Sheet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CShop_Sheet(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);

private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pNCTextureCom;
	Engine::CTexture*					m_pCTextureCom;
	Engine::CTexture*					m_pSTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

private:
	CUI*					m_pParentWindow;
	SHOP_ID					m_eType;
	CShop_ToolTip*				m_pToolTip;
	_vec2					m_vParentBasePos;
	vector<CUI*>			m_vecBlock;
	vector<CUI*>			m_vecSellItem;
	_bool					m_bProgressColl;

public:
	void PtInRect_Collision(const _float& fTimeDelta);
	void SetParent(CUI* pParentWindow);
	vector<CUI*>* GetVecBlock(void);
	vector<CUI*>* GetVecSell(void) { return &m_vecSellItem; }
	SHOP_ID	GetShopID(void) { return m_eType; }
	void AddVecBlock(_uint iIndex, CUI* pUI);
	void ItemPick(void);
	void BuyPotion(void);
	void RenderToolTip(void);
	void FadeCheck(void);
	_uint EmptySpaceCheck(SLOT_ID Id);

public:
	void AddShop(CUI_Item* pItem, _uint iIndex);
	void AddSell(CUI_Item* pItem, _uint iIndex);
	void SetType(SHOP_ID eType) { m_eType = eType; }


private:
	HRESULT Add_Component(void);

public:
	static CShop_Sheet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CShop_Sheet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		CUI* pParentWindow, SHOP_ID eID, _uint iIndex, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};



#endif // Shop_Sheet_h__