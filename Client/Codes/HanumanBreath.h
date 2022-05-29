#ifndef HanumanBreath_h__
#define HanumanBreath_h__

#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CAnimator;
	class CDynamic;
	class CAnimator;
}

class CWave;
class CBreathBullet;
class CSound;

class CHanumanBreath
	: public Engine::CEffect
{
private:
	explicit CHanumanBreath(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHanumanBreath(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;
	Engine::CAnimator*					 m_pTargetAnimator;
	const _matrix*						 m_pTargetBoneMatrix;

	CSound*								 m_pSoundCom;

private:
	_vec3								 vHanumanDir;
	_float								 m_fAngle;
	_int								 m_iShootingCount;

	_double								m_dShootingTimer;

	void								LookAtAngle(_float fAngle);
	
	CBreathBullet*						m_pBreathBullet;

private:
	HRESULT								Ready_Object(void);
	
public:
	static CHanumanBreath* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

};

#endif