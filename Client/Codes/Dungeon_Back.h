/*!
 * \file Dungeon_Back.h
 * \date 2016/12/19 16:42
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

#ifndef Dungeon_Back_h__
#define Dungeon_Back_h__



#include "UI.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
	class CScene;
}
class CUI;
class CDunGeon_Back : public CUI
{
private:
	explicit CDunGeon_Back(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CDunGeon_Back(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);
	void SetParent(CUI* pParent) { m_pParent = pParent; }
	void SetPos(_vec2 vPos);
	void SetScene(Engine::CScene* pScene) { m_pScene = pScene; }

private:
	CUI*								m_pParent;
private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pNCTextureCom;
	Engine::CTexture*					m_pCTextureCom;
	Engine::CTexture*					m_pSTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
	Engine::CScene*						m_pScene;

public:
	void PtInRect_Collision(const _float& fTimeDelta);

private:
	HRESULT Add_Component(void);

public:
	static CDunGeon_Back* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CDunGeon_Back* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		CUI* pParent, _vec2 vPos, Engine::CScene* pScene, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};



#endif // Dungeon_Back_h__