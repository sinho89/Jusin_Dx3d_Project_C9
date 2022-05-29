/*!
 * \file Combo.h
 * \date 2016/12/26 14:46
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

#ifndef Combo_h__
#define Combo_h__




#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CUI;
class CCombo : public CUI
{
private:
	explicit CCombo(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CCombo(void);

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

	CUI*								m_pNum;
	CUI*								m_pNumTen;
	CUI*								m_pNumHun;
	CUI*								m_pEffect;
private:
	HRESULT Add_Component(void);
	void Combo_Animation(const _float& fTimeDelta);
	void Combo_Calculater(const _float& fTimeDelta);
	void Reset(void);


public:
	static CCombo* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Combo_h__