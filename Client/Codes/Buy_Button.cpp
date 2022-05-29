#include "stdafx.h"
#include "Buy_Button.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Partition_Base.h"
#include "UI_Item.h"
#include "..\Engine\Utility\Codes\Scene.h"
#include "UI_Potion.h"
#include "Cursor.h"
#include "Scene_Stage_Ice.h"
#include "Buy_Base.h"
#include "UI_Weapon.h"
#include "Inven_Sheet.h"
#include "Player.h"
#include "UI_Helmet.h"
#include "UI_Armor.h"
#include "UI_Shorder.h"
#include "UI_Leg.h"
#include "UI_Glove.h"
#include "UI_Boots.h"
#include "UI_Amulet.h"
#include "UI_Armlet.h"
#include "UI_Belt.h"
#include "UI_Earing.h"
#include "UI_Ring.h"
#include "Shop_Sheet.h"
#include "Shop_Block.h"
#include "Sound.h"

CBuy_Button::CBuy_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pNCTextureCom(NULL)
	, m_pCTextureCom(NULL)
	, m_pClickTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_bType(false)
	, m_pScene(NULL)
	, m_pSystem(NULL)
{

}

CBuy_Button::~CBuy_Button(void)
{

}

HRESULT CBuy_Button::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (m_bType)
	{
		m_fX = m_pParent->GetInfo().x - 75.f;
		m_fY = m_pParent->GetInfo().y;
	}
	else
	{
		m_fX = m_pParent->GetInfo().x + 75.f;
		m_fY = m_pParent->GetInfo().y;
	}

	m_fSizeX = 100.f;
	m_fSizeY = 40.f;

	m_fAlpha = 1.f;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CBuy_Button::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN /*&& Engine::GetSceneID() != Engine::SCENE_STAGE_ICE*/)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	CUI::Start_ForScene();
	m_pScene = const_cast<Engine::CScene*>(Engine::GetScene());
	return S_OK;
}

void CBuy_Button::Update_Object(const _float& fTimeDelta)
{
	m_bRender = m_pParent->GetRender();

	if (m_bRender)
	{
		if (m_bType)
		{
			m_fX = m_pParent->GetInfo().x - 75.f;
			m_fY = m_pParent->GetInfo().y + 70.f;
		}
		else
		{
			m_fX = m_pParent->GetInfo().x + 75.f;
			m_fY = m_pParent->GetInfo().y + 70.f;
		}

		CUI::Update_Object(fTimeDelta);
		PtInRect_Collision(fTimeDelta);
		m_wSortNum = m_pParent->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
		m_bWindowProcess = false;
		m_bSelect = false;
		m_bMove = false;
	}

}

void CBuy_Button::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect)
		return;

	_uint			iPassCnt;
	_float			fFontAlpha;

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

	m_fAlpha = m_pParent->GetAlpha();
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	if (m_bClick)
	{
		fFontAlpha = 0.75f;
		m_pClickTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}
	else
	{
		if (m_bCollision)
		{
			fFontAlpha = 1.f;
			m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
		}
		else
		{
			fFontAlpha = 0.75f;
			m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
		}
	}

	if (m_fAlpha != 1.f)
		fFontAlpha = 0.f;

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);


if (m_bType)
Engine::Render_Font(L"Font_Main_Button", L"확인", &_vec2(m_fX - 10.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, fFontAlpha));
else
Engine::Render_Font(L"Font_Main_Button", L"취소", &_vec2(m_fX - 10.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, fFontAlpha));


}


HRESULT CBuy_Button::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component 
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Button_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Button_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pClickTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Button_Click");
	pComponent = (Engine::CComponent*)m_pClickTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Click", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_UI");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Calculater Component
	pComponent = m_pCalCom = Engine::CUICalculater::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_UICal", pComponent));

	// Sound
	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");
	m_pSound->Set_Sound(L"ItemSell", L"OutSwap.wav");

	return S_OK;
}
void CBuy_Button::PtInRect_Collision(const _float& fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bCollision = m_pCalCom->Collision_ByMouse(WINSIZEX
		, WINSIZEY
		, m_fX
		, m_fY
		, m_fSizeX
		, m_fSizeY
		, ptMouse);


	if (m_bCollision)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if (m_bClick)
				return;
			m_bClick = true;


			if (m_bType)
			{
				if (((CBuy_Base*)m_pParent)->GetItem() == NULL)
					return;

				if (((CBuy_Base*)m_pParent)->GetSheet()->GetIndex() == 1)
				{
					CUI* pUI = NULL;
					CUI* pInven = NULL;

					switch (((CShop_Sheet*)((CBuy_Base*)m_pParent)->GetSheet())->GetShopID())
					{
					case SHOP_WEAPON:
						pUI = ((CUI*)Engine::Find(L"Shop_Sheet_Weapon_Sell"));
						if (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_CASH)
							pInven = ((CUI*)Engine::Find(L"Inven_Sheet_3"));
						else if (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_NORMAL)
							pInven = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
						((CShop_Block*)(*((CShop_Sheet*)pUI)->GetVecBlock())[((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(NULL);
						((CBuy_Base*)m_pParent)->GetItem()->SetParent(pInven);
						((CInven_Sheet*)pInven)->AddInven(((CBuy_Base*)m_pParent)->GetItem());
						::Safe_Release(pInven);
						::Safe_Release(pUI);
						break;
					case SHOP_ARMOR:
						pUI = ((CUI*)Engine::Find(L"Shop_Sheet_Armor_Sell"));
						if (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_CASH)
							pInven = ((CUI*)Engine::Find(L"Inven_Sheet_3"));
						else if (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_NORMAL)
							pInven = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
						((CShop_Block*)(*((CShop_Sheet*)pUI)->GetVecBlock())[((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(NULL);
						((CBuy_Base*)m_pParent)->GetItem()->SetParent(pInven);
						((CInven_Sheet*)pInven)->AddInven(((CBuy_Base*)m_pParent)->GetItem());
						::Safe_Release(pInven);
						::Safe_Release(pUI);
						break;
					case SHOP_ACC:
						pUI = ((CUI*)Engine::Find(L"Shop_Sheet_Acc_Sell"));
						if (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_CASH)
							pInven = ((CUI*)Engine::Find(L"Inven_Sheet_3"));
						else if (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_NORMAL)
							pInven = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
						((CShop_Block*)(*((CShop_Sheet*)pUI)->GetVecBlock())[((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(NULL);
						((CBuy_Base*)m_pParent)->GetItem()->SetParent(pInven);
						((CInven_Sheet*)pInven)->AddInven(((CBuy_Base*)m_pParent)->GetItem());
						::Safe_Release(pInven);
						::Safe_Release(pUI);
						break;
					case SHOP_POTION:
						pUI = ((CUI*)Engine::Find(L"Shop_Sheet_Potion_Sell"));
						if (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_CASH)
							pInven = ((CUI*)Engine::Find(L"Inven_Sheet_3"));
						else if (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_NORMAL)
							pInven = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
						((CShop_Block*)(*((CShop_Sheet*)pUI)->GetVecBlock())[((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(NULL);
						((CBuy_Base*)m_pParent)->GetItem()->SetParent(pInven);
						((CInven_Sheet*)pInven)->AddInven(((CBuy_Base*)m_pParent)->GetItem());
						::Safe_Release(pInven);
						::Safe_Release(pUI);
						break;
					}
					m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
					((CBuy_Base*)m_pParent)->SetItem(NULL);
					m_pParent->SetRender(false);
				}
				else
				{
					Engine::CGameObject*	pGameObject = NULL;
					CUI*	pSheet1 = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
					CUI*	pSheet3 = ((CUI*)Engine::Find(L"Inven_Sheet_3"));
					::Safe_Release(pSheet1);
					::Safe_Release(pSheet3);

					switch (((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_eItemType)
					{
					case ITEM_BLUE_WEAPON:
						pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_Blue", pSheet1, WEAPON_BLUE, false);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_RED_WEAPON:
						pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_RED", pSheet1, WEAPON_RED, false);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_S_BLUE_WEAPON:
						pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_S_Blue", pSheet1, WEAPON_S_BLUE, false);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_S_RED_WEAPON:
						pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_S_RED", pSheet1, WEAPON_S_RED, false);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_HELMET:
						pGameObject = CUI_Helmet::Create(m_pGraphicDev, L"Helmet_Normal", pSheet1, HELMET_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_S_HELMET:
						pGameObject = CUI_Helmet::Create(m_pGraphicDev, L"Helmet_S_Normal", pSheet1, S_HELMET);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_ARMOR:
						pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_Normal", pSheet1, ARMOR_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;
					case ITEM_S_ARMOR:
						pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_S_Normal", pSheet1, S_ARMOR);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_SHORDER:
						pGameObject = CUI_Shorder::Create(m_pGraphicDev, L"Shorder_Normal", pSheet1, SHORDER_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;
					case ITEM_S_SHORDER:
						pGameObject = CUI_Shorder::Create(m_pGraphicDev, L"Shorder_S_Normal", pSheet1, S_SHORDER);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_LEG:
						pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_Normal", pSheet1, LEG_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;
					case ITEM_S_LEG:
						pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_S_Normal", pSheet1, S_LEG);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_GLOVE:
						pGameObject = CUI_Glove::Create(m_pGraphicDev, L"Glove_Normal", pSheet1, GLOVE_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;
					case ITEM_S_GLOVE:
						pGameObject = CUI_Glove::Create(m_pGraphicDev, L"Glove_S_Normal", pSheet1, S_GLOVE);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_BOOTS:
						pGameObject = CUI_Boots::Create(m_pGraphicDev, L"Boots_Normal", pSheet1, BOOTS_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;
					case ITEM_S_BOOTS:
						pGameObject = CUI_Boots::Create(m_pGraphicDev, L"Boots_Normal", pSheet1, S_BOOTS);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_AMULET:
						pGameObject = CUI_Amulet::Create(m_pGraphicDev, L"Amulet_Normal", pSheet1, AMULET_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_ARMLET:
						pGameObject = CUI_Armlet::Create(m_pGraphicDev, L"Armlet_Normal", pSheet1, ARMLET_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_BELT:
						pGameObject = CUI_Belt::Create(m_pGraphicDev, L"Belt_Normal", pSheet1, BELT_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_EARING:
						pGameObject = CUI_Earing::Create(m_pGraphicDev, L"Earing_Normal", pSheet1, EARING_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_RING:
						pGameObject = CUI_Ring::Create(m_pGraphicDev, L"Ring_Normal", pSheet1, RING_NORMAL);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_RING2:
						pGameObject = CUI_Ring::Create(m_pGraphicDev, L"Ring2_Normal", pSheet1, RING_NORMAL2);
						((CInven_Sheet*)pSheet1)->AddInven(((CUI_Item*)pGameObject));
						Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
						m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
						break;

					case ITEM_HPPOTION:
						for (_uint i = 0; i < ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCount; ++i)
						{
							pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_HP", pSheet3, POTION_HP);
							((CInven_Sheet*)pSheet3)->AddInven(((CUI_Item*)pGameObject));
							Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
							m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
							pGameObject->Start_ForScene();
						}
						break;

					case ITEM_MPPOTION:
						for (_uint i = 0; i < ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCount; ++i)
						{
							pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_MP", pSheet3, POTION_MP);
							((CInven_Sheet*)pSheet3)->AddInven(((CUI_Item*)pGameObject));
							Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
							m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
							pGameObject->Start_ForScene();
						}
						break;

					case ITEM_LEVELPOTION:
						for (_uint i = 0; i < ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCount; ++i)
						{
							pGameObject = CUI_Potion::Create(m_pGraphicDev, L"Potion_Level", pSheet3, POTION_LEVEL);
							((CInven_Sheet*)pSheet3)->AddInven(((CUI_Item*)pGameObject));
							Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
							m_pPlayer->GetPlayerInfo()->m_iMoney -= ((CBuy_Base*)m_pParent)->GetItem()->Get_Item_Info()->m_iCost;
							pGameObject->Start_ForScene();
						}
						break;
					}
					m_pSound->MyPlaySound(L"ItemSell");
					m_pParent->SetRender(false);
				}
			}
			else
			{
				m_pSound->MyPlaySound(L"ButtonPick");
				m_pParent->SetRender(false);
			}
		}
		else
			m_bClick = false;
	}
}

void CBuy_Button::SetParent(CUI* pParent)
{
	m_pParent = pParent;
}

CBuy_Button* CBuy_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBuy_Button*		pInstance = new CBuy_Button(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CBuy_Button* CBuy_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParent, _bool bType, Engine::CScene* pScene, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CBuy_Button*		pInstance = new CBuy_Button(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParent);
	pInstance->SetType(bType);
	pInstance->SetScene(pScene);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBuy_Button::Free(void)
{
	CUI::Free();
}
