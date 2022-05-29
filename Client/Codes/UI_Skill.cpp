#include "stdafx.h"
#include "UI_Skill.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CUI_Skill::CUI_Skill(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_pSkillInfo(NULL)
, m_pCoolTimeEff(NULL)
, m_fFadeTime(0.f)
{
	m_bIsStatic = true;

}

CUI_Skill::~CUI_Skill(void)
{

}

HRESULT CUI_Skill::Start_ForScene(void)
{
	return S_OK;
}

void CUI_Skill::Update_Object(const _float& fTimeDelta)
{
}

void CUI_Skill::Render_Object(void)
{

}

SKILL_INFO* CUI_Skill::Get_Skill_Info(void)
{
	return m_pSkillInfo;
}

void CUI_Skill::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}
	
void CUI_Skill::Free(void)
{
	delete m_pSkillInfo;
	m_pSkillInfo = NULL;

	CUI::Free();
}
