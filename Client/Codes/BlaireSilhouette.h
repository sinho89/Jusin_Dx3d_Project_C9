#ifndef BlaireSilhouette_h__
#define BlaireSilhouette_h__

#include "Defines.h"
#include "Effect.h"


class CRing;
class CSprite;

class CBlaireSilhouette
	: public Engine::CEffect
{
private:
	explicit CBlaireSilhouette(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual  ~CBlaireSilhouette(void);


public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	CRing*								m_pOutLineRing;
	CRing*								m_pSmallRing[4];


	CSprite*							m_pDust[2];



private:
	const vector<Engine::ANIINFO*>*		m_pvecAniInfo;
	Engine::CDynamicMesh*				m_pTargetMesh;
	_bool								m_bSkillCheck;
	Engine::CTransform*					pTargetTransform;
	HRESULT								Ready_Object(void);

public:
	static CBlaireSilhouette* Create(LPDIRECT3DDEVICE9 pGraphicDev
									, const wstring& wstrName);

private:
	virtual		void		Free(void);
	HRESULT					Add_Component(void);

public:
	void					SkillStart(void);
	void					EndAniCheck(void);

	virtual		HRESULT		Start_ForScene(void);


};





#endif // BlaireSilbouette_h__
