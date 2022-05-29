#ifndef Knole_Commander_Ani_h__
#define Knole_Commander_Ani_h__

#include "AnimationState.h"
#include "Defines.h"

namespace Engine
{
	class CDynamicMesh;
}

class CKnole_Commander_Ani
	: public Engine::CAnimationState
{
private:
	explicit CKnole_Commander_Ani(_uint iAniIndex);
	virtual ~CKnole_Commander_Ani(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
private:
	HRESULT Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion);
public:
	static CKnole_Commander_Ani* Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex);
private:
	virtual void Free(void);
};

#endif // Knole_Commander_Ani_h__
