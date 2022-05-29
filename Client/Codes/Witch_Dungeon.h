#ifndef Witch_Move_h__
#define Witch_Move_h__

#include "Witch_State.h"

namespace Engine
{
	class CDynamicMesh;
}

class CWitchBladeWeapon;

class CWitch_Dungeon
	: public CWitch_State
{
private:
	explicit CWitch_Dungeon(_uint iAniIndex);
	virtual ~CWitch_Dungeon(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
private:
	Engine::COLINFO m_tColInfo;
	_vec3 m_vLook;
	_bool m_bCanMove;
	_bool m_bCanJump;
private:
	HRESULT Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CWitchBlade* pPlayer, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword);
private:
	void ActionCheck(void);
	void MoveCheck(const _float& fTimeDelta);
	void JumpCheck(const _float& fTimeDelta);
	void DamageCheck(const _float& fTimeDelta);
public:
	static CWitch_Dungeon* Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
		CWitchBlade* pPlayer, _uint iAniIndex, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword);
private:
	virtual void Free(void);
};

#endif // Witch_Move_h__
