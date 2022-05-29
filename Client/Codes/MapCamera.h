/*!
 * \file MapCamera.h
 * \date 2016/11/21 14:07
 *
 * \author 곽 경훈
 * Contact: user@company.com
 *
 * \brief 
 맵 텍스쳐가 없는 관계로,
 카메라를 위에서 바라보는 것으로 대신한다.
 *
 * TODO: long description
 *
 * \note
*/
#ifndef MapCamera_h__
#define MapCamera_h__

#include "Camera.h"
#include "Defines.h"

class CPlayer;
class CMapCamera
	: public Engine::CCamera
{
private:
	explicit			CMapCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
public:
	virtual				~CMapCamera(void);

public:
	HRESULT				InitCamera(void);

public:
	virtual HRESULT		Start_ForScene(void);
	virtual void		Update_Object(const _float& fTimeDelta);

private:
	void				FixMouse(void);		//카메라 고정

public:
	static CMapCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	CPlayer*			m_pPlayer;			//플레이어
	_vec3				m_vCamDir;			//카메라 방향
	//_float				m_fGap;				//카메라와 플레이어와의 거리.

private:
	virtual	void		Free(void);
};

#endif // MapCamera_h__