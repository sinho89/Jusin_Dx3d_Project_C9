#ifndef Critical_h__
#define Critical_h__


#include "UI.h"

namespace Engine
{
	class CTransform;
	class CRect_Texture;
	class CTexture;
	class CRenderer;
	class CShader;
	class CUICalculater;
	class CGameObject;
}

class CCritical : public CUI
{
private:
	explicit CCritical(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CCritical(void);

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
	Engine::CUICalculater*				m_pUICarCom;

public:
	void Alpha_Calculation(const _float& fTimeDelta);
private:
	HRESULT Add_Component(void);

private:
	Engine::CGameObject* m_pParentObject;
	float		m_fDepth;
	float		m_fCamDist;
	float		m_fYIncrease;
	float		m_fYTime;
	float		m_fFadeTime;
	float		m_fScaleTime;
public:
	void SetParent(Engine::CGameObject* pParent) { m_pParentObject = pParent; }
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CCritical* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CCritical* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pParentObject, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // Critical_h__
