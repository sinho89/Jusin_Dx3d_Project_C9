#ifndef SpinningHeart_h__
#define SpinningHeart_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
	class CAnimator;
}

class CCyclone;
class CWave;
class CCherryBlossom;
class CSpinningHeartTrail;
class CSpinningThrown;

class CSpinningHeart
	: public Engine::CEffect
{
private:
	explicit CSpinningHeart(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSpinningHeart(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;

private:
	CCyclone*							m_pCyclone;
	CWave*								m_pLightWave;
	CWave*								m_pLightWaveTwo;
	CCherryBlossom*						m_pCherryBlossom;
	Engine::CAnimator*					m_pTargetAnimator;
	CSpinningHeartTrail*				m_pTrail;
	const _matrix*						m_pmatTarget; //타겟의 월드행렬
	_bool								m_bTrailStart;
	_bool								m_bShooingState; //쏘는 동작 확인
	_bool								m_bCherryCheck;

	CSpinningThrown*					m_pSpinningBullet;

	
	CWave*								m_pGroundWave[2];
	_bool								m_bGroundWaveCheck;

	_bool								m_bSpinningThrownCheck;

private:
	HRESULT								Ready_Object(void);


public:
	static CSpinningHeart* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);


private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);


public:
	void				    CycloneUpdate(void);
	void					CycloneStart(void);

	void					LightStart(void);
	void					LightUpdate(void);


	void					GroundWaveStart(void);

	void					GroundWaveUpdate(void);

};



#endif