#include "stdafx.h"
#include "SnowBall_Ani.h"

CSnowBall_Ani::CSnowBall_Ani(_uint iAniIndex)
: Engine::CAnimationState(iAniIndex)
{

}

CSnowBall_Ani::~CSnowBall_Ani(void)
{

}

void CSnowBall_Ani::Start_State(void)
{

}

void CSnowBall_Ani::Update_State(const _float& fTimeDelta)
{
	if (m_pMesh == NULL)
		return;

	_double dPosition = m_pMesh->Get_TrackPos();

	if (!(*m_pAniInfo)[m_iAniIndex]->tMotionInfo.bIsLoop)
	{
		if (dPosition >= (*m_pAniInfo)[m_iAniIndex]->tMotionInfo.dEndTime)
			CAnimationState::End_Animation();
	}
}

void CSnowBall_Ani::End_State(void)
{

}

HRESULT CSnowBall_Ani::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion)
{
	if(pMesh == NULL)
		return E_FAIL;
	m_pMesh = pMesh;
	m_pMesh->AddRef();
	m_pmapNextMotion = pmapNextMotion;
	return S_OK;
}

CSnowBall_Ani* CSnowBall_Ani::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex)
{
	CSnowBall_Ani* pInstance = new CSnowBall_Ani(iAniIndex);

	if(FAILED(pInstance->Ready_State(pMesh, pmapNextMotion)))
	{
		MSG_BOX("CSnowBall_Ani Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnowBall_Ani::Free(void)
{
	Engine::Safe_Release(m_pMesh);	
}

