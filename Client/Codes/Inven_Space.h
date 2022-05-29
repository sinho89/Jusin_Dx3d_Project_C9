/*!
 * \file Inven_Space.h
 * \date 2016/11/25 21:49
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

#ifndef Inven_Space_h__
#define Inven_Space_h__

#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CToolTip;
class CUI_Item;
class CUI;
class CWeapon;
class CInven_Space : public CUI
{
protected:
	explicit CInven_Space(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CInven_Space(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);
	CUI_Item* GetItem(void);
	void SetItem(CUI_Item* pItem);
	void SetItemPos(void);
	void PtInRect_Collision(const _float& fTimeDelta);
	void WearItem();
	void OutItem();
	void SwapItem();
	ITEM_ID GetID() { return m_eType; }

private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
protected:
	CUI*					m_pParentWindow;
	_vec2					m_vParentBasePos;
	CUI_Item*				m_pItem;
	ITEM_ID					m_eType;
	_bool					m_bFindTwinsRed;
	_bool					m_bFindTwinsBlue;
	CUI*					m_pSheet;

public:
	void SetParent(CUI* pParentWindow);

private:
	HRESULT Add_Component(void);

public:
	virtual void Free(void);
};
#endif // Inven_Space_h__