#pragma once
#ifndef AI_Knole_Commander_h__
#define AI_Knole_Commander_h__

#include "AI_State.h"
#include "Defines.h"

//클라
namespace Engine
{
	class CTransform;
	class CAnimater;
	class CNavigationMesh;
	class CStaticMesh;
	class CCalculator;
	class CRigidbody;
	class CGameObject;
}

class CAI_Knole_Commander
	: public Engine::CAI_State
{
protected:
	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };
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
	explicit CAI_Knole_Commander(void);
	virtual ~CAI_Knole_Commander(void);

protected:
	_bool*		m_pWeapon;	//true면 손에.. false면 등에 렌더
	_bool		m_bRun;

protected:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

protected:
	void		CubeRayCheck(void);

protected:
	void		Set_Ray(void);
//	void		Collsion_Work(void);
	void        ChangeAni_Work(void);
	void		ChangeAI_Work(void);

protected:
	virtual void Free(void);
};

#endif // AI_Down_h__
