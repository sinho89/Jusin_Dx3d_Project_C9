#ifndef FlyDance_h__
#define FlyDance_h__

#include "Defines.h"
#include "Effect.h"

class CReflectMesh;

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}


class CFlyDanceTrail;
class CSprite;
class CWave;

class CFlyDance
	: public Engine::CEffect
{
private:
	explicit CFlyDance(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CFlyDance(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	CReflectMesh*						 m_pReflect;
	CFlyDanceTrail*						 m_pTrail;
	CSprite*							 m_pDust[2];
	CWave*								 m_pFlash;


	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 pTransform;
	Engine::CTransform*					 pTargetSwordTransform;

	_double								m_fTrailStartTime;
	_double								m_fTrailEndTime;

	_bool								m_bReflectStart;
	_float								m_fYUp;


private:
	HRESULT								Ready_Object(void);

public:
	static CFlyDance* Create(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);


public:
	void					SkillStart(_int iNumber);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);


};




#endif // FlyDance_h__