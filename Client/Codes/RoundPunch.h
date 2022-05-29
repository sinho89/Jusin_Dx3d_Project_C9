#ifndef RoundPunch_h__
#define RoundPunch_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
	class CAnimator;
}

class CWave;
class CSprite;
class CRoundPunch
	: public Engine::CEffect
{
private:
	explicit CRoundPunch(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CRoundPunch(void);

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
	static CRoundPunch* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	CSprite*				m_pRoundDust_A;
	_bool					m_bRoundDust_A;

	CSprite*				m_pRoundDust_B[4];
	_bool					m_bRoundDust_B[4];

	CSprite*				m_pRoundDust_C[7];
	_bool					m_bRoundDust_C[7];

	CSprite*				m_pRoundC1_Start[12];
	_bool					m_bRoundC1_Start[12];

	CSprite*				m_pRoundC1_Ing[3];
	_bool					m_bRoundC1_Ing[3];

	CSprite*				m_pRoundC1_End;
	_bool					m_bRoundC1_End;
	
	_bool					m_bSkillStart;

	_bool					m_bHanumanWhitePower;
	_bool					m_bEffectPowerUpDown;

	Engine::CGameObject*	m_pHanuman;

	const _matrix*			m_pWorldMat;		//¿ùµå
	const _matrix*			m_pLeftHandMat;		//¿Þ¼Õ
	const _matrix*			m_pRightHandMat;	//¿À¸¥¼Õ
	const _matrix*			m_pLeftFootMat;		//¿Þ¹ß
	const _matrix*			m_pRightFootMat;	//¿À¸¥¹ß
	const _matrix*			m_pLeftShoulderMat;	//¿Þ ¾î±ú
	const _matrix*			m_pRightShoulderMat;//¿À¸¥ ¾î±ú
	const _matrix*			m_pBackMat;			//µî
	const _matrix*			m_pBackDownMat;		//µî¾Æ·¡
	const _matrix*			m_plowerBodyMat;	//ÇÏÃ¼
	const _matrix*			m_pFaceMat;			//¾ó±¼

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

public:
	void					RoundDust_A_Start(void);
	void					RoundDust_A_Update(void);

	void					RoundDust_B_Start(_uint iNum);
	void					RoundDust_B_Update(void);

	void					RoundDust_C_Start(_uint iNum);
	void					RoundDust_C_Update(void);

	void					RoundC1_Start_Start(_uint iNum);
	void					RoundC1_Start_Update(void);

	void					RoundC1_Ing_Start(_uint iNum);
	void					RoundC1_Ing_Update(void);

	void					RoundC1_End_Start(void);
	void					RoundC1_End_Update(void);
};

#endif