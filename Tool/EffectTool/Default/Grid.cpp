#include "stdafx.h"
#include "Grid.h"
#include "Export_Engine.h"
#include "Grid_Col.h"


CGrid:: CGrid(LPDIRECT3DDEVICE9	pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pRendererCom(NULL)
, m_pBufferCom(NULL)
, m_pTransCom(NULL)
{

}

CGrid::~CGrid(void)
{

}

HRESULT CGrid::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	
	return S_OK;
}

void CGrid::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
}

void CGrid::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTransCom->Set_Transform(m_pGraphicDev);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CGrid::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	// For.Buffer Component
	m_pBufferCom = (Engine::CGrid_Col*)Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Grid");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));


	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));



	return S_OK;
}

HRESULT CGrid::SetUp_Material(void)
{

	return S_OK;
}

HRESULT CGrid::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{

	return S_OK;
}

CGrid* CGrid::Create(LPDIRECT3DDEVICE9	pGraphicDev, const wstring& wstrName)
{
	CGrid*		pInstance = new CGrid(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGrid Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;

}

void CGrid::Free(void)
{
	Engine::CGameObject::Free();

}

