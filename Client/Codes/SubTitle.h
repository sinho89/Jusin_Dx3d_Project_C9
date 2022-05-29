#ifndef SubTitle_h__
#define SubTitle_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
}
class CSubTitle : public CUI
{
private:
	explicit CSubTitle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CSubTitle(void);

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
	_float			m_fFadeTime;
	_float			m_fTime;
	_uint			m_iFadeType;
	_float			m_fOrifX;
	_float			m_fFadeStop;
public:
	void SetFadeTime(_float fFadeTime) { m_fFadeTime = fFadeTime; }
	void SetFadeType(_uint iFadeType) { m_iFadeType = iFadeType; }
private:
	HRESULT Add_Component(void);

public:
	static CSubTitle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CSubTitle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_uint iIndex, _uint iFadeType, _float fFadeTime, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // SubTitle_h__
