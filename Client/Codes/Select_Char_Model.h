/*!
 * \file Select_Char_Model.h
 * \date 2016/11/15 9:38
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief Model for Select CHAR
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Select_Char_Model_h__
#define Select_Char_Model_h__

#include "Defines.h"
#include "UI.h"

namespace  Engine
{
	class CTransform;
	class CDynamicMesh;
	class CRenderer;
	class CShader;
	class CCamera;
}

class CSound;

class CSelect_Char_Model
	: public CUI
{
private:
	explicit					CSelect_Char_Model(LPDIRECT3DDEVICE9 pDevice, const wstring& wstrName);
	virtual						~CSelect_Char_Model(void);

public:
	virtual HRESULT				Ready_Object(void);
	virtual	HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CDynamicMesh*		m_pMeshCom[5];
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	CSound*						m_pSoundCom;

	_bool						m_bCanTalk;

public:
	void						SetMotionStart();

private:
	_float						m_fFrame;
	_uint						m_iAniIndex;

	Engine::CCamera*			m_pLightCam;
	Engine::CCamera*			m_pCurCam;

private:
	HRESULT						Add_Component(void);
	HRESULT						Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);

public:
	static CSelect_Char_Model*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	virtual	void				Free(void);
};

#endif // Select_Char_Model_h__