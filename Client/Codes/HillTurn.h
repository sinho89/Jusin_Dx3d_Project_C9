#ifndef HillTurn_h__
#define HillTurn_h__

#include "Defines.h"
#include "Effect.h"

class CWave;
class CCherryBlossom;

class CHillTurn
	: public Engine::CEffect
{
private:
	explicit CHillTurn(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CHillTurn(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT		Ready_Object(void);

private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;

	//Wave
	CWave*								m_pOutLine;
	CWave*								m_pInLine;

	//CherryBlossom
	CCherryBlossom*						m_pCherryBlossom;


	Engine::CTransform*					m_pTargetTransform;

	_bool								m_bSkillStart;
	_bool								m_bInLineStart;

	_vec3								m_vTargetPos;


public:
	static CHillTurn* Create(LPDIRECT3DDEVICE9 pGraphicDev
							, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);

	virtual		HRESULT		Start_ForScene(void);


	void					OutLineUpdate(void);
	void					InLineUpdate(void);
	void					CherryBlossomUpdate(void);

};

#endif