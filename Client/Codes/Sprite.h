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


	//��������Ʈ �ʱ� ����
	void			InitSprite(void);

	//������ üũ�Լ�
	void			SetBillBoardCheck(_bool check);


	//Effect �Ӽ� üũ�Լ�
	void			SetMoveMent(Engine::PARTICLE_MOVEMENT move);

	//�ִϸ��̼� Ȱ��ȭ / ��Ȱ��ȭ
	void			SetAni(_bool bAni= TRUE);

	_bool			GetAni(void);

	void			SetSpriteSpeed(_float speed);

	//��������Ʈ ����
	void			SetSpriteFrame(_int frame, _int select);

	//��������Ʈ ��ġ �����Լ�
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