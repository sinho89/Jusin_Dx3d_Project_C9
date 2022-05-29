#ifndef UI_Fade_Sub_h__
#define UI_Fade_Sub_h__



#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}

class CUI_Fade_Sub : public CUI
{
private:
	explicit CUI_Fade_Sub(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CUI_Fade_Sub(void);

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
private:
	_float					m_fAniTime;
	_float					m_fMagicScale;
	_bool					m_bFadeIn;
	_bool					m_bFadeOut;
	FADE_ID					m_eFadeType;


public:
	void SetFadeType(FADE_ID eType) { m_eFadeType = eType; }
	void FadeUpdate(const _float& fTimeDelta);
public:
	static CUI_Fade_Sub* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CUI_Fade_Sub* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		FADE_ID eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // UI_Fade_Sub_h__
