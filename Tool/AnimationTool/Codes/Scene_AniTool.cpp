#include "stdafx.h"
#include "Scene_AniTool.h"

#include "Layer.h"
#include "Camera_AniTool.h"
#include "Grid.h"

#include "Export_Engine.h"

CScene_AniTool::CScene_AniTool(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{

}

CScene_AniTool::~CScene_AniTool(void)
{

}

HRESULT CScene_AniTool::Ready_Scene(void)
{
	if(FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if(FAILED(Ready_Resource()))
		return E_FAIL;

	if(FAILED(Ready_Light()))
		return E_FAIL;

	if(FAILED(Ready_Layer_Environment()))
		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))
		return E_FAIL;

	return S_OK;
}

void CScene_AniTool::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);
}

void CScene_AniTool::Render_Scene(void)
{

}

HRESULT CScene_AniTool::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	return S_OK;
}

HRESULT CScene_AniTool::Ready_Layer_Environment(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	if(pLayer == NULL)
		return E_FAIL;

	Engine::CGameObject* pGameObject = NULL;
	pGameObject = CGrid::Create(m_pGraphicDev, L"Grid");
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->AddObject(L"Grid", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_ENVIRONMENT, pLayer));
	return S_OK;
}

HRESULT CScene_AniTool::Ready_Layer_GameLogic(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	if(pLayer == NULL)
		return E_FAIL;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CCamera_AniTool::Create(m_pGraphicDev, L"MainCamera", &_vec3(-10.f, 5.f, -10.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->AddObject(L"Camera", pGameObject);
	

	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CScene_AniTool::Ready_Light(void)
{
	D3DLIGHT9	LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse	=	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Specular	=	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.Ambient	=	D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	LightInfo.Direction	=	_vec3(1.0f, -1.0f, 1.0f);

	if(FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0))) return E_FAIL;

	return S_OK;
}

HRESULT CScene_AniTool::Ready_Resource(void)
{

	//============피킹렉트======================================//
	// Ready_Buffer
	if(FAILED(Engine::Ready_Buffers(m_pGraphicDev,	RESOURCE_STAGE,
		L"Buffer_RcTex", Engine::BUFFER_RCTEX)))
		return E_FAIL;

	//Texture...
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev
		, RESOURCE_STAGE
		, L"Texture_Collider_Blue"
		, Engine::TEX_NORMAL
		, L"../../../Client/Bin/Resource/Effect/Collider/Blue.png"
		, 1)))
	{
		MSG_BOX("Texture Collider Blue Load Failed");
		return E_FAIL;
	}
	//==========================================================//



	if(FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGE, L"Buffer_Grid", Engine::BUFFER_GRIDCOL, 10, 10, 2)))
	{
		MSG_BOX("Grid Buffer Load Failed");
		return E_FAIL;
	}




	//=================================Effect용 텍스쳐 로드============================================//

	//Particle Texture
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Particle", Engine::TEX_NORMAL, L"../../../Client/Bin/Resource/Effect/Particle/Particle%d.png", 13)))
	{
		MSG_BOX("Texture Particle Loading Failed");
		return E_FAIL;
	}

	//Sprite Texture
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STATIC, L"Texture_Sprite", Engine::TEX_NORMAL, L"../../../Client/Bin/Resource/Effect/Sprite/Sprite%d.dds", 46)))
	{
		MSG_BOX("Texture Particle Loading Failed");
		return E_FAIL;
	}

	//=================================================================================================//



	return S_OK;
}

CScene_AniTool* CScene_AniTool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_AniTool* pInstance = new CScene_AniTool(pGraphicDev);
	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Logo Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}	

void CScene_AniTool::Free(void)
{
	Engine::CScene::Free();

}