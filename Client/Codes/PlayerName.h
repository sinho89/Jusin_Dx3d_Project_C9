#ifndef PlayerName_h__
#define PlayerName_h__


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

class CPlayer_Name : public CUI
{
private:
	explicit CPlayer_Name(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CPlayer_Name(void);

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
	MONSTER_ID		m_eMonType;
	Engine::CGameObject* m_pParentObject;
	float		m_fDepth;
	float		m_fCamDist;
public:
	void SetParent(Engine::CGameObject* pParent) { m_pParentObject = pParent; }
	void SetShopType(MONSTER_ID eType) { m_eMonType = eType; }
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CPlayer_Name* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CPlayer_Name* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pParentObject, MONSTER_ID eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // PlayerName_h__
