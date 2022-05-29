/*!
 * \file Loading_Gage.h
 * \date 2016/11/21 10:36
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

#ifndef Loading_Gage_h__
#define Loading_Gage_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CLoading_Gage : public CUI
{
private:
	explicit CLoading_Gage(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CLoading_Gage(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
public:
	void SetGageSize(const _float& fTimeDelta);
public:
	HRESULT Ready_Object(void);

private:
	Engine::CTransform*					m_pTransCom;
	Engine::CRect_Texture*				m_pBufferCom;
	Engine::CTexture*					m_pTextureCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;

private:
	_float								m_fMagicScale;
public:
	void Alpha_Calculation(const _float& fTimeDelta);
private:
	HRESULT Add_Component(void);
	void SetIndex(_uint iIndex) { m_iIndex = iIndex; }

public:
	static CLoading_Gage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, 
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CLoading_Gage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_uint iIndex, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Loading_Gage_h__