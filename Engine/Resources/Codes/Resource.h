#ifndef Resources_h__
#define Resources_h__

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResource 
	: public CComponent
{
protected:
	explicit CResource(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResource(const CResource& rhs);
	virtual ~CResource(void);
protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
public:
	virtual CResource* Clone(void)PURE;
public:
	virtual void Free(void);
};


END

#endif // Resources_h__
