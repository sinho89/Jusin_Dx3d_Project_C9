/*!
 * \file Model_Floor.h
 * \date 2016/11/18 15:50
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Model_Floor_h__
#define Model_Floor_h__

#include "Defines.h"
#include "UI.h"

namespace  Engine
{
	class CTransform;
	class CStaticMesh;
	class CRenderer;
	class CShader;
}

class CModel_Floor
	: public CUI
{
private:
	explicit				CModel_Floor(LPDIRECT3DDEVICE9 pDevice, const wstring& wstrName);
	virtual					~CModel_Floor(void);

public:
	virtual HRESULT			Ready_Object(void);
	virtual void			Update_Object(const _float& fTimeDelta);
	virtual void			Render_Object(void);
	virtual	void			Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);
public:
	void					SetInit(void);
private:
	Engine::CTransform*		m_pTransCom;
	Engine::CStaticMesh*	m_pMeshCom[5];
	Engine::CRenderer*		m_pRendererCom;
	Engine::CShader*		m_pShaderCom;

private:
	HRESULT					Add_Component(void);
	HRESULT					Set_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CModel_Floor*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	virtual	void			Free(void);
};

#endif // Model_Floor_h__