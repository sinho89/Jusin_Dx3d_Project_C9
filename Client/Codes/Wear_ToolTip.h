/*!
 * \file ToolTip.h
 * \date 2016/11/28 21:03
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

#ifndef Wear_ToolTip_h__
#define Wear_ToolTip_h__


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
class CWear_ToolTip : public CUI
{
private:
	explicit CWear_ToolTip(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CWear_ToolTip(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	void SetItem(CUI_Item* pItem);
	void SetId(TOOLTIP_ID eID);

public:
	HRESULT Ready_Object(void);

private:
	TOOLTIP_ID		m_eType;
	CUI_Item*		m_pItem;
private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom[TOOLTIP_END];
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
private:
	HRESULT Add_Component(void);

public:
	static CWear_ToolTip* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // ToolTip_h__