#ifndef AI_Roar_Hanuman_h__
#define AI_Roar_Hanuman_h__

#include "AI_Hanuman.h"
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
	class CAI;
}

class CAI_Roar_Hanuman
	: public CAI_Hanuman
{
private:
	explicit CAI_Roar_Hanuman(void);
	virtual ~CAI_Roar_Hanuman(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	void		Collsion_Work(void);

private:
	_float		m_fCompareTime;
	_float		m_fIdleTime;
	_bool		m_bIdle;
	_bool		m_bCompareDis;					//거리 비교텀..
	_int		m_iRoarCount;
	_bool		m_bAttackCol;

private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI, CSound* pSound);
public:
	static CAI_Roar_Hanuman* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI, CSound* pSound);
private:
	virtual void Free(void);
};

#endif // AI_Trace_h__
