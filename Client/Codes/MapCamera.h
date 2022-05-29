/*!
 * \file MapCamera.h
 * \date 2016/11/21 14:07
 *
 * \author �� ����
 * Contact: user@company.com
 *
 * \brief 
 �� �ؽ��İ� ���� �����,
 ī�޶� ������ �ٶ󺸴� ������ ����Ѵ�.
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
	void				FixMouse(void);		//ī�޶� ����

public:
	static CMapCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	CPlayer*			m_pPlayer;			//�÷��̾�
	_vec3				m_vCamDir;			//ī�޶� ����
	//_float				m_fGap;				//ī�޶�� �÷��̾���� �Ÿ�.

private:
	virtual	void		Free(void);
};

#endif // MapCamera_h__