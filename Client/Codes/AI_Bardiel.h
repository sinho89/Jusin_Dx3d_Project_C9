#pragma once
#ifndef AI_Bardiel_h__
#define AI_Bardiel_h__

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

class CAI_Bardiel
	: public Engine::CAI_State
{
protected:
	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };
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
	explicit CAI_Bardiel(void);
	virtual ~CAI_Bardiel(void);

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
