#ifndef Animator_h__
#define Animator_h__

#include "Component.h"

BEGIN(Engine)

class CAnimationState;

class ENGINE_DLL CAnimator
	: public Engine::CComponent
{
private:
	explicit CAnimator(void);
	virtual ~CAnimator(void);
public:
	_bool Is_AnimationEnd(const _uint& iIndex, const _double dMagicNumber = 0.0);
	_uint Get_AnimationIndex(void);
	_bool IsPlaying_ThisSection(const _double& dStartTime, const _double& dEndTime);
	void Pause_Animation(const _bool& bIsPlay = false);
	const CAnimationState* Get_CurState(void);
	const CAnimationState* Get_State(const _uint& iState);
	_uint Get_State(void);
	void Reset_State(void);
public:
	_bool Set_AnimationSet(const _uint& iIndex);
	void Set_AnimationSet(const _uint& iState, const _uint iIndex);
	void Set_TrackSpeed(const _float& fSpeed);
	void Set_Animator(vector<ANIINFO*>* pAniInfo);
	void Set_State(const _uint& iIndex, CAnimationState* pAnimationState);
	void Set_State(const _uint& iIndex);
public:
	virtual void Update_Component(const _float& fTimeDelta);
private:
	map<_uint, CAnimationState*> m_mapState;
	typedef map<_uint, CAnimationState*> MAPSTATE;

	_uint m_iState;
private:
	HRESULT Ready_Animator(void);

public:
	static CAnimator* Create(void);
private:
	virtual void Free(void);
};

END

#endif // Animator_h__
