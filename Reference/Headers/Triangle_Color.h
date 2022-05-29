#ifndef Triangle_Color_h__
#define Triangle_Color_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriangle_Color 
	: public Engine::CVIBuffer
{
private:
	explicit CTriangle_Color(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriangle_Color(const CTriangle_Color& rhs);
	virtual ~CTriangle_Color(void);
public:
	HRESULT Ready_Buffer(void);
public:
	virtual void Render_Buffer(void);
public:
	static CTriangle_Color* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResource* Clone(void);
	virtual void Free(void);
};

END


#endif // Triangle_Color_h__
