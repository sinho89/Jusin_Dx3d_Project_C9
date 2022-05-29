#include "Light_Manager.h"
#include "Light.h"
#include "Screen_Texture.h"
#include "Target_Manager.h"
#include "CameraMgr.h"
#include "Camera.h"
#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLight_Manager)

Engine::CLight_Manager::CLight_Manager(void)
	: m_pDirectionalScreen(NULL)
	, m_pPointScreen(NULL)
	, m_pSpotScreen(NULL)
{

}

Engine::CLight_Manager::~CLight_Manager(void)
{

}

//D3DLIGHT9* Engine::CLight_Manager::Get_LightInfo(const _uint& iIndex /*= 0*/)
//{
//	LIGHTLIST::iterator	iter = m_LightList.begin();
//
//	for (_uint i = 0; i < iIndex; ++i)
//	{
//		++iter;
//	}
//
//	return (*iter)->Get_LightInfo();
//}
//
CLight * CLight_Manager::Get_PointLight(const _uint & iIndex)
{
	MAPLIGHT::iterator iter = m_mapPointLight.find(iIndex);
	if (iter == m_mapPointLight.end())
		return NULL;

	return iter->second;
}

map<const _uint, CLight*>* CLight_Manager::Get_PointLightMap(void)
{
	return &m_mapPointLight;
}

map<const _uint, CLight*>* CLight_Manager::Get_SpotLightMap(void)
{
	return &m_mapSpotLight;
}

HRESULT CLight_Manager::Ready_ScreenBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	m_pDirectionalScreen	= CScreen_Texture::Create(pGraphicDev, 0, 0, ViewPort.Width, ViewPort.Height);
	if (NULL == m_pDirectionalScreen)	return E_FAIL;
	m_pPointScreen			= CScreen_Texture::Create(pGraphicDev, 0, 0, _uint(ViewPort.Width), _uint(ViewPort.Height));
	if (NULL == m_pPointScreen)			return E_FAIL;
	m_pSpotScreen			= CScreen_Texture::Create(pGraphicDev, 0, 0, ViewPort.Width, ViewPort.Height);
	if (NULL == m_pSpotScreen)			return E_FAIL;

	return S_OK;
}

HRESULT Engine::CLight_Manager::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight*	pLight = Find_Light(iIndex, pLightInfo->Type);
	if (NULL != pLight)
	{
		MSG_BOX("This Light have already");
		return E_FAIL;
	}

	pLight = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	if(NULL == pLight)
		return E_FAIL;

	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	size_t iLightCnt = 0;
	if (D3DLIGHT_DIRECTIONAL == pLightInfo->Type)
	{
		m_mapDirectionalLight.insert(MAPLIGHT::value_type(iIndex, pLight));
	}
	else if (D3DLIGHT_POINT == pLightInfo->Type)
	{
		m_mapPointLight.insert(MAPLIGHT::value_type(iIndex, pLight));
	}
	else if (D3DLIGHT_SPOT == pLightInfo->Type)
	{
		m_mapSpotLight.insert(MAPLIGHT::value_type(iIndex, pLight));
	}

	//m_LightList.push_back(pLight);

	return S_OK;
}

void CLight_Manager::Render_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect)
{
	/*LIGHTLIST::iterator iter = m_LightList.begin();
	LIGHTLIST::iterator iter_end = m_LightList.end();

	for (; iter != iter_end; ++iter)
		(*iter)->Render(pEffect);*/	
}

void CLight_Manager::Render_Directional_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex)
{
	size_t iLightCount = m_mapDirectionalLight.size();

	if (0 != iLightCount)
	{
		pEffect->BeginPass(iIndex);

		MAPLIGHT::iterator iter = m_mapDirectionalLight.begin();
		MAPLIGHT::iterator iter_end = m_mapDirectionalLight.end();

		for (; iter != iter_end; ++iter)
		{
			Directional_Update(iter->second, pEffect);
		}

		pEffect->EndPass();
	}
}

void CLight_Manager::Render_Point_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex)
{
	size_t iLightCount = m_mapPointLight.size();

	if (0 != iLightCount)
	{
		pEffect->BeginPass(iIndex);

		MAPLIGHT::iterator iter = m_mapPointLight.begin();
		MAPLIGHT::iterator iter_end = m_mapPointLight.end();

		for (; iter != iter_end; ++iter)
		{			
			if (iter->second->GetLightRender())
				Point_Update(pGraphicDev, iter->second, pEffect);
		}
		
		pEffect->EndPass();
	}
}

void CLight_Manager::Render_Spot_Light(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect, const _uint& iIndex)
{
	size_t iLightCount = m_mapSpotLight.size();

	if (0 != iLightCount)
	{
		pEffect->BeginPass(iIndex);

		MAPLIGHT::iterator iter = m_mapSpotLight.begin();
		MAPLIGHT::iterator iter_end = m_mapSpotLight.end();

		for (; iter != iter_end; ++iter)
		{
			if (iter->second->GetLightRender())
				Spot_Update(iter->second, pEffect);
		}


		pEffect->EndPass();
	}
}

void CLight_Manager::Directional_Update(CLight* pLight, LPD3DXEFFECT pEffect)
{
	_vec3 LightDir = pLight->Get_LightInfo()->Direction;
	pEffect->SetVector("g_vLightDir", &_vec4(LightDir, 0.0f));

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Get_LightInfo()->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Get_LightInfo()->Ambient);

	pEffect->SetVector("g_vMtrlDiffuse", &_vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pEffect->SetVector("g_vMtrlAmbient", &_vec4(1.0f, 1.0f, 1.0f, 1.0f));

	/*CTarget_Manager::GetInstance()->SetUp_OnShader(L"Target_Specular", pEffect, "g_SpecularTexture");
	
	_vec3 vViewDir = CCameraMgr::GetInstance()->GetCurrentCamera()->GetAtPosition() - CCameraMgr::GetInstance()->GetCurrentCamera()->GetEyePosition();
	D3DXVec3Normalize(&vViewDir, &vViewDir);
	
	pEffect->SetVector("g_vViewDir", &_vec4(vViewDir, 0.0f));*/

	pEffect->CommitChanges();

	m_pDirectionalScreen->Render_Buffer();
}

void CLight_Manager::Point_Update(LPDIRECT3DDEVICE9 pGraphicDev,CLight* pLight, LPD3DXEFFECT pEffect)
{
	_vec3 LightPos = pLight->Get_LightInfo()->Position;

	//_float fRandRange = _float(rand() % 2);
	float LightRange = pLight->Get_LightInfo()->Range;// -fRandRange;
	
	pEffect->SetVector("g_vLightPos", &_vec4(LightPos, 0.0f));
	pEffect->SetFloat("g_fRange", LightRange);

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Get_LightInfo()->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Get_LightInfo()->Ambient);

	pEffect->SetVector("g_vMtrlDiffuse", &_vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pEffect->SetVector("g_vMtrlAmbient", &_vec4(1.0f, 1.0f, 1.0f, 1.0f));

	_matrix		matView, matProj;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	pEffect->SetMatrix("g_matView", &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView.m[3][0]);

	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	//pEffect->SetMatrix("g_matProj", &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);	

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	float fFar = GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	//SetUp_OnShader(L"Target_Specular", pEffect, "g_SpecularTexture");

	/*_vec3 vViewDir = GetCurrentCamera()->GetAtPosition() - GetCurrentCamera()->GetEyePosition();
	D3DXVec3Normalize(&vViewDir, &vViewDir);*/

	//pEffect->SetVector("g_vViewDir", &_vec4(vViewDir, 0.0f));
	//pEffect->SetVector("g_vCamPosition", &_vec4(GetCurrentCamera()->GetEyePosition(), 0.0f));

	pEffect->CommitChanges();

	m_pPointScreen->Render_Buffer();
}

void CLight_Manager::Spot_Update(CLight* pLight, LPD3DXEFFECT pEffect)
{
	pEffect->CommitChanges();
	m_pSpotScreen->Render_Buffer();
}

void CLight_Manager::LightMap_Clear(void)
{
	for_each(m_mapDirectionalLight.begin(), m_mapDirectionalLight.end(), CRelease_Pair());
	for_each(m_mapPointLight.begin(), m_mapPointLight.end(), CRelease_Pair());
	for_each(m_mapSpotLight.begin(), m_mapSpotLight.end(), CRelease_Pair());
	m_mapDirectionalLight.clear();
	m_mapPointLight.clear();
	m_mapSpotLight.clear();

	return;
}

CLight* CLight_Manager::Find_Light(const _uint sLightTag, const _short LightType)
{
	MAPLIGHT::iterator iter;

	if(D3DLIGHT_DIRECTIONAL == LightType)
	{
		iter = m_mapDirectionalLight.find(sLightTag);

		if (iter == m_mapDirectionalLight.end()) return NULL;
	}
	else if (D3DLIGHT_POINT == LightType)
	{
		iter = m_mapPointLight.find(sLightTag);

		if (iter == m_mapPointLight.end()) return NULL;
	}
	else if (D3DLIGHT_SPOT == LightType)
	{
		iter = m_mapSpotLight.find(sLightTag);

		if (iter == m_mapSpotLight.end()) return NULL;
	}

	return iter->second;
}

D3DLIGHT9* CLight_Manager::Find_LightInfo(const _uint sLightTag, const _short LightType)
{
	MAPLIGHT::iterator iter;

	if (D3DLIGHT_DIRECTIONAL == LightType)
	{
		iter = m_mapDirectionalLight.find(sLightTag);

		if (iter == m_mapDirectionalLight.end()) return NULL;
	}
	else if (D3DLIGHT_POINT == LightType)
	{
		iter = m_mapPointLight.find(sLightTag);

		if (iter == m_mapPointLight.end()) return NULL;
	}
	else if (D3DLIGHT_SPOT == LightType)
	{
		iter = m_mapSpotLight.find(sLightTag);

		if (iter == m_mapSpotLight.end()) return NULL;
	}

	return iter->second->Get_LightInfo();
}

void CLight_Manager::Add_Light(CLight * pLight, const _short LightType)
{
	_uint iIndex = 0;
	
	if (D3DLIGHT_DIRECTIONAL == LightType)
	{
		iIndex = m_mapDirectionalLight.size();
		m_mapDirectionalLight.insert(MAPLIGHT::value_type(iIndex, pLight));
	}
	else if (D3DLIGHT_POINT == LightType)
	{
		iIndex = m_mapPointLight.size();
		m_mapPointLight.insert(MAPLIGHT::value_type(iIndex, pLight));
	}
	else if (D3DLIGHT_SPOT == LightType)
	{
		iIndex = m_mapSpotLight.size();
		m_mapSpotLight.insert(MAPLIGHT::value_type(iIndex, pLight));
	}
}

void Engine::CLight_Manager::Free(void)
{
	//for_each(m_LightList.begin(), m_LightList.end(), CRelease_Single());
	//m_LightList.clear();
	for_each(m_mapDirectionalLight.begin(), m_mapDirectionalLight.end(), CRelease_Pair());
	for_each(m_mapPointLight.begin(), m_mapPointLight.end(), CRelease_Pair());
	for_each(m_mapSpotLight.begin(), m_mapSpotLight.end(), CRelease_Pair());
	m_mapDirectionalLight.clear();
	m_mapPointLight.clear();
	m_mapSpotLight.clear();

	Engine::Safe_Release(m_pDirectionalScreen);
	Engine::Safe_Release(m_pPointScreen);
	Engine::Safe_Release(m_pSpotScreen);
}

