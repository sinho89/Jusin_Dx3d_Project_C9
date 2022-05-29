/*!
 * \file UI_FlyDance.h
 * \date 2016/11/30 21:46
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

#ifndef UI_LiddleKick_h__
#define UI_LiddleKick_h__


#include "UI_Skill.h"

class CUI_LiddleKick : public CUI_Skill
{
protected:
	explicit CUI_LiddleKick(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_LiddleKick(void);

public:
	HRESULT Ready_Object(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void SetId(SKILL_ID Id);
	void SetType(SKILL_TYPE Id);
	void SetInfo(void);

private:
	SKILL_ID		m_eSkillID;
	SKILL_TYPE		m_eSkillType;

private:
	HRESULT Add_Component(void);
public:
	static CUI_LiddleKick* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CUI_LiddleKick* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, SKILL_ID eID, SKILL_TYPE eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // UI_FlyDance_h__