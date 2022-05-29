#ifndef DamNum_h__
#define DamNum_h__


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

class CDamNum : public CUI
{
private:
	explicit CDamNum(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CDamNum(void);

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
	NUM_ID		m_eNumType;
	_bool		m_bFive;
	_bool		m_bCritical;
	_bool		m_bFontScale;
	float		m_fFadeTime;
	float		m_fYTime;
	float		m_fScaleTime;
	_bool		m_bBackBlur;
public:
	void SetParent(Engine::CGameObject* pParent) { m_pParentObject = pParent; }
	void SetNumType(NUM_ID eID) { m_eNumType = eID; }
	void SetFive(_bool bFive) { m_bFive = bFive; }
	void SetCritical(_bool bCritical) { m_bCritical = bCritical; }
	void SetBlur(_bool bBackBlur) { m_bBackBlur = bBackBlur; }
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CDamNum* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CDamNum* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pParentObject, NUM_ID eID, _uint iIndex, _bool bBackBlur, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // DamNum_h__
