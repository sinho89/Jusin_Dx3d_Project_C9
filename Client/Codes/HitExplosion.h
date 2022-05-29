#ifndef HitExplosion_h__
#define HitExplogion_h__

#include "Defines.h"
#include "Effect.h"

class CWave;
class CSprite;

class CHitExplosion
	: public Engine::CEffect
{
private:
	explicit CHitExplosion(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHitExplosion(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT								Ready_Object(void);

public:
	static					CHitExplosion* Create(LPDIRECT3DDEVICE9 pGraphicDev
													, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

private:
	_vec3					m_pStartPos;
	CWave*					m_pWave;
	CSprite*				m_pSprite;
	CSprite*				m_pDust;

public:
	void					SetStartPos(_vec3 vPos);

	void					WaveReady(void);

	void					WaveUpdate(void);

	void					SettingStartPos(void);
};


#endif