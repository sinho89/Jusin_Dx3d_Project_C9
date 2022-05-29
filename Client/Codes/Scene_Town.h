/*!
 * \file Stage_Town.h
 * \date 2016/11/12 18:01
 *
 * \author WINTER
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Stage_Town_h__
#define Stage_Town_h__

#include "Defines.h"
#include "Scene.h"

namespace Engine
{
	class CCamera;
}

class CUI;
class CPlayer;
class CScene_Town
	: public Engine::CScene
{
private:
	explicit				CScene_Town(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual					~CScene_Town(void);

public:
	virtual HRESULT			Ready_Scene(void);
	virtual	HRESULT			Start_ForScene(void);
	virtual void			Update_Scene(const _float& fTimeDelta);
	virtual void			Render_Scene(void);

public:
	void					SaveSlotIndex(_uint iSaveIndex);
	void					LoadPlayerInfo(CPlayer* pPlayer);
	void					SavePlayerInfo(void);
	void					SkipScene(const _float& fTimeDelta);

private:
	HRESULT 				SetUp_DefaultSetting(void);
	HRESULT					Ready_Layer_Environment(void);
	HRESULT					Ready_Layer_UI(void);
	HRESULT 				Ready_Layer_GameLogic(void);
	HRESULT 				Ready_Layer_Weapon(void);
	HRESULT					Ready_Layer_Camera(void);
	HRESULT					Ready_LightInfo(void);
	HRESULT 				Ready_Resource(void);
	HRESULT					Ready_Prototype(void);
	HRESULT					Ready_GameObject(void);
	HRESULT					Ready_GameEffect(void);
	HRESULT					Ready_EffectLight(void);

private:
	void					KeyCheck(const _float& fTimeDelta);

private:
	_uint					m_iSlotIndex;
	_uint					m_iLightCount;
	WORD					m_eCameraID;
	_bool					m_bTestKeyCheck;
	CPlayer*				m_pPlayer;
	WORD					m_WindowCnt;
	CLASSID					m_eClassID;
	_bool					m_bFadeUI;
	_float					m_fFadeTime;
	_float					m_fFadeNum;
	_bool					m_bFadeTest;
	_uint					m_iSceneSkip;
public:
	void SetClassID(CLASSID eClassID) { m_eClassID = eClassID; }
	void SetSkip(_uint iSceneSkip) { m_iSceneSkip = iSceneSkip; }

private:
	_bool					m_bOneClick;

public:
	static CScene_Town*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CScene_Town*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iIndex, CLASSID eClassID);

private:
	virtual void			Free(void);
};

#endif // Stage_Town_h__