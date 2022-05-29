#ifndef AI_Trace_BBow_h__
#define AI_Trace_BBow_h__

#include "AI_Babegazi_Bow.h"
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

class CAI_Trace_BBow
	: public CAI_Babegazi_Bow
{
private:
	explicit CAI_Trace_BBow(void);
	virtual ~CAI_Trace_BBow(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	void		TraceMove(const _float& fTimeDelta);
	_vec3		AStarMove(_vec3* pPos, const _float &fTimeDelta);

private:
	void		Collsion_Work(void);

private:
	list<_vec3> m_BestList;
	_ulong		m_dwIdx;
	_vec3		m_vAStarDir;

private:
	_float		m_fCompareTime;
	_bool		m_bCompareDis;					//거리 비교텀..

private:
	_bool		m_bDamaged;



private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI);
public:
	static CAI_Trace_BBow* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, 
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Trace_h__
