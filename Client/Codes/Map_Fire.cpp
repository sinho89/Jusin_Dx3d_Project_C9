#include "stdafx.h"
#include "Map_Fire.h"

#include "Export_Engine.h"

#include "NavigationMesh.h"
#include "Cell.h"

#include "Camera.h"
//#include "MouseCol.h"

//클라 파이어맵..

CMap_Fire::CMap_Fire(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_fAlpha(1.0f)
{
	for(int i = 0 ; i < 33; ++i)
	{
		m_pMeshCom[i] = NULL;
	}
}
CMap_Fire::~CMap_Fire(void)
{

}

Engine::CStaticMesh** CMap_Fire::GetMapMesh(_int* iSize)
{
	*iSize = 33;
	return m_pMeshCom;
}

HRESULT CMap_Fire::Start_ForScene(void)
{
	m_pLightCam = Engine::GetCamera(Engine::CAMERA_LIGHT);
	m_pCurCam = Engine::GetCurrentCamera();

	//m_pTransCom->Set_Scale(100.f, 100.f, 100.f);

	Engine::CTexture* pTexture = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_EffectNormal");
	if (NULL == pTexture)
		return E_FAIL;

	for (int i = 0; i < 33; ++i)
	{
		m_pMeshCom[i]->SetReflectNormalTex(pTexture);
	}

	return S_OK;
}

void CMap_Fire::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_STAGE_DEPTH, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);

	/*if (m_pRendererCom->GetShadowBuffer())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_SHADOW, this);
	}*/

	_vec3	vScale;
	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

	for (int i = 0; i < 33; ++i)
	{
		m_pMeshCom[i]->PlusTime(fTimeDelta);
	}
}

void CMap_Fire::Render_Object(void)
{
}

void CMap_Fire::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
//	if (g_bDebugRender)
//		m_pNaviCom->Render_NaviCell();

	switch (iPassIdx)
	{
	case 0:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 2:
		if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
		break;

	case 3:
		if (FAILED(Set_Minimap_ConstanTable(pEffect))) return;
		break;

	case 4:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 6:
		return;
	}

	for (int i = 0; i < 33; ++i)
	{
		m_pMeshCom[i]->Render_Mesh(pEffect, iPassIdx);
	}
}

HRESULT CMap_Fire::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

	return S_OK;
}

HRESULT CMap_Fire::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	////Mesh
	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_0");
	m_pMeshCom[0] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_0", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_1");
	m_pMeshCom[1] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_1", pComponent));


	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_2");
	m_pMeshCom[2] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_2", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_3");
	m_pMeshCom[3] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_3", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_4");
	m_pMeshCom[4] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_4", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_5");
	m_pMeshCom[5] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_5", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_6");
	m_pMeshCom[6] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_6", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_7");
	m_pMeshCom[7] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_7", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_8");
	m_pMeshCom[8] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_8", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_9");
	m_pMeshCom[9] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_9", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_10");
	m_pMeshCom[10] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_10", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_11");
	m_pMeshCom[11] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_11", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_12");
	m_pMeshCom[12] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_12", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_13");
	m_pMeshCom[13] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_13", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_14");
	m_pMeshCom[14] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_14", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_15");
	m_pMeshCom[15] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_15", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_16");
	m_pMeshCom[16] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_16", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_17");
	m_pMeshCom[17] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_17", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_18");
	m_pMeshCom[18] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_18", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_19");
	m_pMeshCom[19] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_19", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_20");
	m_pMeshCom[20] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_20", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_21");
	m_pMeshCom[21] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_21", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_22");
	m_pMeshCom[22] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_22", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_23");
	m_pMeshCom[23] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_23", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_24");
	m_pMeshCom[24] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_24", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_25");
	m_pMeshCom[25] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_25", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_26");
	m_pMeshCom[26] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_26", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_27");
	m_pMeshCom[27] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_27", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_28");
	m_pMeshCom[28] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_28", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_29");
	m_pMeshCom[29] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_29", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_30");
	m_pMeshCom[30] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_30", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_31");
	m_pMeshCom[31] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_31", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_32");
	m_pMeshCom[32] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_32", pComponent));

	//pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_33");
	//m_pMeshCom[33] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_33", pComponent));

	//pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Map_Fire_34");
	//m_pMeshCom[34] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_34", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticMesh"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));


	////Navi
	//pComponent = Engine::Clone_Resource(RESOURCE_MESH_FIRE, L"Mesh_Navigation");
	//if (pComponent == NULL)
	//	return E_FAIL;
	//m_pNaviCom = (Engine::CNavigationMesh*)pComponent;
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"NaviCom", pComponent));

	return S_OK;
}

HRESULT CMap_Fire::Set_ConstantTable( LPD3DXEFFECT pEffect )
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 50.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bLimLight", false);
	pEffect->SetBool("g_bSpecularCheck", false);
	pEffect->SetBool("g_bBlurCheck", false);
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMap_Fire::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = m_pLightCam->GetMatView();
	matLightProj = m_pLightCam->GetMatProj();

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMap_Fire::Set_Minimap_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = Engine::GetCamera(Engine::CAMERA_MINIMAP)->GetMatView();
	matLightProj = Engine::GetCamera(Engine::CAMERA_MINIMAP)->GetMatProj();

	pEffect->SetMatrix("g_matView", &matLightView);
	pEffect->SetMatrix("g_matProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}



CMap_Fire* CMap_Fire::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CMap_Fire*		pInstance = new CMap_Fire(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CMap_Fire Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMap_Fire::Free(void)
{
 	Engine::CGameObject::Free();
}


