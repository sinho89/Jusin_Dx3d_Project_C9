#include "stdafx.h"
#include "Create_Class_Button.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Create_Movie.h"
#include "Sound.h"

CCreate_Class_Button::CCreate_Class_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pNCTextureCom(NULL)
,m_pCTextureCom(NULL)
,m_pSTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{
	ZeroMemory(&m_szName, NULL);
}

CCreate_Class_Button::~CCreate_Class_Button(void)
{

}

HRESULT CCreate_Class_Button::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	//m_fX = (WINSIZEX >> 1) + 675;
	//m_fY = (WINSIZEY >> 1) - 390;

	m_fSizeX = 175.0f;
	m_fSizeY = 50.0f;

	return S_OK;
}

HRESULT CCreate_Class_Button::Start_ForScene(void)
{
	return S_OK;
}

void CCreate_Class_Button::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);
	PtInRect_Collision(fTimeDelta);
	CunvertFontPos();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CCreate_Class_Button::Render_Object(void)
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

	if(m_bSelect)
	{
		m_pSTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}
	else
	{
		if(!m_bCollision)
			m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");

		else
			m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	}
	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 

	Engine::Render_Font(L"Font_Create_Char", m_szName, &_vec2(m_fX - 45.f, m_fY - 10.f), D3DXCOLOR(0.f, 1.f, 0.75f, m_fAlpha));
}

void CCreate_Class_Button::PtInRect_Collision(const _float& fTimeDelta)
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

			list<Engine::CGameObject*>*	ButtonList = Engine::FindList(L"UI_BUTTON");

			list<Engine::CGameObject*>::iterator iter = ButtonList->begin();
			list<Engine::CGameObject*>::iterator iter_end = ButtonList->end();

			int iBeforeIndex = 0;

			for(; iter != iter_end; ++iter)
			{
				/*if((*iter) == this)
					continue;*/

				if(((CUI*)(*iter))->GetSelect())
				{
					iBeforeIndex = ((CUI*)(*iter))->GetIndex();
				}
				((CUI*)(*iter))->SetSelect(false);
			}
			m_bSelect = true;

			CUI* pUi = ((CUI*)Engine::Find(L"ClassBackGround"));
			pUi->SetIndex(m_iIndex);
			::Safe_Release(pUi);

			pUi = ((CUI*)Engine::Find(L"ClassBackGround"));
			pUi->SetAlpha(0.f);
			::Safe_Release(pUi);

			pUi = ((CUI*)Engine::Find(L"ClassTexture"));
			pUi->SetIndex(m_iIndex);
			::Safe_Release(pUi);

			pUi = ((CUI*)Engine::Find(L"ClassTexture"));
			pUi->SetAlpha(0.f);
			::Safe_Release(pUi);

			pUi = ((CUI*)Engine::Find(L"ClassToolTip"));
			pUi->SetIndex(m_iIndex);
			::Safe_Release(pUi);

			pUi = ((CUI*)Engine::Find(L"ClassToolTip"));
			pUi->SetAlpha(0.f);
			::Safe_Release(pUi);

			pUi = ((CUI*)Engine::Find(L"Pentagon"));
			pUi->SetIndex(m_iIndex);
			pUi->SetBeforeIndex(iBeforeIndex);
			pUi->SetSelect(true);
			::Safe_Release(pUi);


			CCreate_Movie* pCMovie = NULL;
			if(0 == m_iIndex)
			{
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Fighter"));
				pCMovie->ReStart();
				pCMovie->RenderCheck() = TRUE;
				pCMovie->GetAlpha() = 0.0f;
				::Safe_Release(pCMovie);

				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Hunter"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Shaman"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_WitchBlade"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Mistic"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
			}
			else if(1 == m_iIndex)
			{
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Hunter"));
				pCMovie->ReStart();
				pCMovie->RenderCheck() = TRUE;
				pCMovie->GetAlpha() = 0.0f;
				::Safe_Release(pCMovie);

				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Fighter"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Shaman"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_WitchBlade"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Mistic"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
			}
			else if(2 == m_iIndex)
			{
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Shaman"));
				pCMovie->ReStart();
				pCMovie->RenderCheck() = TRUE;
				pCMovie->GetAlpha() = 0.0f;
				::Safe_Release(pCMovie);

				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Hunter"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Fighter"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_WitchBlade"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Mistic"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
			}
			else if(3 == m_iIndex)
			{
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_WitchBlade"));
				pCMovie->ReStart();
				pCMovie->RenderCheck() = TRUE;
				pCMovie->GetAlpha() = 0.0f;
				::Safe_Release(pCMovie);

				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Hunter"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Shaman"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Fighter"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Mistic"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
			}
			else if(4 == m_iIndex)
			{
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Mistic"));
				pCMovie->ReStart();
				pCMovie->RenderCheck() = TRUE;
				pCMovie->GetAlpha() = 0.0f;
				::Safe_Release(pCMovie);

				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Hunter"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Shaman"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_WitchBlade"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
				pCMovie = ((CCreate_Movie*)Engine::Find(L"Intro_Fighter"));
				pCMovie->RenderCheck() = FALSE;
				::Safe_Release(pCMovie);
			}			
		}
		else
			m_bClick = false;
	}
}

void CCreate_Class_Button::CunvertFontPos(void)
{
	//m_vFontPos = m_pCalCom->CunvertFontPos(m_pTransCom->m_vInfo[Engine::INFO_POSITION], WINSIZEX, WINSIZEY, 0, 0);
}

void CCreate_Class_Button::Alpha_Calculation(const _float& fTimeDelta)
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
			m_fAlpha += 0.01f;
	}
}

void CCreate_Class_Button::Set_Info(_float fX, _float fY, _uint iIndex)
{
	m_fX = (WINSIZEX >> 1) - fX;
	m_fY = (WINSIZEY >> 1) - fY;
	m_iIndex = iIndex;
}

HRESULT CCreate_Class_Button::Add_Component(void)
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

	switch(m_iIndex)
	{
	case 0:
		m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_F_NC");
		pComponent = (Engine::CComponent*)m_pNCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

		m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_F_C");
		pComponent = (Engine::CComponent*)m_pCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

		m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_F_S");
		pComponent = (Engine::CComponent*)m_pSTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_S", pComponent));

		m_szName = L"ÆÄÀÌÅÍ";
		m_bSelect = true;
		break;
	case 1:
		m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_H_NC");
		pComponent = (Engine::CComponent*)m_pNCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

		m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_H_C");
		pComponent = (Engine::CComponent*)m_pCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

		m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_H_S");
		pComponent = (Engine::CComponent*)m_pSTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_S", pComponent));
		m_szName = L"ÇåÅÍ";
		break;
	case 2:
		m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_S_NC");
		pComponent = (Engine::CComponent*)m_pNCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

		m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_S_C");
		pComponent = (Engine::CComponent*)m_pCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

		m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_S_S");
		pComponent = (Engine::CComponent*)m_pSTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_S", pComponent));
		m_szName = L"»þ¸Õ";
		break;
	case 3:
		m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_W_NC");
		pComponent = (Engine::CComponent*)m_pNCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

		m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_W_C");
		pComponent = (Engine::CComponent*)m_pCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

		m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_W_S");
		pComponent = (Engine::CComponent*)m_pSTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_S", pComponent));
		m_szName = L"À§Ä¡ºí·¹ÀÌµå";
		break;
	case 4:
		m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_M_NC");
		pComponent = (Engine::CComponent*)m_pNCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

		m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_M_C");
		pComponent = (Engine::CComponent*)m_pCTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

		m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGOSTATIC, L"Class_Button_M_S");
		pComponent = (Engine::CComponent*)m_pSTextureCom;
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_S", pComponent));
		m_szName = L"¹Ì½ºÆ½";
		break;
	}

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

CCreate_Class_Button* CCreate_Class_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCreate_Class_Button*		pInstance = new CCreate_Class_Button(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CCreate_Class_Button* CCreate_Class_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _float fX, _float fY, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCreate_Class_Button*		pInstance = new CCreate_Class_Button(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->Set_Info(fX, fY, iIndex);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CCreate_Class_Button::Free(void)
{
	CUI::Free();
}
