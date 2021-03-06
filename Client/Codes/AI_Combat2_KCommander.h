#ifndef AI_Combat2_KCommander_h__
#define AI_Combat2_KCommander_h__

#include "AI_Knole_Commander.h"
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

class CAI_Combat2_KCommander
	: public CAI_Knole_Commander
{

private:
	explicit CAI_Combat2_KCommander(void);
	virtual ~CAI_Combat2_KCommander(void);

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
	_float		m_fChangeTime;				//상태변화시간
	_bool		m_bChange;					//상태변화
	_bool       m_bLRMove;					//레프트 라이트 무브

private:
	_bool		m_bDamaged;


private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon,
		Engine::CAI* pAI);
public:
	static CAI_Combat2_KCommander* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon,
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Combat_h__
