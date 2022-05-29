#include "stdafx.h"
#include "Sprite_UI.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CSprite_UI::CSprite_UI(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{

}

CSprite_UI::~CSprite_UI(void)
{

}

HRESULT CSprite_UI::Start_ForScene(void)
{
	return S_OK;
}

void CSprite_UI::Update_Object(const _float& fTimeDelta)
{
}

void CSprite_UI::Render_Object(void)
{

}


void CSprite_UI::Free(void)
{

	CUI::Free();
}
