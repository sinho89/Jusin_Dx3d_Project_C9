#ifndef Sprite_h__
#define Sprite_h__

#include "Defines.h"
#include "Effect.h"

class  CSprite
	: public Engine::CEffect

{
private:
	explicit CSprite(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSprite(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT		 Ready_Object(void);
	_bool		 m_bYRotCheck;

	_bool		m_bBillboardRotate;
	_matrix*	m_matTargetWorld;

public:
	static CSprite* Create(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);

private:
	virtual void Free(void);


	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	_bool			  m_bRenderCheck;
	_bool			  m_bBlurCheck;

public:
	void			  SetBlurCheck(_bool bCheck) { m_bBlurCheck = bCheck; }
	void			  SetPart(_int nIdx);
	void			  SetParticleTexuture(_int _Number);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);
	void			  SetParticleCount(_int iCount);

	void			  SetRenderCheck(_bool bRenderCheck);

	void			SetTargetWorld(_matrix* matTarget);
	void			SetBillboardRotate(_bool bBheck);


	//스프라이트 초기 설정
	void			InitSprite(void);

	//빌보드 체크함수
	void			SetBillBoardCheck(_bool check);


	//Effect 속성 체크함수
	void			SetMoveMent(Engine::PARTICLE_MOVEMENT move);

	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni= TRUE);

	_bool			GetAni(void);

	void			SetSpriteSpeed(_float speed);

	//스프라이트 설정
	void			SetSpriteFrame(_int frame, _int select);

	//스프라이트 위치 조정함수
	void			SetStandardPos(_float pos, _int select);

	//Size
	void			SetSize(_float size, _int select);

	//Color
	void			SetColor(_float color, _int select);

	//Load
	void			LoadSpriteInfo(Engine::PARTICLEINFO* info);

	//Roof
	void			SetRoof(_bool roof);

	//AnimationStart
	void			ResetAnimation(void);


	static CSprite* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

	void			SetYRotCheck(_bool bCheck);


private:
	HRESULT		    LoadReady_Object(void);



};


#endif // Sprite_h__