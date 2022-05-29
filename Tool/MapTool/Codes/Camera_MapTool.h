#ifndef Camera_MapTool_h__
#define Camera_MapTool_h__

#include "Defines.h"
#include "Camera.h"

class CCamera_MapTool
	: public Engine::CCamera
{
public:
	enum eCAMERA_MODE{MODE_NORMAL, MODE_DEBUG};
private:
	explicit CCamera_MapTool(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CCamera_MapTool(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
public:
	_matrix GetProj(void);
	_matrix GetView(void);
	_vec3 GetCamEye(void);
	_vec3 GetCamAt(void);
	
	void  SetCamEye(_vec3 vPos);
	void  SetCamSpeed(int iSpeed);
	void  SetCamMode(eCAMERA_MODE eMode);
private:
	eCAMERA_MODE    m_eCameraMode;
	_float			m_fCameraSpeed;
	_bool			m_bMouseAngle;
	_bool			m_bMouseFix;

private:
	_vec3					m_vCamDir;			//카메라 방향
	_float					m_fEpslnY;			//조정을 위한 Y값
	_float					m_fGap;				//카메라와 플레이어와의 거리.
	_bool					m_bIsMove;
	_bool					m_bPush;

private:
	HRESULT					Ready_Camera(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, _float fFovy, _float fAspect, _float fNear, _float fFar);
	void					KeyCheck(const _float& fTimeDelta);
	void					MouseMove(void);
	void					DebugMouseMove(void);
	void					FixMouse(void);		//카메라 고정/해제
public:
	static CCamera_MapTool* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
		_float fFovy = D3DXToRadian(60.f), _float fAspect = _float(WINSIZEX) / WINSIZEY, _float fNear = 0.1f, _float fFar = 1000.f);
private:
	virtual void Free(void);
};

#endif // Camera_AniTool_h__
