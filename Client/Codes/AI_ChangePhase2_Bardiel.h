#ifndef AI_ChangePhase2_Bardiel_h__
#define AI_ChangePhase2_Bardiel_h__

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

class CAI_ChangePhase2_Bardiel
	: public CAI_Bardiel
{
private:
	explicit CAI_ChangePhase2_Bardiel(void);
	virtual ~CAI_ChangePhase2_Bardiel(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);

private:
	_bool		m_bStart;
private:
	HRESULT Ready_State(Engine::CTransform* pTransform, Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh
		, Engine::CAI* pAI
		, CSound* pSound);
public:
	static CAI_ChangePhase2_Bardiel* Create(Engine::CTransform* pTransform,
		Engine::CAnimator* pAnimator,
		Engine::CNavigationMesh* pNaviMesh,
		Engine::CAI* pAI
		, CSound* pSound);
private:
	virtual void Free(void);
};

#endif // Roaming_h__
