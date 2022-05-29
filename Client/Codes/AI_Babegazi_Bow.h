#pragma once
#ifndef AI_BBow_h__
#define AI_BBow_h__

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

class CAI_Babegazi_Bow
	: public Engine::CAI_State
{
protected:
	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };
	enum BABEGAZI_BOW {
		STAND_IDLE, WALK, WALK_B, WALK_L, WALK_R, MOVE_RUN, DASH_BACK, JUMP_A, JUMP_B, JUMP_C
		, BATTLE_IDLE, ATTACK_A, ATTACK_B, ATTACK_C, DAM_F, DAM_B, DAM_L, DAM_R, DAM_H, DAM_HARD, DAM_UPPER_A
		, DAM_UPPER_B, DAM_UPPER_C, SPIN_A, SPIN_B, DOWN, STAND_UP, STUN, DISCOVERY, STAND_DOWN, CAPTURE, ANI_END
	};
	enum AI_STATE {
		AI_ROAMING, AI_DISCOVERY, AI_TRACE, AI_COMBAT, AI_ATTACK,
		AI_DOWN, AI_IDLE, AI_DEATH, AI_END
	};
protected:
	explicit CAI_Babegazi_Bow(void);
	virtual ~CAI_Babegazi_Bow(void);

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

#endif // AI_BBow_h__
