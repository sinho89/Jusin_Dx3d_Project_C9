#ifndef SoftRanding_h__
#define SoftRanding_h__


#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CGameObject;
}

class CWave;
class CSRParticle;
class CSRDust;


class CSoftRanding
	: public Engine::CEffect
{
private:
	explicit CSoftRanding(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSoftRanding(void);

private:
	enum  STATE {LEFT, RIGHT, END_STATE};

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	Engine::CTransform*		m_pTargetTransform;
	CWave*					m_pLight;
	CSRParticle*			m_pSRParticle[END_STATE];
	CSRDust*				m_pSRDust;
	
	Engine::CGameObject*	m_pPlayer;

	Engine::CGameObject*	m_pSword_L;
	Engine::CGameObject*	m_pSword_R;

private:
	HRESULT				Ready_Object(void);

public:
	static CSoftRanding* Create(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);

private:
	virtual		void		Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);

	virtual		HRESULT		Start_ForScene(void);

};





#endif