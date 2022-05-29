#ifndef AnimationCtrl_h__
#define AnimationCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimationCtrl 
	: public CBase
{
private:
	explicit CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAnimationCtrl(const CAnimationCtrl& rhs);
	virtual ~CAnimationCtrl(void);
public:
	LPD3DXANIMATIONCONTROLLER Get_AnimationCtrl(void);
	_uint Get_MaxAnimationSet(void);
	const char* Get_AnimationName(const _uint& iIndex);
	_bool is_AnimationSetEnd(const _double dMagicNumber = 0.1);
	_double Get_TrackPos(void);
	_double Get_Period(void);
	_double Get_Period(const _uint& iIndex);
	_bool IsPlaying_ThisSection(const _double& dStartTime, const _double& dEndTime);
public:
	void Set_TrackPos(const _double& dPosition);
	void Set_TrackSpeed(const _float& fSpeed);
	void Set_AniInfo(const MOTIONINFO* pMotionInfo);
	void Pause_Animation(const _bool& bIsPlay = false);
public:
	HRESULT Ready_AnimationCtrl(void);
	_bool Set_AnimationSet(const _uint& iIdx, const MOTIONINFO* pMotionInfo, _double dMagicNumber);
	void Set_AnimationSet(const _uint& iIdx);
	void Play_Animation(const _float& fTimeDelta);

private:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;
	const MOTIONINFO*				m_pMotionInfo;
	_uint							m_iCurrentTrack;
	_uint							m_iNewTrack;
	_float							m_fAccTime;
	_uint							m_iOldAniIdx;
	_uint							m_iMaxAniSet;
	_double							m_dPeriod;
	_bool							m_bIsPlaying;
public:
	static CAnimationCtrl* Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAnimationCtrl* Create(const CAnimationCtrl& rhs);
	virtual void Free(void);
};

END

#endif // AnimationCtrl_h__
