#include "AnimationCtrl.h"

USING(Engine)

Engine::CAnimationCtrl::CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
: m_pAniCtrl(pAniCtrl)
, m_iCurrentTrack(0)
, m_iNewTrack(1)
, m_fAccTime(0.f)
, m_iOldAniIdx(99)
, m_iMaxAniSet(0)
, m_dPeriod(0.0)
, m_pMotionInfo(NULL)
, m_bIsPlaying(true)
{
	m_pAniCtrl->AddRef();
}

CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl& rhs)
: m_iCurrentTrack(rhs.m_iCurrentTrack)
, m_iNewTrack(rhs.m_iNewTrack)
, m_fAccTime(rhs.m_fAccTime)
, m_iOldAniIdx(rhs.m_iOldAniIdx)
, m_iMaxAniSet(rhs.m_iMaxAniSet)
, m_dPeriod(0.0)
, m_pMotionInfo(NULL)
, m_bIsPlaying(true)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs()
		, rhs.m_pAniCtrl->GetMaxNumAnimationSets(), rhs.m_pAniCtrl->GetMaxNumTracks(), rhs.m_pAniCtrl->GetMaxNumEvents()
		, &m_pAniCtrl);
}

Engine::CAnimationCtrl::~CAnimationCtrl(void)
{

}


LPD3DXANIMATIONCONTROLLER Engine::CAnimationCtrl::Get_AnimationCtrl(void)
{
	return m_pAniCtrl;
}

_uint Engine::CAnimationCtrl::Get_MaxAnimationSet(void)
{
	return m_iMaxAniSet;
}

const char* Engine::CAnimationCtrl::Get_AnimationName(const _uint& iIndex)
{
	if(iIndex > m_iMaxAniSet)
		return NULL;

	LPD3DXANIMATIONSET		pAS = NULL;

	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	if(pAS == NULL)
		return NULL;

	return pAS->GetName();
}

_bool Engine::CAnimationCtrl::is_AnimationSetEnd(const _double dMagicNumber)
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	if(m_pMotionInfo == NULL)
	{
		if(TrackInfo.Position >= m_dPeriod - 0.1)
			return true;
	}
	else
	{
		if(TrackInfo.Position >= m_pMotionInfo->dEndTime - dMagicNumber)
			return true;
	}
	
	return false;
}


_double Engine::CAnimationCtrl::Get_TrackPos(void)
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	return TrackInfo.Position;
}

_double Engine::CAnimationCtrl::Get_Period(void)
{
	return m_dPeriod;
}

_double Engine::CAnimationCtrl::Get_Period(const _uint& iIndex)
{
	LPD3DXANIMATIONSET		pAS = NULL;

	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);
	
	return pAS->GetPeriod();
}

_bool CAnimationCtrl::IsPlaying_ThisSection(const _double& dStartTime, const _double& dEndTime)
{
	_double dPosition = Get_TrackPos();

	if ((dPosition >= dStartTime) && (dPosition < dEndTime))
		return true;

	return false;
}

void Engine::CAnimationCtrl::Set_TrackPos(const _double& dPosition)
{
	m_pAniCtrl->SetTrackPosition(m_iCurrentTrack, dPosition);
}

void Engine::CAnimationCtrl::Set_TrackSpeed(const _float& fSpeed)
{
	m_pAniCtrl->SetTrackSpeed(m_iCurrentTrack, fSpeed);
}

void Engine::CAnimationCtrl::Set_AniInfo(const MOTIONINFO* pMotionInfo)
{
	m_pMotionInfo = pMotionInfo;

	Set_TrackSpeed(m_pMotionInfo->fSpeed);
}

void CAnimationCtrl::Pause_Animation(const _bool & bIsPlay)
{
	m_bIsPlaying = bIsPlay;
}

HRESULT Engine::CAnimationCtrl::Ready_AnimationCtrl(void)
{
	m_iMaxAniSet = m_pAniCtrl->GetMaxNumAnimationSets();
	return S_OK;
}

_bool CAnimationCtrl::Set_AnimationSet(const _uint& iIdx, const MOTIONINFO* pMotionInfo, _double dMagicNumber)
{
	if(m_iOldAniIdx == iIdx)
		return false;

	if(pMotionInfo == NULL)
		return false;

	m_pMotionInfo = pMotionInfo;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;

	// 애니메이션 셋의 정보를 보관하는 객체.
	LPD3DXANIMATIONSET		pAS = NULL;

	m_pAniCtrl->GetAnimationSet(iIdx, &pAS);

	// 애니메이션 셋의 전체 재생 시간
	m_dPeriod = pAS->GetPeriod();

	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + dMagicNumber);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, dMagicNumber, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, dMagicNumber, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, m_pMotionInfo->fSpeed, m_fAccTime, dMagicNumber, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, dMagicNumber, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime();
	m_fAccTime = 0.f;
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, m_pMotionInfo->dStartTime);
	m_iOldAniIdx = iIdx;

	m_iCurrentTrack = m_iNewTrack;
	return true;
}

void Engine::CAnimationCtrl::Set_AnimationSet(const _uint& iIdx)
{
	if(m_iOldAniIdx == iIdx)
		return;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;

	// 애니메이션 셋의 정보를 보관하는 객체.
	LPD3DXANIMATIONSET		pAS = NULL;

	m_pAniCtrl->GetAnimationSet(iIdx, &pAS);

	// 애니메이션 셋의 전체 재생 시간
	m_dPeriod = pAS->GetPeriod();

	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime();
	m_fAccTime = 0.f;
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	m_iOldAniIdx = iIdx;

	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAnimationCtrl::Play_Animation(const _float& fTimeDelta)
{
	if (!m_bIsPlaying)
		return;

	_double dPosition = Get_TrackPos();
	
	if(m_pMotionInfo != NULL)
	{
		if(dPosition >= m_pMotionInfo->dEndTime)
			Set_TrackPos(m_pMotionInfo->dStartTime);
	}
	else
	{
		_double dPeriod = Get_Period();

		if(dPosition >= dPeriod)
			Set_TrackPos(0.0);
	}
	
	// 애니메이션을 재생한다.
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);

	m_fAccTime += fTimeDelta;
}

CAnimationCtrl* Engine::CAnimationCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAnimationCtrl*			pInstance = new CAnimationCtrl(pAniCtrl);

	if(FAILED(pInstance->Ready_AnimationCtrl()))
	{
		MSG_BOX("CAnimationCtrl Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;

}

CAnimationCtrl* Engine::CAnimationCtrl::Create(const CAnimationCtrl& rhs)
{
	CAnimationCtrl*			pInstance = new CAnimationCtrl(rhs);

	if(FAILED(pInstance->Ready_AnimationCtrl()))
	{
		MSG_BOX("CAnimationCtrl Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;

}

void Engine::CAnimationCtrl::Free(void)
{
	Engine::Safe_Release(m_pAniCtrl);

}
