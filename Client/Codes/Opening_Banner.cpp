#include "stdafx.h"
#include "Opening_Banner.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

COpening_Banner::COpening_Banner(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

COpening_Banner::~COpening_Banner(void)
{

}

HRESULT COpening_Banner::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Scale(0.75f, 0.75f, 1.f);

	return S_OK;
}

HRESULT COpening_Banner::Start_ForScene(void)
{
	return S_OK;
}

void COpening_Banner::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);
}

void COpening_Banner::Render_Object(void)
{
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

	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();
}

void COpening_Banner::Alpha_Calculation(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	/*if(m_fAlpha <= 0.f)
		m_bAlpha_Increase = true;
	if(m_fAlpha >= 1.f)
		m_bAlpha_Increase = false;

	if(m_fAlphaTime >= 0.05f)
	{
		m_fAlphaTime = 0.f;

		if(m_bAlpha_Increase)
			m_fAlpha += 0.01f;
		else
			m_fAlpha -= 0.01f;
	}*/

	if(m_fAlphaTime >= 0.05f)
	{
		m_fAlphaTime = 0.f;

		if(m_bAlpha_Increase)
		{
			m_fAlpha -= 0.00001f;
		}
		else
		{
			m_fAlpha += 0.01f;
			if(m_fAlpha >= 1.0f)
				m_bAlpha_Increase = TRUE;
		}
	}
}

HRESULT COpening_Banner::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGO, L"Texture_Opening_Banner");
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

COpening_Banner* COpening_Banner::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	COpening_Banner*		pInstance = new COpening_Banner(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void COpening_Banner::Free(void)
{
	CUI::Free();
}
