/*!
 * \file WorldCamera.h
 * \date 2016/11/12 17:17
 *
 * \author �� ����
 * Contact: user@company.com
 *
 * \brief 
 ����(����) ī�޶�
 �ǵ帱 ���� ���ϰ� �ǵ帮��ī��.
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
	_float					m_fCamSpeed;		//ī�޶� �ӵ�
	_bool					m_bClick;
	_bool					m_bMouseFix;

private:
	void					FixMouse(void);		//ī�޶� ����/����
	void					MouseMove(void);	//���콺 ������

private:
	virtual	void			Free(void);
};

#endif // WorldCamera_h__