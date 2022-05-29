#ifndef AI_Defence_BWarrior_h__
#define AI_Defence_BWarrior_h__

#include "AI_Babegazi_Warrior.h"
#include "Defines.h"

//클라
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

class CAI_Defence_BWarrior
	: public CAI_Babegazi_Warrior
{
private:
	explicit CAI_Defence_BWarrior(void);
	virtual ~CAI_Defence_BWarrior(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);


private:
	void		Collsion_Work(void);

private:
	_ulong		m_dwIdx;
	_uint		m_iDirection;


private:
	_float		m_fDefenceTime;
	_bool		m_bOne;
	_bool		m_bAttack;							//공격전환
	_bool       m_bDefence;							//방어중
	_bool		m_bAttackCol;						//공격콜라이더추가
private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI, CSound* pSound);
public:
	static CAI_Defence_BWarrior* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI, CSound* pSound);
private:
	virtual void Free(void);
};

#endif // AI_Defence_h__
