#ifndef SphereMesh_h__
#define SphereMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CSphereMesh
	: public CMesh
{
private:
	explicit CSphereMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSphereMesh(const CSphereMesh& rhs);
	virtual ~CSphereMesh(void);

public:
	void GetMesh(LPD3DXMESH* ppMesh);

public:
	void Render_Mesh(void);
private:
	LPD3DXMESH m_pMesh;
private:
	HRESULT Ready_Mesh(void);
public:
	virtual CResource* Clone(void);
	static CSphereMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

END

#endif // SphereMesh_h__
