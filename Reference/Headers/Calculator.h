#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CCalculator
	: public CComponent
{
private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator(void);
public:
	_bool Picking_Mesh(POINT ptMouse, const _uint& iWinSizeX, const _uint& iWinSizeY, 
		LPD3DXMESH* pMesh, const _matrix* pMatWorld, _vec3* vPickPos = NULL, _vec3* vPickDir = NULL);

	_bool Picking_Rect(POINT ptMouse, const _uint& iWinSizeX, const _uint& iWinSizeY, 
		const Engine::VTXTEX* pVertex, const D3DXMATRIX* pMatWorld, float& rDist);

	_bool Part_CollMesh(const _vec3 pPosition, LPD3DXMESH* pMesh, const _vec3 vRayDir, _vec3* vPickPos, _vec3* vPickDir);

	_bool Get_IsCol(void);
	void  Set_IsCol(_bool bCol);
	void  Set_Jump(_bool bJump);

	const _vec3	GetCollPos(void) const { return m_vCollPos; }
	void		SetCollPos(const _vec3 vCollPos) { m_vCollPos = vCollPos; }

public:
	void Set_Transform(CTransform* pTransform);
	void Set_TargetMesh(LPD3DXMESH* pMesh);
	void Clear_Map(void);
public:
	virtual void Update_Component(const _float& fTimeDelta);
	_bool Wall_Collision_Check(const _vec3* pDir);
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CTransform* m_pTransform;
	
private:
	map<_int, LPD3DXMESH*> m_mapMesh;
	typedef map<_int, LPD3DXMESH*> MAPMESH;
	_uint m_iMeshSize;
	_vec3 m_vPivotPos;
	_vec3 m_vRayDir;
	_bool m_bIsCol;
	_bool m_bJump;

	_vec3 m_vCollPos;

private:
	HRESULT Ready_Calculator(void);
	void Translation_ViewSpace(POINT ptMouse, const _uint& iWinSizeX, const _uint& iWinSizeY);
	void Translation_Local(const _matrix* pMatWorld);


public:
	static CCalculator* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

END

#endif // Calculator_h__
