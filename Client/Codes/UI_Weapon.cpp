#include "stdafx.h"
#include "UI_Weapon.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CUI_Weapon::CUI_Weapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI_Item(pGraphicDev, wstrName, eState, bIsActive)
{

}

CUI_Weapon::~CUI_Weapon(void)
{

}

HRESULT CUI_Weapon::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pItemInfo = new ITEM_INFO;
	memset(m_pItemInfo, NULL, sizeof(ITEM_INFO));

	switch(m_eWeaponID)
	{
	case WEAPON_BLUE:
		SetInfo(WEAPON_BLUE);
		m_wSortNum = 3;
		break;
	case WEAPON_RED:
		SetInfo(WEAPON_RED);
		m_wSortNum = 2;
		break;
	case WEAPON_S_BLUE:
		SetInfo(WEAPON_S_BLUE);
		m_wSortNum = 2;
		break;
	case WEAPON_S_RED:
		SetInfo(WEAPON_S_RED);
		m_wSortNum = 2;
		break;
	}

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;

	m_fAlpha = 1.f;

	return S_OK;
}

HRESULT CUI_Weapon::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
		m_bIsActive = false;
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;
	return S_OK;
}

void CUI_Weapon::Update_Object(const _float& fTimeDelta)
{
	
	if (m_bResult)
	{
		m_bIsStatic = false;
		CUI::Update_Object(fTimeDelta);
		m_pRendererCom->Add_WindowUI(30007, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		m_bIsStatic = true;
		if (m_pParentWindow->GetSheetSelect()
			&& m_pParentWindow->GetRender())
		{
			CUI::Update_Object(fTimeDelta);
			if (!m_bMousePick)
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
	
}

void CUI_Weapon::Render_Object(void)
{
	if (!m_bLeftWeapon && m_bRightWeapon)
	{
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		if (NULL == pEffect)
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
		if (m_bFade)
			pEffect->BeginPass(7);
		else
			pEffect->BeginPass(2);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

		matWorld._41 = m_fX - WINSIZEX * 0.5f - 165.f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &m_matProj);

		pEffect->SetFloat("g_fAlpha", m_fAlpha);

		m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

		pEffect->Begin(&iPassCnt, 0);
		if (m_bFade)
			pEffect->BeginPass(7);
		else
			pEffect->BeginPass(2);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
	}
	else if (m_bLeftWeapon && !m_bRightWeapon)
	{
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		if (NULL == pEffect)
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
		if (m_bFade)
			pEffect->BeginPass(7);
		else
			pEffect->BeginPass(2);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

		matWorld._41 = m_fX - WINSIZEX * 0.5f - 165.f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &m_matProj);

		pEffect->SetFloat("g_fAlpha", m_fAlpha);

		m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

		pEffect->Begin(&iPassCnt, 0);
		if (m_bFade)
			pEffect->BeginPass(7);
		else
			pEffect->BeginPass(2);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
	}
	else
	{
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		if (NULL == pEffect)
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
		if (m_bFade)
			pEffect->BeginPass(7);
		else
			pEffect->BeginPass(2);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
	}
	
}

void CUI_Weapon::SetId(WEAPON_ID Id)
{
	m_eWeaponID = Id;
}

void CUI_Weapon::SetInfo(WEAPON_ID Id)
{
	switch(m_eWeaponID)
	{
	case WEAPON_BLUE:
		lstrcpy(m_pItemInfo->m_szName, L"Weapon_Blue");
		m_pItemInfo->m_eItemType = ITEM_BLUE_WEAPON;
		m_pItemInfo->m_eSlotType = SLOT_NORMAL;
		m_pItemInfo->m_iSlotIndex = 0;
		m_pItemInfo->m_iStr	= 83;
		m_pItemInfo->m_iRightAtt = 2325;
		m_pItemInfo->m_iLeftAtt = 3274;
		m_pItemInfo->m_fCritical = 15.f;
		m_pItemInfo->m_iCost = 7000;
		m_pItemInfo->m_fHitAaccuracy = 5.f;
		m_pItemInfo->m_fFireAtt = 20.f;
		m_pItemInfo->m_fSkyAtt = 20.f;
		m_pItemInfo->m_fLandAtt = 20.f;
		m_pItemInfo->m_fWaterAtt = 20.f;
		m_pItemInfo->m_fAttSpeed = 6.f;
		m_pItemInfo->m_iDurabilty = 150;
		m_pItemInfo->m_iMaxDurabilty = 150;
		m_pItemInfo->m_fConcentration = 10.f;
		break;
	case WEAPON_RED:
		lstrcpy(m_pItemInfo->m_szName, L"Weapon_Red");
		m_pItemInfo->m_eItemType = ITEM_RED_WEAPON;
		m_pItemInfo->m_eSlotType = SLOT_NORMAL;
		m_pItemInfo->m_iSlotIndex = 1;
		m_pItemInfo->m_iStr	= 150;
		m_pItemInfo->m_iWis = 80;
		m_pItemInfo->m_iInt = 80;
		m_pItemInfo->m_iRightAtt = 3300;
		m_pItemInfo->m_iLeftAtt = 2600;
		m_pItemInfo->m_fCritical = 20.f;
		m_pItemInfo->m_iCost = 7500;
		m_pItemInfo->m_fHitAaccuracy = 50.f;
		m_pItemInfo->m_fFireAtt = 20.f;
		m_pItemInfo->m_fSkyAtt = 20.f;
		m_pItemInfo->m_fLandAtt = 20.f;
		m_pItemInfo->m_fWaterAtt = 20.f;
		m_pItemInfo->m_iDurabilty = 150;
		m_pItemInfo->m_iMaxDurabilty = 150;
		m_pItemInfo->m_fConcentration = 10.f;
		break;
	case WEAPON_S_BLUE:
		lstrcpy(m_pItemInfo->m_szName, L"Weapon_S_Blue");
		m_pItemInfo->m_eItemType = ITEM_S_BLUE_WEAPON;
		m_pItemInfo->m_eSlotType = SLOT_NORMAL;
		m_pItemInfo->m_iSlotIndex = 0;
		m_pItemInfo->m_iInt = 99;
		m_pItemInfo->m_iRightAtt = 5044;
		m_pItemInfo->m_iLeftAtt = 7064;
		m_pItemInfo->m_fCritical = 15.f;
		m_pItemInfo->m_iCost = 7000;
		m_pItemInfo->m_fHitAaccuracy = 5.f;
		m_pItemInfo->m_fFireAtt = 20.f;
		m_pItemInfo->m_fSkyAtt = 20.f;
		m_pItemInfo->m_fLandAtt = 20.f;
		m_pItemInfo->m_fWaterAtt = 20.f;
		m_pItemInfo->m_fAttSpeed = 6.f;
		m_pItemInfo->m_iDurabilty = 150;
		m_pItemInfo->m_iMaxDurabilty = 150;
		m_pItemInfo->m_fConcentration = 10.f;
		break;
	case WEAPON_S_RED:
		lstrcpy(m_pItemInfo->m_szName, L"Weapon_S_Red");
		m_pItemInfo->m_eItemType = ITEM_S_RED_WEAPON;
		m_pItemInfo->m_eSlotType = SLOT_NORMAL;
		m_pItemInfo->m_iSlotIndex = 0;
		m_pItemInfo->m_iInt = 200;
		m_pItemInfo->m_iWis = 200;
		m_pItemInfo->m_iRightAtt = 9053;
		m_pItemInfo->m_iLeftAtt = 9688;
		m_pItemInfo->m_iCost = 7000;
		m_pItemInfo->m_fFireAtt = 50.f;
		m_pItemInfo->m_fSkyAtt = 50.f;
		m_pItemInfo->m_fLandAtt = 50.f;
		m_pItemInfo->m_fWaterAtt = 50.f;
		m_pItemInfo->m_fAttSpeed = 6.f;
		m_pItemInfo->m_iDurabilty = 150;
		m_pItemInfo->m_iMaxDurabilty = 150;
		m_pItemInfo->m_fConcentration = 10.f;
		break;
	}
}


HRESULT CUI_Weapon::Add_Component(void)
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

	switch(m_eWeaponID)
	{
	case WEAPON_BLUE:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Weapon_Blue");
		break;
	case WEAPON_RED:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Weapon_Red");
		break;
	case WEAPON_S_BLUE:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Shaman_Blue_Weapon");
		break;
	case WEAPON_S_RED:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Shaman_Red_Weapon");
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

CUI_Weapon* CUI_Weapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Weapon*		pInstance = new CUI_Weapon(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Weapon Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CUI_Weapon* CUI_Weapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, WEAPON_ID eID, _bool bFade, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Weapon*		pInstance = new CUI_Weapon(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetId(eID);
	pInstance->SetFade(bFade);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Weapon Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Weapon::Free(void)
{
	CUI_Item::Free();
}

