#ifndef Player_Ani_h__
#define Player_Ani_h__

#include "AnimationState.h"
#include "Defines.h"

namespace Engine
{
	class CDynamicMesh;
}

class CPlayer_Ani
	: public Engine::CAnimationState
{
private:
	explicit CPlayer_Ani(_uint iAniIndex);
	virtual ~CPlayer_Ani(void);

public:
	virtual void Start_State(void);
	virtual void Update_State(const _float& fTimeDelta);
	virtual void End_State(void);
private:
	HRESULT Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion);
public:
	static CPlayer_Ani* Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex);
private:
	virtual void Free(void);
};



#endif // Player_Ani_h__
