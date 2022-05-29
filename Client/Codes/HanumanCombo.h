#ifndef HanumanCombo_h__
#define HanumanCombo_h__

#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CAnimator;
	class CDynamic;
	class CAnimator;
}

class CHanumaAttack01_Trail;
class CReflectMesh;
class CWave;
class CReflectDust;
class CSprite;

class CHanumanCombo
	: public Engine::CEffect
{
private:
	explicit CHanumanCombo(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHanumanCombo(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;
	Engine::CAnimator*					 m_pTargetAnimator;


	//Combo0=================================================
	//Trail
	CHanumaAttack01_Trail*				m_pLeft_0_Trail;
	CHanumaAttack01_Trail*				m_pLeft_1_Trail;
	CHanumaAttack01_Trail*				m_pLeft_2_Trail;
	_bool								m_bLeftSwing;
	_bool								m_bLeftDeleteSwing;
	//=======================================================


	//Reflect Mesh================================================
	CReflectMesh*						m_pReflectGroundWave;
	_bool								m_bReflectGroundWaveCheck;
	_double								m_dReflectSize;
	//============================================================
	
	_bool								m_bRadialBlur;
	_float								m_fRadialBlurTime;

	Engine::CGameObject*				m_pHanuman;

	//Wave========================================================
	CWave*								m_pBlueWave;
	CWave*								m_pRedWave;
	//=============================================================

	//Wave========================================================
	CSprite*							m_pDust[20];
	_bool								m_bDustCheck;
	//============================================================

	//SlideWave===================================================
	CWave*								m_pSlideWave[8];
	_vec3								m_vSlideWaveDir[8]; //슬라이드 웨이브가 나아갈 방향벡터
	//============================================================

private:
	HRESULT								Ready_Object(void);
	
public:
	static CHanumanCombo* Create(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

	void					ReflectGroundWaveStart(void);

	void					ReflectGroundUpdate(void);


	void					ColorWaveStart(void);

	void					ColorWaveUpdate(void);


	void					DustStart(void);

	void					DustUpdate(void);

	void					SlideWaveStart(void);

	void					SlideWaveUpdate(void);

};

#endif