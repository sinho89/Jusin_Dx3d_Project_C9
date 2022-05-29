#ifndef RiddleKick_h__
#define RiddleKick_h__


#include "Defines.h"
#include "Effect.h"

class CSprite;
class CWave;
class CReflectMesh;

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
}

class CRiddleKick
	: public Engine::CEffect
{
private:
	explicit CRiddleKick(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CRiddleKick(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	CWave*								m_pDustWave;
	CSprite*							m_pDust[2];
	CReflectMesh*						 m_pReflect;
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	_bool								m_bSkillCheck;
	_float								m_fNumber;

private:
	HRESULT		Ready_Object(void);

public:
	static CRiddleKick* Create(LPDIRECT3DDEVICE9 pGraphicDev
								, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);

	virtual		HRESULT		Start_ForScene(void);

	void					WaveUpdate(void);
};


#endif // RiddleKick_h__
