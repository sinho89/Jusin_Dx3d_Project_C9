#include "stdafx.h"
#include "MouseCol.h"
#include "MainApp.h"
#include "MapToolView.h"
#include "Export_Engine.h"
#include "Camera_MapTool.h"


#include "Camera.h"

CMouseCol::CMouseCol(void)
:m_fTime(0.f)
{
}

CMouseCol::~CMouseCol(void)
{
	Release();
}

POINT CMouseCol::GetMousePos(void)
{
	POINT	pt;

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return pt;
}

CMouseCol* CMouseCol::Create(void)
{
	CMouseCol* pMouseCol = new CMouseCol;

	if(FAILED(pMouseCol->Initialize()))
		Engine::Safe_Delete(pMouseCol);

	return pMouseCol;
}

_bool CMouseCol::PickMapMesh( _vec3* pOut , LPD3DXMESH* pMesh, _float &rDist)
{
	Translation_ViewSpace();

	_matrix		matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	Translation_Local(&matIdentity);

	_float	fU, fV, fDist;
	BOOL	BHit = false;
	D3DXIntersect(*pMesh, &m_vPivotPos, &m_vRayDir, &BHit, NULL,
		&fU, &fV, &fDist, NULL, NULL);

	if(BHit)
	{
		if(rDist > fDist)
		{
			rDist = fDist;
			D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
			g_vViewMouse = m_vCamEye + m_vRayDir * rDist;
			return true;
		}
	}

	return false;

}

_bool CMouseCol::PickObjMesh( _vec3* pOut, LPD3DXMESH* pMesh, const _matrix* pMatWorld )
{
	if(NULL != pMesh)
	{
		Translation_ViewSpace();

		Translation_Local(pMatWorld);

		_float	fU, fV, fDist;
		BOOL	BHit = false;

		D3DXIntersect(*pMesh, &m_vPivotPos, &m_vRayDir, &BHit, NULL,
			&fU, &fV, &fDist, NULL, NULL);

		if(BHit)
		{
			D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
			g_vViewMouse = m_vCamEye + m_vRayDir * fDist;
			return true;
		}
	}


	return false;
}

_bool CMouseCol::PickRect(D3DXVECTOR3* pOut, 
					   const Engine::VTXTEX* pVertex, 
					   const D3DXMATRIX* pMatWorld,
					   float& rDist)
{
	Translation_ViewSpace();
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
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
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
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		{
			if(rDist > fDist)
			{
				rDist = fDist;
				bCheck = true;
			}
		}
	}

	return bCheck;
}


_bool CMouseCol::PickCube(D3DXVECTOR3* pOut, 
						 const Engine::VTXCUBETEX* pVertex, 
						 const D3DXMATRIX* pMatWorld,
						 const D3DXVECTOR3& vCubePos,
						 D3DXVECTOR3& vCubePlanDir,//큐브면방향
						 float& rDist)//거리 짧은놈 구할라고
{
	Translation_ViewSpace();
	Translation_Local(pMatWorld);

	float fU, fV, fDist;
	bool bCheck = false;

	//(Z-)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[1].vPosition,
		&pVertex[0].vPosition,
		&pVertex[2].vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{
			vCubePlanDir =  D3DXVECTOR3(0.f, 0.f, -1.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(Z-)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[3].vPosition,
		&pVertex[2].vPosition,
		&pVertex[0].vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 0.f, -1.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(Z+)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[6].vPosition,
		&pVertex[7].vPosition,
		&pVertex[5].vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 0.f, 1.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(Z+)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[4].vPosition,
		&pVertex[6].vPosition,
		&pVertex[7]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 0.f, 1.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(Y-)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[2].vPosition,
		&pVertex[3].vPosition,
		&pVertex[6]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, -1.f, 0.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(Y-)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[7].vPosition,
		&pVertex[6].vPosition,
		&pVertex[3]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, -1.f, 0.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}
	//(Y+)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[5].vPosition,
		&pVertex[4].vPosition,
		&pVertex[1]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 1.f, 0.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(Y+)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[0].vPosition,
		&pVertex[1].vPosition,
		&pVertex[4]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 1.f, 0.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(x-)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[0].vPosition,
		&pVertex[4].vPosition,
		&pVertex[3]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(-1.f, 0.f, 0.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(x-)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[7].vPosition,
		&pVertex[3].vPosition,
		&pVertex[4]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(-1.f, 0.f, 0.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(x+)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[5].vPosition,
		&pVertex[1].vPosition,
		&pVertex[6]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(1.f, 0.f, 0.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	//(x+)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[2].vPosition,
		&pVertex[6].vPosition,
		&pVertex[1]. vPosition,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = m_vPivotPos + m_vRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(1.f, 0.f, 0.f) + vCubePos;
			rDist = fDist;
			bCheck = true;
		}
	}

	return bCheck;
}

//bool CMouseCol::PickTerrain(D3DXVECTOR3* pOut, 
//							const Engine::VTXTEX* pTerrainVtx)
//{
//	Translation_ViewSpace();
//
//	D3DXMATRIX		matIdentity;
//	D3DXMatrixIdentity(&matIdentity);
//	Translation_Local(&matIdentity);
//
//	const Engine::VTXTEX*		pVertex = pTerrainVtx;
//
//	float	fU, fV, fDist;
//
//	for(int z = 0; z <  CMainApp::m_iTerrainSizeZ -1; ++z)
//	{
//		for(int x = 0; x < CMainApp::m_iTerrainSizeX-1 ; ++x)
//		{
//			int		iIndex = z *  CMainApp::m_iTerrainSizeX + x;
//
//			// 오른쪽 위
//			if(D3DXIntersectTri(&pVertex[iIndex +  CMainApp::m_iTerrainSizeX + 1].vPos,
//				&pVertex[iIndex +  CMainApp::m_iTerrainSizeX].vPos,
//				&pVertex[iIndex + 1]. vPos,
//				&m_vPivotPos,
//				&m_vRayDir,
//				&fU, &fV, &fDist))
//			{
//				*pOut = pVertex[iIndex +  CMainApp::m_iTerrainSizeX + 1].vPos +
//					(pVertex[iIndex + CMainApp::m_iTerrainSizeX].vPos - pVertex[iIndex +  CMainApp::m_iTerrainSizeX + 1].vPos) * fU +
//					(pVertex[iIndex + 1].vPos - pVertex[iIndex +  CMainApp::m_iTerrainSizeX + 1].vPos) * fV;
//
//				return true;
//			}
//			// 왼쪽 아래
//			if(D3DXIntersectTri(&pVertex[iIndex].vPos,
//				&pVertex[iIndex + 1].vPos,
//				&pVertex[iIndex +  CMainApp::m_iTerrainSizeX]. vPos,
//				&m_vPivotPos,
//				&m_vRayDir,
//				&fU, &fV, &fDist))
//			{
//				*pOut = pVertex[iIndex].vPos +
//					(pVertex[iIndex + 1].vPos - pVertex[iIndex].vPos) * fU +
//					(pVertex[iIndex +  CMainApp::m_iTerrainSizeX].vPos - pVertex[iIndex].vPos) * fV;
//
//				return true;
//			}
//
//
//		}
//	}
//
//}



HRESULT CMouseCol::Initialize(void)
{

	return S_OK;
}

void CMouseCol::Translation_ViewSpace(void)
{

	//POINT	ptMouse = GetMousePos();

	//D3DXMATRIX	matProj = CCamera::m_matProj;

	//D3DXVECTOR3		vTemp;

	//vTemp.x = (float(ptMouse.x) / (WINCXTOOL >> 1) - 1.f) / matProj._11;
	//vTemp.y = (float(-ptMouse.y) / (WINCYTOOL >> 1) + 1.f) / matProj._22;
	//vTemp.z = 1.f;

	//m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	//m_vRayDir   = vTemp - m_vPivotPos;
	//D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	Engine::CGameObject* pGameObject = Engine::Find(L"MainCamera");

	g_vCamEye = m_vCamEye = ((CCamera_MapTool*)pGameObject)->GetCamEye();

	D3DXMATRIX	matProj = ((CCamera_MapTool*)pGameObject)->GetProj();

	D3DXVECTOR3		vTemp;

	vTemp.x = (float(g_vMouse.x) / (WINSIZEX >> 1) - 1.f) / matProj._11;
	vTemp.y = (float(-g_vMouse.y) / (WINSIZEY >> 1) + 1.f) / matProj._22;
	vTemp.z = 1.f;

	m_vPivotPos = _vec3(0.f, 0.f, 0.f);
	//m_vPivotPos = g_vMouse;
	m_vRayDir   = vTemp - m_vPivotPos;
	//D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
	Engine::Safe_Release(pGameObject);
}

void CMouseCol::Translation_Local(const D3DXMATRIX* pWorld)
{
	Engine::CGameObject* pGameObject = Engine::Find(L"MainCamera");

	D3DXMATRIX	matView = ((CCamera_MapTool*)pGameObject)->GetView();

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);


	D3DXMATRIX	matWorld;
	D3DXMatrixInverse(&matWorld, NULL, pWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorld);
	Engine::Safe_Release(pGameObject);
}

DWORD CMouseCol::Release(void)
{
	//--m_dwRefCnt;

	//if(m_dwRefCnt == 0)
	//{
	//	return 0;
	//}

	//return m_dwRefCnt;
	return 0;
}


