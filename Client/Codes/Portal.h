#ifndef Portal_h__
#define Portal_h__


#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CTransform;
}

class  CReflect;
class  CWave;
class  CSphereParticle;
class CSound;


class CPortal
	: public Engine::CEffect
{
private:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CPortal(void);

public:
	virtual HRESULT Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	CReflect*			m_pReflect;
	CWave*				m_pWave;
	CWave*				m_pTwist[4];
	CSphereParticle*	m_pParticle;
	

	CSphereParticle*    m_pPortalParticle;

	CSound*				m_pSoundCom;
	Engine::CTransform*			m_pPlayerTrans;

private:
	HRESULT		Ready_Object(void);

public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev
					, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);
public:
	void					SkillStart(void);
public:
	void					SetPortalPosition(_vec3* vPos);
};




#endif // Portal_h__