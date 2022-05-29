#pragma once
#ifndef Babegazi_Axe_h__
#define Babegazi_Axe_h__

#include "Defines.h"
#include "GameObject.h"
#include "Monster.h"

namespace  Engine
{
	class CCamera;
}

class CSprite;

class CBabegazi_Axe
	:public CMonster
{
private:
	enum BABEGAZI_AXE {
		CAPTURE, STAND_IDLE, WALK, WALK_B, WALK_L, WALK_R, MOVE_RUN, DASH_BACK, JUMP_A, JUMP_B, JUMP_C
		, BATTLE_IDLE, ATTACK1, ATTACK2, DAM_F, DAM_B, DAM_L, DAM_R, DAM_H, DAM_HARD, DAM_SWO, DAM_UPPER_A
		, DAM_UPPER_B, DAM_UPPER_C, DEFENCE_SWORD, DEFENCE, SPIN_A, SPIN_B, DOWN, STAND_UP, STUN, DISCOVERY, STAND_DOWN
		, ANI_END
	};

	enum AI_STATE {
		AI_ROAMING, AI_DISCOVERY, AI_TRACE, AI_COMBAT, AI_ATTACK1, AI_ATTACK2,
		AI_DEFENCE,
		AI_DOWN, AI_IDLE, AI_DEATH, AI_END
	};

protected:
	explicit CBabegazi_Axe(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CBabegazi_Axe(void);
public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	AI_STATE	m_eAIState;

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
	void        DamagedSound(void);

private:
	Engine::CCamera*	m_pLightCam;
	Engine::CCamera*	m_pCurCam;


//==««∞› ¿Ã∆Â∆Æ==========================================//
private:
	CSprite*			m_pDust;

public:
	void				InitEffectDust(void);
	void				CollideEffectDust(const _float& fTimeDelta);

public:
	static CBabegazi_Axe* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
protected:
	virtual void Free(void);
};



#endif Babegazi_Axe_h__