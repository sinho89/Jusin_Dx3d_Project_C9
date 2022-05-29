#ifndef DragonDance_h__
#define DragonDance_h__

#include "Defines.h"
#include "Effect.h"

class CRing;
class CCherryBlossom;
class CPledgeRing;
class CWave;

class CDragonDance
	: public Engine::CEffect
{
private:
	explicit CDragonDance(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual  ~CDragonDance(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;

	CPledgeRing*						 m_pRightLongTrail;	 //오른손 큰Trail 휘두르기
	CPledgeRing*						 m_pRightSmallTrail; //오른손 작은Trail 휘두르기
	CPledgeRing*						 m_pLeftLongTrail;   //왼손 큰Trail 휘두르기
	CPledgeRing*						 m_pLeftSmallTrail;  //왼손 작은 Trail 휘두르기


	//스킬 세팅을 위해서 한번만 호출되게 하기위해서  만든 제어변수
	_bool								 m_bRightTrailCheck;
	_bool								 m_bLeftTrailCheck;

	//쉐이더에서텍스쳐에 알파값을 넣어주는 변수 UI 게이지 처럼 되어있음
	_float								 m_fRightLongGage;
	_float								 m_fRightSmallGage;

	_float								 m_fLeftLongGage;
	_float								 m_fLeftSmallGage;

	Engine::CAnimator*					m_pTargetAnimator;

private:
	HRESULT								Ready_Object(void);

public:
	static CDragonDance* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	virtual		void		Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);


	//Right Trail 관련 함수===========================================================
	void					AllRightTrailInit(void); //초기 설정 함수
	void					RightTrailStart(void);   //스킬발동시 초기화 함수
	void					RightTrailUpdate(const _float & fTimeDelta);
	void					RightTrailSmallUpdate(const _float& fTImeDelta);

	//================================================================================

	//Left Trail 관련 함수============================================================
	void					AllLeftTrailInit(void);
	void					LeftTrailStart(void);
	void					LeftTrailUpdate(const _float& fTimeDelta);
	void					LeftTrailSmallUpdate(const _float& fTImeDelta);
	//================================================================================

};



#endif //DragonDance_h__