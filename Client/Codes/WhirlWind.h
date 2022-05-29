#ifndef WhirlWind_h__
#define WhirlWind_h__

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
class CPhase2_Trail;
class CWhirlWind
	: public Engine::CEffect
{
private:
	explicit CWhirlWind(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CWhirlWind(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CBardiel*							m_pBardiel;

	//Trail--------------------------------------------------------
	CPhase2_Trail*						m_pHammerTrail;
	_bool								m_bFirstSwing_Start;
	_bool								m_bFirstSwing_End;
	_bool								m_bSecondSwing_Start;
	_bool								m_bSecondSwing_End;
	//-------------------------------------------------------------
	CSprite*							m_pActionDust[40];
	CSprite*							m_pHammerDust;
	_bool								m_bHammerDust;
	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pHammerEnd;		//해머 끝.
	_double								m_dDustTime;
private:
	HRESULT								Ready_Object(void);

public:
	static CWhirlWind* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

private:
	void					ActionDustStart(int iIndex);
	void					ActionDustUpdate(void);

	void					HammerDustStart(void);	
};

#endif