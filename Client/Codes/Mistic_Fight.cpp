#include "stdafx.h"
#include "Mistic_Fight.h"

#include "Camera.h"
#include "Mistic_Dungeon.h"

#include "MisticWeapon.h"

CMistic_Fight::CMistic_Fight(_uint iAniIndex)
	: CMistic_State(iAniIndex)
	, m_vLook(0.f, 0.f, 0.f)
	, m_bCanMove(false)
	, m_bCanJump(true)
	, m_fGravityTime(0.f)
	, m_fFireTime(0.f)
	, m_fSpinTime(0.f)
	, m_fLockTime(0.f)
	, m_fGailTime(0.f)
	, m_iSpinDelay(0)
	, m_iRisingDelay(0)
{
	ZeroMemory(&m_tColInfo, sizeof(Engine::COLINFO));
	ZeroMemory(&m_bAddCol, sizeof(_bool) * COL_END);
}

CMistic_Fight::~CMistic_Fight(void)
{

}

void CMistic_Fight::SetActive(void)
{
	m_pWeapon->SetActive(true);
}

void CMistic_Fight::Set_Skill(const SKILL_ID eID)
{
	switch (eID)
	{
	case SKILL_RUSH:
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DISARM))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_RUSH] = true;
		}
		break;

	case SKILL_UPPERKICK:
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_UPPERKICK1))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_UPPERKICK] = true;
		}
		break;

	case SKILL_SPINCRASH:
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_SPINCRUSHA))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_SPINCRUSH] = true;
		}
		break;

	case SKILL_FIREM:
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_FIRESHOTREADY))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_FIRESHOT] = true;
		}
		break;

	case SKILL_SHOTJUMP:
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_SHOOTJUMPREADY))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_SHOOTINGJUMP] = true;
		}
		break;

	case SKILL_GRAVITY:
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_GRAVITYSHOTREADY))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_GRAVITYSHOT] = true;
		}
		break;

	case SKILL_LOCKON:
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_LOCKONTARGETREADY))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_LOCKONTARGETSHOT] = true;
		}
		break;
	}
}

void CMistic_Fight::Set_Move(const _bool & bMove)
{
	m_bCanMove = bMove;
}

void CMistic_Fight::Start_State(void)
{
	m_pWeapon->SetActive(true);
	m_pAnimator->Set_AnimationSet(CMistic::MISTIC_BATTLEIDLE);
}

void CMistic_Fight::Update_State(const _float& fTimeDelta)
{
	if (m_pMesh == NULL)
		return;

	ActionCheck();
	JumpCheck(fTimeDelta);
	MoveCheck(fTimeDelta);
	AttackCheck(fTimeDelta);
	CommandSkillCheck(fTimeDelta);
	ActionSkillCheck(fTimeDelta);
	DemageCheck(fTimeDelta);


	CMistic_State::EndCheck();
}

void CMistic_Fight::End_State(void)
{
	m_pWeapon->SetActive(false);
	m_iAniIndex = CMistic::MISTIC_BATTLEIDLE;
}

HRESULT CMistic_Fight::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
	CMistic* pPlayer, CMisticWeapon* pWeapon)
{
	if (pMesh == NULL || pWeapon == NULL || pPlayer == NULL)
		return E_FAIL;
	m_pMesh = pMesh;
	m_pMesh->AddRef();
	m_pmapNextMotion = pmapNextMotion;
	m_pWeapon = pWeapon;
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

	m_pPlayerInfo = m_pPlayer->GetPlayerInfo();

	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pCollider);
	Engine::Safe_Release(m_pSphereCollider);
	return S_OK;
}

void CMistic_Fight::ActionCheck(void)
{
	m_pTransform->Get_Infomation(Engine::INFO_LOOK, &m_vLook);
	m_vLook.y = 0.f;
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	_uint iPushCount = 0;
	for (_uint i = 0; i < CMistic::ACTION_END; i++)
	{
		if (m_bAction[i])
			iPushCount++;
	}

	if (iPushCount == 0)
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_BATTLEIDLE))
			m_bAction[CMistic::ACTION_JUMP] = false;
	}

	if (iPushCount > 0)
		m_pPlayer->Compute_Camera();

	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_BATTLEIDLE)
	{
		m_bCanMove = true;
		m_bCanJump = true;
	}
	
	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DOWNA))
		{
			m_bCanMove = false;
			m_pCalculator->Set_Jump(true);
			m_pRigidbody->Set_Jump(0.7f);
		}
	}
}

void CMistic_Fight::MoveCheck(const _float & fTimeDelta)
{
	//기본 이동
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_RUNF_IN))
			m_bCanMove = true;

		m_bAction[CMistic::ACTION_MOVEF] = true;
		//앞으로
		if (m_bAction[CMistic::ACTION_MOVEF] && m_bCanMove)
		{
			if (!m_pCalculator->Wall_Collision_Check(&m_vLook)/* && !Engine::IsCol_ForOBB(L"Monster", m_pCollider)*/)
				m_pTransform->Move_Forward(m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
		}
	}
	else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_RUNB_IN))
			m_bCanMove = true;
		m_bAction[CMistic::ACTION_MOVEB] = true;

		_vec3 vDir = m_vLook * -1.f;
		if (!m_pCalculator->Wall_Collision_Check(&vDir)/* && !Engine::IsCol_ForOBB(L"Monster", m_pCollider)*/)
		{
			//뒤로걷기
			if (m_bAction[CMistic::ACTION_MOVEB] && m_bCanMove)
				m_pTransform->Move_Forward(-m_pPlayerInfo->m_fMoveSpeed / 3.f, fTimeDelta);
		}
	}
	else if ((Engine::Get_DIKeyState(DIK_A) & 0x80) && !m_bAction[CMistic::ACTION_MOVEF] && !m_bAction[CMistic::ACTION_MOVEB])
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_RUNL_IN))
			m_bCanMove = true;
		m_bAction[CMistic::ACTION_MOVEL] = true;
		//왼쪽
		if (m_bAction[CMistic::ACTION_MOVEL] && m_bCanMove)
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
			if (!m_pCalculator->Wall_Collision_Check(&vDirection)/* && !Engine::IsCol_ForOBB(L"Monster", m_pCollider)*/)
				m_pTransform->Move_Left(&m_vLook, m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
		}
	}
	else if ((Engine::Get_DIKeyState(DIK_D) & 0x80) && !m_bAction[CMistic::ACTION_MOVEF] && !m_bAction[CMistic::ACTION_MOVEB])
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_RUNR_IN))
			m_bCanMove = true;
		m_bAction[CMistic::ACTION_MOVER] = true;
		//오른쪽
		if (m_bAction[CMistic::ACTION_MOVER] && m_bCanMove)
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
			if (!m_pCalculator->Wall_Collision_Check(&vDirection)/* && !Engine::IsCol_ForOBB(L"Monster", m_pCollider)*/)
				m_pTransform->Move_Right(&m_vLook, m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
		}
	}

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			//왼쪽위
			if (m_bAction[CMistic::ACTION_MOVEF] && !m_bAction[CMistic::ACTION_MOVER])
				m_pTransform->Rotation(Engine::ROT_Y, -45.f);
		}
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			//오른쪽위
			if (m_bAction[CMistic::ACTION_MOVEF] && !m_bAction[CMistic::ACTION_MOVEL])
				m_pTransform->Rotation(Engine::ROT_Y, 45.f);
		}
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			//왼쪽아래
			if (m_bAction[CMistic::ACTION_MOVEB] && !m_bAction[CMistic::ACTION_MOVER])
				m_pTransform->Rotation(Engine::ROT_Y, 45.f);
		}
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			//오른쪽아래
			if (m_bAction[CMistic::ACTION_MOVEB] && !m_bAction[CMistic::ACTION_MOVEL])
				m_pTransform->Rotation(Engine::ROT_Y, -45.f);
		}
	}

	//키가 때엇을때
	if (!(Engine::Get_DIKeyState(DIK_W) & 0x80) && m_bAction[CMistic::ACTION_MOVEF])
		m_bAction[CMistic::ACTION_MOVEF] = false;
	if (!(Engine::Get_DIKeyState(DIK_S) & 0x80) && m_bAction[CMistic::ACTION_MOVEB])
		m_bAction[CMistic::ACTION_MOVEB] = false;
	if (!(Engine::Get_DIKeyState(DIK_A) & 0x80) && m_bAction[CMistic::ACTION_MOVEL])
		m_bAction[CMistic::ACTION_MOVEL] = false;
	if (!(Engine::Get_DIKeyState(DIK_D) & 0x80) && m_bAction[CMistic::ACTION_MOVER])
		m_bAction[CMistic::ACTION_MOVER] = false;
}

void CMistic_Fight::JumpCheck(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		_int iAniIndex = 0;
		iAniIndex = CMistic::MISTIC_JUMPFA_IN;

		if (m_bAction[CMistic::ACTION_MOVEF])
			iAniIndex = CMistic::MISTIC_JUMPFA_IN;
		else if (m_bAction[CMistic::ACTION_MOVEB])
			iAniIndex = CMistic::MISTIC_JUMPBA_IN;
		else if (m_bAction[CMistic::ACTION_MOVEL])
			iAniIndex = CMistic::MISTIC_JUMPLA_IN;
		else if (m_bAction[CMistic::ACTION_MOVER])
			iAniIndex = CMistic::MISTIC_JUMPRA_IN;


		if (m_pAnimator->Set_AnimationSet(iAniIndex))
		{
			m_bCanMove = false;
			m_bCanJump = true;
		}
	}

	if ((m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_JUMPFB_IN
		|| m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_JUMPBB_IN
		|| m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_JUMPRB_IN
		|| m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_JUMPLB_IN) && m_bCanJump)
	{
		if (m_bAction[CMistic::ACTION_MOVEF] || m_bAction[CMistic::ACTION_MOVEB] || m_bAction[CMistic::ACTION_MOVER] || m_bAction[CMistic::ACTION_MOVEL])
			m_bCanMove = true;
		m_bCanJump = false;
		m_pCalculator->Set_Jump(true);
		m_pRigidbody->Set_Jump(0.7f);
	}

	if (m_pCalculator->Get_IsCol())
	{
		_int iAniIndex = 0;
		switch (m_pAnimator->Get_AnimationIndex())
		{
		case CMistic::MISTIC_JUMPFC_IN:
			iAniIndex = CMistic::MISTIC_JUMPFD_IN;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			break;

		case CMistic::MISTIC_JUMPBC_IN:
			iAniIndex = CMistic::MISTIC_JUMPBD_IN;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			break;

		case CMistic::MISTIC_JUMPLC_IN:
			iAniIndex = CMistic::MISTIC_JUMPLD_IN;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			break;

		case CMistic::MISTIC_JUMPRC_IN:
			iAniIndex = CMistic::MISTIC_JUMPRD_IN;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			break;

		case CMistic::MISTIC_DOWNB:
			iAniIndex = CMistic::MISTIC_DOWNC;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			break;
		}
	}
}

void CMistic_Fight::AttackCheck(const _float & fTimeDelta)
{
	//Attack1
	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) && (m_pPlayer->Get_CameraMove()))
	{
		//땅에있을때
		if (m_bCanJump)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_ATTACK1))
			{
				m_bAction[CMistic::ACTION_ATTACK1] = true;
				m_bCanMove = false;
			}
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_ATTACK2))
			{
				m_bAction[CMistic::ACTION_ATTACK2] = true;
				m_bCanMove = false;
			}
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_ATTACK3))
			{
				m_bAction[CMistic::ACTION_ATTACK3] = true;
				m_bCanMove = false;
			}
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_ATTACK1)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_ATTACK2]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CMistic::MISTIC_ATTACK2]->tMotionInfo.dEndTime * 0.3))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 2);

				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage;
				Shot_Bullet(tColInfo);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
			}
		}

	}

	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_ATTACK2)
	{
		_bool bFirstSection = false;

		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_ATTACK2]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CMistic::MISTIC_ATTACK2]->tMotionInfo.dEndTime * 0.3))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, -10.f);
			bFirstSection = true;

			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 2);

				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage;
				Shot_Bullet(tColInfo);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
			}
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}
		
		if (!bFirstSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_ATTACK2]->tMotionInfo.dEndTime * 0.5,
				(*m_pAniInfo)[CMistic::MISTIC_ATTACK2]->tMotionInfo.dEndTime * 0.6))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, -10.f);
				if (!m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = true;
					Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 2);

					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_FLY;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 0.2f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 10.f;
					tColInfo.vDir = m_vLook;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage;
					Shot_Bullet(tColInfo);
				}
			}
			else
			{
				if (m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = false;
				}
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			}
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_ATTACK3)
	{
		_bool bFirstSection = false;

		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_ATTACK3]->tMotionInfo.dEndTime * 0.6,
			(*m_pAniInfo)[CMistic::MISTIC_ATTACK3]->tMotionInfo.dEndTime * 0.62))
		{
			_float fSpeed = (*m_pAniInfo)[CMistic::MISTIC_ATTACK3]->tMotionInfo.fSpeed * 0.1f;
			m_pAnimator->Set_TrackSpeed(fSpeed);
		}
		else
		{
			m_pAnimator->Set_TrackSpeed((*m_pAniInfo)[CMistic::MISTIC_ATTACK3]->tMotionInfo.fSpeed);
		}

		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_ATTACK3]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CMistic::MISTIC_ATTACK3]->tMotionInfo.dEndTime * 0.5))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, -10.f);
			bFirstSection = true;
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 2);

				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage;
				Shot_Bullet(tColInfo);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
			}
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}



		if (!bFirstSection)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_ATTACK3]->tMotionInfo.dEndTime * 0.65,
				(*m_pAniInfo)[CMistic::MISTIC_ATTACK3]->tMotionInfo.dEndTime * 0.75))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, -20.f);
				if (!m_bAddCol[COL3])
				{
					m_bAddCol[COL3] = true;
					Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 5);

					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_FLY;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 0.2f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 10.f;
					tColInfo.vDir = m_vLook;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage;
					Shot_Bullet(tColInfo);
				}
			}
			else
			{
				if (m_bAddCol[COL3])
				{
					m_bAddCol[COL3] = false;
				}
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			}
		}
	}



	//bool 해제
	if (m_bAction[CMistic::ACTION_ATTACK1] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_ATTACK1))
	{
		m_bAction[CMistic::ACTION_ATTACK1] = false;
	}

	if (m_bAction[CMistic::ACTION_ATTACK2] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_ATTACK2))
	{
		m_bAction[CMistic::ACTION_ATTACK2] = false;
	}

	if (m_bAction[CMistic::ACTION_ATTACK3] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_ATTACK3))
	{
		m_bAction[CMistic::ACTION_ATTACK3] = false;
	}
}

void CMistic_Fight::CommandSkillCheck(const _float & fTimeDelta)
{
	//대쉬
	if ((Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80) && m_bCanJump)
	{
		//위
		if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DASHF))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_DASHF] = true;
				m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
			}
		}
		//아래
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DASHB))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_DASHB] = true;
				m_pRigidbody->Set_AccMotion(&m_vLook, -m_pPlayerInfo->m_fMoveSpeed);
			}
		}
		//왼쪽
		else if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DASHL))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_DASHL] = true;
				_vec3			vDirection;
				D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
				m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
			}
		}
		//오른쪽
		else if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DASHR))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_DASHR] = true;
				_vec3			vDirection;
				D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
				m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
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
	if (m_bAction[CMistic::ACTION_DASHF])
	{
		if (m_pAnimator->Is_AnimationEnd(CMistic::MISTIC_DASHF, (*m_pAniInfo)[CMistic::MISTIC_DASHF]->tMotionInfo.dEndTime / 3.0))
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CMistic::ACTION_DASHB])
	{
		if (m_pAnimator->Is_AnimationEnd(CMistic::MISTIC_DASHB, (*m_pAniInfo)[CMistic::MISTIC_DASHB]->tMotionInfo.dEndTime / 3.0))
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CMistic::ACTION_DASHL])
	{
		if (m_pAnimator->Is_AnimationEnd(CMistic::MISTIC_DASHL, (*m_pAniInfo)[CMistic::MISTIC_DASHL]->tMotionInfo.dEndTime / 3.0))
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if (m_bAction[CMistic::ACTION_DASHR])
	{
		if (m_pAnimator->Is_AnimationEnd(CMistic::MISTIC_DASHR, (*m_pAniInfo)[CMistic::MISTIC_DASHR]->tMotionInfo.dEndTime / 3.0))
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}


	
	if (Engine::Get_DIKeyState(DIK_F) & 0x80)
	{
		//다운킥
		if (m_bCanJump)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DOWNKICK))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_DOWNKICK] = true;
			}
		}
		//점프다운킥
		else
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_JUMPDOWNKICK))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_JUMPDOWNKICK] = true;
			}
		}
	}

	//다운킥 충돌
	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_DOWNKICK)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_DOWNKICK]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CMistic::MISTIC_DOWNKICK]->tMotionInfo.dEndTime * 0.3))
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
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				tColInfo.dwSkillID = SKILL_LIDDLEKICK;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.10f + 65);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 2);
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

	//쉘버스터
	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_UPPERKICK1)
	{
		if ((Engine::Get_DIKeyState(DIK_S) & 0x80) && Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB))
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_UPPERKICK2))
			{
				m_bAction[CMistic::ACTION_SHELBUSTER] = true;
				m_bCanMove = false;
			}
		}
	}



	//홀딩샷
	if (Engine::Get_DIKeyState(DIK_E) & 0x80)
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_HOLDINGSHOTFAIL))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_HOLDINGSHOT] = true;
		}
	}

	//이스케이프
	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB))
	{
		//왼쪽
		if (Engine::Get_DIKeyState(DIK_A) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_ESCAPEL))
			{
				m_bAction[CMistic::ACTION_ESCAPEL] = true;
				m_bCanMove = false;	
			}
		}
		//오른쪽
		if (Engine::Get_DIKeyState(DIK_D) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_ESCAPER))
			{
				m_bAction[CMistic::ACTION_ESCAPER] = true;
				m_bCanMove = false;
			}
		}
	}

	//이스케이프 이동
	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_ESCAPEL && m_bAction[CMistic::ACTION_ESCAPEL])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_ESCAPEL]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CMistic::MISTIC_ESCAPEL]->tMotionInfo.dEndTime * 0.6))
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
			m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed * 1.5f);

			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 5);
			}

		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);

			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
			}
		}
	}
	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_ESCAPER && m_bAction[CMistic::ACTION_ESCAPER])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_ESCAPER]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CMistic::MISTIC_ESCAPER]->tMotionInfo.dEndTime * 0.6))
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
			m_pRigidbody->Set_AccMotion(&vDirection, m_pPlayerInfo->m_fMoveSpeed * 1.5f);

			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 5);
			}
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);

			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
			}
		}
	}

	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_DOWN)
	{
		//테크니컬다운액션
		if (Engine::Get_DIKeyState(DIK_W) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_QUICKSTANDF))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_TECHNICALDOWNF] = true;
			}
		}
		else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_QUICKSTANDB))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_TECHNICALDOWNAB] = true;
			}
		}
		else if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_STANDUP))
			{
				m_bCanMove = false;
			}
		}
		//기상공격
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) && m_pPlayer->Get_CameraMove())
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_RISINGATTACK))
			{
				m_bCanMove = false;
				m_bAction[CMistic::ACTION_RISINGATTACK] = true;
			}
		}
	}

	//기상공격
	if (m_bAction[CMistic::ACTION_RISINGATTACK] && m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_RISINGATTACK)
	{

		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_RISINGATTACK]->tMotionInfo.dStartTime, 
			(*m_pAniInfo)[CMistic::MISTIC_RISINGATTACK]->tMotionInfo.dEndTime * 0.6))
		{
			if (m_iRisingDelay % 10 == 9)
			{
				if (m_bAddCol[COL1])
				{
					m_bAddCol[COL1] = false;
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
				}
			}


			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = -2.f;
				tColInfo.vDir = m_vLook;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage;
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}

			m_iRisingDelay++;
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}

			if (!m_bAddCol[COL2])
			{
				m_bAddCol[COL2] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_DEST;
				tColInfo.fJumpForce = 0.5f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage;
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
			}
		}
	}

	//테크니컬 다운액션 이동
	if (m_bAction[CMistic::ACTION_TECHNICALDOWNF] && (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_QUICKSTANDF))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_QUICKSTANDF]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CMistic::MISTIC_QUICKSTANDF]->tMotionInfo.dEndTime * 0.5))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed * 1.5f);
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}
	}

	if (m_bAction[CMistic::ACTION_TECHNICALDOWNAB] && (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_QUICKSTANDB))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_QUICKSTANDB]->tMotionInfo.dEndTime * 0.2,
			(*m_pAniInfo)[CMistic::MISTIC_QUICKSTANDB]->tMotionInfo.dEndTime * 0.7))
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, -m_pPlayerInfo->m_fMoveSpeed);
		}
		else
		{
			m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		}
	}


	//게일샷
	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) && m_pPlayer->Get_CameraMove())
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_GAILSHOTREADY))
		{
			m_bCanMove = false;
			m_bAction[CMistic::ACTION_GAILSHOT] = true;
		}
	}

	//게일샷 세부로직
	if (m_bAction[CMistic::ACTION_GAILSHOT])
	{
		m_fGailTime += fTimeDelta;


		if (m_fGailTime > 2.f)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_GAILSHOTEND))
			{
				m_fGailTime = 0.f;
			}
		}
	}




	//bool 해제
	//대쉬
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DASHF) && (m_bAction[CMistic::ACTION_DASHF]))
	{
		m_bAction[CMistic::ACTION_DASHF] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DASHB) && (m_bAction[CMistic::ACTION_DASHB]))
	{
		m_bAction[CMistic::ACTION_DASHB] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DASHR) && (m_bAction[CMistic::ACTION_DASHR]))
	{
		m_bAction[CMistic::ACTION_DASHR] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DASHL) && (m_bAction[CMistic::ACTION_DASHL]))
	{
		m_bAction[CMistic::ACTION_DASHL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	//다운킥
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DOWNKICK) && (m_bAction[CMistic::ACTION_DOWNKICK]))
		m_bAction[CMistic::ACTION_DOWNKICK] = false;
	//점프다운킥
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_JUMPDOWNKICK) && (m_bAction[CMistic::ACTION_JUMPDOWNKICK]))
	{
		m_bAction[CMistic::ACTION_JUMPDOWNKICK] = false;
	}

	//쉘버스터
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_UPPERKICK2) && (m_bAction[CMistic::ACTION_SHELBUSTER]))
	{
		m_bAction[CMistic::ACTION_SHELBUSTER] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//홀딩샷
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_HOLDINGSHOTFAIL) && (m_bAction[CMistic::ACTION_HOLDINGSHOT]))
	{
		m_bAction[CMistic::ACTION_HOLDINGSHOT] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//이스케이프
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_ESCAPEL) && (m_bAction[CMistic::ACTION_ESCAPEL]))
	{
		m_bAction[CMistic::ACTION_ESCAPEL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_ESCAPER) && (m_bAction[CMistic::ACTION_ESCAPER]))
	{
		m_bAction[CMistic::ACTION_ESCAPER] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//테크니컬 다운액션
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_QUICKSTANDF) && (m_bAction[CMistic::ACTION_TECHNICALDOWNF]))
	{
		m_bAction[CMistic::ACTION_TECHNICALDOWNF] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_QUICKSTANDB) && (m_bAction[CMistic::ACTION_TECHNICALDOWNAB]))
	{
		m_bAction[CMistic::ACTION_TECHNICALDOWNAB] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//기상공격
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_RISINGATTACK) && (m_bAction[CMistic::ACTION_RISINGATTACK]))
	{
		m_bAction[CMistic::ACTION_RISINGATTACK] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		m_iRisingDelay = 0;

		if (m_bAddCol[COL2])
		{
			m_bAddCol[COL2] = false;
			Engine::Delete_Collider(L"Player", m_pSphereCollider);
		}
	}

	//게일샷
	if (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_GAILSHOTEND
		&& m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_GAILSHOTLOOP
		&& m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_GAILSHOTREADY
		&& m_bAction[CMistic::ACTION_GAILSHOT])
	{
		m_bAction[CMistic::ACTION_GAILSHOT] = false;
		m_fGailTime = 0.f;
	}
}

void CMistic_Fight::ActionSkillCheck(const _float & fTimeDelta)
{
	//러쉬
	if (m_bAction[CMistic::ACTION_RUSH] && (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_DISARM))
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_DISARM]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CMistic::MISTIC_DISARM]->tMotionInfo.dEndTime * 0.5))
		{
			m_pWeapon->SetActive(false);
			CMistic_Dungeon* pState = (CMistic_Dungeon*)m_pAnimator->Get_State(PLAYER_DUNGEON);
			pState->SetActive();
		}
	}

	//어퍼킥
	if (m_bAction[CMistic::ACTION_UPPERKICK])
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_UPPERKICK1]->tMotionInfo.dEndTime * 0.5,
			(*m_pAniInfo)[CMistic::MISTIC_UPPERKICK1]->tMotionInfo.dEndTime * 0.6))
		{
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 1.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 0.f;
				tColInfo.vDir = m_vLook;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.89f + 473 * 6);
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

	//스핀크러쉬
	if (m_bAction[CMistic::ACTION_SPINCRUSH])
	{
		if (m_fSpinTime > 1.f)
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DASHF))
			{
				m_fSpinTime = 0.f;
				if (!m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = true;
					m_pSphereCollider->SetScale(7.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_PUSH;
					tColInfo.eColDir = Engine::DIR_LOOK;
					tColInfo.fJumpForce = 0.f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 50.f;
					tColInfo.vDir = m_vLook;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 2.89f + 473 * 6);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
					Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 3);
				}
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

		//스핀크러시 이동
		if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_SPINCRUSHB)
		{
			m_fSpinTime += fTimeDelta;
			m_pRigidbody->Set_AccMotion(&m_vLook, m_pPlayerInfo->m_fMoveSpeed * 1.5f);


			if (m_iSpinDelay % 10 == 9)
			{
				if (m_bAddCol[COL1])
				{
					m_bAddCol[COL1] = false;
					Engine::Delete_Collider(L"Player", m_pSphereCollider);
				}
			}

			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pSphereCollider->SetScale(7.f);
				m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_PUSH;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 30.f;
				tColInfo.vDir = m_vLook;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage + _int(iDamage * 2.89f + 473 * 6);
				m_pSphereCollider->Set_ColInfo(&tColInfo);
				Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 3);
			}

			m_iSpinDelay++;
		}
		else
		{
			m_iSpinDelay = 0;
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Delete_Collider(L"Player", m_pSphereCollider);
			}
		}

		if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_DASHF)
		{
			if (!IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_DASHF]->tMotionInfo.dStartTime,
				(*m_pAniInfo)[CMistic::MISTIC_DASHF]->tMotionInfo.dStartTime + (*m_pAniInfo)[CMistic::MISTIC_DASHF]->tMotionInfo.dEndTime * 0.8))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			}
		}
	}



	//화염방사기	
	if (m_bAction[CMistic::ACTION_FIRESHOT])
	{
		if (!Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_FIRESHOTEND))
			{
				m_fFireTime = 0.f;
			}
		}
		else
		{
			if (m_fFireTime > 2.f)
			{
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_FIRESHOTEND))
				{
					m_fFireTime = 0.f;
				}
			}
		}

		if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_FIRESHOTLOOP)
		{
			m_fFireTime += fTimeDelta;
			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				Engine::CCamera* pCamera = Engine::GetCamera(Engine::CAMERA_PLAYER);
				_matrix matCamWorld = pCamera->GetMatView();
				D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
				_vec3 vRight;
				memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));
				vRight = vRight + _vec3(0.f, 1.f, 0.f);
				vRight *= 0.5f;
				Engine::Set_Shake(Engine::CAMERA_PLAYER, vRight, 0.4f, 1000, 3);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 0);
			}
		}

	}
	//슈팅점프
	if (m_bAction[CMistic::ACTION_SHOOTINGJUMP])
	{
		if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_SHOOTJUMPSHOT)
		{

			if (IsPlaying_ThisSection(0, (*m_pAniInfo)[CMistic::MISTIC_SHOOTJUMPSHOT]->tMotionInfo.dEndTime * 0.2))
			{
				if (!m_bAddCol[COL2])
				{
					m_bAddCol[COL2] = true;
					m_pSphereCollider->SetScale(15.f);
					m_pSphereCollider->SetPos(_vec3(0.f, 3.f, 0.f));
					Engine::COLINFO tColInfo;
					tColInfo.eColInfo = Engine::COL_PUSH;
					tColInfo.eColDir = Engine::DIR_DEST;
					tColInfo.fJumpForce = 0.f;
					tColInfo.fStampForce = 0.f;
					tColInfo.fForce = 30.f;
					tColInfo.vDir = m_vLook;
					_int iDamage = GetDamage(&tColInfo.bCritical);
					tColInfo.iDamage = iDamage + _int(iDamage * 2.89f + 473 * 6);
					m_pSphereCollider->Set_ColInfo(&tColInfo);
					Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pSphereCollider);
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


			if (!m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = true;
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(0.7f);
				m_pRigidbody->Set_AccMotion(&m_vLook, -m_pPlayerInfo->m_fMoveSpeed * 1.5f);
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 5);
				m_pRigidbody->Set_RevisionGravity(-30.f);
			}
		}
		else
		{
			if (m_bAddCol[COL1])
			{
				m_bAddCol[COL1] = false;
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			}
		}
	}

	//중력포
	if (m_bAction[CMistic::ACTION_GRAVITYSHOT])
	{
		if ((m_fGravityTime > 2.f) || (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB)))
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_GRAVITYSHOTEND))
			{
				m_fGravityTime = 0.f;
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.4f, 5);
			}
		}

		if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_GRAVITYSHOTLOOP)
		{
			m_fGravityTime += fTimeDelta;
		}

		if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_GRAVITYSHOTEND)
		{
			if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_GRAVITYSHOTEND]->tMotionInfo.dStartTime,
				(*m_pAniInfo)[CMistic::MISTIC_GRAVITYSHOTEND]->tMotionInfo.dEndTime * 0.4))
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, -m_pPlayerInfo->m_fMoveSpeed * 1.5f);
			}
			else
			{
				m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
			}
		}
	}
	//락온타겟슛
	if (m_bAction[CMistic::ACTION_LOCKONTARGETSHOT])
	{
		m_fLockTime += fTimeDelta;
		if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_LOCKONTARGETREADY)
			m_pPlayer->Camera_MoveFoward(fTimeDelta);
		if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_LOCKONTARGETEND)
			m_pPlayer->Camera_MoveReturn(fTimeDelta);

		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_LOCKONTARGETSHOT))
			{
				Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.2f, 3);

				Engine::COLINFO tColInfo;
				tColInfo.eColInfo = Engine::COL_FLY;
				tColInfo.eColDir = Engine::DIR_LOOK;
				tColInfo.fJumpForce = 0.2f;
				tColInfo.fStampForce = 0.f;
				tColInfo.fForce = 10.f;
				tColInfo.vDir = m_vLook;
				_int iDamage = GetDamage(&tColInfo.bCritical);
				tColInfo.iDamage = iDamage;
				Shot_Bullet(tColInfo);
			}
		}

		if ((m_fLockTime > 5.f) || (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB)))
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_LOCKONTARGETEND))
			{
				m_fLockTime = 0.f;
			}
		}	
	}

	//bool 끄기
	//러쉬
	if (m_bAction[CMistic::ACTION_RUSH] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DISARM))
	{
		m_bAction[CMistic::ACTION_RUSH] = false;
		m_bCanMove = true;
		m_pAnimator->Set_State(PLAYER_DUNGEON);
	}
	//어퍼킥
	if (m_bAction[CMistic::ACTION_UPPERKICK] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_UPPERKICK1)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_UPPERKICK1E))
	{
		m_bAction[CMistic::ACTION_UPPERKICK] = false;
	}
	//스핀크러쉬
	if (m_bAction[CMistic::ACTION_SPINCRUSH] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_SPINCRUSHA)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_SPINCRUSHB)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_SPINCRUSHC)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DASHF))
	{
		m_bAction[CMistic::ACTION_SPINCRUSH] = false;
		m_fSpinTime = 0.f;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
	//화염방사기
	if (m_bAction[CMistic::ACTION_FIRESHOT] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_FIRESHOTREADY)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_FIRESHOTLOOP)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_FIRESHOTEND))
	{
		m_bAction[CMistic::ACTION_FIRESHOT] = false;
		m_fFireTime = 0.f;
	}
	//슈팅점프
	if (m_bAction[CMistic::ACTION_SHOOTINGJUMP] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_SHOOTJUMPREADY)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_SHOOTJUMPSHOT)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_SHOOTJUMPEND))
	{
		m_bAction[CMistic::ACTION_SHOOTINGJUMP] = false;
	}
	//중력포
	if (m_bAction[CMistic::ACTION_GRAVITYSHOT] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_GRAVITYSHOTREADY)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_GRAVITYSHOTLOOP)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_GRAVITYSHOTEND))
	{
		m_bAction[CMistic::ACTION_GRAVITYSHOT] = false;
		m_fGravityTime = 0.f;
	}
	//락온타겟슛
	if (m_bAction[CMistic::ACTION_LOCKONTARGETSHOT] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_LOCKONTARGETREADY)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_LOCKONTARGETLOOP)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_LOCKONTARGETSHOT)
		&& (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_LOCKONTARGETEND))
	{
		m_bAction[CMistic::ACTION_LOCKONTARGETSHOT] = false;
		m_fLockTime = 0.f;
	}
}

void CMistic_Fight::DemageCheck(const _float & fTimeDelta)
{
	if (Engine::IsCollision(L"Monster", &m_tColInfo, m_pCollider))
	{
		_vec3 vDir;

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
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DAMAGER))
				{
					m_bAction[CMistic::ACTION_DAMAGER] = true;
					m_bCanMove = false;
				}
			}
			//왼쪽
			else if (m_tColInfo.vDest.x > 0 && fabs(m_tColInfo.vDest.z) < 5.f)
			{
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DAMAGEL))
				{
					m_bAction[CMistic::ACTION_DAMAGEL] = true;
					m_bCanMove = false;
				}
			}
			//아래쪽
			else if (m_tColInfo.vDest.z > 0)
			{
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DAMAGEB))
				{
					m_bAction[CMistic::ACTION_DAMAGEB] = true;
					m_bCanMove = false;
				}
			}
			//위쪽
			else if (m_tColInfo.vDest.z < 0)
			{
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DAMAGEF))
				{
					m_bAction[CMistic::ACTION_DAMAGEF] = true;
					m_bCanMove = false;
				}
			}
			break;

		case Engine::COL_PUSH:
			//오른쪽
			if (m_tColInfo.vDest.x < 0 && fabs(m_tColInfo.vDest.z) < 5.f)
			{
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DAMAGER))
				{
					m_bAction[CMistic::ACTION_DAMAGER] = true;
					m_bCanMove = false;
					m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
				}
			}
			//왼쪽
			else if (m_tColInfo.vDest.x > 0 && fabs(m_tColInfo.vDest.z) < 5.f)
			{
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DAMAGEL))
				{
					m_bAction[CMistic::ACTION_DAMAGEL] = true;
					m_bCanMove = false;
					m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
				}
			}
			//아래쪽
			else if (m_tColInfo.vDest.z > 0)
			{
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DAMAGEB))
				{
					m_bAction[CMistic::ACTION_DAMAGEB] = true;
					m_bCanMove = false;
					m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
				}
			}
			//위쪽
			else if (m_tColInfo.vDest.z < 0)
			{
				if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DAMAGEF))
				{
					m_bAction[CMistic::ACTION_DAMAGEF] = true;
					m_bCanMove = false;
					m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
				}
			}
			break;

		case Engine::COL_SMASH:
			break;

		case Engine::COL_FLY:
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DOWNA))
			{
				m_bCanMove = false;
				m_pRigidbody->Set_AccMotion(&vDir, m_tColInfo.fForce);
				m_pCalculator->Set_Jump(true);
				m_pRigidbody->Set_Jump(m_tColInfo.fJumpForce);
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

	//상태 전이, bool false
	if (m_bAction[CMistic::ACTION_DAMAGEF] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DAMAGEF))
	{
		m_bAction[CMistic::ACTION_DAMAGEF] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	if (m_bAction[CMistic::ACTION_DAMAGER] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DAMAGER))
	{
		m_bAction[CMistic::ACTION_DAMAGER] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	if (m_bAction[CMistic::ACTION_DAMAGEB] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DAMAGEB))
	{
		m_bAction[CMistic::ACTION_DAMAGEB] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}

	if (m_bAction[CMistic::ACTION_DAMAGEL] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DAMAGEL))
	{
		m_bAction[CMistic::ACTION_DAMAGEL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
	}
}

_int CMistic_Fight::GetDamage(_bool * pCritical)
{
	_int iDamage = 0;
	iDamage = (rand() % (m_pPlayerInfo->m_iTotalMaxAtt - m_pPlayerInfo->m_iTotalMinAtt)) + m_pPlayerInfo->m_iTotalMinAtt;
	_int iRand = iDamage % 100;

	*pCritical = false;

	if (iRand > 90)
	{
		iDamage = iDamage + _int(iDamage * (m_pPlayerInfo->m_fCritical * 0.01f));
		*pCritical = true;
	}

	return iDamage;
}

void CMistic_Fight::Shot_Bullet(Engine::COLINFO& tColInfo)
{
	_vec3 vPos;
	m_pWeapon->Get_ShotPoint(&vPos);
	_vec3 vDir;
	_matrix matWorld = Engine::GetCamera(Engine::CAMERA_PLAYER)->GetMatView();
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	memcpy(&vDir, &matWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vDir, &vDir);

	if (vDir.y <= 0.f)
		vDir.y = 0.f;

	m_pPlayer->Shot_Bullet(vPos, vDir, tColInfo);
}

CMistic_Fight* CMistic_Fight::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
	CMistic* pPlayer, _uint iAniIndex, CMisticWeapon* pWeapon)
{
	CMistic_Fight* pInstance = new CMistic_Fight(iAniIndex);

	if (FAILED(pInstance->Ready_State(pMesh, pmapNextMotion, pPlayer, pWeapon)))
	{
		MSG_BOX("CMistic_Fight Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMistic_Fight::Free(void)
{
	CMistic_State::Free();
}