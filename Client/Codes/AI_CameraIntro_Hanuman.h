#ifndef AI_CameraIntro_Hanuman_h__
#define AI_CameraIntro_Hanuman_h__

#include "AI_Hanuman.h"
#include "Defines.h"

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

class CAI_CameraIntro_Hanuman
	: public CAI_Hanuman
{
private:
	explicit CAI_CameraIntro_Hanuman(void);
	virtual ~CAI_CameraIntro_Hanuman(void);

public:
	void Set_CameraStart(const _bool& bStart);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

public:
	void		SetCameraStart();

private:
	_float		m_fIdleTime;
	_bool		m_bCameraStart;
	_bool		m_bJump;
	_bool		m_bLanding;
	_bool		m_bAngry;
	_bool		m_bCrazyFoot;
	_bool		m_bRoar;
	_float		m_fAggroRad;
	_bool		m_bCombat;
	_float		m_fCombatTime;
private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI, CSound* pSound);
public:
	static CAI_CameraIntro_Hanuman* Create(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator,
		Engine::CRigidbody* pRigidbody, Engine::CCollider* pCollider,
		Engine::COLINFO* pColInfo, _bool* pDamaged,
		Engine::CAI* pAI, CSound* pSound);
private:
	virtual void Free(void);
};

#endif // Roaming_h__
