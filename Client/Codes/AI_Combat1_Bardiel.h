#ifndef AI_Combat1_Bardiel_h__
#define AI_Combat1_Bardiel_h__

#include "AI_Bardiel.h"
#include "Defines.h"

//Å¬¶ó
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

class CAI_Combat1_Bardiel
	: public CAI_Bardiel
{
private:
	explicit CAI_Combat1_Bardiel(void);
	virtual ~CAI_Combat1_Bardiel(void);

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

private:
	_bool		m_bLRMove;
	_bool		m_bChange;
	_float		m_fChangeTime;
	_int		m_iDirection;
	_int		m_iPatternCount;

private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI);
public:
	static CAI_Combat1_Bardiel* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, 
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Trace_h__
