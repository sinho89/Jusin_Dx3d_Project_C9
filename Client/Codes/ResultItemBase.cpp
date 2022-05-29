#include "stdafx.h"
#include "ResultItemBase.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "ResultItemLock.h"
#include "ResultItemX.h"
#include "UI_Item.h"
#include "Fade_Manager.h"
#include "Inven_Sheet.h"
#include "Sound.h"

CResultItemBase::CResultItemBase(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pInven(NULL)
{

}

CResultItemBase::~CResultItemBase(void)
{

}

HRESULT CResultItemBase::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = *m_pParent->GetfX();
	m_fY = *m_pParent->GetfY() + 100.f;

	m_fSizeX = 200.f;
	m_fSizeY = 180.f;

	m_fAlpha = 0.f;
	m_fAngle = 0.f;
	m_bSelect = false;
	m_bFade = false;
	m_bAlpha_Increase = false;
	m_bSound = false;
	m_fAlphaTime = 0.f;
	m_pItem = NULL;

	m_pInven = ((CUI*)Engine::Find(L"Inven_Sheet_1"));
	::Safe_Release(m_pInven);

	return S_OK;
}

HRESULT CResultItemBase::Start_ForScene(void)
{
	return S_OK;
}

void CResultItemBase::Update_Object(const _float& fTimeDelta)
{
	
	if (!m_bFade)
	{
		if (CFade_Manager::GetInstance()->GetResultFading() == true)
		{
			m_fAlpha += 0.05f;

			if (m_iIndex == 0)
			{
				if (m_fAlpha >= 0.5f)
					m_fAlpha = 0.5f;
			}
			else if (m_iIndex == 1)
			{
				if (m_fAlpha >= 0.5f)
				{
					if (!m_bSound)
					{
						m_bSound = true;
						m_pSound->MyPlaySound(L"ItemGet");
					}
				}
				if (m_fAlpha >= 1.f)
					m_fAlpha = 1.f;
			}
		}
		else
		{
			m_fAlpha -= 0.05f;

			m_pItem->SetAlpha(m_fAlpha);

			if (m_fAlpha <= 0.f)
			{
				if (m_iIndex == 1)
				{
					m_pItem->SetParent(m_pInven);
					m_pItem->SetResult(false);
					m_pItem->SetSize(45.f, 45.f);
					m_pItem->SetAlpha(1.f);
					((CInven_Sheet*)m_pInven)->AddResultInven(m_pItem);
					m_pItem = NULL;
				}
				else if (m_iIndex == 0)
				{
					m_pItem->SetParent(NULL);
					m_pItem->SetSize(0.f, 0.f);
					m_pItem->SetWinPos(_vec2(99999.f, 99999.f));
					m_pItem = NULL;
				}
				m_eState = Engine::STATE_DESTROY;
			}

		}

		m_fY -= fTimeDelta * 100.f;
		if (m_fY <= (*m_pParent->GetfY()))
		{
			m_fAlphaTime += fTimeDelta;
			m_fY = *m_pParent->GetfY();

			/*if (!m_bAlpha_Increase)
			{
				if (m_iIndex == 1)
				{
					m_pItem->SetParent(m_pInven);
					m_pItem->SetResult(false);
					((CInven_Sheet*)m_pInven)->AddResultInven(m_pItem);
					m_pItem = NULL;
				}
				m_bAlpha_Increase = true;
			}*/
			
		}
	}

	CUI::Update_Object(fTimeDelta);

	m_pRendererCom->Add_WindowUI(30006, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

	if (m_pItem != NULL)
	{
		m_pItem->SetWinPos(_vec2(m_fX, m_fY - 40.f));
		m_pItem->SetSize(60.f, 60.f);
	}
}

void CResultItemBase::Render_Object(void)
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

	if (m_pItem != NULL)
	{
		if (m_iIndex == 0)
		{
			switch (m_pItem->Get_Item_Info()->m_eItemType)
			{
			case ITEM_BLUE_WEAPON:
				Engine::Render_Font(L"Font_Result_Item", L"전설의 사라드 아크 블레이드", &_vec2(m_fX - 70.f, m_fY + 45.f), D3DXCOLOR(0.3f, 0.3f, 1.f, m_fAlpha));
				break;
			case ITEM_S_BLUE_WEAPON:
				Engine::Render_Font(L"Font_Result_Item", L"데스티네이션", &_vec2(m_fX - 40.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_ARMOR:
				Engine::Render_Font(L"Font_Result_Item", L"소피 샹드린의 물결 상의", &_vec2(m_fX - 60.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_LEG:
				Engine::Render_Font(L"Font_Result_Item", L"이주민의 신성한 영혼 하의", &_vec2(m_fX - 70.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_HELMET:
				Engine::Render_Font(L"Font_Result_Item", L"마스터 피나르킨의 투구", &_vec2(m_fX - 60.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_ARMOR:
				Engine::Render_Font(L"Font_Result_Item", L"혼 스타니카 상의", &_vec2(m_fX - 45.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_LEG:
				Engine::Render_Font(L"Font_Result_Item", L"사라드 명장의 정령 하의", &_vec2(m_fX - 60.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_GLOVE:
				Engine::Render_Font(L"Font_Result_Item", L"펀치 일족의 화염 장갑", &_vec2(m_fX - 55.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_BOOTS:
				Engine::Render_Font(L"Font_Result_Item", L"드 네메리아 신발", &_vec2(m_fX - 45.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_SHORDER:
				Engine::Render_Font(L"Font_Result_Item", L"뤼크니스 영혼의 스카프", &_vec2(m_fX - 60.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			}
		}
		else
		{
			switch (m_pItem->Get_Item_Info()->m_eItemType)
			{
			case ITEM_BLUE_WEAPON:
				Engine::Render_Font(L"Font_Result_Item", L"전설의 사라드 아크 블레이드", &_vec2(m_fX - 70.f, m_fY + 45.f), D3DXCOLOR(0.3f, 0.3f, 1.f, m_fAlpha));
				break;
			case ITEM_S_BLUE_WEAPON:
				Engine::Render_Font(L"Font_Result_Item", L"데스티네이션", &_vec2(m_fX - 40.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_ARMOR:
				Engine::Render_Font(L"Font_Result_Item", L"소피 샹드린의 물결 상의", &_vec2(m_fX - 60.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_LEG:
				Engine::Render_Font(L"Font_Result_Item", L"이주민의 신성한 영혼 하의", &_vec2(m_fX - 70.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_HELMET:
				Engine::Render_Font(L"Font_Result_Item", L"마스터 피나르킨의 투구", &_vec2(m_fX - 60.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_ARMOR:
				Engine::Render_Font(L"Font_Result_Item", L"혼 스타니카 상의", &_vec2(m_fX - 45.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_LEG:
				Engine::Render_Font(L"Font_Result_Item", L"사라드 명장의 정령 하의", &_vec2(m_fX - 60.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_GLOVE:
				Engine::Render_Font(L"Font_Result_Item", L"펀치 일족의 화염 장갑", &_vec2(m_fX - 55.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_BOOTS:
				Engine::Render_Font(L"Font_Result_Item", L"드 네메리아 신발", &_vec2(m_fX - 45.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			case ITEM_S_SHORDER:
				Engine::Render_Font(L"Font_Result_Item", L"뤼크니스 영혼의 스카프", &_vec2(m_fX - 60.f, m_fY + 45.f), D3DXCOLOR(1.f, 1.f, 0.f, m_fAlpha));
				break;
			}
		}
	}
	
}



HRESULT CResultItemBase::Add_Component(void)
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

	if(m_iIndex == 1)
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"GetItemWindow");
	else if (m_iIndex == 0)
		m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"GetItemWindowNS");

	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


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

	// Sound
	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ItemGet", L"ItemGet.wav");

	return S_OK;
}

CResultItemBase* CResultItemBase::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultItemBase*		pInstance = new CResultItemBase(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CResultItemBase* CResultItemBase::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParent, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultItemBase*		pInstance = new CResultItemBase(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetIndex(iIndex);
	pInstance->SetParent(pParent);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CResultItemBase::Free(void)
{
	CUI::Free();
}
