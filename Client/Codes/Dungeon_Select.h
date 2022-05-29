/*!
 * \file Dungeon_Select.h
 * \date 2016/12/19 13:57
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

#ifndef Dungeon_Select_h__
#define Dungeon_Select_h__


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
class CDunGeon_Select : public CUI
{
private:
	explicit CDunGeon_Select(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CDunGeon_Select(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);
	void SetParent(CUI* pParent) { m_pParent = pParent; }
	void SetType(_bool bType) { m_bType = bType; }
	void SetID(DUNGEON_ID bID) { m_eType = bID; }
	void SetPos(_vec2 vPos);

private:
	CUI*								m_pParent;
	_bool								m_bType;
	DUNGEON_ID							m_eType;
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

private:
	HRESULT Add_Component(void);

public:
	static CDunGeon_Select* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CDunGeon_Select* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		CUI* pParent, _vec2 vPos, _bool bType, DUNGEON_ID eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Dungeon_Select_h__