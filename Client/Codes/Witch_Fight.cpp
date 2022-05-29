#include "stdafx.h"
#include "Witch_Fight.h"

#include "WitchBladeWeapon.h"
#include "Fade_Manager.h"
#include "Light.h"
#include "UI_SKill.h"
#include "PlayerCamera.h"
#include "Sound.h"
#include "CoolTimeMgr.h"

CWitch_Fight::CWitch_Fight(_uint iAniIndex)
: CWitch_State(iAniIndex)
, m_vLook(0.f, 0.f, 0.f)
, m_bCanMove(false)
, m_bCanJump(true)
, m_fDarkGrave(0.f)
, m_fSpiralDeley(0.f)
, m_fDarkDelay(0.f)
, m_fSpinSpeed(0.f)
, m_fFadeTime(0.f)
, m_iSoundCnt(0)
, m_bCameraAction(false)
, m_bCameraIntro(false)
{
	ZeroMemory(&m_bAddCol, sizeof(_bool) * COL_END);
	ZeroMemory(&m_tColInfo, sizeof(Engine::COLINFO));
}

CWitch_Fight::~CWitch_Fight(void)
{

}

void CWitch_Fight::SetActive(void)
{
	m_pLeftSword->SetActive(true);
	m_pRightSword->SetActive(true);
}

void CWitch_Fight::Set_Skill(const CUI_Skill* pSkill)
{
	switch (const_cast<CUI_Skill*>(pSkill)->Get_Skill_Info()->m_eSkillId)
	{
	case SKILL_RUSH:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DISARM))
		{
			m_bAction[CWitchBlade::ACTION_RUSH] = true;
			m_bCanMove = false;
			m_pSound->MyPlaySound(L"Disiram");
			m_pSound->MyPlaySound(L"Jump");
		}
		break;
	case SKILL_FLYDANCE:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLFLING))
		{
			m_bAction[CWitchBlade::ACTION_FLYDANCE] = true;
			m_bCanMove = false;
		}
		break;

	case SKILL_FLEDGE_BLADE:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLBASH))
		{
			m_bAction[CWitchBlade::ACTION_PLEDGEBLADE] = true;
			m_bCanMove = false;
			m_pSound->MyPlaySound(L"Fledge");
		}
		break;

	case SKILL_BLACKROSE:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLGROUNDKICKSWO))
		{
			m_bAction[CWitchBlade::ACTION_BLACKROSE] = true;
			m_bCanMove = false;
		}
		break;

	case SKILL_VIOLET:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLBLADERUNNER))
		{
			m_bAction[CWitchBlade::ACTION_VIOLETCROSS] = true;
			m_bCanMove = false;
		}
		break;

	case SKILL_SPIRALDANCE:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLCOMBOATTACK))
		{
			m_bAction[CWitchBlade::ACTION_SPIRALDANCE] = true;
			m_bCanMove = false;
			m_pSound->MyPlaySound(L"Spiral");
		}
		break;

	case SKILL_STUNPIN:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILL2ANTHREAT))
		{
			m_bAction[CWitchBlade::ACTION_SPINTURNTHRUST] = true;
			m_bCanMove = false;
			m_fSpinSpeed = 100.f;
		}
		break;

	case SKILL_DARKPOLLING:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLDARKGRAVE))
		{
			m_bAction[CWitchBlade::ACTION_DARKFALLINGGRAVE] = true;
			m_bCanMove = false;
			m_pSound->MyPlaySound(L"VioletWind1");
			m_pSound->MyPlaySound(L"DarkVoice1");
		}
		break;

	case SKILL_CROSS:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLBUFF))
		{
			m_bAction[CWitchBlade::ACTION_CROSSBLITZ] = true;
			m_bCanMove = false;
		}
		break;
	case SKILL_DRAGONDANCE:
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_ATTACK6))
		{
			m_bAction[CWitchBlade::ACTION_DRAGONDANCE] = true;
			m_bCanMove = false;
		}
		break;
	}
}

void CWitch_Fight::Set_Move(const _bool & bMove)
{
	m_bCanMove = bMove;
}

void CWitch_Fight::Set_ActionEnd(void)
{
	m_bCameraAction = false;
}

void CWitch_Fight::Set_PointLight(Engine::CLight * pLight)
{
	if (pLight == NULL)
		return;

	m_pPointLight = pLight;
}

void CWitch_Fight::Get_Move(_bool* bMove)
{
	*bMove = m_bCanMove;
}

void CWitch_Fight::Start_State(void)
{
	m_pLeftSword->SetActive(true);
	m_pRightSword->SetActive(true);
}

void CWitch_Fight::Update_State(const _float& fTimeDelta)
{
	if (m_pMesh == NULL)
		return;

	CheckCameraAction(fTimeDelta);

	if (!m_bCameraAction)
	{
		ActionCheck();
		JumpCheck(fTimeDelta);
		MoveCheck(fTimeDelta);
		CommandSkillCheck(fTimeDelta);
		ActionSkillCheck(fTimeDelta);
		DemageCheck(fTimeDelta);
		AttackCheck(fTimeDelta);
	}

	CWitch_State::EndCheck();
}

void CWitch_Fight::End_State(void)
{
	m_pLeftSword->SetActive(false);
	m_pRightSword->SetActive(false);
	m_iAniIndex = CWitchBlade::WITCH_ATTACKIDLE;
	m_fDarkGrave = 0.f;
	m_fFadeTime = 0.f;
}

HRESULT CWitch_Fight::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
	CWitchBlade* pPlayer, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword)
{
	if (pMesh == NULL || pLeftSword == NULL || pRightSword == NULL || pPlayer == NULL)
		return E_FAIL;
	m_pMesh = pMesh;
	m_pMesh->AddRef();
	m_pmapNextMotion = pmapNextMotion;
	m_pLeftSword = pLeftSword;
	m_pRightSword = pRightSword;
	m_pPlayer = pPlayer;

	m_pTransform = (Engine::CTransform*)m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);
	if (m_pTransform == NULL)
		return E_FAIL;
	m_pAnimator = (Engine::CAnimator*)m_pPlayer->GetComponent<Engine::CAnimator>(Engine::COMID_DYNAMIC);
	if (m_pAnimator == NULL)
		return E_FAIL;
	m_pRigidbody = (Engine::CRigidbody*)m_pPlayer->GetComponent<Engine::CRigidbody>(Engine::COMID_DYNAMIC);
	if (m_pRigidbody == NULL)
		return E_FAIL;
	m_pCalculator = (Engine::CCalculator*)m_pPlayer->GetComponent<Engine::CCalculator>(Engine::COMID_DYNAMIC);
	if (m_pCalculator == NULL)
		return E_FAIL;
	m_pCollider = (Engine::CCollider*)m_pPlayer->GetComponent<Engine::CBoxCollider>(Engine::COMID_DYNAMIC);
	if (m_pCollider == NULL)
		return E_FAIL;
	m_pSphereCollider = (Engine::CCollider*)m_pPlayer->GetComponent<Engine::CSphereCollider>(Engine::COMID_DYNAMIC);
	if (m_pSphereCollider == NULL)
		return E_FAIL;

	m_pSound = (CSound*)m_pPlayer->GetComponent<CSound>(Engine::COMID_DYNAMIC);
	if (m_pSound == NULL)
		return E_FAIL;

	m_pPlayerInfo = m_pPlayer->GetPlayerInfo();

	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pCollider);
	Engine::Safe_Release(m_pSphereCollider);
	Engine::Safe_Release(m_pSound);
	return S_OK;
}

void CWitch_Fight::ActionCheck(void)
{
	m_pTransform->Get_Infomation(Engine::INFO_LOOK, &m_vLook);
	m_vLook.y = 0.f;
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	_uint iPushCount = 0;
	for (_uint i = 0; i < CWitchBlade::ACTION_END; i++)
	{
		if (m_bAction[i])
			iPushCount++;
	}

	if (iPushCount == 0)
	{
		m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_ATTACKIDLE);
		m_bAction[CWitchBlade::ACTION_JUMP] = false;
	}

	if (iPushCount > 0)
		m_pPlayer->Compute_Camera();

	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACKIDLE)
	{
		m_bCanMove = true;
		m_bCanJump = true;
	}
}

void CWitch_Fight::MoveCheck(const _float & fTimeDelta)
{
	//기본 이동
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		if (m_bCanJump && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_RUNF_IN))
			m_bCanMove = true;

		m_bAction[CWitchBlade::ACTION_MOVEF] = true;
		//앞으로
		if (m_bAction[CWitchBlade::ACTION_MOVEF] && m_bCanMove)
		{
			if (!m_pCalculator->Wall_Collision_Check(&m_vLook) 
				&& !Engine::IsCol_ForOBB(L"Monster", m_pCollider, &(m_vLook * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta))
				&& !Engine::IsCol_ForOBB(L"Static", m_pCollider, &(m_vLook * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta)))
				m_pTransform->Move_Forward(m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
		}
	}
	else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		if (m_bCanJump && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_RUNB_IN))
			m_bCanMove = true;
		m_bAction[CWitchBlade::ACTION_MOVEB] = true;

		_vec3 vDir = m_vLook * -1.f;
		if (!m_pCalculator->Wall_Collision_Check(&vDir) && 
			!Engine::IsCol_ForOBB(L"Monster", m_pCollider, &(-m_vLook * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta))
			&& !Engine::IsCol_ForOBB(L"Static", m_pCollider, &(-m_vLook * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta)))
		{
			//뒤로걷기
			if (m_bAction[CWitchBlade::ACTION_MOVEB] && m_bCanMove)
				m_pTransform->Move_Forward(-m_pPlayerInfo->m_fMoveSpeed / 3.f, fTimeDelta);
		}
	}
	else if ((Engine::Get_DIKeyState(DIK_A) & 0x80) && !m_bAction[CWitchBlade::ACTION_MOVEF] && !m_bAction[CWitchBlade::ACTION_MOVEB])
	{
		if (m_bCanJump && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_RUNL_IN))
			m_bCanMove = true;
		m_bAction[CWitchBlade::ACTION_MOVEL] = true;
		//왼쪽
		if (m_bAction[CWitchBlade::ACTION_MOVEL] && m_bCanMove)
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
			if (!m_pCalculator->Wall_Collision_Check(&vDirection) 
				&& !Engine::IsCol_ForOBB(L"Monster", m_pCollider, &(vDirection * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta))
				&& !Engine::IsCol_ForOBB(L"Static", m_pCollider, &(vDirection * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta)))
				m_pTransform->Move_Left(&m_vLook, m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
		}
	}
	else if ((Engine::Get_DIKeyState(DIK_D) & 0x80) && !m_bAction[CWitchBlade::ACTION_MOVEF] && !m_bAction[CWitchBlade::ACTION_MOVEB])
	{
		if (m_bCanJump && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_RUNR_IN))
			m_bCanMove = true;
		m_bAction[CWitchBlade::ACTION_MOVER] = true;
		//오른쪽
		if (m_bAction[CWitchBlade::ACTION_MOVER] && m_bCanMove)
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
			if (!m_pCalculator->Wall_Collision_Check(&vDirection) 
				&& !Engine::IsCol_ForOBB(L"Monster", m_pCollider, &(vDirection * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta))
				&& !Engine::IsCol_ForOBB(L"Static", m_pCollider, &(vDirection * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta)))
				m_pTransform->Move_Right(&m_vLook, m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
		}
	}

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			//왼쪽위
			if (m_bAction[CWitchBlade::ACTION_MOVEF] && !m_bAction[CWitchBlade::ACTION_MOVER])
				m_pTransform->Rotation(Engine::ROT_Y, -45.f);
		}
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			//오른쪽위
			if (m_bAction[CWitchBlade::ACTION_MOVEF] && !m_bAction[CWitchBlade::ACTION_MOVEL])
				m_pTransform->Rotation(Engine::ROT_Y, 45.f);
		}
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			//왼쪽아래
			if (m_bAction[CWitchBlade::ACTION_MOVEB] && !m_bAction[CWitchBlade::ACTION_MOVER])
				m_pTransform->Rotation(Engine::ROT_Y, 45.f);
		}
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			//오른쪽아래
			if (m_bAction[CWitchBlade::ACTION_MOVEB] && !m_bAction[CWitchBlade::ACTION_MOVEL])
				m_pTransform->Rotation(Engine::ROT_Y, -45.f);
		}
	}

	//소리
	if (m_bAction[CWitchBlade::ACTION_MOVEF] && m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_RUNF_IN)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNF_IN]->tMotionInfo.dEndTime * 0.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNF_IN]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"WalkL");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNF_IN]->tMotionInfo.dEndTime * 0.5
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNF_IN]->tMotionInfo.dEndTime * 0.6))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_MOVEB] && m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_RUNB_IN)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNB_IN]->tMotionInfo.dEndTime * 0.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNB_IN]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNB_IN]->tMotionInfo.dEndTime * 0.5
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNB_IN]->tMotionInfo.dEndTime * 0.6))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_MOVER] && m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_RUNR_IN)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNR_IN]->tMotionInfo.dEndTime * 0.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNR_IN]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"WalkL");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNR_IN]->tMotionInfo.dEndTime * 0.6
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNR_IN]->tMotionInfo.dEndTime * 0.7))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_MOVEL] && m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_RUNL_IN)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNL_IN]->tMotionInfo.dEndTime * 0.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNL_IN]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"WalkL");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNL_IN]->tMotionInfo.dEndTime * 0.6
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNL_IN]->tMotionInfo.dEndTime * 0.7))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
	}


	//키가 때엇을때
	if (!(Engine::Get_DIKeyState(DIK_W) & 0x80) && m_bAction[CWitchBlade::ACTION_MOVEF])
		m_bAction[CWitchBlade::ACTION_MOVEF] = false;
	if (!(Engine::Get_DIKeyState(DIK_S) & 0x80) && m_bAction[CWitchBlade::ACTION_MOVEB])
		m_bAction[CWitchBlade::ACTION_MOVEB] = false;
	if (!(Engine::Get_DIKeyState(DIK_A) & 0x80) && m_bAction[CWitchBlade::ACTION_MOVEL])
		m_bAction[CWitchBlade::ACTION_MOVEL] = false;
	if (!(Engine::Get_DIKeyState(DIK_D) & 0x80) && m_bAction[CWitchBlade::ACTION_MOVER])
		m_bAction[CWitchBlade::ACTION_MOVER] = false;
}

void CWitch_Fight::JumpCheck(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		_int iAniIndex = 0;
		iAniIndex = CWitchBlade::WITCH_JUMPFA;

		if (m_bAction[CWitchBlade::ACTION_MOVEF])
			iAniIndex = CWitchBlade::WITCH_JUMPFA;
		else if (m_bAction[CWitchBlade::ACTION_MOVEB])
			iAniIndex = CWitchBlade::WITCH_JUMPBA;
		else if (m_bAction[CWitchBlade::ACTION_MOVEL])
			iAniIndex = CWitchBlade::WITCH_JUMPLA;
		else if (m_bAction[CWitchBlade::ACTION_MOVER])
			iAniIndex = CWitchBlade::WITCH_JUMPRA;


		if (m_pAnimator->Set_AnimationSet(iAniIndex))
		{
			m_bCanMove = false;
			m_bCanJump = true;
			m_bAction[CWitchBlade::ACTION_JUMP] = true;
			m_pSound->MyPlaySound(L"Jump");
		}
	}

	if ((m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_JUMPFSB
		|| m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_JUMPBB
		|| m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_JUMPRB
		|| m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_JUMPLB) && m_bCanJump)
	{
		if (m_bAction[CWitchBlade::ACTION_MOVEF] || m_bAction[CWitchBlade::ACTION_MOVEB] || m_bAction[CWitchBlade::ACTION_MOVER] || m_bAction[CWitchBlade::ACTION_MOVEL])
			m_bCanMove = true;
		m_bCanJump = false;
		m_pRigidbody->Set_Jump(0.7f);
		m_pCalculator->Set_Jump(true);
	}

	if (m_pCalculator->Get_IsCol())
	{
		_int iAniIndex = 0;
		switch (m_pAnimator->Get_AnimationIndex())
		{
		case CWitchBlade::WITCH_JUMPFSC:
			iAniIndex = CWitchBlade::WITCH_JUMPFSD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_bAction[CWitchBlade::ACTION_JUMP] = false;
			m_pSound->MyPlaySound(L"WalkL");
			break;

		case CWitchBlade::WITCH_JUMPBC:
			iAniIndex = CWitchBlade::WITCH_JUMPBD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_bAction[CWitchBlade::ACTION_JUMP] = false;
			m_pSound->MyPlaySound(L"WalkL");
			break;

		case CWitchBlade::WITCH_JUMPLC:
			iAniIndex = CWitchBlade::WITCH_JUMPLD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_bAction[CWitchBlade::ACTION_JUMP] = false;
			m_pSound->MyPlaySound(L"WalkL");
			break;

		case CWitchBlade::WITCH_JUMPRC:
			iAniIndex = CWitchBlade::WITCH_JUMPRD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_bAction[CWitchBlade::ACTION_JUMP] = false;
			m_pSound->MyPlaySound(L"WalkL");
			break;
		case CWitchBlade::WITCH_DHFB:
			iAniIndex = CWitchBlade::WITCH_DHFC;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			break;
		}
	}
}

void CWitch_Fight::AttackCheck(const _float & fTimeDelta)
{
	//공격 할때 찔끔씩 이동하는 속도
	_float fSpeed = 10.f;

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		fSpeed *= 2.f;

	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) && (m_pPlayer->Get_CameraMove()))
	{
		//땅에 있을때
		if (m_bCanJump)
		{
			//공격 _bool 트루 움직이기 못하게하기
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_ATTACK1) && !m_bAction[CWitchBlade::ACTION_ATTACK1])
			{
				m_bAction[CWitchBlade::ACTION_ATTACK1] = true;
				m_bCanMove = false;
				m_pSound->MyPlaySound(L"Attack1");
				m_pSound->MyPlaySound(L"Sword1");
			}
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_ATTACK2) && !m_bAction[CWitchBlade::ACTION_ATTACK2])
			{
				m_bAction[CWitchBlade::ACTION_ATTACK2] = true;
				m_bCanMove = false;
				m_pSound->MyPlaySound(L"Attack2");
				m_pSound->MyPlaySound(L"Sword2");
			}
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_ATTACK3) && !m_bAction[CWitchBlade::ACTION_ATTACK3])
			{
				m_bAction[CWitchBlade::ACTION_ATTACK3] = true;
				m_bCanMove = false;
				m_pSound->MyPlaySound(L"Attack3");
				m_pSound->MyPlaySound(L"Sword3");
			}
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_ATTACK4) && !m_bAction[CWitchBlade::ACTION_ATTACK4])
			{
				m_bAction[CWitchBlade::ACTION_ATTACK4] = true;
				m_bCanMove = false;
				m_pRigidbody->Set_AccMotion(&m_vLook, (fSpeed * 1.5f));
				m_pSound->MyPlaySound(L"Attack4");
				m_pSound->MyPlaySound(L"Sword4");
			}
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_ATTACK5) && !m_bAction[CWitchBlade::ACTION_ATTACK5])
			{
				m_bAction[CWitchBlade::ACTION_ATTACK5] = true;
				m_bCanMove = false;
				m_pSound->MyPlaySound(L"Attack5");
				m_pSound->MyPlaySound(L"Sword5");
			}
		}
	}

	//일반 공격 할때 이동
	//Attack1
	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK1)
	{
		//벽에 아직 안부딪혓으면 움직여
		if (!m_pCalculator->Wall_Collision_Check(&m_vLook))
		{
			//중간부분 이동
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime / 3.0,
				(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime / 3.0))
				m_pRigidbody->Set_AccMotion(&m_vLook, fSpeed);
			else
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			
			if (!Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
			{
				//끝 부분 이동
				if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime / 4.5, 
					(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime))
					m_pRigidbody->Set_AccMotion(&m_vLook, fSpeed);
			}
		}
	}
	//Attack2
	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK2)
	{
		if(IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dStartTime, (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime / 4.0))
			m_pRigidbody->Set_AccMotion(&m_vLook, fSpeed);
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//Attack3
	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK3)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dStartTime, (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime / 4.0))
			m_pRigidbody->Set_AccMotion(&m_vLook, fSpeed);
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//Attack5
	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK5)
	{
		if (!m_pCalculator->Wall_Collision_Check(&m_vLook))
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime / 4.0,
				(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime))
				m_pRigidbody->Set_AccMotion(&m_vLook, fSpeed);
		}
	}

	//Attack1충돌
	if (m_bAction[CWitchBlade::ACTION_ATTACK1] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK1))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK1]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_RUSH;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	//Attack2충돌
	if (m_bAction[CWitchBlade::ACTION_ATTACK2] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK2))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK2]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK2]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK2]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK2]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_RUSH;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	//Attack3충돌
	if (m_bAction[CWitchBlade::ACTION_ATTACK3] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK3))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK3]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK3]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK3]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK3]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_RUSH;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	//Attack4충돌
	if (m_bAction[CWitchBlade::ACTION_ATTACK4] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK4))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK4]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK4]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK4]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK4]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_RUSH;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	//Attack5충돌
	if (m_bAction[CWitchBlade::ACTION_ATTACK5] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK5))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK5]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK5]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK5]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK5]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(10.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 7.f, 7.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_SPINB;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 300.f;
				tColInfo.fForce = 20.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_RUSH;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	//_bool Off
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_ATTACK1) && (m_bAction[CWitchBlade::ACTION_ATTACK1]))
	{
		m_bAction[CWitchBlade::ACTION_ATTACK1] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_ATTACK2) && (m_bAction[CWitchBlade::ACTION_ATTACK2]))
		m_bAction[CWitchBlade::ACTION_ATTACK2] = false;
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_ATTACK3) && (m_bAction[CWitchBlade::ACTION_ATTACK3]))
		m_bAction[CWitchBlade::ACTION_ATTACK3] = false;
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_ATTACK4) && (m_bAction[CWitchBlade::ACTION_ATTACK4]))
	{
		m_bAction[CWitchBlade::ACTION_ATTACK4] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_ATTACK5) && (m_bAction[CWitchBlade::ACTION_ATTACK5]))
	{
		m_bAction[CWitchBlade::ACTION_ATTACK5] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
}

void CWitch_Fight::CommandSkillCheck(const _float & fTimeDelta)
{
	//F키
	if (Engine::Get_DIKeyState(DIK_F) & 0x80)
	{
		//리들킥

		if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_RIDDLE))
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_GROUNDKICK))
			{

				m_bCanMove = false;
				m_bAction[CWitchBlade::ACTION_RIDDLEKICK] = true;
				CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_RIDDLE);
			}
		}
		//힐턴
		if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_HILLTURN))
			{
				if ((m_pRigidbody->Get_AccDist() > 5.f) && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLJUMPKICK))
				{
					m_pRigidbody->Set_RevisionGravity(-40.f);
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_HILLTURN] = true;
					m_pSound->MyPlaySound(L"HillturnSwing");
					m_pSound->MyPlaySound(L"Attack3");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_HILLTURN);
				}
			}
		}
		//플래닛댄스
		else
		{
			if (m_bAction[CWitchBlade::ACTION_JUMPATTACKL] || m_bAction[CWitchBlade::ACTION_JUMPATTACKR])
			{
				if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_PLANIT))
				{
					if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPATTACK))
					{
						m_pRigidbody->Set_RevisionGravity(-40.f);
						m_bCanMove = false;
						m_bAction[CWitchBlade::ACTION_PLANETDANCE] = true;
						m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
						m_pSound->MyPlaySound(L"Sword3");
						m_pSound->MyPlaySound(L"Attack2");
						CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_PLANIT);
					}

				}
			}
			else
			{
				if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_PLANIT))
				{
					if ((m_pRigidbody->Get_AccDist() > 5.f) && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPATTACK))
					{
						m_pRigidbody->Set_RevisionGravity(-40.f);
						m_bCanMove = false;
						m_bAction[CWitchBlade::ACTION_PLANETDANCE] = true;
						m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed);
						m_pSound->MyPlaySound(L"Sword3");
						m_pSound->MyPlaySound(L"Attack2");
						CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_PLANIT);
					}
				}
			}
		}
	}

	//리들킥 충돌부분
	if (m_bAction[CWitchBlade::ACTION_RIDDLEKICK] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_GROUNDKICK))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_GROUNDKICK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_GROUNDKICK]->tMotionInfo.dEndTime * 0.3
			, (*m_pAniInfo)[CWitchBlade::WITCH_GROUNDKICK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_GROUNDKICK]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_STAMP;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 5.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_LIDDLEKICK;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.10f + 65);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"RiddleKick");
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}

		if (m_pSphereCollider->IsHit())
			m_pSound->MyPlaySound(L"Hit");

	}

	//힐턴 충돌부분
	if (m_bAction[CWitchBlade::ACTION_HILLTURN] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLJUMPKICK))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLJUMPKICK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLJUMPKICK]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLJUMPKICK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLJUMPKICK]->tMotionInfo.dEndTime * 0.5))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(15.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 0.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 5.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_HILLTURN;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 0.3f + 104);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}
	}

	//플래닛 댄스 충돌 부분
	if (m_bAction[CWitchBlade::ACTION_PLANETDANCE] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_JUMPATTACK))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACK]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACK]->tMotionInfo.dEndTime * 0.5))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(10.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 0.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 5.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_FLENITDANCE;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}
	}

	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB))
	{
		if (m_bAction[CWitchBlade::ACTION_JUMPATTACKL] && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPATTACKR))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
			m_pRigidbody->Set_RevisionGravity(-40.f);
			m_bCanMove = false;
			m_bAction[CWitchBlade::ACTION_JUMPATTACKR] = true;
			m_pSound->MyPlaySound(L"Swing5");
		}
		else if ((m_pRigidbody->Get_AccDist() > 5.f) && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPATTACKR))
		{
			m_pRigidbody->Set_RevisionGravity(-40.f);
			m_bCanMove = false;
			m_bAction[CWitchBlade::ACTION_JUMPATTACKR] = true;
			m_pSound->MyPlaySound(L"Swing4");
		}
	}
	//플래닛댄스,점프공격 제어
	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_JUMPFSC)
		m_pRigidbody->Set_RevisionGravity(0.f);

	//JumpAttackR 충돌
	if (m_bAction[CWitchBlade::ACTION_JUMPATTACKR] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_JUMPATTACKR))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACKR]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACKR]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACKR]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACKR]->tMotionInfo.dEndTime * 0.6))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(10.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 0.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_FLENITDANCE;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}
	}

	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
	{
		if (m_bAction[CWitchBlade::ACTION_JUMPATTACKR] && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPATTACKL))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
			m_pRigidbody->Set_RevisionGravity(-40.f);
			m_bCanMove = false;
			m_bAction[CWitchBlade::ACTION_JUMPATTACKL] = true;
			m_pSound->MyPlaySound(L"Swing5");
		}
		else if ((m_pRigidbody->Get_AccDist() > 5.f) && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPATTACKL))
		{
			m_pRigidbody->Set_RevisionGravity(-40.f);
			m_bCanMove = false;
			m_bAction[CWitchBlade::ACTION_JUMPATTACKL] = true;
			m_pSound->MyPlaySound(L"Swing4");
		}
	}

	//JumpAttackL 충돌
	if (m_bAction[CWitchBlade::ACTION_JUMPATTACKL] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_JUMPATTACKL))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACKL]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACKL]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACKL]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_JUMPATTACKL]->tMotionInfo.dEndTime * 0.6))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(10.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 0.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_FLENITDANCE;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}
	}

	//대쉬
	if ((Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80) && m_bCanJump)
	{
		//위
		if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DASHF))
			{
				m_bCanMove = false;
				m_bAction[CWitchBlade::ACTION_DASHF] = true;
				m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
				m_pSound->MyPlaySound(L"Jump");
			}
		}
		//아래
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DASHB))
			{
				m_bCanMove = false;
				m_bAction[CWitchBlade::ACTION_DASHB] = true;
				m_pRigidbody->Set_AccMotion(&m_vLook, -m_pPlayerInfo->m_fMoveSpeed);
				m_pSound->MyPlaySound(L"Jump");
			}
		}
		//왼쪽
		else if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DASHL))
			{
				m_bCanMove = false;
				m_bAction[CWitchBlade::ACTION_DASHL] = true;
				_vec3			vDirection;
				D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
				m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
				m_pSound->MyPlaySound(L"Jump");
			}
		}
		//오른쪽
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DASHR))
			{
				m_bCanMove = false;
				m_bAction[CWitchBlade::ACTION_DASHR] = true;
				_vec3			vDirection;
				D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
				m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
				m_pSound->MyPlaySound(L"Jump");
			}
		}

		if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			//왼쪽위
			if (Engine::Get_DIKeyState(DIK_A) & 0x80)
				m_pTransform->Rotation(Engine::ROT_Y, -45.f);
			//오른쪽위
			else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
				m_pTransform->Rotation(Engine::ROT_Y, 45.f);
		}
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			//왼쪽아래
			if (Engine::Get_DIKeyState(DIK_A) & 0x80)
				m_pTransform->Rotation(Engine::ROT_Y, 45.f);
			//오른쪽아래
			else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
				m_pTransform->Rotation(Engine::ROT_Y, -45.f);
		}
	}

	//대쉬 이동제한
	if (m_bAction[CWitchBlade::ACTION_DASHF])
	{
		if (m_pAnimator->Is_AnimationEnd(CWitchBlade::WITCH_DASHF, (*m_pAniInfo)[CWitchBlade::WITCH_DASHF]->tMotionInfo.dEndTime / 3.0))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_pSound->MyPlaySound(L"WalkL");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_DASHB])
	{
		if (m_pAnimator->Is_AnimationEnd(CWitchBlade::WITCH_DASHB, (*m_pAniInfo)[CWitchBlade::WITCH_DASHB]->tMotionInfo.dEndTime / 3.0))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_pSound->MyPlaySound(L"WalkL");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_DASHL])
	{
		if (m_pAnimator->Is_AnimationEnd(CWitchBlade::WITCH_DASHL, (*m_pAniInfo)[CWitchBlade::WITCH_DASHL]->tMotionInfo.dEndTime / 3.0))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_pSound->MyPlaySound(L"WalkL");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_DASHR])
	{
		if (m_pAnimator->Is_AnimationEnd(CWitchBlade::WITCH_DASHR, (*m_pAniInfo)[CWitchBlade::WITCH_DASHR]->tMotionInfo.dEndTime / 3.0))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_pSound->MyPlaySound(L"WalkL");
		}
	}

	//소프트랜딩
	if ((Engine::Get_DIKeyState(DIK_SPACE) & 0x80) && !m_bCanJump)
	{
		//위
		if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SOFTRANDING))
			{
				if ((m_pRigidbody->Get_AccDist() > 5.f) && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DASHF))
				{
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_SOFTRANDINGF] = true;
					m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
					m_pRigidbody->Set_RevisionGravity(-30.f);
					m_pRigidbody->Set_Jump(0.5f);
					m_pSound->MyPlaySound(L"SoftRanding");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SOFTRANDING);
				}
			}
		}
		//아래
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SOFTRANDING))
			{
				if ((m_pRigidbody->Get_AccDist() > 5.f) && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPBB))
				{
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_SOFTRANDINGB] = true;
					m_pRigidbody->Set_AccMotion(&m_vLook, -m_pPlayerInfo->m_fMoveSpeed);
					m_pRigidbody->Set_RevisionGravity(-30.f);
					m_pRigidbody->Set_Jump(0.5f);
					m_pSound->MyPlaySound(L"SoftRanding");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SOFTRANDING);
				}
			}
		}
		//왼쪽
		else if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SOFTRANDING))
			{
				if ((m_pRigidbody->Get_AccDist() > 5.f) && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DASHL))
				{
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_SOFTRANDINGL] = true;
					_vec3			vDirection;
					D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
					m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
					m_pRigidbody->Set_RevisionGravity(-30.f);
					m_pRigidbody->Set_Jump(0.5f);
					m_pSound->MyPlaySound(L"SoftRanding");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SOFTRANDING);
				}
			}
		}
		//오른쪽
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SOFTRANDING))
			{
				if ((m_pRigidbody->Get_AccDist() > 5.f) && m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DASHR))
				{
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_SOFTRANDINGR] = true;
					_vec3			vDirection;
					D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
					m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
					m_pRigidbody->Set_RevisionGravity(-30.f);
					m_pRigidbody->Set_Jump(0.5f);
					m_pSound->MyPlaySound(L"SoftRanding");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SOFTRANDING);
				}
			}
		}

		if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			//왼쪽위
			if (Engine::Get_DIKeyState(DIK_A) & 0x80)
				m_pTransform->Rotation(Engine::ROT_Y, -45.f);
			//오른쪽위
			else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
				m_pTransform->Rotation(Engine::ROT_Y, 45.f);
		}
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			//왼쪽아래
			if (Engine::Get_DIKeyState(DIK_A) & 0x80)
				m_pTransform->Rotation(Engine::ROT_Y, 45.f);
			//오른쪽아래
			else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
				m_pTransform->Rotation(Engine::ROT_Y, -45.f);
		}
	}

	//스피닝하트
	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) && m_bCanJump && m_pPlayer->Get_CameraMove())
	{
		// 왼쪽
		if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SPINING))
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLMOVELSPINNINGHEART))
				{
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_SPINNINGHEARTL] = true;
					m_pSound->MyPlaySound(L"SpinningMove");
					m_pSound->MyPlaySound(L"Attack4");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SPINING);
				}
			}
		}
		//아래쪽
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SPINING))
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLMOVEBSPINNINGHEART))
				{
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_SPINNINGHEARTB] = true;
					m_pSound->MyPlaySound(L"SpinningMove");
					m_pSound->MyPlaySound(L"Attack4");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SPINING);
				}
			}
		}
		//오른쪽
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SPINING))
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLMOVERSPINNINGHEART))
				{
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_SPINNINGHEARTR] = true;
					m_pSound->MyPlaySound(L"SpinningMove");
					m_pSound->MyPlaySound(L"Attack4");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SPINING);
				}
			}
			
		}
		//그냥
		else
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SPINING))
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLSPINNINGHEARTA))
				{
					m_bCanMove = false;
					m_bAction[CWitchBlade::ACTION_SPINNINGHEART] = true;
					m_pSound->MyPlaySound(L"SpinningLoop");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SPINING);
				}
			}
		}
	}
	else
	{
		if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLSPINNINGHEARTB)
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLSPININGHEARTC))
			{
				m_bAction[CWitchBlade::ACTION_SPINNINGHEART] = false;
				m_bAction[CWitchBlade::ACTION_SPINNINGHEARTL] = false;
				m_bAction[CWitchBlade::ACTION_SPINNINGHEARTR] = false;
				m_bAction[CWitchBlade::ACTION_SPINNINGHEARTB] = false;
				m_pSound->MyPlaySound(L"Attack5");
			}
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLSPINNINGHEARTB)
	{
		m_pSound->MyPlaySound(L"SpinningLoop");
	}

	//스피닝하트 이동
	if (m_bAction[CWitchBlade::ACTION_SPINNINGHEARTL])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVELSPINNINGHEART]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVELSPINNINGHEART]->tMotionInfo.dEndTime / 5.0,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVELSPINNINGHEART]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVELSPINNINGHEART]->tMotionInfo.dEndTime / 3.0)
			&& m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLMOVELSPINNINGHEART)
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
			m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_SPINNINGHEARTR])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVERSPINNINGHEART]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVERSPINNINGHEART]->tMotionInfo.dEndTime / 5.0,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVERSPINNINGHEART]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVERSPINNINGHEART]->tMotionInfo.dEndTime / 3.0)
			&& m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLMOVERSPINNINGHEART)
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
			m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_SPINNINGHEARTB])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVEBSPINNINGHEART]->tMotionInfo.dStartTime,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVEBSPINNINGHEART]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLMOVEBSPINNINGHEART]->tMotionInfo.dEndTime / 5.0)
			&& m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLMOVEBSPINNINGHEART)
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, -(m_pPlayerInfo->m_fMoveSpeed / 2.f));
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	//잡기(바이퍼니킥, 어반브레이크)
	if (Engine::Get_DIKeyState(DIK_E) & 0x80)
	{
		if (Engine::IsCapture(L"Monster", m_pCollider))
		{
			//어반브레이크
			if (Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLRCATCH))
				{

				}
			}
			//바이퍼 니킥
			else
			{

				if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_VIPER))
				{
					if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLCAPTURE))
					{
						if (!m_bAddCol[COL2])
						{
							m_bAddCol[COL2] = true;
							m_pSphereCollider->SetScale(2.f);
							m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
							Engine::COLINFO tColInfo;
							tColInfo.eColInfo = Engine::COL_CAPTURE;
							tColInfo.eColDir = Engine::DIR_LOOK;
							tColInfo.fJumpForce = 0.f;
							tColInfo.fStampForce = 0.f;
							tColInfo.fForce = 0.f;
							tColInfo.vDir = m_vLook;
							tColInfo.dwSkillID = SKILL_BLOODAGIL;
							tColInfo.iDamage = 0;
							tColInfo.bCritical = false;
							m_pSphereCollider->Set_ColInfo(&tColInfo);
							Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
							m_pSound->MyPlaySound(L"Viper1");
							m_pSound->MyPlaySound(L"ViperVoice1");
						}

						m_bAction[CWitchBlade::ACTION_VIPERKNEEKICK] = true;
						m_bCanMove = false;
						CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_VIPER);
					}
				}
				
			}
		}
		else
		{
			m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLCATCH);
		}
	}

	//바이퍼 니킥 이동, 충돌
	if (m_bAction[CWitchBlade::ACTION_VIPERKNEEKICK] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLCAPTURE))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dEndTime * 0.4))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 5.f);
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dStartTime,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dEndTime * 0.2))
		{
			_float fSpeed = (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.fSpeed * 0.5f;
			m_pAnimator->Set_TrackSpeed(fSpeed);
		}
		else
		{
			m_pAnimator->Set_TrackSpeed((*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.fSpeed);
		}

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCAPTURE]->tMotionInfo.dEndTime * 0.3))
		{
			if (m_bAddCol[COL2])
			{
				m_bAddCol[COL2] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 5.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_VIFERKICK;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 3.4f + 832);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"Viper2");
				m_pSound->MyPlaySound(L"ViperVoice2");
				m_pSound->MyPlaySound(L"Hit");
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}
	
		
	//다운 상태일때 커맨드 액션
	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_DOWN)
	{
		//그냥 일어나
		if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
			m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_STANDUP);
		//퀵스탠드앞으로
		else if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_QUICKSTANDUPF))
			{
				m_bAction[CWitchBlade::ACTION_TECHNICALDOWNACTIONF] = true;
				m_pSound->MyPlaySound(L"Jump");
			}
		}
		//퀵스탠드 뒤로
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_QUICKSTANDUPB))
			{
				m_bAction[CWitchBlade::ACTION_TECHNICALDOWNACTIONB] = true;
				m_pSound->MyPlaySound(L"Jump");
			}
		}
		//기상공격
		else if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
		{
			if (!CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_UPATT))
				return;
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLRISINGF))
				m_bAction[CWitchBlade::ACTION_RISINGATTACK] = true;
			CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_UPATT);
		}
	}

	//테크니컬다운액션, 기상공격 이동
	if (m_bAction[CWitchBlade::ACTION_RISINGATTACK])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime / 5.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime / 5.0))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed / 2.f);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_TECHNICALDOWNACTIONB])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_QUICKSTANDUPB]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_QUICKSTANDUPB]->tMotionInfo.dEndTime / 4.0,
			(*m_pAniInfo)[CWitchBlade::WITCH_QUICKSTANDUPB]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime / 6.0))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, -(m_pPlayerInfo->m_fMoveSpeed / 2.f));
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_TECHNICALDOWNACTIONF])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_QUICKSTANDUPF]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_QUICKSTANDUPF]->tMotionInfo.dEndTime / 4.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_QUICKSTANDUPF]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime / 5.0))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed / 2.f);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	//기상공격 충돌
	if (m_bAction[CWitchBlade::ACTION_RISINGATTACK] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLRISINGF))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime * 0.3
			, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 0.5f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_UPATT;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"RiddleKick");
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime * 0.5
			, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRISINGF]->tMotionInfo.dEndTime * 0.6))
		{
			if (!m_bAddCol[COL2])
			{
				m_bAddCol[COL2] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 0.5f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_UPATT;
				tColInfo.iDamage = GetDamage(&tColInfo.bCritical);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"RiddleKick", false, true);
			}
		}
		else
		{
			if (m_bAddCol[COL2])
			{
				m_bAddCol[COL2] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}

	}

	//블레어실루엣, 실루엣댄스
	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) && !(Engine::Get_DIKeyState(DIK_W) & 0x80) && !(Engine::Get_DIKeyState(DIK_S) & 0x80))
	{
		if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			if (m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL])
			{
				if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SILUET))
				{
					if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLEVASIONL_2))
					{
						m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCEL] = true;
						m_bCanMove = false;
						m_pSound->MyPlaySound(L"SwordSkill4");
						m_pSound->MyPlaySound(L"Attack4");
						CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SILUET);
					}

				}
			}
			else
			{
				if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_BLARE))
				{
					if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLEVASIONL))
					{
						m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL] = true;
						m_bCanMove = false;
						m_pSound->MyPlaySound(L"Blair");
						m_pSound->MyPlaySound(L"BlairVoice");
						CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_BLARE);
					}
				}
			}
		}
		else if(Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			if (m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTER])
			{
				if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_SILUET))
				{
					if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLEVASIONR_2))
					{

						m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCER] = true;
						m_bCanMove = false;
						m_pSound->MyPlaySound(L"SwordSkill4");
						m_pSound->MyPlaySound(L"Attack4");
						CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_SILUET);
					}
				}
			}
			else
			{

				if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_BLARE))
				{
					if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLEVASIONR))
					{
						m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTER] = true;
						m_bCanMove = false;
						m_pSound->MyPlaySound(L"Blair");
						m_pSound->MyPlaySound(L"BlairVoice");
						CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_BLARE);
					}
				}
			}
		}
	}

	//블레어 실루엣, 실루엣 댄스 이동
	if (m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->tMotionInfo.dEndTime / 5.0,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->tMotionInfo.dEndTime / 2.0))
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
			m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCEL])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL_2]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL_2]->tMotionInfo.dEndTime / 5.0,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL_2]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL_2]->tMotionInfo.dEndTime / 3.0))
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
			m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTER])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->tMotionInfo.dEndTime / 5.0,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->tMotionInfo.dEndTime / 2.0))
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
			m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCER])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR_2]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR_2]->tMotionInfo.dEndTime / 5.0,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR_2]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR_2]->tMotionInfo.dEndTime / 3.0))
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
			m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	//블레어 실루엣, 실루엣 댄스 충돌
	if (m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLEVASIONL))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL]->tMotionInfo.dEndTime * 0.5))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_BASE;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 0.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_BLARE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 3.22f + 394);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	if (m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCEL] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLEVASIONL_2))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL_2]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL_2]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL_2]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONL_2]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 5.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_SILUETDANCE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 3.20f + 416);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	if (m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTER] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLEVASIONR))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR]->tMotionInfo.dEndTime * 0.5))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_BASE;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 0.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_BLARE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 3.22f + 394);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	if (m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCER] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLEVASIONR_2))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR_2]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR_2]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR_2]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLEVASIONR_2]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 5.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_SILUETDANCE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 3.20f + 416);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
	}

	//트리키 래빗
	if ((Engine::Get_DIKeyState(DIK_S) & 0x80) && m_bCanJump)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
		{
			if (CCoolTime_Manager::GetInstance()->CheckCoolTime(CCoolTime_Manager::COOL_TRICK))
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_SKILLRABBIT))
				{
					m_bAction[CWitchBlade::ACTION_TRICKYRABBIT] = true;
					m_bCanMove = false;
					m_pSound->MyPlaySound(L"Tricky");
					CCoolTime_Manager::GetInstance()->StartCoolTime(CCoolTime_Manager::COOL_TRICK);
				}
			}
		}
	}

	//트리키래빗 이동
	if (m_bAction[CWitchBlade::ACTION_TRICKYRABBIT] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLRABBIT))
	{
		_bool bFirstSection = false;
		_bool bSecondSection = false;
		_bool bThirdSection = false;

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime
			, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.03))
		{
			bFirstSection = true;
			m_pRigidbody->Set_AccMotion(&m_vLook, -10.f);
		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);


		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.03
			, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.1))
		{
			_float fSpeed = (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.fSpeed * 0.5f;
			m_pAnimator->Set_TrackSpeed(fSpeed);
			bFirstSection = true;
		}
		else
			m_pAnimator->Set_TrackSpeed((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.fSpeed);

		if (!bFirstSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.1
				, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.2))
			{
				bSecondSection = true;
				m_pRigidbody->Set_AccMotion(&m_vLook, 20.f);
			}
			else
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}
		
	
		if (!bFirstSection && !bSecondSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.5
				, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.6))
			{
				bThirdSection = true;
				m_pRigidbody->Set_AccMotion(&m_vLook, 20.f);
			}
			else
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}
		if (!bFirstSection && !bSecondSection && !bThirdSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.7
				, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.9))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, -20.f);
			}
			else
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}

		if (!bFirstSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.68
				, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.7))
			{
				_float fSpeed = (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.fSpeed * 0.5f;
				m_pAnimator->Set_TrackSpeed(fSpeed);
			}
			else
				m_pAnimator->Set_TrackSpeed((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.fSpeed);

		}
	}

	//트리키 레빗 충돌
	if (m_bAction[CWitchBlade::ACTION_TRICKYRABBIT] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLRABBIT))
	{
		_bool bFirstSection = false;
		_bool bSecondSection = false;
		_bool bThirdSection = false;

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.1
			, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.2))
		{
			bFirstSection = true;
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 15.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_TRICKEY;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 3.05f + 621);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"Sword1");
				m_pSound->MyPlaySound(L"Attack1");
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}
			

		if (!bFirstSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.3
				, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime * 0.4))
			{
				bSecondSection = true;
				if (!m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = true;
					m_pSphereCollider->SetScale(7.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_BASE;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 0.f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 0.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_TRICKEY;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 3.51f + 714);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"Sword2");
					m_pSound->MyPlaySound(L"Attack2");
				}
			}
			else
			{
				if (m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = false;
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
				}
			}
			
		}
		if (!bFirstSection && !bSecondSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.5
				, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.6))
			{
				bThirdSection = true;
				if (!m_bAddCol[COL3])
				{
					m_bAddCol[COL3] = true;
					m_pSphereCollider->SetScale(7.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_FLY;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 1.2f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 3.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_TRICKEY;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 3.05f + 621);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"Sword3");
					m_pSound->MyPlaySound(L"Attack3");
					m_pSound->MyPlaySound(L"Swing1");
				}
			}
			else
			{
				if (m_bAddCol[COL3])
				{
					m_bAddCol[COL3] = false;
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
				}
			}
			
		}
		if (!bFirstSection && !bSecondSection && !bThirdSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.7
				, (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLRABBIT]->tMotionInfo.dEndTime  * 0.8))
			{
				if (!m_bAddCol[COL4])
				{
					m_bAddCol[COL4] = true;
					m_pSphereCollider->SetScale(10.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 5.f, 7.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_SPINB;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 1.f;
					tColInfo.fStampForce = 300.f;
					tColInfo.fForce = 20.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_TRICKEY;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 3.51f + 714);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"Attack4");
					m_pSound->MyPlaySound(L"RiddleKick");
				}
			}
			else
			{
				if (m_bAddCol[COL4])
				{
					m_bAddCol[COL4] = false;
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
				}
			}
		}
	}


	//bool값 꺼줌
	//리들킥
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_GROUNDKICK) && (m_bAction[CWitchBlade::ACTION_RIDDLEKICK]))
		m_bAction[CWitchBlade::ACTION_RIDDLEKICK] = false;
	//힐턴
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLJUMPKICK) && (m_bAction[CWitchBlade::ACTION_HILLTURN]))
	{
		m_bAction[CWitchBlade::ACTION_HILLTURN] = false;
		m_pRigidbody->Set_RevisionGravity(300.f);
	}
	//점프공격들
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_JUMPATTACK) && (m_bAction[CWitchBlade::ACTION_PLANETDANCE]))
		m_bAction[CWitchBlade::ACTION_PLANETDANCE] = false;
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_JUMPATTACKL) && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_JUMPATTACKR) && (m_bAction[CWitchBlade::ACTION_JUMPATTACKR]))
		m_bAction[CWitchBlade::ACTION_JUMPATTACKR] = false;
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_JUMPATTACKL) && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_JUMPATTACKR) && (m_bAction[CWitchBlade::ACTION_JUMPATTACKL]))
		m_bAction[CWitchBlade::ACTION_JUMPATTACKL] = false;
	//대쉬
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DASHF) && (m_bAction[CWitchBlade::ACTION_DASHF]))
	{
		m_bAction[CWitchBlade::ACTION_DASHF] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DASHB) && (m_bAction[CWitchBlade::ACTION_DASHB]))
	{
		m_bAction[CWitchBlade::ACTION_DASHB] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DASHR) && (m_bAction[CWitchBlade::ACTION_DASHR]))
	{
		m_bAction[CWitchBlade::ACTION_DASHR] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DASHL) && (m_bAction[CWitchBlade::ACTION_DASHL]))
	{
		m_bAction[CWitchBlade::ACTION_DASHL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//소프트랜딩(끝나는 부분)
	if (m_bAction[CWitchBlade::ACTION_SOFTRANDINGF])
	{
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPFSC))
		{
			m_bAction[CWitchBlade::ACTION_SOFTRANDINGF] = false;
			m_pRigidbody->Set_RevisionGravity(0.f);
		}
	}
	if (m_bAction[CWitchBlade::ACTION_SOFTRANDINGB])
	{
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPBC))
		{
			m_bAction[CWitchBlade::ACTION_SOFTRANDINGB] = false;
			m_pRigidbody->Set_RevisionGravity(0.f);
		}
	}
	if (m_bAction[CWitchBlade::ACTION_SOFTRANDINGL])
	{
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPLC))
		{
			m_bAction[CWitchBlade::ACTION_SOFTRANDINGL] = false;
			m_pRigidbody->Set_RevisionGravity(0.f);
		}
	}
	if (m_bAction[CWitchBlade::ACTION_SOFTRANDINGR])
	{
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_JUMPRC))
		{
			m_bAction[CWitchBlade::ACTION_SOFTRANDINGR] = false;
			m_pRigidbody->Set_RevisionGravity(0.f);
		}
	}
	//기상공격, 테크니컬 다운액션
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_QUICKSTANDUPF) && (m_bAction[CWitchBlade::ACTION_TECHNICALDOWNACTIONF]))
	{
		m_bAction[CWitchBlade::ACTION_TECHNICALDOWNACTIONF] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_QUICKSTANDUPB) && (m_bAction[CWitchBlade::ACTION_TECHNICALDOWNACTIONB]))
	{
		m_bAction[CWitchBlade::ACTION_TECHNICALDOWNACTIONB] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLRISINGF) && (m_bAction[CWitchBlade::ACTION_RISINGATTACK]))
	{
		m_bAction[CWitchBlade::ACTION_RISINGATTACK] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//블레어실루엣, 실루엣댄스
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLEVASIONL) && (m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL]))
	{
		m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTEL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLEVASIONR) && (m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTER]))
	{
		m_bAction[CWitchBlade::ACTION_BLAIRESILHOUETTER] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLEVASIONL_2) && (m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCEL]))
	{
		m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCEL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLEVASIONR_2) && (m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCER]))
	{
		m_bAction[CWitchBlade::ACTION_SILHOUETTEDANCER] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//트리키래빗
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLRABBIT) && (m_bAction[CWitchBlade::ACTION_TRICKYRABBIT]))
	{
		m_bAction[CWitchBlade::ACTION_TRICKYRABBIT] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//바이퍼니킥
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLCAPTURE) && (m_bAction[CWitchBlade::ACTION_VIPERKNEEKICK]))
	{
		m_bAction[CWitchBlade::ACTION_VIPERKNEEKICK] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//어반브레이크
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLRCATCH) && (m_bAction[CWitchBlade::ACTION_URBANBREAK]))
	{
		m_bAction[CWitchBlade::ACTION_URBANBREAK] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
}

void CWitch_Fight::ActionSkillCheck(const _float & fTimeDelta)
{
	//러쉬 스킬
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DISARM) && m_bAction[CWitchBlade::ACTION_RUSH])
	{
		m_bAction[CWitchBlade::ACTION_RUSH] = false;
		m_bCanMove = true;
		m_pAnimator->Set_State(PLAYER_DUNGEON);
	}

	//플라이댄스
	if (m_bAction[CWitchBlade::ACTION_FLYDANCE] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLFLING))
	{
		_bool bFirstSectionIn = false;
		
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dStartTime,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dEndTime / 4.1))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 5.f);
			bFirstSectionIn = true;
			_float fSpeed = (*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.fSpeed * 0.7f;
			m_pAnimator->Set_TrackSpeed(fSpeed);
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			m_pAnimator->Set_TrackSpeed((*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.fSpeed);
		}

		
		if (!bFirstSectionIn)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dEndTime / 3.8,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dEndTime - (*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dEndTime / 4.0))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
			}
			else
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLFLING]->tMotionInfo.dEndTime * 0.5))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_FLYDANCE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.46f + 603);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"SwordSkill4");
				m_pSound->MyPlaySound(L"Attack1");
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}
	}

	//플렛지 블레이드
	if (m_bAction[CWitchBlade::ACTION_PLEDGEBLADE] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLBASH))
	{
		_bool bFirstSectionIn = false;
		_bool bSecondSectionIn = false;
		_bool bThirdSectionIn = false;

		//이동
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dEndTime * 0.1,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dEndTime * 0.2))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
			bFirstSectionIn = true;
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 5.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_FLEDGE_BLADE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 3.98f + 228);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"FledgeVoice1");
				m_pSound->MyPlaySound(L"SwordSkill1");
			}
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}

		if (!bFirstSectionIn)
		{
			//이동
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dEndTime * 0.3,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dEndTime * 0.4))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
				bSecondSectionIn = true;
				if (!m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = true;
					m_pSphereCollider->SetScale(7.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_PUSH;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 0.f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 5.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_FLEDGE_BLADE;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 3.18f + 182);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"FledgeVoice2");
					m_pSound->MyPlaySound(L"SwordSkill2");
				}
			}
			else
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
				if (m_bAddCol[COL2])
				{
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
					m_bAddCol[COL2] = false;
				}
			}
		}
		if (!bFirstSectionIn && !bSecondSectionIn)
		{
			//이동
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dEndTime * 0.5,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dEndTime * 0.6))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
				bThirdSectionIn = true;
				if (!m_bAddCol[COL3])
				{
					m_bAddCol[COL3] = true;
					m_pSphereCollider->SetScale(7.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_PUSH;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 0.f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 5.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_FLEDGE_BLADE;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 2.39f + 137);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"FledgeVoice3");
					m_pSound->MyPlaySound(L"SwordSkill3");
				}
			}
			else
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
				if (m_bAddCol[COL3])
				{
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
					m_bAddCol[COL3] = false;
				}
			}
		}
		if (!bFirstSectionIn && !bSecondSectionIn && !bThirdSectionIn)
		{
			//이동
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dEndTime * 0.7,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBASH]->tMotionInfo.dEndTime * 0.8))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
				if (!m_bAddCol[COL4])
				{
					m_bAddCol[COL4] = true;
					m_pSphereCollider->SetScale(7.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_SPINA;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 1.0f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 20.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_FLEDGE_BLADE;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 1.99f + 114);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"FledgeVoice4");
					m_pSound->MyPlaySound(L"SwordSkill4");
				}
			}
			else
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
				if (m_bAddCol[COL4])
				{
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
					m_bAddCol[COL4] = false;
				}
			}
		}
	}
	//블랙로즈
	if (m_bAction[CWitchBlade::ACTION_BLACKROSE] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLGROUNDKICKSWO))
	{
		_bool bFirstSection = false;
		_bool bSecondSection = false;

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.25,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.3))
		{
			_float fSpeed = (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.fSpeed * 0.5f;
			m_pAnimator->Set_TrackSpeed(fSpeed);
		}
		else
		{
			m_pAnimator->Set_TrackSpeed((*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.fSpeed);
		}

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.1,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.2))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
			bFirstSection = true;

			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 5.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_BLACKROSE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.48f + 411);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"BlackRose1");
				m_pSound->MyPlaySound(L"BlackRoseVoice1");
			}

			if(m_pSphereCollider->IsHit())
				m_pSound->MyPlaySound(L"RiddleKick");
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}

		if (!bFirstSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.3,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.4))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
				bSecondSection = true;
				if (!m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = true;
					m_pSphereCollider->SetScale(10.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 7.f, 7.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_SPINB;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 1.f;
					tColInfo.fStampForce = 300.f;
					tColInfo.fForce = 5.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_BLACKROSE;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 2.48f + 411);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"BlackRose2");
					m_pSound->MyPlaySound(L"BlackRoseVoice2");
				}
			}
			else
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
				if (m_bAddCol[COL2])
				{
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
					m_bAddCol[COL2] = false;
				}
			}
			if (m_pSphereCollider->IsHit())
				m_pSound->MyPlaySound(L"RiddleKick2");
		}
		if (!bFirstSection && !bSecondSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.5,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.7))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 20.f);
			}
			else
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);

			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.65,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLGROUNDKICKSWO]->tMotionInfo.dEndTime * 0.75))
			{
				if (!m_bAddCol[COL3])
				{
					m_bAddCol[COL3] = true;
					m_pSphereCollider->SetScale(7.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_PUSH;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 0.0f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 10.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_BLACKROSE;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 2.48f + 411);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"Attack4");
					m_pSound->MyPlaySound(L"Sword5");
				}
			}
			else
			{
				if (m_bAddCol[COL3])
				{
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
					m_bAddCol[COL3] = false;
				}
			}
		}
	}

	//바이올렛크로스
	if (m_bAction[CWitchBlade::ACTION_VIOLETCROSS] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLBLADERUNNER))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.dStartTime,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.dEndTime * 0.2))
		{
			_float fSpeed = (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.fSpeed / 3.f;
			m_pAnimator->Set_TrackSpeed(fSpeed);
			m_pSound->MyPlaySound(L"VioletVoice");
			m_pSound->MyPlaySound(L"VioletWind1");
		}
		else
		{
			m_pAnimator->Set_TrackSpeed((*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.fSpeed);
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.dEndTime * 0.3,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLBLADERUNNER]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_SMASH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 20.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_VIOLET;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.48f + 411 * 4);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyStopSound(L"VioletVoice");
				m_pSound->MyStopSound(L"VioletWind1");
				m_pSound->MyPlaySound(L"Attack4");
				m_pSound->MyPlaySound(L"VioletWind2");
			}

			if (m_pSphereCollider->IsHit())
				m_pSound->MyPlaySound(L"Sword5");
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}
	}
	//스핀턴스러스트
	if (m_bAction[CWitchBlade::ACTION_SPINTURNTHRUST] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILL2ANTHREAT))
	{
		_bool bFirstSection = false;

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.1,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.2))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 50.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_STUNPIN;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.50f + 579);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"Sword4");
				m_pSound->MyPlaySound(L"Attack4");
				m_pSound->MyPlaySound(L"SpinTurnA");
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}

		
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.1,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.3))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, m_fSpinSpeed);
			bFirstSection = true;
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.3))
		{
			m_fSpinSpeed -= 400.f * fTimeDelta;
		}

		if (!bFirstSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.4,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILL2ANTHREAT]->tMotionInfo.dEndTime * 0.6))
			{
				if (!m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = true;
					m_pSphereCollider->SetScale(7.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 3.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_SPINA;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 1.f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 20.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_STUNPIN;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 2.75f + 637);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					m_pSound->MyPlaySound(L"Sword5");
					m_pSound->MyPlaySound(L"SpinTurnB");
				}
			}
			else
			{
				if (m_bAddCol[COL2])
				{
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
					m_bAddCol[COL2] = false;
				}
			}
		}
	}
	//스파이럴댄스
	if (m_bAction[CWitchBlade::ACTION_SPIRALDANCE] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLCOMBOATTACK))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLCOMBOATTACK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCOMBOATTACK]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLCOMBOATTACK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCOMBOATTACK]->tMotionInfo.dEndTime * 0.7))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 20.f);	

		}
		else
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLCOMBOATTACK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCOMBOATTACK]->tMotionInfo.dEndTime * 0.1,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLCOMBOATTACK]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLCOMBOATTACK]->tMotionInfo.dEndTime * 0.7))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.fJumpForce = 0.5f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_SPIRALDANCE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.89f + 473 * 6);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
			if (m_fSpiralDeley > 0.3f)
			{
				if (m_bAddCol[COL1])
				{
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
					m_bAddCol[COL1] = false;
				}
				m_fSpiralDeley = 0.f;
			}
			m_fSpiralDeley += fTimeDelta;
			m_pSound->MyPlaySound(L"SpiralSwing");		

			if ((m_iSoundCnt % 3 == 2) && m_pSphereCollider->IsHit())
				m_pSound->MyPlaySound(L"Sword1", false, true);

			m_iSoundCnt++;
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
				m_pSound->MyPlaySound(L"Swing2");
				m_pSound->MyStopSound(L"SpiralSwing");
			}
		}
	}
	//다크폴링그레이브
	if (m_bAction[CWitchBlade::ACTION_DARKFALLINGGRAVE] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLDARKGRAVE))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.43))
		{
			m_pSound->MyPlaySound(L"DarkVoice2");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.6))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 20.f);
			m_pSound->MyPlaySound(L"DarkWind1");
			m_pSound->MyStopSound(L"DarkVoice1");
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.7,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.71))
				Pause_Animation();

			if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.6,
				(*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.7))
			{
				if (!m_bAddCol[COL1])
				{
					m_bAddCol[COL1] = true;
					m_pSphereCollider->SetScale(10.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_STUN;
					tColInfo.fJumpForce = 0.f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 0.f;
					tColInfo.vDir = m_vLook;
					tColInfo.dwSkillID = SKILL_DARKPOLLING;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 6.0f + 657);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 21);
					m_pSound->MyPlaySound(L"Sword1", false, true);
				}
			}
		}



		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.7,
			(*m_pAniInfo)[CWitchBlade::WITCH_SKILLDARKGRAVE]->tMotionInfo.dEndTime * 0.8))
		{
			m_fDarkDelay += fTimeDelta;

			if (!m_bAddCol[COL2])
			{
				m_bAddCol[COL2] = true;
				m_pSphereCollider->SetScale(10.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_BASE;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 0.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_DARKPOLLING;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 6.0f + 657);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
			if (m_iSoundCnt % 3 == 2)
				m_pSound->MyPlaySound(L"Sword1", false, true);

			m_iSoundCnt++;
		}

		if (m_fDarkDelay > 0.4f)
		{
			if (m_bAddCol[COL2])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL2] = false;
			}
			m_fDarkDelay = 0.f;
		}

		m_fDarkGrave += fTimeDelta;

		if (m_fDarkGrave > 1.0f)
		{
			m_fDarkGrave = 0.f;
			Pause_Animation(true);
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
				m_pSound->MyPlaySound(L"DarkWind2");
			}
		}
	}
	//크로스블리츠
	if (m_bAction[CWitchBlade::ACTION_CROSSBLITZ] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_SKILLBUFF))
	{

	}

	//드래곤 댄스
	if (m_bAction[CWitchBlade::ACTION_DRAGONDANCE] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK6))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK6]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK6]->tMotionInfo.dEndTime * 0.3,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK6]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK6]->tMotionInfo.dEndTime))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
		}

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK6]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK6]->tMotionInfo.dEndTime * 0.3,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK6]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK6]->tMotionInfo.dEndTime * 0.4))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(10.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_DRAGONDANCE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 1.37f + 342 * 3);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"SwordSkill3");
				m_pSound->MyPlaySound(L"Attack3");
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL1] = false;
			}
		}

	}
	if (m_bAction[CWitchBlade::ACTION_DRAGONDANCE] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_ATTACK7))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK7]->tMotionInfo.dStartTime,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK7]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK7]->tMotionInfo.dEndTime * 0.5))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 10.f);
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}

		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_ATTACK7]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK7]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CWitchBlade::WITCH_ATTACK7]->tMotionInfo.dStartTime + (*m_pAniInfo)[CWitchBlade::WITCH_ATTACK7]->tMotionInfo.dEndTime * 0.5))
		{
			if (!m_bAddCol[COL2])
			{
				m_bAddCol[COL2] = true;
				m_pSphereCollider->SetScale(10.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_SPINA;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 1.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 20.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_DRAGONDANCE;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 1.37f + 342);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				m_pSound->MyPlaySound(L"SwordSkill4");
				m_pSound->MyPlaySound(L"Attack4");
			}
		}
		else
		{
			if (m_bAddCol[COL2])
			{
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
				m_bAddCol[COL2] = false;
			}
		}
	}
	

	//bool False
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLFLING) && m_bAction[CWitchBlade::ACTION_FLYDANCE])
	{
		m_bAction[CWitchBlade::ACTION_FLYDANCE] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		Engine::Delete_Collider(L"Player", m_pSphereCollider);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLBASH) && m_bAction[CWitchBlade::ACTION_PLEDGEBLADE])
	{
		m_bAction[CWitchBlade::ACTION_PLEDGEBLADE] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLGROUNDKICKSWO) && m_bAction[CWitchBlade::ACTION_BLACKROSE])
	{
		m_bAction[CWitchBlade::ACTION_BLACKROSE] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLBLADERUNNER) && m_bAction[CWitchBlade::ACTION_VIOLETCROSS])
	{
		m_bAction[CWitchBlade::ACTION_VIOLETCROSS] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILL2ANTHREAT) && m_bAction[CWitchBlade::ACTION_SPINTURNTHRUST])
	{
		m_bAction[CWitchBlade::ACTION_SPINTURNTHRUST] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLCOMBOATTACK) && m_bAction[CWitchBlade::ACTION_SPIRALDANCE])
	{
		m_bAction[CWitchBlade::ACTION_SPIRALDANCE] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		m_iSoundCnt = 0;
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLDARKGRAVE) && m_bAction[CWitchBlade::ACTION_DARKFALLINGGRAVE])
	{
		m_bAction[CWitchBlade::ACTION_DARKFALLINGGRAVE] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		Pause_Animation(true);
		m_fDarkGrave = 0.f;
		m_fDarkDelay = 0.f;

		if (m_bAddCol[COL2])
		{
			Engine::Delete_Collider(L"Player", m_pSphereCollider);
			m_bAddCol[COL2] = false;
		}
		m_iSoundCnt = 0;
	}
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_SKILLBUFF) && m_bAction[CWitchBlade::ACTION_CROSSBLITZ])
	{
		m_bAction[CWitchBlade::ACTION_CROSSBLITZ] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_DRAGONDANCE] && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_ATTACK6) && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_ATTACK7))
	{
		m_bAction[CWitchBlade::ACTION_DRAGONDANCE] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
}

void CWitch_Fight::DemageCheck(const _float & fTimeDelta)
{
	if (Engine::IsCollision(L"Monster", &m_tColInfo, m_pCollider) 
		&& m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DOWN)
	{
		_vec3 vDir;

		// for UI Effect
		m_pPlayerInfo->m_bHit = true;

		const _vec3 vPlayer = m_pPlayerInfo->m_vLook;
		const _vec3 vPlayerRight = m_pPlayerInfo->m_vRight;
		const _vec3 vMonster = m_tColInfo.vDir * -1.f;

		_float fCos = acosf(D3DXVec3Dot(&vPlayer, &vMonster));

		float fSourAngle = D3DXVec3Dot(&vPlayerRight, &vMonster);

		_float fDegree = D3DXToDegree(fCos);
		if (fSourAngle >= 0)
			m_pPlayerInfo->m_fHitAngle = -fDegree;
		else 
			m_pPlayerInfo->m_fHitAngle = fDegree;

		m_pPlayerInfo->m_fHitDam = _float(m_tColInfo.iDamage);
		/*if (m_pPlayerInfo->m_fHitAngle > 180.f)
			m_pPlayerInfo->m_fHitAngle = 360.f - m_pPlayerInfo->m_fHitAngle;*/
		//
		m_pPlayerInfo->m_iHp -= m_tColInfo.iDamage;

		if (m_pPlayerInfo->m_iHp < 0)
			m_pPlayerInfo->m_iHp = m_pPlayerInfo->m_iMaxHp;

		if (m_tColInfo.eColDir == Engine::DIR_DEST)
			vDir = m_tColInfo.vDest;
		else
			vDir = m_tColInfo.vDir;

		switch (m_tColInfo.eColInfo)
		{
		case Engine::COL_BASE:
			//오른쪽
			if (m_tColInfo.vDest.x < 0 && fabs(m_tColInfo.vDest.z) < 5.f)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DAMAGER))
				{
					m_bAction[CWitchBlade::ACTION_DAMAGER] = true;
					m_bCanMove = false;
					m_pSound->MyPlaySound(L"Damaged2");
				}
			}
			//왼쪽
			else if (m_tColInfo.vDest.x > 0 && fabs(m_tColInfo.vDest.z) < 5.f)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DAMAGEL))
				{
					m_bAction[CWitchBlade::ACTION_DAMAGEL] = true;
					m_bCanMove = false;
					m_pSound->MyPlaySound(L"Damaged2");
				}
			}
			//아래쪽
			else if (m_tColInfo.vDest.z > 0)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DAMAGEB))
				{
					m_bAction[CWitchBlade::ACTION_DAMAGEB] = true;
					m_bCanMove = false;
					m_pSound->MyPlaySound(L"Damaged2");
				}
			}
			//위쪽
			else if (m_tColInfo.vDest.z < 0)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DAMAGEF))
				{
					m_bAction[CWitchBlade::ACTION_DAMAGEF] = true;
					m_bCanMove = false;
					m_pSound->MyPlaySound(L"Damaged2");
				}
			}
			break;

		case Engine::COL_PUSH:
			//오른쪽
			if (m_tColInfo.vDest.x < 0 && fabs(m_tColInfo.vDest.z) < 5.f)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DAMAGER))
				{
					m_bAction[CWitchBlade::ACTION_DAMAGER] = true;
					m_bCanMove = false;
					m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
					m_pSound->MyPlaySound(L"Damaged2");
				}
			}
			//왼쪽
			else if (m_tColInfo.vDest.x > 0 && fabs(m_tColInfo.vDest.z) < 5.f)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DAMAGEL))
				{
					m_bAction[CWitchBlade::ACTION_DAMAGEL] = true;
					m_bCanMove = false;
					m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
					m_pSound->MyPlaySound(L"Damaged2");
				}
			}
			//아래쪽
			else if (m_tColInfo.vDest.z > 0)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DAMAGEB))
				{
					m_bAction[CWitchBlade::ACTION_DAMAGEB] = true;
					m_bCanMove = false;
					m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
					m_pSound->MyPlaySound(L"Damaged2");
				}
			}
			//위쪽
			else if (m_tColInfo.vDest.z < 0)
			{
				if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DAMAGEF))
				{
					m_bAction[CWitchBlade::ACTION_DAMAGEF] = true;
					m_bCanMove = false;
					m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
					m_pSound->MyPlaySound(L"Damaged2");
				}
			}
			break;

		case Engine::COL_SMASH:
			break;

		case Engine::COL_FLY:
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DHFA))
			{
				m_bCanMove = false;
				m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_tColInfo.fJumpForce);
				m_pSound->MyPlaySound(L"Damaged1");
			}
			break;

		case Engine::COL_SPINA:
			break;

		case Engine::COL_SPINB:
			break;

		case Engine::COL_STAMP:
			break;

		case Engine::COL_CAPTURE:
			break;

		case Engine::COL_STUN:
			break;
		}
	}
	if (m_bAction[CWitchBlade::ACTION_DAMAGEF] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_DAMAGEF))
	{
		if(IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_DAMAGEF]->tMotionInfo.dEndTime * 0.8,
			(*m_pAniInfo)[CWitchBlade::WITCH_DAMAGEF]->tMotionInfo.dEndTime))
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CWitchBlade::ACTION_DAMAGER] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_DAMAGER))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_DAMAGER]->tMotionInfo.dEndTime * 0.8,
			(*m_pAniInfo)[CWitchBlade::WITCH_DAMAGER]->tMotionInfo.dEndTime))
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	if (m_bAction[CWitchBlade::ACTION_DAMAGEB] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_DAMAGEB))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_DAMAGEB]->tMotionInfo.dEndTime * 0.8,
			(*m_pAniInfo)[CWitchBlade::WITCH_DAMAGEB]->tMotionInfo.dEndTime))
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	if (m_bAction[CWitchBlade::ACTION_DAMAGEL] && (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_DAMAGEL))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_DAMAGEL]->tMotionInfo.dEndTime * 0.8,
			(*m_pAniInfo)[CWitchBlade::WITCH_DAMAGEL]->tMotionInfo.dEndTime))
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}



	//상태 전이, bool false
	if (m_bAction[CWitchBlade::ACTION_DAMAGEF] && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DAMAGEF))
	{
		m_bAction[CWitchBlade::ACTION_DAMAGEF] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	if (m_bAction[CWitchBlade::ACTION_DAMAGER] && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DAMAGER))
	{
		m_bAction[CWitchBlade::ACTION_DAMAGER] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	if (m_bAction[CWitchBlade::ACTION_DAMAGEB] && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DAMAGEB))
	{
		m_bAction[CWitchBlade::ACTION_DAMAGEB] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	if (m_bAction[CWitchBlade::ACTION_DAMAGEL] && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DAMAGEL))
	{
		m_bAction[CWitchBlade::ACTION_DAMAGEL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
}

void CWitch_Fight::CheckCameraAction(const _float& fTimeDelta)
{
	Engine::COLINFO tColInfo;
	if ((Engine::IsCollision(L"ActionStart", &tColInfo, m_pCollider)) && (m_fFadeTime < 2.f))
	{
		m_bCameraIntro = true;
		m_bCameraAction = true;
		m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_ATTACKIDLE);
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);

	
		if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		{
			((CPlayerCamera*)Engine::GetCamera(Engine::CAMERA_PLAYER))->SetPlayBGM(L"Stage1Boss");

			CFade_Manager::GetInstance()->FadeImmidy(2.f, 0.1f, CFade_Manager::FADE_BOSS_INTRO);
			CFade_Manager::GetInstance()->SetFading(true);
		}
		if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
		{
			((CPlayerCamera*)Engine::GetCamera(Engine::CAMERA_PLAYER))->SetPlayBGM(L"Stage2Boss");

			CFade_Manager::GetInstance()->FadeImmidy(2.f, 0.1f, CFade_Manager::FADE_BOSS_INTRO);
			CFade_Manager::GetInstance()->SetFading(true);
			//스테이지2 여기서 페이드인
		}
	}

	if (m_bCameraIntro)
	{
		//페이드아웃대기시간
		if (m_fFadeTime > 2.f)
		{
			m_bCameraIntro = false;
			m_pPlayer->Set_CameraAction(true);
		}

		m_fFadeTime += fTimeDelta;
		return;
	}
}

_int CWitch_Fight::GetDamage(_bool* pCritical)
{
	_int iDamage = 0;
	iDamage = (rand() % (m_pPlayerInfo->m_iTotalMaxAtt - m_pPlayerInfo->m_iTotalMinAtt)) + m_pPlayerInfo->m_iTotalMinAtt;
	_int iRand = iDamage % 100;

	*pCritical = false;

	if (iRand > 70)
	{
		iDamage = iDamage + _int(iDamage * (m_pPlayerInfo->m_fCritical * 0.01f));
		*pCritical = true;
	}
	
	return iDamage;
}

CWitch_Fight* CWitch_Fight::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
	CWitchBlade* pPlayer, _uint iAniIndex, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword)
{
	CWitch_Fight* pInstance = new CWitch_Fight(iAniIndex);

	if (FAILED(pInstance->Ready_State(pMesh, pmapNextMotion, pPlayer, pLeftSword, pRightSword)))
	{
		MSG_BOX("CWitch_Fight Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_Fight::Free(void)
{
	CWitch_State::Free();
}