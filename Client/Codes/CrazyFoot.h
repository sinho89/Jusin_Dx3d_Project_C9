#ifndef CrazyFoot_h__
#define CrazyFoot_h__

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
class CIcicle;

class CCrazyFoot
	: public Engine::CEffect
{
private:
	explicit CCrazyFoot(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CCrazyFoot(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	const vector<Engine::ANIINFO*>*		 m_pvecAniInfo;
	Engine::CDynamicMesh*				 m_pTargetMesh;
	Engine::CTransform*					 m_pTargetTransform;
	Engine::CAnimator*					 m_pTargetAnimator;

	CIcicle*							 m_pIcicle;

private:
	HRESULT								Ready_Object(void);

public:
	static CCrazyFoot* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	CWave*					m_pCrazyFootWave_0;
	_bool					m_bCrazyFootWave_0;

	CWave*					m_pCrazyFootWave_1;
	_bool					m_bCrazyFootWave_1;

	CSprite*				m_pDust[2];
	_bool					m_bDustCheck[2];

	_bool					m_bStartSetting;

	const _matrix*			m_pWorldMat;		//월드
	const _matrix*			m_pLeftFootMat;		//왼발
	const _matrix*			m_pRightFootMat;	//오른발

	Engine::CGameObject*	m_pHanuman;

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
	virtual		HRESULT		Start_ForScene(void);

	void					CrazyFootWaveStart(int iNum);
	void					CrazyFootWaveUpdate(void);

	void					CrazyFootDustStart(int iNum);
	void					CrazyFootDustUpdate(void);

	void					CreateIcicle(void);
};

#endif