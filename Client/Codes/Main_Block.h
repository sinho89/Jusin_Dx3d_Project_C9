/*!
 * \file Main_Block.h
 * \date 2016/12/01 13:35
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

#ifndef Main_Block_h__
#define Main_Block_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CUI_Skill;
class CUI_Item;
class CMain_Block : public CUI
{
protected:
	explicit CMain_Block(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CMain_Block(void);

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
	CUI_Skill* GetSkill(void);
	CUI_Item*  GetItem(void);
	void SetSkill(CUI_Skill* pSkill);
	void SetItem(CUI_Item* pItem);
	void SetSkillPos(void);
	void SetItemPos(void);

private:
	CUI*					m_pParentWindow;
	_vec2					m_vParentBasePos;
	CUI_Skill*				m_pSkill;
	CUI_Item*				m_pItem;

private:
	HRESULT Add_Component(void);

public:
	static CMain_Block* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CMain_Block* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, _vec2 vPos,const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};



#endif // Main_Block_h__