#include "stdafx.h"
#include "MapObj.h"

#include "Export_Engine.h"

//#include "Camera_MapTool.h"
//#include "MouseCol.h"

CMapObj::CMapObj(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_pRendererCom(NULL)
, m_pResourceMgr(NULL)
{

}
CMapObj::~CMapObj(void)
{

}

void CMapObj::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	_vec3	vScale;
	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

}

void CMapObj::Render_Object(void)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect) return;

	pEffect->AddRef();

	if(FAILED(Set_ConstantTable(pEffect))) return;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

}

void CMapObj::SetID( MAPID eID )
{
	m_eID = eID;
}


HRESULT CMapObj::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

//	m_pMouseCol = CMouseCol::Create();
//	m_pTransCom->Set_Scale(1.f, 0.0001f, 0.0001f);

	

	return S_OK;
}

HRESULT CMapObj::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	switch(m_eID)
	{
	case MAP_SHOP:
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Shop");
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
		break;
	case MAP_ICE:
		//Mesh
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Ice");
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
		break;
	}


	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Object"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

HRESULT CMapObj::Set_ConstantTable( LPD3DXEFFECT pEffect )
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo();

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);

	Engine::Safe_Release(pEffect);

	return S_OK;
}


CMapObj* CMapObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, MAPID eID)
{
	CMapObj*		pInstance = new CMapObj(pGraphicDev, wstrName);

	pInstance->SetID(eID);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CMapObj Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMapObj::Free(void)
{
	Engine::CGameObject::Free();
//	Safe_Delete(m_pMouseCol);
}



