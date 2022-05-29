#ifndef Fade_h__
#define Fade_h__



#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CFade : public CUI
{
private:
	explicit CFade(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CFade(void);

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

private:
	HRESULT Add_Component(void);
public:
	void CreateSelectItem(void);
private:
	_float	m_fTime;

public:
	static CFade* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Fade_h__
