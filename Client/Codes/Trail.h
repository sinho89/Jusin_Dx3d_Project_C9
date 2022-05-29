#ifndef Trail_h__
#define Trail_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CDynamicMesh;
	class CVIBuffer;
	class CTrail_Texture;
}

class CTrail
	: public Engine::CEffect
{
private:
	explicit CTrail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CTrail(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual	void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

private:
	Engine::CTrail_Texture*		m_pBufferCom;
	Engine::VTXTEX*				m_pTrailVtx;
	_int						m_iCntX;
	_int						m_iCntZ;
	_int						m_iInterval;
	_int						m_iTexture;
	_int						m_iShaderPath;
	_vec4						m_vColor;
	_int						m_iLocalCoordinates;
	_bool						m_bPatternSilhouette;

private:
	const _matrix*				m_pmatTarget; //타겟의 월드행렬
	wstring						m_strSwordName;
	wstring						m_strBufferName;
	_vec3						vStart;
	_vec3						vEnd;
	const _bool*				m_pPlayerAction;


private:
	HRESULT Ready_Obejct(void);

public:
	static CTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev
						, const wstring& wstrName, wstring& wstrTagName);

private:
	virtual			 void Free(void);
	HRESULT			 Add_Component(void);
	HRESULT			 SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	Engine::VTXTEX*			GetTrail(void);
	virtual		HRESULT		Start_ForScene(void);
	void		SetName(wstring Name);
	void		SetBufferName(wstring Name);
	void		ChangingTrailSetting(void);
};


#endif // Trail_h__
