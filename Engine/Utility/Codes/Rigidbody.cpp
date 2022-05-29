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
	//�ӵ� �ʱ�ȭ
	m_vVelocity = _vec3(0.f, 0.f, 0.f);

	//�߷�
	SetUp_Gravity(fTimeDelta);
	//��ӵ� �
	SetUp_AccMotionLine(fTimeDelta);

	//�ӵ��� ���� ������ ����
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
	if(!m_bGravity) //�߷��� false ���鿡 ��Ҵ�.
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

	if (GetSceneID() == SCENE_STAGE_ICE)			// ���������� ���� ���̰� ����
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

	//������
	//�Ÿ� = 1/2 * �߷°��ӵ� * �ð� * �ð�
    m_fGravityDist -= (m_fGravityAcc + m_fRevisionPower) * fTimeDelta * fTimeDelta;

	//���� ���� �Ÿ�(����϶��� ����)
	if(m_fGravityDist > 0.f)
		m_fMaxGravityDist += m_fGravityDist;

	//��ġ�ߴ� Y���� 10 �̻� Ŀ���� ���̻� �����ø���
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

