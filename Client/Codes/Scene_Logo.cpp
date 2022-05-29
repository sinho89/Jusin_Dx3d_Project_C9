#include "stdafx.h"
#include "Scene_Logo.h"
#include "Opening_Banner.h"

#include "Export_Engine.h"
#include "Loading.h"

#include "Cursor.h"
#include "Scene_Login.h"
#include "Scene_Select.h"
#include "Scene_Create.h"
#include "Scene_LoadingForTown.h"
#include "Layer.h"
#include "MediaUI.h"
#include "UICamera.h"
#include "PlayerCamera.h"
#include "LightCamera.h"
#include "WorldCamera.h"
#include "MapCamera.h"

CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_fSkipTime(0.f)
, m_pLoading(NULL)
, m_pOpenningMedia(NULL)
{
	Engine::SetSceneID(Engine::SCENE_LOGO);
}

CScene_Logo::~CScene_Logo(void)
{

}

HRESULT CScene_Logo::Ready_Scene(void)
{
	if(FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if(FAILED(Ready_Resource()))
		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))
		return E_FAIL;

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_LOGO);
	if(NULL == m_pLoading) return E_FAIL;

	return S_OK;
}

HRESULT CScene_Logo::Start_ForScene(void)
{
	Engine::CScene::Start_ForScene();

	return S_OK;
}

void CScene_Logo::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);
	SkipScene(fTimeDelta);
}

void CScene_Logo::Render_Scene(void)
{

}

HRESULT CScene_Logo::Ready_Layer_GameLogic(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.BackGround_Logo Instance
	
	pGameObject = COpening_Banner::Create(m_pGraphicDev, L"OpeningBanner");
	if(NULL == pGameObject)								
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))	
		goto except;

	pGameObject = CMediaUI::Create(m_pGraphicDev, L"Openning_Media", L"../Bin/Resource/Vedio/Openning_Movie.avi", FALSE);
	if(NULL == pGameObject)								
		goto except;
	
	m_pOpenningMedia = static_cast<CMediaUI*>(pGameObject);
	if(FAILED(pLayer->AddObject(L"UI", pGameObject)))	
		goto except;

	pGameObject = CCursor::Create(m_pGraphicDev, L"Cursor", false);
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));	

	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;	
}

HRESULT CScene_Logo::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CScene_Logo::Ready_Resource(void)
{
	
	return S_OK;
}

CScene_Logo* CScene_Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Logo*			pInstance = new CScene_Logo(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Logo Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}
void CScene_Logo::SkipScene(const _float& fTimeDelta)
{
	m_fSkipTime += fTimeDelta;

	if(TRUE == m_pLoading->Get_Finish())
	{
		if(Engine::Get_DIKeyState(DIK_RETURN) || m_pOpenningMedia->EndCheck())
		{
			Engine::CScene*	pScene = NULL;

			pScene = CScene_Login::Create(m_pGraphicDev);

			if(NULL == pScene)								
				return;
			if(FAILED(Engine::SetUp_CurrentScene(pScene)))	
				return;
		}		
	}
}
void CScene_Logo::Free(void)
{
	::Safe_Release(m_pLoading);

	Engine::CScene::Free();
}



