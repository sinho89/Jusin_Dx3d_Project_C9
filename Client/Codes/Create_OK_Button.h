/*!
 * \file Create_OK_Button.h
 * \date 2016/11/17 9:47
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

#ifndef Create_OK_Button_h__
#define Create_OK_Button_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CCreate_OK_Button : public CUI
{
private:
	explicit CCreate_OK_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CCreate_OK_Button(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);

public:
	void Render_Font_MoniterToWindow(const _float& fX, const _float& fY, const _tchar* pFontString);
private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pNotColTextureCom;
	Engine::CTexture*					m_pColTextureCom;
	Engine::CTexture*					m_pClickTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

public:
	void PtInRect_Collision();
	void Alpha_Calculation(const _float& fTimeDelta);
	void Set_Info(_float fX, _float fY, _uint iIndex);
private:
	HRESULT Add_Component(void);

public:
	static CCreate_OK_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CCreate_OK_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_float fX, _float fY, _uint iIndex, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Create_OK_Button_h__