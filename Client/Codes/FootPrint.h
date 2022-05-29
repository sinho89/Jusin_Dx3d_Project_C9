#pragma once
#ifndef FootPrint_h__
#define FootPrint_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CCube_Texture;
	class CTexture;
	class CShader;
}

class CFootPrint
	:public Engine::CGameObject
{
private:
	explicit CFootPrint(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _int iDecalNum);
	virtual ~CFootPrint(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

public:
	Engine::CTransform*			GetTransCom(void);

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CRenderer*		m_pRendererCom;
	Engine::CShader*		m_pShaderCom;
	Engine::CCube_Texture*	m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::LAYERTYPE		m_eLayerType;
	OBJ_INFO				m_tInfo;

	_int					m_iDecalNum;

	_float					m_fDecalDeadTime;

	_vec3					m_pDecalPosition;

private:
	HRESULT Ready_Object(const _vec3 vPickPos, const _vec3 vPickAngle);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CFootPrint* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _vec3 vPickPos, const _vec3 vPickAngle, const _int iDecalNum);
private:
	virtual void Free(void);
};

#endif