#ifndef SphereParticle_h__
#define SphereParticle_h__

#include "Defines.h"
#include "Effect.h"

class  CSphereParticle
	: public Engine::CEffect
{
private:
	explicit CSphereParticle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _int Count);
	virtual ~CSphereParticle(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT Ready_Object(void);


public:
	static CSphereParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName, _int Count);


private:
	virtual void Free(void);

private:
	_bool			 m_bRenderGroupCheck;
	_float			 m_fGoalResult;

	_bool			m_bBillboardRotate;
	_matrix*		m_matTargetWorld;

private:
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetRenderGroupCheck(_bool bCheck);
	void			  SetGoalResult(_float fResult);


public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(void);
	void			  Update_RePart(void);

public:
	void			SetAni(_bool bAni = TRUE);//�ִϸ��̼� Ȱ��ȭ / ��Ȱ��ȭ
											  
    //��ƼŬ �ʱ� ����
	void			InitSphereParticle(void);


	void			 SetTargetWorld(_matrix * matTarget);

	void			 SetBillBoardRotateCheck(_bool bCheck);
};

#endif // SphereParticle_h__
