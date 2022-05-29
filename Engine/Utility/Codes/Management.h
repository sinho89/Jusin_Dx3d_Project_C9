#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CScene;
class CLayer;
class CRenderer;

class ENGINE_DLL CManagement 
	: public CBase
{
public:
	DECLARE_SINGLETON(CManagement)

private:
	explicit				CManagement(void);
	virtual					~CManagement(void);

public:
	CGameObject* 			FindWithTag(const _tchar* pObjKey);
	CGameObject* 			Find(const _tchar* pObjKey);
	list<CGameObject*>* 	FindList(const _tchar* pObjKey);
	CLayer*					FindLayer(WORD eLayerID);
	_bool					Get_Delete(void);
	PSRWLOCK				Get_Lock(PSRWLOCK* pLock);
public:
	void					AddObject(const WORD& wLayerID, const _tchar* pObjectKey, CGameObject* pGameObject);

public:
	HRESULT 				Ready_Management_ForClient(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT					Ready_For_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT 				Ready_Management_ForTool(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT 				SetUp_CurrentScene(CScene* pCurrentScene);
	HRESULT					Return_SelectScene(CScene* pCurrentScene);
	void 					Update_Scene(const _float& fTimeDelta);
	void 					Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*					m_pCurrentScene;
	CRenderer*				m_pRenderer;
	SCENEID					m_eSceneID;
	PROJECTID				m_eProjectID;
private:
	HRESULT					SetUp_SendObject(CScene* pCurrentScene);
public:
	const SCENEID			GetSceneID(void) const { return m_eSceneID; }
	const CScene*			GetScene(void) const { return m_pCurrentScene; }
	void					SetSceneID(const SCENEID eSceneID) { m_eSceneID = eSceneID; }

	void					SetManagerProjectID(const PROJECTID eProjectID);

public:
	void					SafeRelease(void);

public:
	virtual void			Free(void);
};

END

#endif // Management_h__
