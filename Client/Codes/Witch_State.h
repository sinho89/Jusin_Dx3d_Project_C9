#ifndef WITCH_STATE__
#define WITCH_STATE__

#include "AnimationState.h"
#include "Defines.h"
#include "WitchBlade.h"

namespace Engine
{
	class CRigidbody;
	class CTransform;
	class CAnimator;
	class CCalculator;
	class CCollider;
}

class CSound;

class CWitchBladeWeapon;

class CWitch_State
	: public Engine::CAnimationState
{
protected:
	explicit CWitch_State(_uint iAniIndex);
	virtual ~CWitch_State(void);
public:
	const _bool* Get_CurAction(void) const;
public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
public:
	const _bool		GetAction(CWitchBlade::WITCHBLADEACTION eWBAction) { return m_bAction[eWBAction]; }
protected:
	Engine::CRigidbody* m_pRigidbody;
	Engine::CTransform* m_pTransform;
	Engine::CAnimator* m_pAnimator;
	Engine::CCalculator* m_pCalculator;
	Engine::CCollider* m_pCollider;
	Engine::CCollider* m_pSphereCollider;
	CSound*	m_pSound;
protected:
	CWitchBladeWeapon* m_pLeftSword;		//�޼հ�
	CWitchBladeWeapon* m_pRightSword;		//�����հ�
	CWitchBlade*	   m_pPlayer;			//�÷��̾�
	PLAYER_INFO*	   m_pPlayerInfo;		//�÷��̾� ����
protected:
	_bool			   m_bAction[CWitchBlade::ACTION_END];
protected:
	void EndCheck(void);
protected:
	virtual void Free(void);
};

#endif // !WITCH_STATE__
