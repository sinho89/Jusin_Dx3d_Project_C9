#include "stdafx.h"
#include "Shop_Base.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Player.h"
#include "Npc.h"
#include "WitchBlade.h"
#include "Cursor.h"
#include "Npc_Col.h"

CShop_Base::CShop_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pInven(NULL)
{

}

CShop_Base::~CShop_Base(void)
{

}

HRESULT CShop_Base::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1) - 350.f;
	m_fY = (WINSIZEY >> 1) - 50.f;

	m_fSizeX = 330.f;
	m_fSizeY = 560.f;

	m_fAlpha = 1.f;
	m_bRender = false;
	m_eWinType = WINDOW_SHOP;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CShop_Base::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN /*&& Engine::GetSceneID() != Engine::SCENE_STAGE_ICE*/)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;
	CUI::Start_ForScene();
	m_pCursor = ((CUI*)Engine::Find(L"Cursor"));

	::Safe_Release(m_pCursor);
	switch (m_eType)
	{
	case SHOP_WEAPON:
		m_pNpc = ((CNPC*)Engine::Find(L"Mesh_NPC_Weapon"));
		break;
	case SHOP_ARMOR:
		m_pNpc = ((CNPC*)Engine::Find(L"Mesh_NPC_Armor"));
		break;
	case SHOP_ACC:
		m_pNpc = ((CNPC*)Engine::Find(L"Mesh_NPC_Book"));
		break;
	case SHOP_POTION:
		m_pNpc = ((CNPC*)Engine::Find(L"Mesh_NPC_Potion"));
		break;
	}

	::Safe_Release(m_pNpc);

	m_pInven = ((CUI*)Engine::Find(L"Inven_Base"));
	::Safe_Release(m_pInven);
	return S_OK;
}

void CShop_Base::Update_Object(const _float& fTimeDelta)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN)
		return;

	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		RenderToolTip();
		PtInRect_Collision(fTimeDelta);
	}
	else
	{
		m_fX = (WINSIZEX >> 1) - 350.f;
		m_fY = (WINSIZEY >> 1) - 50.f;

		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
		m_bWindowProcess = false;
		m_bSelect = false;
		m_bMove = false;
		m_bFoward = true;
	}

	RenderColl();
}

void CShop_Base::Render_Object(void)
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

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}

void CShop_Base::RenderToolTip()
{

	/*for (size_t i = 0; i < m_vecSpace.size(); ++i)
	{
		if (m_vecSpace[i]->GetButtonCol() && m_vecSpace[i]->GetItem())
		{
			switch (m_vecSpace[i]->GetItem()->Get_Item_Info()->m_eItemType)
			{
			case ITEM_BLUE_WEAPON:
				m_pToolTip->SetId(TOOLTIP_WEAPON_BLUE);
				break;

			case ITEM_RED_WEAPON:
				m_pToolTip->SetId(TOOLTIP_WEAPON_RED);
				break;

			case ITEM_HELMET:
				m_pToolTip->SetId(TOOLTIP_HELMET);
				break;

			case ITEM_ARMOR:
				m_pToolTip->SetId(TOOLTIP_ARMOR);
				break;

			case ITEM_SHORDER:
				m_pToolTip->SetId(TOOLTIP_SHORDER);
				break;

			case ITEM_LEG:
				m_pToolTip->SetId(TOOLTIP_LEG);
				break;

			case ITEM_GLOVE:
				m_pToolTip->SetId(TOOLTIP_GLOVE);
				break;

			case ITEM_BOOTS:
				m_pToolTip->SetId(TOOLTIP_BOOTS);
				break;

			case ITEM_AMULET:
				m_pToolTip->SetId(TOOLTIP_AMULET);
				break;

			case ITEM_ARMLET:
				m_pToolTip->SetId(TOOLTIP_ARMLET);
				break;

			case ITEM_BELT:
				m_pToolTip->SetId(TOOLTIP_BELT);
				break;

			case ITEM_EARING:
				m_pToolTip->SetId(TOOLTIP_EARING);
				break;

			case ITEM_RING:
				m_pToolTip->SetId(TOOLTIP_RING);
				break;

			case ITEM_RING2:
				m_pToolTip->SetId(TOOLTIP_RING2);
				break;

			case ITEM_HPPOTION:
				m_pToolTip->SetId(TOOLTIP_HP_POTION);
				break;

			case ITEM_MPPOTION:
				m_pToolTip->SetId(TOOLTIP_MP_POTION);
				break;

			case ITEM_LEVELPOTION:
				m_pToolTip->SetId(TOOLTIP_LEVELUP_ITEM);
				break;
			}

			if (m_bFoward)
				m_pToolTip->SetRender(true);

			if (*m_vecSpace[i]->GetfY() <= (WINSIZEY >> 1))
			{
				if (*m_vecSpace[i]->GetfX() > (WINSIZEX >> 1))
					m_pToolTip->SetWinPos(_vec2(m_vecSpace[i]->GetInfo().x - 150.f, m_vecSpace[i]->GetInfo().y + 200.f));
				else if (*m_vecSpace[i]->GetfX() <= (WINSIZEX >> 1))
					m_pToolTip->SetWinPos(_vec2(m_vecSpace[i]->GetInfo().x + 150.f, m_vecSpace[i]->GetInfo().y + 200.f));
			}
			else if (*m_vecSpace[i]->GetfY() > (WINSIZEY >> 1))
			{
				if (*m_vecSpace[i]->GetfX() > (WINSIZEX >> 1))
					m_pToolTip->SetWinPos(_vec2(m_vecSpace[i]->GetInfo().x - 150.f, m_vecSpace[i]->GetInfo().y - 200.f));
				else if (*m_vecSpace[i]->GetfX() <= (WINSIZEX >> 1))
					m_pToolTip->SetWinPos(_vec2(m_vecSpace[i]->GetInfo().x + 150.f, m_vecSpace[i]->GetInfo().y - 200.f));
			}

			return;
		}

	}

	m_pToolTip->SetRender(false);*/


}

void CShop_Base::RenderColl()
{
	_vec3 vPlayerPos, vNpcPos;
	((Engine::CTransform*)m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);
	((Engine::CTransform*)m_pNpc->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vNpcPos);

	float fLength = D3DXVec3Length(&(vPlayerPos - vNpcPos));

	if ((fLength < 15.f) && m_bAlive)
	{
		m_pNpc->SetOutlineCheck(true);
		if(m_pNpc->GetUIEff() != NULL)
			((CNpc_Col*)m_pNpc->GetUIEff())->SetRender(true);
		m_bPick = true;
		m_bAlive = false;
	}
	else if (fLength > 15.f)
	{
		m_pNpc->SetOutlineCheck(false);
		((CNpc_Col*)m_pNpc->GetUIEff())->SetRender(false);

		m_bAlive = true;
		m_bPick = false;
	}

	if (m_bPick)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if (m_bClick)
				return;
			m_bClick = true;

			CUI* pEsc = ((CUI*)Engine::Find(L"Esc_Base"));
			CUI* pInfo = ((CUI*)Engine::Find(L"Window_Info"));
			CUI* pSkill = ((CUI*)Engine::Find(L"Skill_Base"));

			::Safe_Release(pEsc);
			::Safe_Release(pInfo);
			::Safe_Release(pSkill);

			if (pEsc->GetRender() || pInfo->GetRender() || pSkill->GetRender())
				return;

			m_bRender = true;
			m_pInven->SetRender(true);
			m_bPick = false;

			m_pNpc->PlayNPCSound();
		}
		else
			m_bClick = false;
	}
}

void CShop_Base::PtInRect_Collision(const _float& fTimeDelta)
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
			if(((CCursor*)m_pCursor)->GetItem() != NULL)
				m_bCollision = true;
		}
		else
		{
			m_bClick = false;
			m_bCollision = false;
		}
	}
}

HRESULT CShop_Base::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Shop_Base");
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

	// For.Calculater Component
	pComponent = m_pCalCom = Engine::CUICalculater::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_UICal", pComponent));

	return S_OK;
}

CShop_Base* CShop_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CShop_Base*		pInstance = new CShop_Base(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CShop_Base* CShop_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, SHOP_ID eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CShop_Base*		pInstance = new CShop_Base(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetType(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CShop_Base::Free(void)
{;
	CUI::Free();
}
