#ifndef Clean_h__
#define Clean_h__

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
class CClean
	: public Engine::CEffect
{
private:
	explicit CClean(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CClean(void);


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
	_bool								m_bHammerTrail;

	CSprite*							m_pDust_0[6];	//0.0697f
	CSprite*							m_pDust_1[6];	//0.055777f
	CSprite*							m_pDust_2;

	_bool								m_bDust_0[6];
	_bool								m_bDust_1[6];
	_bool								m_bDust_2;

	CWave*								m_pHammerWave[3];
	_bool								m_bHammerWave;

	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pHammerEndMat;	//해머 끝.

private:
	HRESULT								Ready_Object(void);

public:
	static CClean* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

private:
	void					Dust_0_Start(int iIndex);
	void					Dust_1_Start(int iIndex);
	void					Dust_2_Start(void);

	void					HammerWaveStart(void);
	void					HammerWaveUpdate(void);
};

#endif