#ifndef BossOutBar_h__
#define BossOutBar_h__





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
class CBossOutBar : public CUI
{
private:
	explicit CBossOutBar(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CBossOutBar(void);

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
	_uint			m_iHpLineCnt;
private:
	HRESULT Add_Component(void);
	void SetTarget(Engine::CGameObject* pTarget) { m_pBossTarget = pTarget; }

public:
	static CBossOutBar* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CBossOutBar* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pTarget, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // BossOutBar_h__
