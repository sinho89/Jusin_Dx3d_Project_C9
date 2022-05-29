/*!
 * \file Shop_Base.h
 * \date 2016/12/15 18:42
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

#ifndef Shop_Base_h__
#define Shop_Base_h__

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
class CNPC;
class CShop_Base : public CUI
{
private:
	explicit CShop_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CShop_Base(void);

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
	//CShop_ToolTip*						m_pToolTip;

private:
	SHOP_ID								m_eType;
	CNPC*								m_pNpc;
	CUI*								m_pInven;
public:
	void RenderToolTip();
	void SetType(SHOP_ID eType) { m_eType = eType; }
	void RenderColl();
	void PtInRect_Collision(const _float& fTimeDelta);

private:
	HRESULT Add_Component(void);

public:
	static CShop_Base* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CShop_Base* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		SHOP_ID eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Shop_Base_h__