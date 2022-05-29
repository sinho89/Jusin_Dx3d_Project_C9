#ifndef VioletCross_h__
#define VioletCross_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CWave;
class CSprite;

class CVioletCross
	: public Engine::CEffect
{
private:
	explicit CVioletCross(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CVioletCross(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;


private:
	HRESULT								Ready_Object(void);

private:
	CWave*								m_pRune;
	CWave*								m_pRuneOutLine;
	CWave*								m_pWhiteCircle[3];

	CWave*								m_pSwordStar;
	CWave*								m_pSwordAround_1;
	CWave*								m_pSwordAround_2;
	CWave*								m_pAfterGlow;
	_bool								m_bSwordTimming;

	CWave*								m_pGroundWave;
	CWave*								m_pGroundDustWave[3];
	_bool								m_bSkillStart;
	CWave*								m_pHitWave;
	CWave*								m_pSwordWave[2];

	//=============================================================
	Engine::CTransform*					m_pSword1Transform;
	Engine::CTransform*					m_pSwordL1Transform;
	//=============================================================

	CSprite*							m_pSwordSphere[2];

	_bool								m_bPuppleSphereCheck;

public:
	static CVioletCross* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

	void					RuneUpdate(void);
	void					GroundUpdate(void);
	void					PuppleSphereUpdate(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

};





#endif