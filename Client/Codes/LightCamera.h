#ifndef LightCamera_h__
#define LightCamera_h__

#include "Defines.h"
#include "Camera.h"

class CLightCamera
	: public Engine::CCamera
{
private:
	explicit				CLightCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
public:
	virtual					~CLightCamera(void);

private:
	HRESULT					InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

public:
	virtual void			Update_Object(const _float& fTimeDelta);
	
public:
	static CLightCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	virtual	void			Free(void);
};

#endif // LightCamera_h__