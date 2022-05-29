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
	class CNavigationMesh;
}

class CMouseCol;

class CMap_Store
	:public Engine::CGameObject
{
public:
	enum	NAVI_MODE{MODE_NONE, MODE_ADD, MODE_MODIFY, MODE_CHECK};

private:
	explicit CMap_Store(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName);
	virtual ~CMap_Store(void);

public:
	Engine::CStaticMesh**	GetMapMesh(_int* iSize);
public:
	virtual void Update_Object(const _float& fTimeDelta);
	virtual void Render_Object(void);

public:
	D3DXVECTOR3				m_vNaviPos[3];
	NAVI_MODE				m_eNaviMode;
	Engine::NAVI_TYPE		m_eNaviType;

public:
	Engine::CNavigationMesh*	GetNaviCom(void);

private:
	Engine::CResource_Manager* m_pResourceMgr;
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CRenderer*			m_pRendererCom;
	Engine::CShader*			m_pShaderCom;
	Engine::CStaticMesh*		m_pMeshCom[9];
	Engine::CNavigationMesh*	m_pNaviCom;
	CMouseCol*					m_pMouseCol;


private:
	void	NaviSetting(void);
	void	AddCellMode(void);
	void	ModifyMode(void);
	_bool	CheckMode(void);

private:
	_bool					m_bClick;
	_bool					m_bDrag;


private:
	HRESULT Ready_Object(void);
	HRESULT Add_Component(void);
	HRESULT	Set_ConstantTable(LPD3DXEFFECT pEffect);


public:
	static CMap_Store* Create(LPDIRECT3DDEVICE9 pGraphicDev
					,const wstring& wstrName);
private:
	virtual void Free(void);
};
#endif // MapObj_h__
