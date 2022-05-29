#ifndef Roaming_h__
#define Roaming_h__

#include "AI_State.h"
#include "Defines.h"

class CAI_State;

namespace Engine
{
	class CTransform;
	class CAnimater;
	class CNavigationMesh;
}

class CAI_Roaming
	: public Engine::CAI_State
{
public:
	enum ROAM_DIR { DIR_EAST, DIR_WEST, DIR_SOUTH, DIR_NORTH, DIR_END };
	enum BABEGAZI_WARRIOR {
		STAND_IDLE, WALK, WALK_B, WALK_L, WALK_R, STAND_DOWN, STAND_UP, STUN, SPIN_A, SPIN_B
		, MOVE_RUN, DOWN, DISCOVERY, DASH_BACK, DEFENCE_SWORD, DEFENCE, ATTACK, BATTLE_IDLE, CAPTURE, DAM_B, DAM_F
		, DAM_H, DAM_HARD, DAM_L, DAM_R, DAM_SWO, DAM_UPPER_A, DAM_UPPER_B, DAM_UPPER_C, JUMP_A, JUMP_B, JUMP_C, ANI_END
	};
private:
	explicit CAI_Roaming(void);
	virtual ~CAI_Roaming(void);

public:
	virtual void Start_State(void);
	virtual void Start_State(_vec3* pDir, _float fForce);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	_vec3		m_vRoamPos[DIR_END];
	_float		m_fRoamingTime;
	_bool		m_bRoam;
	ROAM_DIR	m_eRoamDir;
	ROAM_DIR	m_eCompareRoamDir;
private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator, Engine::CNavigationMesh* pNaviMesh);
public:
	static CAI_Roaming* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator, Engine::CNavigationMesh* pNaviMesh);
private:
	virtual void Free(void);
};

#endif // Roaming_h__
