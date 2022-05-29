#include "stdafx.h"
#include "Inven_Base.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Player.h"
#include "Wear_ToolTip.h"
#include "Inven_Space.h"
#include "UI_Item.h"

CInven_Base::CInven_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
, m_pPlayer(NULL)
{

}

CInven_Base::~CInven_Base(void)
{

}

HRESULT CInven_Base::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1) + 350.f;
	m_fY = (WINSIZEY >> 1) - 50.f;

	m_fSizeX = 350.f;
	m_fSizeY = 600.f;

	m_fAlpha = 1.f;
	m_bRender = false;
	m_eWinType = WINDOW_INVEN;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CInven_Base::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	CUI::Start_ForScene();

	m_pToolTip = ((CWear_ToolTip*)((CUI*)Engine::Find(L"Wear_ToolTip")));
	m_pPlayer = ((CPlayer*)Engine::Find(L"Player"));
	m_pCursor = ((CUI*)Engine::Find(L"Cursor"));
	Engine::Safe_Release(m_pPlayer);
	::Safe_Release(m_pToolTip);
	::Safe_Release(m_pCursor);
	return S_OK;
}


void CInven_Base::Update_Object(const _float& fTimeDelta)
{
	if(m_bRender)
	{		
		CUI::Update_Object(fTimeDelta);
		Window_MoveToMouse();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		RenderToolTip();
	}
	else
	{
		m_fX = (WINSIZEX >> 1) + 350.f;
		m_fY = (WINSIZEY >> 1) - 50.f;

		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
		m_bWindowProcess = false;
		m_bSelect = false;
		m_bMove = false;
		m_bFoward = true;
		m_pToolTip->SetRender(false);
	}

}

void CInven_Base::Render_Object(void)
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

	_tchar		szInfo[128] = L"";
	wsprintf(szInfo, L"%d", m_pPlayer->GetPlayerInfo()->m_iMoney);	// 원천 능력
	Engine::Render_Font(L"Font_Info_Data", szInfo, &_vec2(m_fX + 75.f, m_fY + 270.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));

	
}


void CInven_Base::Window_MoveToMouse()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bBarCollision = m_pCalCom->Move_Window_UI(WINSIZEX, WINSIZEY, m_fX, m_fY, m_fSizeX, m_fSizeY, ptMouse);
	m_bCollision = m_pCalCom->Collision_ByMouse(WINSIZEX, WINSIZEY, m_fX, m_fY, m_fSizeX, m_fSizeY, ptMouse);

	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Window")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Window")->end();

	for(; iter != iter_end; ++iter)
	{
		if(this == (*iter))
			continue;
		if(((CUI*)(*iter))->GetWindowProcess())
		{
			//m_bClick = false;
			m_bBarCollision = false;
			m_bCollision = false;
		}
	}

	if(m_bMove)
	{
		m_bMove = false;
		m_fTermX = m_fX - ptMouse.x;
		m_fTermY = m_fY - ptMouse.y;
	}
	if(m_bClick)
	{
		m_bWindowProcess = true;
		m_fX = ptMouse.x + m_fTermX;
		m_fY = ptMouse.y + m_fTermY;
	}

	if(m_bWindowProcess || m_bSelect)
	{
		iter = Engine::FindList(L"UI_Window")->begin();

		for(; iter != iter_end; ++iter)
		{
			if(this == (*iter))
				m_wSortNum = 2;
			else if(((CUI*)(*iter))->GetSelect())
			{
				((CUI*)(*iter))->SetSortNum(this->GetSortNum() - 1);
				((CUI*)(*iter))->SetSelect(false);
			}
			else
				((CUI*)(*iter))->SetSortNum(0);
		}

		m_bSelect;
	}
	if(m_bBarCollision)
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if(m_bClick)
				return;
			m_bClick = true;
			m_bMove = true;
		}
		else
		{
			if(m_bClick)
				m_bWindowProcess = false;
			m_bClick = false;
		}
	}

	if(m_bCollision)
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			iter = Engine::FindList(L"UI_Window")->begin();

			for(; iter != iter_end; ++iter)
				((CUI*)(*iter))->SetSelect(false);
	
			m_bSelect = true;

			iter = Engine::FindList(L"UI_Window")->begin();

			for (; iter != iter_end; ++iter)
				((CUI*)(*iter))->SetFoward(false);

			m_bFoward = true;
		}
	}

	Overlap_Window();
}

void CInven_Base::RenderToolTip()
{

	for (size_t i = 0; i < m_vecSpace.size(); ++i)
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

			case ITEM_S_BLUE_WEAPON:
				m_pToolTip->SetId(TOOLTIP_S_WEAPON_BLUE);
				break;

			case ITEM_S_RED_WEAPON:
				m_pToolTip->SetId(TOOLTIP_S_WEAPON_RED);
				break;

			case ITEM_HELMET:
				m_pToolTip->SetId(TOOLTIP_HELMET);
				break;
			case ITEM_S_HELMET:
				m_pToolTip->SetId(TOOLTIP_S_HELMET);
				break;
			case ITEM_ARMOR:
				m_pToolTip->SetId(TOOLTIP_ARMOR);
				break;
			case ITEM_S_ARMOR:
				m_pToolTip->SetId(TOOLTIP_S_ARMOR);
				break;

			case ITEM_SHORDER:
				m_pToolTip->SetId(TOOLTIP_SHORDER);
				break;
			case ITEM_S_SHORDER:
				m_pToolTip->SetId(TOOLTIP_S_SHORDER);
				break;

			case ITEM_LEG:
				m_pToolTip->SetId(TOOLTIP_LEG);
				break;
			case ITEM_S_LEG:
				m_pToolTip->SetId(TOOLTIP_S_LEG);
				break;

			case ITEM_GLOVE:
				m_pToolTip->SetId(TOOLTIP_GLOVE);
				break;
			case ITEM_S_GLOVE:
				m_pToolTip->SetId(TOOLTIP_S_GLOVE);
				break;

			case ITEM_BOOTS:
				m_pToolTip->SetId(TOOLTIP_BOOTS);
				break;
			case ITEM_S_BOOTS:
				m_pToolTip->SetId(TOOLTIP_S_BOOTS);
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

			if(m_bFoward)
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

	m_pToolTip->SetRender(false);


}

HRESULT CInven_Base::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Window_Inven_Base");
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

CInven_Base* CInven_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CInven_Base*		pInstance = new CInven_Base(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CInven_Base::Free(void)
{
	
	CUI::Free();
}
