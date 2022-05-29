/*!
 * \file Skill_Block.h
 * \date 2016/11/30 15:04
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

#ifndef Skill_Block_h__
#define Skill_Block_h__


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
class CSkill_Block : public CUI
{
protected:
	explicit CSkill_Block(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CSkill_Block(void);

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
	void SetSkill(CUI_Skill* pSkill);
	void SetSkillPos(void);
	void SetSlotIndex(_uint iSlotIndex);

private:
	CUI*					m_pParentWindow;
	_vec2					m_vParentBasePos;
	CUI_Skill*				m_pSkill;
	_uint					m_iSlotIndex;

private:
	HRESULT Add_Component(void);

public:
	static CSkill_Block* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CSkill_Block* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, _vec2 vPos, _uint iSlotIndex = -1, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Skill_Block_h__