#ifndef AnimationState_h__
#define AnimationState_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CDynamicMesh;

class ENGINE_DLL CAnimationState
	: public CBase
{
protected:
	explicit CAnimationState(_uint iAniIndex);
	virtual ~CAnimationState(void);
public:
	_bool IsPlaying_ThisSection(const _double& dStartTime, const _double& dEndTime);
	_bool Is_AnimationEnd(const _uint& iIndex, const _double dMagicNumber = 0.0);
	_uint Get_AnimationIndex(void);
public:
	void Pause_Animation(const _bool& bIsPlay = false);
	_bool Set_AnimationSet(const _uint& iState, const _uint& iIndex);
	void Set_TrackSpeed(const _float& fSpeed);
	void Set_Animator(vector<ANIINFO*>* pAniInfo);
public:
	virtual void Start_State(void)PURE;
	virtual void Update_State(const _float& fTimeDelta)PURE;
	virtual void End_State(void)PURE;
protected:
	CDynamicMesh* m_pMesh;
protected:
	map<_uint, list<ANIMATORINFO*>>* m_pmapNextMotion;
	typedef map<_uint, list<ANIMATORINFO*>> MAPNANIMATOR;
	typedef list<ANIMATORINFO*> ANIMATORLIST;
	vector<ANIINFO*>* m_pAniInfo;
	_uint m_iAniIndex;
	_bool m_bFirstSetting;

protected:
	void End_Animation(void);

protected:
	virtual void Free(void)PURE;
};

END

#endif // AnimationState_h__