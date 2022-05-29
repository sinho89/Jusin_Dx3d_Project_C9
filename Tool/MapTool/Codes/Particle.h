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

	//��ƼŬ ��ġ �����Լ�
	void			SetStandardPos(_float pos, _int select);

	//��ƼŬ ũ�� �����Լ�
	void			SetSize(_float size, _int select);

	//��ƼŬ ���� �����Լ�
	void			SetColor(_float color, _int select);

	//��ƼŬ �ʱ� ����
	void			InitParticle(void);

	//�ٶ� ���� �Լ�
	void			SetWindControl(_float wind, _int select);

	//������ üũ�Լ�
	void			SetBillBoardCheck(_bool check);

	void			SetMoveMent(Engine::PARTICLE_MOVEMENT move);

	//ȭ��ȿ��
	void			FireMoveUpdate(void);
	void			FireUpdate_RePart(void);

	void			FireSetPart(_int nIdx);

	//��ƼŬ ��������
	void			ChangingMoveMent(Engine::PARTICLE_MOVEMENT move);


	//��ƼŬ ��������
	void			ChangingArrange(_float range, _int select);


	//��ƼŬ ���̵� ����
	void			ChangingFadeArrange(_int range);

	//��ƼŬ LimitFHeight
	void			ChangingFHeight(_int hegiht);

	void			LoadParticleInfo(Engine::PARTICLEINFO*	info);


	void			SetAni(_bool bAni= TRUE);//�ִϸ��̼� Ȱ��ȭ / ��Ȱ��ȭ

	//���� ����
	void			SetRoof(_bool roof);


};

#endif // PParticle_h__