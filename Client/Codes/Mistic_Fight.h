#ifndef Mistic_FIGHT__
#define Mistic_FIGHT__

#include "Mistic_State.h"

namespace Engine
{
	class CDynamicMesh;
}

class CMisticWeapon;

class CMistic_Fight
	: public CMistic_State
{
public:
	enum COLCNT { COL1, COL2, COL3, COL4, COL5, COL6, COL_END };
private:
	explicit CMistic_Fight(_uint iAniIndex);
	virtual ~CMistic_Fight(void);
public:
	void SetActive(void);
	void Set_Skill(const SKILL_ID eID);
	void Set_Move(const _bool& bMove);
public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
private:
	Engine::COLINFO m_tColInfo;
	_vec3	m_vLook;
	_bool	m_bCanMove;
	_bool	m_bCanJump;
	_float  m_fGravityTime;	//중력포 루프지속시간
	_float  m_fFireTime;	//화염방사기 루프지속시간
	_float  m_fSpinTime;	//스핀크러쉬 루프지속시간
	_float  m_fLockTime;	//락온타겟슛 루프지속시간
	_float  m_fGailTime;	//게일샷 지속시간
	_int	m_iSpinDelay;
	_int    m_iRisingDelay;
private:
	_bool m_bAddCol[COL_END];
private:
	HRESULT Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CMistic* pPlayer, CMisticWeapon* pWeapon);
private:
	void ActionCheck(void);
	void MoveCheck(const _float& fTimeDelta);
	void JumpCheck(const _float& fTimeDelta);
	void AttackCheck(const _float& fTimeDelta);
	void CommandSkillCheck(const _float& fTimeDelta);
	void ActionSkillCheck(const _float& fTimeDelta);
	void DemageCheck(const _float& fTimeDelta);
private:
	_int GetDamage(_bool* pCritical);
	void Shot_Bullet(Engine::COLINFO& tColInfo);
public:
	static CMistic_Fight* Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CMistic* pPlayer, _uint iAniIndex, CMisticWeapon* pWeapon);
private:
	virtual void Free(void);
};


#endif // !Mistic_FIGHT__
