#ifndef Scene_AniTool_h__
#define Scene_AniTool_h__

#include "Defines.h"
#include "Scene.h"

class CScene_AniTool
	: public Engine::CScene
{
private:
	explicit CScene_AniTool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene_AniTool(void);
public:
	virtual HRESULT Ready_Scene(void);
	virtual void Update_Scene(const _float& fTimeDelta);
	virtual void Render_Scene(void);
private:
	HRESULT SetUp_DefaultSetting(void);
	HRESULT Ready_Layer_Environment(void);
	HRESULT Ready_Layer_GameLogic(void);
	HRESULT Ready_Light(void);
	HRESULT Ready_Resource(void);

public:
	static CScene_AniTool* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

#endif // Scene_AniTool_h__
