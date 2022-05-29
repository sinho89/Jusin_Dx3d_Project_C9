#include "stdafx.h"
#include "UI_MaxHpUp.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CUI_MaxHpUp::CUI_MaxHpUp(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI_Skill(pGraphicDev, wstrName, eState, bIsActive)
{

}

CUI_MaxHpUp::~CUI_MaxHpUp(void)
{

}

HRESULT CUI_MaxHpUp::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pSkillInfo = new SKILL_INFO;
	ZeroMemory(m_pSkillInfo, NULL);

	SetInfo();

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;

	m_fAlpha = 1.f;
	m_iIndex = -1;

	return S_OK;
}

HRESULT CUI_MaxHpUp::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;
	return S_OK;
}

void CUI_MaxHpUp::Update_Object(const _float& fTimeDelta)
{
	if (m_bFade)
	{
		m_fFadeTime += fTimeDelta;
	}
	else
	{
		POINT		ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		m_bButtonCollision = m_pCalCom->Collision_ByMouse(WINSIZEX
			, WINSIZEY
			, m_fX
			, m_fY
			, m_fSizeX
			, m_fSizeY
			, ptMouse);

		if (m_bButtonCollision)
		{
			if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
			{
				if (m_bClick)
					return;
				m_bClick = true;

				m_bFade = true;
				m_fFadeTime = 0.f;
			}
			else
				m_bClick = false;
		}
	}

	if (m_fFadeTime >= 1.f)
	{
		m_fFadeTime = 0.f;
		m_bFade = false;
	}

	if(m_pParentWindow->GetSheetSelect()
		&& m_pParentWindow->GetRender()
		&& m_pParentWindow->GetSlotLevel() == 1)
	{		
		CUI::Update_Object(fTimeDelta);
		m_wSortNum = m_pParentWindow->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
}

void CUI_MaxHpUp::Render_Object(void)
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
	if (m_bFade)
		pEffect->BeginPass(7);
	else
		pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 
}

void CUI_MaxHpUp::SetId(SKILL_ID Id)
{
	m_eSkillID = Id;
}
void CUI_MaxHpUp::SetType(SKILL_TYPE Id)
{
	m_eSkillType = Id;
}
void CUI_MaxHpUp::SetInfo()
{
	m_pSkillInfo->m_eSkillId = m_eSkillID;
	m_pSkillInfo->m_eSkillType = m_eSkillType;
	m_pSkillInfo->m_iLevel = 5;
}


HRESULT CUI_MaxHpUp::Add_Component(void)
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


	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MaxHpUp");
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

CUI_MaxHpUp* CUI_MaxHpUp::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_MaxHpUp*		pInstance = new CUI_MaxHpUp(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Rush Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CUI_MaxHpUp* CUI_MaxHpUp::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, SKILL_ID eID, SKILL_TYPE eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_MaxHpUp*		pInstance = new CUI_MaxHpUp(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetId(eID);
	pInstance->SetType(eType);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Rush Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MaxHpUp::Free(void)
{
	CUI_Skill::Free();
}

