#include "stdafx.h"
#include "AI_CameraIntro_Bardiel.h"
#include "Export_Engine.h"
#include "GameObject.h"
#include "AI.h"
#include "PlayerCamera.h"
#include "Sound.h"
//클라

CAI_CameraIntro_Bardiel::CAI_CameraIntro_Bardiel(void)
	:CAI_Bardiel()
	, m_fRoamingTime(0.f)
	, m_fAggroRad(0.f)
	, m_fDelayTime(0.f)
	, m_bWakeUp(false)
	, m_bStart(false)
	, m_bSummon(false)
	, m_bFadeName(false)
{
}

CAI_CameraIntro_Bardiel::~CAI_CameraIntro_Bardiel(void)
{
}

void CAI_CameraIntro_Bardiel::Set_CameraStart(const _bool & bStart)
{
	m_bWakeUp = bStart;
}

void CAI_CameraIntro_Bardiel::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_vStartPos = m_pTransform->Get_Now_Position();
	for (int i = 0; i < DIR_END; ++i)
	{
		m_vRoamPos[i] = m_vStartPos;
	}
	m_vRoamPos[DIR_EAST].x = m_vStartPos.x - 5.f;
	m_vRoamPos[DIR_EAST].z = m_vStartPos.z - 5.f;

	m_vRoamPos[DIR_WEST].x = m_vStartPos.x + 5.f;
	m_vRoamPos[DIR_WEST].z = m_vStartPos.z - 5.f;

	m_vRoamPos[DIR_SOUTH].x = m_vStartPos.x - 5.f;
	m_vRoamPos[DIR_SOUTH].z = m_vStartPos.z + 5.f;

	m_vRoamPos[DIR_NORTH].x = m_vStartPos.x + 5.f;
	m_vRoamPos[DIR_NORTH].z = m_vStartPos.z + 5.f;

	m_eCompareRoamDir = DIR_END;
	m_bFadeName = false;
}


void CAI_CameraIntro_Bardiel::Update_State(const _float & fTimeDelta)
{
	if (m_pTransform == NULL || m_pAnimator == NULL)
		return;

	m_vTargetPos = m_pTargetTransform->Get_Now_Position();
	m_vPlayerPos = m_pTargetTransform->Get_Now_Position();
	m_vPos = m_pTransform->Get_Now_Position();
	m_vPivotPos = m_pTransform->Get_Now_Position();
	m_vPivotPos.y += 2.f;
	m_vRayDir = m_vTargetPos - m_vPivotPos;
	m_fPlayerDis = D3DXVec3Length(&m_vRayDir);


	if (!m_bWakeUp && !m_bStart)
	{
		m_pAnimator->Set_AnimationSet(IDLE_1);
	}

	if (m_bWakeUp && !m_bStart)
	{
		m_pAnimator->Set_AnimationSet(WAKEUP);
	}

	if (m_pAnimator->Get_AnimationIndex() == WAKEUP)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.05,
			(*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.15))
		{
			m_pSound->MyPlaySound(L"CameraIntro_EarthCrack");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.2,
			(*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.25))
		{
			m_pSound->MyPlaySound(L"CameraIntro_Voice_3");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.4,
			(*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.95))
		{
			m_pSound->MyPlaySound(L"CameraIntro_SpinCircle_2");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.9,
			(*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[WAKEUP]->tMotionInfo.dEndTime * 0.95))
		{
			m_pSound->MyPlaySound(L"CameraIntro_Voice_1");
		}
	}

	if (!m_bStart)
	{
		if (m_pAnimator->Is_AnimationEnd(WAKEUP, 0.1))
		{
			if (m_pAnimator->Set_AnimationSet(SUMMON))
			{
				m_bStart = true;
			}
		}
	}

	if (!m_bSummon)
	{
		if (m_pAnimator->Get_AnimationIndex() == SUMMON)
		{
			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.0
				, (*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.7))
			{
				m_pSound->MyPlaySound(L"CameraIntro_SpinCircle_2");
			}

			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.25
				, (*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.35))
			{
				m_pSound->MyPlaySound(L"CameraIntro_Ball");
			}

			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.55
				, (*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.65))
			{
				m_pSound->MyPlaySound(L"CameraIntro_Ball");
				m_pAnimator->Pause_Animation(false);
				m_bSummon = true;
			}
		}
	}

	if (m_bSummon)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.0
			, (*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.7))
		{
			m_pSound->MyPlaySound(L"CameraIntro_SpinCircle_2");
		}
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.75
			, (*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.8))
		{
			m_pSound->MyPlaySound(L"CameraIntro_Voice_2");
		}

		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.85
			, (*m_pvecAniInfo)[SUMMON]->tMotionInfo.dEndTime * 0.9))
		{
			m_pSound->MyPlaySound(L"CameraIntro_Weapon");
		}

		//여기서 바르디엘 이름 생성 이시간 늘리면 대기시간길어짐
		if (m_fDelayTime > 2.f)
		{
			m_pAnimator->Pause_Animation(true);
		}

		m_fDelayTime += fTimeDelta;
	}

	if (m_pAnimator->Is_AnimationEnd(SUMMON, 0.1))
	{
		if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE_1))
		{
			((CPlayerCamera*)Engine::GetCamera(Engine::CAMERA_PLAYER))->Set_ActionEnd();
			m_pAnimator->Set_State(MONSTER_FIGHT);
			m_pAI->Set_State(AI_IDLE_1);
		}
	}

	//if (!m_bRoam)
	//{
	//	m_fRoamingTime += fTimeDelta;
	//	m_pAnimator->Set_AnimationSet(IDLE_1);

	//	if (m_fRoamingTime > 4.f)
	//	{
	//		//srand(unsigned int(time(NULL)));

	//		//출발할 방향을 정한다....
	//		m_eRoamDir = ROAM_DIR(rand() % 4);

	//		//출발하기 전에 내가 왔던길이랑 비교...
	//		if (m_eRoamDir == m_eCompareRoamDir)
	//		{
	//			return;
	//		}
	//		//출발...
	//		else
	//		{
	//			m_bRoam = true;
	//			m_fRoamingTime = 0.f;
	//		}

	//	}
	//}

	//if (m_bRoam)// m_bRoam == true
	//{
	//	m_pAnimator->Set_AnimationSet(WALK);
	//	_float fLength = 0.f;

	//	_vec3 vDir;
	//	_vec3 vPos;
	//	m_pTransform->Get_Infomation(Engine::INFO_POSITION, &vPos);
	//	vDir = m_vRoamPos[m_eRoamDir] - vPos;
	//	fLength = D3DXVec3Length(&vDir);

	//	D3DXVec3Normalize(&vDir, &vDir);

	//	LookAtDir(&vDir, fTimeDelta);
	//	_vec3 vEndPos = vPos + vDir * fTimeDelta * 3.f;

	//	m_pTransform->Set_Position(&vEndPos);

	//	if (fLength < 2.f)
	//	{
	//		m_eCompareRoamDir = m_eRoamDir;
	//		m_bRoam = false;
	//		m_fRoamingTime = 0.f;
	//	}
	//}


}

void CAI_CameraIntro_Bardiel::End_State(void)
{

}

HRESULT CAI_CameraIntro_Bardiel::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator
	, Engine::CNavigationMesh* pNaviMesh, Engine::CAI* pAI, CSound* pSound)
{
	if (pTransform == NULL || pAnimator == NULL)
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

	m_fAggroRad = 40.0f;

	Start_State();

	return S_OK;
}

CAI_CameraIntro_Bardiel * CAI_CameraIntro_Bardiel::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CAI* pAI, CSound* pSound)
{
	CAI_CameraIntro_Bardiel* pInstance = new CAI_CameraIntro_Bardiel;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pAI, pSound)))
	{
		MSG_BOX("CAI_CameraIntro_Bardiel Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_CameraIntro_Bardiel::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
}
