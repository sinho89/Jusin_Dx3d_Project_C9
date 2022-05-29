/*!
 * \file Skill_ToolTip.h
 * \date 2016/12/11 10:30
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

#ifndef Skill_ToolTip_h__
#define Skill_ToolTip_h__


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
class CSkill_ToolTip : public CUI
{
private:
	explicit CSkill_ToolTip(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CSkill_ToolTip(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	void SetSkill(CUI_Skill* pSkill);
	void SetId(SKILL_ID eID);

public:
	HRESULT Ready_Object(void);

private:
	SKILL_ID		m_eType;
	CUI_Skill*		m_pSkill;
private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom[SKILL_END];
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
private:
	HRESULT Add_Component(void);

public:
	static CSkill_ToolTip* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Skill_ToolTip_h__