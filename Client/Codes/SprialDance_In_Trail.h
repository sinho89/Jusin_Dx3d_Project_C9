#ifndef SprialDance_In_Trail_h__
#define SprialDance_In_Trail_h__


#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CDynamicMesh;
	class CVIBuffer;
	class CTrail_Texture;
}

class CSprialDance_In_Trail
	: public Engine::CEffect
{
private:
	explicit CSprialDance_In_Trail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSprialDance_In_Trail(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	HRESULT Ready_Obejct(void);

public:
	static CSprialDance_In_Trail* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName);

private:
	Engine::CTrail_Texture*		m_pBufferCom;
	Engine::VTXTEX*				m_pTrailVtx;

	_int						m_iCntX;
	_int						m_iCntZ;
	_int						m_iInterval;
	_int						m_iTexture;
	_int						m_iShaderPath;
	_vec4						m_vColor;

private:
	const _matrix*				m_pmat_Sword;
	wstring						m_strSwordName;
	wstring						m_strBufferName;

	_vec3						vStart;
	_vec3						vEnd;

	const _bool*				m_pPlayerAction;

private:
	virtual			 void Free(void);
	HRESULT			 Add_Component(void);
	HRESULT			 SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			 SettingTrail(void);
	void			 SetTargetMatrix(const _matrix* pSwordMat);
	void			 SetAni(_bool bAni);
	_bool			 GetAni(void);
	const _vec4		 GetColor(void) const { return m_vColor; }
	void			 SetColor(const _vec4 vColor) { m_vColor = vColor; }
};

#endif