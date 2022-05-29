#include "stdafx.h"
#include "Shop_Sheet.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_Item.h"
#include "Cursor.h"
#include "Shop_ToolTip.h"
#include "Partition_Base.h"
#include "Shop_Block.h"
#include "Buy_Base.h"
#include "Sell_Base.h"
#include "Inven_Sheet.h"
#include "Inven_Block.h"
#include "Sound.h"

CShop_Sheet::CShop_Sheet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pNCTextureCom(NULL)
	, m_pCTextureCom(NULL)
	, m_pSTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pToolTip(NULL)
	, m_bProgressColl(false)
{

}

CShop_Sheet::~CShop_Sheet(void)
{

}

HRESULT CShop_Sheet::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = m_pParentWindow->GetInfo();

	switch (m_iIndex)
	{
	case 0:
		m_bSheetSelect = true;
		m_fX = vInfo.x - 150.f;
		m_fY = vInfo.y - 400.f;
		break;
	case 1:
		m_fX = vInfo.x + 150.f;
		m_fY = vInfo.y - 400.f;
		break;
	}
	m_fSizeX = 100.f;
	m_fSizeY = 35.f;

	m_fAlpha = 1.f;
	m_eWinType = WINDOW_SHOP;

	m_bIsStatic = true;

	return S_OK;
}

HRESULT CShop_Sheet::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN /*&& Engine::GetSceneID() != Engine::SCENE_STAGE_ICE*/)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	m_pToolTip = ((CShop_ToolTip*)((CUI*)Engine::Find(L"Shop_ToolTip")));
	m_pCursor = ((CUI*)Engine::Find(L"Cursor"));

	::Safe_Release(m_pToolTip);
	::Safe_Release(m_pCursor);
	return S_OK;
}

void CShop_Sheet::Update_Object(const _float& fTimeDelta)
{
	m_bRender = m_pParentWindow->GetRender();
	if (m_bRender)
	{
		if (!m_bProgressColl)
			m_bProgressColl = true;
		CUI::Update_Object(fTimeDelta);
		m_wSortNum = m_pParentWindow->GetSortNum();
		PtInRect_Collision(fTimeDelta);
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		ItemPick();
		BuyPotion();
		RenderToolTip();
		FadeCheck();
	}
	else
	{
		switch (m_iIndex)
		{
		case 0:
			m_bSheetSelect = true;
			break;
		case 1:
			m_bSheetSelect = false;
			break;
		}

		if (m_bProgressColl)
		{
			m_bProgressColl = false;
			m_pToolTip->SetRender(false);
		}
	}
}

void CShop_Sheet::Render_Object(void)
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

	if (m_bSheetSelect)
		m_pSTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	else
		m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");


	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	switch (m_eType)
	{
	case SHOP_WEAPON:
		if(!m_iIndex)
			Engine::Render_Font(L"Font_Main_Button", L"무기", &_vec2(m_fX - 10.f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case SHOP_ARMOR:
		if (!m_iIndex)
			Engine::Render_Font(L"Font_Main_Button", L"방어구", &_vec2(m_fX - 20.f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case SHOP_ACC:
		if (!m_iIndex)
			Engine::Render_Font(L"Font_Main_Button", L"장신구", &_vec2(m_fX - 20.f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case SHOP_POTION:
		if (!m_iIndex)
			Engine::Render_Font(L"Font_Main_Button", L"물약", &_vec2(m_fX - 10.f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	}

	if (m_iIndex)
		Engine::Render_Font(L"Font_Main_Button", L"재매입", &_vec2(m_fX - 20.f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));

}


HRESULT CShop_Sheet::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_S");
	pComponent = (Engine::CComponent*)m_pSTextureCom;
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

	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");

	return S_OK;
}
void CShop_Sheet::PtInRect_Collision(const _float& fTimeDelta)
{
	_vec4 vInfo = m_pParentWindow->GetInfo();

	switch (m_iIndex)
	{
	case 0:
		m_fX = vInfo.x - 100.f;
		m_fY = vInfo.y - 230.f;
		break;
	case 1:
		m_fX = vInfo.x + 100.f;
		m_fY = vInfo.y - 230.f;
		break;
	}

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bButtonCollision = m_pCalCom->Collision_ByMouse(WINSIZEX
		, WINSIZEY
		, m_fX
		, m_fY
		, m_fSizeX
		, m_fSizeY
		, ptMouse);

	if (m_bButtonCollision)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			m_pCursor = ((CUI*)Engine::Find(L"Cursor"));
			if (((CCursor*)m_pCursor)->GetItem() != NULL)
			{
				::Safe_Release(m_pCursor);
				return;
			}
			::Safe_Release(m_pCursor);

			CUI*	pPartitionBase = ((CUI*)Engine::Find(L"Partition_Base"));
			if (pPartitionBase->GetRender())
			{
				::Safe_Release(pPartitionBase);
				return;
			}
			::Safe_Release(pPartitionBase);


			if (m_bClick)
				return;
			m_bClick = true;
			m_pSound->MyPlaySound(L"ButtonPick");

			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Window")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Window")->end();

			for (; iter != iter_end; ++iter)
			{
				if (((CUI*)(*iter))->GetWinType() == WINDOW_SHOP)
					((CUI*)(*iter))->SetSheetSelect(false);
			}

			m_bSheetSelect = true;
		}
		else
			m_bClick = false;
	}


	if (m_bSheetSelect)
		m_fAlpha = 1.f;
	else
		m_fAlpha = 0.5f;

}

void CShop_Sheet::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}

vector<CUI*>* CShop_Sheet::GetVecBlock(void)
{
	return &m_vecBlock;
}

void CShop_Sheet::AddVecBlock(_uint iIndex, CUI* pUI)
{
	if (m_iIndex == iIndex)
	{
		m_vecBlock.push_back(pUI);
	}
}

void CShop_Sheet::ItemPick(void)
{
	for (size_t i = 0; i < m_vecBlock.size(); ++i)
	{
		if (m_vecBlock[i]->GetBlockPick())
		{ // Item Picking

			if (((CCursor*)m_pCursor)->GetItem() == NULL && ((CShop_Block*)m_vecBlock[i])->GetItem() != NULL) // 사기
			{
				CUI* pBuy = ((CUI*)Engine::Find(L"Buy_Base"));
				::Safe_Release(pBuy);
				pBuy->SetRender(true);
				((CBuy_Base*)pBuy)->SetItem(((CShop_Block*)m_vecBlock[i])->GetItem());
				m_iIndex;
				((CBuy_Base*)pBuy)->SetSheet(this);
				m_vecBlock[i]->SetBlockPick(false);
				break;
			}
			else if (((CCursor*)m_pCursor)->GetItem() != NULL && ((CShop_Block*)m_vecBlock[i])->GetItem() == NULL) // 팔기
			{
				CUI* pSell = ((CUI*)Engine::Find(L"Sell_Base"));
				::Safe_Release(pSell);
				pSell->SetRender(true);
				((CSell_Base*)pSell)->SetItem(((CCursor*)m_pCursor)->GetItem());
				((CSell_Base*)pSell)->SetSheet(this);

				if (((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_NORMAL) //Inven_Sheet_1
				{
					CUI* pInven = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
					::Safe_Release(pInven);
					((CInven_Block*)(*((CInven_Sheet*)pInven)->GetVecBlock())[((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(((CCursor*)m_pCursor)->GetItem());
				}
				else if (((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_CASH) //Inven_Sheet_1
				{
					CUI* pInven = ((CUI*)Engine::Find(L"Inven_Sheet_3"));
					::Safe_Release(pInven);
					((CInven_Block*)(*((CInven_Sheet*)pInven)->GetVecBlock())[((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(((CCursor*)m_pCursor)->GetItem());
				}

				((CCursor*)m_pCursor)->SetItem(NULL);
				m_vecBlock[i]->SetBlockPick(false);
				break;
			}
			else if (((CCursor*)m_pCursor)->GetItem() == NULL && ((CShop_Block*)m_vecBlock[i])->GetItem() == NULL)
			{
				m_vecBlock[i]->SetBlockPick(false);
				break;
			}
		}

	}
	
}

void CShop_Sheet::BuyPotion(void)
{
	
}

void CShop_Sheet::RenderToolTip(void)
{
	if (m_bSheetSelect)
	{
		m_pCursor = ((CUI*)Engine::Find(L"Cursor"));

		for (size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if (m_vecBlock[i]->GetButtonCol() && !((CCursor*)m_pCursor)->GetItem() && ((CShop_Block*)m_vecBlock[i])->GetItem())
			{
				switch (((CShop_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType)
				{
				case ITEM_BLUE_WEAPON:
					m_pToolTip->SetId(TOOLTIP_WEAPON_BLUE);
					break;

				case ITEM_RED_WEAPON:
					m_pToolTip->SetId(TOOLTIP_WEAPON_RED);
					break;

				case ITEM_S_BLUE_WEAPON:
					m_pToolTip->SetId(TOOLTIP_S_WEAPON_BLUE);
					break;

				case ITEM_S_RED_WEAPON:
					m_pToolTip->SetId(TOOLTIP_S_WEAPON_RED);
					break;

				case ITEM_HELMET:
					m_pToolTip->SetId(TOOLTIP_HELMET);
					break;
				case ITEM_S_HELMET:
					m_pToolTip->SetId(TOOLTIP_S_HELMET);
					break;

				case ITEM_ARMOR:
					m_pToolTip->SetId(TOOLTIP_ARMOR);
					break;
				case ITEM_S_ARMOR:
					m_pToolTip->SetId(TOOLTIP_S_ARMOR);
					break;

				case ITEM_SHORDER:
					m_pToolTip->SetId(TOOLTIP_SHORDER);
					break;
				case ITEM_S_SHORDER:
					m_pToolTip->SetId(TOOLTIP_S_SHORDER);
					break;

				case ITEM_LEG:
					m_pToolTip->SetId(TOOLTIP_LEG);
					break;
				case ITEM_S_LEG:
					m_pToolTip->SetId(TOOLTIP_S_LEG);
					break;

				case ITEM_GLOVE:
					m_pToolTip->SetId(TOOLTIP_GLOVE);
					break;
				case ITEM_S_GLOVE:
					m_pToolTip->SetId(TOOLTIP_S_GLOVE);
					break;

				case ITEM_BOOTS:
					m_pToolTip->SetId(TOOLTIP_BOOTS);
					break;
				case ITEM_S_BOOTS:
					m_pToolTip->SetId(TOOLTIP_S_BOOTS);
					break;

				case ITEM_AMULET:
					m_pToolTip->SetId(TOOLTIP_AMULET);
					break;

				case ITEM_ARMLET:
					m_pToolTip->SetId(TOOLTIP_ARMLET);
					break;

				case ITEM_BELT:
					m_pToolTip->SetId(TOOLTIP_BELT);
					break;

				case ITEM_EARING:
					m_pToolTip->SetId(TOOLTIP_EARING);
					break;

				case ITEM_RING:
					m_pToolTip->SetId(TOOLTIP_RING);
					break;

				case ITEM_RING2:
					m_pToolTip->SetId(TOOLTIP_RING2);
					break;

				case ITEM_HPPOTION:
					m_pToolTip->SetId(TOOLTIP_HP_POTION);
					break;

				case ITEM_MPPOTION:
					m_pToolTip->SetId(TOOLTIP_MP_POTION);
					break;

				case ITEM_LEVELPOTION:
					m_pToolTip->SetId(TOOLTIP_LEVELUP_ITEM);
					break;
				}

				m_pToolTip->SetRender(true);

				if (*m_vecBlock[i]->GetfY() <= (WINSIZEY >> 1))
				{
					if (*m_vecBlock[i]->GetfX() > (WINSIZEX >> 1))
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 150.f, m_vecBlock[i]->GetInfo().y + 200.f));
					else if (*m_vecBlock[i]->GetfX() <= (WINSIZEX >> 1))
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 150.f, m_vecBlock[i]->GetInfo().y + 200.f));
				}
				else if (*m_vecBlock[i]->GetfY() > (WINSIZEY >> 1))
				{
					if (*m_vecBlock[i]->GetfX() > (WINSIZEX >> 1))
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 150.f, m_vecBlock[i]->GetInfo().y - 200.f));
					else if (*m_vecBlock[i]->GetfX() <= (WINSIZEX >> 1))
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 150.f, m_vecBlock[i]->GetInfo().y - 200.f));
				}

				::Safe_Release(m_pCursor);
				return;
			}
		}

		m_pToolTip->SetRender(false);
		::Safe_Release(m_pCursor);
	}
}

void CShop_Sheet::FadeCheck(void)
{
	if (m_iIndex == 1)
	{
		for (size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if (((CShop_Block*)m_vecBlock[i])->GetItem() != NULL)
				((CShop_Block*)m_vecBlock[i])->GetItem()->SetFade(false);
		}
	}
}

_uint CShop_Sheet::EmptySpaceCheck(SLOT_ID Id)
{
	for (size_t i = 0; i < m_vecBlock.size(); ++i)
	{
		if (((CShop_Block*)m_vecBlock[i])->GetItem() == NULL)
		{
			return i;
		}
	}
	return 99;
}

void CShop_Sheet::AddShop(CUI_Item* pItem, _uint iIndex)
{
	if (iIndex == m_iIndex)
	{
		for (size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if (((CShop_Block*)m_vecBlock[i])->GetItem() == NULL)
			{
				((CShop_Block*)m_vecBlock[i])->SetItem(pItem);
				pItem->Get_Item_Info()->m_iSlotIndex = i;
				break;
			}
		}

	}
}

void CShop_Sheet::AddSell(CUI_Item* pItem, _uint iIndex)
{
	if (iIndex == m_iIndex)
	{
		for (size_t i = 0; i < m_vecSellItem.size(); ++i)
		{
			if (((CShop_Block*)m_vecSellItem[i])->GetItem() == NULL)
			{
				((CShop_Block*)m_vecSellItem[i])->SetItem(pItem);
				break;
			}
		}
	}
}

CShop_Sheet* CShop_Sheet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CShop_Sheet*		pInstance = new CShop_Sheet(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CShop_Sheet* CShop_Sheet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, SHOP_ID eID, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CShop_Sheet*		pInstance = new CShop_Sheet(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetType(eID);
	pInstance->SetIndex(iIndex);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop_Sheet::Free(void)
{
	//::Safe_Release(m_pParentWindow);
	CUI::Free();
}
