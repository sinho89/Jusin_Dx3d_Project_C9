/*!
 * \file UI_Item.h
 * \date 2016/11/27 19:02
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

#ifndef UI_Item_h__
#define UI_Item_h__

#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CUI_Item : public CUI
{
protected:
	explicit CUI_Item(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_Item(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	ITEM_INFO*	Get_Item_Info(void);
	_bool		GetEqui(void);
	void		SetEqui(_bool bEqui);
	_bool		GetResult(void) { return m_bResult; }
	void		SetResult(_bool bResult) { m_bResult = bResult; }
	_bool		GetImmidy(void) { return m_bImmidy; }
	void		SetImmidy(_bool bImmidy) { m_bImmidy = bImmidy; }
protected:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

public:
	void SetParent(CUI* pParentWindow);
protected:
	CUI*								m_pParentWindow;
	ITEM_INFO*							m_pItemInfo;
	_bool								m_bEqu;
	_bool								m_bResult;
	_bool								m_bImmidy;
public:
	virtual void Free(void);
};

#endif // UI_Item_h__