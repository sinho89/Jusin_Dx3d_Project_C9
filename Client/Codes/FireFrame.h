#ifndef FireFrame_h__
#define FireFrame_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CSprite;
class CFrameProjector;

class CFireFrame
	: public Engine::CEffect
{
private:
	explicit CFireFrame(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CFireFrame(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;
	const _matrix*						m_pLeftHandMat;
	CFrameProjector*					m_pFireProjecter;
	_bool								m_bFireStart;
	_vec3								m_vFireDir;
	_float								m_fAngle;
	_bool								m_bLast;

	CSprite*							m_pDust[5];

	_double								m_dDustTime;

private:
	HRESULT								Ready_Object(void);

public:
	static					CFireFrame* Create(LPDIRECT3DDEVICE9 pGraphicDev
											, const wstring& wstrName);

private:
	virtual							void  Free(void);
	HRESULT							Add_Component(void);

public:
	void							SkillStart(void);
	void							EndAniCheck(void);
	virtual		HRESULT				Start_ForScene(void);


public:
	void							FireUpdate(void);
	void							LookAtAngle(_float fAngle);
	void							SpriteDustStart(int iIndex);
	void							SpriteDustUpdate(void);
};

#endif