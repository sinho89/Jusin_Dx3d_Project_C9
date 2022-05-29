#include "stdafx.h"
#include "Skill_Sheet.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_Item.h"
#include "Inven_Block.h"
#include "Cursor.h"
#include "ToolTip.h"
#include "Skill_Block.h"
#include "UI_Skill.h"
#include "UI_Rush.h"
#include "UI_FlyDance.h"
#include "UI_FladgeBlade.h"
#include "UI_SpinTurn.h"
#include "UI_Violet.h"
#include "UI_CrossBrits.h"
#include "UI_SpirerDance.h"
#include "UI_DarkPolling.h"
#include "UI_BlackRose.h"
#include "Skill_ToolTip.h"
#include "Skill_Avi.h"
#include "UI_DragonDance.h"
#include "UI_SpinCrash.h"
#include "UI_LockOn.h"
#include "UI_ShotJump.h"
#include "UI_UpperKick.h"
#include "UI_FireM.h"
#include "UI_Gravity.h"
#include "Sound.h"

CSkill_Sheet::CSkill_Sheet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pNCTextureCom(NULL)
,m_pCTextureCom(NULL)
,m_pSTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_pCapySkill(NULL)
, m_pToolTip(NULL)
, m_pAvi(NULL)
{

}

CSkill_Sheet::~CSkill_Sheet(void)
{

}

HRESULT CSkill_Sheet::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = m_pParentWindow->GetInfo();

	m_fSizeX = 130.f;
	m_fSizeY = 40.f;

	m_fAlpha = 1.f;
	m_eWinType = WINDOW_SKILL;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CSkill_Sheet::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	m_pToolTip = ((CSkill_ToolTip*)((CUI*)Engine::Find(L"Skill_ToolTip")));
	m_pAvi = ((CSkill_Avi*)((CUI*)Engine::Find(L"Skill_Avi")));

	::Safe_Release(m_pAvi);
	::Safe_Release(m_pToolTip);

	return S_OK;
}

void CSkill_Sheet::Update_Object(const _float& fTimeDelta)
{
	m_bRender = m_pParentWindow->GetRender();
	DeleteSkill();

	if(m_bRender)
	{		
		CUI::Update_Object(fTimeDelta);
		m_wSortNum = m_pParentWindow->GetSortNum();
		PtInRect_Collision(fTimeDelta);
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		if(m_bSheetSelect && m_iIndex == 0)
			SkillPick();

		if(m_iIndex == 1 && !m_bType)
		{
			if(!m_bSheetSelect)
				m_iSlotLevel = 1;
		}

		RenderToolTip();
	}
	else
	{
		switch(m_iIndex)
		{
		case 0:
			m_bSheetSelect = true;
			break;
		case 1:
			if(m_bType == false)
				m_iSlotLevel = 1;
			m_bSheetSelect = false;
			break;
		case 2:
			m_bSheetSelect = false;
			break;
		}

		if(m_pCursor != NULL)
		{
			if(((CCursor*)m_pCursor)->GetSkill() != NULL && ((CCursor*)m_pCursor)->GetSkill()->GetMain() == false)
			{
				CUI_Skill* pSkill = ((CCursor*)m_pCursor)->GetSkill();
				((CCursor*)m_pCursor)->GetSkill()->SetAlive(false);
				Engine::Safe_Release(pSkill);
				((CCursor*)m_pCursor)->SetSkill(NULL);
			}
		}

		m_pToolTip->SetRender(false);
		m_pAvi->SetRender(false);
	}

}

void CSkill_Sheet::Render_Object(void)
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

	if(m_bSheetSelect)
		m_pSTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	else
		m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");


	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 

	switch(m_iIndex)
	{
	case 0:
		if(m_bSheetSelect)
		{
			_tchar		szInfo[128] = L"";
			wsprintf(szInfo, L"%d / 1", m_iSlotLevel);
			Engine::Render_Font(L"Font_Main_Data",  szInfo, &_vec2(*m_pParentWindow->GetfX() - 15.f, *m_pParentWindow->GetfY() + 200.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		Engine::Render_Font(L"Font_Main_Button", L"액　　션", &_vec2(m_fX - 27.5f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 1:
		if(m_bSheetSelect)
		{
			_tchar		szInfo[128] = L"";
			if(m_bType == false)
				wsprintf(szInfo, L"%d / 2", m_iSlotLevel);
			else
				wsprintf(szInfo, L"%d / 1", m_iSlotLevel);
			Engine::Render_Font(L"Font_Main_Data",  szInfo, &_vec2(*m_pParentWindow->GetfX() - 15.f, *m_pParentWindow->GetfY() + 200.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		Engine::Render_Font(L"Font_Main_Button", L"커맨드 액션", &_vec2(m_fX - 32.5f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	case 2:
		if(m_bSheetSelect)
		{
			_tchar		szInfo[128] = L"";
			wsprintf(szInfo, L"%d / 1", m_iSlotLevel);
			Engine::Render_Font(L"Font_Main_Data",  szInfo, &_vec2(*m_pParentWindow->GetfX() - 15.f, *m_pParentWindow->GetfY() + 200.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		}
		Engine::Render_Font(L"Font_Main_Button", L"패 시 브", &_vec2(m_fX - 22.5f, m_fY - 8.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
		break;
	}
}


HRESULT CSkill_Sheet::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pSTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Inven_Sheet_S");
	pComponent = (Engine::CComponent*)m_pSTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Click", pComponent));

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

	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");

	return S_OK;
}
void CSkill_Sheet::PtInRect_Collision(const _float& fTimeDelta)
{
	_vec4 vInfo = m_pParentWindow->GetInfo();

	switch(m_iIndex)
	{
	case 0:
		m_fX = vInfo.x - 130.f;
		m_fY = vInfo.y - 225.f;
		break;
	case 1:
		m_fX = vInfo.x - 0.f;
		m_fY = vInfo.y - 225.f;
		break;
	case 2:
		m_fX = vInfo.x + 130.f;
		m_fY = vInfo.y - 225.f;
		break;
	}

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

	if(m_bButtonCollision)
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			m_pCursor = ((CUI*)Engine::Find(L"Cursor"));
			if(((CCursor*)m_pCursor)->GetSkill() != NULL || ((CCursor*)m_pCursor)->GetItem() != NULL)
			{	
				::Safe_Release(m_pCursor);
				return;
			}
			::Safe_Release(m_pCursor);

			CUI*	pPartitionBase = ((CUI*)Engine::Find(L"Partition_Base"));
			if (pPartitionBase->GetRender())
			{
				::Safe_Release(pPartitionBase);
				return;
			}
			::Safe_Release(pPartitionBase);

			if(m_bClick)
				return;
			m_bClick = true;
			m_pSound->MyPlaySound(L"ButtonPick");

			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Window")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Window")->end();

			for (; iter != iter_end; ++iter)
			{
				if (((CUI*)(*iter))->GetWinType() == WINDOW_SKILL)
					((CUI*)(*iter))->SetSheetSelect(false);
			}

			m_bSheetSelect = true;
		}
		else
			m_bClick = false;
	}


	if(m_bSheetSelect)
		m_fAlpha = 1.f;
	else
		m_fAlpha = 0.5f;

}

void CSkill_Sheet::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}


void CSkill_Sheet::SkillPick(void)
{
	if(m_bSheetSelect)
	{
		for(size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if(m_vecBlock[i]->GetBlockPick())
			{ // Item Picking
				m_pCursor = ((CUI*)Engine::Find(L"Cursor"));
				if(((CCursor*)m_pCursor)->GetSkill() != NULL)
				{
					if (m_pCapySkill)
					{
						m_pSound->MyPlaySound(L"ButtonPick");
						Engine::Safe_Release(m_pCapySkill);
						m_pCapySkill = NULL;
						((CCursor*)m_pCursor)->SetSkill(NULL);
					}
					m_vecBlock[i]->SetBlockPick(false);
					::Safe_Release(m_pCursor);
					return;
				}
				if(((CCursor*)m_pCursor)->GetSkill() == NULL && ((CSkill_Block*)m_vecBlock[i])->GetSkill() != NULL) // 빼기
				{
					switch(((CSkill_Block*)m_vecBlock[i])->GetSkill()->Get_Skill_Info()->m_eSkillId)
					{
					case SKILL_RUSH:
						m_pCapySkill = CUI_Rush::Create(m_pGraphicDev, L"CapyRush", this, SKILL_RUSH, SKILL_TYPE_ACTION);
						break;

					case SKILL_FLYDANCE:
						m_pCapySkill = CUI_FlyDance::Create(m_pGraphicDev, L"CapyFly", this, SKILL_FLYDANCE, SKILL_TYPE_ACTION);
						break;

					case SKILL_FLEDGE_BLADE:
						m_pCapySkill = CUI_FladgeBlade::Create(m_pGraphicDev, L"CapyFladge", this, SKILL_FLEDGE_BLADE, SKILL_TYPE_ACTION);
						break;

					case SKILL_STUNPIN:
						m_pCapySkill = CUI_SpinTurn::Create(m_pGraphicDev, L"CapySpin", this, SKILL_STUNPIN, SKILL_TYPE_ACTION);
						break;

					case SKILL_VIOLET:
						m_pCapySkill = CUI_Violet::Create(m_pGraphicDev, L"CapyViolet", this, SKILL_VIOLET, SKILL_TYPE_ACTION);
						break;

					case SKILL_CROSS:
						m_pCapySkill = CUI_CrossBrits::Create(m_pGraphicDev, L"CapyCross", this, SKILL_CROSS, SKILL_TYPE_ACTION);
						break;

					case SKILL_SPIRALDANCE:
						m_pCapySkill = CUI_SpirerDance::Create(m_pGraphicDev, L"CapySpirer", this, SKILL_SPIRALDANCE, SKILL_TYPE_ACTION);
						break;

					case SKILL_DARKPOLLING:
						m_pCapySkill = CUI_DarkPolling::Create(m_pGraphicDev, L"CapyDarkPolling", this, SKILL_DARKPOLLING, SKILL_TYPE_ACTION);
						break;

					case SKILL_BLACKROSE:
						m_pCapySkill = CUI_BlackRose::Create(m_pGraphicDev, L"CapyBlackRose", this, SKILL_BLACKROSE, SKILL_TYPE_ACTION);
						break;

					case SKILL_DRAGONDANCE:
						m_pCapySkill = CUI_DragonDance::Create(m_pGraphicDev, L"CapyDragonDance", this, SKILL_DRAGONDANCE, SKILL_TYPE_ACTION);
						break;

					case SKILL_SPINCRASH:
						m_pCapySkill = CUI_SpinCrash::Create(m_pGraphicDev, L"SpinCrash", this, SKILL_SPINCRASH, SKILL_TYPE_ACTION);
						break;

					case SKILL_LOCKON:
						m_pCapySkill = CUI_LockOn::Create(m_pGraphicDev, L"LockOn", this, SKILL_LOCKON, SKILL_TYPE_ACTION);
						break;

					case SKILL_SHOTJUMP:
						m_pCapySkill = CUI_ShotJump::Create(m_pGraphicDev, L"ShotJump", this, SKILL_SHOTJUMP, SKILL_TYPE_ACTION);
						break;

					case SKILL_UPPERKICK:
						m_pCapySkill = CUI_UpperKick::Create(m_pGraphicDev, L"UpperKick", this, SKILL_UPPERKICK, SKILL_TYPE_ACTION);
						break;

					case SKILL_FIREM:
						m_pCapySkill = CUI_FireM::Create(m_pGraphicDev, L"FireM", this, SKILL_FIREM, SKILL_TYPE_ACTION);
						break;

					case SKILL_GRAVITY:
						m_pCapySkill = CUI_Gravity::Create(m_pGraphicDev, L"Gravity", this, SKILL_GRAVITY, SKILL_TYPE_ACTION);
						break;
					}

					m_pSound->MyPlaySound(L"ButtonPick");
					((CCursor*)m_pCursor)->SetSkill(m_pCapySkill);
					(((CCursor*)m_pCursor)->GetSkill())->SetMousePick(true);
					m_vecBlock[i]->SetBlockPick(false);
					::Safe_Release(m_pCursor);
					break;
				}
				::Safe_Release(m_pCursor);
			}
		}
	}
}

void CSkill_Sheet::RenderToolTip(void)
{
	if (m_bSheetSelect)
	{
		m_pCursor = ((CUI*)Engine::Find(L"Cursor"));

		for (size_t i = 0; i < m_vecBlock.size(); ++i)
		{
			if (m_vecBlock[i]->GetButtonCol() && !((CCursor*)m_pCursor)->GetSkill() && ((CSkill_Block*)m_vecBlock[i])->GetSkill())
			{

				switch (((CSkill_Block*)m_vecBlock[i])->GetSkill()->Get_Skill_Info()->m_eSkillId)
				{
				case SKILL_RUSH:
					m_pToolTip->SetId(SKILL_RUSH);
					m_pAvi->SetType(SKILL_RUSH);
					m_pAvi->Play();
					break;
				case SKILL_FLYDANCE:
					m_pToolTip->SetId(SKILL_FLYDANCE);
					m_pAvi->SetType(SKILL_FLYDANCE);
					m_pAvi->Play();
					break;
				case SKILL_FLEDGE_BLADE:
					m_pToolTip->SetId(SKILL_FLEDGE_BLADE);
					m_pAvi->SetType(SKILL_FLEDGE_BLADE);
					m_pAvi->Play();
					break;
				case SKILL_STUNPIN:
					m_pToolTip->SetId(SKILL_STUNPIN);
					m_pAvi->SetType(SKILL_STUNPIN);
					m_pAvi->Play();
					break;
				case SKILL_VIOLET:
					m_pToolTip->SetId(SKILL_VIOLET);
					m_pAvi->SetType(SKILL_VIOLET);
					m_pAvi->Play();
					break;
				case SKILL_CROSS:
					m_pToolTip->SetId(SKILL_CROSS);
					m_pAvi->SetType(SKILL_CROSS);
					m_pAvi->Play();
					break;
				case SKILL_SPIRALDANCE:
					m_pToolTip->SetId(SKILL_SPIRALDANCE);
					m_pAvi->SetType(SKILL_SPIRALDANCE);
					m_pAvi->Play();
					break;
				case SKILL_DARKPOLLING:
					m_pToolTip->SetId(SKILL_DARKPOLLING);
					m_pAvi->SetType(SKILL_DARKPOLLING);
					m_pAvi->Play();
					break;
				case SKILL_DASH:
					m_pToolTip->SetId(SKILL_DASH);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_LIDDLEKICK:
					m_pToolTip->SetId(SKILL_LIDDLEKICK);
					m_pAvi->SetType(SKILL_LIDDLEKICK);
					m_pAvi->Play();
					break;
				case SKILL_EVADEATT:
					m_pToolTip->SetId(SKILL_EVADEATT);
					m_pAvi->SetType(SKILL_EVADEATT);
					m_pAvi->Play();
					break;
				case SKILL_TECHDOWN:
					m_pToolTip->SetId(SKILL_TECHDOWN);
					m_pAvi->SetType(SKILL_TECHDOWN);
					m_pAvi->Play();
					break;
				case SKILL_BLARE:
					m_pToolTip->SetId(SKILL_BLARE);
					m_pAvi->SetType(SKILL_BLARE);
					m_pAvi->Play();
					break;
				case SKILL_UPATT:
					m_pToolTip->SetId(SKILL_UPATT);
					m_pAvi->SetType(SKILL_UPATT);
					m_pAvi->Play();
					break;
				case SKILL_VIFERKICK:
					m_pToolTip->SetId(SKILL_VIFERKICK);
					m_pAvi->SetType(SKILL_VIFERKICK);
					m_pAvi->Play();
					break;
				case SKILL_SPININGHEART:
					m_pToolTip->SetId(SKILL_SPININGHEART);
					m_pAvi->SetType(SKILL_SPININGHEART);
					m_pAvi->Play();
					break;
				case SKILL_BLACKROSE:
					m_pToolTip->SetId(SKILL_BLACKROSE);
					m_pAvi->SetType(SKILL_BLACKROSE);
					m_pAvi->Play();
					break;
				case SKILL_TRICKEY:
					m_pToolTip->SetId(SKILL_TRICKEY);
					m_pAvi->SetType(SKILL_TRICKEY);
					m_pAvi->Play();
					break;
				case SKILL_FLENITDANCE:
					m_pToolTip->SetId(SKILL_FLENITDANCE);
					m_pAvi->SetType(SKILL_FLENITDANCE);
					m_pAvi->Play();
					break;
				case SKILL_PLORA:
					m_pToolTip->SetId(SKILL_PLORA);
					m_pAvi->SetType(SKILL_PLORA);
					m_pAvi->Play();
					break;
				case SKILL_SOFTLANDING:
					m_pToolTip->SetId(SKILL_SOFTLANDING);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_SILUETDANCE:
					m_pToolTip->SetId(SKILL_SILUETDANCE);
					m_pAvi->SetType(SKILL_SILUETDANCE);
					m_pAvi->Play();
					break;
				case SKILL_HILLTURN:
					m_pToolTip->SetId(SKILL_HILLTURN);
					m_pAvi->SetType(SKILL_HILLTURN);
					m_pAvi->Play();
					break;
				case SKILL_URBANBREAT:
					m_pToolTip->SetId(SKILL_URBANBREAT);
					m_pAvi->SetType(SKILL_URBANBREAT);
					m_pAvi->Play();
					break;
				case SKILL_MAXHP:
					m_pToolTip->SetId(SKILL_MAXHP);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_MAXMP:
					m_pToolTip->SetId(SKILL_MAXMP);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_CRITICAL:
					m_pToolTip->SetId(SKILL_CRITICAL);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_ATTMASTER:
					m_pToolTip->SetId(SKILL_ATTMASTER);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_BLOODAGIL:
					m_pToolTip->SetId(SKILL_BLOODAGIL);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_DRAGONDANCE:
					m_pToolTip->SetId(SKILL_DRAGONDANCE);
					m_pAvi->SetType(SKILL_DRAGONDANCE);
					m_pAvi->Play();
					break;
				case SKILL_SPINCRASH:
					m_pToolTip->SetId(SKILL_SPINCRASH);
					m_pAvi->SetType(SKILL_SPINCRASH);
					m_pAvi->Play();
					break;
				case SKILL_LOCKON:
					m_pToolTip->SetId(SKILL_LOCKON);
					m_pAvi->SetType(SKILL_LOCKON);
					m_pAvi->Play();
					break;
				case SKILL_SHOTJUMP:
					m_pToolTip->SetId(SKILL_SHOTJUMP);
					m_pAvi->SetType(SKILL_SHOTJUMP);
					m_pAvi->Play();
					break;
				case SKILL_UPPERKICK:
					m_pToolTip->SetId(SKILL_UPPERKICK);
					m_pAvi->SetType(SKILL_UPPERKICK);
					m_pAvi->Play();
					break;
				case SKILL_FIREM:
					m_pToolTip->SetId(SKILL_FIREM);
					m_pAvi->SetType(SKILL_FIREM);
					m_pAvi->Play();
					break;
				case SKILL_DOWNKICK:
					m_pToolTip->SetId(SKILL_DOWNKICK);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_JUMPDOWNKICK:
					m_pToolTip->SetId(SKILL_JUMPDOWNKICK);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_SHELLBUSTER:
					m_pToolTip->SetId(SKILL_SHELLBUSTER);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_HOLDSHOT:
					m_pToolTip->SetId(SKILL_HOLDSHOT);
					m_pAvi->SetType(SKILL_HOLDSHOT);
					m_pAvi->Play();
					break;
				case SKILL_GRAVITY:
					m_pToolTip->SetId(SKILL_GRAVITY);
					m_pAvi->SetType(SKILL_GRAVITY);
					m_pAvi->Play();
					break;
				case SKILL_ESCAPE:
					m_pToolTip->SetId(SKILL_ESCAPE);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_GEILSHOT:
					m_pToolTip->SetId(SKILL_GEILSHOT);
					m_pAvi->SetType(SKILL_NONE);
					break;
				case SKILL_MOVINGCANNON:
					m_pToolTip->SetId(SKILL_MOVINGCANNON);
					m_pAvi->SetType(SKILL_MOVINGCANNON);
					m_pAvi->Play();
					break;
				}


				if (m_pParentWindow->GetFoward())
				{
					m_pToolTip->SetRender(true);
					m_pAvi->SetRender(true);
				}


				if (*m_vecBlock[i]->GetfY() <= (WINSIZEY >> 1))
				{
					if (*m_vecBlock[i]->GetfX() > (WINSIZEX >> 1))
					{
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 100.f, m_vecBlock[i]->GetInfo().y + 150.f));
						m_pAvi->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 100.f, m_vecBlock[i]->GetInfo().y + 350.f));

					}
					else if (*m_vecBlock[i]->GetfX() <= (WINSIZEX >> 1))
					{
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 100.f, m_vecBlock[i]->GetInfo().y + 150.f));
						m_pAvi->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 100.f, m_vecBlock[i]->GetInfo().y + 350.f));

					}
				}
				else if (*m_vecBlock[i]->GetfY() > (WINSIZEY >> 1))
				{
					if (*m_vecBlock[i]->GetfX() > (WINSIZEX >> 1))
					{
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 100.f, m_vecBlock[i]->GetInfo().y - 150.f));
						m_pAvi->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x - 100.f, m_vecBlock[i]->GetInfo().y - 350.f));

					}
					else if (*m_vecBlock[i]->GetfX() <= (WINSIZEX >> 1))
					{
						m_pToolTip->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 100.f, m_vecBlock[i]->GetInfo().y - 150.f));
						m_pAvi->SetWinPos(_vec2(m_vecBlock[i]->GetInfo().x + 100.f, m_vecBlock[i]->GetInfo().y - 350.f));

					}
				}

				::Safe_Release(m_pCursor);
				return;
			}
		}

		m_pToolTip->SetRender(false);
		m_pAvi->SetRender(false);
		::Safe_Release(m_pCursor);
	}
}


void CSkill_Sheet::AddSheet(_uint iIndex, CUI* pUI)
{
	if(m_iIndex == iIndex)
	{
		if(pUI == NULL)
			return;

		m_vecBlock.push_back(pUI);
	}	
}

void CSkill_Sheet::AddSkill(_uint iSheet, _uint iIndex, CUI_Skill* pSkill)
{
	if(m_iIndex == iSheet)
	{
		if(m_vecBlock[iIndex] == NULL)
			return;
		((CSkill_Block*)m_vecBlock[iIndex])->SetSkill(pSkill);
	}
}

void CSkill_Sheet::DeleteSkill()
{
	if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
	{
		if (m_bKeyBoard)
			return;
		m_bKeyBoard = true;

		if (m_bSheetSelect)
		{
			m_pCursor = ((CUI*)Engine::Find(L"Cursor"));
			CUI* pMain = ((CUI*)Engine::Find(L"Main_Base"));

			if (((CCursor*)m_pCursor)->GetSkill() != NULL && !pMain->GetCollision())
			{
					m_pCapySkill = ((CCursor*)m_pCursor)->GetSkill();
					Engine::Safe_Release(m_pCapySkill);
					m_pCapySkill = NULL;
					((CCursor*)m_pCursor)->SetSkill(NULL);
				
				::Safe_Release(m_pCursor);
				::Safe_Release(pMain);
				return;
			}

			::Safe_Release(m_pCursor);
			::Safe_Release(pMain);
		}

	}
	else
		m_bKeyBoard = false;
}

vector<CUI*>* CSkill_Sheet::GetVecPage()
{
	return &m_vecBlock;
}

CSkill_Sheet* CSkill_Sheet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSkill_Sheet*		pInstance = new CSkill_Sheet(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CSkill_Sheet* CSkill_Sheet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, _uint iIndex, _bool bType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSkill_Sheet*		pInstance = new CSkill_Sheet(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetIndex(iIndex);
	pInstance->SetType(bType);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Sheet::Free(void)
{
	//::Safe_Release(m_pParentWindow);
	CUI::Free();
}
