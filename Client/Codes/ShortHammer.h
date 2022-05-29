#ifndef ShortHammer_h__
#define ShortHammer_h__

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
class CShortHammer
	: public Engine::CEffect
{
private:
	explicit CShortHammer(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CShortHammer(void);


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

	CSprite*							m_pDust_0[5];
	CSprite*							m_pDust_1[8];
	CSprite*							m_pDust_2;
	CSprite*							m_pDust_3;
	CSprite*							m_pDust_4;

	_bool								m_bDust_0[5];
	_bool								m_bDust_1[8];
	_bool								m_bDust_2;
	_bool								m_bDust_3;
	_bool								m_bDust_4;

	CWave*								m_pHammerWave_0[2];
	CWave*								m_pHammerWave_1[2];

	_bool								m_bHammerWave_0;
	_bool								m_bHammerWave_1;

	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pHammerEnd;		//해머 끝.
	const _matrix*						m_pLeftLegMat;		//왼발

private:
	HRESULT								Ready_Object(void);

public:
	static CShortHammer* Create(LPDIRECT3DDEVICE9 pGraphicDev
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
	void					Dust_3_Start(void);
	void					Dust_4_Start(void);

	void					HammerWave_0_Start(void);
	void					HammerWave_1_Start(void);
	void					HammerWave_0_Update(void);
	void					HammerWave_1_Update(void);
};

#endif