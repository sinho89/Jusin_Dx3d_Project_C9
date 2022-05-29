#include "stdafx.h"
#include "PickingRect.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Camera_AniTool.h"

CPickingRect::CPickingRect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
:CGameObject(pGraphicDev, wstrName)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_fDist(-1.0f)
,m_bRenderCheck(true)
{
	ZeroMemory(&m_pTextureCom, NULL);
}

CPickingRect::~CPickingRect(void)
{

}

HRESULT CPickingRect::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Scale(2.f, 2.f, 1.f);


	return S_OK;
}

HRESULT CPickingRect::Start_ForScene(void)
{
	return S_OK;
}

void CPickingRect::Update_Object(const _float& fTimeDelta)
{

	CGameObject::Update_Object(fTimeDelta);


	D3DXMATRIX	matBill;
	D3DXMatrixIdentity(&matBill);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matBill);

	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	D3DXMATRIX  matWorld;
	
	m_pTransCom->Get_WorldMatrix(&matWorld);

	matWorld  *= matBill;

	memcpy(&matWorld.m[3][0], &m_pTransCom->Get_Now_Position(), sizeof(D3DXVECTOR3));


	m_pTransCom->Set_WorldMatrix(&matWorld);



	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	_vec3	vScale;

	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));
}

void CPickingRect::Render_Object(void)
{

	if(!m_bRenderCheck)
		return ;


	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	_uint			iPassCnt;

	_matrix				matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetFloat("g_fAlpha", 1);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CPickingRect::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Collider_Blue");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));


	return S_OK;
}

CPickingRect* CPickingRect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CPickingRect*		pInstance = new CPickingRect(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CPickingRect Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPickingRect::Free(void)
{
	CGameObject::Free();
}

void CPickingRect::Set_Positon( _vec3* vPos)
{
	m_pTransCom->Set_Position(vPos);
}


Engine::CRect_Texture* CPickingRect::Get_BufferCom( void )
{
	return m_pBufferCom;
}

Engine::CTransform* CPickingRect::Get_TransCom( void )
{
	return m_pTransCom;
}

void CPickingRect::InitDist(void)
{
	m_fDist = -1.0f;
}
