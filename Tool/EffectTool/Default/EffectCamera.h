#ifndef EffectCamera_h__
#define EffectCamera_h__

#include "Defines.h"
#include "Camera.h"


class CEffectCamera : public Engine::CCamera
{
private:
	explicit  CEffectCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CEffectCamera(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);

private:
	float	m_fCameraSpeed;

private:
	HRESULT Ready_Camera(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, _float fFovy, _float fAspect, _float fNear, _float fFar);
	void KeyCheck(const _float& fTimeDelta);
	void MouseMove(void);

public:
	static CEffectCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
		_float fFovy = D3DXToRadian(45.0f), _float fAspect = _float(WINSIZEX) / _float(WINSIZEY), _float fNear = 1.f, _float fFar = 10000.f);

private:
	virtual void Free(void);


public:
	D3DXMATRIX		GetView(void);

};


#endif // EffectCamera_h__
