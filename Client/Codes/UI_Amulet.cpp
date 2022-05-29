#include "stdafx.h"
#include "UI_Amulet.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CUI_Amulet::CUI_Amulet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI_Item(pGraphicDev, wstrName, eState, bIsActive)
{

}

CUI_Amulet::~CUI_Amulet(void)
{

}

HRESULT CUI_Amulet::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pItemInfo = new ITEM_INFO;
	memset(m_pItemInfo, NULL, sizeof(ITEM_INFO));

	SetInfo(m_eAmuletID);

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;

	m_fAlpha = 1.f;

	return S_OK;
}

HRESULT CUI_Amulet::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
		m_bIsActive = false;
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;
	return S_OK;
}

void CUI_Amulet::Update_Object(const _float& fTimeDelta)
{
	if(m_pParentWindow->GetSheetSelect()
		&& m_pParentWindow->GetRender())
	{		
		CUI::Update_Object(fTimeDelta);
		if(!m_bMousePick)
			m_wSortNum = m_pParentWindow->GetSortNum();
		else
			m_wSortNum = 10;
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	if (m_bEqu && !m_pParentWindow->GetSheetSelect())
	{
		CUI::Update_Object(fTimeDelta);
		m_wSortNum = m_pParentWindow->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
}

void CUI_Amulet::Render_Object(void)
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
}

void CUI_Amulet::SetId(AMULET_ID Id)
{
	m_eAmuletID = Id;
}

void CUI_Amulet::SetInfo(AMULET_ID Id)
{
	switch(m_eAmuletID)
	{
	case AMULET_NORMAL:
		lstrcpy(m_pItemInfo->m_szName, L"Amulet_Normal");
		m_pItemInfo->m_eItemType = ITEM_AMULET;
		m_pItemInfo->m_eSlotType = SLOT_NORMAL;
		m_pItemInfo->m_iSlotIndex = 8;
		m_pItemInfo->m_iStr	= 30;
		m_pItemInfo->m_iCon = 30;
		m_pItemInfo->m_iWis = 30;
		m_pItemInfo->m_iInt = 30;
		m_pItemInfo->m_iRightAtt = 1500;
		m_pItemInfo->m_iLeftAtt = 1500;
		m_pItemInfo->m_iPhysicalDefense = 800;
		m_pItemInfo->m_iCost = 15000;
		break;
	}
}


HRESULT CUI_Amulet::Add_Component(void)
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

	switch(m_eAmuletID)
	{
	case AMULET_NORMAL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Amulet_Normal");
		break;
	}
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

	// For.Calculater Component
	pComponent = m_pCalCom = Engine::CUICalculater::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_UICal", pComponent));

	return S_OK;
}

CUI_Amulet* CUI_Amulet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Amulet*		pInstance = new CUI_Amulet(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Weapon Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CUI_Amulet* CUI_Amulet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, AMULET_ID eID, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Amulet*		pInstance = new CUI_Amulet(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetId(eID);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Weapon Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Amulet::Free(void)
{
	CUI_Item::Free();
}

