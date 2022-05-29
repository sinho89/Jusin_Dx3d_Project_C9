#include "stdafx.h"
#include "Compare_ToolTip.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CCompare_ToolTip::CCompare_ToolTip(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pItem(NULL)
{
	for (int i = 0; i < TOOLTIP_END; ++i)
		m_pTextureCom[i] = NULL;
}

CCompare_ToolTip::~CCompare_ToolTip(void)
{

}

HRESULT CCompare_ToolTip::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = 0.f;
	m_fY = 0.f;

	m_fSizeX = 250.f;
	m_fSizeY = 400.f;

	m_fAlpha = 1.f;
	m_wSortNum = 8;
	m_bRender = false;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CCompare_ToolTip::Start_ForScene(void)
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

void CCompare_ToolTip::Update_Object(const _float& fTimeDelta)
{
	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);
		m_wSortNum = 8;
		if (m_eType != TOOLTIP_END)
			m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		m_fX = (WINSIZEX >> 1) + 350.f;
		m_fY = (WINSIZEY >> 1) - 50.f;

		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
		m_eType = TOOLTIP_END;
	}
}

void CCompare_ToolTip::Render_Object(void)
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
	matWorld._33 = 0.001f;
	matWorld._41 = m_fX - WINSIZEX * 0.5f;
	matWorld._42 = -m_fY + WINSIZEY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom[m_eType]->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CCompare_ToolTip::SetItem(CUI_Item* pItem)
{
	m_pItem = pItem;
}

void CCompare_ToolTip::SetId(TOOLTIP_ID eID)
{
	m_eType = eID;
}

HRESULT CCompare_ToolTip::Add_Component(void)
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
	m_pTextureCom[TOOLTIP_WEAPON_BLUE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Weapon_Blue_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_WEAPON_BLUE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	m_pTextureCom[TOOLTIP_WEAPON_RED] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Weapon_Red_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_WEAPON_RED];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture2", pComponent));

	m_pTextureCom[TOOLTIP_S_WEAPON_BLUE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Weapon_S_Blue_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_WEAPON_BLUE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture0", pComponent));

	m_pTextureCom[TOOLTIP_S_WEAPON_RED] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Weapon_S_Red_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_WEAPON_RED];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture00", pComponent));

	m_pTextureCom[TOOLTIP_S_HELMET] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"S_Helmet_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_S_HELMET];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture03", pComponent));

	m_pTextureCom[TOOLTIP_S_ARMOR] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"S_Armor_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_S_ARMOR];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture04", pComponent));

	m_pTextureCom[TOOLTIP_S_SHORDER] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"S_Shorder_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_S_SHORDER];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture05", pComponent));

	m_pTextureCom[TOOLTIP_S_LEG] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"S_Leg_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_S_LEG];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture6", pComponent));

	m_pTextureCom[TOOLTIP_S_BOOTS] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"S_Boots_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_S_BOOTS];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture07", pComponent));

	m_pTextureCom[TOOLTIP_S_GLOVE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"S_Glove_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_S_GLOVE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture08", pComponent));

	m_pTextureCom[TOOLTIP_HELMET] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Helmet_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_HELMET];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture3", pComponent));

	m_pTextureCom[TOOLTIP_ARMOR] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Armor_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_ARMOR];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture4", pComponent));

	m_pTextureCom[TOOLTIP_SHORDER] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Shorder_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_SHORDER];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture5", pComponent));

	m_pTextureCom[TOOLTIP_LEG] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Leg_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_LEG];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture6", pComponent));

	m_pTextureCom[TOOLTIP_BOOTS] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Boots_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_BOOTS];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture7", pComponent));

	m_pTextureCom[TOOLTIP_GLOVE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Glove_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_GLOVE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture8", pComponent));

	m_pTextureCom[TOOLTIP_AMULET] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Amulet_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_AMULET];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture9", pComponent));

	m_pTextureCom[TOOLTIP_ARMLET] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Armlet_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_ARMLET];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture10", pComponent));

	m_pTextureCom[TOOLTIP_BELT] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Belt_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_BELT];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture11", pComponent));

	m_pTextureCom[TOOLTIP_EARING] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Earing_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_EARING];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture12", pComponent));

	m_pTextureCom[TOOLTIP_RING] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Ring1_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_RING];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture13", pComponent));

	m_pTextureCom[TOOLTIP_RING2] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Ring2_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_RING2];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture14", pComponent));

	m_pTextureCom[TOOLTIP_HP_POTION] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"HP_Potion_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_HP_POTION];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture15", pComponent));

	m_pTextureCom[TOOLTIP_MP_POTION] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MP_Potion_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_MP_POTION];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture16", pComponent));


	m_pTextureCom[TOOLTIP_LEVELUP_ITEM] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Level_Potion_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[TOOLTIP_LEVELUP_ITEM];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture17", pComponent));

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

CCompare_ToolTip* CCompare_ToolTip::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCompare_ToolTip*		pInstance = new CCompare_ToolTip(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CCompare_ToolTip::Free(void)
{
	CUI::Free();
}
