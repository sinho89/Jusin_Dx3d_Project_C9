#ifndef AI_Combat_BAxe_h__
#define AI_Combat_BAxe_h__

#include "AI_Babegazi_Axe.h"
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
}

class CAI_Combat_BAxe
	: public CAI_Babegazi_Axe
{

private:
	explicit CAI_Combat_BAxe(void);
	virtual ~CAI_Combat_BAxe(void);

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
	_float		m_fChangeTime;				//���º�ȭ�ð�
	_bool		m_bChange;					//���º�ȭ
	_bool       m_bLRMove;					//����Ʈ ����Ʈ ����


private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI);
public:
	static CAI_Combat_BAxe* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // AI_Combat_h__
