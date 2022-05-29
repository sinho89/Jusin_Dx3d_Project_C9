#ifndef AI_Down_BAxe_h__
#define AI_Down_BAxe_h__

#include "AI_Babegazi_Axe.h"
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

class CAI_Down_BAxe
	: public CAI_Babegazi_Axe
{
private:
	explicit CAI_Down_BAxe(void);
	virtual ~CAI_Down_BAxe(void);

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
	static CAI_Down_BAxe* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInf, _bool* pDamaged,
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Down_h__
