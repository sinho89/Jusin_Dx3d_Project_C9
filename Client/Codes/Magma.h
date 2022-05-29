#ifndef Magma_h__
#define Magma_h__

#include "Defines.h"
#include "Effect.h"


class CSprite;
class CWave;


class CMagma
	: public Engine::CEffect
{
private:
	explicit CMagma(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMagma(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	HRESULT			Ready_Object(void);

public:
	static CMagma* Create(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);

private:
	virtual					void  Free(void);
	HRESULT					Add_Component(void);


private:
	double					m_dTimer;
	CSprite*				m_pMagmaBall[10];

};

#endif