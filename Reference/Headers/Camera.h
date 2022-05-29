#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera 
	: public CGameObject
{
protected:
	explicit		CCamera(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual			~CCamera(void);

public:
	HRESULT			Ready_Object(void);
	virtual void	Update_Object(const _float& fTimeDelta);

	void			MakeViewMatrix(void);
	void			MakeProjMatrix(float fWinSizeX, float fWinSizeY);
	void			MakeProjOrthoLHMatrix(float fWinSizeX, float fWinSizeY, float fZn = 0 , float fZf = 0);

public:
	void			Set_Shake(const _vec3& vShakeDir, const _float& fPower = 0.2f , const _int& iCnt = 5, const _int& iDelay = 1);

public:
	const _bool		GetCamON(void) const { return m_bCamera_ON; }
	void			SetCamOn(const _bool bCamOn) { m_bCamera_ON = bCamOn; }

	const _vec3		GetEyePosition(void) const { return m_vEye; }
	void			SetEyePosition(const _vec3 vEye) { m_vEye = vEye; }

	const _vec3		GetAtPosition(void) const { return m_vAt; }
	void			SetAtPosition(const _vec3 vAt) { m_vAt = vAt; }

	const _float	GetCamFar(void) const { return m_fFar; }
	void			SetCamFar(const _float fFar) { m_fFar = fFar; }

	const _matrix	GetMatView(void) const { return m_matView; }
	void			SetMatView(const _matrix matView) { m_matView = matView; }

	const _matrix	GetMatProj(void) const { return m_matProj; }
	void			SetMatPorj(const _matrix matProj) { m_matProj = matProj; }

	const _float	GetFovy(void) const { return m_fFovy; }
	void			SetFovy(const _float fFovy) { m_fFovy = fFovy; }

	const _float	GetNear(void) const { return m_fNear; }
	void			SetNear(const _float fNear) { m_fNear = fNear; }

	const _float	GetAspect(void) const { return m_fAspect; }
	void			SetAspect(const _float fAspect) { m_fAspect = fAspect; }

	const _matrix	Get_CACD_Proj(const _uint iCnt) const { return m_mat_CACD_Proj[iCnt]; }
	void			Set_CACD_Proj(const _matrix matProj, const _uint iCnt) { m_mat_CACD_Proj[iCnt] = matProj; }

	const _float	Get_Gap(void) const { return m_fGap; }
	void			Set_Gap(const _float fGap) { m_fGap = fGap; }

protected:
	_vec3			m_vEye, m_vAt, m_vUp;
	_matrix			m_matView;

	_float			m_fFovy, m_fAspect, m_fNear, m_fFar;
	_matrix			m_matProj;

	_matrix			m_mat_CACD_Proj[3];

	_float			m_fGap;

	_bool			m_bCamera_ON;

protected:
	_bool					m_bShake;
	_int					m_iShakeCnt;
	_int					m_iShakeDelay;

	_int					m_iShakeMaxCnt;
	_float					m_fShakePower;
	_vec3					m_vShakeDir;

protected:
	void					CameraShake(void);

protected:
	virtual void	Free(void);
};

END

#endif // Camera_h__
