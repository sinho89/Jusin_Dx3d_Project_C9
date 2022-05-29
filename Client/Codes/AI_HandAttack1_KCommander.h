#ifndef AI_HandAttack1_KCommander_h__
#define AI_HandAttack1_KCommander_h__

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

class CAI_HandAttack1_KCommander
	: public CAI_Knole_Commander
{
private:
	explicit CAI_HandAttack1_KCommander(void);
	virtual ~CAI_HandAttack1_KCommander(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	void		Collsion_Work(void);

private:
	_bool       m_bAttackCol;
	_bool		m_bAttackTime;
	_float		m_fAttackTime;


private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon,
		Engine::CAI* pAI, CSound* pSound);
public:
	static CAI_HandAttack1_KCommander* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon,
		Engine::CAI* pAI, CSound* pSound);
private:
	virtual void Free(void);
};

#endif // AI_Combat_h__
