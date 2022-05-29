#include "Rigidbody.h"

#include "Transform.h"
#include "Calculator.h"
#include "Export_Engine.h"

USING(Engine)

Engine::CRigidbody::CRigidbody(void)
: m_pTransform(NULL)
, m_vVelocity(0.f, 0.f, 0.f)
, m_vAccMotionVelocity(0.f, 0.f, 0.f)
, m_fMass(1.f)
, m_fGravityAcc(0.f)
, m_fGravityDist(0.f)
, m_fMaxGravityDist(0.f)
, m_fRevisionPower(0.f)
, m_bGravity(false)
{

}

Engine::CRigidbody::~CRigidbody(void)
{

}

_bool Engine::CRigidbody::Get_Gravity(void)
{
	return m_bGravity;
}

_float CRigidbody::Get_AccDist(void)
{
	return m_fMaxGravityDist;
}

void CRigidbody::Set_Gravity(_bool bGravity)
{
	m_bGravity = bGravity;
}

void CRigidbody::Set_RevisionGravity(const _float & fRevisionPower)
{
	m_fRevisionPower = fRevisionPower;
}

void Engine::CRigidbody::Set_Jump(const _float& fForce)
{
	m_fGravityDist = fForce;
}

void CRigidbody::Set_AccMotion(const _vec3 * pDir, const float & fPower)
{
	m_vAccMotionVelocity = *pDir;
	m_vAccMotionVelocity.y = 0.f;
	D3DXVec3Normalize(&m_vAccMotionVelocity, &m_vAccMotionVelocity);
	m_vAccMotionVelocity = m_vAccMotionVelocity * fPower;
}

void Engine::CRigidbody::Update_Component(const _float& fTimeDelta)
{
	//속도 초기화
	m_vVelocity = _vec3(0.f, 0.f, 0.f);

	//중력
	SetUp_Gravity(fTimeDelta);
	//등속도 운동
	SetUp_AccMotionLine(fTimeDelta);

	//속도에 따른 포지션 변경
	m_pTransform->Set_AccMotion(&m_vVelocity);
}

HRESULT Engine::CRigidbody::Ready_Rigidbody(CTransform* pTransform, CCalculator* pCalculator)
{
	if(pTransform == NULL)
		return E_FAIL;

	if(pCalculator == NULL)
		return E_FAIL;

	m_pTransform = pTransform;
	m_pCalculator = pCalculator;

	m_fGravityAcc = 50.f;
	return S_OK;
}

void Engine::CRigidbody::SetUp_Gravity(const _float& fTimeDelta)
{
	if(!m_bGravity) //중력이 false 지면에 닿았다.
	{	
		m_fMaxGravityDist = 0.f;
		m_fRevisionPower = 0.f;
		if (m_fGravityDist < 0.f)
			m_fGravityDist = 0.f;

		return;
	}

	_vec3 vPos;
	m_pTransform->Get_Infomation(Engine::INFO_POSITION, &vPos);

	_float fRimitY = 0.0f;

	if (GetSceneID() == SCENE_STAGE_ICE)			// 스테이지에 따른 와이값 보정
		fRimitY = -20.f;
	else if (GetSceneID() == SCENE_STAGE_FIRE)
		fRimitY = -380.f;

	if (vPos.y < fRimitY)
	{
		m_pTransform->Set_Position(&_vec3(vPos.x, 0.f, vPos.z));
		m_fMaxGravityDist = 0.f;
		m_fRevisionPower = 0.f;
		m_fGravityDist = 0.f;
	}

	//점프중
	//거리 = 1/2 * 중력가속도 * 시간 * 시간
    m_fGravityDist -= (m_fGravityAcc + m_fRevisionPower) * fTimeDelta * fTimeDelta;

	//점프 누적 거리(양수일때만 더해)
	if(m_fGravityDist > 0.f)
		m_fMaxGravityDist += m_fGravityDist;

	//위치했던 Y값이 10 이상 커지면 더이상 못떠올르게
	if (m_fMaxGravityDist > 12.f)
	{
		m_fGravityDist = 0.f;
		m_fMaxGravityDist = 0.f;
	}
	m_vVelocity.y = m_fGravityDist;
}

void CRigidbody::SetUp_AccMotionLine(const _float & fTimeDelta)
{
	if(!m_pCalculator->Wall_Collision_Check(&m_vAccMotionVelocity))
		m_vVelocity += m_vAccMotionVelocity * fTimeDelta;
}

CRigidbody* Engine::CRigidbody::Create(CTransform* pTransform, CCalculator* pCalculator)
{
	CRigidbody* pInstance = new CRigidbody;
	if(FAILED(pInstance->Ready_Rigidbody(pTransform, pCalculator)))
	{
		MSG_BOX("CRigidbody Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CRigidbody::Free(void)
{

}

