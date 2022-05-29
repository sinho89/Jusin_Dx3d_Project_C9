#ifndef Rect_Color_h__
#define Rect_Color_h__


#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRect_Color 
	: public CVIBuffer
{
private:
	explicit CRect_Color(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRect_Color(const CRect_Color& rhs);
	virtual ~CRect_Color(void);
public:
	HRESULT Ready_Buffer(void);
public:
	virtual void Render_Buffer(void);
public:
	static CRect_Color* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResource* Clone(void);
	virtual void Free(void);
};

END


#endif // Rect_Color_h__
