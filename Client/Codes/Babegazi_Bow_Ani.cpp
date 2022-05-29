#include "stdafx.h"
#include "Babegazi_Bow_Ani.h"

CBabegazi_Bow_Ani::CBabegazi_Bow_Ani(_uint iAniIndex)
: Engine::CAnimationState(iAniIndex)
{

}

CBabegazi_Bow_Ani::~CBabegazi_Bow_Ani(void)
{

}

void CBabegazi_Bow_Ani::Start_State(void)
{

}

void CBabegazi_Bow_Ani::Update_State(const _float& fTimeDelta)
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

void CBabegazi_Bow_Ani::End_State(void)
{

}

HRESULT CBabegazi_Bow_Ani::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion)
{
	if(pMesh == NULL)
		return E_FAIL;
	m_pMesh = pMesh;
	m_pMesh->AddRef();
	m_pmapNextMotion = pmapNextMotion;
	return S_OK;
}

CBabegazi_Bow_Ani* CBabegazi_Bow_Ani::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex)
{
	CBabegazi_Bow_Ani* pInstance = new CBabegazi_Bow_Ani(iAniIndex);

	if(FAILED(pInstance->Ready_State(pMesh, pmapNextMotion)))
	{
		MSG_BOX("CBabegazi_Bow_Ani Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBabegazi_Bow_Ani::Free(void)
{
	Engine::Safe_Release(m_pMesh);	
}

