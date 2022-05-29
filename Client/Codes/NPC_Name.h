#ifndef NPC_Name_h__
#define NPC_Name_h__




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

class CNPC_Name : public CUI
{
private:
	explicit CNPC_Name(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CNPC_Name(void);

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
	SHOP_ID		m_eShopType;
	Engine::CGameObject* m_pParentObject;
	float		m_fDepth;
	float		m_fCamDist;
public:
	void SetParent(Engine::CGameObject* pParent) { m_pParentObject = pParent; }
	void SetShopType(SHOP_ID eType) { m_eShopType = eType; }
	HRESULT SetUp_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CNPC_Name* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CNPC_Name* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pParentObject, SHOP_ID eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // NPC_Name_h__
