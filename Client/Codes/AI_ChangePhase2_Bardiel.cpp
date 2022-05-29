#include "stdafx.h"
#include "AI_ChangePhase2_Bardiel.h"
#include "Export_Engine.h"
#include "GameObject.h"
#include "AI.h"
#include "Sound.h"
//Å¬¶ó

CAI_ChangePhase2_Bardiel::CAI_ChangePhase2_Bardiel(void)
	:CAI_Bardiel()
	,m_bStart(false)
{
}

CAI_ChangePhase2_Bardiel::~CAI_ChangePhase2_Bardiel(void)
{
}

void CAI_ChangePhase2_Bardiel::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);

}


void CAI_ChangePhase2_Bardiel::Update_State(const _float & fTimeDelta)
{
	if (m_pTransform == NULL || m_pAnimator == NULL)
		return;

	Set_Ray();
	
	if (m_pAnimator->Get_AnimationIndex() == CHANGEPHASE2)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"Bardiel_ChangePhase2_Voice");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.2,
			(*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.25))
		{
			m_pSound->MyPlaySound(L"Bardiel_ChangePhase2_Burf");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.55,
			(*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.6))
		{
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.6f, 4);
			m_pSound->MyPlaySound(L"Bardiel_ChangePhase2_Stamp");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.7,
			(*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.75))
		{
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.6f, 4);
			m_pSound->MyPlaySound(L"Bardiel_ChangePhase2_Weapon");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.8,
			(*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CHANGEPHASE2]->tMotionInfo.dEndTime * 0.85))
		{
			m_pSound->MyPlaySound(L"Bardiel_Swing_2");
		}

		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE_2))
		{
			m_pAI->Set_State(AI_IDLE_2);
		}
	}

}

void CAI_ChangePhase2_Bardiel::End_State(void)
{

}

HRESULT CAI_ChangePhase2_Bardiel::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator
	, Engine::CNavigationMesh* pNaviMesh, Engine::CAI* pAI, CSound* pSound)
{
	if (pTransform == NULL || pAnimator == NULL )
		return E_FAIL;

	m_pTransform = pTransform;
	m_pTransform->AddRef();

	m_pAnimator = pAnimator;
	m_pAnimator->AddRef();

	m_pAI = pAI;

	m_pSound = pSound;

	m_pNavigationMesh = pNaviMesh;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"Player");
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	Engine::Safe_Release(pGameObject);

	Start_State();

	return S_OK;
}

CAI_ChangePhase2_Bardiel * CAI_ChangePhase2_Bardiel::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CAI* pAI, CSound* pSound)
{
	CAI_ChangePhase2_Bardiel* pInstance = new CAI_ChangePhase2_Bardiel;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pAI, pSound)))
	{
		MSG_BOX("CAI_ChangePhase2_Bardiel Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_ChangePhase2_Bardiel::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
}
