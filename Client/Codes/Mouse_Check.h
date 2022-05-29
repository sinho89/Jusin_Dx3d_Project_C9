/*!
 * \file Mouse_Check.h
 * \date 2016/12/14 21:08
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

#ifndef Mouse_Check_h__
#define Mouse_Check_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
	class CScene;
}

class CMouse_Check : public CUI
{
private:
	explicit CMouse_Check(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CMouse_Check(void);

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
	Engine::CScene*						m_pScene;

private:
	CUI*								m_pParent;
public:
	void PtInRect_Collision(const _float& fTimeDelta);
	void SetParent(CUI* pParent);
	void SetScene(Engine::CScene* pScene) { m_pScene = pScene; }

private:
	HRESULT Add_Component(void);

public:
	static CMouse_Check* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CMouse_Check* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		CUI* pParent, Engine::CScene* pScene, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Mouse_Check_h__