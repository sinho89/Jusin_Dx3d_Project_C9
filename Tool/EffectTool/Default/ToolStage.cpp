#include "stdafx.h"
#include "ToolStage.h"
#include "Export_Engine.h"
#include "Layer.h"
#include "EffectCamera.h"
#include "Grid.h"
#include "Particle.h"
#include "Sprite.h"

CToolStage::CToolStage(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{

}

CToolStage::~CToolStage(void)
{

}

HRESULT CToolStage::Ready_Scene(void)
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

void CToolStage::Update_Scene(const _float& fTimeDelta)
{
	Engine::CScene::Update_Scene(fTimeDelta);
}

void CToolStage::Render_Scene(void)
{

}

HRESULT CToolStage::SetUp_DefaultSetting(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}


HRESULT CToolStage::Ready_Layer_Environment(void)
{

	return S_OK;
}



HRESULT CToolStage::Ready_Layer_GameLogic(void)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	if(pLayer == NULL)
		return E_FAIL;

	Engine::CGameObject* pGameObject = NULL;
	pGameObject = CEffectCamera::Create(m_pGraphicDev, L"Camera",  &_vec3(0.f, 10.f, -5.f), &_vec3(0.f, 0.f, 0.f), &_vec3(0.f, 1.f, 0.f));
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->AddObject(L"Camera", pGameObject);


	// For.Grid Instance
	pGameObject = CGrid::Create(m_pGraphicDev, L"ToolGrid");
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->AddObject(L"Grid", pGameObject);



	// For.Particle Instance
	pGameObject = CParticle::Create(m_pGraphicDev, L"ToolParticle");
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->AddObject(L"Particle", pGameObject);


	// For.Sprite Instance
	pGameObject = CSprite::Create(m_pGraphicDev, L"ToolSprite");
	if(pGameObject == NULL)
		return E_FAIL;
	pLayer->AddObject(L"Sprite", pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(Engine::LAYER_GAMELOGIC, pLayer));

	return S_OK;
}


HRESULT CToolStage::Ready_Light(void)
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





HRESULT CToolStage::Ready_Resource(void)
{
	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGE, L"Buffer_Grid", Engine::BUFFER_GRIDCOL, 10, 10, 2)))
	{
		MSG_BOX("Grid Buffer Load Failed");
		return E_FAIL;
	}

	//Particle Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Particle", Engine::TEX_NORMAL, L"../../../Client/Bin/Resource/Effect/Particle/Particle%d.png", 20)))
	{
		MSG_BOX("Texture Particle Loading Failed");
		return E_FAIL;
	}

	//Sprite Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Sprite", Engine::TEX_NORMAL, L"../../../Client/Bin/Resource/Effect/Sprite/Sprite%d.dds", 100)))
	{
		MSG_BOX("Texture Particle Loading Failed");
		return E_FAIL;
	}
	

	//UIEffect Texture
	if (FAILED(Engine::Ready_Textures(m_pGraphicDev, RESOURCE_STAGE, L"Texture_UIEffect", Engine::TEX_NORMAL, L"../../../Client/Bin/Resource/Effect/UIEffect/UIEffect%d.dds", 54)))
	{
		MSG_BOX("Texture Particle Loading Failed");
		return E_FAIL;
	}



	return S_OK;
}

CToolStage* CToolStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CToolStage*			pInstance = new CToolStage(pGraphicDev);

	if(FAILED(pInstance->Ready_Scene()))
	{
		MSG_BOX("ToolStage Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;	
}

void CToolStage::Free(void)
{
	Engine::CScene::Free();
}
