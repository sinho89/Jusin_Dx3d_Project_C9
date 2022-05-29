#ifndef NPC_h__
#define NPC_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CDynamicMesh;
	class CResource_Manager;
	class CShader;
	class CGameObject;
}

class CMouseCol;
class CSound;

class CNPC
	:public Engine::CGameObject
{

protected:
	explicit					CNPC(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual						~CNPC(void);
public:
	void						PlayNPCSound(void);

public:
	virtual void				Update_Object(const _float& fTimeDelta);
	virtual void				Render_Object(void);
	virtual	void				Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);

public:
	Engine::CTransform*			GetTransCom(void);
	Engine::CGameObject*		GetUIEff(void) { return m_pUIEff; }
public:
	void						SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle);
	OBJ_INFO*					GetObjectInfo(void);

protected:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CDynamicMesh*		m_pMeshCom;
	Engine::CGameObject*		m_pUIEff;
	CSound*						m_pSoundCom;

private:
	_float						m_fFrame;
	_long						m_lMouseMove;
	_float						m_fAlpha;

private:
	_bool						m_bCheck;

protected:
	virtual HRESULT				Ready_Object(void);
	HRESULT						Add_Component(void);
	HRESULT						Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT						Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	
public:
	static CNPC*				Create(LPDIRECT3DDEVICE9 pGraphicDev
										,const wstring& wstrName);
protected:
	virtual void				Free(void);
};


#endif // Environment_h__
