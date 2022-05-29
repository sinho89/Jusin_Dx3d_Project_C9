#include "stdafx.h"
#include "Main_Base.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Cursor.h"
#include "Skill_Block.h"
#include "UI_Skill.h"
#include "UI_Item.h"
#include "Main_Block.h"
#include "Partition_Base.h"
#include "Fade_Manager.h"
#include "Sound.h"

CMain_Base::CMain_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
{

}

CMain_Base::~CMain_Base(void)
{

}

HRESULT CMain_Base::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1) + 412.5f;

	m_fSizeX = WINSIZEX - 550;
	m_fSizeY = 75.f;

	m_fAlpha = 1.f;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CMain_Base::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
		m_bIsActive = false;
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	return S_OK;
}

void CMain_Base::Update_Object(const _float& fTimeDelta)
{
	if (CFade_Manager::GetInstance()->GetFading() == true)
		return;
	CUI::Update_Object(fTimeDelta);

	MainPick();
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	PtInRect_Collision(fTimeDelta);

}

void CMain_Base::Render_Object(void)
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

	_tchar		szInfo[128] = L"";
	wsprintf(szInfo, L"%d", m_iSlotLevel);
		Engine::Render_Font(L"Font_Main_Slot",  szInfo, &_vec2(m_fX + 267, m_fY - 15.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));
}


HRESULT CMain_Base::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Main_Base");
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

	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");


	return S_OK;
}

void CMain_Base::AddVecBlock(_uint iIndex, CUI* pUI)
{
	m_vecBlock.push_back(pUI);
	pUI->SetIndex(iIndex);
}

void CMain_Base::MainPick()
{
	for(size_t i = 0; i < m_vecBlock.size(); ++i)
	{
		if(m_vecBlock[i]->GetBlockPick())
		{
			m_pCursor = ((CUI*)Engine::Find(L"Cursor"));

			if(((CCursor*)m_pCursor)->GetSkill() != NULL && ((CCursor*)m_pCursor)->GetItem() == NULL) // 스킬들었을때
			{
				if(((CMain_Block*)m_vecBlock[i])->GetSkill() == NULL && ((CMain_Block*)m_vecBlock[i])->GetItem() == NULL) // 스킬놓기
				{
					((CMain_Block*)m_vecBlock[i])->SetSkill(((CCursor*)m_pCursor)->GetSkill());
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetMousePick(false);
					((CCursor*)m_pCursor)->SetSkill(NULL);
					m_vecBlock[i]->SetBlockPick(false);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetIndex(i);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->Get_Skill_Info()->m_iSlotNum = i;
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetMain(true);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetParent(m_vecBlock[i]);
					m_pSound->MyPlaySound(L"ButtonPick");
					::Safe_Release(m_pCursor);
					break;
				}
				else if(((CMain_Block*)m_vecBlock[i])->GetSkill() != NULL && ((CCursor*)m_pCursor)->GetItem() == NULL) // 스킬스왑
				{
					CUI_Skill*	pTemp = NULL;
					pTemp = ((CCursor*)m_pCursor)->GetSkill();
					((CCursor*)m_pCursor)->SetSkill(((CMain_Block*)m_vecBlock[i])->GetSkill());
					((CMain_Block*)m_vecBlock[i])->SetSkill(pTemp);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->Get_Skill_Info()->m_iSlotNum = i;
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetMain(true);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetMousePick(false);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetIndex(i);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetParent((CMain_Block*)m_vecBlock[i]);
					m_vecBlock[i]->SetBlockPick(false);
					m_pSound->MyPlaySound(L"ButtonPick");
					::Safe_Release(m_pCursor);
					break;
				}
				else if(((CMain_Block*)m_vecBlock[i])->GetSkill() == NULL && ((CMain_Block*)m_vecBlock[i])->GetItem() != NULL) // 스킬-아이템스왑
				{
					CUI_Skill*	pTemp = NULL;
					pTemp = ((CCursor*)m_pCursor)->GetSkill();
					((CCursor*)m_pCursor)->SetSkill(NULL);
					((CCursor*)m_pCursor)->SetItem(((CMain_Block*)m_vecBlock[i])->GetItem());
					(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
					((CMain_Block*)m_vecBlock[i])->SetSkill(pTemp);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetMousePick(false);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->Get_Skill_Info()->m_iSlotNum = i;
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetMain(true);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetMousePick(false);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetIndex(i);
					(((CMain_Block*)m_vecBlock[i])->GetSkill())->SetParent((CMain_Block*)m_vecBlock[i]);
					((CMain_Block*)m_vecBlock[i])->SetItem(NULL);
					m_pSound->MyPlaySound(L"ButtonPick");

					m_vecBlock[i]->SetBlockPick(false);
					::Safe_Release(m_pCursor);
					break;
				}
			}

			else if(((CCursor*)m_pCursor)->GetSkill() == NULL && ((CCursor*)m_pCursor)->GetItem() != NULL) // 아이템들었을때
			{
				if(((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_HPPOTION
					|| ((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_MPPOTION
					|| ((CCursor*)m_pCursor)->GetItem()->Get_Item_Info()->m_eItemType == ITEM_LEVELPOTION)
				{
					if(((CMain_Block*)m_vecBlock[i])->GetItem() == NULL && ((CMain_Block*)m_vecBlock[i])->GetSkill() == NULL) // 아이템놓기
					{
						((CMain_Block*)m_vecBlock[i])->SetItem(((CCursor*)m_pCursor)->GetItem());
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetMousePick(false);
						((CCursor*)m_pCursor)->SetItem(NULL);
						m_vecBlock[i]->SetBlockPick(false);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetIndex(i);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->Get_Item_Info()->m_iSlotIndex = i;
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetMain(true);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetMousePick(false);
						m_pSound->MyPlaySound(L"ButtonPick");

						::Safe_Release(m_pCursor);
						break;
					}
					else if(((CMain_Block*)m_vecBlock[i])->GetItem() != NULL && ((CCursor*)m_pCursor)->GetSkill() == NULL) // 아이템스왑
					{
						CUI_Item*	pTemp = NULL;
						pTemp = ((CCursor*)m_pCursor)->GetItem();
						((CCursor*)m_pCursor)->SetItem(((CMain_Block*)m_vecBlock[i])->GetItem());
						((CMain_Block*)m_vecBlock[i])->SetItem(pTemp);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetIndex(i);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->Get_Item_Info()->m_iSlotIndex = i;
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetMain(true);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetMousePick(false);
						m_vecBlock[i]->SetBlockPick(false);
						m_pSound->MyPlaySound(L"ButtonPick");
						::Safe_Release(m_pCursor);
						break;
					}
					else if(((CMain_Block*)m_vecBlock[i])->GetItem() == NULL && ((CMain_Block*)m_vecBlock[i])->GetSkill() != NULL) // 아이템-스킬스왑
					{
						CUI_Item*	pTemp = NULL;
						pTemp = ((CCursor*)m_pCursor)->GetItem();
						((CCursor*)m_pCursor)->SetItem(NULL);
						((CCursor*)m_pCursor)->SetSkill(((CMain_Block*)m_vecBlock[i])->GetSkill());
						(((CCursor*)m_pCursor)->GetSkill())->SetMousePick(true);
						((CMain_Block*)m_vecBlock[i])->SetItem(pTemp);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetIndex(i);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetMousePick(false);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->Get_Item_Info()->m_iSlotIndex = i;
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetMain(true);
						(((CMain_Block*)m_vecBlock[i])->GetItem())->SetMousePick(false);
						((CMain_Block*)m_vecBlock[i])->SetSkill(NULL);
						m_pSound->MyPlaySound(L"ButtonPick");

						m_vecBlock[i]->SetBlockPick(false);
						::Safe_Release(m_pCursor);
						break;
					}
				}
			}

			else if(((CCursor*)m_pCursor)->GetSkill() == NULL && ((CCursor*)m_pCursor)->GetItem() == NULL) // 안들었을때
			{
				if(((CMain_Block*)m_vecBlock[i])->GetSkill() != NULL && ((CMain_Block*)m_vecBlock[i])->GetItem() == NULL) // 스킬 빼기
				{
					if(((CCursor*)m_pCursor)->GetDropDelay())
					{
						((CCursor*)m_pCursor)->SetDropDelay(false);
						m_vecBlock[i]->SetBlockPick(false);
						::Safe_Release(m_pCursor);
						return;
					}
					((CCursor*)m_pCursor)->SetSkill(((CMain_Block*)m_vecBlock[i])->GetSkill());
					(((CCursor*)m_pCursor)->GetSkill())->SetMousePick(true);
					((CMain_Block*)m_vecBlock[i])->SetSkill(NULL);
					m_vecBlock[i]->SetBlockPick(false);
					m_pSound->MyPlaySound(L"ButtonPick");

					::Safe_Release(m_pCursor);
					break;
				}
				else if(((CMain_Block*)m_vecBlock[i])->GetSkill() == NULL && ((CMain_Block*)m_vecBlock[i])->GetItem() != NULL) // 아이템 빼기
				{
					((CCursor*)m_pCursor)->SetItem(((CMain_Block*)m_vecBlock[i])->GetItem());
					(((CCursor*)m_pCursor)->GetItem())->SetMousePick(true);
					((CMain_Block*)m_vecBlock[i])->SetItem(NULL);
					m_vecBlock[i]->SetBlockPick(false);
					m_pSound->MyPlaySound(L"ButtonPick");

					::Safe_Release(m_pCursor);
					break;
				}
				else if(((CMain_Block*)m_vecBlock[i])->GetSkill() == NULL && ((CMain_Block*)m_vecBlock[i])->GetItem() == NULL) // 빈슬롯
				{
					m_vecBlock[i]->SetBlockPick(false);
					::Safe_Release(m_pCursor);
					break;
				}
			}
			::Safe_Release(m_pCursor);
		}
	}
}

void CMain_Base::PtInRect_Collision(const _float& fTimeDelta)
{
	for (size_t i = 0; i < m_vecBlock.size(); ++i)
	{
		if (m_vecBlock[i]->GetButtonCol())
		{
			m_bCollision = true;
			return;
		}
	}
	m_bCollision = false;
}

vector<CUI*>*	CMain_Base::GetVecBlock(void)
{
	return &m_vecBlock;
}
CMain_Base* CMain_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CMain_Base*		pInstance = new CMain_Base(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMain_Base::Free(void)
{
	CUI::Free();
}
