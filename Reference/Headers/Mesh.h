#ifndef Mesh_h__
#define Mesh_h__

#include "Resource.h"

BEGIN(Engine)

class ENGINE_DLL CMesh : public CResource
{
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh(void);
public:
	_bool					m_bClone;

public:
	virtual CResource* Clone(void)PURE;
public:
	virtual void Free(void);

};

END

#endif // Mesh_h__
