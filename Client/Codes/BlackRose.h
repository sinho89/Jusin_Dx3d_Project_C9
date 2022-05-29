#ifndef BlackRose_h__
#define BlackRose_h__


#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CBlackRoseTrail;
class CSprite;

class CBlackRose
	: public Engine::CEffect
{

private:
	explicit CBlackRose(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CBlackRose(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;

private:
	CBlackRoseTrail*					m_pLeftLeg;
	CBlackRoseTrail*					m_pRightLeg;
	CBlackRoseTrail*					m_pRightSword;

	_bool								m_bLeftLegCheck;
	_bool								m_bRightLegCheck;
	_bool								m_bLegTrailOff;
	_bool								m_bRightSwordCheck;
	_bool								m_bEndTrail;

	CSprite*							m_pDust[2];
	_bool								m_bDustCheck;

private:
	HRESULT								Ready_Object(void);


public:
	static CBlackRose* Create(LPDIRECT3DDEVICE9 pGraphicDev
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