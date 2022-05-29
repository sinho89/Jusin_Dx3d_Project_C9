/*!
 * \file Inven_Char_Model.h
 * \date 2017/01/02 13:13
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

#ifndef Inven_Char_Model_h__
#define Inven_Char_Model_h__


#include "Inven_Space.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
	class CRenderer;
	class CShader;
}

class CInven_Char_Model : public CInven_Space
{
protected:
	explicit CInven_Char_Model(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CInven_Char_Model(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(void);

private:
	_float					m_fFrame;
	_uint					m_iAniIndex;
	_float					m_fRenderZ;
	_bool					m_bType;
private:
	Engine::CTransform*					m_pTransCom;
	Engine::CDynamicMesh*				m_pMeshCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
	_matrix								m_matUITransform;

public:
	void SetParent(CUI* pParentWindow);
	void SetType(_bool bType) { m_bType = bType; }
	_matrix* GetUITransForm(void) { return &m_matUITransform; }

private:
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CInven_Char_Model* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	static CInven_Char_Model* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		CUI* pParentWindow, _bool bType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};


#endif // Inven_Char_Model_h__