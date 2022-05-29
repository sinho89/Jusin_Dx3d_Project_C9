#ifndef LFoot_h__
#define LFoot_h__

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
class CLFoot
	: public Engine::CEffect
{
private:
	explicit CLFoot(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CLFoot(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CBardiel*							m_pBardiel;

	CPhase2_Trail*						m_pRightLegTrail;
	_bool								m_bLegSwingStart;
	_bool								m_bLegSwingEnd;

	CSprite*							m_pDust_0;
	CSprite*							m_pDust_1[5]; //0.0406f

	_bool								m_bDust_0;
	_bool								m_bDust_1[5];

	CWave*								m_pLegWave;
	_bool								m_bLegWave;

	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pRightLegMat;		//오른발

private:
	HRESULT								Ready_Object(void);

public:
	static CLFoot* Create(LPDIRECT3DDEVICE9 pGraphicDev
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

	void					LegWaveStart(void);
	void					LegWaveUpdate(void);
};

#endif