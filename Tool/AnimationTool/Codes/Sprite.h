#ifndef Sprite_h__
#define Sprite_h__


#include "Defines.h"
#include "GameObject.h"
#include "Effect.h"

//�ִϸ��̼����� 

namespace Engine
{
	class CCalculator;
}


class CPickingRect;

class CSprite
	: public Engine::CEffect
{
private:
	explicit CSprite(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSprite(void);

private:
	const _matrix* m_pmatTarget;
	const _matrix* m_pmatBone;


public: //��ŷ��Ʈ��
	Engine::VTXTEX*			m_pPickingVtx;
	CPickingRect*			m_pPickingRect;

private:
	_bool					m_bIsSelect;
	Engine::CCalculator*	m_pCalculatorCom;


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	HRESULT		 Ready_Object(void);


public:
	static CSprite* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);


private:
	virtual			  void Free(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);


	static CSprite* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);


	//�ִϸ��̼� Ȱ��ȭ / ��Ȱ��ȭ
	void			SetAni(_bool bAni= TRUE);

	void			SetBone(Engine::CGameObject* pGameObject, const char* pBoneName);

private:
	HRESULT		    LoadReady_Object(void);

public:
	void			SetSelected(_bool bIsSelect);
	_bool			IsSelected(void);


	_bool			GetMouseCol(POINT ptMouse);

	//AnimationStart
	void			ResetAnimation(void);
};





#endif // Fire_h__
