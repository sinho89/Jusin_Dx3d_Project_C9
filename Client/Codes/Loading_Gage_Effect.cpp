#include "stdafx.h"
#include "Loading_Gage_Effect.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CLoading_Gage_Effect::CLoading_Gage_Effect(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

CLoading_Gage_Effect::~CLoading_Gage_Effect(void)
{

}

HRESULT CLoading_Gage_Effect::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1) + 375.f;

	m_fSizeX = 40.f;
	m_fSizeY = 50.f;

	m_bLoading = true;
	return S_OK;
}

void CLoading_Gage_Effect::SetGageSize(const _float& fTimeDelta)
{
	if(m_iIndex == 0)
		m_fMagicScale = Engine::GetLoadScale() / 450.f;
	else if (m_iIndex == 1)
		m_fMagicScale = Engine::GetLoadScale() / 80.f;
	else if (m_iIndex == 2)
		m_fMagicScale = Engine::GetLoadScale() / 35.f;

	if (m_fMagicScale >= 1.f)
		m_fMagicScale = 1.f;
	m_fX = ((WINSIZEX >> 1) - 380.f) + (((WINSIZEX >> 1) - 37.5f) * m_fMagicScale);
}

HRESULT CLoading_Gage_Effect::Start_ForScene(void)
{

	if (Engine::GetSceneID() == Engine::SCENE_LOADING)
		m_bIsActive = true;
	return S_OK;
}

void CLoading_Gage_Effect::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);
	SetGageSize(fTimeDelta);
	if (Engine::GetSceneID() == Engine::SCENE_LOADING)
		m_bIsActive = true;
	else
		m_bIsActive = false;
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CLoading_Gage_Effect::Render_Object(void)
{
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

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 
}

void CLoading_Gage_Effect::Alpha_Calculation(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if(m_fAlpha <= 0.f)
		m_bAlpha_Increase = true;
	if(m_fAlpha >= 1.f)
		m_bAlpha_Increase = false;

	if(m_fAlphaTime >= 0.05f)
	{
		m_fAlphaTime = 0.f;

		if(m_bAlpha_Increase)
			m_fAlpha += 0.05f;

	}
}

HRESULT CLoading_Gage_Effect::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Loading_Effect_Gage");
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

CLoading_Gage_Effect* CLoading_Gage_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CLoading_Gage_Effect*		pInstance = new CLoading_Gage_Effect(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CLoading_Gage_Effect* CLoading_Gage_Effect::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CLoading_Gage_Effect*		pInstance = new CLoading_Gage_Effect(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetIndex(iIndex);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoading_Gage_Effect::Free(void)
{
	CUI::Free();
}
