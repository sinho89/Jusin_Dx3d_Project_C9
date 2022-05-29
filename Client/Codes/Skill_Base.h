/*!
 * \file Skill_Base.h
 * \date 2016/11/25 12:38
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

#ifndef Skill_Base_h__
#define Skill_Base_h__



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
class CUI;
class CSkill_Base : public CUI
{
private:
	explicit CSkill_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CSkill_Base(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);

private:
	CUI*			m_pActionSheet;
	CUI*			m_pComandSheet;
	CUI*			m_pPassiveSheet;
private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom[4];
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

private:
	_bool m_bType;

public:
	void Window_MoveToMouse();
	void SetType(_bool bType) { m_bType = bType; }
private:
	HRESULT Add_Component(void);

public:
	static CSkill_Base* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CSkill_Base* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_bool bType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Skill_Base_h__