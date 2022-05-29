#ifndef BlackRoseTrail_h__
#define BlackRoseTrail_h__

#include "Defines.h"
#include "Effect.h"

namespace Engine
{
	class CDynamicMesh;
	class CVIBuffer;
	class CTrail_Texture;
}


class CBlackRoseTrail
	: public Engine::CEffect
{
public:
	enum  TRAILPART { DIR_RightReg, DIR_LeftReg, DIR_RightSword };

private:
	explicit CBlackRoseTrail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, TRAILPART eDir);
	virtual ~CBlackRoseTrail(void);

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
	TRAILPART					m_eDir;


private:
	const _matrix*				m_pmatTarget; //타겟의 월드행렬
	const _matrix*				m_pmatBoneUp; //타겟의 본행렬
	const _matrix*				m_pmatBoneDown;
	wstring						m_strSwordName;
	wstring						m_strBufferName;
	_vec3						vStart;
	_vec3						vEnd;
	const _bool*				m_pPlayerAction;

private:
	HRESULT Ready_Obejct(void);

public:
	static CBlackRoseTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, TRAILPART eDir);

private:
	virtual			 void Free(void);
	HRESULT			 Add_Component(void);
	HRESULT			 SetUp_ConstantTable(LPD3DXEFFECT pEffect);


public:
	void			 SettingTrail(void);
	void			 SetTargetMatrix(const _matrix* pTargetMatrix);
	void			 SetTargetBoneUpMatrix(const _matrix* pTargetMatrix);
	void			 SetTargetBoneDownMatrix(const _matrix* pTargetMatrix);
	void			 SetAni(_bool bAni);
	_bool			 GetAni(void);
	void			 SetTrailDir(TRAILPART  Dir);
};
#endif