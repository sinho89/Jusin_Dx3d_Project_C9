#ifndef Sprite_h__
#define Sprite_h__

#include "Defines.h"
#include "Effect.h"

//����Ʈ����

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


	//��������Ʈ �ʱ� ����
	void			InitSprite(void);

	//������ üũ�Լ�
	void			SetBillBoardCheck(_bool check);


	//Effect �Ӽ� üũ�Լ�
	void			SetMoveMent(Engine::PARTICLE_MOVEMENT move);

	//�ִϸ��̼� Ȱ��ȭ / ��Ȱ��ȭ
	void			SetAni(_bool bAni= TRUE);

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


	static CSprite* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);


	void			SetPath(int	 path);


private:
	HRESULT		    LoadReady_Object(void);



};


#endif // Sprite_h__