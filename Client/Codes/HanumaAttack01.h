#ifndef HanumaAttack01_h__
#define HanumaAttack01_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CHanumaAttack01_Trail;
class CHanumaAttack01
	: public Engine::CEffect
{
private:
	explicit CHanumaAttack01(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHanumaAttack01(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

	CHanumaAttack01_Trail*				m_pLeft_0_Trail;
	CHanumaAttack01_Trail*				m_pLeft_1_Trail;
	CHanumaAttack01_Trail*				m_pLeft_2_Trail;

	CHanumaAttack01_Trail*				m_pRight_0_Trail;
	CHanumaAttack01_Trail*				m_pRight_1_Trail;
	CHanumaAttack01_Trail*				m_pRight_2_Trail;

	_bool								m_bLeftSwing;
	_bool								m_bLeftDeleteSwing;
	_bool								m_bRightSwing;
	_bool								m_bRightDeleteSwing;

	Engine::CGameObject*				m_pHanuman;

private:
	HRESULT								Ready_Object(void);

public:
	static CHanumaAttack01* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const wstring& wstrName);

private:
	virtual								void  Free(void);
	HRESULT								Add_Component(void);

public:
	void								SkillStart(void);
	void								EndAniCheck(void);
	virtual		HRESULT					Start_ForScene(void);
};

#endif