#ifndef Ring_h__
#define	Ring_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CVIBuffer;
	class CTrail_Texture;
}

//원모양의 검기

class CRing
	: public Engine::CEffect
{
private:
	explicit CRing(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _int iCntX);
	virtual  ~CRing(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	Engine::CTrail_Texture*			m_pBufferCom;
	Engine::VTXTEX*					m_pTrailVtx;
	_int							m_iCntX;
	_int							m_iCntZ;
	_int							m_iInterval;
	_int							m_iTexture;
	_float							m_fRotSpeed;
	_float							m_fSmallRadius;
	_float							m_fBigRadius;
	_float							m_fTrailDotY;
	_float							m_fIntervalAngle;
	_vec3							m_vRingPosition;

	Engine::CTransform*				pTargetTransCom;


	_bool							m_bRenderCheck;


private:
	HRESULT		Ready_Object(void);

public:
	static CRing* Create(LPDIRECT3DDEVICE9 pGraphicDev
			, const wstring& wstrName, _int iCntX);

private:
	virtual		void	Free(void);
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetAni(_bool bAni = TRUE);
	_bool			  GetAni(void);

	void			  SetTargetTransCom(Engine::CTransform* TranCom);

	void			  SetRotSpeed(_float fSpeed);
	void			  SetTextureNumber(_int iTextureNumber);
	void			  SetRadius(_float fBigRadius, _float fSmallRadius);

	void			  SettingTrail(void);

	void			  SetTrailY(_float fY);
	void			  SetRingPos(_vec3 vPos);

	void			  SetIntervalAngle(_float fAngle);

	void			  SetRenderCheck(_bool bCheck);
	
};


#endif // Ring_h__
