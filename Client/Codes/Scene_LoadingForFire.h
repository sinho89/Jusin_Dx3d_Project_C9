/*!
 * \file Scene_LoadingForTown.h
 * \date 2016/11/13 8:49
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

#ifndef Scene_LoadingForFire_h__
#define Scene_LoadingForFire_h__


#include "Defines.h"
#include "Scene.h"

class CLoading;
class CScene_LoadingForFire
	: public Engine::CScene
{
private:
	explicit			CScene_LoadingForFire(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CScene_LoadingForFire(void);

public:
	virtual HRESULT		Ready_Scene(void);
	virtual void		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);

private:
	HRESULT 			SetUp_DefaultSetting(void);
	HRESULT				Ready_Layer_Camera(void);
	HRESULT 			Ready_Layer_GameLogic(void);
	HRESULT 			Ready_Layer_Weapon(void);
	HRESULT 			Ready_Layer_UI(void);
	HRESULT 			Ready_Resource(void);

public:
	void				SaveSlotIndex(_uint iSaveIndex);

private:
	CLoading*			m_pLoading;
private:
	_uint					m_iSlotIndex;

public:
	static CScene_LoadingForFire* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		Free(void);
};

#endif // Scene_LoadingForFire_h__