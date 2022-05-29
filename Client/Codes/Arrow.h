#ifndef Arrow_h__
#define Arrow_h__

#include "Weapon.h"

class CArrow
	: public CWeapon
{
private:
	explicit					CArrow(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual						~CArrow(void);

public:
	virtual HRESULT				Start_ForScene(void);
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT						Ready_Object(const _tchar* pTargetTag, const _tchar* pFilePath);
	HRESULT						Add_Component(void);
	HRESULT						SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);

public:
	static CArrow*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName,
		const _tchar* pTargetTag, const _tchar* pFilePath);

private:
	virtual void				Free(void);
};

#endif // WitchBladeWeapon_h__
