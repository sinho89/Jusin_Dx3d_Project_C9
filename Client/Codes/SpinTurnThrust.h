#ifndef SpinTurnThrust_h__
#define SpinTurnThrust_h__


#include "Defines.h"
#include "Effect.h"

class CWave;
class CSprite;
class CSpinTurnThrustTrail;
class CSpinTurnThrustDust;

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CSpinTurnThrust
	: public Engine::CEffect
{
private:
	explicit CSpinTurnThrust(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSpinTurnThrust(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT    Ready_Object(void);

private:
	CWave*								m_pSphere;
	CSprite*							m_pBlackSpark;
	CSpinTurnThrustTrail*				m_pTrail;
	CSpinTurnThrustDust*				m_pDust;

	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;

	_bool								m_bStartSkillCheck;

	_bool								m_bTrailStartCheck;

	Engine::CTransform*					pPlayerTransform;

								
	const _matrix*						m_pmatTarget; //타겟의 월드행렬

	_double								m_fTrailEndTime;


public:
	static CSpinTurnThrust* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);


private:
	virtual					void Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);

	virtual		HRESULT		Start_ForScene(void);


};


#endif // SpinTurnThrust_h__
