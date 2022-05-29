#ifndef HanumanAttack02_h__
#define HanumanAttack02_h__

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
class CHanumanAttack02
	: public Engine::CEffect
{
private:
	explicit CHanumanAttack02(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHanumanAttack02(void);

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
	static CHanumanAttack02* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	CWave*					m_pAttackWave_0;
	CWave*					m_pAttackWave_1;
	_bool					m_bAttackWave;

	CSprite*				m_pAttackDust[10];
	_bool					m_bAttackDust[5];

	_bool					m_bSkillStart;

	_bool					m_bRadialBlur;
	_float					m_fRadialBlurTime;

	const _matrix*			m_pWorldMat;
	const _matrix*			m_pLeftHandMat;
	const _matrix*			m_pRightHandMat;

	Engine::CGameObject*	m_pHanuman;

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

public:
	void					AttackWaveStart(void);
	void					AttackWaveUpdate(void);

	void					AttackDustStart(_uint iNum);
	void					AttackDustUpdate(void);
};

#endif