#ifndef SRDust_h__
#define SRParticle_h__

#include "Defines.h"
#include "GameObject.h"
#include "Effect.h"

class CSRDust
	: public Engine::CEffect
{
private:
	explicit		CSRDust(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual			~CSRDust(void);

public:
	virtual void	 Update_Object(const _float& fTimeDelta);
	virtual void	  Render_Object(void);


private:
	float							m_fOriginAlpha;
	float							m_fAlphaDecrement;
	const Engine::CTransform*		m_pTargetTransform;
	int								m_iEffectCount;
	float							m_fTickCount;
	

private:
	virtual			  void Free(void);
	HRESULT			  Add_Component(void);
	HRESULT			  SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			  SetPart(_int nIdx);
	void			  MoveUpdate(const _float& fTimeDelta);
	void			  Update_RePart(void);

	static CSRDust*   LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);


	//애니메이션 활성화 / 비활성화
	void			SetAni(_bool bAni = TRUE);

	_bool			GetAni(void);

	void			SetAlphaDecrement(_float fAlpha);

	HRESULT		    LoadReady_Object(void);

	void			LifeCheck(void);

	void			SetTargetTransform(const Engine::CTransform* pTargetTransform);
};




#endif