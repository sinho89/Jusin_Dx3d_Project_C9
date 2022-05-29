#pragma once
#ifndef Babegazi_Bow_h__
#define Babegazi_Bow_h__

#include "Defines.h"
#include "GameObject.h"
#include "Monster.h"

namespace  Engine
{
	class CCamera;
}

class CSprite;
class CArrow;

class CBabegazi_Bow
	:public CMonster
{
private:
	enum BABEGAZI_BOW {
		STAND_IDLE, WALK, WALK_B, WALK_L, WALK_R, MOVE_RUN, DASH_BACK, JUMP_A, JUMP_B, JUMP_C
		, BATTLE_IDLE, ATTACK_A, ATTACK_B, ATTACK_C, DAM_F, DAM_B, DAM_L, DAM_R, DAM_H, DAM_HARD, DAM_UPPER_A
		, DAM_UPPER_B, DAM_UPPER_C, SPIN_A, SPIN_B, DOWN, STAND_UP, STUN, DISCOVERY, STAND_DOWN, CAPTURE, ANI_END
	};

	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };

	enum AI_STATE {
		AI_ROAMING, AI_DISCOVERY, AI_TRACE, AI_COMBAT, AI_ATTACK,
		AI_DOWN, AI_IDLE, AI_DEATH, AI_END
	};

protected:
	explicit CBabegazi_Bow(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CBabegazi_Bow(void);
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
	CArrow*				m_pArrow;

private:
	_bool				m_bArrowCreate;
	_bool				m_bAttack;
	_bool				m_bShot;
	_float				m_fAttackTime;

	_bool				m_bTest;

public:
	void				InitEffectDust(void);
	void				CollideEffectDust(const _float& fTimeDelta);

public:
	static CBabegazi_Bow* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
protected:
	virtual void Free(void);
};



#endif Babegazi_Warrior_h__