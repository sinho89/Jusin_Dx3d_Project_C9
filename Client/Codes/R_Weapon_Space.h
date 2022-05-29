/*!
 * \file Armor_Space.h
 * \date 2016/11/26 10:43
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

#ifndef R_Weapon_Space_h__
#define R_Weapon_Space_h__


#include "Inven_Space.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CR_Weapon_Space : public CInven_Space
{
protected:
	explicit CR_Weapon_Space(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CR_Weapon_Space(void);

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

private:
	CWeapon*							m_pRedLeftWeapon;
	CWeapon*							m_pRedRightWeapon;
	CWeapon*							m_pRedLeftWeapon_In;
	CWeapon*							m_pRedRightWeapon_In;
	CUI*							m_pModel_RedLeftWeapon;
	CUI*							m_pModel_RedRightWeapon;
	CUI*							m_pModel_RedLeftWeapon_In;
	CUI*							m_pModel_RedRightWeapon_In;
public:
	void SetParent(CUI* pParentWindow);
	void RenderItem();
private:
	HRESULT Add_Component(void);
	_bool			m_bLastBackUp;

public:
	static CR_Weapon_Space* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CR_Weapon_Space* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		CUI* pParentWindow, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Armor_Space_h__