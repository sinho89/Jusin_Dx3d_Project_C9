#include "stdafx.h"
#include "ESC_Button.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Partition_Base.h"
#include "UI_Item.h"
#include "..\Engine\Utility\Codes\Scene.h"
#include "UI_Potion.h"
#include "Cursor.h"
#include "Scene_Stage_Ice.h"
#include "Scene_Stage_Fire.h"
#include "Sound.h"

CESC_Button::CESC_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pNCTextureCom(NULL)
	, m_pCTextureCom(NULL)
	, m_pClickTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_bType(false)
	, m_pScene(NULL)
	, m_pSystem(NULL)
{

}

CESC_Button::~CESC_Button(void)
{

}

HRESULT CESC_Button::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (m_bType)
	{
		m_fX = m_pParent->GetInfo().x;
		m_fY = m_pParent->GetInfo().y - 10.f;
	}
	else
	{
		m_fX = m_pParent->GetInfo().x;
		m_fY = m_pParent->GetInfo().y + 60.f;
	}

	m_fSizeX = 140.f;
	m_fSizeY = 40.f;

	m_fAlpha = 1.f;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CESC_Button::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	m_pScene = const_cast<Engine::CScene*>(Engine::GetScene());
	m_pSystem = ((CUI*)Engine::Find(L"System_Base"));
	::Safe_Release(m_pSystem);
	return S_OK;
}

void CESC_Button::Update_Object(const _float& fTimeDelta)
{
	m_bRender = m_pParent->GetRender();

	if (m_bRender)
	{
		if (m_bType)
		{
			m_fX = m_pParent->GetInfo().x;
			m_fY = m_pParent->GetInfo().y;
		}
		else
		{
			m_fX = m_pParent->GetInfo().x;
			m_fY = m_pParent->GetInfo().y + 60.f;
		}

		CUI::Update_Object(fTimeDelta);
		PtInRect_Collision(fTimeDelta);
		m_wSortNum = m_pParent->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
		m_bWindowProcess = false;
		m_bSelect = false;
		m_bMove = false;
	}

}

void CESC_Button::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect)
		return;

	_uint			iPassCnt;
	_float			fFontAlpha;

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

	m_fAlpha = m_pParent->GetAlpha();
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	if (m_bClick)
	{
		fFontAlpha = 0.75f;
		m_pClickTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}
	else
	{
		if (m_bCollision)
		{
			fFontAlpha = 1.f;
			m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
		}
		else
		{
			fFontAlpha = 0.75f;
			m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
		}
	}

	if (m_fAlpha != 1.f)
		fFontAlpha = 0.f;

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);


	if (m_bType)
		Engine::Render_Font(L"Font_Main_Button", L"환경 설정", &_vec2(m_fX - 20.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, fFontAlpha));
	else
	{
		if (Engine::GetSceneID() == Engine::SCENE_TOWN)
			Engine::Render_Font(L"Font_Main_Button", L"게임 종료", &_vec2(m_fX - 20.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, fFontAlpha));
		if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
			Engine::Render_Font(L"Font_Main_Button", L"마을 복귀", &_vec2(m_fX - 20.f, m_fY - 10.f), D3DXCOLOR(1.f, 1.f, 1.f, fFontAlpha));
	}

}


HRESULT CESC_Button::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Button_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Button_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pClickTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Button_Click");
	pComponent = (Engine::CComponent*)m_pClickTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Click", pComponent));

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

	// For.Calculater Component
	pComponent = m_pCalCom = Engine::CUICalculater::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_UICal", pComponent));

	// Sound
	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");

	return S_OK;
}
void CESC_Button::PtInRect_Collision(const _float& fTimeDelta)
{
	if (m_pSystem->GetRender())
		return;
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


	if (m_bCollision)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if (m_bClick)
				return;
			m_bClick = true;
			m_pSound->MyPlaySound(L"ButtonPick");

			/*CUI* pWeaponShop = ((CUI*)Engine::Find(L"Shop_Base_Weapon"));
			CUI* pArmorShop = ((CUI*)Engine::Find(L"Shop_Base_Armor"));
			CUI* pAccShop = ((CUI*)Engine::Find(L"Shop_Base_Acc"));
			CUI* pPotionShop = ((CUI*)Engine::Find(L"Shop_Base_Potion"));

			::Safe_Release(pWeaponShop);
			::Safe_Release(pArmorShop);
			::Safe_Release(pAccShop);
			::Safe_Release(pPotionShop);

			if (pWeaponShop->GetPick() || pArmorShop->GetPick() || pAccShop->GetPick() || pPotionShop->GetPick())
				return;*/

			if (m_bType)
			{
				m_pSystem->SetRender(true);
				// 환경설정창 키기
			}
			else
			{
				if (Engine::GetSceneID() == Engine::SCENE_TOWN)
					PostQuitMessage(0);

				else if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
				{
					((CScene_Stage_Ice*)m_pScene)->SetExit(true);
					m_pParent->SetRender(false);
				}

				else if(Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
				{
					((CScene_Stage_Fire*)m_pScene)->SetExit(true);
					m_pParent->SetRender(false);
				}
			}
		}
		else
			m_bClick = false;
	}
}

void CESC_Button::SetParent(CUI* pParent)
{
	m_pParent = pParent;
}

CESC_Button* CESC_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CESC_Button*		pInstance = new CESC_Button(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CESC_Button* CESC_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParent, _bool bType, Engine::CScene* pScene, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CESC_Button*		pInstance = new CESC_Button(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParent);
	pInstance->SetType(bType);
	pInstance->SetScene(pScene);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CESC_Button::Free(void)
{
	CUI::Free();
}
