#ifndef DarkPollingGrave_h__
#define DarkPollingGrave_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}


class CDarkSword;
class CWave;

class CDarkPollingGrave
	: public Engine::CEffect
{
private:
	explicit CDarkPollingGrave(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CDarkPollingGrave(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	//===========================================================================
	//Player 정보 관련 함수
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;
	const _bool*						 m_pPlayerAction; //Player의 동작을 받아오는 변수
	//===========================================================================

	//===========================================================================
	//스킬 초기에 생기는 이펙트들================================================
	CWave*								m_pWave;			//초기에 Player 주위로 퍼져나가는 검은색 웨이브
	CWave*								m_pWhiteLine[2];	//초기에 스킬 사용시 퍼져나가는 흰색 웨이브
	CWave*								m_pBlackLine;		//Player 반경으로 생기는 검은 원의 라인
	CWave*								m_pBlueFlowerWave;	//초기에 Player를 중심으로 생겨나는 파란색 꽃 문양
	//마법진======================================================================
	CWave*								m_pRuneOutLine;
	CWave*								m_pMainRune;
	CWave*								m_pRedCircle;
	//============================================================================


	//검 메시
	//===========================================================================
	CDarkSword*							m_pDarkSword[20];			
	CDarkSword*							m_pDarkOutLineSword[20];	
	CDarkSword*							m_pDarkMiddleLineSword[20];	
	CDarkSword*							m_pCenterSword[20];
	//===========================================================================

	//===========================================================================
	_bool								 m_bSkillCheck; //초기에 스킬발동시 생성되는 이펙트들 관리 변수
	_bool								 m_bBlueFlowerCheck; //BlueFlower 웨이브 제어 변수
	//============================================================================

	//=============================================================================
	_bool								 m_bDarkSwordCheck;
	_bool								 m_bSwordTargetCheck;
	_matrix								 PlayerMatrix;
	_vec3								 m_pTargetLook;
	_vec3								 m_vTargetPosition;
	//===========================================================================

	//Center Sword 관련 변수=====================================================
	_bool								 m_bCenterSwordCheck; //검이펙트가 생성되는 순간 판단 변수
	_bool								 m_bCenterShoot; //CenterSword가 꽂히는 순간을 판단하는 변수

	//Player의 월드 행렬을 저장하는 matrix 변수
	_matrix								 CenterMatrix;
	//가장 안쪽 검행렬의 최종 결과 matrix변수
	_matrix								 CenterResultMatrix;
	//Inside의 월드행렬을 저장하는 matrix 변수
	_matrix								 InsideResultMatrix;
	//Middle의 월드행렬을 저장하는 matrix 변수
	_matrix								 MiddleResultMatrix;
	//Out의 월드행렬을 저장하는 matrix변수
	_matrix								 OutResultMatrix;


	_bool								 m_bDarkPollingSwordEnd; //검 종료 변수
	//============================================================================

	//검 메쉬의 로컬상의 좌표 저장 변수
	//가장안쪽 검의 좌표 저장
	_vec3								m_vCenterSwordPosition[20];
	_vec3								m_vInsideSwordPosition[20];
	_vec3								m_vMiddleSwordPosition[20];
	_vec3								m_vOutSwordPosition[20];
	_bool								m_bSwordPositionCheck;



	//검 메시 별로 스케일 줄여줄 숫자값 저장
	//가장 바깥쪽 검메시
	_float								m_fOutSwordNumber[20];
	//가장 안쪽 검메시
	_float								m_fCenterSwordNumber[20];
	//안쪽 검메시
	_float								m_pInsideSwordNumber[20];

	//중간 검메시
	_float								m_pMiddleSwordNumber[20];

private:
	HRESULT								Ready_Object(void);

public:
	static CDarkPollingGrave* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);


public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);


public:
	//==초기 사용 이펙트 함수=======================================================
	void					BlackWaveUpdate(void); //초기에 퍼져나가는 검은 웨이브
	void					WhiteLineUpdate(void);
	void					BlackLineUpdate(void);
	void					BlueFlowerUpdate(void);
	//==============================================================================

	void					RuneOutLine(void);
	void					RuneOutLineUpdate(void);

	//가장 안쪽 SwordUpdate Update
	void					CenterSwordUpdate(const _float& fTimeDelta);
	//Inside Sword Update
	void					InsideSwordUpdate(const _float& fTimeDelta);
	//Middle Sword Update
	void					MiddleSwordUpdate(const _float& fTimeDelta);

	//Out Sword Update
	void					OutSwordUpdate(const _float& fTimeDelta);
};

#endif