#pragma once
#ifndef AI_Hanuman_h__
#define AI_Hanuman_h__

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

class CAI_Hanuman
	: public Engine::CAI_State
{
protected:
	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };
	enum HANUMAN {
		STAND_IDLE, WALK, MOVE_RUN, BATTLE_IDLE, ATTACK1, ATTACK2, ROUNDPUNCH_A, ROUNDPUNCH_B, ROUNDPUNCH_C
		, ROUNDPUNCH_C1_START, ROUNDPUNCH_C1_ING, ROUNDPUNCH_C1_END, COMBO0, COMBO1, COMBO2, COMBO2_END, BREATH_0, BREATH_1, ROLL_L, ROLL_R
		, ROAR, CRAZYFOOT, STAND_DOWN, STUN, DAMAGE, ROUNDPUNCH_DAMAGED, JUMP_A, JUMP_B, JUMP_C, SHIELD_START, SHIELD_ING, SHIELD_END,
		ANI_END
	};
	enum AI_STATE {
		AI_CAMERAINTRO, AI_IDLE, AI_TRACE, AI_ROUNDPUNCH, AI_ATTACK1, AI_ATTACK2, AI_ROAR,
		AI_COMBO, AI_CRAZYFOOT, AI_BREATH, AI_END
	};
protected:
	explicit CAI_Hanuman(void);
	virtual ~CAI_Hanuman(void);

protected:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

protected:
	_uint		m_iPatternCount;

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
