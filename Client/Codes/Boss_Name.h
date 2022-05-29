#ifndef Boss_Name_h__
#define Boss_Name_h__


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

class CBossName : public CUI
{
private:
	explicit CBossName(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CBossName(void);

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
	_uint m_iTypeIndex;
	_float m_fFadeTime;
	_float m_fTime;
	_float m_fStartUV;
	_float m_fEndUV;
	FADE_ID					m_eFadeType;
	_bool	m_bSound;

public:
	void SetFadeType(FADE_ID eType) { m_eFadeType = eType; }
public:
	static CBossName* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CBossName* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		_uint iIndex, FADE_ID eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Boss_Name_h__
