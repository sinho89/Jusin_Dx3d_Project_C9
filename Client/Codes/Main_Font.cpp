#include "stdafx.h"
#include "Main_Font.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_Skill.h"
#include "Cursor.h"
#include "Main_Block.h"
#include "Fade_Manager.h"

CMain_Font::CMain_Font(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

CMain_Font::~CMain_Font(void)
{

}

HRESULT CMain_Font::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = m_pParentWindow->GetInfo();

	m_fX = vInfo.x;
	m_fY = vInfo.y;

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;

	m_fAlpha = 1.f;
	m_iIndex = m_pParentWindow->GetIndex();
	m_bSheetSelect = m_pParentWindow->GetSheetSelect();
	m_eWinType = WINDOW_SKILL;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CMain_Font::Start_ForScene(void)
{

	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
		m_bIsActive = false;
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;
	return S_OK;
}

void CMain_Font::Update_Object(const _float& fTimeDelta)
{

	if (CFade_Manager::GetInstance()->GetFading() == true)
		return;

	switch(m_pParentWindow->GetSlotLevel())
	{
	case 1:
		if(m_pParentWindow->GetIndex() > 10 && m_pParentWindow->GetIndex() <= 30)
			return;
		break;
	case 2:
		if(m_pParentWindow->GetIndex() > 0 && m_pParentWindow->GetIndex() <= 10)
			return;
		if(m_pParentWindow->GetIndex() > 20 && m_pParentWindow->GetIndex() <= 30)
			return;
		break;
	case 3:
		if(m_pParentWindow->GetIndex() > 0 && m_pParentWindow->GetIndex() <= 20)
			return;
		break;
	}


	_vec4 vInfo = m_pParentWindow->GetInfo();

	m_fX = vInfo.x;
	m_fY = vInfo.y;

	CUI::Update_Object(fTimeDelta);

	if(((CMain_Block*)m_pParentWindow)->GetSkill())
		m_wSortNum = 2;
	if(((CMain_Block*)m_pParentWindow)->GetItem())
		m_wSortNum = 2;
	else
		m_wSortNum = 0;
	m_pRendererCom->Add_WindowUI(m_wSortNum, this);
	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);


}

void CMain_Font::Render_Object(void)
{
	switch(m_iIndex)
	{
	case 31:
		Engine::Render_Font(L"Font_Main_Slot",  L"Z", &_vec2(m_fX - 17.5f, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 32:
		Engine::Render_Font(L"Font_Main_Slot",  L"X", &_vec2(m_fX - 17.5f, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 33:
		Engine::Render_Font(L"Font_Main_Slot",  L"C", &_vec2(m_fX - 17.5f, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 34:
		Engine::Render_Font(L"Font_Main_Slot",  L"V", &_vec2(m_fX - 17.5f, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 35:
		Engine::Render_Font(L"Font_Main_Slot",  L"R", &_vec2(m_fX - 17.5f, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 36:
		Engine::Render_Font(L"Font_Main_Slot",  L"G", &_vec2(m_fX - 17.5f, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 37:
		Engine::Render_Font(L"Font_Main_Slot",  L"T", &_vec2(m_fX - 17.5f, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 38:
		Engine::Render_Font(L"Font_Main_Slot",  L"N", &_vec2(m_fX - 17.5f, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	}
	

	_tchar		szInfo[128] = L"";
	wsprintf(szInfo, L"%d", m_pParentWindow->GetIndex());
	if(m_pParentWindow->GetIndex() == 10 || m_pParentWindow->GetIndex() == 20 || m_pParentWindow->GetIndex() == 30)
		Engine::Render_Font(L"Font_Main_Slot",  L"0", &_vec2(m_fX - 20, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	else if(m_pParentWindow->GetIndex() <= 10)
		Engine::Render_Font(L"Font_Main_Slot",  szInfo, &_vec2(m_fX - 20, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	else if(m_pParentWindow->GetIndex() > 10 && m_pParentWindow->GetIndex() <= 20)
	{
		wsprintf(szInfo, L"%d", m_pParentWindow->GetIndex() - 10);
		Engine::Render_Font(L"Font_Main_Slot",  szInfo, &_vec2(m_fX - 20, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	}
	else if(m_pParentWindow->GetIndex() > 20 && m_pParentWindow->GetIndex() <= 30)
	{
		wsprintf(szInfo, L"%d", m_pParentWindow->GetIndex() - 20);
		Engine::Render_Font(L"Font_Main_Slot",  szInfo, &_vec2(m_fX - 20, m_fY - 22.5f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	}

}


HRESULT CMain_Font::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Main_Slot_Base");
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
CMain_Font* CMain_Font::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CMain_Font*		pInstance = new CMain_Font(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CMain_Font* CMain_Font::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, _vec2 vPos, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CMain_Font*		pInstance = new CMain_Font(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetPos(vPos);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}
void CMain_Font::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}


void CMain_Font::SetPos(_vec2 vPos)
{
	m_vParentBasePos = vPos;
}

void CMain_Font::Free(void)
{
	//::Safe_Release(m_pParentWindow);
	CUI::Free();
}
