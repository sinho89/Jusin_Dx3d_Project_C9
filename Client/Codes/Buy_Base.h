/*!
 * \file Buy_Base.h
 * \date 2016/12/21 15:02
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

#ifndef Buy_Base_h__
#define Buy_Base_h__


#include "UI.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CUI;
class CUI_Item;
class CBuy_Base : public CUI
{
private:
	explicit CBuy_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CBuy_Base(void);

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
	CUI*								m_pSheet;
	CUI_Item*							m_pItem;

public:
	void SetItem(CUI_Item* pItem) { m_pItem = pItem; }
	void SetSheet(CUI* pSheet) { m_pSheet = pSheet; }
public:
	CUI_Item* GetItem(void) { return m_pItem; }
	CUI*	GetSheet(void) { return m_pSheet; }
private:
	HRESULT Add_Component(void);

public:
	static CBuy_Base* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};
#endif // Buy_Base_h__