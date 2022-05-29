#ifndef FireBallParticle_h__
#define FireBallParticle_h__

#include "Defines.h"
#include "Effect.h"

class  CFireBallParticle
	: public Engine::CEffect
{
private:
	explicit CFireBallParticle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _int Count);
	virtual ~CFireBallParticle(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT Ready_Object(void);


public:
	static CFireBallParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName, _int Count);
	
private:
	virtual void	Free(void);

private:
	_bool			 m_bRenderGroupCheck;
	_float			 m_fGoalResult;
	
private:
	HRESULT			 Add_Component(void);
	HRESULT			 SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			 SetRenderGroupCheck(_bool bCheck);
	void			 SetGoalResult(_float fResult);
	
public:
	void			 SetPart(_int nIdx);
	void			 MoveUpdate(void);
	void			 Update_RePart(void);

public:
	void			SetAni(_bool bAni = TRUE);//애니메이션 활성화 / 비활성화
	const _bool		GetAni(void) const { return m_bAni; }

	//파티클 초기 설정
	void			InitSphereParticle(void);
};

#endif