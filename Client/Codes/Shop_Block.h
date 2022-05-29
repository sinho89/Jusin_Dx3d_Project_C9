/*!
 * \file Shop_Block.h
 * \date 2016/12/20 21:22
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

#ifndef Shop_Block_h__
#define Shop_Block_h__


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
class CShop_Block : public CUI
{
protected:
	explicit CShop_Block(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CShop_Block(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);

private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

public:
	void SetParent(CUI* pParentWindow);
	void SetPos(_vec2 vPos);
	void PtInRect_Collision(const _float& fTimeDelta);
	CUI_Item* GetItem(void);
	void SetItem(CUI_Item* pItem);
	void SetItemPos(void);
private:
	CUI*					m_pParentWindow;
	_vec2					m_vParentBasePos;
	CUI_Item*				m_pItem;

private:
	HRESULT Add_Component(void);

public:
	static CShop_Block* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CShop_Block* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		CUI* pParentWindow, _vec2 vPos, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Shop_Block_h__