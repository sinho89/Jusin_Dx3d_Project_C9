#include "stdafx.h"
#include "AI_CameraIntro_Hanuman.h"
#include "Export_Engine.h"
#include "GameObject.h"
#include "AI.h"
#include "Sound.h"

#include "PlayerCamera.h"
//Å¬¶ó

CAI_CameraIntro_Hanuman::CAI_CameraIntro_Hanuman(void)
	:CAI_Hanuman()
	,m_fIdleTime(0.f)
	,m_fAggroRad(0.f)
	,m_fCombatTime(0.f)
	,m_bJump(false)
	,m_bCameraStart(false)
	,m_bLanding(false)
	,m_bAngry(false)
	,m_bRoar(false)
	,m_bCrazyFoot(false)
	,m_bCombat(false)
{
}

CAI_CameraIntro_Hanuman::~CAI_CameraIntro_Hanuman(void)
{
}

void CAI_CameraIntro_Hanuman::Set_CameraStart(const _bool & bStart)
{
	m_bCameraStart = bStart;
}

void CAI_CameraIntro_Hanuman::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_vStartPos = m_pTransform->Get_Now_Position();
	m_bJump = false;
	m_bCameraStart = false;
	m_pAnimator->Set_AnimationSet(WALK);
}


void CAI_CameraIntro_Hanuman::Update_State(const _float & fTimeDelta)
{
	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	Set_Ray();

	if (!m_bCameraStart)
	{
		m_pAnimator->Set_AnimationSet(STAND_IDLE);
	}

	if (m_bCameraStart)
	{
		if (!m_bJump)
		{
			m_pAnimator->Set_AnimationSet(STAND_IDLE);
			m_fIdleTime += fTimeDelta;

			if (m_fIdleTime > 5.f)
			{
				if (m_pAnimator->Set_AnimationSet(JUMP_A))
				{
					m_pCalculator->Set_Jump(true);
					m_pRigidbody->Set_Jump(0.7f);
					m_bJump = true;
				}
			}
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == JUMP_A)
	{
		m_vPos = m_vPos + m_vLook * fTimeDelta * m_fMonsterSpeed;
		m_pTransform->Set_Position(&m_vPos);
		if (!m_pCalculator->Get_IsCol())
		{
			m_pAnimator->Set_AnimationSet(JUMP_B);
		}
		else
		{
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == JUMP_B)
	{
		//if (m_pCalculator->Get_IsCol())
		if(m_vPos.y < 155.f)
		{
			m_pAnimator->Set_AnimationSet(JUMP_C);
		}

		m_vPos = m_vPos + m_vLook * fTimeDelta * m_fMonsterSpeed;
		m_pTransform->Set_Position(&m_vPos);
	}

	if (m_pAnimator->Get_AnimationIndex() == JUMP_C)
	{
		if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[JUMP_C]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[JUMP_C]->tMotionInfo.dEndTime * 0.0,
			(*m_pvecAniInfo)[JUMP_C]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[JUMP_C]->tMotionInfo.dEndTime * 0.4))
		{
			m_pSound->MyPlaySound(L"Floor_Hit_2");
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 6);
		}
		if (m_pAnimator->Set_AnimationSet(STAND_IDLE))
		{
			m_bLanding = true;
		}
	}

	if (m_bLanding && !m_bAngry)
	{
		_vec3 vLook;
		m_pTransform->Get_Infomation(Engine::INFO_LOOK, &vLook);

		//float fAngle = acosf(D3DXVec3Dot(&m_vRayDir, &vLook));

		_vec3 vUp = _vec3(0.f, 1.f, 0.f);
		_vec3 vRight;

		D3DXVec3Cross(&vRight, &vUp, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		float fCos = D3DXVec3Dot(&vRight, &m_vRayDir);
		float fRealCos = D3DXVec3Dot(&vLook, &m_vRayDir);
		float fAngle = acosf(fRealCos);

		fAngle = D3DXToDegree(fAngle);

		if (fAngle > 180.f)
			fAngle = 360.f - fAngle;

		if (fCos > 0.2f)
		{
			m_pTransform->Rotation(Engine::ROT_Y, 80.f * fTimeDelta);
			m_pAnimator->Set_AnimationSet(ROLL_L);
		}
		else if (fCos < -0.2f)
		{
			m_pTransform->Rotation(Engine::ROT_Y, -80.f * fTimeDelta);
			m_pAnimator->Set_AnimationSet(ROLL_R);
		}
		else
		{
			if (m_pAnimator->Set_AnimationSet(STAND_IDLE))
			{
				m_bAngry = true;
			}
		}
	}

	if (m_bAngry)
	{
		if (m_pAnimator->Get_AnimationIndex() == STAND_IDLE)
		{
			LookAtDir(&m_vRayDir, fTimeDelta);
			if (m_pAnimator->Set_AnimationSet(CRAZYFOOT))
			{
				m_bCrazyFoot = true;
				m_bAngry = false;
			}
		}
	}

	if (m_bCrazyFoot)
	{
		if (m_pAnimator->Get_AnimationIndex() == CRAZYFOOT)
		{
			m_pSound->MyPlaySound(L"CrazyFoot_Ice_Fall");
			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[CRAZYFOOT]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CRAZYFOOT]->tMotionInfo.dEndTime * 0.3,
				(*m_pvecAniInfo)[CRAZYFOOT]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CRAZYFOOT]->tMotionInfo.dEndTime * 0.4))
			{
				m_pSound->MyPlaySound(L"Floor_Hit_1");
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 4);
			}

			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[CRAZYFOOT]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CRAZYFOOT]->tMotionInfo.dEndTime * 0.6,
				(*m_pvecAniInfo)[CRAZYFOOT]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[CRAZYFOOT]->tMotionInfo.dEndTime * 0.7))
			{
				m_pSound->MyPlaySound(L"Floor_Hit_2");
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 4);
			}

			if (m_pAnimator->Set_AnimationSet(ROAR))
			{
				m_bRoar = true;
				m_bCrazyFoot = false;
				((CPlayerCamera*)Engine::GetCamera(Engine::CAMERA_PLAYER))->Set_ActionEnd();
			}
		}
	}

	if (m_bRoar)
	{
		if (m_pAnimator->Get_AnimationIndex() == ROAR)
		{
			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROAR]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROAR]->tMotionInfo.dEndTime * 0.0,
				(*m_pvecAniInfo)[ROAR]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROAR]->tMotionInfo.dEndTime * 0.1))
			{
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 4);
			}

			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROAR]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROAR]->tMotionInfo.dEndTime * 0.3,
				(*m_pvecAniInfo)[ROAR]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROAR]->tMotionInfo.dEndTime * 0.4))
			{
				m_pSound->MyPlaySound(L"Roar_Voice");
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 4);
			}
			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROAR]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROAR]->tMotionInfo.dEndTime * 0.5,
				(*m_pvecAniInfo)[ROAR]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROAR]->tMotionInfo.dEndTime * 0.6))
			{
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 4);
			}
			if (m_pAnimator->IsPlaying_ThisSection((*m_pvecAniInfo)[ROAR]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROAR]->tMotionInfo.dEndTime * 0.7,
				(*m_pvecAniInfo)[ROAR]->tMotionInfo.dStartTime + (*m_pvecAniInfo)[ROAR]->tMotionInfo.dEndTime * 0.8))
			{
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 1.0f, 4);
			}

			if (m_pAnimator->Set_AnimationSet(BATTLE_IDLE))
			{
				m_pAnimator->Set_State(MONSTER_FIGHT);
				m_bCombat = true;
				m_bRoar = false;
			}
		}
	}

	if (m_bCombat)
	{
		m_fCombatTime += fTimeDelta;
		if (m_fCombatTime > 4.5f)
		{
			m_pAI->Set_State(AI_IDLE);
		}
	}




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

	//if (m_fPlayerDis < m_fAggroRad)
	//{
	//	m_pAI->Set_State(AI_TRACE);
	//}
	

}

void CAI_CameraIntro_Hanuman::End_State(void)
{

}

void CAI_CameraIntro_Hanuman::SetCameraStart()
{
	m_bCameraStart = true;
}

HRESULT CAI_CameraIntro_Hanuman::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
{
	if (pTransform == NULL || pAnimator == NULL || pNaviMesh == NULL)
		return E_FAIL;

	m_pTransform = pTransform;
	m_pTransform->AddRef();

	m_pAnimator = pAnimator;
	m_pAnimator->AddRef();

	m_pNavigationMesh = pNaviMesh;

	m_pCalculator = pCalculator;
	m_pCalculator->AddRef();

	m_pRigidbody = pRigidbody;
	m_pRigidbody->AddRef();

	m_pCollider = pCollider;

	m_pColInfo = pColInfo;

	m_pDamaged = pDamaged;

	m_pAI = pAI;

	m_pSound = pSound;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"Player");
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	Engine::Safe_Release(pGameObject);

	m_fAggroRad = 200.f;

	m_fMonsterSpeed = 20.f;

	Start_State();

	return S_OK;
}

CAI_CameraIntro_Hanuman * CAI_CameraIntro_Hanuman::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CCalculator* pCalculator, Engine::CRigidbody* pRigidbody
	, Engine::CCollider* pCollider, Engine::COLINFO* pColInfo, _bool* pDamaged
	, Engine::CAI* pAI, CSound* pSound)
{
	CAI_CameraIntro_Hanuman* pInstance = new CAI_CameraIntro_Hanuman;


	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pCalculator, pRigidbody, pCollider
		, pColInfo, pDamaged, pAI, pSound)))
	{
		MSG_BOX("CAI_CameraIntro_Hanuman Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_CameraIntro_Hanuman::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pTargetTransform);
}
