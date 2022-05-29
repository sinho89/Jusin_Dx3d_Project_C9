#ifndef SpinTurnThrustTrail_h__
#define SpinTurnThrustTrail_h__


#include "Defines.h"
#include "Effect.h"


namespace Engine
{
	class CDynamicMesh;
	class CVIBuffer;
	class CTrail_Texture;
}


class CSpinTurnThrustTrail
	: public Engine::CEffect
{
private:
	explicit CSpinTurnThrustTrail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CSpinTurnThrustTrail(void);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);


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
	static CSpinTurnThrustTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName);

private:
	virtual			 void Free(void);
	HRESULT			 Add_Component(void);
	HRESULT			 SetUp_ConstantTable(LPD3DXEFFECT pEffect);

public:
	void			 SettingTrail(void);
	void			 SetTargetMatrix(const _matrix* pTargetMatrix);

	void			 SetAni(_bool bAni);

	_bool			 GetAni(void);

};


#endif // Trail_h__