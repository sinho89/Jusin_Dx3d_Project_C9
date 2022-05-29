/*!
 * \file Game_Start_Banner.h
 * \date 2016/11/15 21:15
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

#ifndef Game_Start_Banner_h__
#define Game_Start_Banner_h__

#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CGame_Start_Banner : public CUI
{
private:
	explicit CGame_Start_Banner(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CGame_Start_Banner(void);

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
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

public:
	void PtInRect_Collision(void);
	void CunvertFontPos(void);
public:
	void Alpha_Calculation(const _float& fTimeDelta);
	void Set_Info(_float fY, _uint iIndex);

private:
	HRESULT Add_Component(void);

public:
	static CGame_Start_Banner* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CGame_Start_Banner* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_float fY, _uint iIndex, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Game_Start_Banner_h__