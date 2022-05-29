#ifndef Meteo_h__
#define Meteo_h__


#include "Defines.h"
#include "Effect.h"

class CRock;
class CSprite;
class CHitExplosion;

class CMeteo
	: public Engine::CEffect
{
private:
	explicit CMeteo(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMeteo(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT								Ready_Object(void);

public:
	static					CMeteo* Create(LPDIRECT3DDEVICE9 pGraphicDev
											, const wstring& wstrName);

private:
	Engine::CCollider*					m_pColliderCom;	
	CRock*								m_pRock;
	CSprite*							m_pSprite;
	_vec3								m_vLimitPos;	//지반 위치
	_vec3								m_vShootingDir;
	_float								m_fDownTime;
	_float								m_fSpeed;
	_double								m_dLifeTime;
	_float								m_fTimeAdd;

	CSprite*							m_pMeteoDust[10];
	_double								m_dExplosionTime;
	CHitExplosion*						m_pExplosion;

	_vec3								m_vBombPos;
	_bool								m_bBombCheck;

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);


public:
	void					SetShootingDir(_vec3 vDir);
	void					SetLimitPos(_vec3 vLimitPos);

	void					MoveParabola(void);

	void					SetTimeAdd(_float fTime);

	void					SetAngle(_vec3 vAngle);

	void					SpriteDustStart(int iIndex);
	void					SpriteDustUpdate(void);

};

#endif
