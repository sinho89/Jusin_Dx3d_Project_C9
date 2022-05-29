#ifndef AI_Death_Hanuman_h__
#define AI_Death_Hanuman_h__

#include "AI_Hanuman.h"
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
}

class CAI_Death_Hanuman
	: public CAI_Hanuman
{
private:
	explicit CAI_Death_Hanuman(void);
	virtual ~CAI_Death_Hanuman(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	void		Collsion_Work(void);

private:
	_uint		m_iDirection;


private:
	_bool		m_bOne;
	_float		m_fDownTime;
	_bool		m_bStand;
	_bool		m_bDown;



private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* _pDamaged,
		Engine::CAI* pAI);
public:
	static CAI_Death_Hanuman* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInf, _bool* pDamaged,
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Down_h__
