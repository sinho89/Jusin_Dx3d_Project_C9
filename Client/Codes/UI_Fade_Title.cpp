#include "stdafx.h"
#include "UI_Fade_Title.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CUI_Fade_Title::CUI_Fade_Title(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_fAniTime(0.0f)
	, m_fMagicScale(5.f)
	, m_bFadeIn(false)
	, m_bFadeOut(false)
{

}

CUI_Fade_Title::~CUI_Fade_Title(void)
{

}

HRESULT CUI_Fade_Title::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	switch (m_eFadeType)
	{
	case FADE_BIM:
		m_fX = (WINSIZEX >> 1) - 650.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_PELL:
		m_fX = (WINSIZEX >> 1) - 610.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_LEE:
		m_fX = (WINSIZEX >> 1) - 585.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_SUNG:
		m_fX = (WINSIZEX >> 1) - 545.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
		//// ICE
	case FADE_SO:
		m_fX = (WINSIZEX >> 1) - 650.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_UL:
		m_fX = (WINSIZEX >> 1) - 625.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_VELL:
		m_fX = (WINSIZEX >> 1) - 585.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_ICE_LEE:
		m_fX = (WINSIZEX >> 1) - 560.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
		//// Fire
	case FADE_VOL:
		m_fX = (WINSIZEX >> 1) - 650.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_S:
		m_fX = (WINSIZEX >> 1) - 625.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_LAN:
		m_fX = (WINSIZEX >> 1) - 585.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	case FADE_D:
		m_fX = (WINSIZEX >> 1) - 560.f;
		m_fY = (WINSIZEY >> 1) + 200.f;
		break;
	}

	m_fSizeX = 40.f;
	m_fSizeY = 45.f;

	m_fAlpha = 0.f;

	return S_OK;
}

HRESULT CUI_Fade_Title::Start_ForScene(void)
{
	return S_OK;
}

void CUI_Fade_Title::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	FadeUpdate(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}
void CUI_Fade_Title::FadeUpdate(const _float& fTimeDelta)
{
	m_fAniTime += fTimeDelta;

	if (!m_bFadeIn && !m_bFadeOut)
	{
		switch (m_eFadeType)
		{
		case FADE_BIM:
			if (m_fAniTime >= 0.f)
				m_bFadeIn = true;
			break;
		case FADE_PELL:
			if (m_fAniTime >= 0.5f)
				m_bFadeIn = true;
			break;
		case FADE_LEE:
			if (m_fAniTime >= 1.f)
				m_bFadeIn = true;
			break;
		case FADE_SUNG:
			if (m_fAniTime >= 1.5f)
				m_bFadeIn = true;
			break;
			// ICE
		case FADE_SO:
			if (m_fAniTime >= 0.f)
				m_bFadeIn = true;
			break;
		case FADE_UL:
			if (m_fAniTime >= 0.5f)
				m_bFadeIn = true;
			break;
		case FADE_VELL:
			if (m_fAniTime >= 1.f)
				m_bFadeIn = true;
			break;
		case FADE_ICE_LEE:
			if (m_fAniTime >= 1.5f)
				m_bFadeIn = true;
			break;
			/// Fire
		case FADE_VOL:
			if (m_fAniTime >= 0.f)
				m_bFadeIn = true;
			break;
		case FADE_S:
			if (m_fAniTime >= 0.5f)
				m_bFadeIn = true;
			break;
		case FADE_LAN:
			if (m_fAniTime >= 1.f)
				m_bFadeIn = true;
			break;
		case FADE_D:
			if (m_fAniTime >= 1.5f)
				m_bFadeIn = true;
			break;
		}

	}

	if (m_bFadeIn && !m_bFadeOut)
	{
		m_fAlpha += 0.02f;

		if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
		{
			m_fSizeX += 0.2f;
			m_fSizeY += 0.2f;
		}
		else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
		{
			m_fSizeX -= 0.2f;
			m_fSizeY -= 0.2f;
		}

		if (m_fAlpha > 1.f)
		{
			m_bFadeIn = false;
			m_bFadeOut = true;
		}
	}

	if (!m_bFadeIn && m_bFadeOut)
	{
		switch (m_eFadeType)
		{
		case FADE_BIM:
			if (m_fAniTime >= 7.1f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_PELL:
			if (m_fAniTime >= 6.9f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_LEE:
			if (m_fAniTime >= 6.7f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_SUNG:
			if (m_fAniTime >= 6.5f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
			///// ICE
		case FADE_SO:
			if (m_fAniTime >= 7.1f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_UL:
			if (m_fAniTime >= 6.9f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_VELL:
			if (m_fAniTime >= 6.7f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_ICE_LEE:
			if (m_fAniTime >= 6.5f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
			//// Fire
		case FADE_VOL:
			if (m_fAniTime >= 7.1f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_S:
			if (m_fAniTime >= 6.9f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_LAN:
			if (m_fAniTime >= 6.7f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		case FADE_D:
			if (m_fAniTime >= 6.5f)
			{
				m_fAlpha -= 0.02f;

				if ((m_fAlpha > 0.f) && (m_fAlpha <= 0.5f))
				{
					m_fSizeX -= 0.02f;
					m_fSizeY -= 0.02f;
				}
				else if ((m_fAlpha > 0.5f) && (m_fAlpha <= 1.f))
				{
					m_fSizeX += 0.02f;
					m_fSizeY += 0.02f;
				}

				if (m_fAlpha < 0.f)
					m_eState = Engine::STATE_DESTROY;
			}
			break;
		}
	}

}



void CUI_Fade_Title::Render_Object(void)
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

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}


HRESULT CUI_Fade_Title::Add_Component(void)
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

	switch (m_eFadeType)
	{
	case FADE_BIM:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Bim");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_PELL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Pell");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_LEE:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Lee");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_SUNG:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Sung");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_SO:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_So");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_UL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Ul");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_VELL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Vell");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_ICE_LEE:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Lee");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_VOL:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Bol");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_S:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_S");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_LAN:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_Lan");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	case FADE_D:
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Fade_D");
		pComponent = (Engine::CComponent*)m_pTextureCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		break;
	}

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

	return S_OK;
}


CUI_Fade_Title* CUI_Fade_Title::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Fade_Title*		pInstance = new CUI_Fade_Title(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}



CUI_Fade_Title* CUI_Fade_Title::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, FADE_ID eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_Fade_Title*		pInstance = new CUI_Fade_Title(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetFadeType(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Fade_Title::Free(void)
{
	CUI::Free();
}
