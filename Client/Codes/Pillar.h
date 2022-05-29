#ifndef Pillar_h__
#define Pillar_h__


#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CWave;
class CCurtain;
class CMeteo;

class CPillar
	: public Engine::CEffect
{
private:
	explicit CPillar(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CPillar(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;
	_double								m_dShootingTimer;
private:
	HRESULT								Ready_Object(void);


public:
	static					CPillar* Create(LPDIRECT3DDEVICE9 pGraphicDev
											, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);


private:
	CWave*					m_pStartWave;  //Pillar 동작 1때 생성되는 웨이브
	_bool					m_bStartWave;

	CWave*					m_pGroundWave[3];
	_bool					m_bStartGroundWave;
	_bool					m_bEndAction;

	CCurtain*				m_pCurtain;
	_bool					m_bCloseCurtain;
	_vec3					m_vBardielDir;

	CMeteo*					m_pRockBullet;

public:
	void					StartWaveStart(void);
	void					StartWaveUpdate(void);

	void					GroundWaveStart(void);
	void					GroundWaveUpdate(void);

	void					CurtainStart(void);
	void					CurtainUpdate(void);
	void					LookAtAngle(_float fAngle);

};

#endif