#ifndef Mistic_STATE__
#define Mistic_STATE__

#include "AnimationState.h"
#include "Defines.h"
#include "Mistic.h"

namespace Engine
{
	class CRigidbody;
	class CTransform;
	class CAnimator;
	class CCalculator;
	class CCollider;
}

class CMisticWeapon;

class CMistic_State
	: public Engine::CAnimationState
{
protected:
	explicit CMistic_State(_uint iAniIndex);
	virtual ~CMistic_State(void);
public:
	const _bool* Get_CurAction(void) const;
public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
protected:
	Engine::CRigidbody* m_pRigidbody;
	Engine::CTransform* m_pTransform;
	Engine::CAnimator* m_pAnimator;
	Engine::CCalculator* m_pCalculator;
	Engine::CCollider* m_pCollider;
	Engine::CCollider* m_pSphereCollider;
protected:
	CMisticWeapon*		m_pWeapon;			//왼손검
	CMistic*			m_pPlayer;				//플레이어
	PLAYER_INFO*	 	m_pPlayerInfo;		//플레이어 정보
protected:
	_bool			   m_bAction[CMistic::ACTION_END];
protected:
	void EndCheck(void);
protected:
	virtual void Free(void);
};

#endif // !Mistic_STATE__
