#pragma once
#ifndef AI_Discovery_h__
#define AI_Discovery_h__

#include "AI_State.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
	class CAnimater;
	class CNavigationMesh;
	class CStaticMesh;
	class CCalculator;
	class CRigidbody;
}

class CAI_Discovery
	: public Engine::CAI_State
{
public:
	//	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };
	enum BABEGAZI_WARRIOR {
		STAND_IDLE, WALK, WALK_B, WALK_L, WALK_R, STAND_DOWN, STAND_UP, STUN, SPIN_A, SPIN_B
		, MOVE_RUN, DOWN, DISCOVERY, DASH_BACK, DEFENCE_SWORD, DEFENCE, ATTACK, BATTLE_IDLE, CAPTURE, DAM_B, DAM_F
		, DAM_H, DAM_HARD, DAM_L, DAM_R, DAM_SWO, DAM_UPPER_A, DAM_UPPER_B, DAM_UPPER_C, JUMP_A, JUMP_B, JUMP_C, ANI_END
	};
private:
	explicit CAI_Discovery(void);
	virtual ~CAI_Discovery(void);


public:
	virtual void Start_State(void);
	virtual void Start_State(_vec3* pDir, _float fForce);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	void		CubeRayCheck(void);

private:
	HANDLE						m_hThread;
	CRITICAL_SECTION			m_Crt;

private:
	_float m_fAggroRad;
	_bool  m_bAggro;

private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody);
public:
	static CAI_Discovery* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody);
private:
	virtual void Free(void);
};

#endif // AI_Trace_h__
