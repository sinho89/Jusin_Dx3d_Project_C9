#include "stdafx.h"
#include "UI_SpirerDance.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CUI_SpirerDance::CUI_SpirerDance(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI_Skill(pGraphicDev, wstrName, eState, bIsActive)
{

}

CUI_SpirerDance::~CUI_SpirerDance(void)
{

}

HRESULT CUI_SpirerDance::Ready_Object(void)
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

HRESULT CUI_SpirerDance::Start_ForScene(void)
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

void CUI_SpirerDance::Update_Object(const _float& fTimeDelta)
{
	if (m_pSkillInfo->m_bActive)
	{
		m_fAlpha = 0.5f;
		m_pSkillInfo->m_fActiveTime += fTimeDelta;

		if (m_pSkillInfo->m_fActiveTime >= 5.f)
		{
			m_fAlpha = 1.f;
			m_pSkillInfo->m_bActive = false;
			m_pSkillInfo->m_fActiveTime = 0.f;
		}
	}
	if(m_iIndex == -1)
	{
		if(m_pParentWindow->GetSheetSelect()
			&& m_pParentWindow->GetRender())
		{		
			CUI::Update_Object(fTimeDelta);
			if(!m_bMousePick)
				m_wSortNum = m_pParentWindow->GetSortNum();
			else
				m_wSortNum = 10;
			m_pRendererCom->Add_WindowUI(m_wSortNum, this);
			D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		}
	}
	else
	{
		CUI::Update_Object(fTimeDelta);
		if(m_bMousePick)
			m_wSortNum = 10;
		else
			m_wSortNum = 0;
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

	}
}

void CUI_SpirerDance::Render_Object(void)
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
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 
}

void CUI_SpirerDance::SetId(SKILL_ID Id)
{
	m_eSkillID = Id;
}
void CUI_SpirerDance::SetType(SKILL_TYPE Id)
{
	m_eSkillType = Id;
}
void CUI_SpirerDance::SetInfo()
{
	m_pSkillInfo->m_eSkillId = m_eSkillID;
	m_pSkillInfo->m_eSkillType = m_eSkillType;
	m_pSkillInfo->m_iLevel = 5;
	m_pSkillInfo->m_bActive = false;
	m_pSkillInfo->m_fActiveTime = 0.f;
}


HRESULT CUI_SpirerDance::Add_Component(void)
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


	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SpirerDance");
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

CUI_SpirerDance* CUI_SpirerDance::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_SpirerDance*		pInstance = new CUI_SpirerDance(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CUI_Rush Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CUI_SpirerDance* CUI_SpirerDance::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, SKILL_ID eID, SKILL_TYPE eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CUI_SpirerDance*		pInstance = new CUI_SpirerDance(pGraphicDev, wstrName, eState, bIsActive);

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

void CUI_SpirerDance::Free(void)
{
	CUI_Skill::Free();
}

