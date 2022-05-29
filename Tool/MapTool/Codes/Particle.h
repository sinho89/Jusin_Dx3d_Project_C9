#ifndef PParticle_h__
#define PParticle_h__


#include "Defines.h"
#include "Effect.h"


class  CParticle
	: public Engine::CEffect

{
private:
	explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CParticle(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT Ready_Object(void);


public:
	static CParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual void Free(void);


private:
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetPart(_int nIdx);
	void			  SetParticleTexuture(_int _Number);
	void			  MoveUpdate(void);
	void			  Update_RePart(void);
	void			  SetParticleCount(_int iCount);

	//파티클 위치 조정함수
	void			SetStandardPos(_float pos, _int select);

	//파티클 크기 조정함수
	void			SetSize(_float size, _int select);

	//파티클 색깔 조정함수
	void			SetColor(_float color, _int select);

	//파티클 초기 설정
	void			InitParticle(void);

	//바람 조정 함수
	void			SetWindControl(_float wind, _int select);

	//빌보드 체크함수
	void			SetBillBoardCheck(_bool check);

	void			SetMoveMent(Engine::PARTICLE_MOVEMENT move);

	//화염효과
	void			FireMoveUpdate(void);
	void			FireUpdate_RePart(void);

	void			FireSetPart(_int nIdx);

	//파티클 설정변경
	void			ChangingMoveMent(Engine::PARTICLE_MOVEMENT move);


	//파티클 범위지정
	void			ChangingArrange(_float range, _int select);


	//파티클 페이드 지정
	void			ChangingFadeArrange(_int range);

	//파티클 LimitFHeight
	void			ChangingFHeight(_int hegiht);

	void			LoadParticleInfo(Engine::PARTICLEINFO*	info);


	void			SetAni(_bool bAni= TRUE);//애니메이션 활성화 / 비활성화

	//루프 설정
	void			SetRoof(_bool roof);


};

#endif // PParticle_h__