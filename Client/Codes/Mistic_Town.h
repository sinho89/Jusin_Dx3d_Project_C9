#ifndef Mistic_Town_h__
#define Mistic_Town_h__

#include "Mistic_State.h"

namespace Engine
{
	class CDynamicMesh;
}

class CMisticWeapon;

class CMistic_Town
	: public CMistic_State
{
private:
	explicit CMistic_Town(_uint iAniIndex);
	virtual ~CMistic_Town(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
private:
	_vec3	m_vLook;
	_bool	m_bCanMove;
	_bool	m_bCanJump;
private:
	HRESULT Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CMistic* pPlayer, CMisticWeapon* pWeapon);
private:
	void ActionCheck(void);
	void MoveCheck(const _float& fTimeDelta);
	void JumpCheck(const _float& fTimeDelta);
public:
	static CMistic_Town* Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CMistic* pPlayer, _uint iAniIndex, CMisticWeapon* pWeapon);
private:
	virtual void Free(void);
};


#endif // Mistic_Town_h__
