#ifndef SprialDance_h__
#define SprialDance_h__


#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CAnimator;
	class CDynamicMesh;
	class CAnimator;
}

class CWave;
class CSprialDance_In_Trail;
class CSprialDance_Out_Trail;
class CCherryBlossom;
class CSprialDance
	: public Engine::CEffect
{
private:
	explicit CSprialDance(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSprialDance(void);

public:
	virtual	HRESULT	Start_ForScene(void);
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	Engine::CTransform*					m_pTargetTransform;
	Engine::CAnimator*					m_pTargetAnimator;

private:
	CSprialDance_In_Trail*				m_pIn_L_Trail;
	CSprialDance_In_Trail*				m_pIn_R_Trail;
	CSprialDance_Out_Trail*				m_pOut_L_Trail;
	CSprialDance_Out_Trail*				m_pOut_R_Trail;
	_bool								m_bTrailTimming;
	_bool								m_bTrailOffTimming;

	CCherryBlossom*						m_pCherryBlossom;

	CWave*								m_pGroundWave;
	_bool								m_bWaveTimming;

	const _matrix*						m_pmat_In_L_Sword;
	const _matrix*						m_pmat_In_R_Sword;
	_bool								m_bTrailStart;

	_bool								m_bSkillStart;

private:
	void								GroundWaveUpdate(void);
	void								CherryBlossomUpdate(void);

private:
	HRESULT Ready_Object(void);

public:
	static CSprialDance* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName);
	
private:
	virtual			 void Free(void);
	HRESULT			 Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);
};

#endif