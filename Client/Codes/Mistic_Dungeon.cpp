#include "stdafx.h"
#include "Mistic_Dungeon.h"

#include "MisticWeapon.h"
#include "Mistic_Fight.h"

CMistic_Dungeon::CMistic_Dungeon(_uint iAniIndex)
	: CMistic_State(iAniIndex)
	, m_vLook(0.f, 0.f, 0.f)
	, m_bCanMove(true)
	, m_bCanJump(true)
{
	ZeroMemory(&m_tColInfo, sizeof(Engine::COLINFO));
}

CMistic_Dungeon::~CMistic_Dungeon(void)
{

}

void CMistic_Dungeon::SetActive(void)
{
	m_pWeapon->SetActive(true);
}

void CMistic_Dungeon::Start_State(void)
{
	m_pWeapon->SetActive(true);
}

void CMistic_Dungeon::Update_State(const _float& fTimeDelta)
{
	if (m_pMesh == NULL)
		return;


	JumpCheck(fTimeDelta);
	MoveCheck(fTimeDelta);
	DamageCheck(fTimeDelta);
	ActionCheck();

	CMistic_State::EndCheck();
}

void CMistic_Dungeon::End_State(void)
{
	m_pWeapon->SetActive(false);
	m_iAniIndex = CMistic::MISTIC_IDLE;
	m_bCanMove = true;
}

HRESULT CMistic_Dungeon::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
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

	m_pCollider = (Engine::CCollider*)m_pPlayer->GetComponent<Engine::CCollider>(Engine::COMID_DYNAMIC);
	if (m_pCollider == NULL)
		return E_FAIL;

	m_pPlayerInfo = m_pPlayer->GetPlayerInfo();

	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pCalculator);
	Engine::Safe_Release(m_pCollider);
	return S_OK;
}

void CMistic_Dungeon::ActionCheck(void)
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
		m_pAnimator->Set_AnimationSet(CMistic::MISTIC_IDLE);

	if (iPushCount > 0)
		m_pPlayer->Compute_Camera();

	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_IDLE)
	{
		m_bCanMove = true;
		m_bCanJump = true;
	}

	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) && m_pPlayer->Get_CameraMove())
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_INSTALL))
		{
			//불값 켜줌
			m_bAction[CMistic::ACTION_INSTALL] = true;
			//못움직이게 해줘
			m_bCanMove = false;
		}
	}


	if (m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_INSTALL)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CMistic::MISTIC_INSTALL]->tMotionInfo.dEndTime * 0.4,
			(*m_pAniInfo)[CMistic::MISTIC_INSTALL]->tMotionInfo.dEndTime * 0.5))
		{
			//전투 상태 칼 보이게하고
			CMistic_Fight* pState = (CMistic_Fight*)m_pAnimator->Get_State(PLAYER_FIGHT);
			pState->SetActive();
			//칼숨기고
			m_pWeapon->SetActive(false);
		}
	}


	//칼뽑는 동작이 아닌데 칼 뽑는 동작 불값이 켜저있을때
	if ((m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_INSTALL) && m_bAction[CMistic::ACTION_INSTALL])
	{
		//불값 꺼주고
		m_bAction[CMistic::ACTION_INSTALL] = false;
		//움직일 수 있게 해줘
		m_bCanMove = true;
		//상태 바꿔줘
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}
}

void CMistic_Dungeon::MoveCheck(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)
		m_bAction[CMistic::ACTION_WALKF] = true;
	else
		m_bAction[CMistic::ACTION_WALKF] = false;

	//기본 이동
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		if (m_bAction[CMistic::ACTION_WALKF])
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_WALKF))
				m_bCanMove = true;
		}
		else
		{
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_RUNF))
				m_bCanMove = true;
		}

		m_bAction[CMistic::ACTION_MOVEF] = true;
		//앞으로
		if (m_bAction[CMistic::ACTION_MOVEF] && m_bCanMove)
		{
			if (!m_pCalculator->Wall_Collision_Check(&m_vLook))
			{
				//걷기
				if (m_bAction[CMistic::ACTION_WALKF])
					m_pTransform->Move_Forward(m_pPlayerInfo->m_fMoveSpeed / 3.f, fTimeDelta);
				//뛰기
				else
					m_pTransform->Move_Forward(m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
			}
		}

	}
	else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_WALKB))
			m_bCanMove = true;
		m_bAction[CMistic::ACTION_MOVEB] = true;

		_vec3 vDir = m_vLook * -1.f;
		if (!m_pCalculator->Wall_Collision_Check(&vDir))
		{
			//뒤로걷기
			if (m_bAction[CMistic::ACTION_MOVEB] && m_bCanMove)
				m_pTransform->Move_Forward(-m_pPlayerInfo->m_fMoveSpeed / 3.f, fTimeDelta);
		}
	}
	else if ((Engine::Get_DIKeyState(DIK_A) & 0x80) && !m_bAction[CMistic::ACTION_MOVEF] && !m_bAction[CMistic::ACTION_MOVEB])
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_RUNL))
			m_bCanMove = true;
		m_bAction[CMistic::ACTION_MOVEL] = true;
		//왼쪽
		if (m_bAction[CMistic::ACTION_MOVEL] && m_bCanMove)
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &m_vLook, &_vec3(0.0f, 1.0f, 0.0f));
			if (!m_pCalculator->Wall_Collision_Check(&vDirection))
				m_pTransform->Move_Left(&m_vLook, m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
		}
	}
	else if ((Engine::Get_DIKeyState(DIK_D) & 0x80) && !m_bAction[CMistic::ACTION_MOVEF] && !m_bAction[CMistic::ACTION_MOVEB])
	{
		if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_RUNR))
			m_bCanMove = true;
		m_bAction[CMistic::ACTION_MOVER] = true;
		//오른쪽
		if (m_bAction[CMistic::ACTION_MOVER] && m_bCanMove)
		{
			_vec3			vDirection;
			D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), &m_vLook);
			if (!m_pCalculator->Wall_Collision_Check(&vDirection))
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

void CMistic_Dungeon::JumpCheck(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		_int iAniIndex = 0;
		iAniIndex = CMistic::MISTIC_JUMPFA;

		if (m_bAction[CMistic::ACTION_MOVEF])
			iAniIndex = CMistic::MISTIC_JUMPFA;
		else if (m_bAction[CMistic::ACTION_MOVEB])
			iAniIndex = CMistic::MISTIC_JUMPBA;
		else if (m_bAction[CMistic::ACTION_MOVEL])
			iAniIndex = CMistic::MISTIC_JUMPLA;
		else if (m_bAction[CMistic::ACTION_MOVER])
			iAniIndex = CMistic::MISTIC_JUMPRA;


		if (m_pAnimator->Set_AnimationSet(iAniIndex))
		{
			m_bCanMove = false;
			m_bCanJump = true;
		}
	}

	if ((m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_JUMPFB
		|| m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_JUMPBB
		|| m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_JUMPRB
		|| m_pAnimator->Get_AnimationIndex() == CMistic::MISTIC_JUMPLB) && m_bCanJump)
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
		case CMistic::MISTIC_JUMPFC:
			iAniIndex = CMistic::MISTIC_JUMPFD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			break;

		case CMistic::MISTIC_JUMPBC:
			iAniIndex = CMistic::MISTIC_JUMPBD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			break;

		case CMistic::MISTIC_JUMPLC:
			iAniIndex = CMistic::MISTIC_JUMPLD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			break;

		case CMistic::MISTIC_JUMPRC:
			iAniIndex = CMistic::MISTIC_JUMPRD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			break;
		}
	}
}

void CMistic_Dungeon::DamageCheck(const _float & fTimeDelta)
{
	if (Engine::IsCollision(L"Monster", &m_tColInfo, m_pCollider))
	{
		//전투 상태 칼 보이게하고
		CMistic_Fight* pState = (CMistic_Fight*)m_pAnimator->Get_State(PLAYER_FIGHT);
		pState->SetActive();
		//칼숨기고
		m_pWeapon->SetActive(false);
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
			m_pAnimator->Set_State(PLAYER_FIGHT);
			if (m_pAnimator->Set_AnimationSet(CMistic::MISTIC_DOWNA))
			{
				CMistic_Fight* pFight = (CMistic_Fight*)m_pAnimator->Get_State(PLAYER_FIGHT);
				pFight->Set_Move(false);
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
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}

	if (m_bAction[CMistic::ACTION_DAMAGER] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DAMAGER))
	{
		m_bAction[CMistic::ACTION_DAMAGER] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}

	if (m_bAction[CMistic::ACTION_DAMAGEB] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DAMAGEB))
	{
		m_bAction[CMistic::ACTION_DAMAGEB] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}

	if (m_bAction[CMistic::ACTION_DAMAGEL] && (m_pAnimator->Get_AnimationIndex() != CMistic::MISTIC_DAMAGEL))
	{
		m_bAction[CMistic::ACTION_DAMAGEL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}
}

CMistic_Dungeon* CMistic_Dungeon::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
	CMistic* pPlayer, _uint iAniIndex, CMisticWeapon* pWeapon)
{
	CMistic_Dungeon* pInstance = new CMistic_Dungeon(iAniIndex);

	if (FAILED(pInstance->Ready_State(pMesh, pmapNextMotion, pPlayer, pWeapon)))
	{
		MSG_BOX("CMistic_Dungeon Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMistic_Dungeon::Free(void)
{
	CMistic_State::Free();
}