/*!
 * \file Dungeon_Base.h
 * \date 2016/12/19 10:08
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

#ifndef Dungeon_Base_h__
#define Dungeon_Base_h__

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
class CPortal;
class CDunGeon_Base : public CUI
{
private:
	explicit CDunGeon_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CDunGeon_Base(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);
	void PotalCollision(void);
	void MoveToMouse(void);

private:
	CPortal*								m_pPortal;
	_bool									m_bDirection[4];
private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;


private:
	HRESULT Add_Component(void);

public:
	static CDunGeon_Base* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Dungeon_Base_h__