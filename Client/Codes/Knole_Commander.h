#pragma once
#ifndef Knole_Commander_h__
#define Knole_Commander_h__

#include "Defines.h"
#include "GameObject.h"
#include "Monster.h"

namespace  Engine
{
	class CCamera;
}

class CSprite;

class CKnole_Commander
	:public CMonster
{
private:
	enum KNOLE_COMMANDER {
		BATTLE_IDLE_1, STAND_IDLE, WALK_F, WALK_L, WALK_R, MOVE_RUN,
		DAM_B, DAM_F, DAM_HARD, DAM_L, DAM_R, DAM_UPPER_A, DAM_UPPER_B, DAM_UPPER_C,
		MOVE_DASH_L, MOVE_DASH_R,
		DEFENCE_START, DEFENCE_ING, DEFENCE_END, DOWN, BATTLE_IDLE_2,
		HAND_START, HAND_END, HAND_ING, MODE_1_IDLE,
		JUMP_A, JUMP_B, JUMP_RUN, JUMP_C, BODY_SPIN_A, BODY_SPIN_B, BODY_SPIN_C,
		SPIN_A, SPIN_B, CAPTURE, CAPTURE_DEFENCE, STAND_DOWN, STAND_UP,
		WEAPON_IN, WEAPON_OUT, ATTACK_1, ATTACK_2, HOLD, MODE_2_BATTLE,
		DISCOVERY, MOVE_RUN_READY, STUN,
		ANI_END
	};
	enum AI_STATE {
		AI_ROAMING, AI_DISCOVERY, AI_TRACE, AI_COMBAT1, AI_HANDATTACK1, AI_BODYATTACK1,
		AI_COMBAT2, AI_ATTACK1, AI_ATTACK2, AI_DEFENCE,
		AI_DOWN, AI_IDLE, AI_DEATH, AI_END
	};

protected:
	explicit CKnole_Commander(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CKnole_Commander(void);
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
	void		DamagedSound(void);

private:
	Engine::CCamera*	m_pLightCam;
	Engine::CCamera*	m_pCurCam;

private:
	_bool				m_bWeapon;


//==««∞› ¿Ã∆Â∆Æ==========================================//
private:
	CSprite*			m_pDust;

public:
	void				InitEffectDust(void);
	void				CollideEffectDust(const _float& fTimeDelta);

public:
	static CKnole_Commander* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
protected:
	virtual void Free(void);
};



#endif Knole_Commander_h__