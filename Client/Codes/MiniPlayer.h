/*!
 * \file MiniPlayer.h
 * \date 2016/11/22 15:21
 *
 * \author WINTER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef MiniPlayer_h__
#define MiniPlayer_h__

#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CMiniPlayer
	: public CUI
{
private:
	explicit				CMiniPlayer(LPDIRECT3DDEVICE9 pGraphicDev, 
										const wstring& wstrName, 
										const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, 
										const _bool bIsActive = true);
	virtual					~CMiniPlayer(void);

public:
	virtual 				HRESULT Start_ForScene(void);
	virtual 				void Update_Object(const _float& fTimeDelta);
	virtual 				void Render_Object(void);

public:
	HRESULT					Ready_Object(void);

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CRect_Texture*	m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CRenderer*		m_pRendererCom;
	Engine::CShader*		m_pShaderCom;

private:
	Engine::CTransform*		m_pPlayerTrans;

private:
	HRESULT					Add_Component(void);

public:
	static CMiniPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const wstring& wstrName, 
									const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, 
									const _bool bIsActive = true);

public:
	virtual void			Free(void);
};

#endif // MiniPlayer_h__