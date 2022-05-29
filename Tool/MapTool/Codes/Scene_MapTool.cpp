#include "stdafx.h"
#include "Scene_MapTool.h"

#include "Layer.h"
#include "Camera_MapTool.h"

#include "Export_Engine.h"

#include "Map_Store.h"

int CScene_MapTool::m_iPointCount = 0;


CScene_MapTool::CScene_MapTool(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{

}

CScene_MapTool::~CScene_MapTool(void)
{

}

HRESULT CScene_MapTool::Ready_Scene(void)
{
	if(FAILED(Engine::CScene::Ready_Scene()))
		return E_FAIL;

	if(FAILED(SetUp_DefaultSetting()))
		return E_FAIL;

	if(FAILED(Ready_Resource()))
		return E_FAIL;

	if(FAILED(Ready_Light()))
		return E_FAIL;

	if(FAILED(Ready_Layer_GameLogic()))
		return E_FAIL;

	return S_OK;
}

void CScene_MapTool::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);

}

void CScene_MapTool::Render_Scene(void)
{

}

HRESULT CScene_MapTool::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CScene_MapTool::Ready_Layer_GameLogic(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	if(pLayer == NULL)
		return E_FAIL;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = CCamera_MapTool::Create(m_pGraphicDev, L"MainCamera", &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->AddObject(L"Camera", pGameObject);

	//pGameObject = CMapObj::Create(m_pGraphicDev, L"StaticMesh");
	//((CMapObj*)pGameObject)->SetID(MAP_ICE);
	//if(pGameObject == NULL)
	//	return E_FAIL;
	//pLayer->AddObject(L"Map", pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));

	return S_OK;
}

HRESULT CScene_MapTool::Ready_Light(void)
{
	D3DLIGHT9				LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	LightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	if(FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_MapTool::Ready_Resource(void)
{


	// Ready_Buffer
	if(FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX)))
		return E_FAIL;

	if(FAILED(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Buffer_CubeTex",
		Engine::BUFFER_CUBETEX)))
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


	//Sprite Texture
	if(FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Sprite", Engine::TEX_NORMAL, L"../../../Client/Bin/Resource/Effect/Sprite/Sprite%d.dds", 46)))
	{
		MSG_BOX("Texture Particle Loading Failed");
		return E_FAIL;
	}

	//if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Map_Shop",
	//	Engine::MESH_STATIC,
	//	L"../../../Client/Bin/Resource/Mesh/Static/Store/",
	//	L"Store.X")))
	//{
	//	MSG_BOX("Mesh Map Shop Load Failed");
	//	return E_FAIL;
	//}

	//네비메쉬
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Navigation"
		, Engine::MESH_NAVIGATION
		,L""
		,L"")))
	{
		MSG_BOX("NaviMesh Loading Failed");
		return E_FAIL;
	}


	//상점
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_BigHall",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_BigHall.X")))
	{
		MSG_BOX("Mesh Map Store_BigHall Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_FrontHall",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_FrontHall.X")))
	{
		MSG_BOX("Mesh Map Store_FrontHall Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_FrontHall_Front",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_FrontHall_Front.X")))
	{
		MSG_BOX("Mesh Map Store_FrontHall_Front Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_FrontHall_Right",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_FrontHall_Right.X")))
	{
		MSG_BOX("Mesh Map Store_FrontHall_Right Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_LeftHall",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_LeftHall.X")))
	{
		MSG_BOX("Mesh Map Store_LeftHall Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_LeftHall_Front",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_LeftHall_Front.X")))
	{
		MSG_BOX("Mesh Map Store_LeftHall_Front Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_LeftHall_Right",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_LeftHall_Right.X")))
	{
		MSG_BOX("Mesh Map Store_LeftHall_Right Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_Stairs",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_Stairs.X")))
	{
		MSG_BOX("Mesh Map Store_Stairs Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Store_Stairs_front",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Store_divide/",
		L"Store_Stairs_front.X")))
	{
		MSG_BOX("Mesh Map Store_Stairs_front Load Failed");
		return E_FAIL;
	}



	//아이스통맵
	//if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Map_Ice",
	//	Engine::MESH_STATIC,
	//	L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
	//	L"SoulValley.X")))
	//{
	//	MSG_BOX("Mesh Map Ice Load Failed");
	//	return E_FAIL;
	//}

	//아이스분할맵
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_0",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_0.X")))
	{
		MSG_BOX("Mesh Map Ice 0 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_1",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_1.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_2",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_2.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_3",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_3.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_4",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_4.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_5",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_5.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_6",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_6.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_7",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_7.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_8",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_8.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_9",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_9.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_10",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_10.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_11",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_11.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_12",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_12.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_13",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_13.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_14",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_14.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_15",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_15.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_16",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_16.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_17",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_17.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}


	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_18",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_18.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_19",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_19.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_20",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_20.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_21",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_21.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_22",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_22.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_23",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_23.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_24",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_24.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_25",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_25.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_26",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_26.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_27",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_27.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_28",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_28.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_29",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_29.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_30",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_30.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_31",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_31.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_32",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_32.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_33",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_33.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_34",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_34.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_35",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_35.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_36",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_36.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_37",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_37.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_38",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_38.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_39",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_39.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_40",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_40.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_41",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_41.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_42",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_42.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_43",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_43.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_44",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_44.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_45",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_45.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_46",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_46.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_47",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_47.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_48",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_48.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_49",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_49.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_50",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_50.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_51",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_51.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_52",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_52.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_53",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_53.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_54",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_54.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_55",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_55.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_56",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_56.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_57",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_57.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_58",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_58.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_59",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_59.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_60",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_60.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_61",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_61.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_62",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_62.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_63",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_63.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_64",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_64.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_65",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_65.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_66",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_66.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_67",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_67.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Ice_68",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/SoulValley/",
		L"SoulValley_68.X")))
	{
		MSG_BOX("Mesh Map Ice 1 Load Failed");
		return E_FAIL;
	}
	//파이어 통맵
	//if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
	//	RESOURCE_STAGE,
	//	L"Mesh_Map_Fire",
	//	Engine::MESH_STATIC,
	//	L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
	//	L"VolsLand_UT.X")))
	//{
	//	MSG_BOX("Mesh Map Fire Load Failed");
	//	return E_FAIL;
	//}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_0",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_0.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_1",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_1.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_2",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_2.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_3",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_3.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_4",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_4.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_5",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_5.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_6",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_6.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_7",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_7.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_8",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_8.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_9",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_9.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_10",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_10.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_11",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_11.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_12",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_12.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_13",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_13.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_14",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_14.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_15",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_15.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_16",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_16.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_17",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_17.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_18",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_18.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_19",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_19.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_20",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_20.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_21",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_21.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_22",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_22.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_23",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_23.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_24",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_24.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_25",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_25.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_26",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_26.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_27",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_27.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_28",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_28.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_29",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_29.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_30",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_30.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_31",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_31.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Map_Fire_32",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
		L"VolsLand_UT_32.X")))
	{
		MSG_BOX("Mesh Map Fire Load Failed");
		return E_FAIL;
	}

	//if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Map_Fire_33",
	//	Engine::MESH_STATIC,
	//	L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
	//	L"VolsLand_UT_33.X")))
	//{
	//	MSG_BOX("Mesh Map Fire Load Failed");
	//	return E_FAIL;
	//}

	//if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Map_Fire_34",
	//	Engine::MESH_STATIC,
	//	L"../../../Client/Bin/Resource/Mesh/Static/Volsland_UT/",
	//	L"VolsLand_UT_34.X")))
	//{
	//	MSG_BOX("Mesh Map Fire Load Failed");
	//	return E_FAIL;
	//}
	//마을 오브젝트.. 메쉬...

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_BookShelf",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/BookShelf/",
		L"BookShelf.X")))
	{
		MSG_BOX("Mesh BookShelf Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_PaperBook",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/PaperBook/",
		L"PaperBook.X")))
	{
		MSG_BOX("Mesh PaperBook Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreArmor",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreArmor/",
		L"StoreArmor.X")))
	{
		MSG_BOX("Mesh StoreArmor Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreBow",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreBow/",
		L"StoreBow.X")))
	{
		MSG_BOX("Mesh Bow Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreDagger",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreDagger/",
		L"StoreDagger.X")))
	{
		MSG_BOX("Mesh StoreDagger Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StorePotion",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StorePotion/",
		L"StorePotion.X")))
	{
		MSG_BOX("Mesh StorePotion Load Failed");
		return E_FAIL;
	}


	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreSword",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreSword/",
		L"StoreSword.X")))
	{
		MSG_BOX("Mesh StoreSword Load Failed");
		return E_FAIL;
	}


	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreShield",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreShield/",
		L"StoreShield.X")))
	{
		MSG_BOX("Mesh StoreShield Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreBook",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreBook/",
		L"StoreBook.X")))
	{
		MSG_BOX("Mesh StoreShield Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreTable",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreTable/",
		L"StoreTable.X")))
	{
		MSG_BOX("Mesh StoreTable Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreTable2",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreTable2/",
		L"StoreTable2.X")))
	{
		MSG_BOX("Mesh StoreTable2 Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreBoard",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreBoard/",
		L"StoreBoard.X")))
	{
		MSG_BOX("Mesh StoreBoard Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreBoots",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreBoots/",
		L"StoreBoots.X")))
	{
		MSG_BOX("Mesh StoreBoots Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreCandle",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreCandle/",
		L"StoreCandle.X")))
	{
		MSG_BOX("Mesh StoreCandle Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_StoreSpear",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/StoreSpear/",
		L"StoreSpear.X")))
	{
		MSG_BOX("Mesh StoreSpear Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Bardiel_Sealed",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Bardiel_Sealed/",
		L"Bardiel_Sealed.X")))
	{
		MSG_BOX("Mesh StoreSpear Load Failed");
		return E_FAIL;
	}

	//아이스맵 메쉬...

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_Torch1",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Torch1/",
		L"Torch1.X")))
	{
		MSG_BOX("Mesh Torch1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Cristal_1",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Cristal_1/",
		L"Cristal_1.X")))
	{
		MSG_BOX("Mesh Cristal_1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Cristal_2",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Cristal_2/",
		L"Cristal_2.X")))
	{
		MSG_BOX("Mesh Cristal_2 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Ice_Rock_1",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Ice_Rock_1/",
		L"Ice_Rock_1.X")))
	{
		MSG_BOX("Mesh Cristal_1 Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Lamp_Bone_A",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Lamp_Bone_A/",
		L"Lamp_Bone_A.X")))
	{
		MSG_BOX("Mesh Lamp_Bone_A Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Lamp_Bone_B",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Lamp_Bone_B/",
		L"Lamp_Bone_B.X")))
	{
		MSG_BOX("Mesh Lamp_Bone_B Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Lamp_Bone_C",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Lamp_Bone_C/",
		L"Lamp_Bone_C.X")))
	{
		MSG_BOX("Mesh Lamp_Bone_C Load Failed");
		return E_FAIL;
	}

	//Fire맵 메쉬
	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Torch2",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Static/Torch2/",
		L"Torch2.X")))
	{
		MSG_BOX("Mesh Torch2 Load Failed");
		return E_FAIL;
	}

	//NPC 메쉬..

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_NPC_Weapon",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/NPC/Alegon/",
		L"Alegon.X")))
	{
		MSG_BOX("Mesh_NPC_Weapon Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_NPC_Armor",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/NPC/Eyine/",
		L"Eyine.X")))
	{
		MSG_BOX("Mesh_NPC_Armor Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_NPC_Potion",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/NPC/Aremoni/",
		L"Aremoni.X")))
	{
		MSG_BOX("Mesh_NPC_Potion Load Failed");
		return E_FAIL;
	}


	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_NPC_Book",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/NPC/Ruisa/",
		L"Ruisa.X")))
	{
		MSG_BOX("Mesh_NPC_Book Load Failed");
		return E_FAIL;
	}

	//몬스터 메쉬..

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_Babegazi_Warrior",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/Monster/Babegazi_Warrior/",
		L"Babegazi_Warrior.X")))
	{
		MSG_BOX("Mesh_Babegazi_Warrior Load Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_Babegazi_Axe",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/Monster/Babegazi_Axe/",
		L"Babegazi_Axe.X")))
	{
		MSG_BOX("Mesh_Babegazi_Axe Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Babegazi_Bow",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/Monster/Babegazi_Bow/",
		L"Babegazi_Bow.X")))
	{
		MSG_BOX("Mesh_Babegaze_Warrior Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Hanuman",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/HanumanBoss/",
		L"Hanuman.X")))
	{
		MSG_BOX("Mesh_Hanuman Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Bardiel",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/BardielBoss/",
		L"Bardiel.X")))
	{
		MSG_BOX("Mesh_Bardiel Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Knole_Commander",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/Monster/Knole_Commander/",
		L"Knole_Commander.X")))
	{
		MSG_BOX("Mesh_Knole_Commander Load Failed");
		return E_FAIL;
	}

	//if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
	//	RESOURCE_STAGE,
	//	L"Mesh_Knole_Warrior",
	//	Engine::MESH_DYNAMIC,
	//	L"../../../Client/Bin/Resource/Mesh/Dynamic/Monster/Knole_Warrior/",
	//	L"Knole_Warrior.X")))
	//{
	//	MSG_BOX("Knole_Warrior Load Failed");
	//	return E_FAIL;
	//}

	//플레이어 메쉬...

	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
		RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/Player_WitchBlade/",
		L"WitchBlade.X")))
	{
		MSG_BOX("WitchBlade Load Failed");
		return E_FAIL;
	}

	//함정 메쉬..

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_SnowBall",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/Trap/SnowBall/",
		L"SnowBall.X")))
	{
		MSG_BOX("SnowBall Load Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_IceTwister",
		Engine::MESH_DYNAMIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/Trap/IceTwister/",
		L"IceTwister.X")))
	{
		MSG_BOX("IceTwister Load Failed");
		return E_FAIL;
	}

	//문

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Gate_A",
		Engine::MESH_STATIC,
		L"../../../Client/Bin/Resource/Mesh/Dynamic/Gate/",
		L"Gate_A.X")))
	{
		MSG_BOX("Gate_A Load Failed");
		return E_FAIL;
	}
	//if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, 
	//	RESOURCE_STAGE,
	//	L"Mesh_NPC_Quest",
	//	Engine::MESH_STATIC,
	//	L"../../../Client/Bin/Resource/Mesh/Dynamic/NPC/NANA/",
	//	L"NANA.X")))
	//{
	//	MSG_BOX("Mesh_NPC_Quest Load Failed");
	//	return E_FAIL;
	//}


	return S_OK;
}


CScene_MapTool* CScene_MapTool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_MapTool*			pInstance = new CScene_MapTool(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("CScene_Logo Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CScene_MapTool::Free(void)
{
	Engine::CScene::Free();
}



