#ifndef FireWall_h__
#define FireWall_h__

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
class CFireWall
	: public Engine::CEffect
{
private:
	explicit CFireWall(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CFireWall(void);


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
	_bool								m_bFirstSwing_Start;
	_bool								m_bFirstSwing_End;

	CSprite*							m_pDust_0;
	CSprite*							m_pDust_1[5];

	_bool								m_bDust_0;
	_bool								m_bDust_1[5];

	CWave*								m_pHammerWave[2];
	_bool								m_bHammerWave;

	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pHammerEnd;		//해머 끝.

private:
	HRESULT								Ready_Object(void);

public:
	static CFireWall* Create(LPDIRECT3DDEVICE9 pGraphicDev
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
	void					Dust_1_Start(int iIndex);

	void					HammerWaveStart(void);
	void					HammerWaveUpdate(void);
};

#endif