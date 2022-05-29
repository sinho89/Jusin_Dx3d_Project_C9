#include "stdafx.h"
#include "Tired_Gage.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Fade_Manager.h"

CTired_Gage::CTired_Gage(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

CTired_Gage::~CTired_Gage(void)
{

}

HRESULT CTired_Gage::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	if(!m_iIndex)
	{
		m_fX = (WINSIZEX >> 1) - 165.f;
		m_fY = (WINSIZEY >> 1) + 432.5f;
	}
	else
	{
		m_fX = (WINSIZEX >> 1) + 165.f;
		m_fY = (WINSIZEY >> 1) + 432.5f;
	}

	m_fSizeX = 250.f;
	m_fSizeY = 10.f;

	m_fAlpha = 1.f;
	m_iTiredCount = 150;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CTired_Gage::Start_ForScene(void)
{

	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
		m_bIsActive = false;
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;
	return S_OK;
}

void CTired_Gage::Update_Object(const _float& fTimeDelta)
{
	if (CFade_Manager::GetInstance()->GetFading() == true)
		return;
	CUI::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CTired_Gage::Render_Object(void)
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
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	if(!m_iIndex)
	{
		_tchar		szInfo[128] = L"";
		wsprintf(szInfo, L"%d", m_iTiredCount);
		Engine::Render_Font(L"Font_Main_Slot", szInfo, &_vec2((WINSIZEX >> 1) - 10.f, m_fY - 5.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	}
}


HRESULT CTired_Gage::Add_Component(void)
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

	if(!m_iIndex)
	{
		// For.Texture Component 
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Main_Tired_Gage_Left");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
	}
	else
	{
		// For.Texture Component 
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Main_Tired_Gage_Right");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	}
	
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

CTired_Gage* CTired_Gage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CTired_Gage*		pInstance = new CTired_Gage(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CTired_Gage* CTired_Gage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CTired_Gage*		pInstance = new CTired_Gage(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetIndex(iIndex);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CTired_Gage::Free(void)
{
	CUI::Free();
}
