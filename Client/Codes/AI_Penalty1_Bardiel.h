#ifndef AI_Penalty1_Bardiel_h__
#define AI_Penalty1_Bardiel_h__

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

class CAI_Penalty1_Bardiel
	: public CAI_Bardiel
{
private:
	explicit CAI_Penalty1_Bardiel(void);
	virtual ~CAI_Penalty1_Bardiel(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	void		Collsion_Work(void);

private:
	_bool		m_bAttackCol1;
	_bool		m_bAttackCol2;
	_bool		m_bAttackCol3;


private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI, CSound* pSound);
public:
	static CAI_Penalty1_Bardiel* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, 
		Engine::CAI* pAI, CSound* pSound);
private:
	virtual void Free(void);
};

#endif // AI_Trace_h__
