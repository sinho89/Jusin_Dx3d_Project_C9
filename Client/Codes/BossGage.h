#ifndef BossGage_h__
#define BossGage_h__




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
class CBossGage : public CUI
{
private:
	explicit CBossGage(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CBossGage(void);

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
	Engine::CGameObject*				m_pBossTarget;

private:
	_float			m_fGage;
	_float			m_fOriGage;
	_float			m_fGageTime;
	_float			m_fEffectSpeed;
	_float			m_fWhiteAlpha;
	_bool			m_bEffect;
	_bool			m_bWhiteAlpha;
	int				m_fHPLevel;
private:
	HRESULT Add_Component(void);
	void SetTarget(Engine::CGameObject* pTarget) { m_pBossTarget = pTarget; }

public:
	static CBossGage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CBossGage* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pTarget, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};
#endif // BossGage_h__
