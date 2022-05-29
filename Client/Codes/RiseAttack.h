#ifndef RiseAttack_h__
#define RiseAttack_h__

#include "Defines.h"
#include "Effect.h"
//기상공격

namespace Engine
{
	class CVIBuffer;
	class CTrail_Texture;
}

class CRiseAttack
	: public Engine::CEffect
{
private:
	explicit CRiseAttack(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CRiseAttack(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

private:
	Engine::CTrail_Texture*		m_pBufferCom;
	Engine::VTXTEX*				m_pTrailVtx;
	int							m_iCntX;
	int							m_iCntZ;
	int							m_iInterval;



private:
	const _matrix*				m_pmatTarget; //월드행렬
	const _matrix*				m_pmatBoneUp;
	const _matrix*				m_pmatBoneDown;

private:
	HRESULT					Ready_Obejct(void);

public:
	static CRiseAttack* Create(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName);

private:
	virtual			 void Free(void);
	HRESULT			 Add_Component(void);
	HRESULT			 SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:

	virtual		HRESULT		Start_ForScene(void);
};



#endif // RiseAttack_h__
