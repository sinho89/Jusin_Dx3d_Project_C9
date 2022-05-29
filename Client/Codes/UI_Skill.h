/*!
 * \file CUI_Skill.h
 * \date 2016/11/30 15:07
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

#ifndef UI_Skill_h__
#define UI_Skill_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CCoolTime;
class CUI_Skill : public CUI
{
protected:
	explicit CUI_Skill(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_Skill(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	SKILL_INFO*	Get_Skill_Info(void);

protected:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
	CCoolTime*							m_pCoolTimeEff;

public:
	void SetParent(CUI* pParentWindow);
protected:
	CUI*								m_pParentWindow;
	SKILL_INFO*							m_pSkillInfo;
	_float								m_fFadeTime;
public:
	virtual void Free(void);
};

#endif // CUI_Skill_h__