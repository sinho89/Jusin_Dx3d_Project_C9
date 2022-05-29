#ifndef AI_Death_KCommander_h__
#define AI_Death_KCommander_h__

#include "AI_Knole_Commander.h"
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

class CAI_Death_KCommander
	: public CAI_Knole_Commander
{
private:
	explicit CAI_Death_KCommander(void);
	virtual ~CAI_Death_KCommander(void);

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
		Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon,
		Engine::CAI* pAI);
public:
	static CAI_Death_KCommander* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon,
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Down_h__
