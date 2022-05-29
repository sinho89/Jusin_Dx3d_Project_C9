#include "stdafx.h"
#include "Scene_Login.h"
#include "Export_Engine.h"
#include "Scene_Select.h"
#include "Scene_LoadingForTown.h"

#include "Cursor.h"
#include "Login_BackGround.h"
#include "Login_TopBack.h"
#include "Login_BottomBack.h"
#include "Login_ADM.h"
#include "Login_Banner.h"
#include "Login_Window.h"
#include "Login_Enter_Button.h"

#include "UICamera.h"

#include "Layer.h"
#include "LightCamera.h"
#include "WorldCamera.h"
#include "PlayerCamera.h"
#include "MapCamera.h"
#include "UIEffect.h"

CScene_Login::CScene_Login(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{
	Engine::SetSceneID(Engine::SCENE_LOGIN);
}

CScene_Login::~CScene_Login(void)
{

}

HRESULT CScene_Login::Ready_Scene(void)
{
	if(FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if(FAILED(Ready_Resource()))
		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))
		return E_FAIL;

	g_bEnterToTown = false;
	g_bEnterToIce = false;
	g_bStageResource = false;
	m_fEffTime = 0.f;
	m_fStartTime = 0.f;
	m_bEffStart = false;
	m_bStar1 = false;
	m_bStar2 = false;
	m_bStar3 = false;
	m_bStar4 = false;
	m_bStar5 = false;
	m_fStar1 = 0.f;
	m_fStar2 = 0.f;
	m_fStar3 = 0.f;
	m_fStar4 = 0.f;
	m_fStar5 = 0.f;
	return S_OK;
}

void CScene_Login::Update_Scene(const _float& fTimeDelta)
{
	m_fStartTime += fTimeDelta;

	if (m_fStartTime >= 3.f)
		m_bEffStart = true;

	if (m_bEffStart)
	{
		m_fStar1 += fTimeDelta;
		m_fStar2 += fTimeDelta;
		m_fStar3 += fTimeDelta;

		if (m_fStar1 >= 0.25f)
		{
			m_bStar1 = true;
			m_fStar1 = 0.f;
		}
		if (m_fStar2 >= 0.5f)
		{
			m_bStar2 = true;
			m_fStar2 = 0.f;
		}
		if (m_fStar3 >= 0.75f)
		{
			m_bStar3 = true;
			m_fStar3 = 0.f;
		}
		if (m_fStar4 >= 1.f)
		{
			m_bStar4 = true;
			m_fStar4 = 0.f;
		}
		if (m_fStar5 >= 1.25f)
		{
			m_bStar5 = true;
			m_fStar5 = 0.f;
		}
	
		if (m_bStar1)
		{
			int iRandX = (rand() % 800 + 400);
			int iRandY = (rand() % 300 + 150);

			Engine::CGameObject* pGameObject = CUIEffect::Create(m_pGraphicDev, L"LoginEnterButton", NULL, 21, 1.f, 1.f, 16.f, 4, 4, 16, _float(iRandX), _float(iRandY), true);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Effect", pGameObject);
			m_bStar1 = false;
		}
		if (m_bStar2)
		{
			int iRandX = (rand() % 800 + 400);
			int iRandY = (rand() % 300 + 150);

			Engine::CGameObject* pGameObject = CUIEffect::Create(m_pGraphicDev, L"LoginEnterButton", NULL, 21, 1.f, 1.f, 16.f, 4, 4, 16, _float(iRandX), _float(iRandY), true);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Effect", pGameObject);
			m_bStar2 = false;
		}
		if (m_bStar3)
		{
			int iRandX = (rand() % 800 + 400);
			int iRandY = (rand() % 300 + 150);

			Engine::CGameObject* pGameObject = CUIEffect::Create(m_pGraphicDev, L"LoginEnterButton", NULL, 21, 1.f, 1.f, 16.f, 4, 4, 16, _float(iRandX), _float(iRandY), true);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Effect", pGameObject);
			m_bStar3 = false;
		}
		if (m_bStar4)
		{
			int iRandX = (rand() % 800 + 400);
			int iRandY = (rand() % 300 + 150);

			Engine::CGameObject* pGameObject = CUIEffect::Create(m_pGraphicDev, L"LoginEnterButton", NULL, 21, 1.f, 1.f, 16.f, 4, 4, 16, _float(iRandX), _float(iRandY), true);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Effect", pGameObject);
			m_bStar4 = false;
		}
		if (m_bStar5)
		{
			int iRandX = (rand() % 800 + 400);
			int iRandY = (rand() % 300 + 150);

			Engine::CGameObject* pGameObject = CUIEffect::Create(m_pGraphicDev, L"LoginEnterButton", NULL, 21, 1.f, 1.f, 16.f, 4, 4, 16, _float(iRandX), _float(iRandY), true);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Effect", pGameObject);
			m_bStar5 = false;
		}
		
	}
	

	Engine::CScene::Update_Scene(fTimeDelta);
	SkipScene();
	static_cast<CPlayerCamera*>(Engine::GetCamera(Engine::CAMERA_PLAYER))->Loading_Update(fTimeDelta);
}

void CScene_Login::Render_Scene(void)
{

}

HRESULT CScene_Login::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CScene_Login::Ready_Layer_GameLogic(void)
{

	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.BackGround_Logo Instance


	pGameObject = CLogin_BackGround::Create(m_pGraphicDev, L"LoginBack");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLogin_TopBack::Create(m_pGraphicDev, L"LoginTopBack");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLogin_BottomBack::Create(m_pGraphicDev, L"LoginBottomBack");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLogin_ADM::Create(m_pGraphicDev, L"LoginADM");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLogin_Banner::Create(m_pGraphicDev, L"LoginBanner");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLogin_Window::Create(m_pGraphicDev, L"LoginWindow");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CLogin_Enter_Button::Create(m_pGraphicDev, L"LoginEnterButton");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;


	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_UI, pLayer));

	Engine::CCamera* pCamera = NULL;

	if (NULL == Engine::GetCamera(Engine::CAMERA_PLAYER))
	{
		pCamera = CPlayerCamera::Create(m_pGraphicDev, L"PlayerCamera");
		if (NULL == pCamera) return E_FAIL;

		Engine::ReadyCamera(pCamera, Engine::CAMERA_PLAYER);
	}

	if (NULL == Engine::GetCamera(Engine::CAMERA_LIGHT))
	{
		pCamera = CLightCamera::Create(m_pGraphicDev, L"LightCamera", &_vec3(0.0f, 0.0f, 0.0f), &_vec3(0.0f, 0.0f, 0.0f));
		if (NULL == pCamera) return E_FAIL;

		Engine::ReadyCamera(pCamera, Engine::CAMERA_LIGHT);
	}

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

	if (NULL == Engine::GetCamera(Engine::CAMERA_MINIMAP))
	{
		pCamera = CMapCamera::Create(m_pGraphicDev, L"MiniMapCamera");
		if (NULL == pCamera) return E_FAIL;

		Engine::ReadyCamera(pCamera, Engine::CAMERA_MINIMAP);
	}

	Engine::DefineCurrentCamera(Engine::CAMERA_UI);
	
	return S_OK;

except:
	::Safe_Release(pLayer);
	return E_FAIL;	
}

HRESULT CScene_Login::Ready_Resource(void)
{
	return S_OK;
}

void CScene_Login::SkipScene()
{
	CUI*	pUI = ((CUI*)Engine::Find(L"LoginEnterButton"));

	if(pUI->GetSkipScene())
	{
		Engine::Safe_Release(pUI);

		Engine::CScene*	pScene = NULL;

		pScene = CScene_Select::Create(m_pGraphicDev);

		if(NULL == pScene)								
			return;
		if(FAILED(Engine::Return_SelectScene(pScene)))	
			return;
	}
	else
		Engine::Safe_Release(pUI);
}
CScene_Login* CScene_Login::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Login*			pInstance = new CScene_Login(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Login Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CScene_Login::Free(void)
{
	Engine::CScene::Free();
}


