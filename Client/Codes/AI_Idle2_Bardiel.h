#ifndef AI_Idle2_Bardiel_h__
#define AI_Idle2_Bardiel_h__

#include "AI_Bardiel.h"
#include "Defines.h"

//Ŭ??
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

class CAI_Idle2_Bardiel
	: public CAI_Bardiel
{
private:
	explicit CAI_Idle2_Bardiel(void);
	virtual ~CAI_Idle2_Bardiel(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	void		Collsion_Work(void);
	void		TurnWork(const _float& fTimeDelta);

private:
	_bool		m_bOne;
	_bool		m_bIdleTime;
	_float      m_fIdleTime;



private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* _pDamaged,
		Engine::CAI* pAI);
public:
	static CAI_Idle2_Bardiel* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInf, _bool* pDamaged,
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Idle2_Bardiel_h__
