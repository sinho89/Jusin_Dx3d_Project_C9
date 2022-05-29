#include "stdafx.h"
#include "UI_Item.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CUI_Item::CUI_Item(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_pItemInfo(NULL)
,m_bEqu(false)
, m_bResult(false)
, m_bImmidy(false)
{
	m_bIsStatic = true;

}

CUI_Item::~CUI_Item(void)
{

}

HRESULT CUI_Item::Start_ForScene(void)
{
	return S_OK;
}

void CUI_Item::Update_Object(const _float& fTimeDelta)
{
}

void CUI_Item::Render_Object(void)
{
	
}

ITEM_INFO* CUI_Item::Get_Item_Info(void)
{
	return m_pItemInfo;
}

_bool CUI_Item::GetEqui(void)
{
	return m_bEqu;
}

void CUI_Item::SetEqui(_bool bEqui)
{
	m_bEqu = bEqui;
}

void CUI_Item::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}

void CUI_Item::Free(void)
{
	delete m_pItemInfo;
	m_pItemInfo = NULL;

	CUI::Free();
}
