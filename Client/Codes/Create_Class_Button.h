/*!
 * \file Create_Class_Button.h
 * \date 2016/11/16 16:37
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

#ifndef Create_Class_Button_h__
#define Create_Class_Button_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CCreate_Class_Button : public CUI
{
private:
	explicit CCreate_Class_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CCreate_Class_Button(void);

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

public:
	void PtInRect_Collision(const _float& fTimeDelta);
	void CunvertFontPos(void);
public:
	void Alpha_Calculation(const _float& fTimeDelta);
	void Set_Info(_float fX, _float fY, _uint iIndex);

private:
	_tchar*	m_szName;

private:
	HRESULT Add_Component(void);

public:
	static CCreate_Class_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CCreate_Class_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_float fX, _float fY, _uint iIndex, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Create_Class_Button_h__