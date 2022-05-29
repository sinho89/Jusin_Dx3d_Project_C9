#ifndef Cube_h__
#define Cube_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CCube_Texture;
	class CTexture;
	class CShader;
}

class CCube
	:public Engine::CGameObject
{

private:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CCube(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void						Stop(void);
	void						Complete(void);
	Engine::CTransform*			GetTransCom(void);


public:
	void		SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);
	_bool		GetChecked(void);
	void		CheckTrue(void);
	void		CheckFalse(void);
	_bool		CheckRayCol(_float* pOut, _vec3* pPivotPos, _vec3* pRayDir, _float rDist);
	OBJ_INFO*	GetObjectInfo(void);

private:
	Engine::CTransform*		m_pTransCom;
	Engine::CRenderer*		m_pRendererCom;
	Engine::CShader*		m_pShaderCom;
	Engine::CCube_Texture*	m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::LAYERTYPE		m_eLayerType;
	OBJ_INFO				m_tInfo;


private:
	_bool					m_bStop;
	_bool					m_bComplete;
	_long					m_lMouseMove;
	_float					m_fAlpha;

private:
	_bool					m_bCheck;


private:
	HRESULT Ready_Object(void);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CCube* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
};


#endif // Environment_h__
