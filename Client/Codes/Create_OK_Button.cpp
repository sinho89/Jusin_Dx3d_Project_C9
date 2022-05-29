#include "stdafx.h"
#include "Create_OK_Button.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Sound.h"

CCreate_OK_Button::CCreate_OK_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pNotColTextureCom(NULL)
,m_pColTextureCom(NULL)
,m_pClickTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

CCreate_OK_Button::~CCreate_OK_Button(void)
{

}

HRESULT CCreate_OK_Button::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	//m_fX = (WINSIZEX >> 1) + 700;
	//m_fY = (WINSIZEY >> 1) + 400;

	m_fSizeX = 150.0f;
	m_fSizeY = 40.0f;

	return S_OK;
}

HRESULT CCreate_OK_Button::Start_ForScene(void)
{
	return S_OK;
}

void CCreate_OK_Button::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);
	PtInRect_Collision();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CCreate_OK_Button::Render_Object(void)
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

	if(!m_bCollision)
		m_pNotColTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	else
		m_pColTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 

	if(!m_iIndex)
		Engine::Render_Font(L"Font_Create_Char", L"캐릭터 생성", &_vec2(m_fX - 35.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
	else
		Engine::Render_Font(L"Font_Create_Char", L"이전 화면", &_vec2(m_fX - 30.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));

}

void CCreate_OK_Button::Render_Font_MoniterToWindow(const _float& fX, const _float& fY, const _tchar* pFontString)
{
}

void CCreate_OK_Button::PtInRect_Collision()
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
		if(GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if(m_bClick)
				return;
			m_bClick = true;
			m_pSound->MyPlaySound(L"ButtonPick");
			
			if(!m_iIndex)
			{
				// 세이브
				m_bSkipScene = true;
			}
			else
				m_bSkipScene = true;
		}
		else
			m_bClick = false;
	}

}

void CCreate_OK_Button::Alpha_Calculation(const _float& fTimeDelta)
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
			m_fAlpha += 0.02f;
	}
}

void CCreate_OK_Button::Set_Info(_float fX, _float fY, _uint iIndex)
{
	m_fX = (WINSIZEX >> 1) + fX;
	m_fY = (WINSIZEY >> 1) + fY;
	m_iIndex = iIndex;
}

HRESULT CCreate_OK_Button::Add_Component(void)
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
	m_pNotColTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Texture_NorMal_Button_NC");
	pComponent = (Engine::CComponent*)m_pNotColTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	m_pColTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Texture_NorMal_Button_C");
	pComponent = (Engine::CComponent*)m_pColTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture2", pComponent));

	m_pClickTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Texture_NorMal_Button_Click");
	pComponent = (Engine::CComponent*)m_pClickTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture3", pComponent));


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
	pComponent = m_pSound = CSound::Create(m_pTransCom);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");

	return S_OK;
}

CCreate_OK_Button* CCreate_OK_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCreate_OK_Button*		pInstance = new CCreate_OK_Button(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CCreate_OK_Button* CCreate_OK_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _float fX, _float fY, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCreate_OK_Button*		pInstance = new CCreate_OK_Button(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->Set_Info(fX, fY, iIndex);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CCreate_OK_Button::Free(void)
{
	CUI::Free();
}
