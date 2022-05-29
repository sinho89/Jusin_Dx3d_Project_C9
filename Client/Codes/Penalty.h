#ifndef Penalty_h__
#define Penalty_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CSprite;
class CWave;
class CPenaltyTrail;
class CBardiel;
class CPenalty
	: public Engine::CEffect
{
private:
	explicit CPenalty(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CPenalty(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CPenaltyTrail*						m_pLeftHand_Trail;
	_bool								m_bLeftHand_Trail;
	_bool								m_bLeftHand_Trail_End;

	CPenaltyTrail*						m_pRightHand_Trail;
	_bool								m_bRightHand_Trail;
	_bool								m_bRightHand_Trail_End;

	_bool								m_bSmash;
	_bool								m_bSmash_End;

	CSprite*							m_pLeftLegDust[10];
	CSprite*							m_pRightLegDust[10];
	CSprite*							m_pLeftLegDustTwo[5];
	CSprite*							m_pRightLegDustTwo[5];
	CSprite*							m_pRightLastStepDust;

	_bool								m_bLegDust_End;
	_bool								m_bLegDust_Tow_End;

	_bool								m_bLegDust[10];
	_bool								m_bLegDust_Two[5];
	_bool								m_bLastDustStep;


	//¶¥À» ³»·Á ÃÆÀ»¶§ ³ª¿À´Â ¿þÀÌºê Ãæ°ÝÆÄ
	CWave*								m_pWave_0;
	CWave*								m_pWave_1;
	CWave*								m_pWave_2;
	_bool								m_bSmashWave;

	CWave*								m_pShotGunFireWave[30];
	_bool								m_bShotGunFireWave[30];

	_vec3								m_vBulletPos;
	_bool								m_bBulletSet;

	const _matrix*						m_pWorldMat;			//¿ùµå
	const _matrix*						m_pRightHandFingerMat;	//¿À¸¥¼Õ ÁßÁö ¸¶µð
	const _matrix*						m_pLeftHandFingerMat;	//¿Þ¼Õ ÁßÁö ¸¶µð
	const _matrix*						m_pRightElbowMat;		//¿À¸¥ÂÊ ÆÈ²ÞÄ¡
	const _matrix*						m_pLeftElbowMat;		//¿ÞÂÊ ÆÈ²ÞÄ¡
	const _matrix*						m_pRightLegMat;			//¿À¸¥ÂÊ ¹ß¹Ù´Ú
	const _matrix*						m_pLeftLegMat;			//¿ÞÂÊ ¹ß¹Ù´Ú

	CBardiel*							m_pBardiel;

private:
	HRESULT								Ready_Object(void);

public:
	static CPenalty* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

private:
	void					RightLeft_Leg_Dust_Start(int iIndex);
	void					RightLeft_Leg_Dust_Update(void);	

	void					RightLeft_Leg_Dust_Two_Start(int iIndex);

	void					RightLastStep_Dust_Start(void);

	void					SmashWaveStart(void);
	void					SmashwaveUpdate(void);
	
	void					ShotGunFireWaveStart(int iIndex);
	void					ShotGunFireWaveUpdate(void);

	void					BulletPosStart(void);
	void					BulletPosUpdate(void);
};

#endif