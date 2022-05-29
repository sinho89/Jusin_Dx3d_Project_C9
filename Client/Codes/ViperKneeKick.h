#ifndef ViperKneeKick_h__
#define ViperKneeKick_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CSprite;
class CReflect;
class CWave;

class CViperKneekick
	: public Engine::CEffect
{
private:
	explicit CViperKneekick(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CViperKneekick(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	CSprite*						    m_pDust;
	CWave*								m_pLightWave[2];
	CReflect*							m_pReflect;
	_bool								m_bSkillStart;
	CWave*								m_pRainBow;


	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;


private:
	HRESULT		Ready_Object(void);

	void		ReflectUpdateCheck(void);

public:
	static CViperKneekick* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);


private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);


public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);
	void					RainBowUpdate(void);

};




#endif