#include "stdafx.h"
#include "Scene_LoadingForFire.h"
#include "Export_Engine.h"

#include "Loading.h"
#include "Scene_Stage_Fire.h"
#include "Loading_Background.h"
#include "Loading_Bar.h"
#include "Loading_Char.h"
#include "Loading_Effect.h"
#include "Loading_Gage.h"
#include "Loading_Gage_Effect.h"
#include "UICamera.h"
#include "PlayerCamera.h"
#include "Scene_Stage_Fire.h"

CScene_LoadingForFire::CScene_LoadingForFire(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_pLoading(NULL)
{
	Engine::SetSceneID(Engine::SCENE_LOADING);
}

CScene_LoadingForFire::~CScene_LoadingForFire(void)
{

}

HRESULT CScene_LoadingForFire::Ready_Scene(void)
{
	if (!g_bEnterToLoadingFire)
	{
		// 처음 들어왔을때만 조건문 들어옴 

		g_bEnterToLoadingFire = true;
	}

	((CPlayerCamera*)Engine::GetCamera(Engine::CAMERA_PLAYER))->SetPlayBGM(L"IntoDungeon");

	if(FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if(FAILED(Ready_Resource()))
		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Weapon()))			return E_FAIL;

	if(FAILED(Ready_Layer_UI()))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_DUNGEON_FIRE);
	if(NULL == m_pLoading)
		return E_FAIL;
	

	return S_OK;
}

void CScene_LoadingForFire::Update_Scene(const _float& fTimeDelta)
{

	Engine::CScene::Update_Scene(fTimeDelta);
	
	if (TRUE == m_pLoading->Get_Finish())
	{
		Engine::CScene*	pScene = NULL;

		pScene = CScene_Stage_Fire::Create(m_pGraphicDev);

		if(NULL == pScene)								
			return;
		if(FAILED(Engine::SetUp_CurrentScene(pScene)))	
			return;
	}
	static_cast<CPlayerCamera*>(Engine::GetCamera(Engine::CAMERA_PLAYER))->Loading_Update(fTimeDelta);
}

void CScene_LoadingForFire::Render_Scene(void)
{
	//Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
}

HRESULT CScene_LoadingForFire::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

HRESULT CScene_LoadingForFire::Ready_Layer_Camera(void)
{
	return S_OK;
}
HRESULT CScene_LoadingForFire::Ready_Layer_GameLogic(void)
{

	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	Engine::CGameObject*	pGameObject = NULL;


	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CScene_LoadingForFire::Ready_Layer_Weapon(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_WEAPON, pLayer));
	return S_OK;
}

HRESULT CScene_LoadingForFire::Ready_Layer_UI(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	if (NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CLoading_BackGround::Create(m_pGraphicDev, L"Loading_Back", 2);
	if (NULL == pGameObject)
		goto except;

	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CLoading_Bar::Create(m_pGraphicDev, L"Loading_Bar");
	if (NULL == pGameObject)
		goto except;

	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CLoading_Char::Create(m_pGraphicDev, L"Loading_Char");
	if (NULL == pGameObject)
		goto except;

	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CLoading_Effect::Create(m_pGraphicDev, L"Loading_Effect_BOTTOM", _vec2(0.f, 310.f));
	if (NULL == pGameObject)
		goto except;

	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CLoading_Effect::Create(m_pGraphicDev, L"Loading_Effect_TOP", _vec2(-900.f, -385.f));
	if (NULL == pGameObject)
		goto except;

	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CLoading_Gage::Create(m_pGraphicDev, L"Loading_Gage", 2);
	if (NULL == pGameObject)
		goto except;

	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	pGameObject = CLoading_Gage_Effect::Create(m_pGraphicDev, L"Loading_Gage_Effect", 2);
	if (NULL == pGameObject)
		goto except;

	if (FAILED(pLayer->AddObject(L"UI", pGameObject)))
		goto except;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));

	Engine::DefineCurrentCamera(Engine::CAMERA_UI);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}
HRESULT CScene_LoadingForFire::Ready_Resource(void)
{
	return S_OK;
}

void CScene_LoadingForFire::SaveSlotIndex(_uint iSaveIndex)
{
	m_iSlotIndex = iSaveIndex; 
}

CScene_LoadingForFire* CScene_LoadingForFire::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_LoadingForFire*			pInstance = new CScene_LoadingForFire(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Town Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CScene_LoadingForFire::Free(void)
{
	::Safe_Release(m_pLoading);

	float fA = Engine::GetLoadScale(); // 로딩바 길이 재기위한 코드

	Engine::CScene::Free();
}



