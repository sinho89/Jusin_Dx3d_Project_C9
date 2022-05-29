#ifndef Camera_AniTool_h__
#define Camera_AniTool_h__

#include "Defines.h"
#include "Camera.h"

class CCamera_AniTool
	: public Engine::CCamera
{
private:
	explicit CCamera_AniTool(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CCamera_AniTool(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
private:
	float	m_fCameraSpeed;
private:
	HRESULT Ready_Camera(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, _float fFovy, _float fAspect, _float fNear, _float fFar);
	void KeyCheck(const _float& fTimeDelta);
	void MouseMove(void);
public:
	static CCamera_AniTool* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
		_float fFovy = D3DXToRadian(60.0f), _float fAspect = _float(WINSIZEX) / WINSIZEY, _float fNear = 0.1f, _float fFar = 1000.f);
private:
	virtual void Free(void);
	
};

#endif // Camera_AniTool_h__
