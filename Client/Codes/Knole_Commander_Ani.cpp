#include "stdafx.h"
#include "Knole_Commander_Ani.h"

CKnole_Commander_Ani::CKnole_Commander_Ani(_uint iAniIndex)
: Engine::CAnimationState(iAniIndex)
{

}

CKnole_Commander_Ani::~CKnole_Commander_Ani(void)
{

}

void CKnole_Commander_Ani::Start_State(void)
{

}

void CKnole_Commander_Ani::Update_State(const _float& fTimeDelta)
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

void CKnole_Commander_Ani::End_State(void)
{

}

HRESULT CKnole_Commander_Ani::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion)
{
	if(pMesh == NULL)
		return E_FAIL;
	m_pMesh = pMesh;
	m_pMesh->AddRef();
	m_pmapNextMotion = pmapNextMotion;
	return S_OK;
}

CKnole_Commander_Ani* CKnole_Commander_Ani::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion, _uint iAniIndex)
{
	CKnole_Commander_Ani* pInstance = new CKnole_Commander_Ani(iAniIndex);

	if(FAILED(pInstance->Ready_State(pMesh, pmapNextMotion)))
	{
		MSG_BOX("CKnole_Commander_Ani Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CKnole_Commander_Ani::Free(void)
{
	Engine::Safe_Release(m_pMesh);	
}

