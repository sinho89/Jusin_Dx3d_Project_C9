/*!
 * \file ComboNum.h
 * \date 2016/12/26 18:56
 *
 * \author ShinHo
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ComboNum_h__
#define ComboNum_h__


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

class CComboNum : public CUI
{
private:
	explicit CComboNum(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CComboNum(void);

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
	void SetParent(Engine::CGameObject* pParent) { m_pParent = pParent; }
	void SetType(NUM_ID eType) { m_eNumType = eType; }
private:
	_float					m_fAniTime;
	_short					m_sIndex;
	Engine::CGameObject*	m_pParent;
	NUM_ID					m_eNumType;

public:
	static CComboNum* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CComboNum* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		Engine::CGameObject* pParent, NUM_ID eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};

#endif // ComboNum_h__