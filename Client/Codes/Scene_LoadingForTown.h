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

#ifndef Scene_LoadingForTown_h__
#define Scene_LoadingForTown_h__


#include "Defines.h"
#include "Scene.h"

class CLoading;
class CScene_LoadingForTown
	: public Engine::CScene
{
private:
	explicit			CScene_LoadingForTown(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CScene_LoadingForTown(void);

public:
	virtual HRESULT		Ready_Scene(void);
	virtual void		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);

private:
	HRESULT 			SetUp_DefaultSetting(void);
	HRESULT 			Ready_Layer_UI(void);
	HRESULT 			Ready_Layer_GameLogic(void);
	HRESULT 			Ready_Layer_Weapon(void);
	HRESULT				Ready_Camera(void);
	HRESULT 			Ready_Resource(void);

public:
	void				SaveSlotIndex(_uint iSaveIndex);
	void				SetClassID(CLASSID eClassId) { m_eClassID = eClassId; }
	void				SkipScene(const _float& fTimeDelta);
private:
	CLoading*			m_pLoading;

private:
	_uint					m_iSlotIndex;
	CLASSID					m_eClassID;
public:
	static CScene_LoadingForTown* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		Free(void);
};

#endif // Scene_LoadingForTown_h__