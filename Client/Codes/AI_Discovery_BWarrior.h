#pragma once
#ifndef AI_Discovery_BWarrior_h__
#define AI_Discovery_BWarrior_h__

#include "AI_Babegazi_Warrior.h"
#include "Defines.h"

//Å¬¶ó..

namespace Engine
{
	class CTransform;
	class CAnimater;
	class CNavigationMesh;
	class CStaticMesh;
	class CCalculator;
	class CRigidbody;
	class CCollider;
}

class CSound;

class CAI_Discovery_BWarrior
	: public CAI_Babegazi_Warrior
{
private:
	explicit CAI_Discovery_BWarrior(void);
	virtual ~CAI_Discovery_BWarrior(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	void		Collsion_Work(void);


private:
	_float m_fAggroRad;
	_bool  m_bAggro;

private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI, CSound* pSound);
public:
	static CAI_Discovery_BWarrior* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, 
		Engine::CAI* pAI, CSound* pSound);
private:
	virtual void Free(void);
};

#endif // AI_Trace_h__
