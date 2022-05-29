#ifndef FireBall_h__
#define FireBall_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CBardiel;
class CSprite;
class CWave;
class CFireBallParticle;
class CFireBall
	: public Engine::CEffect
{
private:
	explicit CFireBall(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CFireBall(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CFireBallParticle*					m_pFireBallParticle;
	_bool								m_bFireBallParticle;

	CSprite*							m_pFireBallSprite;
	_bool								m_bFireBallSprite;

	CSprite*							m_pFireBallShotDustSprite;
	_bool								m_bFireBallShotDustSprite;

	CWave*								m_pFrontFireWave[3];
	CWave*								m_pMiddleFireWave[4];
	CWave*								m_pBackFireWave[5];

	CSprite*							m_pFireBallGroundWave[30];
	_bool								m_bFireBallGroundWave[30];

	CSprite*							m_pFireBallGroundDustSprite[60];

	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pLeftHandMat;		//왼손.
	const _matrix*						m_pRightFootMat;	//오른발

	CBardiel*							m_pBardiel;

private:
	HRESULT								Ready_Object(void);

public:
	static CFireBall* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

private:
	void					FireBallParticleStart(void);
	void					FireBallParticleUpdate(void);

	void					FireBallSpriteStart(void);

	void					FireWaveSart(void);
	void					FireWaveUpdate(void);

	void					FireBallGroundWaveStart(int iIndex);
	void					FireBallGroundWaveUpdate(void);

	void					FireBallGroundDustSpriteStart(int iIndex);
	void					FireBallGroundDustSpriteUpdate(void);

	void					FireBallShotDustSpriteStart(void);
	void					FireBallShotDustSpriteUpdate(void);
};

#endif