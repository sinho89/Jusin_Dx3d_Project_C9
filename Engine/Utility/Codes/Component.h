#ifndef Component_h__
#define Component_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent 
	: public CBase
{
protected:
	explicit CComponent(void);
	virtual ~CComponent(void);
public:
	virtual void Update_Component(const _float& fTimeDelta);
public:
	virtual CComponent* Clone(void);
protected:
	virtual void Free(void)PURE;
};

END

#endif // Component_h__
