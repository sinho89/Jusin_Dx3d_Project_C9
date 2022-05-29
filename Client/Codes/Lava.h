#ifndef Lava_h__
#define Lava_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CSprite;
class CWave;
class CLaveTrail;
class CLava
	: public Engine::CEffect
{
private:
	explicit CLava(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CLava(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	//트레일
	CLaveTrail*							m_pRightLeg_Trail;
	_bool								m_bRightLeg_Swing_Start;
	_bool								m_bRightLeg_Swing_End;

	CWave*								m_pGroundFireWave[40];
	_bool								m_bGroundFireWave[40];

	CSprite*							m_pGroundFireSmokeSprite[30];
	_bool								m_bGroundFireSmokeSprite[30];

	CSprite*							m_pGroundFire[40];

	const _matrix*						m_pWorldMat;		//월드
	const _matrix*						m_pRightFootMat;	//오른발

private:
	HRESULT								Ready_Object(void);

public:
	static CLava* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

private:
	void					GroundFireWaveStart(int iIndex);
	void					GroundFireWaveUpdate(void);

	void					GroundFireSmokeSpriteStart(int iIndex);
	void					GroundFireSmokeSpriteUpdate(void);

	void					GroundFireSpriteStart(int iIndex);
	void					GroundFireSpriteUpdate(void);
};


#endif