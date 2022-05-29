#pragma once
#ifndef Hanuman_h__
#define Hanuman_h__

#include "Defines.h"
#include "GameObject.h"
#include "Monster.h"

namespace  Engine
{
	class CCamera;
}

class CSprite;
class CUI;
class CHanuman
	:public CMonster
{
public:
	enum HANUMAN {
		STAND_IDLE, WALK, MOVE_RUN, BATTLE_IDLE, ATTACK1, ATTACK2, ROUNDPUNCH_A, ROUNDPUNCH_B, ROUNDPUNCH_C
		, ROUNDPUNCH_C1_START, ROUNDPUNCH_C1_ING, ROUNDPUNCH_C1_END, COMBO0, COMBO1, COMBO2, COMBO2_END, BREATH_0, BREATH_1, ROLL_L, ROLL_R
		, ROAR, CRAZYFOOT, STAND_DOWN, STUN, DAMAGE, ROUNDPUNCH_DAMAGED, JUMP_A, JUMP_B, JUMP_C, SHIELD_START, SHIELD_ING, SHIELD_END,
		ANI_END
	};
	enum AI_STATE {
		AI_CAMERAINTRO, AI_IDLE, AI_TRACE, AI_ROUNDPUNCH, AI_ATTACK1, AI_ATTACK2, AI_ROAR,
		AI_COMBO, AI_CRAZYFOOT, AI_BREATH, AI_DEATH, AI_END
	};

protected:
	explicit CHanuman(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHanuman(void);

public:
	void						Set_CameraStart(const _bool& bStart);

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

	void		BlurPowerControl(const _float& fTimeDelta);

private:
	Engine::CCamera*	m_pLightCam;
	Engine::CCamera*	m_pCurCam;

	Engine::CTexture*	m_pBlurTransCom;
	Engine::CShader*	m_pEffectShaderCom;

	_matrix				m_matEffectWorld;
	
	_float				m_fEffectPowerTime;
	_float				m_fEffectPower;

	_float				m_fDeadRadialBlur;

	_matrix				m_matOldWorld;
	_matrix				m_matOldView;
	_matrix				m_matOldProj;

//==««∞› ¿Ã∆Â∆Æ==========================================//
private:
	CSprite*			m_pDust;
private:
	CUI*				m_pBossUIEffect;
public:
	void				InitEffectDust(void);
	void				CollideEffectDust(const _float& fTimeDelta);

	//const _float		GetEffectPower(void) const { return m_fEffectPower; }
	//void				SetEffectPower(const _float fEffectPower) { m_fEffectPower = fEffectPower; }
	//void				PlusEffectPower(const _float fEffectPower) { m_fEffectPower += fEffectPower; }

public:
	static CHanuman* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);
protected:
	virtual void Free(void);
};



#endif Hanuman_h__