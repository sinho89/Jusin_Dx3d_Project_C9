/*!
 * \file WorldCamera.h
 * \date 2016/11/12 17:17
 *
 * \author 곽 경훈
 * Contact: user@company.com
 *
 * \brief 
 월드(관찰) 카메라
 건드릴 때는 말하고 건드리라카이.
 *
 * TODO: long description
 *
 * \note
*/
#ifndef WorldCamera_h__
#define WorldCamera_h__

#include "Defines.h"
#include "Camera.h"

class CWorldCamera
	: public Engine::CCamera
{
private:
	explicit				CWorldCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
public:
	virtual					~CWorldCamera(void);

public:
	HRESULT					InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

public:
	virtual void			Update_Object(const _float& fTimeDelta);

private:
	void					KeyCheck(const _float& fTimeDelta);

public:
	static CWorldCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	_float					m_fCamSpeed;		//카메라 속도
	_bool					m_bClick;
	_bool					m_bMouseFix;

private:
	void					FixMouse(void);		//카메라 고정/해제
	void					MouseMove(void);	//마우스 움직입

private:
	virtual	void			Free(void);
};

#endif // WorldCamera_h__