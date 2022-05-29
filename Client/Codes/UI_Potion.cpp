#include "stdafx.h"
#include "UI_Potion.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Fade_Manager.h"

CUI_Potion::CUI_Potion(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI_Item(pGraphicDev, wstrName, eState, bIsActive)
,m_pMainBase(NULL)
{

}

CUI_Potion::~CUI_Potion(void)
{

}

HRESULT CUI_Potion::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pItemInfo = new ITEM_INFO;
	memset(m_pItemInfo, NULL, sizeof(ITEM_INFO));

	SetInfo(m_ePotionID);

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;

	m_fAlpha = 1.f;

	return S_OK;
}

HRESULT CUI_Potion::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	m_pMainBase = (CUI*)Engine::Find(L"Main_Base");
	::Safe_Release(m_pMainBase);

	return S_OK;
}

void CUI_Potion::Update_Object(const _float& fTimeDelta)
{

	if (CFade_Manager::GetInstance()->GetFading() == true)
		return;

	if(!m_bMain)
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
	}
	else
	{
		CUI::Update_Object(fTimeDelta);
		if(m_bMousePick)
			m_wSortNum = 10;
		else
			m_wSortNum = 0;

		if(m_pMainBase->GetSlotLevel() == 1 && m_pItemInfo->m_iSlotIndex >= 0 && m_pItemInfo->m_iSlotIndex < 10)
			m_pRendererCom->Add_WindowUI(m_wSortNum, this);

		if(m_pMainBase->GetSlotLevel() == 2 && m_pItemInfo->m_iSlotIndex >= 10 && m_pItemInfo->m_iSlotIndex < 20)
			m_pRendererCom->Add_WindowUI(m_wSortNum, this);

		if(m_pMainBase->GetSlotLevel() == 3 && m_pItemInfo->m_iSlotIndex >= 20 && m_pItemInfo->m_iSlotIndex < 30)
			m_pRendererCom->Add_WindowUI(m_wSortNum, this);

		if(m_pItemInfo->m_iSlotIndex >= 30 && m_pItemInfo->m_iSlotIndex < 38)
			m_pRendererCom->Add_WindowUI(m_wSortNum, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}

	if (m_pItemInfo->m_iCount <= 0.f)
		m_eState = Engine::STATE_DESTROY;
}

void CUI_Potion::Render_Object(void)
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

	if (m_pItemInfo->m_iCount >= 2)
	{
		_tchar		szInfo[128] = L"";
		wsprintf(szInfo, L"%d", m_pItemInfo->m_iCount);	// 원천 능력
		Engine::Render_Font(L"Font_Main_Data", szInfo, &_vec2(m_fX + 10.f, m_fY + 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	}
}

void CUI_Potion::SetId(POTION_ID Id)
{
	m_ePotionID = Id;
}

void CUI_Potion::SetInfo(POTION_ID Id)
{
	switch(m_ePotionID)
	{
	case POTION_HP:
		lstrcpy(m_pItemInfo->m_szName, L"Potion_HP");
		m_pItemInfo->m_eItemType = ITEM_HPPOTION;
		m_pItemInfo->m_eSlotType = SLOT_CASH;
		m_pItemInfo->m_iSlotIndex = 0;
		m_pItemInfo->m_iCost = 500;
		m_pItemInfo->m_iHp = 500;
		m_pItemInfo->m_iCount = 1;
		break;
	case POTION_MP:
		lstrcpy(m_pItemInfo->m_szName, L"Potion_MP");
		m_pItemInfo->m_eItemType = ITEM_MPPOTION;
		m_pItemInfo->m_eSlotType = SLOT_CASH;
		m_pItemInfo->m_iSlotIndex = 1;
		m_pItemInfo->m_iCost = 500;
		m_pItemInfo->m_iMp = 500;
		m_pItemInfo->m_iCount = 1;
		break;
	case POTION_LEVEL:
		lstrcpy(m_pItemInfo->m_szName, L"Potion_Level");
		m_pItemInfo->m_eItemType = ITEM_LEVELPOTION;
		m_pItemInfo->m_eSlotType = SLOT_CASH;
		m_pItemInfo->m_iSlotIndex = 2;
		m_pItemInfo->m_iCost = 500;
		m_pItemInfo->m_iCount = 1;
		break;
	}
}


HRESULT CUI_Potion::Add_Component(void)
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

	switch(m_ePotionID)
	{
	case POTION_HP:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hp_Potion");
		break;
	case POTION_MP:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Mp_Potion");
		break;
	case POTION_LEVEL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"LevelUp_Potion");
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

CUI_Potion* CUI_Potion::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Potion*		pInstance = new CUI_Potion(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Weapon Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CUI_Potion* CUI_Potion::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, POTION_ID eID, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Potion*		pInstance = new CUI_Potion(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetId(eID);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Weapon Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Potion::Free(void)
{
	CUI_Item::Free();
}

