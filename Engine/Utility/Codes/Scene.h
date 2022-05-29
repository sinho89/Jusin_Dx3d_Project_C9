#ifndef Scene_h__
#define Scene_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class CLayer;
class CGameObject;

class ENGINE_DLL CScene 
	: public CBase
{
protected:
	explicit								CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual									~CScene(void);
public:
	CGameObject*							FindWithTag(const _tchar* pObjKey);
	CGameObject*							Find(const _tchar* pObjKey);
	list<CGameObject*>* 					FindList(const _tchar* pObjKey);
	CLayer*									FindLayer(WORD eLayerID);
	map<WORD, CLayer*>*			FindLayerMap();
	_bool									Get_Delete(void);
	PSRWLOCK								Get_Lock(PSRWLOCK* pLock);
public:
	HRESULT									Set_Object(CScene * pCurrentScene);
	void									AddObject(const WORD& wLayerID, const _tchar* pObjectKey, CGameObject* pGameObject);
public:
	virtual HRESULT 						Ready_Scene(void);
	virtual HRESULT 						Start_ForScene(void);	//씬 생성 전에 레이어에 접근 할 때 필요한 초기화 함수.
	virtual void 							Update_Scene(const _float& fTimeDelta);
	virtual void 							Render_Scene(void)PURE;
protected:
	LPDIRECT3DDEVICE9						m_pGraphicDev;
protected:
	map<WORD, CLayer*>			m_mapLayer;
	typedef map<WORD, CLayer*>	MAPLAYER;

protected:
	_bool									m_bSkipScene;

public:
	void SetSkipScene(_bool bSkipScene) { m_bSkipScene = bSkipScene; }
public:
	void									SafeRelease(void);
public:
	virtual void							Free(void);
};


END

#endif // Scene_h__
