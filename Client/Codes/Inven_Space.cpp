#include "stdafx.h"
#include "Inven_Space.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_Item.h"
#include "Cursor.h"
#include "ToolTip.h"
#include "Player.h"
#include "Inven_Sheet.h"

CInven_Space::CInven_Space(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_vParentBasePos(0.f, 0.f)
	, m_pItem(NULL)
	, m_bFindTwinsRed(false)
	, m_bFindTwinsBlue(false)
{
	m_bIsStatic = true;
	m_bAlpha_Increase = false;

}

CInven_Space::~CInven_Space(void)
{

}

HRESULT CInven_Space::Ready_Object(void)
{
	return S_OK;
}

HRESULT CInven_Space::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	return S_OK;
}
void CInven_Space::Update_Object(const _float& fTimeDelta)
{

}

void CInven_Space::Render_Object(void)
{

}


HRESULT CInven_Space::Add_Component(void)
{
	return S_OK;
}
CUI_Item* CInven_Space::GetItem(void)
{
	return m_pItem;
}

void CInven_Space::SetItem(CUI_Item* pItem)
{
	m_pItem = pItem;
}

void CInven_Space::SetItemPos(void)
{
	if(m_pItem != NULL)
	{
		m_pItem->SetWinPos(_vec2(m_fX, m_fY));
		m_pItem->SetMousePick(false);
	}
}



void CInven_Space::PtInRect_Collision(const _float& fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec4 vInfo = m_pParentWindow->GetInfo();

	m_fX = vInfo.x - m_vParentBasePos.x;
	m_fY = vInfo.y - m_vParentBasePos.y;

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
			if (m_bPick)
				return;
			m_bPick = true;

			CUI* pCursour = ((CUI*)Engine::Find(L"Cursor"));

			if(m_pItem == NULL && ((CCursor*)pCursour)->GetItem() != NULL)
				m_bWear = true;
			else if (m_pItem != NULL && ((CCursor*)pCursour)->GetItem() == NULL)
				m_bOut = true;
			else if (m_pItem != NULL && ((CCursor*)pCursour)->GetItem() != NULL)
				m_bSwap = true;

			::Safe_Release(pCursour);
		}
		else
			m_bPick = false;

		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80)
		{
			if (m_bAlpha_Increase)
				return;
			m_bAlpha_Increase = true;

			CUI* pCursour = ((CUI*)Engine::Find(L"Cursor"));

			if (m_pItem != NULL && ((CCursor*)pCursour)->GetItem() == NULL)
			{
				m_pItem->SetImmidy(true);
				m_bOut = true;
			}

			::Safe_Release(pCursour);
		}
		else
			m_bAlpha_Increase = false;
	}
}

void CInven_Space::WearItem()
{
	if(m_bWear)
	{
		m_bWear = false;

		if (m_pItem != NULL)
			return;

		CUI* pCursour = ((CUI*)Engine::Find(L"Cursor"));
		CPlayer* pPlayer = ((CPlayer*)Engine::Find(L"Player"));

		if (((CCursor*)pCursour)->GetItem() != NULL)
		{
			if (((CCursor*)pCursour)->GetItem()->GetFade() == true)
			{
				::Safe_Release(pCursour);
				::Safe_Release(pPlayer);
				return;
			}
		}

		if(((CCursor*)pCursour)->GetItem() != NULL && m_pItem == NULL)
		{	
			if (((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RED_WEAPON
				|| ((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_BLUE_WEAPON
				|| ((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_RED_WEAPON
				|| ((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_BLUE_WEAPON)
			{
				CUI* pSpace = NULL;
				
				if (m_eType == ITEM_RED_WEAPON || m_eType == ITEM_S_RED_WEAPON)
				{
					pSpace = ((CUI*)Engine::Find(L"L_Weapon_Space"));
					m_pItem = ((CCursor*)pCursour)->GetItem();
					m_pItem->SetEqui(true);
					m_pItem->SetRightWeapon(true);
					m_pItem->SetLeftWeapon(false);
					m_pItem->SetImmidy(false);
					((CInven_Space*)pSpace)->SetItem(m_pItem);
					((CCursor*)pCursour)->SetItem(NULL);
				}
				else if (m_eType == ITEM_BLUE_WEAPON || m_eType == ITEM_S_BLUE_WEAPON)
				{
					pSpace = ((CUI*)Engine::Find(L"R_Weapon_Space"));
					m_pItem = ((CCursor*)pCursour)->GetItem();
					m_pItem->SetEqui(true);
					m_pItem->SetRightWeapon(false);
					m_pItem->SetLeftWeapon(true);
					m_pItem->SetImmidy(false);
					((CInven_Space*)pSpace)->SetItem(m_pItem);
					((CCursor*)pCursour)->SetItem(NULL);
				}
				else
					return;

				(pPlayer)->GetPlayerInfo()->m_iStr += m_pItem->Get_Item_Info()->m_iStr;
				(pPlayer)->GetPlayerInfo()->m_iWis += m_pItem->Get_Item_Info()->m_iWis;
				(pPlayer)->GetPlayerInfo()->m_iInt += m_pItem->Get_Item_Info()->m_iInt;
				(pPlayer)->GetPlayerInfo()->m_iCon += m_pItem->Get_Item_Info()->m_iCon;
				(pPlayer)->GetPlayerInfo()->m_iLeftAtt += m_pItem->Get_Item_Info()->m_iLeftAtt;
				(pPlayer)->GetPlayerInfo()->m_iRightAtt += m_pItem->Get_Item_Info()->m_iRightAtt;
				(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense += m_pItem->Get_Item_Info()->m_iPhysicalDefense;
				(pPlayer)->GetPlayerInfo()->m_iMegicalDefense += m_pItem->Get_Item_Info()->m_iMegicalDefense;
				(pPlayer)->GetPlayerInfo()->m_fAttSpeed += m_pItem->Get_Item_Info()->m_fAttSpeed;
				(pPlayer)->GetPlayerInfo()->m_fCastSpeed += m_pItem->Get_Item_Info()->m_fCastSpeed;
				(pPlayer)->GetPlayerInfo()->m_fConcentration += m_pItem->Get_Item_Info()->m_fConcentration;
				(pPlayer)->GetPlayerInfo()->m_fCritical += m_pItem->Get_Item_Info()->m_fCritical;
				(pPlayer)->GetPlayerInfo()->m_fCriticalResist += m_pItem->Get_Item_Info()->m_fCriticalResist;
				(pPlayer)->GetPlayerInfo()->m_fFireAtt += m_pItem->Get_Item_Info()->m_fFireAtt;
				(pPlayer)->GetPlayerInfo()->m_fFireDef += m_pItem->Get_Item_Info()->m_fFireDef;
				(pPlayer)->GetPlayerInfo()->m_fGlancingArmor += m_pItem->Get_Item_Info()->m_fGlancingArmor;
				(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy += m_pItem->Get_Item_Info()->m_fHitAaccuracy;
				(pPlayer)->GetPlayerInfo()->m_fHitResist += m_pItem->Get_Item_Info()->m_fHitResist;
				(pPlayer)->GetPlayerInfo()->m_fLandAtt += m_pItem->Get_Item_Info()->m_fLandAtt;
				(pPlayer)->GetPlayerInfo()->m_fLandDef += m_pItem->Get_Item_Info()->m_fLandDef;
				(pPlayer)->GetPlayerInfo()->m_fLimitDefence += m_pItem->Get_Item_Info()->m_fLimitDefence;
				(pPlayer)->GetPlayerInfo()->m_fMoveSpeed += m_pItem->Get_Item_Info()->m_fMoveSpeed;
				(pPlayer)->GetPlayerInfo()->m_fSkyAtt += m_pItem->Get_Item_Info()->m_fSkyAtt;
				(pPlayer)->GetPlayerInfo()->m_fSkyDef += m_pItem->Get_Item_Info()->m_fSkyDef;
				(pPlayer)->GetPlayerInfo()->m_fWaterAtt += m_pItem->Get_Item_Info()->m_fWaterAtt;
				(pPlayer)->GetPlayerInfo()->m_fWaterDef += m_pItem->Get_Item_Info()->m_fWaterDef;

				::Safe_Release(pCursour);
				::Safe_Release(pSpace);
				::Safe_Release(pPlayer);
				return;
				
			}
			else if (((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RING
				|| ((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RING2)
			{
				if (m_eType == ITEM_RING || m_eType == ITEM_RING2)
				{
					m_pItem = ((CCursor*)pCursour)->GetItem();
					((CCursor*)pCursour)->SetItem(NULL);
					m_pItem->SetEqui(true);
					m_pItem->SetImmidy(false);


					(pPlayer)->GetPlayerInfo()->m_iStr += m_pItem->Get_Item_Info()->m_iStr;
					(pPlayer)->GetPlayerInfo()->m_iWis += m_pItem->Get_Item_Info()->m_iWis;
					(pPlayer)->GetPlayerInfo()->m_iInt += m_pItem->Get_Item_Info()->m_iInt;
					(pPlayer)->GetPlayerInfo()->m_iCon += m_pItem->Get_Item_Info()->m_iCon;
					(pPlayer)->GetPlayerInfo()->m_iLeftAtt += m_pItem->Get_Item_Info()->m_iLeftAtt;
					(pPlayer)->GetPlayerInfo()->m_iRightAtt += m_pItem->Get_Item_Info()->m_iRightAtt;
					(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense += m_pItem->Get_Item_Info()->m_iPhysicalDefense;
					(pPlayer)->GetPlayerInfo()->m_iMegicalDefense += m_pItem->Get_Item_Info()->m_iMegicalDefense;
					(pPlayer)->GetPlayerInfo()->m_fAttSpeed += m_pItem->Get_Item_Info()->m_fAttSpeed;
					(pPlayer)->GetPlayerInfo()->m_fCastSpeed += m_pItem->Get_Item_Info()->m_fCastSpeed;
					(pPlayer)->GetPlayerInfo()->m_fConcentration += m_pItem->Get_Item_Info()->m_fConcentration;
					(pPlayer)->GetPlayerInfo()->m_fCritical += m_pItem->Get_Item_Info()->m_fCritical;
					(pPlayer)->GetPlayerInfo()->m_fCriticalResist += m_pItem->Get_Item_Info()->m_fCriticalResist;
					(pPlayer)->GetPlayerInfo()->m_fFireAtt += m_pItem->Get_Item_Info()->m_fFireAtt;
					(pPlayer)->GetPlayerInfo()->m_fFireDef += m_pItem->Get_Item_Info()->m_fFireDef;
					(pPlayer)->GetPlayerInfo()->m_fGlancingArmor += m_pItem->Get_Item_Info()->m_fGlancingArmor;
					(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy += m_pItem->Get_Item_Info()->m_fHitAaccuracy;
					(pPlayer)->GetPlayerInfo()->m_fHitResist += m_pItem->Get_Item_Info()->m_fHitResist;
					(pPlayer)->GetPlayerInfo()->m_fLandAtt += m_pItem->Get_Item_Info()->m_fLandAtt;
					(pPlayer)->GetPlayerInfo()->m_fLandDef += m_pItem->Get_Item_Info()->m_fLandDef;
					(pPlayer)->GetPlayerInfo()->m_fLimitDefence += m_pItem->Get_Item_Info()->m_fLimitDefence;
					(pPlayer)->GetPlayerInfo()->m_fMoveSpeed += m_pItem->Get_Item_Info()->m_fMoveSpeed;
					(pPlayer)->GetPlayerInfo()->m_fSkyAtt += m_pItem->Get_Item_Info()->m_fSkyAtt;
					(pPlayer)->GetPlayerInfo()->m_fSkyDef += m_pItem->Get_Item_Info()->m_fSkyDef;
					(pPlayer)->GetPlayerInfo()->m_fWaterAtt += m_pItem->Get_Item_Info()->m_fWaterAtt;
					(pPlayer)->GetPlayerInfo()->m_fWaterDef += m_pItem->Get_Item_Info()->m_fWaterDef;

					::Safe_Release(pCursour);
					::Safe_Release(pPlayer);
					return;
				}
			}
			else if(((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == m_eType && m_pItem == NULL) // ÀåÂø
			{
				m_pItem = ((CCursor*)pCursour)->GetItem();
				((CCursor*)pCursour)->SetItem(NULL);
				m_pItem->SetEqui(true);
				m_pItem->SetImmidy(false);


				(pPlayer)->GetPlayerInfo()->m_iStr += m_pItem->Get_Item_Info()->m_iStr;
				(pPlayer)->GetPlayerInfo()->m_iWis += m_pItem->Get_Item_Info()->m_iWis;
				(pPlayer)->GetPlayerInfo()->m_iInt += m_pItem->Get_Item_Info()->m_iInt;
				(pPlayer)->GetPlayerInfo()->m_iCon += m_pItem->Get_Item_Info()->m_iCon;
				(pPlayer)->GetPlayerInfo()->m_iLeftAtt += m_pItem->Get_Item_Info()->m_iLeftAtt;
				(pPlayer)->GetPlayerInfo()->m_iRightAtt += m_pItem->Get_Item_Info()->m_iRightAtt;
				(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense += m_pItem->Get_Item_Info()->m_iPhysicalDefense;
				(pPlayer)->GetPlayerInfo()->m_iMegicalDefense += m_pItem->Get_Item_Info()->m_iMegicalDefense;
				(pPlayer)->GetPlayerInfo()->m_fAttSpeed += m_pItem->Get_Item_Info()->m_fAttSpeed;
				(pPlayer)->GetPlayerInfo()->m_fCastSpeed += m_pItem->Get_Item_Info()->m_fCastSpeed;
				(pPlayer)->GetPlayerInfo()->m_fConcentration += m_pItem->Get_Item_Info()->m_fConcentration;
				(pPlayer)->GetPlayerInfo()->m_fCritical += m_pItem->Get_Item_Info()->m_fCritical;
				(pPlayer)->GetPlayerInfo()->m_fCriticalResist += m_pItem->Get_Item_Info()->m_fCriticalResist;
				(pPlayer)->GetPlayerInfo()->m_fFireAtt += m_pItem->Get_Item_Info()->m_fFireAtt;
				(pPlayer)->GetPlayerInfo()->m_fFireDef += m_pItem->Get_Item_Info()->m_fFireDef;
				(pPlayer)->GetPlayerInfo()->m_fGlancingArmor += m_pItem->Get_Item_Info()->m_fGlancingArmor;
				(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy += m_pItem->Get_Item_Info()->m_fHitAaccuracy;
				(pPlayer)->GetPlayerInfo()->m_fHitResist += m_pItem->Get_Item_Info()->m_fHitResist;
				(pPlayer)->GetPlayerInfo()->m_fLandAtt += m_pItem->Get_Item_Info()->m_fLandAtt;
				(pPlayer)->GetPlayerInfo()->m_fLandDef += m_pItem->Get_Item_Info()->m_fLandDef;
				(pPlayer)->GetPlayerInfo()->m_fLimitDefence += m_pItem->Get_Item_Info()->m_fLimitDefence;
				(pPlayer)->GetPlayerInfo()->m_fMoveSpeed += m_pItem->Get_Item_Info()->m_fMoveSpeed;
				(pPlayer)->GetPlayerInfo()->m_fSkyAtt += m_pItem->Get_Item_Info()->m_fSkyAtt;
				(pPlayer)->GetPlayerInfo()->m_fSkyDef += m_pItem->Get_Item_Info()->m_fSkyDef;
				(pPlayer)->GetPlayerInfo()->m_fWaterAtt += m_pItem->Get_Item_Info()->m_fWaterAtt;
				(pPlayer)->GetPlayerInfo()->m_fWaterDef += m_pItem->Get_Item_Info()->m_fWaterDef;

				::Safe_Release(pCursour);
				::Safe_Release(pPlayer);
				return;
			}
		}

		::Safe_Release(pCursour);
		::Safe_Release(pPlayer);
	}
}


void CInven_Space::OutItem()
{
	if (m_bOut)
	{
		m_bOut = false;
		if (m_pItem == NULL)
			return;

		CUI* pCursour = ((CUI*)Engine::Find(L"Cursor"));
		CPlayer* pPlayer = ((CPlayer*)Engine::Find(L"Player"));

		if (((CCursor*)pCursour)->GetItem() != NULL)
		{
			if (((CCursor*)pCursour)->GetItem()->GetFade() == true)
			{
				::Safe_Release(pCursour);
				::Safe_Release(pPlayer);
				return;
			}
		}

		if (((CCursor*)pCursour)->GetItem() == NULL && m_pItem != NULL)
		{
			CUI* pSpace = NULL;

			if (m_eType == ITEM_RED_WEAPON || m_eType == ITEM_S_RED_WEAPON)
			{
				pSpace = ((CUI*)Engine::Find(L"L_Weapon_Space"));
				((CCursor*)pCursour)->SetItem(m_pItem);
				m_pItem->SetEqui(false);
				m_pItem->SetRightWeapon(false);
				m_pItem->SetLeftWeapon(false);

				(pPlayer)->GetPlayerInfo()->m_iStr -= m_pItem->Get_Item_Info()->m_iStr;
				(pPlayer)->GetPlayerInfo()->m_iWis -= m_pItem->Get_Item_Info()->m_iWis;
				(pPlayer)->GetPlayerInfo()->m_iInt -= m_pItem->Get_Item_Info()->m_iInt;
				(pPlayer)->GetPlayerInfo()->m_iCon -= m_pItem->Get_Item_Info()->m_iCon;
				(pPlayer)->GetPlayerInfo()->m_iLeftAtt -= m_pItem->Get_Item_Info()->m_iLeftAtt;
				(pPlayer)->GetPlayerInfo()->m_iRightAtt -= m_pItem->Get_Item_Info()->m_iRightAtt;
				(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense -= m_pItem->Get_Item_Info()->m_iPhysicalDefense;
				(pPlayer)->GetPlayerInfo()->m_iMegicalDefense -= m_pItem->Get_Item_Info()->m_iMegicalDefense;
				(pPlayer)->GetPlayerInfo()->m_fAttSpeed -= m_pItem->Get_Item_Info()->m_fAttSpeed;
				(pPlayer)->GetPlayerInfo()->m_fCastSpeed -= m_pItem->Get_Item_Info()->m_fCastSpeed;
				(pPlayer)->GetPlayerInfo()->m_fConcentration -= m_pItem->Get_Item_Info()->m_fConcentration;
				(pPlayer)->GetPlayerInfo()->m_fCritical -= m_pItem->Get_Item_Info()->m_fCritical;
				(pPlayer)->GetPlayerInfo()->m_fCriticalResist -= m_pItem->Get_Item_Info()->m_fCriticalResist;
				(pPlayer)->GetPlayerInfo()->m_fFireAtt -= m_pItem->Get_Item_Info()->m_fFireAtt;
				(pPlayer)->GetPlayerInfo()->m_fFireDef -= m_pItem->Get_Item_Info()->m_fFireDef;
				(pPlayer)->GetPlayerInfo()->m_fGlancingArmor -= m_pItem->Get_Item_Info()->m_fGlancingArmor;
				(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy -= m_pItem->Get_Item_Info()->m_fHitAaccuracy;
				(pPlayer)->GetPlayerInfo()->m_fHitResist -= m_pItem->Get_Item_Info()->m_fHitResist;
				(pPlayer)->GetPlayerInfo()->m_fLandAtt -= m_pItem->Get_Item_Info()->m_fLandAtt;
				(pPlayer)->GetPlayerInfo()->m_fLandDef -= m_pItem->Get_Item_Info()->m_fLandDef;
				(pPlayer)->GetPlayerInfo()->m_fLimitDefence -= m_pItem->Get_Item_Info()->m_fLimitDefence;
				(pPlayer)->GetPlayerInfo()->m_fMoveSpeed -= m_pItem->Get_Item_Info()->m_fMoveSpeed;
				(pPlayer)->GetPlayerInfo()->m_fSkyAtt -= m_pItem->Get_Item_Info()->m_fSkyAtt;
				(pPlayer)->GetPlayerInfo()->m_fSkyDef -= m_pItem->Get_Item_Info()->m_fSkyDef;
				(pPlayer)->GetPlayerInfo()->m_fWaterAtt -= m_pItem->Get_Item_Info()->m_fWaterAtt;
				(pPlayer)->GetPlayerInfo()->m_fWaterDef -= m_pItem->Get_Item_Info()->m_fWaterDef;

				m_pItem = NULL;
				((CInven_Space*)pSpace)->SetItem(NULL);

				if (((CCursor*)pCursour)->GetItem()->GetImmidy() == true)
				{
					m_pSheet = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
					((CCursor*)pCursour)->GetItem()->SetImmidy(false);
					((CInven_Sheet*)m_pSheet)->AddInven(((CCursor*)pCursour)->GetItem());
					((CCursor*)pCursour)->SetItem(NULL);
					::Safe_Release(m_pSheet);

				}

				::Safe_Release(pCursour);
				::Safe_Release(pSpace);
				::Safe_Release(pPlayer);


				return;
			}
			else if (m_eType == ITEM_BLUE_WEAPON || m_eType == ITEM_S_BLUE_WEAPON)
			{
				pSpace = ((CUI*)Engine::Find(L"R_Weapon_Space"));
				((CCursor*)pCursour)->SetItem(m_pItem);
				m_pItem->SetEqui(false);
				m_pItem->SetRightWeapon(false);
				m_pItem->SetLeftWeapon(false);

				(pPlayer)->GetPlayerInfo()->m_iStr -= m_pItem->Get_Item_Info()->m_iStr;
				(pPlayer)->GetPlayerInfo()->m_iWis -= m_pItem->Get_Item_Info()->m_iWis;
				(pPlayer)->GetPlayerInfo()->m_iInt -= m_pItem->Get_Item_Info()->m_iInt;
				(pPlayer)->GetPlayerInfo()->m_iCon -= m_pItem->Get_Item_Info()->m_iCon;
				(pPlayer)->GetPlayerInfo()->m_iLeftAtt -= m_pItem->Get_Item_Info()->m_iLeftAtt;
				(pPlayer)->GetPlayerInfo()->m_iRightAtt -= m_pItem->Get_Item_Info()->m_iRightAtt;
				(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense -= m_pItem->Get_Item_Info()->m_iPhysicalDefense;
				(pPlayer)->GetPlayerInfo()->m_iMegicalDefense -= m_pItem->Get_Item_Info()->m_iMegicalDefense;
				(pPlayer)->GetPlayerInfo()->m_fAttSpeed -= m_pItem->Get_Item_Info()->m_fAttSpeed;
				(pPlayer)->GetPlayerInfo()->m_fCastSpeed -= m_pItem->Get_Item_Info()->m_fCastSpeed;
				(pPlayer)->GetPlayerInfo()->m_fConcentration -= m_pItem->Get_Item_Info()->m_fConcentration;
				(pPlayer)->GetPlayerInfo()->m_fCritical -= m_pItem->Get_Item_Info()->m_fCritical;
				(pPlayer)->GetPlayerInfo()->m_fCriticalResist -= m_pItem->Get_Item_Info()->m_fCriticalResist;
				(pPlayer)->GetPlayerInfo()->m_fFireAtt -= m_pItem->Get_Item_Info()->m_fFireAtt;
				(pPlayer)->GetPlayerInfo()->m_fFireDef -= m_pItem->Get_Item_Info()->m_fFireDef;
				(pPlayer)->GetPlayerInfo()->m_fGlancingArmor -= m_pItem->Get_Item_Info()->m_fGlancingArmor;
				(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy -= m_pItem->Get_Item_Info()->m_fHitAaccuracy;
				(pPlayer)->GetPlayerInfo()->m_fHitResist -= m_pItem->Get_Item_Info()->m_fHitResist;
				(pPlayer)->GetPlayerInfo()->m_fLandAtt -= m_pItem->Get_Item_Info()->m_fLandAtt;
				(pPlayer)->GetPlayerInfo()->m_fLandDef -= m_pItem->Get_Item_Info()->m_fLandDef;
				(pPlayer)->GetPlayerInfo()->m_fLimitDefence -= m_pItem->Get_Item_Info()->m_fLimitDefence;
				(pPlayer)->GetPlayerInfo()->m_fMoveSpeed -= m_pItem->Get_Item_Info()->m_fMoveSpeed;
				(pPlayer)->GetPlayerInfo()->m_fSkyAtt -= m_pItem->Get_Item_Info()->m_fSkyAtt;
				(pPlayer)->GetPlayerInfo()->m_fSkyDef -= m_pItem->Get_Item_Info()->m_fSkyDef;
				(pPlayer)->GetPlayerInfo()->m_fWaterAtt -= m_pItem->Get_Item_Info()->m_fWaterAtt;
				(pPlayer)->GetPlayerInfo()->m_fWaterDef -= m_pItem->Get_Item_Info()->m_fWaterDef;

				m_pItem = NULL;
				((CInven_Space*)pSpace)->SetItem(NULL);

				if (((CCursor*)pCursour)->GetItem()->GetImmidy() == true)
				{
					m_pSheet = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
					((CCursor*)pCursour)->GetItem()->SetImmidy(false);
					((CInven_Sheet*)m_pSheet)->AddInven(((CCursor*)pCursour)->GetItem());
					((CCursor*)pCursour)->SetItem(NULL);
					::Safe_Release(m_pSheet);

				}
				::Safe_Release(pCursour);
				::Safe_Release(pSpace);
				::Safe_Release(pPlayer);
				return;
			}
			else
			{
				m_pItem->SetEqui(false);
				((CCursor*)pCursour)->SetItem(m_pItem);

				(pPlayer)->GetPlayerInfo()->m_iStr -= m_pItem->Get_Item_Info()->m_iStr;
				(pPlayer)->GetPlayerInfo()->m_iWis -= m_pItem->Get_Item_Info()->m_iWis;
				(pPlayer)->GetPlayerInfo()->m_iInt -= m_pItem->Get_Item_Info()->m_iInt;
				(pPlayer)->GetPlayerInfo()->m_iCon -= m_pItem->Get_Item_Info()->m_iCon;
				(pPlayer)->GetPlayerInfo()->m_iLeftAtt -= m_pItem->Get_Item_Info()->m_iLeftAtt;
				(pPlayer)->GetPlayerInfo()->m_iRightAtt -= m_pItem->Get_Item_Info()->m_iRightAtt;
				(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense -= m_pItem->Get_Item_Info()->m_iPhysicalDefense;
				(pPlayer)->GetPlayerInfo()->m_iMegicalDefense -= m_pItem->Get_Item_Info()->m_iMegicalDefense;
				(pPlayer)->GetPlayerInfo()->m_fAttSpeed -= m_pItem->Get_Item_Info()->m_fAttSpeed;
				(pPlayer)->GetPlayerInfo()->m_fCastSpeed -= m_pItem->Get_Item_Info()->m_fCastSpeed;
				(pPlayer)->GetPlayerInfo()->m_fConcentration -= m_pItem->Get_Item_Info()->m_fConcentration;
				(pPlayer)->GetPlayerInfo()->m_fCritical -= m_pItem->Get_Item_Info()->m_fCritical;
				(pPlayer)->GetPlayerInfo()->m_fCriticalResist -= m_pItem->Get_Item_Info()->m_fCriticalResist;
				(pPlayer)->GetPlayerInfo()->m_fFireAtt -= m_pItem->Get_Item_Info()->m_fFireAtt;
				(pPlayer)->GetPlayerInfo()->m_fFireDef -= m_pItem->Get_Item_Info()->m_fFireDef;
				(pPlayer)->GetPlayerInfo()->m_fGlancingArmor -= m_pItem->Get_Item_Info()->m_fGlancingArmor;
				(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy -= m_pItem->Get_Item_Info()->m_fHitAaccuracy;
				(pPlayer)->GetPlayerInfo()->m_fHitResist -= m_pItem->Get_Item_Info()->m_fHitResist;
				(pPlayer)->GetPlayerInfo()->m_fLandAtt -= m_pItem->Get_Item_Info()->m_fLandAtt;
				(pPlayer)->GetPlayerInfo()->m_fLandDef -= m_pItem->Get_Item_Info()->m_fLandDef;
				(pPlayer)->GetPlayerInfo()->m_fLimitDefence -= m_pItem->Get_Item_Info()->m_fLimitDefence;
				(pPlayer)->GetPlayerInfo()->m_fMoveSpeed -= m_pItem->Get_Item_Info()->m_fMoveSpeed;
				(pPlayer)->GetPlayerInfo()->m_fSkyAtt -= m_pItem->Get_Item_Info()->m_fSkyAtt;
				(pPlayer)->GetPlayerInfo()->m_fSkyDef -= m_pItem->Get_Item_Info()->m_fSkyDef;
				(pPlayer)->GetPlayerInfo()->m_fWaterAtt -= m_pItem->Get_Item_Info()->m_fWaterAtt;
				(pPlayer)->GetPlayerInfo()->m_fWaterDef -= m_pItem->Get_Item_Info()->m_fWaterDef;

				m_pItem = NULL;

				if (((CCursor*)pCursour)->GetItem()->GetImmidy() == true)
				{
					m_pSheet = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
					((CCursor*)pCursour)->GetItem()->SetImmidy(false);
					((CInven_Sheet*)m_pSheet)->AddInven(((CCursor*)pCursour)->GetItem());
					((CCursor*)pCursour)->SetItem(NULL);
					::Safe_Release(m_pSheet);
				}

				::Safe_Release(pCursour);
				::Safe_Release(pPlayer);

				return;
			}
		}
		::Safe_Release(pCursour);
		::Safe_Release(pPlayer);
	}

}

void CInven_Space::SwapItem()
{
	if (m_bSwap)
	{
		m_bSwap = false;
		if (m_pItem == NULL)
			return;

		CUI* pCursour = ((CUI*)Engine::Find(L"Cursor"));
		CPlayer* pPlayer = ((CPlayer*)Engine::Find(L"Player"));

		if (((CCursor*)pCursour)->GetItem() != NULL)
		{
			if (((CCursor*)pCursour)->GetItem()->GetFade() == true)
			{
				::Safe_Release(pCursour);
				::Safe_Release(pPlayer);
				return;
			}
		}

		if (((CCursor*)pCursour)->GetItem() != NULL && m_pItem != NULL)
		{
			if (((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RED_WEAPON
				|| ((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_BLUE_WEAPON
				|| ((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_RED_WEAPON
				|| ((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_S_BLUE_WEAPON)
			{
				CUI* pSpace = NULL;

				if (m_eType == ITEM_RED_WEAPON || m_eType == ITEM_S_RED_WEAPON)
				{
					pSpace = ((CUI*)Engine::Find(L"L_Weapon_Space"));

					(pPlayer)->GetPlayerInfo()->m_iStr -= m_pItem->Get_Item_Info()->m_iStr;
					(pPlayer)->GetPlayerInfo()->m_iWis -= m_pItem->Get_Item_Info()->m_iWis;
					(pPlayer)->GetPlayerInfo()->m_iInt -= m_pItem->Get_Item_Info()->m_iInt;
					(pPlayer)->GetPlayerInfo()->m_iCon -= m_pItem->Get_Item_Info()->m_iCon;
					(pPlayer)->GetPlayerInfo()->m_iLeftAtt -= m_pItem->Get_Item_Info()->m_iLeftAtt;
					(pPlayer)->GetPlayerInfo()->m_iRightAtt -= m_pItem->Get_Item_Info()->m_iRightAtt;
					(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense -= m_pItem->Get_Item_Info()->m_iPhysicalDefense;
					(pPlayer)->GetPlayerInfo()->m_iMegicalDefense -= m_pItem->Get_Item_Info()->m_iMegicalDefense;
					(pPlayer)->GetPlayerInfo()->m_fAttSpeed -= m_pItem->Get_Item_Info()->m_fAttSpeed;
					(pPlayer)->GetPlayerInfo()->m_fCastSpeed -= m_pItem->Get_Item_Info()->m_fCastSpeed;
					(pPlayer)->GetPlayerInfo()->m_fConcentration -= m_pItem->Get_Item_Info()->m_fConcentration;
					(pPlayer)->GetPlayerInfo()->m_fCritical -= m_pItem->Get_Item_Info()->m_fCritical;
					(pPlayer)->GetPlayerInfo()->m_fCriticalResist -= m_pItem->Get_Item_Info()->m_fCriticalResist;
					(pPlayer)->GetPlayerInfo()->m_fFireAtt -= m_pItem->Get_Item_Info()->m_fFireAtt;
					(pPlayer)->GetPlayerInfo()->m_fFireDef -= m_pItem->Get_Item_Info()->m_fFireDef;
					(pPlayer)->GetPlayerInfo()->m_fGlancingArmor -= m_pItem->Get_Item_Info()->m_fGlancingArmor;
					(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy -= m_pItem->Get_Item_Info()->m_fHitAaccuracy;
					(pPlayer)->GetPlayerInfo()->m_fHitResist -= m_pItem->Get_Item_Info()->m_fHitResist;
					(pPlayer)->GetPlayerInfo()->m_fLandAtt -= m_pItem->Get_Item_Info()->m_fLandAtt;
					(pPlayer)->GetPlayerInfo()->m_fLandDef -= m_pItem->Get_Item_Info()->m_fLandDef;
					(pPlayer)->GetPlayerInfo()->m_fLimitDefence -= m_pItem->Get_Item_Info()->m_fLimitDefence;
					(pPlayer)->GetPlayerInfo()->m_fMoveSpeed -= m_pItem->Get_Item_Info()->m_fMoveSpeed;
					(pPlayer)->GetPlayerInfo()->m_fSkyAtt -= m_pItem->Get_Item_Info()->m_fSkyAtt;
					(pPlayer)->GetPlayerInfo()->m_fSkyDef -= m_pItem->Get_Item_Info()->m_fSkyDef;
					(pPlayer)->GetPlayerInfo()->m_fWaterAtt -= m_pItem->Get_Item_Info()->m_fWaterAtt;
					(pPlayer)->GetPlayerInfo()->m_fWaterDef -= m_pItem->Get_Item_Info()->m_fWaterDef;

					CUI_Item*	pItem = m_pItem;
					m_pItem = ((CCursor*)pCursour)->GetItem();
					((CInven_Space*)pSpace)->SetItem(m_pItem);
					((CCursor*)pCursour)->SetItem(pItem);

					(pPlayer)->GetPlayerInfo()->m_iStr += m_pItem->Get_Item_Info()->m_iStr;
					(pPlayer)->GetPlayerInfo()->m_iWis += m_pItem->Get_Item_Info()->m_iWis;
					(pPlayer)->GetPlayerInfo()->m_iInt += m_pItem->Get_Item_Info()->m_iInt;
					(pPlayer)->GetPlayerInfo()->m_iCon += m_pItem->Get_Item_Info()->m_iCon;
					(pPlayer)->GetPlayerInfo()->m_iLeftAtt += m_pItem->Get_Item_Info()->m_iLeftAtt;
					(pPlayer)->GetPlayerInfo()->m_iRightAtt += m_pItem->Get_Item_Info()->m_iRightAtt;
					(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense += m_pItem->Get_Item_Info()->m_iPhysicalDefense;
					(pPlayer)->GetPlayerInfo()->m_iMegicalDefense += m_pItem->Get_Item_Info()->m_iMegicalDefense;
					(pPlayer)->GetPlayerInfo()->m_fAttSpeed += m_pItem->Get_Item_Info()->m_fAttSpeed;
					(pPlayer)->GetPlayerInfo()->m_fCastSpeed += m_pItem->Get_Item_Info()->m_fCastSpeed;
					(pPlayer)->GetPlayerInfo()->m_fConcentration += m_pItem->Get_Item_Info()->m_fConcentration;
					(pPlayer)->GetPlayerInfo()->m_fCritical += m_pItem->Get_Item_Info()->m_fCritical;
					(pPlayer)->GetPlayerInfo()->m_fCriticalResist += m_pItem->Get_Item_Info()->m_fCriticalResist;
					(pPlayer)->GetPlayerInfo()->m_fFireAtt += m_pItem->Get_Item_Info()->m_fFireAtt;
					(pPlayer)->GetPlayerInfo()->m_fFireDef += m_pItem->Get_Item_Info()->m_fFireDef;
					(pPlayer)->GetPlayerInfo()->m_fGlancingArmor += m_pItem->Get_Item_Info()->m_fGlancingArmor;
					(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy += m_pItem->Get_Item_Info()->m_fHitAaccuracy;
					(pPlayer)->GetPlayerInfo()->m_fHitResist += m_pItem->Get_Item_Info()->m_fHitResist;
					(pPlayer)->GetPlayerInfo()->m_fLandAtt += m_pItem->Get_Item_Info()->m_fLandAtt;
					(pPlayer)->GetPlayerInfo()->m_fLandDef += m_pItem->Get_Item_Info()->m_fLandDef;
					(pPlayer)->GetPlayerInfo()->m_fLimitDefence += m_pItem->Get_Item_Info()->m_fLimitDefence;
					(pPlayer)->GetPlayerInfo()->m_fMoveSpeed += m_pItem->Get_Item_Info()->m_fMoveSpeed;
					(pPlayer)->GetPlayerInfo()->m_fSkyAtt += m_pItem->Get_Item_Info()->m_fSkyAtt;
					(pPlayer)->GetPlayerInfo()->m_fSkyDef += m_pItem->Get_Item_Info()->m_fSkyDef;
					(pPlayer)->GetPlayerInfo()->m_fWaterAtt += m_pItem->Get_Item_Info()->m_fWaterAtt;
					(pPlayer)->GetPlayerInfo()->m_fWaterDef += m_pItem->Get_Item_Info()->m_fWaterDef;

					m_pItem->SetEqui(true);
					m_pItem->SetRightWeapon(true);
					m_pItem->SetLeftWeapon(false);
					((CCursor*)pCursour)->GetItem()->SetEqui(false);
					((CCursor*)pCursour)->GetItem()->SetRightWeapon(false);
					((CCursor*)pCursour)->GetItem()->SetLeftWeapon(false);

					if (((CCursor*)pCursour)->GetItem()->GetImmidy() == true)
					{
						m_pSheet = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
						((CCursor*)pCursour)->GetItem()->SetImmidy(false);
						((CInven_Sheet*)m_pSheet)->AddInven(((CCursor*)pCursour)->GetItem());
						((CCursor*)pCursour)->SetItem(NULL);
						::Safe_Release(m_pSheet);
					}
				}
				else if (m_eType == ITEM_BLUE_WEAPON || m_eType == ITEM_S_BLUE_WEAPON)
				{
					pSpace = ((CUI*)Engine::Find(L"R_Weapon_Space"));

					(pPlayer)->GetPlayerInfo()->m_iStr -= m_pItem->Get_Item_Info()->m_iStr;
					(pPlayer)->GetPlayerInfo()->m_iWis -= m_pItem->Get_Item_Info()->m_iWis;
					(pPlayer)->GetPlayerInfo()->m_iInt -= m_pItem->Get_Item_Info()->m_iInt;
					(pPlayer)->GetPlayerInfo()->m_iCon -= m_pItem->Get_Item_Info()->m_iCon;
					(pPlayer)->GetPlayerInfo()->m_iLeftAtt -= m_pItem->Get_Item_Info()->m_iLeftAtt;
					(pPlayer)->GetPlayerInfo()->m_iRightAtt -= m_pItem->Get_Item_Info()->m_iRightAtt;
					(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense -= m_pItem->Get_Item_Info()->m_iPhysicalDefense;
					(pPlayer)->GetPlayerInfo()->m_iMegicalDefense -= m_pItem->Get_Item_Info()->m_iMegicalDefense;
					(pPlayer)->GetPlayerInfo()->m_fAttSpeed -= m_pItem->Get_Item_Info()->m_fAttSpeed;
					(pPlayer)->GetPlayerInfo()->m_fCastSpeed -= m_pItem->Get_Item_Info()->m_fCastSpeed;
					(pPlayer)->GetPlayerInfo()->m_fConcentration -= m_pItem->Get_Item_Info()->m_fConcentration;
					(pPlayer)->GetPlayerInfo()->m_fCritical -= m_pItem->Get_Item_Info()->m_fCritical;
					(pPlayer)->GetPlayerInfo()->m_fCriticalResist -= m_pItem->Get_Item_Info()->m_fCriticalResist;
					(pPlayer)->GetPlayerInfo()->m_fFireAtt -= m_pItem->Get_Item_Info()->m_fFireAtt;
					(pPlayer)->GetPlayerInfo()->m_fFireDef -= m_pItem->Get_Item_Info()->m_fFireDef;
					(pPlayer)->GetPlayerInfo()->m_fGlancingArmor -= m_pItem->Get_Item_Info()->m_fGlancingArmor;
					(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy -= m_pItem->Get_Item_Info()->m_fHitAaccuracy;
					(pPlayer)->GetPlayerInfo()->m_fHitResist -= m_pItem->Get_Item_Info()->m_fHitResist;
					(pPlayer)->GetPlayerInfo()->m_fLandAtt -= m_pItem->Get_Item_Info()->m_fLandAtt;
					(pPlayer)->GetPlayerInfo()->m_fLandDef -= m_pItem->Get_Item_Info()->m_fLandDef;
					(pPlayer)->GetPlayerInfo()->m_fLimitDefence -= m_pItem->Get_Item_Info()->m_fLimitDefence;
					(pPlayer)->GetPlayerInfo()->m_fMoveSpeed -= m_pItem->Get_Item_Info()->m_fMoveSpeed;
					(pPlayer)->GetPlayerInfo()->m_fSkyAtt -= m_pItem->Get_Item_Info()->m_fSkyAtt;
					(pPlayer)->GetPlayerInfo()->m_fSkyDef -= m_pItem->Get_Item_Info()->m_fSkyDef;
					(pPlayer)->GetPlayerInfo()->m_fWaterAtt -= m_pItem->Get_Item_Info()->m_fWaterAtt;
					(pPlayer)->GetPlayerInfo()->m_fWaterDef -= m_pItem->Get_Item_Info()->m_fWaterDef;

					CUI_Item*	pItem = m_pItem;
					m_pItem = ((CCursor*)pCursour)->GetItem();
					((CInven_Space*)pSpace)->SetItem(m_pItem);
					((CCursor*)pCursour)->SetItem(pItem);

					(pPlayer)->GetPlayerInfo()->m_iStr += m_pItem->Get_Item_Info()->m_iStr;
					(pPlayer)->GetPlayerInfo()->m_iWis += m_pItem->Get_Item_Info()->m_iWis;
					(pPlayer)->GetPlayerInfo()->m_iInt += m_pItem->Get_Item_Info()->m_iInt;
					(pPlayer)->GetPlayerInfo()->m_iCon += m_pItem->Get_Item_Info()->m_iCon;
					(pPlayer)->GetPlayerInfo()->m_iLeftAtt += m_pItem->Get_Item_Info()->m_iLeftAtt;
					(pPlayer)->GetPlayerInfo()->m_iRightAtt += m_pItem->Get_Item_Info()->m_iRightAtt;
					(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense += m_pItem->Get_Item_Info()->m_iPhysicalDefense;
					(pPlayer)->GetPlayerInfo()->m_iMegicalDefense += m_pItem->Get_Item_Info()->m_iMegicalDefense;
					(pPlayer)->GetPlayerInfo()->m_fAttSpeed += m_pItem->Get_Item_Info()->m_fAttSpeed;
					(pPlayer)->GetPlayerInfo()->m_fCastSpeed += m_pItem->Get_Item_Info()->m_fCastSpeed;
					(pPlayer)->GetPlayerInfo()->m_fConcentration += m_pItem->Get_Item_Info()->m_fConcentration;
					(pPlayer)->GetPlayerInfo()->m_fCritical += m_pItem->Get_Item_Info()->m_fCritical;
					(pPlayer)->GetPlayerInfo()->m_fCriticalResist += m_pItem->Get_Item_Info()->m_fCriticalResist;
					(pPlayer)->GetPlayerInfo()->m_fFireAtt += m_pItem->Get_Item_Info()->m_fFireAtt;
					(pPlayer)->GetPlayerInfo()->m_fFireDef += m_pItem->Get_Item_Info()->m_fFireDef;
					(pPlayer)->GetPlayerInfo()->m_fGlancingArmor += m_pItem->Get_Item_Info()->m_fGlancingArmor;
					(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy += m_pItem->Get_Item_Info()->m_fHitAaccuracy;
					(pPlayer)->GetPlayerInfo()->m_fHitResist += m_pItem->Get_Item_Info()->m_fHitResist;
					(pPlayer)->GetPlayerInfo()->m_fLandAtt += m_pItem->Get_Item_Info()->m_fLandAtt;
					(pPlayer)->GetPlayerInfo()->m_fLandDef += m_pItem->Get_Item_Info()->m_fLandDef;
					(pPlayer)->GetPlayerInfo()->m_fLimitDefence += m_pItem->Get_Item_Info()->m_fLimitDefence;
					(pPlayer)->GetPlayerInfo()->m_fMoveSpeed += m_pItem->Get_Item_Info()->m_fMoveSpeed;
					(pPlayer)->GetPlayerInfo()->m_fSkyAtt += m_pItem->Get_Item_Info()->m_fSkyAtt;
					(pPlayer)->GetPlayerInfo()->m_fSkyDef += m_pItem->Get_Item_Info()->m_fSkyDef;
					(pPlayer)->GetPlayerInfo()->m_fWaterAtt += m_pItem->Get_Item_Info()->m_fWaterAtt;
					(pPlayer)->GetPlayerInfo()->m_fWaterDef += m_pItem->Get_Item_Info()->m_fWaterDef;

					m_pItem->SetEqui(true);
					m_pItem->SetRightWeapon(true);
					m_pItem->SetLeftWeapon(false);
					((CCursor*)pCursour)->GetItem()->SetEqui(false);
					((CCursor*)pCursour)->GetItem()->SetRightWeapon(false);
					((CCursor*)pCursour)->GetItem()->SetLeftWeapon(false);

					if (((CCursor*)pCursour)->GetItem()->GetImmidy() == true)
					{
						m_pSheet = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
						((CCursor*)pCursour)->GetItem()->SetImmidy(false);
						((CInven_Sheet*)m_pSheet)->AddInven(((CCursor*)pCursour)->GetItem());
						((CCursor*)pCursour)->SetItem(NULL);
						::Safe_Release(m_pSheet);
					}
				}
				::Safe_Release(pCursour);
				::Safe_Release(pSpace);
				::Safe_Release(pPlayer);
				return;

			}
			else if (((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RING
				|| ((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_RING2)
			{
				if (m_eType == ITEM_RING || m_eType == ITEM_RING2)
				{
					CUI_Item*	pItem = m_pItem;

					(pPlayer)->GetPlayerInfo()->m_iStr -= m_pItem->Get_Item_Info()->m_iStr;
					(pPlayer)->GetPlayerInfo()->m_iWis -= m_pItem->Get_Item_Info()->m_iWis;
					(pPlayer)->GetPlayerInfo()->m_iInt -= m_pItem->Get_Item_Info()->m_iInt;
					(pPlayer)->GetPlayerInfo()->m_iCon -= m_pItem->Get_Item_Info()->m_iCon;
					(pPlayer)->GetPlayerInfo()->m_iLeftAtt -= m_pItem->Get_Item_Info()->m_iLeftAtt;
					(pPlayer)->GetPlayerInfo()->m_iRightAtt -= m_pItem->Get_Item_Info()->m_iRightAtt;
					(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense -= m_pItem->Get_Item_Info()->m_iPhysicalDefense;
					(pPlayer)->GetPlayerInfo()->m_iMegicalDefense -= m_pItem->Get_Item_Info()->m_iMegicalDefense;
					(pPlayer)->GetPlayerInfo()->m_fAttSpeed -= m_pItem->Get_Item_Info()->m_fAttSpeed;
					(pPlayer)->GetPlayerInfo()->m_fCastSpeed -= m_pItem->Get_Item_Info()->m_fCastSpeed;
					(pPlayer)->GetPlayerInfo()->m_fConcentration -= m_pItem->Get_Item_Info()->m_fConcentration;
					(pPlayer)->GetPlayerInfo()->m_fCritical -= m_pItem->Get_Item_Info()->m_fCritical;
					(pPlayer)->GetPlayerInfo()->m_fCriticalResist -= m_pItem->Get_Item_Info()->m_fCriticalResist;
					(pPlayer)->GetPlayerInfo()->m_fFireAtt -= m_pItem->Get_Item_Info()->m_fFireAtt;
					(pPlayer)->GetPlayerInfo()->m_fFireDef -= m_pItem->Get_Item_Info()->m_fFireDef;
					(pPlayer)->GetPlayerInfo()->m_fGlancingArmor -= m_pItem->Get_Item_Info()->m_fGlancingArmor;
					(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy -= m_pItem->Get_Item_Info()->m_fHitAaccuracy;
					(pPlayer)->GetPlayerInfo()->m_fHitResist -= m_pItem->Get_Item_Info()->m_fHitResist;
					(pPlayer)->GetPlayerInfo()->m_fLandAtt -= m_pItem->Get_Item_Info()->m_fLandAtt;
					(pPlayer)->GetPlayerInfo()->m_fLandDef -= m_pItem->Get_Item_Info()->m_fLandDef;
					(pPlayer)->GetPlayerInfo()->m_fLimitDefence -= m_pItem->Get_Item_Info()->m_fLimitDefence;
					(pPlayer)->GetPlayerInfo()->m_fMoveSpeed -= m_pItem->Get_Item_Info()->m_fMoveSpeed;
					(pPlayer)->GetPlayerInfo()->m_fSkyAtt -= m_pItem->Get_Item_Info()->m_fSkyAtt;
					(pPlayer)->GetPlayerInfo()->m_fSkyDef -= m_pItem->Get_Item_Info()->m_fSkyDef;
					(pPlayer)->GetPlayerInfo()->m_fWaterAtt -= m_pItem->Get_Item_Info()->m_fWaterAtt;
					(pPlayer)->GetPlayerInfo()->m_fWaterDef -= m_pItem->Get_Item_Info()->m_fWaterDef;

					m_pItem = ((CCursor*)pCursour)->GetItem();
					((CCursor*)pCursour)->SetItem(pItem);

					(pPlayer)->GetPlayerInfo()->m_iStr += m_pItem->Get_Item_Info()->m_iStr;
					(pPlayer)->GetPlayerInfo()->m_iWis += m_pItem->Get_Item_Info()->m_iWis;
					(pPlayer)->GetPlayerInfo()->m_iInt += m_pItem->Get_Item_Info()->m_iInt;
					(pPlayer)->GetPlayerInfo()->m_iCon += m_pItem->Get_Item_Info()->m_iCon;
					(pPlayer)->GetPlayerInfo()->m_iLeftAtt += m_pItem->Get_Item_Info()->m_iLeftAtt;
					(pPlayer)->GetPlayerInfo()->m_iRightAtt += m_pItem->Get_Item_Info()->m_iRightAtt;
					(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense += m_pItem->Get_Item_Info()->m_iPhysicalDefense;
					(pPlayer)->GetPlayerInfo()->m_iMegicalDefense += m_pItem->Get_Item_Info()->m_iMegicalDefense;
					(pPlayer)->GetPlayerInfo()->m_fAttSpeed += m_pItem->Get_Item_Info()->m_fAttSpeed;
					(pPlayer)->GetPlayerInfo()->m_fCastSpeed += m_pItem->Get_Item_Info()->m_fCastSpeed;
					(pPlayer)->GetPlayerInfo()->m_fConcentration += m_pItem->Get_Item_Info()->m_fConcentration;
					(pPlayer)->GetPlayerInfo()->m_fCritical += m_pItem->Get_Item_Info()->m_fCritical;
					(pPlayer)->GetPlayerInfo()->m_fCriticalResist += m_pItem->Get_Item_Info()->m_fCriticalResist;
					(pPlayer)->GetPlayerInfo()->m_fFireAtt += m_pItem->Get_Item_Info()->m_fFireAtt;
					(pPlayer)->GetPlayerInfo()->m_fFireDef += m_pItem->Get_Item_Info()->m_fFireDef;
					(pPlayer)->GetPlayerInfo()->m_fGlancingArmor += m_pItem->Get_Item_Info()->m_fGlancingArmor;
					(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy += m_pItem->Get_Item_Info()->m_fHitAaccuracy;
					(pPlayer)->GetPlayerInfo()->m_fHitResist += m_pItem->Get_Item_Info()->m_fHitResist;
					(pPlayer)->GetPlayerInfo()->m_fLandAtt += m_pItem->Get_Item_Info()->m_fLandAtt;
					(pPlayer)->GetPlayerInfo()->m_fLandDef += m_pItem->Get_Item_Info()->m_fLandDef;
					(pPlayer)->GetPlayerInfo()->m_fLimitDefence += m_pItem->Get_Item_Info()->m_fLimitDefence;
					(pPlayer)->GetPlayerInfo()->m_fMoveSpeed += m_pItem->Get_Item_Info()->m_fMoveSpeed;
					(pPlayer)->GetPlayerInfo()->m_fSkyAtt += m_pItem->Get_Item_Info()->m_fSkyAtt;
					(pPlayer)->GetPlayerInfo()->m_fSkyDef += m_pItem->Get_Item_Info()->m_fSkyDef;
					(pPlayer)->GetPlayerInfo()->m_fWaterAtt += m_pItem->Get_Item_Info()->m_fWaterAtt;
					(pPlayer)->GetPlayerInfo()->m_fWaterDef += m_pItem->Get_Item_Info()->m_fWaterDef;

					m_pItem->SetEqui(true);
					((CCursor*)pCursour)->GetItem()->SetEqui(false);

					if (((CCursor*)pCursour)->GetItem()->GetImmidy() == true)
					{
						m_pSheet = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
						((CCursor*)pCursour)->GetItem()->SetImmidy(false);
						((CInven_Sheet*)m_pSheet)->AddInven(((CCursor*)pCursour)->GetItem());
						((CCursor*)pCursour)->SetItem(NULL);
						::Safe_Release(m_pSheet);
					}

					::Safe_Release(pCursour);
					::Safe_Release(pPlayer);
					return;
				}
			}
			else if (((CCursor*)pCursour)->GetItem()->Get_Item_Info()->m_eItemType == m_eType) // ÀåÂø
			{
				CUI_Item*	pItem = m_pItem;

				(pPlayer)->GetPlayerInfo()->m_iStr -= m_pItem->Get_Item_Info()->m_iStr;
				(pPlayer)->GetPlayerInfo()->m_iWis -= m_pItem->Get_Item_Info()->m_iWis;
				(pPlayer)->GetPlayerInfo()->m_iInt -= m_pItem->Get_Item_Info()->m_iInt;
				(pPlayer)->GetPlayerInfo()->m_iCon -= m_pItem->Get_Item_Info()->m_iCon;
				(pPlayer)->GetPlayerInfo()->m_iLeftAtt -= m_pItem->Get_Item_Info()->m_iLeftAtt;
				(pPlayer)->GetPlayerInfo()->m_iRightAtt -= m_pItem->Get_Item_Info()->m_iRightAtt;
				(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense -= m_pItem->Get_Item_Info()->m_iPhysicalDefense;
				(pPlayer)->GetPlayerInfo()->m_iMegicalDefense -= m_pItem->Get_Item_Info()->m_iMegicalDefense;
				(pPlayer)->GetPlayerInfo()->m_fAttSpeed -= m_pItem->Get_Item_Info()->m_fAttSpeed;
				(pPlayer)->GetPlayerInfo()->m_fCastSpeed -= m_pItem->Get_Item_Info()->m_fCastSpeed;
				(pPlayer)->GetPlayerInfo()->m_fConcentration -= m_pItem->Get_Item_Info()->m_fConcentration;
				(pPlayer)->GetPlayerInfo()->m_fCritical -= m_pItem->Get_Item_Info()->m_fCritical;
				(pPlayer)->GetPlayerInfo()->m_fCriticalResist -= m_pItem->Get_Item_Info()->m_fCriticalResist;
				(pPlayer)->GetPlayerInfo()->m_fFireAtt -= m_pItem->Get_Item_Info()->m_fFireAtt;
				(pPlayer)->GetPlayerInfo()->m_fFireDef -= m_pItem->Get_Item_Info()->m_fFireDef;
				(pPlayer)->GetPlayerInfo()->m_fGlancingArmor -= m_pItem->Get_Item_Info()->m_fGlancingArmor;
				(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy -= m_pItem->Get_Item_Info()->m_fHitAaccuracy;
				(pPlayer)->GetPlayerInfo()->m_fHitResist -= m_pItem->Get_Item_Info()->m_fHitResist;
				(pPlayer)->GetPlayerInfo()->m_fLandAtt -= m_pItem->Get_Item_Info()->m_fLandAtt;
				(pPlayer)->GetPlayerInfo()->m_fLandDef -= m_pItem->Get_Item_Info()->m_fLandDef;
				(pPlayer)->GetPlayerInfo()->m_fLimitDefence -= m_pItem->Get_Item_Info()->m_fLimitDefence;
				(pPlayer)->GetPlayerInfo()->m_fMoveSpeed -= m_pItem->Get_Item_Info()->m_fMoveSpeed;
				(pPlayer)->GetPlayerInfo()->m_fSkyAtt -= m_pItem->Get_Item_Info()->m_fSkyAtt;
				(pPlayer)->GetPlayerInfo()->m_fSkyDef -= m_pItem->Get_Item_Info()->m_fSkyDef;
				(pPlayer)->GetPlayerInfo()->m_fWaterAtt -= m_pItem->Get_Item_Info()->m_fWaterAtt;
				(pPlayer)->GetPlayerInfo()->m_fWaterDef -= m_pItem->Get_Item_Info()->m_fWaterDef;

				m_pItem = ((CCursor*)pCursour)->GetItem();
				((CCursor*)pCursour)->SetItem(pItem);

				(pPlayer)->GetPlayerInfo()->m_iStr += m_pItem->Get_Item_Info()->m_iStr;
				(pPlayer)->GetPlayerInfo()->m_iWis += m_pItem->Get_Item_Info()->m_iWis;
				(pPlayer)->GetPlayerInfo()->m_iInt += m_pItem->Get_Item_Info()->m_iInt;
				(pPlayer)->GetPlayerInfo()->m_iCon += m_pItem->Get_Item_Info()->m_iCon;
				(pPlayer)->GetPlayerInfo()->m_iLeftAtt += m_pItem->Get_Item_Info()->m_iLeftAtt;
				(pPlayer)->GetPlayerInfo()->m_iRightAtt += m_pItem->Get_Item_Info()->m_iRightAtt;
				(pPlayer)->GetPlayerInfo()->m_iPhysicalDefense += m_pItem->Get_Item_Info()->m_iPhysicalDefense;
				(pPlayer)->GetPlayerInfo()->m_iMegicalDefense += m_pItem->Get_Item_Info()->m_iMegicalDefense;
				(pPlayer)->GetPlayerInfo()->m_fAttSpeed += m_pItem->Get_Item_Info()->m_fAttSpeed;
				(pPlayer)->GetPlayerInfo()->m_fCastSpeed += m_pItem->Get_Item_Info()->m_fCastSpeed;
				(pPlayer)->GetPlayerInfo()->m_fConcentration += m_pItem->Get_Item_Info()->m_fConcentration;
				(pPlayer)->GetPlayerInfo()->m_fCritical += m_pItem->Get_Item_Info()->m_fCritical;
				(pPlayer)->GetPlayerInfo()->m_fCriticalResist += m_pItem->Get_Item_Info()->m_fCriticalResist;
				(pPlayer)->GetPlayerInfo()->m_fFireAtt += m_pItem->Get_Item_Info()->m_fFireAtt;
				(pPlayer)->GetPlayerInfo()->m_fFireDef += m_pItem->Get_Item_Info()->m_fFireDef;
				(pPlayer)->GetPlayerInfo()->m_fGlancingArmor += m_pItem->Get_Item_Info()->m_fGlancingArmor;
				(pPlayer)->GetPlayerInfo()->m_fHitAaccuracy += m_pItem->Get_Item_Info()->m_fHitAaccuracy;
				(pPlayer)->GetPlayerInfo()->m_fHitResist += m_pItem->Get_Item_Info()->m_fHitResist;
				(pPlayer)->GetPlayerInfo()->m_fLandAtt += m_pItem->Get_Item_Info()->m_fLandAtt;
				(pPlayer)->GetPlayerInfo()->m_fLandDef += m_pItem->Get_Item_Info()->m_fLandDef;
				(pPlayer)->GetPlayerInfo()->m_fLimitDefence += m_pItem->Get_Item_Info()->m_fLimitDefence;
				(pPlayer)->GetPlayerInfo()->m_fMoveSpeed += m_pItem->Get_Item_Info()->m_fMoveSpeed;
				(pPlayer)->GetPlayerInfo()->m_fSkyAtt += m_pItem->Get_Item_Info()->m_fSkyAtt;
				(pPlayer)->GetPlayerInfo()->m_fSkyDef += m_pItem->Get_Item_Info()->m_fSkyDef;
				(pPlayer)->GetPlayerInfo()->m_fWaterAtt += m_pItem->Get_Item_Info()->m_fWaterAtt;
				(pPlayer)->GetPlayerInfo()->m_fWaterDef += m_pItem->Get_Item_Info()->m_fWaterDef;

				m_pItem->SetEqui(true);
				((CCursor*)pCursour)->GetItem()->SetEqui(false);

				if (((CCursor*)pCursour)->GetItem()->GetImmidy() == true)
				{
					m_pSheet = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
					((CCursor*)pCursour)->GetItem()->SetImmidy(false);
					((CInven_Sheet*)m_pSheet)->AddInven(((CCursor*)pCursour)->GetItem());
					((CCursor*)pCursour)->SetItem(NULL);
					::Safe_Release(m_pSheet);
				}

				::Safe_Release(pCursour);
				::Safe_Release(pPlayer);
				return;
			}
		}

		::Safe_Release(pCursour);
		::Safe_Release(pPlayer);

	}
}




void CInven_Space::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}


void CInven_Space::Free(void)
{
	CUI::Free();
}
