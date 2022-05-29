#ifndef Divide_h__
#define Divide_h__

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
class CDivide
	: public Engine::CEffect
{
private:
	explicit CDivide(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CDivide(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CBardiel*							m_pBardiel;

	CPhase2_Trail*						m_pHammerTrail;
	_bool								m_bHammerSwingStart;
	_bool								m_bHammerSwingEnd;

	CPhase2_Trail*						m_pLeftLegTrail;
	_bool								m_bLeftLegSwingStart;
	_bool								m_bLeftLegSwingEnd;

	CPhase2_Trail*						m_pRightLegTrail;
	_bool								m_bRightLegSwingStart;
	_bool								m_bRightLegSwingEnd;

	CSprite*							m_pDust_0;
	CSprite*							m_pDust_1;
	CSprite*							m_pDust_2;

	_bool								m_bDust_0;
	_bool								m_bDust_1;
	_bool								m_bDust_2;

	CWave*								m_pLandingWave;
	_bool								m_bLandingWave;

	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pHammerEndMat;	//해머 끝.
	const _matrix*						m_pRightLegMat;		//오른발
	const _matrix*						m_pLeftLegMat;		//왼발

private:
	HRESULT								Ready_Object(void);

public:
	static CDivide* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

private:
	void					Dust_0_Start(void);
	void					Dust_1_Start(void);
	void					Dust_2_Start(void);

	void					LandingWaveStart(void);
	void					LandingWaveUpdate(void);
};

#endif
