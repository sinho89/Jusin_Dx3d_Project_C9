/*!
 * \file Cursor.h
 * \date 2016/11/22 10:20
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

#ifndef Cursor_h__
#define Cursor_h__



#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
	class CGameObject;
}
class CUI_Item;
class CUI_Skill;
class CCursor : public CUI
{
private:
	explicit CCursor(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CCursor(void);

public:
	void SetItem(CUI_Item* pItem);
	void SetSkill(CUI_Skill* pSkill);
	void SetDropDelay(_bool bDropDelay);
public:
	CUI_Item* GetItem(void);
	CUI_Skill* GetSkill(void);
	_bool		GetDropDelay(void);
	Engine::CGameObject* GetEffect(void) { return m_pMouseEff; }
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
	Engine::CGameObject*				m_pMouseEff;

private:
	CUI_Item*				m_pItem;
	CUI_Skill*				m_pSkill;
	_bool					m_bDropDelay;
	_bool					m_bEffect;
public:
	void MoveFollowMousePos();
	void SetItemPos(void);
	void SetSkillPos(void);
private:
	HRESULT Add_Component(void);

public:
	static CCursor* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CCursor* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		_bool bRender, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Cursor_h__