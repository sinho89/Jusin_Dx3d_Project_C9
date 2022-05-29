#ifndef Triangle_Texture_h__
#define Triangle_Texture_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriangle_Texture 
	: public CVIBuffer
{
private:
	explicit CTriangle_Texture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTriangle_Texture(const CTriangle_Texture& rhs);
	virtual ~CTriangle_Texture(void);
public:
	HRESULT Ready_Buffer(void);
public:
	virtual void Render_Buffer(void);
public:
	static CTriangle_Texture* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResource* Clone(void);
	virtual void Free(void);
};

END

#endif // Triangle_Texture_h__
