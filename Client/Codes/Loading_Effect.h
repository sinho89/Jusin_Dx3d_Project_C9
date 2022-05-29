/*!
 * \file Loading_Effect.h
 * \date 2016/11/21 10:04
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

#ifndef Loading_Effect_h__
#define Loading_Effect_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CLoading_Effect : public CUI
{
private:
	explicit CLoading_Effect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CLoading_Effect(void);

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

public:
	void Alpha_Calculation(const _float& fTimeDelta);
	void Set_Pos(_vec2 vPos);
private:
	HRESULT Add_Component(void);

public:
	static CLoading_Effect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CLoading_Effect* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_vec2 vPos,	const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Loading_Effect_h__