#ifndef MisticBullet_h__
#define MisticBullet_h__


#include "GameObject.h"
#include "Defines.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CShader;
	class CCollider;
	class CStaticMesh;
}


class CMisticBullet
	: public Engine::CGameObject
{
private:
	explicit CMisticBullet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMisticBullet(void);

public:
	void Shot_Bullet(const _vec3& vPos, const _vec3& vDir, const Engine::COLINFO& tColInfo);
	void Set_MapMesh(LPD3DXMESH* pMesh);
	void ClearMap(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);
	virtual void Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx);
private:
	Engine::CTransform*		 m_pTransCom;
	Engine::CRenderer*		 m_pRendererCom;
	Engine::CShader*		 m_pShaderCom;
	Engine::CCollider*		 m_pColliderCom;
	Engine::CStaticMesh*	 m_pMesh;
private:
	map<_int, LPD3DXMESH*> m_mapMesh;
	typedef map<_int, LPD3DXMESH*> MAPMESH;
	_uint m_iMeshSize;
private:
	Engine::COLINFO m_tColInfo;
	_vec3		m_vOldPos;
	_vec3		m_vDir;

	_float m_fSpeed;

private:
	HRESULT				Ready_Object(void);
	HRESULT				Add_Component(void);
	HRESULT				SetUp_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT				Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
	HRESULT				Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect);
private:
	void				LimitDistance(void);
	void				Collision(void);

public:
	static CMisticBullet* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);

private:
	virtual void Free(void);
};



#endif // MisticBullet_h__
