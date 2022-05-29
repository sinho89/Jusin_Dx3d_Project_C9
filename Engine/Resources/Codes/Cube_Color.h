#ifndef Cube_Color_h__
#define Cube_Color_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCube_Color 
	: public CVIBuffer
{
private:
	explicit CCube_Color(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCube_Color(const CCube_Color& rhs);
	virtual ~CCube_Color(void);

public:
	HRESULT Ready_Buffer(const _ulong& dwColor, const _matrix* pmatParent);
public:
	virtual void Render_Buffer(void);
private:
	const _matrix* m_pmatParent;
public:
	static CCube_Color* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pmatParent, const _ulong& dwColor);
	virtual CResource* Clone(void);
	virtual void Free(void);
};

END

#endif // Cube_Color_h__
