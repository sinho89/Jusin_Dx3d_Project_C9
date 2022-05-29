#ifndef ResultNum_h__
#define ResultNum_h__

#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
	class CGameObject;
}

class CResultNum : public CUI
{
private:
	explicit CResultNum(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CResultNum(void);

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
	void SetTypeIndex(_uint iTypeIndex) { m_iTypeIndex = iTypeIndex; }

private:
	_uint m_iTypeIndex;
	_float m_fFadeTime;
	_float m_fTime;
public:
	static CResultNum* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CResultNum* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_uint iIndex, _uint iTypeIndex, _vec2 vPos, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // ResultNum_h__
