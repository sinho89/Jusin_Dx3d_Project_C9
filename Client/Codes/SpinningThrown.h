#ifndef SpinningThrown_h__
#define SpinningThrown_h__



#include "Defines.h"
#include "Effect.h"


class CSpinningBullet;
class CSprite;
class CWave;

class CSpinningThrown
	: public Engine::CEffect
{
private:
	explicit CSpinningThrown(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSpinningThrown(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	CSpinningBullet*	m_pBullet;
	_double				m_dTimer;
	Engine::CCollider*	m_pColliderCom;
	CWave*				m_pBombWave[20];

private:
	HRESULT								Ready_Object(void);

private:
	_vec3								m_vLook;
	_vec3								m_vTargetLook;
	_vec3								m_vTargetAngle;

public:
	static CSpinningThrown* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);
	void					SetAni(_bool bAni);
	_bool					GetAni(void);
	void					Setting(Engine::CTransform* TargetTransform);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SpinningThrownStart(int iIndex);
	void					SpinningUpdate(void);
};





#endif