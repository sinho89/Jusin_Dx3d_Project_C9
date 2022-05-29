/*!
 * \file Scene_Select.h
 * \date 2016/11/12 20:06
 *
 * \author Shinho
 * Contact: user@company.com
 *
 * \brief Select Player for GamePlay
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Scene_Select_h__
#define Scene_Select_h__

#include "Defines.h"
#include "Scene.h"

class CScene_Select
	: public Engine::CScene
{
private:
	explicit					CScene_Select(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual						~CScene_Select(void);

public:
	virtual HRESULT				Ready_Scene(void);
	virtual void				Update_Scene(const _float& fTimeDelta);
	virtual void				Render_Scene(void);
	virtual HRESULT 			Start_ForScene(void);

private:
	HRESULT 					SetUp_DefaultSetting(void);
	HRESULT 					Ready_Layer_GameLogic(void);
	HRESULT						Ready_LightInfo(void);
	HRESULT 					Ready_Resource(void);

public:
	void						SetIndex(_uint iIndex);
	void						ModelRotation();

private:
	_uint						m_iSlotIndex;
	_bool						m_bClick;

private:
	void						SkipScene();

private:
	list<Engine::CGameObject*>	m_pSelectButtonList;

public:
	static CScene_Select*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CScene_Select*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iSlotIndex);
private:
	virtual void				Free(void);
};


#endif // Scene_Select_h__