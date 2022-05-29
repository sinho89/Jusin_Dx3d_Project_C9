#include "stdafx.h"
#include "Camera.h"
#include "WitchBladeWeapon.h"
#include "Trail.h"
#include "LightCamera.h"

CWitchBladeWeapon::CWitchBladeWeapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: CWeapon(pGraphicDev, wstrName)
{

}

CWitchBladeWeapon::~CWitchBladeWeapon(void)
{

}

HRESULT CWitchBladeWeapon::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	CTrail* pTrail = CTrail::Create(m_pGraphicDev, L"Trail", m_wstrName);

	if(pTrail == NULL)
		return E_FAIL;

	((CTrail*)pTrail)->Start_ForScene();

	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", pTrail);
	return S_OK;
}

void CWitchBladeWeapon::Update_Object(const _float& fTimeDelta)
{
	CWeapon::Update_Object(fTimeDelta);
	m_pTransCom->Set_ParentMatrix(m_pmatWeapon);
	m_pTransCom->Set_ParentMatrix(m_pmatTarget);
}

void CWitchBladeWeapon::Render_Object(void)
{
	/*LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if (Engine::MRT_DEFERRED_0 == m_pRendererCom->GetMRTID())
	{
		if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
	}
	else if (Engine::MRT_DEFERRED_1 == m_pRendererCom->GetMRTID())
	{
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
	}
	else if (Engine::MRT_DEFERRED_2 == m_pRendererCom->GetMRTID())
	{
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
	}

	pEffect->Begin(NULL, 0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->End();

	Engine::Safe_Release(pEffect);*/
}

void CWitchBladeWeapon::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	switch (iPassIdx)
	{
	case 0:
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
		break;

	case 2:
		if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
		break;

	case 3:
		return;

	case 4:
		return;

	case 5:
		if (FAILED(Set_CACD_Shadow_ConstanTable(pEffect))) return;
		break;

	/*case 6:
		if (m_bOutlineDraw)
		{
			if (FAILED(SetUp_ConstantTable(pEffect))) return;
		}
		else
			return;
		break;*/
	}

	m_pMeshCom->Render_Mesh(pEffect, iPassIdx);
}

HRESULT CWitchBladeWeapon::Ready_Object(const _tchar* pTargetTag, const _tchar* pFilePath)
{
	m_strParentTag = pTargetTag;
	if(pTargetTag == NULL)
		return E_FAIL;
	if(pFilePath == NULL)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	if(FAILED(LoadWeaponData(pTargetTag, pFilePath)))
		return E_FAIL;


	m_bIsActive = false;
	return S_OK;
}

HRESULT CWitchBladeWeapon::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	_tchar szMeshTag[MAX_PATH] = L"";
	lstrcpy(szMeshTag, L"Mesh_");
	lstrcat(szMeshTag, m_wstrName.c_str());
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, szMeshTag);
	if(pComponent == NULL)
		return E_FAIL;
	m_pMeshCom = (Engine::CStaticMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	//Renderer
	pComponent = Engine::Get_Renderer();
	if(pComponent == NULL)
		return E_FAIL;
	m_pRendererCom = (Engine::CRenderer*)pComponent;
	m_pRendererCom->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));

	//Shader
	pComponent = Engine::Clone_Prototype(L"Shader_StaticMesh");
	if(pComponent == NULL)
		return E_FAIL;
	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));
	return S_OK;
}

HRESULT CWitchBladeWeapon::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vLightDir", &_vec4(1.0f, -1.0f, 1.0f, 0.0f));

	//const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);
		
	//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	////지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 40.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bOutline", m_bOutlineCheck);
	pEffect->SetFloat("g_fColorCheck", 0.0f);
	pEffect->SetFloat("g_fOutlineThickness", 0.00045f);

	pEffect->SetBool("g_bLimLight", true);
	pEffect->SetFloat("g_bOutlinePower", 18.0f);
	
	if (Engine::SCENE_SELECT == Engine::GetSceneID())
		pEffect->SetVector("g_vColor", &_vec4(1.0f, 1.0f, 1.0f, 0.0f));
	else if (Engine::SCENE_TOWN == Engine::GetSceneID())
		pEffect->SetVector("g_vColor", &_vec4(0.9411f, 0.5019f, 0.5019f, 0.0f));
	else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
		pEffect->SetVector("g_vColor", &_vec4(0.5294f, 0.8078f, 0.9215f, 0.0f));
	else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
		pEffect->SetVector("g_vColor", &_vec4(1.0f, 0.6274f, 0.4784f, 0.0f));

	pEffect->SetBool("g_bSpecularCheck", true);
	pEffect->SetBool("g_bBlurCheck", true);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CWitchBladeWeapon::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = Engine::GetCamera(Engine::CAMERA_LIGHT)->GetMatView();
	matLightProj = Engine::GetCamera(Engine::CAMERA_LIGHT)->GetMatProj();

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CWitchBladeWeapon::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	CLightCamera* pLightCam = (CLightCamera*)Engine::GetCamera(Engine::CAMERA_LIGHT);

	pEffect->SetMatrix("g_matLightView", &pLightCam->GetMatView());

	if (0 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(0));
	else if (1 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(1));
	else if (2 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(2));

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CWitchBladeWeapon* CWitchBladeWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _tchar* pTargetTag, const _tchar* pFilePath)
{
	CWitchBladeWeapon* pInstance = new CWitchBladeWeapon(pGraphicDev, wstrName);
	if(FAILED(pInstance->Ready_Object(pTargetTag, pFilePath)))
	{
		MSG_BOX("CWitchBladeWeapon Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void CWitchBladeWeapon::Free(void)
{
	CWeapon::Free();
}