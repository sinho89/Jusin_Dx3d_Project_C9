#include "stdafx.h"
#include "Scene_LoadingForTown.h"
#include "Export_Engine.h"
#include "UICamera.h"
#include "WorldCamera.h"
#include "Loading.h"
#include "Cursor.h"
#include "Scene_Town.h"
#include "Loading_Background.h"
#include "Loading_Bar.h"
#include "Loading_Char.h"
#include "Loading_Effect.h"
#include "Loading_Gage.h"
#include "Loading_Gage_Effect.h"
#include "PlayerCamera.h"
#include "LightCamera.h"
#include "MapCamera.h"

CScene_LoadingForTown::CScene_LoadingForTown(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_pLoading(NULL)
{
	Engine::SetSceneID(Engine::SCENE_LOADING);
}

CScene_LoadingForTown::~CScene_LoadingForTown(void)
{

}

HRESULT CScene_LoadingForTown::Ready_Scene(void)
{
	if (g_bEnterToLoadingTown)
		((CPlayerCamera*)Engine::GetCamera(Engine::CAMERA_PLAYER))->SetPlayBGM(L"Town");

	if(FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if(FAILED(Ready_Resource()))
		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Weapon()))			return E_FAIL;

	if (FAILED(Ready_Camera()))
		return E_FAIL;


	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_TOWN);
	if(NULL == m_pLoading)
		return E_FAIL;

	return S_OK;
}

void CScene_LoadingForTown::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);
	SkipScene(fTimeDelta);

	static_cast<CPlayerCamera*>(Engine::GetCamera(Engine::CAMERA_PLAYER))->Loading_Update(fTimeDelta);
}

void CScene_LoadingForTown::Render_Scene(void)
{
}

void CScene_LoadingForTown::SaveSlotIndex(_uint iSaveIndex)
{
	m_iSlotIndex = iSaveIndex; 
}

void CScene_LoadingForTown::SkipScene(const _float& fTimeDelta)
{
	if(TRUE == m_pLoading->Get_Finish())
	{
		Engine::CScene*	pScene = NULL;

		pScene = CScene_Town::Create(m_pGraphicDev, m_iSlotIndex, m_eClassID);
		if(NULL == pScene)								
			return;
		if(FAILED(Engine::SetUp_CurrentScene(pScene)))	
			return;
	}
}
HRESULT CScene_LoadingForTown::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CScene_LoadingForTown::Ready_Layer_GameLogic(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	if (NULL == pLayer)
		return E_FAIL;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));

	return S_OK;

}
HRESULT CScene_LoadingForTown::Ready_Layer_UI(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CLoading_BackGround::Create(m_pGraphicDev, L"Loading_Back", 0);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLoading_Bar::Create(m_pGraphicDev, L"Loading_Bar");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLoading_Char::Create(m_pGraphicDev, L"Loading_Char");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLoading_Effect::Create(m_pGraphicDev, L"Loading_Effect_BOTTOM", _vec2(0.f, 310.f));
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLoading_Effect::Create(m_pGraphicDev, L"Loading_Effect_TOP", _vec2(-900.f, -385.f));
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLoading_Gage::Create(m_pGraphicDev, L"Loading_Gage", 0);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLoading_Gage_Effect::Create(m_pGraphicDev, L"Loading_Gage_Effect", 0);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;


	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));	

	Engine::DefineCurrentCamera(Engine::CAMERA_UI);

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;
}

HRESULT CScene_LoadingForTown::Ready_Layer_Weapon(void)
{
	Engine::CLayer*	pLayer = Engine::CLayer::Create();
	if (NULL == pLayer) return E_FAIL;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_WEAPON, pLayer));
	return S_OK;
}

HRESULT CScene_LoadingForTown::Ready_Camera(void)
{
	Engine::CCamera* pCamera = NULL;

	if (NULL == Engine::GetCamera(Engine::CAMERA_WORLD))
	{
		pCamera = CWorldCamera::Create(m_pGraphicDev, L"WorldCamera", &_vec3(0.0f, 0.0f, 0.0f), &_vec3(0.0f, 0.0f, 0.0f));
		if (NULL == pCamera) return E_FAIL;

		Engine::ReadyCamera(pCamera, Engine::CAMERA_WORLD);
	}

	if (NULL == Engine::GetCamera(Engine::CAMERA_UI))
	{
		pCamera = CUICamera::Create(m_pGraphicDev, L"UICamera", &_vec3(0.0f, 0.0f, -2.5f), &_vec3(0.0f, 0.0f, 0.0f));
		if (NULL == pCamera) return E_FAIL;

		Engine::ReadyCamera(pCamera, Engine::CAMERA_UI);
	}

	Engine::DefineCurrentCamera(Engine::CAMERA_UI);

	return S_OK;
}

HRESULT CScene_LoadingForTown::Ready_Resource(void)
{
	return S_OK;
}

CScene_LoadingForTown* CScene_LoadingForTown::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_LoadingForTown*			pInstance = new CScene_LoadingForTown(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Town Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CScene_LoadingForTown::Free(void)
{
	::Safe_Release(m_pLoading);

	float fA = Engine::GetLoadScale(); // 로딩바 길이 재기위한 코드

	Engine::CScene::Free();
}


