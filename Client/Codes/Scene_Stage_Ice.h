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
#ifndef Stage_Stage_Ice_h__
#define Stage_Stage_Ice_h__

#include "Defines.h"
#include "Scene.h"


namespace Engine
{
	class CCamera;
}

class CMonster;
class CScene_Stage_Ice
	: public Engine::CScene
{
private:
	explicit			CScene_Stage_Ice(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual				~CScene_Stage_Ice(void);

public:
	virtual HRESULT		Ready_Scene(void);
	virtual void		Update_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);

public:
	void				LoadPlayerInfo(void);
	void				SaveSlotIndex(_uint iSaveIndex);

private:
	HRESULT 			SetUp_DefaultSetting(void);
	HRESULT				Ready_Layer_Environment(void);
	HRESULT 			Ready_Layer_GameLogic(void);
	HRESULT 			Ready_Layer_Weapon(void);
	HRESULT				Ready_Layer_UI(void);
	HRESULT				Ready_Layer_Camera(void);
	HRESULT				Ready_LightInfo(void);
	HRESULT 			Ready_Resource(void);
	HRESULT				Ready_Prototype(void);
	HRESULT				Ready_GameObject(void);
	HRESULT				Ready_EffectLight(void);
	virtual				HRESULT		Start_ForScene(void);

	HRESULT				Ready_Light_Toch(void);
	HRESULT				Ready_Light_Cristal_0(void);
	HRESULT				Ready_Light_Cristal_1(void);
	HRESULT				Ready_Light_Wall(void);
	HRESULT				Ready_Light_Health(void);

private:
	void				KeyCheck(const _float& fTimeDelta);

private:
	_uint					m_iSlotIndex;
	WORD					m_eCameraID;
	_bool					m_bExit;
	_bool					m_bFadeUI;
	_float					m_fFadeTime;
	_float					m_fFadeNum;
	_uint					m_iLightCount;


	CMonster*				m_pHanuman;

	list<_vec3>				m_listBoneA;

public:
	void SetExit(_bool bExit) { m_bExit = bExit; }

public:
	static CScene_Stage_Ice* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void		Free(void);
};

#endif // Stage_Town_h__