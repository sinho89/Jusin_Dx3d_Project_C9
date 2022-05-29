#include "stdafx.h"
#include "Witch_Dungeon.h"

#include "WitchBladeWeapon.h"
#include "Witch_Fight.h"
#include "Sound.h"

CWitch_Dungeon::CWitch_Dungeon(_uint iAniIndex)
: CWitch_State(iAniIndex)
, m_vLook(0.f, 0.f, 0.f)
, m_bCanMove(true)
, m_bCanJump(true)
{
	ZeroMemory(&m_tColInfo, sizeof(Engine::COLINFO));
}

CWitch_Dungeon::~CWitch_Dungeon(void)
{

}

void CWitch_Dungeon::Start_State(void)
{
	m_pLeftSword->SetActive(true);
	m_pRightSword->SetActive(true);
	m_iAniIndex = CWitchBlade::WITCH_IDLE;
}

void CWitch_Dungeon::Update_State(const _float& fTimeDelta)
{
	if (m_pMesh == NULL)
		return;

	
	JumpCheck(fTimeDelta);
	MoveCheck(fTimeDelta);
	DamageCheck(fTimeDelta);
	ActionCheck();

	CWitch_State::EndCheck();
}

void CWitch_Dungeon::End_State(void)
{
	m_pLeftSword->SetActive(false);
	m_pRightSword->SetActive(false);
	m_iAniIndex = CWitchBlade::WITCH_IDLE;
	m_bCanMove = true;
}

HRESULT CWitch_Dungeon::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
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

	m_pCollider = (Engine::CCollider*)m_pPlayer->GetComponent<Engine::CCollider>(Engine::COMID_DYNAMIC);
	if (m_pCollider == NULL)
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
	Engine::Safe_Release(m_pSound);
	return S_OK;
}

void CWitch_Dungeon::ActionCheck(void)
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
		m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_IDLE);

	if (iPushCount > 0)
		m_pPlayer->Compute_Camera();

	if (m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_IDLE)
	{
		m_bCanMove = true;
		m_bCanJump = true;
	}

	//전투상태
	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) && m_pPlayer->Get_CameraMove())
	{
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_INSTALL))
		{
			//전투 상태 칼 보이게하고
			CWitch_Fight* pState = (CWitch_Fight*)m_pAnimator->Get_State(PLAYER_FIGHT);
			pState->SetActive();
			//칼숨기고
			m_pLeftSword->SetActive(false);
			m_pRightSword->SetActive(false);
			//불값 켜줌
			m_bAction[CWitchBlade::ACTION_INSTALL] = true;

			//못움직이게 해줘
			m_bCanMove = false;
			m_pSound->MyPlaySound(L"Install");
			m_pSound->MyPlaySound(L"Jump");
		}
	}
	//칼뽑는 동작이 아닌데 칼 뽑는 동작 불값이 켜저있을때
	if ((m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_INSTALL) && m_bAction[CWitchBlade::ACTION_INSTALL])
	{
		//불값 꺼주고
		m_bAction[CWitchBlade::ACTION_INSTALL] = false;
		//움직일 수 있게 해줘
		m_bCanMove = true;
		//상태 바꿔줘
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}

}

void CWitch_Dungeon::MoveCheck(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_LSHIFT) & 0x80)
		m_bAction[CWitchBlade::ACTION_WALKF] = true;
	else
		m_bAction[CWitchBlade::ACTION_WALKF] = false;

	//기본 이동
	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		if (m_bAction[CWitchBlade::ACTION_WALKF])
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_WALKF))
				m_bCanMove = true;
		}
		else
		{
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_RUNF))
				m_bCanMove = true;
		}

		m_bAction[CWitchBlade::ACTION_MOVEF] = true;
		//앞으로
		if (m_bAction[CWitchBlade::ACTION_MOVEF] && m_bCanMove)
		{
			if (!m_pCalculator->Wall_Collision_Check(&m_vLook)
				&& !Engine::IsCol_ForOBB(L"Monster", m_pCollider, &(m_vLook * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta))
				&& !Engine::IsCol_ForOBB(L"Static", m_pCollider, &(m_vLook * m_pPlayerInfo->m_fMoveSpeed * fTimeDelta)))
			{
				//걷기
				if (m_bAction[CWitchBlade::ACTION_WALKF])
					m_pTransform->Move_Forward(m_pPlayerInfo->m_fMoveSpeed / 3.f, fTimeDelta);
				//뛰기
				else
					m_pTransform->Move_Forward(m_pPlayerInfo->m_fMoveSpeed, fTimeDelta);
			}
		}

	}
	else if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_WALKB))
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
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_RUNL))
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
		if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_RUNR))
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
	if (m_bAction[CWitchBlade::ACTION_MOVEF] && m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_RUNF)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNF]->tMotionInfo.dEndTime * 0.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNF]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"WalkL");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNF]->tMotionInfo.dEndTime * 0.5
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNF]->tMotionInfo.dEndTime * 0.6))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_MOVEB] && m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_WALKB)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_WALKB]->tMotionInfo.dEndTime * 0.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_WALKB]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_WALKB]->tMotionInfo.dEndTime * 0.5
			, (*m_pAniInfo)[CWitchBlade::WITCH_WALKB]->tMotionInfo.dEndTime * 0.6))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_MOVER] && m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_RUNR)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNR]->tMotionInfo.dEndTime * 0.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNR]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"WalkL");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNR]->tMotionInfo.dEndTime * 0.6
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNR]->tMotionInfo.dEndTime * 0.7))
		{
			m_pSound->MyPlaySound(L"WalkR");
		}
	}
	if (m_bAction[CWitchBlade::ACTION_MOVEL] && m_pAnimator->Get_AnimationIndex() == CWitchBlade::WITCH_RUNL)
	{
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNL]->tMotionInfo.dEndTime * 0.0
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNL]->tMotionInfo.dEndTime * 0.1))
		{
			m_pSound->MyPlaySound(L"WalkL");
		}
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_RUNL]->tMotionInfo.dEndTime * 0.6
			, (*m_pAniInfo)[CWitchBlade::WITCH_RUNL]->tMotionInfo.dEndTime * 0.7))
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

void CWitch_Dungeon::JumpCheck(const _float & fTimeDelta)
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
			m_pSound->MyPlaySound(L"WalkL");
			break;

		case CWitchBlade::WITCH_JUMPBC:
			iAniIndex = CWitchBlade::WITCH_JUMPBD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			m_pSound->MyPlaySound(L"WalkL");
			break;

		case CWitchBlade::WITCH_JUMPLC:
			iAniIndex = CWitchBlade::WITCH_JUMPLD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			m_pSound->MyPlaySound(L"WalkL");
			break;

		case CWitchBlade::WITCH_JUMPRC:
			iAniIndex = CWitchBlade::WITCH_JUMPRD;
			m_pAnimator->Set_AnimationSet(iAniIndex);
			m_bCanMove = false;
			m_bCanJump = true;
			m_pSound->MyPlaySound(L"WalkL");
			break;
		}
	}
}

void CWitch_Dungeon::DamageCheck(const _float & fTimeDelta)
{
	if (Engine::IsCollision(L"Monster", &m_tColInfo, m_pCollider))
	{
		//전투 상태 칼 보이게하고
		CWitch_Fight* pState = (CWitch_Fight*)m_pAnimator->Get_State(PLAYER_FIGHT);
		pState->SetActive();
		//칼숨기고
		m_pLeftSword->SetActive(false);
		m_pRightSword->SetActive(false);
		_vec3 vDir;

		m_pPlayerInfo->m_iHp -= m_tColInfo.iDamage;

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
			m_pAnimator->Set_State(PLAYER_FIGHT);
			if (m_pAnimator->Set_AnimationSet(CWitchBlade::WITCH_DHFA))
			{
				CWitch_Fight* pFight = (CWitch_Fight*)m_pAnimator->Get_State(PLAYER_FIGHT);
				pFight->Set_Move(false);
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
		if (IsPlaying_ThisSection((*m_pAniInfo)[CWitchBlade::WITCH_DAMAGEF]->tMotionInfo.dEndTime * 0.8,
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
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}

	if (m_bAction[CWitchBlade::ACTION_DAMAGER] && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DAMAGER))
	{
		m_bAction[CWitchBlade::ACTION_DAMAGER] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}

	if (m_bAction[CWitchBlade::ACTION_DAMAGEB] && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DAMAGEB))
	{
		m_bAction[CWitchBlade::ACTION_DAMAGEB] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}

	if (m_bAction[CWitchBlade::ACTION_DAMAGEL] && (m_pAnimator->Get_AnimationIndex() != CWitchBlade::WITCH_DAMAGEL))
	{
		m_bAction[CWitchBlade::ACTION_DAMAGEL] = false;
		m_pRigidbody->Set_AccMotion(&m_vLook, 0.f);
		m_pAnimator->Set_State(PLAYER_FIGHT);
	}
}

CWitch_Dungeon* CWitch_Dungeon::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
	CWitchBlade* pPlayer, _uint iAniIndex, CWitchBladeWeapon* pLeftSword, CWitchBladeWeapon* pRightSword)
{
	CWitch_Dungeon* pInstance = new CWitch_Dungeon(iAniIndex);

	if (FAILED(pInstance->Ready_State(pMesh, pmapNextMotion, pPlayer, pLeftSword, pRightSword)))
	{
		MSG_BOX("CWitch_Dungeon Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_Dungeon::Free(void)
{
	CWitch_State::Free();
}