#include "stdafx.h"
#include "Scene_Select.h"
#include "Export_Engine.h"
#include "Scene_Create.h"
#include "Scene_LoadingForTown.h"

#include "Cursor.h"
#include "Select_BackGround.h"
#include "Select_Side_Bar.h"
#include "Select_Char_Button.h"
#include "Select_Char_Model.h"
#include "Game_Start_Banner.h"
#include "Model_Floor.h"
#include "UICamera.h"
#include "Layer.h"
#include "Export_Engine.h"
#include "LightCamera.h"

CScene_Select::CScene_Select(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
, m_iSlotIndex(0)
, m_bClick(false)
{
	Engine::SetSceneID(Engine::SCENE_SELECT);
}

CScene_Select::~CScene_Select(void)
{

}

HRESULT CScene_Select::Ready_Scene(void)
{
	if(FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if(FAILED(Ready_LightInfo()))				
		return E_FAIL;

	if(FAILED(Ready_Resource()))
		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))
		return E_FAIL;

	Engine::Reset_Resource(RESOURCE_LOGO);
	Engine::Reset_Resource(RESOURCE_STAGESTATIC);
	Engine::Reset_Resource(RESOURCE_TOWN);
	
	g_bEnterToTown = false;
	g_bEnterToIce = false;
	g_bEnterToLoadingTown = false;
	g_bEnterToLoadingIce = false;
	g_bEnterToLoadingFire = false;
	g_bStageResource = false;

	Engine::SetDebugBuffer(FALSE);
	Engine::SetShadowBuffer(FALSE);
	Engine::SetCACDShadow(FALSE);
	Engine::SetSoftShadow(FALSE);
	Engine::SetMotionBlurBuffer(FALSE);
	Engine::SetGaussianBlur(FALSE);
	Engine::SetHDR(FALSE);
	Engine::SetDOF(FALSE);
	Engine::SetSSAO(FALSE);
	Engine::SetExpFog(FALSE);
	Engine::SetSquareFog(FALSE);
	Engine::SetHeightFog(FALSE);
	Engine::SetLimLight(FALSE);
	Engine::SetBlurValue(0.0f);

	return S_OK;
}

void CScene_Select::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);
	SkipScene();
	ModelRotation();
}

void CScene_Select::Render_Scene(void)
{
	_tchar		szSlotIndex[128];

	wsprintf(szSlotIndex, L"Slot Index: %d", m_iSlotIndex);

	Engine::Render_Font(L"Font_UI_ID_PASSWORD", szSlotIndex, &_vec2(50, 200), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CScene_Select::Start_ForScene(void)
{
	if (FAILED(Engine::CScene::Start_ForScene()))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Select::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

void CScene_Select::SkipScene()
{
	list<Engine::CGameObject*>::iterator iter	  = m_pSelectButtonList.begin();
	list<Engine::CGameObject*>::iterator iter_end = m_pSelectButtonList.end();

	for(; iter != iter_end; ++iter)
	{
		if(((CUI*)(*iter))->GetSkipScene())
		{
			Engine::CScene*	pScene = NULL;
			_int iA = ((CUI*)(*iter))->GetIndex();
			pScene = CScene_Create::Create(m_pGraphicDev);
			((CScene_Create*)pScene)->SaveSlotIndex(iA);

			if(NULL == pScene)								
				return;
			if(FAILED(Engine::SetUp_CurrentScene(pScene)))	
				return;

			return;
		}
	}

	CUI*	pUI = ((CUI*)Engine::Find(L"GameStartBanner"));

	if(pUI->GetSkipScene())
	{
		list<Engine::CGameObject*>::iterator iter = m_pSelectButtonList.begin();
		list<Engine::CGameObject*>::iterator iter_end = m_pSelectButtonList.end();

		for (_uint i = 0; i < (pUI->GetIndex() - 1); ++i)
			++iter;

		if (((CSelect_Char_Button*)(*iter))->GetPlayerInfo()->m_eClassID != CLASS_WITHBLADE)
			return;

		Engine::CScene*	pScene = NULL;
		pScene = CScene_LoadingForTown::Create(m_pGraphicDev);
		((CScene_LoadingForTown*)pScene)->SaveSlotIndex(pUI->GetIndex());

		((CScene_LoadingForTown*)pScene)->SetClassID(((CSelect_Char_Button*)(*iter))->GetPlayerInfo()->m_eClassID);

		if(NULL == pScene)								
			return;
		if(FAILED(Engine::SetUp_CurrentScene(pScene)))	
			return;
	}

	::Safe_Release(pUI);
}

HRESULT CScene_Select::Ready_Layer_GameLogic(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create();
	if(NULL == pLayer)
		return E_FAIL;

	Engine::CGameObject*		pGameObject = NULL;

	// For.BackGround_Logo Instance

	pGameObject = CSelect_BackGround::Create(m_pGraphicDev, L"SelectBackGround");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CSelect_Side_Bar::Create(m_pGraphicDev, L"SelectSideBar");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	_int iIndex = 1;

	for (_float fY = 390; fY > -200; fY -= 110)
	{
		pGameObject = CSelect_Char_Button::Create(m_pGraphicDev, L"SelectCharButton", fY, iIndex);

		if(NULL == pGameObject)
			goto except;

		if(FAILED(pLayer->AddObject(L"UI_Char_Button",pGameObject)))
			goto except;

		++iIndex;

		m_pSelectButtonList.push_back(pGameObject);
	}

	pGameObject = CSelect_Char_Model::Create(m_pGraphicDev, L"SelectModel");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CModel_Floor::Create(m_pGraphicDev, L"ModelFloor");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CGame_Start_Banner::Create(m_pGraphicDev, L"GameStartBanner");
	if(NULL == pGameObject)
		goto except;

	if(FAILED(pLayer->AddObject(L"UI",pGameObject)))
		goto except;

	pGameObject = CCursor::Create(m_pGraphicDev, L"Cursor", false);
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

HRESULT CScene_Select::Ready_LightInfo(void)
{
	Engine::LightMap_Clear();

	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));
	
	if (NULL == Engine::Find_Light(0, D3DLIGHT_DIRECTIONAL))
	{
		LightInfo.Type = D3DLIGHT_DIRECTIONAL;
		LightInfo.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
		LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		LightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		LightInfo.Direction = _vec3(1.0f, -1.0f, -1.0f);

		if (FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))	return E_FAIL;

		return S_OK;
	}
	
	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	LightInfo.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	LightInfo.Direction = _vec3(1.0f, -1.0f, -1.0f);

	return S_OK;
}

HRESULT CScene_Select::Ready_Resource(void)
{
	/*if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_LOGO, L"Mesh_Player", Engine::MESH_DYNAMIC, L"../Bin/Resource/Mesh/Dynamic/Player_Shaman/", L"Shaman_Vikini.X")))
	{
		MSG_BOX("Mesh_Player Loading Failed");
		return E_FAIL;
	}*/

	return S_OK;
}

void CScene_Select::SetIndex(_uint iIndex)
{
	m_iSlotIndex = iIndex;
}

void CScene_Select::ModelRotation()
{
	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80)
	{
		_int iDistance = 0;

		if(iDistance = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
		{
			Engine::CGameObject* pModelUi = Engine::Find(L"SelectModel");
			Engine::CGameObject* pFloorUi = Engine::Find(L"ModelFloor");

			Engine::CTransform* pModelTransform = ((Engine::CTransform*)pModelUi->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC));
			Engine::CTransform* pFloorTransform = ((Engine::CTransform*)pFloorUi->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC));

			pModelTransform->Rotation(Engine::ROT_Y, -iDistance / 5.f);
			pFloorTransform->Rotation(Engine::ROT_Y, -iDistance / 5.f);

			::Safe_Release(pModelTransform);
			::Safe_Release(pFloorTransform);
			::Safe_Release(pModelUi);
			::Safe_Release(pFloorUi);
		}
	}
}
CScene_Select* CScene_Select::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_Select*			pInstance = new CScene_Select(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Select Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;	
}

CScene_Select* CScene_Select::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iSlotIndex)
{
	CScene_Select*			pInstance = new CScene_Select(pGraphicDev);

	pInstance->SetIndex(iSlotIndex);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Select Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CScene_Select::Free(void)
{
	m_pSelectButtonList.clear();
	Engine::CScene::Free();
}



