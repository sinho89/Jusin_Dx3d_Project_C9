#ifndef AI_Roaming_BAxe_h__
#define AI_Roaming_BAxe_h__

#include "AI_Babegazi_Axe.h"
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

class CAI_Roaming_BAxe
	: public CAI_Babegazi_Axe
{
private:
	explicit CAI_Roaming_BAxe(void);
	virtual ~CAI_Roaming_BAxe(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	_vec3		m_vRoamPos[DIR_END];
	_float		m_fRoamingTime;
	_bool		m_bRoam;
	ROAM_DIR	m_eRoamDir;
	ROAM_DIR	m_eCompareRoamDir;
	_float		m_fAggroRad;
private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh
		, Engine::CAI* pAI);
public:
	static CAI_Roaming_BAxe* Create(Engine::CTransform* pTransform,
		Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh,
		Engine::CAI* pAI);
private:
	virtual void Free(void);
};

#endif // Roaming_h__
