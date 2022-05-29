#ifndef PledgeBlade_h__
#define PledgeBlade_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CPledgeRing;
class CSprite;
class CWave;

class CPledgeBlade
	: public Engine::CEffect
{
private:
	explicit CPledgeBlade(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CPledgeBlade(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;


	CPledgeRing*						 m_pLeftOne;	//1번째 휘두를때
	CPledgeRing*						 m_pRightTwo;	//2번째 휘두를때
	CPledgeRing*						 m_pLeftThree;  //3번째 휘두를때
	CPledgeRing*						 m_pRightFour;  //4번째 휘두를때

	CSprite*							 m_pLightSprite; //Player 앞에 생겨나는 Light
	CWave*								 m_pWhiteWave;
	CWave*								 m_pWhiteWaveTwo;
	CWave*								 m_pWhiteWaveThree;
	CWave*								 m_pWhiteWaveFour;

	CWave*								 m_pHitBlade[4];

	_bool								 m_bleftOne;
	_bool								 m_bRightTwo;
	_bool								 m_bLeftThree;
	_bool								 m_bRightFour;

	_float								 m_fLeftOneGage;
	_float								 m_fRightTwoGage;
	_float								 m_fLeftThreeGage;
	_float								 m_fRightFourGage;

private:
	HRESULT								Ready_Object(void);


public:
	static CPledgeBlade* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);


public:
	void					SkillStart(int iNumber);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

public:
	void					LeftOneUpdate(const _float & fTimeDelta);

	void					RightTwoUpdate(const _float& fTimeDelta);

	void					LeftThreeUpdate(const _float& fTimeDelta);

	void					RightFourUpdate(const _float& fTimeDelta);


	void					LeftOneInit(void);

	void					RightTwoInit(void);

	void					LeftThreeInit(void);

	void					RightFourInit(void);

	void					LightUpdate(void);

	void					WhiteUpdate(void);

};




#endif