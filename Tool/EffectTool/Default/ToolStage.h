#ifndef ToolStage_h__
#define ToolStage_h__

#include "Defines.h"
#include "Scene.h"


class CToolStage	
	: public Engine::CScene
{
private:
	explicit CToolStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolStage(void);
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
	static CToolStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);

};



#endif // ToolStage_h__
