#ifndef AI_CameraIntro_Bardiel_h__
#define AI_CameraIntro_Bardiel_h__

#include "AI_Bardiel.h"
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

class CAI_CameraIntro_Bardiel
	: public CAI_Bardiel
{
private:
	explicit CAI_CameraIntro_Bardiel(void);
	virtual ~CAI_CameraIntro_Bardiel(void);
public:
	void Set_CameraStart(const _bool& bStart);
public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

public:
	_bool*	GetFadeName(void) { return &m_bFadeName; }
private:
	_vec3		m_vRoamPos[DIR_END];
	_float		m_fRoamingTime;
	_bool		m_bWakeUp;
	_bool		m_bStart;
	ROAM_DIR	m_eRoamDir;
	ROAM_DIR	m_eCompareRoamDir;
	_float		m_fAggroRad;

	_bool		m_bSummon;
	_bool		m_bFadeName;

	_float		m_fDelayTime;
private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh
		, Engine::CAI* pAI
		, CSound*	pSound);
public:
	static CAI_CameraIntro_Bardiel* Create(Engine::CTransform* pTransform,
		Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh,
		Engine::CAI* pAI
		, CSound* pSound);
private:
	virtual void Free(void);
};

#endif // Roaming_h__
