#include "stdafx.h"
#include "Scene_Logo.h"

#include "Export_Engine.h"


CScene_Logo::CScene_Logo(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{

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

	return S_OK;
}

void CScene_Logo::Update_Scene(const _float& fTimeDelta)
{
	
}

void CScene_Logo::Render_Scene(void)
{

}

HRESULT CScene_Logo::Ready_Layer_GameLogic(void)
{
	return S_OK;
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

void CScene_Logo::Free(void)
{
	Engine::CScene::Free();
}

