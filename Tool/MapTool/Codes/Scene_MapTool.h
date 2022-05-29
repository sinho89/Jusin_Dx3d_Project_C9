#ifndef Scene_MapTool_h__
#define Scene_MapTool_h__

#include "Defines.h"
#include "Scene.h"

class CScene_MapTool 
	: public Engine::CScene
{
private:
	explicit CScene_MapTool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_MapTool(void);

public:
	static int m_iPointCount;

public:
	virtual HRESULT Ready_Scene(void);
	virtual void Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);
private:
	HRESULT SetUp_DefaultSetting(void);
	HRESULT Ready_Layer_GameLogic(void);
	HRESULT Ready_Light(void);
	HRESULT Ready_Resource(void);



private:
	void ChangeMap(void);

public:
	static CScene_MapTool* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);

};

#endif // Scene_Logo_h__
