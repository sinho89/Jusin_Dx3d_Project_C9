/*!
 * \file Scene_Create.h
 * \date 2016/11/16 9:49
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Scene_Create_h__
#define Scene_Create_h__

#include "Defines.h"
#include "Scene.h"

class CScene_Create
	: public Engine::CScene
{
private:
	explicit				CScene_Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual					~CScene_Create(void);

public:
	virtual HRESULT			Ready_Scene(void);
	virtual void			Update_Scene(const _float& fTimeDelta);
	virtual void			Render_Scene(void);

private:
	HRESULT 				SetUp_DefaultSetting(void);
	HRESULT 				Ready_Layer_GameLogic(void);
	HRESULT 				Ready_Resource(void);

private:
	void					SkipScene();
	void					Button_Progress();
public:
	void					SaveSlotIndex(_uint iSaveIndex);
	void					SavePlayerDat(_uint iIndex);

private:
	_uint					m_iSlotIndex;

public:
	static CScene_Create*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void			Free(void);
};


#endif // Scene_Create_h__