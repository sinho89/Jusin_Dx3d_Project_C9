#include "stdafx.h"
#include "Player_Ani.h"

CPlayer_Ani::CPlayer_Ani(_uint iAniIndex)
: Engine::CAnimationState(iAniIndex)
{

}

CPlayer_Ani::~CPlayer_Ani(void)
{

}

void CPlayer_Ani::Start_State(void)
{

}

void CPlayer_Ani::Update_State(const _float& fTimeDelta)
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

void CPlayer_Ani::End_State(void)
{

}

HRESULT CPlayer_Ani::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion)
{
	if(pMesh == NULL)
		return E_FAIL;
	m_pMesh = pMesh;
	m_pMesh->AddRef();
	m_pmapNextMotion = pmapNextMotion;
	return S_OK;
}

CPlayer_Ani* CPlayer_Ani::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex)
{
	CPlayer_Ani* pInstance = new CPlayer_Ani(iAniIndex);

	if(FAILED(pInstance->Ready_State(pMesh, pmapNextMotion)))
	{
		MSG_BOX("CPlayer_Ani Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Ani::Free(void)
{
	Engine::Safe_Release(m_pMesh);	
}

