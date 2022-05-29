#include "Calculator.h"

#include "Transform.h"

USING(Engine)

Engine::CCalculator::CCalculator(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pTransform(NULL)
, m_iMeshSize(0)
, m_bIsCol(true)
, m_bJump(false)
{
	m_pGraphicDev->AddRef();
}	

Engine::CCalculator::~CCalculator(void)
{

}

_bool Engine::CCalculator::Picking_Mesh(POINT ptMouse, const _uint& iWinSizeX, const _uint& iWinSizeY, LPD3DXMESH* pMesh, const _matrix* pMatWorld, _vec3* vPickPos, _vec3* vPickDir)
{
	Translation_ViewSpace(ptMouse, iWinSizeX, iWinSizeY);
	Translation_Local(pMatWorld);
	
	_float	fU, fV, fDist;
	BOOL	BHit = false;

	D3DXIntersect(*pMesh, &m_vPivotPos, &m_vRayDir, &BHit, NULL,
		&fU, &fV, &fDist, NULL, NULL);

	if (BHit)
	{
		if (vPickPos != NULL)
		{
			*vPickPos = m_vPivotPos + (m_vRayDir * fDist);
			*vPickDir = m_vRayDir;
		}
		return true;
	}
	return false;
}

_bool Engine::CCalculator::Picking_Rect(POINT ptMouse, const _uint& iWinSizeX, const _uint& iWinSizeY, 
										const Engine::VTXTEX* pVertex, const D3DXMATRIX* pMatWorld, float& rDist)
{
	Translation_ViewSpace(ptMouse, iWinSizeX, iWinSizeY);
	Translation_Local(pMatWorld);

	float fU, fV, fDist;
	bool  bCheck = false;


	//(Z-)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[1].vPosition,
		&pVertex[0].vPosition,
		&pVertex[2].vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(rDist < fDist)
		{
			rDist = fDist;
			bCheck = true;
		}
	}
	//(Z-)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[3].vPosition,
		&pVertex[2].vPosition,
		&pVertex[0]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
			if(rDist < fDist)
			{
				rDist = fDist;
				bCheck = true;
			}
	}

	return bCheck;
}

_bool CCalculator::Part_CollMesh(const _vec3 pPosition, LPD3DXMESH* pMesh, const _vec3 vRayDir, _vec3* vPickPos, _vec3* vPickDir)
{
	_float	fU, fV, fDist;
	BOOL	BHit = false;

	D3DXIntersect(*pMesh, &pPosition, &vRayDir, &BHit, NULL, &fU, &fV, &fDist, NULL, NULL);

	if (BHit)
	{
		if (vPickPos != NULL)
		{
			*vPickPos = pPosition + (vRayDir * fDist);
			*vPickDir = vRayDir;
		}
		return true;
	}

	return false;
}

_bool Engine::CCalculator::Get_IsCol(void)
{
	return m_bIsCol;
}
void Engine::CCalculator::Set_IsCol( _bool bCol )
{
	m_bIsCol = bCol;
}

void CCalculator::Set_Jump(_bool bJump)
{
	m_bJump = bJump;
}

void Engine::CCalculator::Clear_Map(void)
{
	m_iMeshSize = 0;
	m_mapMesh.clear();
}

void Engine::CCalculator::Set_Transform(CTransform* pTransform)
{
	if(pTransform == NULL)
		return;
	m_pTransform = pTransform;
	m_pTransform->AddRef();
}

void Engine::CCalculator::Set_TargetMesh(LPD3DXMESH* pMesh)
{
	if(pMesh == NULL)
		return;
	
	m_mapMesh.insert(MAPMESH::value_type(m_iMeshSize, pMesh));
	m_iMeshSize = m_mapMesh.size();
}

void Engine::CCalculator::Update_Component(const _float& fTimeDelta)
{
	// 지형 충돌
	if(m_pTransform == NULL)
		return;

	_float fMagicNumber = 4.0f;
	_vec3 vRayPos;
	_vec3 vRayDir = _vec3(0.f, -1.f, 0.f);
	m_pTransform->Get_Infomation(Engine::INFO_POSITION, &vRayPos);
	vRayPos.y += fMagicNumber;
	BOOL bHit;
	_float fU = 0.f;
	_float fV = 0.f;
	_float fDist = 0.f;
	_float fResultDist = 10000.f;
	_bool IsHit = false;

	for (_uint i = 0; i < m_iMeshSize; i++)
	{
		D3DXIntersect(*(m_mapMesh[i]), &vRayPos, &vRayDir, &bHit, NULL, &fU, & fV, &fDist, NULL, NULL);

		if (bHit)
		{
			if (fResultDist > fDist)
			{
				fResultDist = fDist;
				//m_vCollPos = vRayPos + (vRayDir * fDist);
				IsHit = true;
				break;
			}
		}
	}

	if (IsHit && fResultDist > fMagicNumber && m_bJump)
		m_bJump = false;

	if (m_bJump)
	{
		m_bIsCol = false;
		return;
	}

	if (IsHit && (fResultDist < fMagicNumber))
	{
		_vec3 vPos = vRayPos + (vRayDir * fResultDist * 1.05f);
		m_vCollPos = vPos;
		m_pTransform->Set_Position(&vPos);
		m_bIsCol = true;
	}
	else
		m_bIsCol = false;
}

_bool Engine::CCalculator::Wall_Collision_Check(const _vec3* pDir)
{
	if(m_pTransform == NULL)
		return false;
	_vec3 vDir = *pDir;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	_vec3 vHandPos;
	_vec3 vFootPos;
	m_pTransform->Get_Infomation(INFO_POSITION, &vHandPos);
	m_pTransform->Get_Infomation(INFO_POSITION, &vFootPos);
	vHandPos.y += 2.f;
	
	_float fResultDist = 10000.f;
	
	_float fU, fV, fDist;
	BOOL bHit = false;

	for(_uint i = 0; i < m_iMeshSize; i++)
	{
		D3DXIntersect(*m_mapMesh[i], &vHandPos, &vDir, &bHit, NULL, &fU, &fV, &fDist, NULL, NULL);

		if(bHit)
		{
			if(fResultDist > fDist)
				fResultDist = fDist;
		}
	}

	if(fResultDist != 10000.f && fResultDist <= 3.f)
	{
		return true;
	}
	return false;
}

HRESULT Engine::CCalculator::Ready_Calculator(void)
{
	m_vRayDir = _vec3(0.f, -1.f, 0.f);
	return S_OK;
}

void Engine::CCalculator::Translation_ViewSpace(POINT ptMouse, const _uint& iWinSizeX, const _uint& iWinSizeY)
{
	_matrix matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	_vec3 vTemp;

	vTemp.x = (float(ptMouse.x) / (iWinSizeX >> 1) - 1.f) / matProj._11;
	vTemp.y = (float(-ptMouse.y) / (iWinSizeY >> 1) + 1.f) / matProj._22;
	vTemp.z = 1.f;

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRayDir   = vTemp - m_vPivotPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void Engine::CCalculator::Translation_Local(const _matrix* pMatWorld)
{
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	_matrix	matWorld;
	D3DXMatrixInverse(&matWorld, NULL, pMatWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorld);
}

CCalculator* Engine::CCalculator::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCalculator* pInstance = new CCalculator(pGraphicDev);
	if(FAILED(pInstance->Ready_Calculator()))
	{
		MSG_BOX("CCalculator Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CCalculator::Free(void)
{	
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pGraphicDev);
}


