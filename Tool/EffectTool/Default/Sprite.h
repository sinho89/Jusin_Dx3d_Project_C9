#ifndef Sprite_h__
#define Sprite_h__

#include "Defines.h"
#include "Effect.h"

//이펙트툴용

class  CSprite
	: public Engine::CEffect

{
private:
	explicit CSprite(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSprite(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT		 Ready_Object(void);


public:
	static CSprite* Create(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);

private:
	virtual void Free(void);


	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetPart(_int nIdx);
	void			  SetParticleTexuture(_int _Number);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);
	void			  SetParticleCount(_int iCount);


	//스프라이트 초기 설정
	void			InitSprite(void);

	//빌보드 체크함수
	void			SetBillBoardCheck(_bool check);


	//Effect 속성 체크함수
	void			SetMoveMent(Engine::PARTICLE_MOVEMENT move);

	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni= TRUE);

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


	static CSprite* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);


	void			SetPath(int	 path);


private:
	HRESULT		    LoadReady_Object(void);



};


#endif // Sprite_h__