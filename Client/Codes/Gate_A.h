#pragma once
#ifndef Gate_A_h__
#define Gate_A_h__

#include "Defines.h"
#include "GameObject.h"
#include "Gate.h"

class CSprite;

class CGate_A
	:public CGate
{
private:
	enum Gate_A {
		OPENED, CLOSED, OPEN
		, ANI_END
	};

protected:
	explicit CGate_A(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CGate_A(void);
public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

protected:
	HRESULT		Ready_Object(void);
	HRESULT		Add_Component(void);
	HRESULT		Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT		Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT		Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);

protected:
	HRESULT 	LoadAniInfo(const _tchar* pFileName);
	void		Render_Ray(void);

private:
	Engine::CCamera*	m_pLightCam;
	Engine::CCamera*	m_pCurCam;

private:
	_bool				m_bOpen;
	_vec3				m_vOpenPos;


//==««∞› ¿Ã∆Â∆Æ==========================================//
private:
	CSprite*			m_pDust;

public:
	void				InitEffectDust(void);
	void				CollideEffectDust(const _float& fTimeDelta);

public:
	static CGate_A* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
protected:
	virtual void Free(void);
};



#endif Gate_A_h__