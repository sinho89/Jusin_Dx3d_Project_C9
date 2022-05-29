#include "AI_State.h"
#include "Transform.h"

Engine::CAI_State::CAI_State(void)
	: m_pTransform(NULL)
	, m_pAnimator(NULL)
	, m_pNavigationMesh(NULL)
	, m_pCalculator(NULL)
	, m_pRigidbody(NULL)
	, m_pCollider(NULL)
	, m_pSphereCollider(NULL)
	, m_pAI(NULL)
	, m_pSound(NULL)
	, m_fRayDis(0.f)
	, m_fPlayerDis(0.f)
	, m_fStopTime(0.f)
	, m_fMonsterSpeed(0.f)
	, m_fRayTime(0.f)
	, m_iMeshSize(0)
	, m_dwIdx(0)
	, m_bCubeRayCol(false)
	, m_bRayMove(false)
	, m_bAStarMove(false)
	, m_bJump(false)
	, m_bJumpMove(false)
	, m_bStop(false)
	, m_bRayTime(false)
	, m_bLookAtDir(true)
	, m_bSpinB(false)
	, m_bDamaged(false)
	, m_bWallCol(false)
	, m_pStopState(NULL)
	, m_pvecAniInfo(NULL)
{
	ZeroMemory(m_bCol, sizeof(bool) * Engine::COL_INFO_END);
}

Engine::CAI_State::~CAI_State(void)
{

}

void Engine::CAI_State::Set_MapMesh(LPD3DXMESH * pMesh)
{
	if (pMesh == NULL)
		return;
	m_mapMesh.insert(MAPMESH::value_type(m_iMeshSize, pMesh));
	m_iMeshSize = m_mapMesh.size();
}

void Engine::CAI_State::Set_StartPos(_vec3 * pPos)
{
	m_vStartPos = *pPos;
}

void Engine::CAI_State::Set_AniInfo(vector<Engine::ANIINFO*>* pvecAniInfo)
{
	m_pvecAniInfo = pvecAniInfo;
}

void Engine::CAI_State::Set_SphereCol(Engine::CCollider * pSphereCollider)
{
	m_pSphereCollider = pSphereCollider;
}

//void Engine::CAI_State::Set_Target_Transcom(Engine::CTransform * pTransform)
//{		
//	m_pTargetTransform = pTransform;
//}


void Engine::CAI_State::PlayerRayCheck(void)
{

	_float rDist = 99999.f;

	for (_uint i = 0; i < m_iMeshSize; ++i)
	{
		_float	fU, fV, fDist;
		BOOL BHit = false;
		D3DXIntersect(*m_mapMesh[i], &m_vPivotPos, &m_vRayDir, &BHit, NULL, &fU, &fV, &fDist, NULL, NULL);
		//D3DXIntersect(*((CMap_Ice*)(*iter_map))->GetMeshCom()[i]->Get_Mesh(),
		//	&m_vPivotPos, &m_vRayDir,
		//	&BHit, NULL, &fU, &fV, &fDist, NULL, NULL);

		if (BHit)
		{
			if (rDist > fDist)
			{
				rDist = fDist;
				D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
				m_vTargetPos = m_vPivotPos + m_vRayDir * rDist;

				//레이의 길이...
				_vec3 vDis = m_vTargetPos - m_vPivotPos;
				m_fRayDis = D3DXVec3Length(&vDis);
			}
		}
	}


}

//void Engine::CAI_State::CubeRayCheck(void)
//{
//	//if (NULL != Engine::FindList(L"Cube"))
//	//{
//	//	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Cube")->begin();
//	//	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Cube")->end();
//
//	//	//			_float	fU, fV, fDist;
//	//	_float fDist = 99999.f;
//	//	_float rDist = 99999.f;
//	//	BOOL BHit = false;
//	//	_vec3 vDis;
//	//	for (iter; iter != iter_end; ++iter)
//	//	{
//	//		((CCube*)(*iter))->CheckRayCol(&fDist, &m_vPivotPos, &m_vRayDir, rDist);
//	//	}
//	//	//플레이어와의 거리 비교..
//	//	if (fDist < m_fPlayerDis)
//	//	{
//	//		m_bCubeRayCol = true;
//	//	}
//	//	else
//	//	{
//	//		m_bCubeRayCol = false;
//	//	}
//	//}
//
//}


bool Engine::CAI_State::LookAtDir(_vec3 * pDir, const _float & fTimeDelta)
{
	if (m_bLookAtDir)
	{
		_vec3 vLook;
		m_pTransform->Get_Infomation(Engine::INFO_LOOK, &vLook);

		//float fAngle = acosf(D3DXVec3Dot(&m_vRayDir, &vLook));

		_vec3 vUp = _vec3(0.f, 1.f, 0.f);
		_vec3 vRight;
		vRight = m_pTransform->Get_Now_Right();

		//D3DXVec3Cross(&vRight, &vUp, &vLook);
		//D3DXVec3Normalize(&vRight, &vRight);
		float fCos = D3DXVec3Dot(&vRight, pDir);	//fCos = 돌 방향
		float fRealCos = D3DXVec3Dot(&vLook, pDir); // 돌 각도
		float fAngle = acosf(fRealCos);				//fAngle == 0 ~ 180
		float fRotationSpeed = 80.f;
		float fDegree = D3DXToDegree(fAngle);
		float fRight = 1.f;

		//if (fDegree < 5.f)
		//{
		//	return false;
		//}

		_bool bRight = false;
		if (fCos < 0)
		{
			fRight = -1.f;
			bRight = false;
		}
		else
		{
			bRight = true;
		}

		//if (fDegree > fRotationSpeed * fTimeDelta)
		//	m_pTransform->Rotation(Engine::ROT_Y, fRight * fRotationSpeed * fTimeDelta);
		//else //if (fDegree <= fRotationSpeed * fTimeDelta)
		//	m_pTransform->Rotation(Engine::ROT_Y, fRight * fDegree);

		if (fCos > 0.1f)
		{
			m_pTransform->Rotation(Engine::ROT_Y, fRight * fRotationSpeed * fTimeDelta);
		}
		else if (fCos < -0.1f)
		{
			m_pTransform->Rotation(Engine::ROT_Y, fRight * fRotationSpeed * fTimeDelta);
		}
		return bRight;
	}

	return true;
}

bool Engine::CAI_State::LookAtRoamDir(_vec3 * pDir, const _float & fTimeDelta)
{
	if (m_bLookAtDir)
	{
		_vec3 vLook;
		m_pTransform->Get_Infomation(Engine::INFO_LOOK, &vLook);

		//float fAngle = acosf(D3DXVec3Dot(&m_vRayDir, &vLook));

		_vec3 vUp = _vec3(0.f, 1.f, 0.f);
		_vec3 vRight;
		vRight = m_pTransform->Get_Now_Right();

		//D3DXVec3Cross(&vRight, &vUp, &vLook);
		//D3DXVec3Normalize(&vRight, &vRight);
		float fCos = D3DXVec3Dot(&vRight, pDir);	//fCos = 돌 방향
		float fRealCos = D3DXVec3Dot(&vLook, pDir); // 돌 각도
		float fAngle = acosf(fRealCos);				//fAngle == 0 ~ 180
		float fRotationSpeed = 120.f;
		float fDegree = D3DXToDegree(fAngle);
		float fRight = 1.f;

		//if (fDegree < 5.f)
		//{
		//	return false;
		//}

		_bool bRight = false;
		if (fCos < 0)
		{
			fRight = -1.f;
			bRight = false;
		}
		else
		{
			bRight = true;
		}

		if (fDegree > fRotationSpeed * fTimeDelta)
			m_pTransform->Rotation(Engine::ROT_Y, fRight * fRotationSpeed * fTimeDelta);
		else //if (fDegree <= fRotationSpeed * fTimeDelta)
			m_pTransform->Rotation(Engine::ROT_Y, fRight * fDegree);

		return bRight;
	}

	return true;
}

void Engine::CAI_State::Set_LookAtDir(_bool bCheck)
{
	m_bLookAtDir = bCheck;
}


