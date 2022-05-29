#include "stdafx.h"
#include "AI_Roaming_BWarrior.h"
#include "Export_Engine.h"
#include "GameObject.h"
#include "AI.h"
//클라

CAI_Roaming_BWarrior::CAI_Roaming_BWarrior(void)
	:CAI_Babegazi_Warrior()
	,m_fRoamingTime(0.f)
	,m_fAggroRad(0.f)
	,m_bRoam(false)
{
}

CAI_Roaming_BWarrior::~CAI_Roaming_BWarrior(void)
{
}

void CAI_Roaming_BWarrior::Start_State(void)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
	m_vStartPos = m_pTransform->Get_Now_Position();
	for (int i = 0; i < DIR_END; ++i)
	{
		m_vRoamPos[i] = m_vStartPos;
	}
	m_vRoamPos[DIR_EAST].x = m_vStartPos.x - 5.f;
	m_vRoamPos[DIR_EAST].z = m_vStartPos.z - 6.f;

	m_vRoamPos[DIR_WEST].x = m_vStartPos.x + 4.f;
	m_vRoamPos[DIR_WEST].z = m_vStartPos.z - 5.f;

	m_vRoamPos[DIR_SOUTH].x = m_vStartPos.x - 5.f;
	m_vRoamPos[DIR_SOUTH].z = m_vStartPos.z + 4.f;

	m_vRoamPos[DIR_NORTH].x = m_vStartPos.x + 6.f;
	m_vRoamPos[DIR_NORTH].z = m_vStartPos.z + 5.f;

	m_eCompareRoamDir = DIR_END;
}


void CAI_Roaming_BWarrior::Update_State(const _float & fTimeDelta)
{
	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	m_vTargetPos = m_pTargetTransform->Get_Now_Position();
	m_vPlayerPos = m_pTargetTransform->Get_Now_Position();
	m_vPos = m_pTransform->Get_Now_Position();
	m_vPivotPos = m_pTransform->Get_Now_Position();
	m_vPivotPos.y += 2.f;
	m_vRayDir = m_vTargetPos - m_vPivotPos;
	m_fPlayerDis = D3DXVec3Length(&m_vRayDir);


	if (!m_bRoam)
	{
		m_fRoamingTime += fTimeDelta;
		m_pAnimator->Set_AnimationSet(STAND_IDLE);

		if (m_fRoamingTime > 4.f)
		{
			//srand(unsigned int(time(NULL)));

			//출발할 방향을 정한다....
			m_eRoamDir = ROAM_DIR(rand() % 4);

			//출발하기 전에 내가 왔던길이랑 비교...
			if (m_eRoamDir == m_eCompareRoamDir)
			{
				return;
			}
			//출발...
			else
			{
				m_bRoam = true;
				m_fRoamingTime = 0.f;
			}

		}
	}

	if (m_bRoam)// m_bRoam == true
	{
		m_pAnimator->Set_AnimationSet(WALK);
		_float fLength = 0.f;

		_vec3 vDir;
		_vec3 vPos;
		m_pTransform->Get_Infomation(Engine::INFO_POSITION, &vPos);
		vDir = m_vRoamPos[m_eRoamDir] - vPos;
		fLength = D3DXVec3Length(&vDir);

		D3DXVec3Normalize(&vDir, &vDir);

		LookAtRoamDir(&vDir, fTimeDelta);
		_vec3 vEndPos = vPos + vDir * fTimeDelta * 3.f;

		m_pTransform->Set_Position(&vEndPos);

		if (fLength < 2.f)
		{
			m_eCompareRoamDir = m_eRoamDir;
			m_bRoam = false;
			m_fRoamingTime = 0.f;
		}
	}

	if (m_fPlayerDis < m_fAggroRad)
	{
		m_pAI->Set_State(AI_DISCOVERY);
	}
	

}

void CAI_Roaming_BWarrior::End_State(void)
{

}

HRESULT CAI_Roaming_BWarrior::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator
	, Engine::CNavigationMesh* pNaviMesh, Engine::CAI* pAI)
{
	if (pTransform == NULL || pAnimator == NULL || pNaviMesh == NULL)
		return E_FAIL;

	m_pTransform = pTransform;
	m_pTransform->AddRef();

	m_pAnimator = pAnimator;
	m_pAnimator->AddRef();

	m_pAI = pAI;

	m_pNavigationMesh = pNaviMesh;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"Player");
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	Engine::Safe_Release(pGameObject);

	m_fAggroRad = 60.f;

	Start_State();

	return S_OK;
}

CAI_Roaming_BWarrior * CAI_Roaming_BWarrior::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator,
	Engine::CNavigationMesh* pNaviMesh, Engine::CAI* pAI)
{
	CAI_Roaming_BWarrior* pInstance = new CAI_Roaming_BWarrior;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh, pAI)))
	{
		MSG_BOX("CAI_Roaming_BWarrior Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Roaming_BWarrior::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
}
