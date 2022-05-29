#ifndef Summon_h__
#define Summon_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CCurtain;
class CSprite;
class CSummonParticle;
class CTwister;
class CWave;
class CSound;

class CSummon
	: public Engine::CEffect
{
private:
	explicit CSummon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSummon(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CSound*								m_pSoundCom;

	const _matrix*						m_pLeftHandBoneMat;
	const _matrix*						m_pRightHandBoneMat;

	_bool								m_bCurtainCheck;
	_bool								m_bHandParticleCheck;
	_bool								m_bTwoHandFaceCheck;
	_int								m_iSpriteCheck;
	_bool								m_bSpriteOff;
	_bool								m_bWakeUpCheck;
	_bool								m_bSpriteScaleMinus;
	_bool								m_bReturnHandSprite;
	_bool								m_bLastScaleMinus;
	_bool								m_bCutainCheck;
	_bool								m_bWakeUpTwistCheck;
	_bool								m_bDestroyTwist;



	CCurtain*							m_pCurtain;
	CSprite*							m_pLeftSprite;
	CSprite*							m_pRightSprite;
	CSummonParticle*					m_pLightParticle;
	CSummonParticle*					m_pLeftParticle;

	CTwister*							m_pTwister;


	CWave*								m_pGroundDecal;

	CWave*								m_pGroundWave[3];
	_bool								m_bWaveState;

	_bool								m_bDecalCheck;

	_float								m_fEffectPower;
private:
	HRESULT								Ready_Object(void);


public:
	static					CSummon* Create(LPDIRECT3DDEVICE9 pGraphicDev
											, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);


public:
	void					RedCurtainStart(void);
	void					RedCurtainUpdate(void);

public:
	void					HandParticleStart(void);
	void					HandParticleUpdate(void);

	void					TwistUpdate(void);

	void					WaveUpdate(void);

	void					DecalUpdate(void);
};


#endif