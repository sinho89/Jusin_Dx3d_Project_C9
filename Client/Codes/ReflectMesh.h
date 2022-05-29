#ifndef ReflectMesh_h__
#define ReflectMesh_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CStaticMesh;
}

class CReflectMesh
	:public Engine::CEffect
{
private:
	explicit CReflectMesh(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CReflectMesh(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	enum PATTERN_REFLECT { PATTERN_ONE, PATTERN_TWO, PATTERN_THREE, PATTERN_END};


private:
	virtual		void Free(void);
	HRESULT		Ready_Object(void);
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT pEffect);

private:
	Engine::CStaticMesh*				m_pMeshCom;
	_float								m_fReflectionIndex;//±¼Àý·ü
	PATTERN_REFLECT						m_ePattern;
	_float								m_fScaleSize;
	_float								m_fAlpha;

public:
	static CReflectMesh*				Create(LPDIRECT3DDEVICE9 pGraphicDev
												, const wstring& wstrName);

	void								SetPattern(PATTERN_REFLECT ePattern);

	void								SetAni(_bool bAni);
	_bool								GetAni(void);
	void							    SetReflectIndex(_float fReflect);

	_float								GetReflectIndex(void);

	void								MinusIndex(_float fResult);

};



#endif // ReflectMesh_h__