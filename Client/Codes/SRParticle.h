#ifndef SRParticle_h__
#define SRParticle_h__


#include "Defines.h"
#include "GameObject.h"
#include "Effect.h"


class CSRParticle
	: public Engine::CEffect
{
private:
	explicit CSRParticle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSRParticle(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


private:
	const _matrix* m_pMatTarget;
	const _matrix* m_pMatBone;

	float			m_fOriginAlpha;
	float			m_fAlphaDecrement;


private:
	virtual			  void Free(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);


	static CSRParticle* LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);


	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni = TRUE);

	_bool			GetAni(void);

	void			SetBone(const _matrix* mTargetWorld, const _matrix*  mBoneWorld);

	void			SetAlphaDecrement(_float fAlpha);


private:
	HRESULT		    LoadReady_Object(void);

};




#endif // SRParticle_h__