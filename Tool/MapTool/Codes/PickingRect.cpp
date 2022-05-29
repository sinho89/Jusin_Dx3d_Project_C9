#include "stdafx.h"
#include "PickingRect.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CPickingRect::CPickingRect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
:CGameObject(pGraphicDev, wstrName)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
//,m_pVtx(NULL)
//,m_pConvertVtx(NULL)
//,m_iVtx(0)
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

	m_pTransCom->Set_Scale(5.f, 5.f, 1.f);
	//m_iVtx = 4;
	//m_pVtx = new Engine::VTXTEX[m_iVtx];
	//m_pConvertVtx = new Engine::VTXTEX[m_iVtx];



	return S_OK;
}

HRESULT CPickingRect::Start_ForScene(void)
{
	return S_OK;
}

void CPickingRect::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	_vec3 vPos;
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
	_vec3 vLook = g_vCamEye - vPos;
	//m_pTransCom->Get_Infomation(Engine::INFO_LOOK, &vLook);

	D3DXVec3Normalize(&vLook, &vLook);
	float fAngle = acosf(D3DXVec3Dot(&vLook, &_vec3(0.f, 0.f, -1.f))) * 180.f/3.141592f;
	float fAngle2 = D3DXVec3Dot(&vLook, &D3DXVECTOR3(1.f, 0.f, 0.f));

	if(fAngle2 >= 0)
	{
		m_pTransCom->Set_Angle(&_vec3(0.f, -fAngle, 0.f));
	}
	else
		m_pTransCom->Set_Angle(&_vec3(0.f, fAngle, 0.f));

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	_vec3	vScale;

	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));
}

void CPickingRect::Render_Object(void)
{
	//if(m_iIndex < 0 || m_iIndex > 4)
	//	return;

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

void CPickingRect::Alpha_Calculation(const _float& fTimeDelta)
{
	//m_fAlphaTime += fTimeDelta;

	//if(m_fAlpha <= 0.f)
	//	m_bAlpha_Increase = true;
	//if(m_fAlpha >= 1.f)
	//	m_bAlpha_Increase = false;

	//if(m_fAlphaTime >= 0.005f)
	//{
	//	m_fAlphaTime = 0.f;

	//	if(m_bAlpha_Increase)
	//		m_fAlpha += 0.01f;

	//}
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

	// For.Shader Component
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
