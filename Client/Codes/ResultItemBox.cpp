#include "stdafx.h"
#include "ResultItemBox.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "ResultItemLock.h"
#include "ResultItemX.h"
#include "ResultItemBase.h"
#include "UI_Weapon.h"
#include "UI_Armor.h"
#include "UI_Leg.h"
#include "UI_Helmet.h"
#include "UI_Glove.h"
#include "UI_Boots.h"
#include "UI_Shorder.h"
#include "Fade_Manager.h"
#include "Sound.h"

CResultItemBox::CResultItemBox(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{

}

CResultItemBox::~CResultItemBox(void)
{

}

HRESULT CResultItemBox::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	switch (m_iIndex)
	{
	case 0:
		m_pSound->MyPlaySound(L"CreatBox");
		m_fX = (WINSIZEX >> 1) - 450.f;
		break;
	case 1:
		m_fX = (WINSIZEX >> 1) - 150.f;
		break;
	case 2:
		m_fX = (WINSIZEX >> 1) + 150.f;
		break;
	case 3:
		m_fX = (WINSIZEX >> 1) + 450.f;
		break;
	}
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = 150.f;
	m_fSizeY = 140.f;

	m_fAlpha = 0.f;
	m_fAngle = 0.f;
	m_bSelect = false;
	m_bAlpha_Increase = false;
	m_bSheetSelect = false;
	m_fAlphaTime = 0.f;

	Engine::CGameObject* pGameObject = CResultItemLock::Create(m_pGraphicDev, L"ItemLock", this);
	Engine::AddObject(Engine::LAYER_UI, L"UI_Result_Item", pGameObject);

	return S_OK;
}

HRESULT CResultItemBox::Start_ForScene(void)
{
	return S_OK;
}

void CResultItemBox::Update_Object(const _float& fTimeDelta)
{
	if (CFade_Manager::GetInstance()->GetResultFading() == false)
		return;

	if (!m_bFade)
		m_fAlpha = m_pParent->GetAlpha();

	else if (m_bFade)
	{
		if (!m_bSelect)
		{
			Engine::CGameObject* pGameObject = CResultItemX::Create(m_pGraphicDev, L"ItemLock", this, m_bPick);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Result_Item", pGameObject);
			m_bSelect = true;
		}
		if (!m_bPick)
		{
			m_fAlpha -= 0.05f;
			if (m_fAlpha <= 0.3f)
			{
				if (!m_bAlpha_Increase)
				{
					Engine::CGameObject* pGameObject = CResultItemBase::Create(m_pGraphicDev, L"ResultItemBase", this, m_bPick);
					Engine::AddObject(Engine::LAYER_UI, L"UI_Result_Item", pGameObject);

					CUI* pParent = ((CUI*)pGameObject);
					int i = rand() % 10;

					switch (i)
					{
					case 0:
						pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_Blue", pParent, WEAPON_BLUE, false);
						break;
					case 1:
						pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_S_Blue", pParent, WEAPON_S_BLUE, false);
						break;
					case 2:
						pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_Normal", pParent, ARMOR_NORMAL);
						break;
					case 3:
						pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_Normal", pParent, LEG_NORMAL);
						break;
					case 4:
						pGameObject = CUI_Helmet::Create(m_pGraphicDev, L"Helmet_S_Normal", pParent, S_HELMET);
						break;
					case 5:
						pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_S_Normal", pParent, S_ARMOR);
						break;
					case 6:
						pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_S_Normal", pParent, S_LEG);
						break;
					case 7:
						pGameObject = CUI_Glove::Create(m_pGraphicDev, L"Glove_Normal", pParent, GLOVE_NORMAL);
						break;
					case 8:
						pGameObject = CUI_Boots::Create(m_pGraphicDev, L"Boots_S_Normal", pParent, S_BOOTS);
						break;
					case 9:
						pGameObject = CUI_Shorder::Create(m_pGraphicDev, L"Shorder_S_Normal", pParent, S_SHORDER);
						break;
					}
					Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
					((CUI_Item*)pGameObject)->SetResult(true);
					((CUI*)pGameObject)->SetAlpha(0.75f);
					((CResultItemBase*)pParent)->SetItem(((CUI_Item*)((CUI*)pGameObject)));


					m_bAlpha_Increase = true;
				}
			}

			if (m_fAlpha <= 0.f)
				m_fAlpha = 0.f;
		}
		else if (m_bPick)
		{
			m_fAlphaTime += fTimeDelta;

			if (m_fAlphaTime >= 2.f)
			{
				m_fAlpha -= 0.05f;
				if (!m_bAlpha_Increase)
				{
					Engine::CGameObject* pGameObject = CResultItemBase::Create(m_pGraphicDev, L"ResultItemBase", this, m_bPick);
					Engine::AddObject(Engine::LAYER_UI, L"UI_Result_Item", pGameObject);

					CUI* pParent = ((CUI*)pGameObject);
					int i = rand() % 10;

					switch (i)
					{
					case 0:
						pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_Blue", pParent, WEAPON_BLUE, false);
						break;
					case 1:
						pGameObject = CUI_Weapon::Create(m_pGraphicDev, L"Weapon_S_Blue", pParent, WEAPON_S_BLUE, false);
						break;
					case 2:
						pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_Normal", pParent, ARMOR_NORMAL);
						break;
					case 3:
						pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_Normal", pParent, LEG_NORMAL);
						break;
					case 4:
						pGameObject = CUI_Helmet::Create(m_pGraphicDev, L"Helmet_S_Normal", pParent, S_HELMET);
						break;
					case 5:
						pGameObject = CUI_Armor::Create(m_pGraphicDev, L"Armor_S_Normal", pParent, S_ARMOR);
						break;
					case 6:
						pGameObject = CUI_Leg::Create(m_pGraphicDev, L"Leg_S_Normal", pParent, S_LEG);
						break;
					case 7:
						pGameObject = CUI_Glove::Create(m_pGraphicDev, L"Glove_Normal", pParent, GLOVE_NORMAL);
						break;
					case 8:
						pGameObject = CUI_Boots::Create(m_pGraphicDev, L"Boots_S_Normal", pParent, S_BOOTS);
						break;
					case 9:
						pGameObject = CUI_Shorder::Create(m_pGraphicDev, L"Shorder_S_Normal", pParent, S_SHORDER);
						break;
					}
					Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pGameObject);
					((CUI_Item*)pGameObject)->SetResult(true);
					((CUI*)pGameObject)->SetAlpha(1.f);
					((CResultItemBase*)pParent)->SetItem(((CUI_Item*)((CUI*)pGameObject)));

					m_bAlpha_Increase = true;
				}

				if (m_fAlpha <= 0.f)
					m_fAlpha = 0.f;
			}

			if (m_fAlphaTime >= 5.f)
			{
				if (!m_bSheetSelect)
				{
					m_bSheetSelect = true;
					
					CFade_Manager::GetInstance()->FadeEnd(0.f, 0.05f, CFade_Manager::FADE_CLEAR_GETITEM);
					CFade_Manager::GetInstance()->SetResultFading(false);
				}
			}
		}
	}

	
	

	CUI::Update_Object(fTimeDelta);

	m_pRendererCom->Add_WindowUI(30001, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CResultItemBox::Render_Object(void)
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



HRESULT CResultItemBox::Add_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SelectItemBox");
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

	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"CreatBox", L"CreatBox.wav");

	return S_OK;
}

CResultItemBox* CResultItemBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultItemBox*		pInstance = new CResultItemBox(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CResultItemBox* CResultItemBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParent, _uint iIndex, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CResultItemBox*		pInstance = new CResultItemBox(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetIndex(iIndex);
	pInstance->SetParent(pParent);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CResultItemBox::Free(void)
{
	CUI::Free();
}
