#ifndef AI_Trace_KCommander_h__
#define AI_Trace_KCommander_h__

#include "AI_Knole_Commander.h"
#include "Defines.h"

//Ŭ��
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

class CAI_Trace_KCommander
	: public CAI_Knole_Commander
{
private:
	explicit CAI_Trace_KCommander(void);
	virtual ~CAI_Trace_KCommander(void);

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
	_bool		m_bCompareDis;					//�Ÿ� ����..

private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon,
		Engine::CAI* pAI);
public:
	static CAI_Trace_KCommander* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged, _bool* pWeapon,
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Trace_h__
