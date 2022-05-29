#ifndef Bardiel_Ani_h__
#define Bardiel_Ani_h__

#include "AnimationState.h"
#include "Defines.h"

namespace Engine
{
	class CDynamicMesh;
}

class CBardiel_Ani
	: public Engine::CAnimationState
{
private:
	explicit CBardiel_Ani(_uint iAniIndex);
	virtual ~CBardiel_Ani(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
private:
	HRESULT Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion);
public:
	static CBardiel_Ani* Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex);
private:
	virtual void Free(void);
};

#endif // Bardiel_Ani_h__
