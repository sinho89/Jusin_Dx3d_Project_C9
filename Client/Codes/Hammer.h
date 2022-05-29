#ifndef Hammer_h__
#define Hammer_h__

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
class CHammer
	: public Engine::CEffect
{
private:
	explicit CHammer(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHammer(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CBardiel*							m_pBardiel;
	CSprite*							m_pDust_0;
	CSprite*							m_pDust_1;
	CSprite*							m_pDust_2;
	CSprite*							m_pDust_3;

	_bool								m_bDust_0;
	_bool								m_bDust_1;
	_bool								m_bDust_2;
	_bool								m_bDust_3;

	CWave*								m_pHammerFireWave[4];
	_bool								m_bHammerFireWave[4];

	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pHammerEnd;		//해머 끝.

	CWave*								m_pBigRune[4];
	CWave*								m_pSmallRune[4];

	CSprite*							m_pBombFrameSprite[2];

private:
	HRESULT								Ready_Object(void);

public:
	static CHammer* Create(LPDIRECT3DDEVICE9 pGraphicDev
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
	void					Dust_3_Start(void);

	void					HammerFireWaveStart(int iIndex);
	void					HammerFireWaveUpdate(void);

	void					RuneUpdate(void);
};

#endif
