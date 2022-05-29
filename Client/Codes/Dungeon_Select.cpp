#include "stdafx.h"
#include "Dungeon_Select.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Effect.h"
#include "Player.h"
#include "Portal.h"
#include "Sound.h"

CDunGeon_Select::CDunGeon_Select(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pNCTextureCom(NULL)
	, m_pCTextureCom(NULL)
	, m_pSTextureCom(NULL)
	, m_pParent(NULL)
{
}

CDunGeon_Select::~CDunGeon_Select(void)
{

}

HRESULT CDunGeon_Select::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSizeX = 250.f;
	m_fSizeY = 80.f;

	m_fAlpha = 1.f;
	m_wSortNum = 90;
	m_bRender = false;
	m_eWinType = WINDOW_DUNGEON;

	m_bIsStatic = true;
	return S_OK;
}

void CDunGeon_Select::SetPos(_vec2 vPos)
{
	m_fX = vPos.x;
	m_fY = vPos.y;
}

HRESULT CDunGeon_Select::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN /*&& Engine::GetSceneID() != Engine::SCENE_STAGE_ICE*/)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;
	return S_OK;
}

void CDunGeon_Select::Update_Object(const _float& fTimeDelta)
{
	m_bRender = m_pParent->GetRender();
	if (m_bRender)
	{
		if (!m_bSelect && !m_bCollision)
		{
			m_fSizeX = 250.f;
			m_fSizeY = 60.f;
		}
		CUI::Update_Object(fTimeDelta);
		if (m_bType)
			PtInRect_Collision(fTimeDelta);
		m_wSortNum = m_pParent->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
		m_bSelect = false;

}

void CDunGeon_Select::Render_Object(void)
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

	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	if (m_bSelect)
	{
		m_pSTextureCom->Set_Texture(pEffect, "g_BaseTexture");
		fFontAlpha = 0.75f;
	}
	else
	{
		if (m_bCollision)
		{
			m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
			fFontAlpha = 1.f;

		}
		else
		{
			m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
			fFontAlpha = 0.75f;
		}
	}

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	if(m_eType == DUNGEON_ICE)
		Engine::Render_Font(L"Font_Select_DunGeun", L"��ȥ�� ���", &_vec2(m_fX - 30.f, m_fY - 15.f), D3DXCOLOR(1.f, 1.f, 1.f, fFontAlpha));
		// ��ȥ�ǰ��
	else if (m_eType == DUNGEON_FIRE)
		Engine::Render_Font(L"Font_Select_DunGeun", L"ȭ�꼶 �������", &_vec2(m_fX - 30.f, m_fY - 15.f), D3DXCOLOR(1.f, 1.f, 1.f, fFontAlpha));
		// ȭ�꼶 �������
}


void CDunGeon_Select::PtInRect_Collision(const _float& fTimeDelta)
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


	if (m_bCollision)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if (m_bClick)
				return;
			m_bClick = true;
			m_pSound->MyPlaySound(L"ButtonPick");
			
			CUI* pUI = ((CUI*)Engine::Find(L"Dungeon_Select_Ice"));
			pUI->SetSelect(false);
			::Safe_Release(pUI);
			pUI = ((CUI*)Engine::Find(L"Dungeon_Select_Fire"));
			pUI->SetSelect(false);
			::Safe_Release(pUI);
			pUI = ((CUI*)Engine::Find(L"Dungeon_Texture_Ice"));
			pUI->SetSelect(false);
			::Safe_Release(pUI);
			if (m_eType == DUNGEON_ICE)
			{
				pUI->SetSelect(true);
				pUI = ((CUI*)Engine::Find(L"Dungeon_Base"));
				pUI->SetWinPos(_vec2(900, 570));
				::Safe_Release(pUI);
			}
			pUI = ((CUI*)Engine::Find(L"Dungeon_Texture_Fire"));
			pUI->SetSelect(false);
			::Safe_Release(pUI);
			if (m_eType == DUNGEON_FIRE)
			{
				pUI->SetSelect(true);
				pUI = ((CUI*)Engine::Find(L"Dungeon_Base"));
				pUI->SetWinPos(_vec2(700, 0));
				::Safe_Release(pUI);
			}

			m_bSelect = true;
		}
		else
			m_bClick = false;
	}
}

HRESULT CDunGeon_Select::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"DunGeon_Select_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"DunGeon_Select_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture2", pComponent));

	m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"DunGeon_Select_S");
	pComponent = (Engine::CComponent*)m_pSTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture3", pComponent));
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

CDunGeon_Select* CDunGeon_Select::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CDunGeon_Select*		pInstance = new CDunGeon_Select(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CDunGeon_Select* CDunGeon_Select::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParent, _vec2 vPos, _bool bType, DUNGEON_ID eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CDunGeon_Select*		pInstance = new CDunGeon_Select(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParent);
	pInstance->SetType(bType);
	pInstance->SetPos(vPos);
	pInstance->SetID(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CDunGeon_Select Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDunGeon_Select::Free(void)
{

	CUI::Free();
}
