#ifndef ChangePhase2_h__
#define ChangePhase2_h__

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

class CChangePhase2
	: public Engine::CEffect
{
private:
	explicit CChangePhase2(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CChangePhase2(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CCurtain*				m_pCurtain;
	_bool					m_bArmorCurtain;

	CWave*					m_pCallHammerWave;
	_bool					m_bCallHammerWave;

	const _matrix*			m_pWorldMat;
	const _matrix*			m_pLeftElbowMat;

private:
	HRESULT								Ready_Object(void);
	
public:
	static					CChangePhase2* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

	void					CurtainStart(void);
	void					CurtainUpdate(void);

	void					CallHammerWaveStart(void);
	void					CallHammerWaveUpdate(void);
};

#endif