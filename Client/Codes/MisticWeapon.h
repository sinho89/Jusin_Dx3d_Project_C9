#ifndef MisticWeapon_h__
#define MisticWeapon_h__

#include "Weapon.h"

class CMisticWeapon
	: public CWeapon
{
private:
	explicit					CMisticWeapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual						~CMisticWeapon(void);

public:
	const _vec3* Get_ShotPoint(_vec3* pPoint);

public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	_vec3						m_vShotPoint;

private:
	HRESULT						Ready_Object(const _tchar* pTargetTag, const _tchar* pFilePath);
	HRESULT						Add_Component(void);
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);

public:
	static CMisticWeapon*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const _tchar* pTargetTag, const _tchar* pFilePath);

private:
	virtual void				Free(void);
};

#endif // MisticWeapon_h__
