#ifndef Sphere_Color_h__
#define Sphere_Color_h__

#include "VIBuffer.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CSphere_Color
	: public CVIBuffer
{
private:
	explicit CSphere_Color(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphere_Color(void);
public:
	virtual void Render_Buffer(void);
private:
	LPD3DXMESH m_pMesh;
private:
	const _matrix* m_pmatParent;
private:
	HRESULT Ready_Sphere(const _matrix* pmatParent);
public:
	virtual CResource* Clone(void);
	static CSphere_Color* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pmatParent);
private:
	virtual void Free(void);
};

END

#endif // Sphere_Color_h__
