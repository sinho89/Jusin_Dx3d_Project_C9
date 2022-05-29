#ifndef SummonParticle_h__
#define SummonParticle_h__

#include "Defines.h"
#include "Effect.h"

class  CSummonParticle
	: public Engine::CEffect
{
private:
	explicit CSummonParticle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _int Count);
	virtual ~CSummonParticle(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT Ready_Object(void);

public:
	static CSummonParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName, _int Count);


private:
	virtual void Free(void);

private:
	_bool			 m_bRenderGroupCheck;
	_float			 m_fGoalResult;
	_float			 m_fRadius;

private:
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	//어떤 렌더 그룹에 넣을건지 확인
	void			  SetRenderGroupCheck(_bool bCheck);
	void			  SetGoalResult(_float fResult);


public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(void);
	void			  Update_RePart(void);

public:
	void			SetAni(_bool bAni = TRUE);//애니메이션 활성화 / 비활성화
	_bool			GetAni(void);

											  //파티클 초기 설정
	void			InitSphereParticle(void);

	void			SetRadius(_float fRadius);

};

#endif