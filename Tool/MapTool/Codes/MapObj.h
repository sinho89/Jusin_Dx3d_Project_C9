#ifndef MapObj_h__
#define MapObj_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CResource_Manager;
	class CShader;
}

class CMouseCol;

class CMapObj
	:public Engine::CGameObject
{
private:
	explicit CMapObj(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMapObj(void);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	void	SetID(MAPID eID);

private:
	Engine::CResource_Manager* m_pResourceMgr;
private:
	Engine::CTransform*		m_pTransCom;
	Engine::CRenderer*		m_pRendererCom;
	Engine::CShader*		m_pShaderCom;
	Engine::CStaticMesh*	m_pMeshCom;
	CMouseCol*				m_pMouseCol;
	MAPID					m_eID;


private:
	HRESULT Ready_Object(void);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CMapObj* Create(LPDIRECT3DDEVICE9 pGraphicDev
					,const wstring& wstrName
					,MAPID eID = MAP_ICE);
private:
	virtual void Free(void);
};
#endif // MapObj_h__
