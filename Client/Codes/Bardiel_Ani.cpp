#include "stdafx.h"
#include "Bardiel_Ani.h"

CBardiel_Ani::CBardiel_Ani(_uint iAniIndex)
: Engine::CAnimationState(iAniIndex)
{

}

CBardiel_Ani::~CBardiel_Ani(void)
{

}

void CBardiel_Ani::Start_State(void)
{

}

void CBardiel_Ani::Update_State(const _float& fTimeDelta)
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

void CBardiel_Ani::End_State(void)
{

}

HRESULT CBardiel_Ani::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion)
{
	if(pMesh == NULL)
		return E_FAIL;
	m_pMesh = pMesh;
	m_pMesh->AddRef();
	m_pmapNextMotion = pmapNextMotion;
	return S_OK;
}

CBardiel_Ani* CBardiel_Ani::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex)
{
	CBardiel_Ani* pInstance = new CBardiel_Ani(iAniIndex);

	if(FAILED(pInstance->Ready_State(pMesh, pmapNextMotion)))
	{
		MSG_BOX("CBardiel_Ani Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBardiel_Ani::Free(void)
{
	Engine::Safe_Release(m_pMesh);	
}

