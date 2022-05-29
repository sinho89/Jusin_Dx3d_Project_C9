#include "stdafx.h"
#include "Select_BackGround.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CSelect_BackGround::CSelect_BackGround(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{
	ZeroMemory(&m_pTextureCom, NULL);
}

CSelect_BackGround::~CSelect_BackGround(void)
{

}

HRESULT CSelect_BackGround::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_iIndex = -1;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = WINSIZEX;
	m_fSizeY = WINSIZEY;

	return S_OK;
}

HRESULT CSelect_BackGround::Start_ForScene(void)
{
	return S_OK;
}

void CSelect_BackGround::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CSelect_BackGround::Render_Object(void)
{
	if(m_iIndex < 0 || m_iIndex > 4)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	_uint			iPassCnt;

	_matrix				matWorld, matView;
	_matrix				matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINSIZEX * 0.5f;
	matWorld._42 = -m_fY + WINSIZEY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom[m_iIndex]->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CSelect_BackGround::Alpha_Calculation(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if(m_fAlpha <= 0.f)
		m_bAlpha_Increase = true;
	if(m_fAlpha >= 1.f)
		m_bAlpha_Increase = false;

	if(m_fAlphaTime >= 0.005f)
	{
		m_fAlphaTime = 0.f;

		if(m_bAlpha_Increase)
			m_fAlpha += 0.01f;
	}
}

HRESULT CSelect_BackGround::Add_Component(void)
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
	m_pTextureCom[0] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Select_Char_Back_F");
	pComponent = (Engine::CComponent*)m_pTextureCom[0];
	if(NULL == pComponent)
		return E_FAIL;

	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	m_pTextureCom[1] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Select_Char_Back_H");
	pComponent = (Engine::CComponent*)m_pTextureCom[1];
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture2", pComponent));

	m_pTextureCom[2] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Select_Char_Back_S");
	pComponent = (Engine::CComponent*)m_pTextureCom[2];
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture3", pComponent));

	m_pTextureCom[3] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Select_Char_Back_W");
	pComponent = (Engine::CComponent*)m_pTextureCom[3];
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture4", pComponent));

	m_pTextureCom[4] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Select_Char_Back_M");
	pComponent = (Engine::CComponent*)m_pTextureCom[4];
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture5", pComponent));


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

CSelect_BackGround* CSelect_BackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSelect_BackGround*		pInstance = new CSelect_BackGround(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSelect_BackGround::Free(void)
{
	CUI::Free();
}
