/*!
 * \file MouseCol.h
 * \date 2016/08/03 10:15
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef MouseCol_h__
#define MouseCol_h__

//#include "MapTool_Include.h"
#include "Defines.h"

class CMouseCol //: public Engine::CCollision
{
private:
	CMouseCol(void);
public:
	virtual ~CMouseCol(void);

public:
	static POINT GetMousePos(void);
	static CMouseCol* Create(void);

public:
	//bool	PickTerrain(D3DXVECTOR3* pOut,
	//				const Engine::VTXTEX* pTerrainVtx);
	_bool	PickMapMesh(_vec3* pOut, LPD3DXMESH* pMesh, _float &rDist);
	_bool	PickObjMesh(_vec3* pOut, LPD3DXMESH* pMesh, const _matrix* pMatWorld);
	_bool   PickRect(D3DXVECTOR3* pOut, const Engine::VTXTEX* pVertex, const D3DXMATRIX* pMatWorld,
						float& rDist);
	_bool	PickCube(D3DXVECTOR3* pOut, 
		const Engine::VTXCUBETEX* pVertex, 
		const D3DXMATRIX* pMatWorld,
		const D3DXVECTOR3& vCubePos,
		D3DXVECTOR3& vCubePlanDir,//큐브면방향 
		float& rDist);//거리 짧은놈 구할라고
private:
	HRESULT	Initialize(void);
	void	Translation_ViewSpace(void);
	void	Translation_Local(const _matrix* pWorld);
	DWORD	Release(void);
private:
	D3DXVECTOR3			m_vPivotPos;
	D3DXVECTOR3			m_vRayDir;
	_vec3				m_vCamEye;
	float				m_fTime;
};

#endif // MouseCol_h__