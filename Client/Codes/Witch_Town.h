#ifndef Witch_Town_h__
#define Witch_Town_h__

#include "Witch_State.h"

namespace Engine
{
	class CDynamicMesh;
}

class CWitchBladeWeapon;

class CWitch_Town
	: public CWitch_State
{
private:
	explicit CWitch_Town(_uint iAniIndex);
	virtual ~CWitch_Town(void);

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
		CWitchBlade* pPlayer, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword);
private:
	void ActionCheck(void);
	void MoveCheck(const _float& fTimeDelta);
	void JumpCheck(const _float& fTimeDelta);
public:
	static CWitch_Town* Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CWitchBlade* pPlayer, _uint iAniIndex, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword);
private:
	virtual void Free(void);
};


#endif // Witch_Town_h__
