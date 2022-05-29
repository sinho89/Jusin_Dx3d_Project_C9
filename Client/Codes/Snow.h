#ifndef Snow_h__
#define Snow_h__

#include "Defines.h"
#include "Effect.h"


class  CSnow
	: public Engine::CEffect

{
private:
	explicit CSnow(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSnow(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);


private:
	HRESULT Ready_Object(void);

public:
	static CSnow* Create(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);

private:
	virtual				void Free(void);

private:
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(void);
	void			  Update_RePart(void);

	static CSnow*	  LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);


	//애니메이션 활성화 / 비활성화
	void			 SetAni(_bool bAni = TRUE);

	_bool			 GetAni(void);
	virtual				HRESULT		Start_ForScene(void);



private:
	const _bool*						m_pPlayerAction; //Player의 동작을 받아오는 변수
	Engine::CAnimator*					m_pTargetAnimator;
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;


private:
	HRESULT		      LoadReady_Object(void);
};

#endif