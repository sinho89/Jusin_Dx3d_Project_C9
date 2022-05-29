/*!
 * \file Model_WB_Weapon.h
 * \date 2017/01/02 13:22
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

#ifndef Model_WB_Weapon_h__
#define Model_WB_Weapon_h__


#include "Inven_Space.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
	class CRenderer;
	class CShader;
}

class CModel_WB_Weapon : public CInven_Space
{
protected:
	explicit CModel_WB_Weapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);
	virtual ~CModel_WB_Weapon(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	HRESULT Ready_Object(const _tchar* pTargetName, const _tchar* pFilePath);

private:
	_float					m_fFrame;
	_uint					m_iAniIndex;
	_float					m_fRenderZ;
	MODEL_ID				m_eMeshType;
	_bool					m_bUpdate;
private:
	Engine::CTransform*					m_pTransCom;
	Engine::CStaticMesh*				m_pMeshCom;
	Engine::CRenderer*					m_pRendererCom;
	Engine::CShader*					m_pShaderCom;
	Engine::CAnimator*					m_pPlayerAniCom;

private:
	const _matrix* m_pmatModelTarget;
	const _matrix* m_pmatModelWeapon;
	CUI*		m_pTargetUI;
	CPlayer*	m_pWorldPlayer;

public:
	void SetParent(CUI* pParentWindow);
	void SetType(MODEL_ID eType) { m_eMeshType = eType; }

private:
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT LoadModelWeaponData(const _tchar* pTargetName, const _tchar* pFilePath);

public:
	static CModel_WB_Weapon* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const _tchar* pTargetName, const _tchar* pFilePath, CUI* pParentWindow, MODEL_ID eType, const Engine::OBJECTSTATE& eState = Engine::STATE_DEFAULT, const _bool bIsActive = true);

public:
	virtual void Free(void);
};



#endif // Model_WB_Weapon_h__