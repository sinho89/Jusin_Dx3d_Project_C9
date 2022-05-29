#include "stdafx.h"
#include "Grid.h"

#include "Grid_Col.h"
#include "Export_Engine.h"

CGrid::CGrid(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pBufferCom(NULL)
, m_pRendererCom(NULL)
{

}
CGrid::~CGrid(void)
{

}

void CGrid::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);
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

HRESULT CGrid::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vPos = _vec3(-10.f, 0.f, -10.f);

	m_pTransCom->Set_Position(&vPos);

	return S_OK;
}

HRESULT CGrid::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	m_pBufferCom = (Engine::CGrid_Col*)Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Grid");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if(pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Renderer
	pComponent = Engine::Get_Renderer();
	if(pComponent == NULL)
		return E_FAIL;
	m_pRendererCom = (Engine::CRenderer*)pComponent;
	m_pRendererCom->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));

	return S_OK;
}

CGrid* CGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
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