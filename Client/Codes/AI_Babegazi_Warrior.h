#pragma once
#ifndef AI_BWarrior_h__
#define AI_BWarrior_h__

#include "AI_State.h"
#include "Defines.h"

//Å¬¶ó
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

class CAI_Babegazi_Warrior
	: public Engine::CAI_State
{
protected:
	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };
	enum BABEGAZI_WARRIOR {
		STAND_IDLE, WALK, WALK_B, WALK_L, WALK_R, STAND_DOWN, STAND_UP, STUN, SPIN_A, SPIN_B
		, MOVE_RUN, DOWN, DISCOVERY, DASH_BACK, DEFENCE_SWORD, DEFENCE, ATTACK, BATTLE_IDLE, CAPTURE, DAM_B, DAM_F
		, DAM_H, DAM_HARD, DAM_L, DAM_R, DAM_SWO, DAM_UPPER_A, DAM_UPPER_B, DAM_UPPER_C, JUMP_A, JUMP_B, JUMP_C,
		ANI_END
	};
	enum AI_STATE {
		AI_ROAMING, AI_DISCOVERY, AI_TRACE, AI_COMBAT, AI_ATTACK, AI_DEFENCE,
		AI_DOWN, AI_IDLE, AI_DEATH, AI_END
	};
protected:
	explicit CAI_Babegazi_Warrior(void);
	virtual ~CAI_Babegazi_Warrior(void);

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
