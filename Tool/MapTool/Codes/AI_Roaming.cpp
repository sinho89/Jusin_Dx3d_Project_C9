#include "stdafx.h"
#include "AI_Roaming.h"

//맵툴

CAI_Roaming::CAI_Roaming(void)
	:Engine::CAI_State()
	,m_fRoamingTime(0.f)
	,m_bRoam(false)
{
}

CAI_Roaming::~CAI_Roaming(void)
{
}

void CAI_Roaming::Start_State(void)
{
	m_vStartPos = m_pTransform->Get_Now_Position();
	for (int i = 0; i < DIR_END; ++i)
	{
		m_vRoamPos[i] = m_vStartPos;
	}
	m_vRoamPos[DIR_EAST].x = m_vStartPos.x - 5.f;
	m_vRoamPos[DIR_EAST].z = m_vStartPos.z - 5.f;

	m_vRoamPos[DIR_WEST].x = m_vStartPos.x + 5.f;
	m_vRoamPos[DIR_WEST].z = m_vStartPos.z - 5.f;

	m_vRoamPos[DIR_SOUTH].x = m_vStartPos.x - 5.f;
	m_vRoamPos[DIR_SOUTH].z = m_vStartPos.z + 5.f;

	m_vRoamPos[DIR_NORTH].x = m_vStartPos.x + 5.f;
	m_vRoamPos[DIR_NORTH].z = m_vStartPos.z + 5.f;

	m_eCompareRoamDir = DIR_END;
}

void CAI_Roaming::Start_State(_vec3* pDir, _float fForce)
{

}
//
//void CAI_Roaming::Start_State(_vec3* pDir)
//{
//	m_vStartPos = m_pTransform->Get_Now_Position();
//	for (int i = 0; i < DIR_END; ++i)
//	{
//		m_vRoamPos[i] = m_vStartPos;
//	}
//	m_vRoamPos[DIR_EAST].x = m_vStartPos.x - 5.f;
//	m_vRoamPos[DIR_EAST].z = m_vStartPos.z - 5.f;
//
//	m_vRoamPos[DIR_WEST].x = m_vStartPos.x + 5.f;
//	m_vRoamPos[DIR_WEST].z = m_vStartPos.z - 5.f;
//
//	m_vRoamPos[DIR_SOUTH].x = m_vStartPos.x - 5.f;
//	m_vRoamPos[DIR_SOUTH].z = m_vStartPos.z + 5.f;
//
//	m_vRoamPos[DIR_NORTH].x = m_vStartPos.x + 5.f;
//	m_vRoamPos[DIR_NORTH].z = m_vStartPos.z + 5.f;
//
//	m_eCompareRoamDir = DIR_END;
//}

void CAI_Roaming::Update_State(const _float & fTimeDelta)
{
	if (m_pTransform == NULL || m_pAnimator == NULL || m_pNavigationMesh == NULL)
		return;

	PlayerRayCheck();

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
				//m_fRoamingTime = 0.f;
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

		LookAtDir(&vDir, fTimeDelta);
		_vec3 vEndPos = vPos + vDir * 0.1f;

		m_pTransform->Set_Position(&vEndPos);

		if (fLength < 1.f)
		{
			m_eCompareRoamDir = m_eRoamDir;
			m_bRoam = false;
			m_fRoamingTime = 0.f;
		}
	}
	

}

void CAI_Roaming::End_State(void)
{
}

HRESULT CAI_Roaming::Ready_State(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator, Engine::CNavigationMesh* pNaviMesh)
{
	if (pTransform == NULL || pAnimator == NULL || pNaviMesh == NULL)
		return E_FAIL;

	m_pTransform = pTransform;
	m_pTransform->AddRef();

	m_pAnimator = pAnimator;
	m_pAnimator->AddRef();

	m_pNavigationMesh = pNaviMesh;

	Start_State();

	return S_OK;
}

CAI_Roaming * CAI_Roaming::Create(Engine::CTransform * pTransform, Engine::CAnimator * pAnimator, Engine::CNavigationMesh* pNaviMesh)
{
	CAI_Roaming* pInstance = new CAI_Roaming;

	if (FAILED(pInstance->Ready_State(pTransform, pAnimator, pNaviMesh)))
	{
		MSG_BOX("CAI_Roaming Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CAI_Roaming::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pAnimator);
}
