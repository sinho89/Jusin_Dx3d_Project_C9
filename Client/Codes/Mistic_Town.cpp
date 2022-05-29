#include "stdafx.h"
#include "Mistic_Town.h"

#include "MisticWeapon.h"

CMistic_Town::CMistic_Town(_uint iAniIndex)
	: CMistic_State(iAniIndex)
	, m_vLook(0.f, 0.f, 0.f)
	, m_bCanMove(true)
	, m_bCanJump(true)
{

}

CMistic_Town::~CMistic_Town(void)
{

}

void CMistic_Town::Start_State(void)
{
	m_pWeapon->SetActive(true);
}

void CMistic_Town::Update_State(const _float& fTimeDelta)
{
	if (m_pMesh == NULL)
		return;

	ActionCheck();
	JumpCheck(fTimeDelta);
	MoveCheck(fTimeDelta);

	CMistic_State::EndCheck();
}

void CMistic_Town::End_State(void)
{
	m_pWeapon->SetActive(false);
}

HRESULT CMistic_Town::Ready_State(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
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

	m_pPlayerInfo = m_pPlayer->GetPlayerInfo();

	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
	Engine::Safe_Release(m_pRigidbody);
	Engine::Safe_Release(m_pCalculator);
	return S_OK;
}

void CMistic_Town::ActionCheck(void)
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
}

void CMistic_Town::MoveCheck(const _float & fTimeDelta)
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

void CMistic_Town::JumpCheck(const _float & fTimeDelta)
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

CMistic_Town* CMistic_Town::Create(Engine::CDynamicMesh* pMesh, map<_uint, list<Engine::ANIMATORINFO*>>* pmapNextMotion,
	CMistic* pPlayer, _uint iAniIndex, CMisticWeapon* pWeapon)
{
	CMistic_Town* pInstance = new CMistic_Town(iAniIndex);

	if (FAILED(pInstance->Ready_State(pMesh, pmapNextMotion, pPlayer, pWeapon)))
	{
		MSG_BOX("CMistic_Town Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMistic_Town::Free(void)
{
	CMistic_State::Free();
}
