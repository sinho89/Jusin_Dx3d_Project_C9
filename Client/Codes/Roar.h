#ifndef Roar_h__
#define Roar_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
	class CAnimator;
}

class CWave;
class CReflect;
class CRoar
	: public Engine::CEffect
{
private:
	explicit CRoar(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CRoar(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;
	Engine::CAnimator*					 m_pTargetAnimator;

private:
	HRESULT								Ready_Object(void);

public:
	static CRoar* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	CReflect*				m_pReflect[8];
	_bool					m_bReflect[8];

	CWave*					m_pRoarWave[8];

	CWave*					m_pGroundWave;
	_bool					m_bGroundWave;

	_bool					m_bRadialBlurCheck;
	_bool					m_bRadialBlur_0;
	_bool					m_bRadialBlur_1;
	_float					m_fRadialBlurPower;
	_float					m_fRadialBlurTime;

	Engine::CGameObject*	m_pHanuman;

	const _matrix*			m_pWorldMat;		//¿ùµå

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

	void					ReflectStart(int iNum);
	void					ReflectUpdate(void);

	void					RoarWaveStart(int iNum);
	void					RoarWaveUpdate(void);

	void					GroundWaveStart(void);
	void					GroundWaveUpdate(void);
};

#endif