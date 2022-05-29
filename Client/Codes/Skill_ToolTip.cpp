#include "stdafx.h"
#include "Skill_ToolTip.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"

CSkill_ToolTip::CSkill_ToolTip(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pSkill(NULL)
{
	for (int i = 0; i < SKILL_END; ++i)
		m_pTextureCom[i] = NULL;
}

CSkill_ToolTip::~CSkill_ToolTip(void)
{

}

HRESULT CSkill_ToolTip::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = 0.f;
	m_fY = 0.f;

	m_fSizeX = 250.f;
	m_fSizeY = 250.f;

	m_fAlpha = 1.f;
	m_wSortNum = 8;
	m_bRender = false;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CSkill_ToolTip::Start_ForScene(void)
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

void CSkill_ToolTip::Update_Object(const _float& fTimeDelta)
{
	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);
		m_wSortNum = 8;
		if (m_eType != SKILL_END)
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
		m_eType = SKILL_END;
	}
}

void CSkill_ToolTip::Render_Object(void)
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

void CSkill_ToolTip::SetSkill(CUI_Skill* pSkill)
{
	m_pSkill = pSkill;
}

void CSkill_ToolTip::SetId(SKILL_ID eID)
{
	m_eType = eID;
}

HRESULT CSkill_ToolTip::Add_Component(void)
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
	m_pTextureCom[SKILL_RUSH] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Rush_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_RUSH];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	m_pTextureCom[SKILL_FLYDANCE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"FlyDance_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_FLYDANCE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture2", pComponent));

	m_pTextureCom[SKILL_FLEDGE_BLADE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"PledgeBlade_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_FLEDGE_BLADE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture3", pComponent));

	m_pTextureCom[SKILL_STUNPIN] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SpinTurn_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_STUNPIN];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture4", pComponent));

	m_pTextureCom[SKILL_VIOLET] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Violet_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_VIOLET];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture5", pComponent));

	m_pTextureCom[SKILL_CROSS] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Cross_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_CROSS];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture6", pComponent));

	m_pTextureCom[SKILL_SPIRALDANCE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Spirer_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_SPIRALDANCE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture7", pComponent));

	m_pTextureCom[SKILL_DARKPOLLING] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"DarkPolling_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_DARKPOLLING];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture8", pComponent));

	m_pTextureCom[SKILL_DASH] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Dash_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_DASH];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture9", pComponent));

	m_pTextureCom[SKILL_LIDDLEKICK] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"RiddleKick_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_LIDDLEKICK];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture10", pComponent));

	m_pTextureCom[SKILL_EVADEATT] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"EvadeAtt_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_EVADEATT];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture11", pComponent));

	m_pTextureCom[SKILL_TECHDOWN] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"TechDown_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_TECHDOWN];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture12", pComponent));

	m_pTextureCom[SKILL_BLARE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Blare_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_BLARE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture13", pComponent));

	m_pTextureCom[SKILL_UPATT] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"UpAtt_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_UPATT];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture14", pComponent));

	m_pTextureCom[SKILL_VIFERKICK] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Viper_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_VIFERKICK];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture15", pComponent));

	m_pTextureCom[SKILL_SPININGHEART] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Spining_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_SPININGHEART];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture16", pComponent));

	m_pTextureCom[SKILL_BLACKROSE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"BlackRose_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_BLACKROSE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture17", pComponent));

	m_pTextureCom[SKILL_SKINNING] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Rush_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_SKINNING];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture18", pComponent));

	m_pTextureCom[SKILL_TRICKEY] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Tricky_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_TRICKEY];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture19", pComponent));

	m_pTextureCom[SKILL_FLENITDANCE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Planit_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_FLENITDANCE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture20", pComponent));

	m_pTextureCom[SKILL_PLORA] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Plora_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_PLORA];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture21", pComponent));

	m_pTextureCom[SKILL_SOFTLANDING] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SoftLanding_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_SOFTLANDING];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture22", pComponent));

	m_pTextureCom[SKILL_SILUETDANCE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Siluet_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_SILUETDANCE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture23", pComponent));

	m_pTextureCom[SKILL_HILLTURN] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"HillTurn_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_HILLTURN];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture24", pComponent));

	m_pTextureCom[SKILL_URBANBREAT] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"UrbanBreak_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_URBANBREAT];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture25", pComponent));

	m_pTextureCom[SKILL_CRITICAL] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Critical_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_CRITICAL];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture26", pComponent));

	m_pTextureCom[SKILL_MAXHP] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MaxHpUp_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_MAXHP];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture27", pComponent));

	m_pTextureCom[SKILL_MAXMP] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MaxMpUp_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_MAXMP];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture28", pComponent));

	m_pTextureCom[SKILL_ATTMASTER] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"DualAtt_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_ATTMASTER];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture29", pComponent));

	m_pTextureCom[SKILL_BLOODAGIL] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"BloodAgil_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_BLOODAGIL];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture30", pComponent));

	m_pTextureCom[SKILL_DRAGONDANCE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"DragonDance_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_DRAGONDANCE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture31", pComponent));

	m_pTextureCom[SKILL_MOVINGCANNON] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MoveCannon_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_MOVINGCANNON];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture32", pComponent));

	m_pTextureCom[SKILL_GEILSHOT] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"GeilShot_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_GEILSHOT];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture33", pComponent));

	m_pTextureCom[SKILL_ESCAPE] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Escape_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_ESCAPE];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture34", pComponent));

	m_pTextureCom[SKILL_HOLDSHOT] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"HoldShot_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_HOLDSHOT];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture35", pComponent));

	m_pTextureCom[SKILL_SHELLBUSTER] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"ShellBuster_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_SHELLBUSTER];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture36", pComponent));

	m_pTextureCom[SKILL_JUMPDOWNKICK] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"JumpKick_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_JUMPDOWNKICK];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture37", pComponent));

	m_pTextureCom[SKILL_DOWNKICK] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"DownKick_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_DOWNKICK];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture38", pComponent));

	m_pTextureCom[SKILL_GRAVITY] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Gravity_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_GRAVITY];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture39", pComponent));

	m_pTextureCom[SKILL_FIREM] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"FireM_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_FIREM];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture40", pComponent));

	m_pTextureCom[SKILL_UPPERKICK] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"UpperKick_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_UPPERKICK];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture41", pComponent));

	m_pTextureCom[SKILL_SHOTJUMP] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"ShotJump_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_SHOTJUMP];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture42", pComponent));

	m_pTextureCom[SKILL_LOCKON] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"LockOn_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_LOCKON];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture43", pComponent));

	m_pTextureCom[SKILL_SPINCRASH] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SpinCrash_ToolTip");
	pComponent = (Engine::CComponent*)m_pTextureCom[SKILL_SPINCRASH];
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture44", pComponent));

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

CSkill_ToolTip* CSkill_ToolTip::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSkill_ToolTip*		pInstance = new CSkill_ToolTip(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_ToolTip::Free(void)
{
	CUI::Free();
}
