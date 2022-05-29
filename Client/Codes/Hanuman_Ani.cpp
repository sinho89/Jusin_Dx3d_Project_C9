#include "stdafx.h"
#include "Hanuman_Ani.h"

CHanuman_Ani::CHanuman_Ani(_uint iAniIndex)
: Engine::CAnimationState(iAniIndex)
{

}

CHanuman_Ani::~CHanuman_Ani(void)
{

}

void CHanuman_Ani::Start_State(void)
{

}

void CHanuman_Ani::Update_State(const _float& fTimeDelta)
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

void CHanuman_Ani::End_State(void)
{

}

HRESULT CHanuman_Ani::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion)
{
	if(pMesh == NULL)
		return E_FAIL;
	m_pMesh = pMesh;
	m_pMesh->AddRef();
	m_pmapNextMotion = pmapNextMotion;
	return S_OK;
}

CHanuman_Ani* CHanuman_Ani::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex)
{
	CHanuman_Ani* pInstance = new CHanuman_Ani(iAniIndex);

	if(FAILED(pInstance->Ready_State(pMesh, pmapNextMotion)))
	{
		MSG_BOX("CHanuman_Ani Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHanuman_Ani::Free(void)
{
	Engine::Safe_Release(m_pMesh);	
}

