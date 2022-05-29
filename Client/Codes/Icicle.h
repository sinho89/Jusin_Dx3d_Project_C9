#ifndef Icicle_h__
#define Icicle_h__

#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CStaticMesh;
}

class CSprite;
class CWave;

class CIcicle
	: public Engine::CEffect
{
private:
	explicit CIcicle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CIcicle(void);

public:
	virtual void		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);
	virtual	void		Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	virtual			    void Free(void);
	HRESULT			    Ready_Object(void);
	HRESULT			    Add_Component(void);
	HRESULT			    SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT			    Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT			    Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);

	_vec3				m_vDownDir;
	_bool				m_bDown;
	_double				m_dTimer;
	_float				m_fDownTime;
	_vec3				m_vDustStartPoint;

	CSprite*			m_pDust[2];
	CWave*				m_pWave;

private:
	Engine::CStaticMesh*				m_pMeshCom;
	Engine::CTransform*					m_pTargetTransform;
	_vec3								m_vLimitPos;	//지반 위치

	Engine::CCollider*					m_pColliderCom;

public:
	static CIcicle* Create(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);

	void			Falling(void);

	void			SetLimitPos(_vec3 vLimitPos);

	void			SetDustStartPoint(_vec3 vStartPoint);


	void			WaveUpdate(void);

};

#endif