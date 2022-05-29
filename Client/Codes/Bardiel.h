#pragma once
#ifndef Bardiel_h__
#define Bardiel_h__

#include "Defines.h"
#include "GameObject.h"
#include "Monster.h"

namespace  Engine
{
	class CCamera;
}

class CSprite;

class CBardiel
	:public CMonster
{
public:
	enum BARDIEL {
		WAKEUP, WALK_F_2, ATT_WHIRLWIND, ATT_VOLCANO, ATT_SHORTHAMMER,
		ATT_LFOOT, ATT_HAMMER, ATT_FIRE_WALL, ATT_FIRE_RAIN,
		ATT_FIRE_EXPLODE, ATT_FIRE, ATT_DIVIDE, ATT_CLEAN, DESTROYSHIELD,
		CREATESHIELD, CHANGEPHASE2, WALK_R, WALK_L, WALK_F, WALK_B,
		BATTLE_IDLE_1, RUN_CRASH_ING, RUN_CRASH_START, RUN_CRASH_END,
		PENALTY, LAVA, FIREWAVE_START, FIREWAVE_ING, FIREWAVE_END,
		FIRESTORM,
		FIREPILLAR_START, FIREPILLAR_ING_1, FIREPILLAR_ING2, FIREPILLAR_ING3, FIREPILLAR_END,
		FIREFRAME_E, FIREFRAME_D, FIREFRAME_C, FIREFRAME_B, FIREFRAME_A,
		FIREBALL, ATT_AIR_START, ATT_AIR_ING, ATT_AIR_END,
		ABSORB_START, ABSORB_ING, ABSORB_END,
		IDLE_1, RUN_WEAPON, SUMMON, BATTLE_IDLE_2,
		ANI_END
	};
	enum AI_STATE {
		AI_CAMERAINTRO, AI_IDLE_1, AI_COMBAT, AI_PENALTY, AI_LAVA,
		AI_FIREBALL, AI_FIREFRAME, AI_FIREPILLAR,
		AI_CHANGEPHASE2, AI_IDLE_2, AI_COMBAT_2,
		AI_WHIRLWIND, AI_VOLCANO, AI_SHORTHAMMER, AI_LFOOT,
		AI_HAMMER, AI_FIREWALL, AI_FIRERAIN, AI_DIVIDE, AI_CLEAN,
		AI_COMBO, AI_CRAZYFOOT, AI_BREATH, AI_END
	};

protected:
	explicit CBardiel(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CBardiel(void);
public:
	void						Set_CameraStart(const _bool& bStart);
public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	AI_STATE	m_eAIState;
	_bool		m_bWalk[2];
protected:
	HRESULT		Ready_Object(void);
	HRESULT		Add_Component(void);
	HRESULT		Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT		Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT		Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);

protected:
	HRESULT 	LoadAniInfo(const _tchar* pFileName);
	void		Render_Ray(void);

	void		BlurPowerControl(const _float& fTimeDelta);

private:
	Engine::CCamera*	m_pLightCam;
	Engine::CCamera*	m_pCurCam;

private:
	_bool	   m_bGravityStart;
	_bool	   m_bWeapon1;
	_bool	   m_bWeapon2;
	_bool	   m_bArmor;
	_bool	   m_bPhase2;

	_bool		m_bDeath;
	_float		m_fRadialTime;
	_float		m_fDeathRadialBlur;

	_matrix		m_matEffectWorld;

	_float		m_fEffectPowerTime;
	_float		m_fEffectPower;

	Engine::CTexture*	m_pBlur_Body_TransCom;
	Engine::CTexture*	m_pBlur_Shield_TransCom;
	Engine::CTexture*	m_pBlur_HandGun_TransCom;
	Engine::CShader*	m_pEffectShaderCom;

	_matrix				m_matOldWorld;
	_matrix				m_matOldView;
	_matrix				m_matOldProj;
//==««∞› ¿Ã∆Â∆Æ==========================================//
private:
	CSprite*			m_pDust;

private:
	CUI*				m_pBossUIEffect;
	_bool				m_bFadeName;
	_bool				m_bActionUI;
public:
	void				InitEffectDust(void);
	void				CollideEffectDust(const _float& fTimeDelta);
	_bool				GetFadeName(void) { return m_bFadeName; }
public:
	static CBardiel* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
protected:
	virtual void Free(void);
};



#endif Hanuman_h__