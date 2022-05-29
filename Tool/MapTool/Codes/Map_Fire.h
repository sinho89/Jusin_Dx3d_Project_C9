#ifndef Map_Fire_h__
#define Map_Fire_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CRenderer;
	class CStaticMesh;
	class CResource_Manager;
	class CShader;
	class CNavigationMesh;
}

class CMouseCol;

class CMap_Fire
	:public Engine::CGameObject
{
public:
	enum	NAVI_MODE{MODE_NONE, MODE_ADD, MODE_MODIFY, MODE_CHECK};
private:
	explicit CMap_Fire(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMap_Fire(void);

public:
	Engine::CStaticMesh**	GetMapMesh(_int* iSize);

public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	D3DXVECTOR3					m_vNaviPos[3];
	NAVI_MODE					m_eNaviMode;
	Engine::NAVI_TYPE			m_eNaviType;

public:
	Engine::CNavigationMesh*	GetNaviCom(void);
	Engine::CStaticMesh**		GetMeshCom(void);

private:
	Engine::CResource_Manager* m_pResourceMgr;
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CStaticMesh*		m_pMeshCom[33];
	//Engine::CStaticMesh*		m_pMeshCom;
	Engine::CNavigationMesh*	m_pNaviCom;
	CMouseCol*					m_pMouseCol;

private:
	_bool					m_bClick;
	_bool					m_bDrag;
private:
	void	NaviSetting(void);
	void	AddCellMode(void);
	void	ModifyMode(void);
	_bool	CheckMode(void);


private:
	HRESULT Ready_Object(void);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);
public:
	static CMap_Fire* Create(LPDIRECT3DDEVICE9 pGraphicDev
		,const wstring& wstrName);
private:
	virtual void Free(void);
};
#endif // MapObj_h__
