#include "stdafx.h"
#include "SnowBall.h"
#include "Trap.h"
#include "Export_Engine.h"

CSnowBall::CSnowBall(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: CTrap(pGraphicDev, wstrName)
	, m_fJumpForce(0.f)
	, m_fSpeed(0.f)
{
	//ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}
CSnowBall::~CSnowBall(void)
{

}

HRESULT CSnowBall::Start_ForScene(void)
{
	CTrap::Start_ForScene();

	return S_OK;
}

void CSnowBall::Update_Object(const _float& fTimeDelta)
{
	CTrap::Update_Object(fTimeDelta);

	Engine::COLINFO tColInfo;
	tColInfo.eColInfo = Engine::COL_FLY;
	tColInfo.eColDir = Engine::DIR_LOOK;
	tColInfo.fJumpForce = 1.2f;
	tColInfo.fStampForce = 0.f;
	tColInfo.fForce = 20.f;
	tColInfo.iDamage = 50;
	tColInfo.vDir = _vec3(0.f, 0.f, 1.f);
	m_pSphereColCom->Set_ColInfo(&tColInfo);
	Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pSphereColCom);

	m_fSpeed += fTimeDelta * 10.f;
	m_fJumpForce -= fTimeDelta * 0.25f;

	_vec3 vEnd;
	vEnd = m_pTransCom->Get_Now_Position() + m_vDir * fTimeDelta * m_fSpeed;
	m_pTransCom->Set_Position(&vEnd);

	if (m_pCalculatorCom->Get_IsCol())
	{
		m_pRigidbodyCom->Set_Gravity(false);
		if (m_fJumpForce > 0.2f)
		{
			m_pCalculatorCom->Set_Jump(true);
			m_pRigidbodyCom->Set_Jump(m_fJumpForce);
		}
	}
	else
	{
		m_pRigidbodyCom->Set_Gravity(true);
	}

	if (m_pCalculatorCom->Wall_Collision_Check(&m_vDir))
	{
		Engine::Delete_Collider(L"Monster", m_pSphereColCom);
		m_eState = Engine::STATE_DESTROY;
	}

}

void CSnowBall::Render_Object(void)
{
	//CTrap::Render_Object();
}

void CSnowBall::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	CTrap::Render_Object(pEffect, iPassIdx);

	if (m_pSphereColCom)
		m_pSphereColCom->Render_Collider(g_bDebugRender);
}

HRESULT CSnowBall::Ready_Object(void)
{
	CTrap::Ready_Object();

	int iRand = rand() % 2;

	m_eType = (CSnowBall::BALL_TYPE)iRand;

	m_pTransCom->Set_Scale(0.3f, 0.3f, 0.3f);

	if (m_eType == LEFT)
	{
		m_pTransCom->Set_Position(&_vec3(330.f, 60.f, -26.f));
		m_vTargetPos = _vec3(338.f, 25.f, 284.f);
		m_vDir = m_vTargetPos - m_pTransCom->Get_Now_Position();
	}
	else //RIGHT
	{
		m_pTransCom->Set_Position(&_vec3(315.f, 60.f, -26.f));
		m_vTargetPos = _vec3(317.f, 25.f, 284.f);
		m_vDir = m_vTargetPos - m_pTransCom->Get_Now_Position();
	}
	D3DXVec3Normalize(&m_vDir, &m_vDir);


	m_pSphereColCom->SetScale(5.f);
	m_pSphereColCom->SetPos(_vec3(0.f, 0.f, 0.f));

	m_bOutlineColor = _vec3(1.0f, 0.8549f, 0.7254f);

	m_fJumpForce = 1.0f;

	m_fSpeed = 30.f;
	return S_OK;
}


void CSnowBall::Free(void)
{
	CTrap::Free();

	//Engine::Delete_Collider(L"Monster", m_pSphereColCom);
	//	Safe_Delete(m_pMouseCol);

}

CSnowBall* CSnowBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CSnowBall*		pInstance = new CSnowBall(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGrid Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}
