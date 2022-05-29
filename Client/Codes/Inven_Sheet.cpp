#include "stdafx.h"
#include "Inven_Sheet.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_Item.h"
#include "Inven_Block.h"
#include "Cursor.h"
#include "ToolTip.h"
#include "Compare_ToolTip.h"
#include "Partition_Base.h"
#include "L_Weapon_Space.h"
#include "Boots_Space.h"
#include "Glove_Space.h"
#include "Leg_Space.h"
#include "Shorder_Space.h"
#include "Armor_Space.h"
#include "Helmet_Space.h"
#include "Player.h"
#include "Amulet_Space.h"
#include "Armlet_Space.h"
#include "Belt_Space.h"
#include "Earing_Space.h"
#include "Ring1_Space.h"
#include "Ring2_Space.h"
#include "R_Weapon_Space.h"
#include "Sound.h"

CInven_Sheet::CInven_Sheet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pNCTextureCom(NULL)
,m_pCTextureCom(NULL)
,m_pSTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_pToolTip(NULL)
, m_pCompare_ToolTip(NULL)
{

}

CInven_Sheet::~CInven_Sheet(void)
{

}

HRESULT CInven_Sheet::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = m_pParentWindow->GetInfo();

	switch(m_iIndex)
	{
	case 0:
		m_bSheetSelect = true;
		m_fX = vInfo.x - 110.f;
		m_fY = vInfo.y + 40.f;
		break;
	case 1:
		m_fX = vInfo.x - 10.f;
		m_fY = vInfo.y + 40.f;
		break;
	case 2:
		m_fX = vInfo.x + 90.f;
		m_fY = vInfo.y + 40.f;
		break;
	}
	m_fSizeX = 100.f;
	m_fSizeY = 35.f;

	m_fAlpha = 1.f;
	m_eWinType = WINDOW_INVEN;

	m_bIsStatic = true;

	return S_OK;
}

HRESULT CInven_Sheet::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	CUI::Start_ForScene();

	m_pToolTip = ((CToolTip*)((CUI*)Engine::Find(L"ToolTip")));
	m_pCompare_ToolTip = ((CCompare_ToolTip*)((CUI*)Engine::Find(L"Compare_ToolTip")));
	m_pCursor = ((CUI*)Engine::Find(L"Cursor"));
	m_pArrayItem = ((CUI*)Engine::Find(L"Inven_Array"));

	::Safe_Release(m_pCompare_ToolTip);
	::Safe_Release(m_pToolTip);
	::Safe_Release(m_pCursor);
	::Safe_Release(m_pArrayItem);
	return S_OK;
}

void CInven_Sheet::Update_Object(const _float& fTimeDelta)
{
	m_bRender = m_pParentWindow->GetRender();
	if(m_bRender)
	{		
		if (m_pCursor)
		{
			
			if (((CCursor*)m_pCursor)->GetItem() != NULL)
			{
				if (((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_NORMAL)
				{
					switch (m_iIndex)
					{
					case 0:
						m_bSheetSelect = true;
						break;
					case 1:
						m_bSheetSelect = false;
						break;
					case 2:
						m_bSheetSelect = false;
						break;
					}
				}
			}
		}

		CUI::Update_Object(fTimeDelta);
		m_wSortNum = m_pParentWindow->GetSortNum();
		PtInRect_Collision(fTimeDelta);
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		ItemPick();
		ItemPartition();
		RenderToolTip();
		FadeCheck();

		if (m_pArrayItem->GetFade())
		{
			ArrayItem();
			m_pArrayItem->SetFade(false);
		}
	}
	else
	{
		switch(m_iIndex)
		{
		case 0:
			m_bSheetSelect = true;
			break;
		case 1:
			m_bSheetSelect = false;
			break;
		case 2:
			m_bSheetSelect = false;
			break;
		}

		if(m_pToolTip->GetRender())
			m_pToolTip->SetRender(false);
		if (m_pCompare_ToolTip->GetRender())
			m_pCompare_ToolTip->SetRender(false);

		if(m_pCursor != NULL)
		{
			if(((CCursor*)m_pCursor)->GetItem() != NULL && ((CCursor*)m_pCursor)->GetItem()->GetMain() == false
				&& ((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eSlotType == (SLOT_ID)m_iIndex)
			{
				switch(((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eSlotType)
				{
				case SLOT_NORMAL:
					if(m_iIndex == 0)
						((CInven_Block*)m_vecBlock[((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(((CCursor*)m_pCursor)->GetItem());
					((CCursor*)m_pCursor)->SetItem(NULL);
					break;
				case SLOT_QUEST:
					if(m_iIndex == 1)
						((CInven_Block*)m_vecBlock[((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(((CCursor*)m_pCursor)->GetItem());
					((CCursor*)m_pCursor)->SetItem(NULL);
					break;
				case SLOT_CASH:
					if(m_iIndex == 2)
						((CInven_Block*)m_vecBlock[((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_iSlotIndex])->SetItem(((CCursor*)m_pCursor)->GetItem());
					((CCursor*)m_pCursor)->SetItem(NULL);
					break;
				}
			}
		}
	}
}

void CInven_Sheet::Render_Object(void)
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

	if(m_bSheetSelect)
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

	switch(m_iIndex)
	{
	case 0:
		Engine::Render_Font(L"Font_Main_Button", L"일반", &_vec2(m_fX - 10.f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 1:
		Engine::Render_Font(L"Font_Main_Button", L"퀘스트", &_vec2(m_fX - 15.f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 2:
		Engine::Render_Font(L"Font_Main_Button", L"캐시", &_vec2(m_fX - 10.f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	}
}


HRESULT CInven_Sheet::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_S");
	pComponent = (Engine::CComponent*)m_pSTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Click", pComponent));

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

	// Sound
	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");
	m_pSound->Set_Sound(L"ItemPick", L"OutSwap.wav");
	m_pSound->Set_Sound(L"Wear", L"Wear.wav"); 
	m_pSound->Set_Sound(L"Potion", L"PotionDrink.wav"); 

	return S_OK;
}
void CInven_Sheet::PtInRect_Collision(const _float& fTimeDelta)
{
	_vec4 vInfo = m_pParentWindow->GetInfo();

	switch(m_iIndex)
	{
	case 0:
		m_fX = vInfo.x - 110.f;
		m_fY = vInfo.y + 40.f;
		break;
	case 1:
		m_fX = vInfo.x - 10.f;
		m_fY = vInfo.y + 40.f;
		break;
	case 2:
		m_fX = vInfo.x + 90.f;
		m_fY = vInfo.y + 40.f;
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

	if(m_bButtonCollision)
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			m_pCursor = ((CUI*)Engine::Find(L"Cursor"));
			if(((CCursor*)m_pCursor)->GetItem() != NULL)
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


			if(m_bClick)
				return;
			m_bClick = true;
			m_pSound->MyPlaySound(L"ButtonPick"); 

			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Window")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Window")->end();

			for (; iter != iter_end; ++iter)
			{
				if (((CUI*)(*iter))->GetWinType() == WINDOW_INVEN)
					((CUI*)(*iter))->SetSheetSelect(false);
			}

			m_bSheetSelect = true;
		}
		else
			m_bClick = false;
	}
	

	if(m_bSheetSelect)
		m_fAlpha = 1.f;
	else
		m_fAlpha = 0.5f;

}

void CInven_Sheet::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}

vector<CUI*>* CInven_Sheet::GetVecBlock(void)
{
	return &m_vecBlock;
}

void CInven_Sheet::AddVecBlock(_uint iIndex, CUI* pUI)
{
	if(m_iIndex == iIndex)
	{
		m_vecBlock.push_back(pUI);
	}
}

void CInven_Sheet::ItemPick(void)
{
	if(m_bSheetSelect)
	{
		for(size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if(m_vecBlock[i]->GetBlockPick())
			{ // Item Picking

				CUI*	pPartitionBase = ((CUI*)Engine::Find(L"Partition_Base"));
				if (pPartitionBase->GetRender())
				{
					m_vecBlock[i]->SetBlockPick(false);
					m_vecBlock[i]->SetRBPick(false);
					::Safe_Release(pPartitionBase);
					return;
				}
				::Safe_Release(pPartitionBase);

				m_pCursor = ((CUI*)Engine::Find(L"Cursor"));

				if(((CCursor*)m_pCursor)->GetSkill())
				{
					m_vecBlock[i]->SetBlockPick(false);
					m_vecBlock[i]->SetRBPick(false);
					::Safe_Release(m_pCursor);
					return;
				}

				if(((CCursor*)m_pCursor)->GetItem() == NULL && ((CInven_Block*)m_vecBlock[i])->GetItem() != NULL) // 빼기
				{
					((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
					(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
					((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
					m_vecBlock[i]->SetBlockPick(false);
					m_vecBlock[i]->SetRBPick(false);
					m_pSound->MyPlaySound(L"ItemPick");
					::Safe_Release(m_pCursor);
					break;
				}
				else if(((CCursor*)m_pCursor)->GetItem() != NULL && ((CInven_Block*)m_vecBlock[i])->GetItem() == NULL) // 놓기
				{
					((CInven_Block*)m_vecBlock[i])->SetItem(((CCursor*)m_pCursor)->GetItem());
					(((CInven_Block*)m_vecBlock[i])->GetItem())->SetMousePick(false);
					(((CInven_Block*)m_vecBlock[i])->GetItem())->Get_Item_Info()->m_iSlotIndex = i;
					(((CInven_Block*)m_vecBlock[i])->GetItem())->SetMain(false);
					((CCursor*)m_pCursor)->SetItem(NULL);
					((CCursor*)m_pCursor)->SetDropDelay(true);
					m_vecBlock[i]->SetBlockPick(false);
					m_vecBlock[i]->SetRBPick(false);
					m_pSound->MyPlaySound(L"ItemPick");
					::Safe_Release(m_pCursor);
					break;
				}
				else if(((CCursor*)m_pCursor)->GetItem() != NULL && ((CInven_Block*)m_vecBlock[i])->GetItem() != NULL) // 스왑
				{
					if (((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_CASH && ((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eSlotType == SLOT_CASH)
					{
						if (((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eItemType == ((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType)
						{
							((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount += ((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_iCount;
							((CCursor*)m_pCursor)->GetItem()->SetState(Engine::STATE_DESTROY);
							m_vecBlock[i]->SetBlockPick(false);
							m_vecBlock[i]->SetRBPick(false);
							((CCursor*)m_pCursor)->SetItem(NULL);
							m_pSound->MyPlaySound(L"ItemPick");
							::Safe_Release(m_pCursor);
						}
						else
						{
							CUI_Item*	pTemp = NULL;
							pTemp = ((CCursor*)m_pCursor)->GetItem();
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							((CInven_Block*)m_vecBlock[i])->SetItem(pTemp);
							(((CInven_Block*)m_vecBlock[i])->GetItem())->Get_Item_Info()->m_iSlotIndex = i;
							(((CInven_Block*)m_vecBlock[i])->GetItem())->SetMain(false);
							(((CCursor*)m_pCursor)->GetItem())->Get_Item_Info()->m_iSlotIndex = EmptySpaceCheck((((CCursor*)m_pCursor)->GetItem())->Get_Item_Info()->m_eSlotType);
							m_vecBlock[i]->SetBlockPick(false);
							m_vecBlock[i]->SetRBPick(false);
							m_pSound->MyPlaySound(L"ItemPick");
							::Safe_Release(m_pCursor);
						}
					}
					else
					{
						CUI_Item*	pTemp = NULL;
						pTemp = ((CCursor*)m_pCursor)->GetItem();
						((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
						((CInven_Block*)m_vecBlock[i])->SetItem(pTemp);
						(((CInven_Block*)m_vecBlock[i])->GetItem())->Get_Item_Info()->m_iSlotIndex = i;
						(((CInven_Block*)m_vecBlock[i])->GetItem())->SetMain(false);
						(((CCursor*)m_pCursor)->GetItem())->Get_Item_Info()->m_iSlotIndex = EmptySpaceCheck((((CCursor*)m_pCursor)->GetItem())->Get_Item_Info()->m_eSlotType);
						m_vecBlock[i]->SetBlockPick(false);
						m_vecBlock[i]->SetRBPick(false);
						m_pSound->MyPlaySound(L"ItemPick");
						::Safe_Release(m_pCursor);
					}
					
					break;
				}
				else if(((CCursor*)m_pCursor)->GetItem() == NULL && ((CInven_Block*)m_vecBlock[i])->GetItem() == NULL) // 빈곳
				{
					m_vecBlock[i]->SetBlockPick(false);
					m_pSound->MyPlaySound(L"ItemPick");
					::Safe_Release(m_pCursor);
					break;
				}
				::Safe_Release(m_pCursor);
			}
			else if (m_vecBlock[i]->GetRBPick())	// 우클릭
			{
				if (((CCursor*)m_pCursor)->GetSkill() && ((CCursor*)m_pCursor)->GetItem())
				{
					m_vecBlock[i]->SetBlockPick(false);
					m_vecBlock[i]->SetRBPick(false);
					return;
				}

				if (((CInven_Block*)m_vecBlock[i])->GetItem() != NULL)
				{
					CUI* pUI = NULL;
					CUI* pUI2 = NULL;
					switch (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType)
					{
					case ITEM_RED_WEAPON:
						pUI = ((CUI*)Engine::Find(L"L_Weapon_Space"));
						pUI2 = ((CUI*)Engine::Find(L"R_Weapon_Space"));
						::Safe_Release(pUI);
						::Safe_Release(pUI2);
						if (((CL_Weapon_Space*)pUI)->GetItem() == NULL && ((CR_Weapon_Space*)pUI2)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CL_Weapon_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							m_pSound->MyPlaySound(L"Wear");
						}
						else if (((CL_Weapon_Space*)pUI)->GetItem() != NULL || ((CR_Weapon_Space*)pUI2)->GetItem() != NULL) // 스왑
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							(((CCursor*)m_pCursor)->GetItem())->SetImmidy(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CL_Weapon_Space*)pUI)->SetSwap(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_BLUE_WEAPON:
						pUI = ((CUI*)Engine::Find(L"L_Weapon_Space"));
						pUI2 = ((CUI*)Engine::Find(L"R_Weapon_Space"));
						::Safe_Release(pUI);
						::Safe_Release(pUI2);
						if (((CL_Weapon_Space*)pUI)->GetItem() == NULL && ((CR_Weapon_Space*)pUI2)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CL_Weapon_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						else if (((CL_Weapon_Space*)pUI)->GetItem() != NULL || ((CR_Weapon_Space*)pUI2)->GetItem() != NULL) // 스왑
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							(((CCursor*)m_pCursor)->GetItem())->SetImmidy(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CL_Weapon_Space*)pUI)->SetSwap(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_HELMET:
						pUI = ((CUI*)Engine::Find(L"Helmet_Space"));
						::Safe_Release(pUI);
						if (((CHelmet_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CHelmet_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_SHORDER:
						pUI = ((CUI*)Engine::Find(L"Shorder_Space"));
						::Safe_Release(pUI);
						if (((CShorder_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CShorder_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_ARMOR:
						pUI = ((CUI*)Engine::Find(L"Armor_Space"));
						::Safe_Release(pUI);
						if (((CArmor_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CArmor_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_LEG:
						pUI = ((CUI*)Engine::Find(L"Leg_Space"));
						::Safe_Release(pUI);
						if (((CLeg_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CLeg_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_GLOVE:
						pUI = ((CUI*)Engine::Find(L"Glove_Space"));
						::Safe_Release(pUI);
						if (((CGlove_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CGlove_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_BOOTS:
						pUI = ((CUI*)Engine::Find(L"Boots_Space"));
						::Safe_Release(pUI);
						if (((CBoots_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CBoots_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_AMULET:
						pUI = ((CUI*)Engine::Find(L"Amulet_Space"));
						::Safe_Release(pUI);
						if (((CAmulet_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CAmulet_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_ARMLET:
						pUI = ((CUI*)Engine::Find(L"Armlet_Space"));
						::Safe_Release(pUI);
						if (((CArmlet_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CArmlet_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_EARING:
						pUI = ((CUI*)Engine::Find(L"Earing_Space"));
						::Safe_Release(pUI);
						if (((CEaring_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CEaring_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_BELT:
						pUI = ((CUI*)Engine::Find(L"Belt_Space"));
						::Safe_Release(pUI);
						if (((CBelt_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CBelt_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						break;
					case ITEM_RING:
						pUI = ((CUI*)Engine::Find(L"Ring1_Space"));
						pUI2 = ((CUI*)Engine::Find(L"Ring2_Space"));
						::Safe_Release(pUI);
						::Safe_Release(pUI2);
						if (((CRing1_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CRing1_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						else
						{
							if (((CRing2_Space*)pUI2)->GetItem() == NULL) // 아이템 바로장착
							{
								((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
								(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
								((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
								((CRing2_Space*)pUI2)->SetWear(true);
								m_vecBlock[i]->SetRBPick(false);
								m_vecBlock[i]->SetBlockPick(false);
								//m_pSound->MyPlaySound(L"Wear");
							}
						}
						break;

					case ITEM_RING2:
						pUI = ((CUI*)Engine::Find(L"Ring1_Space"));
						pUI2 = ((CUI*)Engine::Find(L"Ring2_Space"));
						::Safe_Release(pUI);
						::Safe_Release(pUI2);
						if (((CRing1_Space*)pUI)->GetItem() == NULL) // 아이템 바로장착
						{
							((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
							(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
							((CRing1_Space*)pUI)->SetWear(true);
							m_vecBlock[i]->SetRBPick(false);
							m_vecBlock[i]->SetBlockPick(false);
							//m_pSound->MyPlaySound(L"Wear");
						}
						else
						{
							if (((CRing2_Space*)pUI2)->GetItem() == NULL) // 아이템 바로장착
							{
								((CCursor*)m_pCursor)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
								(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
								((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
								((CRing2_Space*)pUI2)->SetWear(true);
								m_vecBlock[i]->SetRBPick(false);
								m_vecBlock[i]->SetBlockPick(false);
								//m_pSound->MyPlaySound(L"Wear"); 
							}
						}
						break;
					case ITEM_HPPOTION:
						if (m_pPlayer->GetPlayerInfo()->m_iHp < m_pPlayer->GetPlayerInfo()->m_iMaxHp)
						{
							m_pPlayer->GetPlayerInfo()->m_iHp += ((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iHp;

							if (m_pPlayer->GetPlayerInfo()->m_iHp > m_pPlayer->GetPlayerInfo()->m_iMaxHp)
								m_pPlayer->GetPlayerInfo()->m_iHp = m_pPlayer->GetPlayerInfo()->m_iMaxHp;

							--((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount;
							
							if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount <= 0)
								((CInven_Block*)m_vecBlock[i])->SetItem(NULL);

							m_vecBlock[i]->SetBlockPick(false);
							m_vecBlock[i]->SetRBPick(false);
							m_pSound->MyPlaySound(L"Potion");
						}
						break;
					case ITEM_MPPOTION:
						if (m_pPlayer->GetPlayerInfo()->m_iMp < m_pPlayer->GetPlayerInfo()->m_iMaxMp)
						{
							m_pPlayer->GetPlayerInfo()->m_iMp += ((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iMp;

							if (m_pPlayer->GetPlayerInfo()->m_iMp > m_pPlayer->GetPlayerInfo()->m_iMaxMp)
								m_pPlayer->GetPlayerInfo()->m_iMp = m_pPlayer->GetPlayerInfo()->m_iMaxMp;

							--((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount;

							if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount <= 0)
								((CInven_Block*)m_vecBlock[i])->SetItem(NULL);

							m_vecBlock[i]->SetBlockPick(false);
							m_vecBlock[i]->SetRBPick(false);
							m_pSound->MyPlaySound(L"Potion");
						}
						break;
					case ITEM_LEVELPOTION:
						m_pPlayer->GetPlayerInfo()->m_fExp += (_float(m_pPlayer->GetPlayerInfo()->m_iLevel) * 200.f);

						--((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount;

						if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount <= 0)
							((CInven_Block*)m_vecBlock[i])->SetItem(NULL);

						m_vecBlock[i]->SetBlockPick(false);
						m_vecBlock[i]->SetRBPick(false);
						m_pSound->MyPlaySound(L"Potion");
	
						break;

					default:
						m_vecBlock[i]->SetRBPick(false);
						m_vecBlock[i]->SetBlockPick(false);
						break;
					}
					m_vecBlock[i]->SetRBPick(false);
					m_vecBlock[i]->SetBlockPick(false);
				}
			}
		}
	}
}

void CInven_Sheet::ItemPartition(void)
{
	if (m_bSheetSelect)
	{
		for (size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if (m_vecBlock[i]->GetPartition())
			{ // Item Picking

				if (((CInven_Block*)m_vecBlock[i])->GetItem() == NULL)
					continue;

				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eSlotType != SLOT_CASH
					|| ((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount < 2)
					continue;

				m_pCursor = ((CUI*)Engine::Find(L"Cursor"));

				if (((CCursor*)m_pCursor)->GetItem() || ((CCursor*)m_pCursor)->GetSkill())
				{
					::Safe_Release(m_pCursor);
					return;
				}

				// 분할창 띄우기
				CUI*	pPartitionBase = ((CUI*)Engine::Find(L"Partition_Base"));
				m_pSound->MyPlaySound(L"ItemPick");
				
				pPartitionBase->SetRender(true);
				pPartitionBase->SetPartitionMinCnt(1);
				pPartitionBase->SetPartitionMaxCnt(((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount);
				((CPartition_Base*)pPartitionBase)->SetItem(((CInven_Block*)m_vecBlock[i])->GetItem());
				m_pToolTip->SetRender(false);

				m_vecBlock[i]->SetPartition(false);
				::Safe_Release(m_pCursor);
				::Safe_Release(pPartitionBase);
				break;

			}
		}
	}
}

void CInven_Sheet::RenderToolTip(void)
{
	if(m_bSheetSelect)
	{
		m_pCursor = ((CUI*)Engine::Find(L"Cursor"));

		for(size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if(m_vecBlock[i]->GetButtonCol() && !((CCursor*)m_pCursor)->GetItem() && ((CInven_Block*)m_vecBlock[i])->GetItem())
			{

				CUI* pUI = NULL;
				switch(((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType)
				{
				case ITEM_BLUE_WEAPON:
					pUI = ((CUI*)Engine::Find(L"L_Weapon_Space"));
					::Safe_Release(pUI);
					if (((CL_Weapon_Space*)pUI)->GetItem() != NULL)
					{
						if(((CL_Weapon_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RED_WEAPON)
							m_pCompare_ToolTip->SetId(TOOLTIP_WEAPON_RED);
						else if (((CL_Weapon_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_BLUE_WEAPON)
							m_pCompare_ToolTip->SetId(TOOLTIP_WEAPON_BLUE);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_WEAPON_BLUE);
					break;

				case ITEM_RED_WEAPON:
					pUI = ((CUI*)Engine::Find(L"L_Weapon_Space"));
					::Safe_Release(pUI);
					if (((CL_Weapon_Space*)pUI)->GetItem() != NULL)
					{
						if (((CL_Weapon_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RED_WEAPON)
							m_pCompare_ToolTip->SetId(TOOLTIP_WEAPON_RED);
						else if (((CL_Weapon_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_BLUE_WEAPON)
							m_pCompare_ToolTip->SetId(TOOLTIP_WEAPON_BLUE);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_WEAPON_RED);
					break;
				case ITEM_S_BLUE_WEAPON:
					pUI = ((CUI*)Engine::Find(L"L_Weapon_Space"));
					::Safe_Release(pUI);
					if (((CL_Weapon_Space*)pUI)->GetItem() != NULL)
					{
						if (((CL_Weapon_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_RED_WEAPON)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_WEAPON_RED);
						else if (((CL_Weapon_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_BLUE_WEAPON)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_WEAPON_BLUE);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_S_WEAPON_BLUE);
					break;

				case ITEM_S_RED_WEAPON:
					pUI = ((CUI*)Engine::Find(L"L_Weapon_Space"));
					::Safe_Release(pUI);
					if (((CL_Weapon_Space*)pUI)->GetItem() != NULL)
					{
						if (((CL_Weapon_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_RED_WEAPON)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_WEAPON_RED);
						else if (((CL_Weapon_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_BLUE_WEAPON)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_WEAPON_BLUE);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_S_WEAPON_RED);
					break;

				case ITEM_HELMET:
					pUI = ((CUI*)Engine::Find(L"Helmet_Space"));
					::Safe_Release(pUI);
					if (((CHelmet_Space*)pUI)->GetItem() != NULL)
					{
						if (((CHelmet_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_HELMET)
							m_pCompare_ToolTip->SetId(TOOLTIP_HELMET);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_HELMET);
					break;

				case ITEM_S_HELMET:
					pUI = ((CUI*)Engine::Find(L"Helmet_Space"));
					::Safe_Release(pUI);
					if (((CHelmet_Space*)pUI)->GetItem() != NULL)
					{
						if (((CHelmet_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_HELMET)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_HELMET);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_S_HELMET);
					break;

				case ITEM_ARMOR:
					pUI = ((CUI*)Engine::Find(L"Armor_Space"));
					::Safe_Release(pUI);
					if (((CArmor_Space*)pUI)->GetItem() != NULL)
					{
						if (((CArmor_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_ARMOR)
							m_pCompare_ToolTip->SetId(TOOLTIP_ARMOR);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_ARMOR);
					break;
				case ITEM_S_ARMOR:
					pUI = ((CUI*)Engine::Find(L"Armor_Space"));
					::Safe_Release(pUI);
					if (((CArmor_Space*)pUI)->GetItem() != NULL)
					{
						if (((CArmor_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_ARMOR)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_ARMOR);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_S_ARMOR);
					break;

				case ITEM_SHORDER:
					pUI = ((CUI*)Engine::Find(L"Shorder_Space"));
					::Safe_Release(pUI);
					if (((CShorder_Space*)pUI)->GetItem() != NULL)
					{
						if (((CShorder_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_SHORDER)
							m_pCompare_ToolTip->SetId(TOOLTIP_SHORDER);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_SHORDER);
					break;

				case ITEM_S_SHORDER:
					pUI = ((CUI*)Engine::Find(L"Shorder_Space"));
					::Safe_Release(pUI);
					if (((CShorder_Space*)pUI)->GetItem() != NULL)
					{
						if (((CShorder_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_SHORDER)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_SHORDER);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_S_SHORDER);
					break;

				case ITEM_LEG:
					pUI = ((CUI*)Engine::Find(L"Leg_Space"));
					::Safe_Release(pUI);
					if (((CLeg_Space*)pUI)->GetItem() != NULL)
					{
						if (((CLeg_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_LEG)
							m_pCompare_ToolTip->SetId(TOOLTIP_LEG);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_LEG);
					break;

				case ITEM_S_LEG:
					pUI = ((CUI*)Engine::Find(L"Leg_Space"));
					::Safe_Release(pUI);
					if (((CLeg_Space*)pUI)->GetItem() != NULL)
					{
						if (((CLeg_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_LEG)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_LEG);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_S_LEG);
					break;

				case ITEM_GLOVE:
					pUI = ((CUI*)Engine::Find(L"Glove_Space"));
					::Safe_Release(pUI);
					if (((CGlove_Space*)pUI)->GetItem() != NULL)
					{
						if (((CGlove_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_GLOVE)
							m_pCompare_ToolTip->SetId(TOOLTIP_GLOVE);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_GLOVE);
					break;
				case ITEM_S_GLOVE:
					pUI = ((CUI*)Engine::Find(L"Glove_Space"));
					::Safe_Release(pUI);
					if (((CGlove_Space*)pUI)->GetItem() != NULL)
					{
						if (((CGlove_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_GLOVE)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_GLOVE);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_S_GLOVE);
					break;
				case ITEM_BOOTS:
					pUI = ((CUI*)Engine::Find(L"Boots_Space"));
					::Safe_Release(pUI);
					if (((CBoots_Space*)pUI)->GetItem() != NULL)
					{
						if (((CBoots_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_BOOTS)
							m_pCompare_ToolTip->SetId(TOOLTIP_BOOTS);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_BOOTS);
					break;
				case ITEM_S_BOOTS:
					pUI = ((CUI*)Engine::Find(L"Boots_Space"));
					::Safe_Release(pUI);
					if (((CBoots_Space*)pUI)->GetItem() != NULL)
					{
						if (((CBoots_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_BOOTS)
							m_pCompare_ToolTip->SetId(TOOLTIP_S_BOOTS);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_S_BOOTS);
					break;

				case ITEM_AMULET:
					pUI = ((CUI*)Engine::Find(L"Amulet_Space"));
					::Safe_Release(pUI);
					if (((CAmulet_Space*)pUI)->GetItem() != NULL)
					{
						if (((CAmulet_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_AMULET)
							m_pCompare_ToolTip->SetId(TOOLTIP_AMULET);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_AMULET);
					break;

				case ITEM_ARMLET:
					pUI = ((CUI*)Engine::Find(L"Armlet_Space"));
					::Safe_Release(pUI);
					if (((CArmlet_Space*)pUI)->GetItem() != NULL)
					{
						if (((CArmlet_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_ARMLET)
							m_pCompare_ToolTip->SetId(TOOLTIP_ARMLET);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_ARMLET);
					break;

				case ITEM_BELT:
					pUI = ((CUI*)Engine::Find(L"Belt_Space"));
					::Safe_Release(pUI);
					if (((CBelt_Space*)pUI)->GetItem() != NULL)
					{
						if (((CBelt_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_BELT)
							m_pCompare_ToolTip->SetId(TOOLTIP_BELT);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_BELT);
					break;

				case ITEM_EARING:
					pUI = ((CUI*)Engine::Find(L"Earing_Space"));
					::Safe_Release(pUI);
					if (((CEaring_Space*)pUI)->GetItem() != NULL)
					{
						if (((CEaring_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_EARING)
							m_pCompare_ToolTip->SetId(TOOLTIP_BELT);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_EARING);
					break;

				case ITEM_RING:
					pUI = ((CUI*)Engine::Find(L"Ring1_Space"));
					::Safe_Release(pUI);
					if (((CRing1_Space*)pUI)->GetItem() != NULL)
					{
						if (((CRing1_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RING)
							m_pCompare_ToolTip->SetId(TOOLTIP_RING);
						if (((CRing1_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RING2)
							m_pCompare_ToolTip->SetId(TOOLTIP_RING2);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
					m_pToolTip->SetId(TOOLTIP_RING);
					break;

				case ITEM_RING2:
					pUI = ((CUI*)Engine::Find(L"Ring1_Space"));
					::Safe_Release(pUI);
					if (((CRing1_Space*)pUI)->GetItem() != NULL)
					{
						if (((CRing1_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RING)
							m_pCompare_ToolTip->SetId(TOOLTIP_RING);
						if (((CRing1_Space*)pUI)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RING2)
							m_pCompare_ToolTip->SetId(TOOLTIP_RING2);
						m_pCompare_ToolTip->SetRender(true);
					}
					else
						m_pCompare_ToolTip->SetRender(false);
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

				if(m_pParentWindow->GetFoward())
					m_pToolTip->SetRender(true);

				if(*m_vecBlock[i]->GetfY() <= (WINSIZEY >> 1))
				{
					if(*m_vecBlock[i]->GetfX() > (WINSIZEX >> 1))
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 150.f, m_vecBlock[i]->GetInfo().y + 200.f));
					else if(*m_vecBlock[i]->GetfX() <= (WINSIZEX >> 1))
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 150.f, m_vecBlock[i]->GetInfo().y + 200.f));
				}
				else if(*m_vecBlock[i]->GetfY() > (WINSIZEY >> 1))
				{
					if(*m_vecBlock[i]->GetfX() > (WINSIZEX >> 1))
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 150.f, m_vecBlock[i]->GetInfo().y - 200.f));
					else if(*m_vecBlock[i]->GetfX() <= (WINSIZEX >> 1))
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 150.f, m_vecBlock[i]->GetInfo().y - 200.f));
				}

				if (m_pCompare_ToolTip->GetRender())
				{
					if (*m_vecBlock[i]->GetfY() <= (WINSIZEY >> 1))
					{
						if (*m_vecBlock[i]->GetfX() > (WINSIZEX >> 1))
							m_pCompare_ToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 400.f, m_vecBlock[i]->GetInfo().y + 200.f));
						else if (*m_vecBlock[i]->GetfX() <= (WINSIZEX >> 1))
							m_pCompare_ToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 400.f, m_vecBlock[i]->GetInfo().y + 200.f));
					}
					else if (*m_vecBlock[i]->GetfY() > (WINSIZEY >> 1))
					{
						if (*m_vecBlock[i]->GetfX() > (WINSIZEX >> 1))
							m_pCompare_ToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 400.f, m_vecBlock[i]->GetInfo().y - 200.f));
						else if (*m_vecBlock[i]->GetfX() <= (WINSIZEX >> 1))
							m_pCompare_ToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 400.f, m_vecBlock[i]->GetInfo().y - 200.f));
					}
				}

				
				::Safe_Release(m_pCursor);
				return;
			}
		}

		m_pCompare_ToolTip->SetRender(false);
		m_pToolTip->SetRender(false);
		::Safe_Release(m_pCursor);
	}
}

void CInven_Sheet::FadeCheck(void)
{

	if (m_pPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		for (size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if (((CInven_Block*)m_vecBlock[i])->GetItem() != NULL)
			{
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_BLUE_WEAPON)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_RED_WEAPON)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_ARMOR)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_HELMET)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_BOOTS)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_GLOVE)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_SHORDER)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_LEG)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
			}
		}

	}
	else
	{
		for (size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if (((CInven_Block*)m_vecBlock[i])->GetItem() != NULL)
			{
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_BLUE_WEAPON)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RED_WEAPON)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_ARMOR)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_HELMET)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_BOOTS)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_GLOVE)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_SHORDER)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
				if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == ITEM_LEG)
					((CInven_Block*)m_vecBlock[i])->GetItem()->SetFade(true);
			}
		}

	}
}

_uint CInven_Sheet::EmptySpaceCheck(SLOT_ID Id)
{
	switch(Id)
	{
	case SLOT_NORMAL:
		if(m_iIndex == 0)
		{
			for(size_t i = 0; i < m_vecBlock.size(); ++i)
			{
				if(((CInven_Block*)m_vecBlock[i])->GetItem() == NULL)
				{
					return i;
				}
			}
		}
		break;
	case SLOT_QUEST:
		if(m_iIndex == 1)
		{
			for(size_t i = 0; i < m_vecBlock.size(); ++i)
			{
				if(((CInven_Block*)m_vecBlock[i])->GetItem() == NULL)
				{
					return i;
				}
			}
		}
		break;
	case SLOT_CASH:
		if(m_iIndex == 2)
		{
			for(size_t i = 0; i < m_vecBlock.size(); ++i)
			{
				if(((CInven_Block*)m_vecBlock[i])->GetItem() == NULL)
				{
					return i;
				}
			}
		}
		break;
	}

	return 99;
}	

void CInven_Sheet::ArrayItem(void)
{
	if (m_bSheetSelect)
	{
		for (size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if (((CInven_Block*)m_vecBlock[i])->GetItem())
			{
				m_ArrayMap.insert(ARRAYMAP::value_type(((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType, ((CInven_Block*)m_vecBlock[i])->GetItem()));
				((CInven_Block*)m_vecBlock[i])->SetItem(NULL);
			}
		}

		ARRAYMAP::iterator iter = m_ArrayMap.begin();
		ARRAYMAP::iterator iter_end = m_ArrayMap.end();

		for (; iter != iter_end; ++iter)
			AddInven(iter->second);

		m_ArrayMap.clear();
	}
}

void CInven_Sheet::AddInven(CUI_Item* pItem)
{
	switch(pItem->Get_Item_Info()->m_eSlotType)
	{
	case SLOT_NORMAL:
		if(m_iIndex == 0)
		{
			for(size_t i = 0; i < m_vecBlock.size(); ++i)
			{
				if(((CInven_Block*)m_vecBlock[i])->GetItem() == NULL)
				{
					((CInven_Block*)m_vecBlock[i])->SetItem(pItem);
					pItem->Get_Item_Info()->m_iSlotIndex = i;
					break;
				}
			}
		}
		break;
	case SLOT_QUEST:
		if(m_iIndex == 1)
		{

		}
		break;
	case SLOT_CASH:
		if(m_iIndex == 2)
		{
			for (size_t i = 0; i < m_vecBlock.size(); ++i)
			{
				if (((CInven_Block*)m_vecBlock[i])->GetItem() != NULL)
				{
					if (((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_eItemType == pItem->Get_Item_Info()->m_eItemType
						&& ((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount >= 1)
					{
						((CInven_Block*)m_vecBlock[i])->GetItem()->Get_Item_Info()->m_iCount += pItem->Get_Item_Info()->m_iCount;
						pItem->SetState(Engine::STATE_DESTROY);
						return;
					}
				}
			}
			for(size_t i = 0; i < m_vecBlock.size(); ++i)
			{
				if(((CInven_Block*)m_vecBlock[i])->GetItem() == NULL)
				{
					((CInven_Block*)m_vecBlock[i])->SetItem(pItem);
					pItem->Get_Item_Info()->m_iSlotIndex = i;
					break;
				}
			}
		}
		break;
	}
}

void CInven_Sheet::AddResultInven(CUI_Item* pItem)
{
	for (size_t i = 0; i < m_vecBlock.size(); ++i)
	{
		if (((CInven_Block*)m_vecBlock[i])->GetItem() == NULL)
		{
			((CInven_Block*)m_vecBlock[i])->SetItem(pItem);
			pItem->Get_Item_Info()->m_iSlotIndex = i;
			break;
		}
	}
}

CInven_Sheet* CInven_Sheet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CInven_Sheet*		pInstance = new CInven_Sheet(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CInven_Sheet* CInven_Sheet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CInven_Sheet*		pInstance = new CInven_Sheet(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetIndex(iIndex);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CInven_Sheet::Free(void)
{
	//::Safe_Release(m_pParentWindow);
	CUI::Free();
}
