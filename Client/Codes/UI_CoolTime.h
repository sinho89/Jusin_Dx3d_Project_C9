#ifndef UI_CoolTime_h__
#define UI_CoolTime_h__


#include "UI_Skill.h"

class CUI_CoolTime : public CUI_Skill
{
protected:
	explicit CUI_CoolTime(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_CoolTime(void);

public:
	HRESULT Ready_Object(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void SetId(SKILL_ID Id);

private:
	SKILL_ID		m_eSkillID;
	CUI*			m_pEffect;

private:
	HRESULT Add_Component(void);
public:
	static CUI_CoolTime* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		SKILL_ID Id, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // UI_CoolTime_h__
