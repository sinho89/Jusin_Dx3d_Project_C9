/*!
 * \file Select_Char_Button.h
 * \date 2016/11/14 13:33
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

#ifndef Select_Char_Button_h__
#define Select_Char_Button_h__

#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CSelect_Char_Button : public CUI
{
private:
	explicit CSelect_Char_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CSelect_Char_Button(void);

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
	Engine::CTexture*					m_pClickTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

public:
	void PtInRect_Collision(const _float& fTimeDelta);
	void CunvertFontPos(void);
	_bool	GetSlotActive();
public:
	void Alpha_Calculation(const _float& fTimeDelta);
	void Set_Info(_float fY, _uint iIndex);
	bool Load_Player_Info();

	PLAYER_INFO*	GetPlayerInfo();

	void SetSlotActive(_bool bSlotActive);

private:
	PLAYER_INFO		m_tPlayerInfo;
	_bool			m_bSlotActive;
	_float			m_fClickTime;
private:
	HRESULT Add_Component(void);

public:
	static CSelect_Char_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CSelect_Char_Button* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_float fY, _uint iIndex, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Select_Char_Button_h__