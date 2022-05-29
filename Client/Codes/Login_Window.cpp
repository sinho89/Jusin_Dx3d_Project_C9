#include "stdafx.h"
#include "Login_Window.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CLogin_Window::CLogin_Window(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_bPWCollision(false)
,m_vFontIDPos(0.f, 0.f)
,m_vFontPWPos(0.f, 0.f)
{

}

CLogin_Window::~CLogin_Window(void)
{

}

HRESULT CLogin_Window::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1) + 150.f;

	m_fSizeX = 400.f;
	m_fSizeY = 250.f;

	return S_OK;
}

HRESULT CLogin_Window::Start_ForScene(void)
{
	return S_OK;
}

void CLogin_Window::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);
	CunvertFontPos();

	InPut_ID();
	InPut_PassWord();

	m_pRendererCom->Add_WindowUI(2, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

}

void CLogin_Window::Render_Object(void)
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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	Engine::Render_Font(L"Font_Select_DunGeun", L"ID : ", &_vec2(m_fX - 117.5f, m_fY - 55.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	Engine::Render_Font(L"Font_Select_DunGeun", L"Password : ", &_vec2(m_fX - 175.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));

	Engine::Render_Font(L"Font_Login", g_wstrID.c_str(), &_vec2(m_fX + 5.f, m_fY - 53.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	Engine::Render_Font(L"Font_Login", g_wstrPW.c_str(), &_vec2(m_fX + 5.f, m_fY - 6.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
}

void CLogin_Window::CunvertFontPos()
{
}

void CLogin_Window::Alpha_Calculation(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if(m_fAlpha <= 0.f)
		m_bAlpha_Increase = true;
	if(m_fAlpha >= 1.f)
		m_bAlpha_Increase = false;


	if(m_fAlphaTime >= 0.05f)
	{
		m_fAlphaTime = 0.f;

		if(m_bAlpha_Increase)
			m_fAlpha += 0.03f;
	}
}

void CLogin_Window::InPut_ID()
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

	if(m_bCollision)
	{
		if(GetKeyState(VK_LBUTTON) & 0x8000) //g_bInputID
		{
			if(m_bClick)
				return;
			m_bClick = true;
			
			if(!g_bInputID)
			{
				g_bInputID = true;
				g_bInputPW = false;
			}
		}
		else
			m_bClick = false;
	}
}

void CLogin_Window::InPut_PassWord()
{
	if(g_bInputID)
	{
		if(GetKeyState(VK_TAB) & 0x8000) //g_bInputID
		{
			if(m_bClick)
				return;
			m_bClick = true;

			if(!g_bInputPW)
			{
				g_bInputID = false;
				g_bInputPW = true;
			}
		}
		else
			m_bClick = false;
	}
}

HRESULT CLogin_Window::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGO, L"Texture_Login_Window");
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

CLogin_Window* CLogin_Window::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CLogin_Window*		pInstance = new CLogin_Window(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogin_Window::Free(void)
{
	CUI::Free();
}
