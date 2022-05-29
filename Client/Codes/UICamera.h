/*!
 * \file UICamera.h
 * \date 2016/11/15 9:19
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief Camera for UI
 *
 * TODO: long description
 *
 * \note
*/

#ifndef UICamera_h__
#define UICamera_h__


#include "Defines.h"
#include "Camera.h"

class CUICamera
	: public Engine::CCamera
{
private:
	explicit				CUICamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
public:
	virtual					~CUICamera(void);

public:
	HRESULT					InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

public:
	virtual void			Update_Object(const _float& fTimeDelta);

public:
	static CUICamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	virtual	void			Free(void);
};


#endif // UICamera_h__