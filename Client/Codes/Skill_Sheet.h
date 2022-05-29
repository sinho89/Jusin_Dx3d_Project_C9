/*!
 * \file Skill_Sheet.h
 * \date 2016/11/30 10:48
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

#ifndef Skill_Sheet_h__
#define Skill_Sheet_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CSkill_ToolTip;
class CUI_Skill;
class CSkill_Avi;
class CSkill_Sheet : public CUI
{
private:
	explicit CSkill_Sheet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CSkill_Sheet(void);

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
	_vec2					m_vParentBasePos;
	vector<CUI*>			m_vecBlock;
	CUI_Skill*				m_pCapySkill;
	_uint					m_maxSlotLevel;
	CSkill_ToolTip*			m_pToolTip;
	CSkill_Avi*				m_pAvi;

public:
	void PtInRect_Collision(const _float& fTimeDelta);
	void SetParent(CUI* pParentWindow);
	void SkillPick(void);
	void RenderToolTip(void);
	void AddSheet(_uint iIndex, CUI* pSkill);
	void AddSkill(_uint iSheet, _uint iIndex, CUI_Skill* pSkill);
	void DeleteSkill();
	vector<CUI*>* GetVecPage();

public:
	void SetType(_bool bType) { m_bType = bType; }
	_bool GetType() { return m_bType; }

private:
	_bool	m_bType;

private:
	HRESULT Add_Component(void);

public:
	static CSkill_Sheet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CSkill_Sheet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, _uint iIndex, _bool bType = false, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Skill_Sheet_h__