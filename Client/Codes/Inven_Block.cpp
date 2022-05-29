#include "stdafx.h"
#include "Inven_Block.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_Item.h"
#include "Cursor.h"

CInven_Block::CInven_Block(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_pItem(NULL)
{

}

CInven_Block::~CInven_Block(void)
{

}

HRESULT CInven_Block::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = m_pParentWindow->GetInfo();

	m_fX = vInfo.x - m_vParentBasePos.x;
	m_fY = vInfo.y - m_vParentBasePos.y;

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;

	m_fAlpha = 1.f;
	m_iIndex = m_pParentWindow->GetIndex();
	m_bSheetSelect = m_pParentWindow->GetSheetSelect();
	m_eWinType = WINDOW_INVEN;

	m_bIsStatic = true;
	m_bSelect = false;
	m_bWear = false;
	m_bAlpha_Increase = false;
	return S_OK;
}

HRESULT CInven_Block::Start_ForScene(void)
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

void CInven_Block::Update_Object(const _float& fTimeDelta)
{
	if(m_pParentWindow->GetSheetSelect()
		&& m_pParentWindow->GetRender())
	{		
		_vec4 vInfo = m_pParentWindow->GetInfo();

		m_fX = vInfo.x - m_vParentBasePos.x;
		m_fY = vInfo.y - m_vParentBasePos.y;

		CUI::Update_Object(fTimeDelta);
		PtInRect_Collision(fTimeDelta);
		m_wSortNum = m_pParentWindow->GetSortNum();
		//m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		SetItemPos();
	}
	if (!m_pParentWindow->GetRender() || !m_pParentWindow->GetSheetSelect())
	{
		m_bKeyBoard = false;
		m_bRClick = false;
		m_bPick = false;
		m_bBlockPick = false;
		m_bPartition = false;
	}
}

void CInven_Block::Render_Object(void)
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
	
	pEffect->SetInt("g_iIndex", (int)m_iIndex);
	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(5);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pBufferCom->Render_Buffer();	
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 
}


HRESULT CInven_Block::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Helmet_Space");
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
CInven_Block* CInven_Block::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CInven_Block*		pInstance = new CInven_Block(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CInven_Block* CInven_Block::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, _vec2 vPos, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CInven_Block*		pInstance = new CInven_Block(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetPos(vPos);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}
void CInven_Block::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}


void CInven_Block::SetPos(_vec2 vPos)
{
	m_vParentBasePos = vPos;
}

void CInven_Block::PtInRect_Collision(const _float& fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_vec4 vInfo = m_pParentWindow->GetInfo();

	m_fX = vInfo.x - m_vParentBasePos.x;
	m_fY = vInfo.y - m_vParentBasePos.y;

	m_bButtonCollision = m_pCalCom->Collision_ByMouse(WINSIZEX
		, WINSIZEY
		, m_fX
		, m_fY
		, m_fSizeX
		, m_fSizeY
		, ptMouse);


	if(m_bButtonCollision && m_pParentWindow->GetSheetSelect())
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if(m_bPick)
				return;
			m_bPick = true;
			m_bBlockPick = true;
		}
		else
			m_bPick = false;

		if (Engine::Get_DIKeyState(DIK_F1) & 0x80)
		{
			if (m_bKeyBoard)
				return;
			m_bKeyBoard = true;
			m_bPartition = true;
		}
		else
			m_bKeyBoard = false;

		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80)
		{
			if (m_bRClick)
				return;
			m_bRClick = true;

			CUI* pCursor = ((CUI*)Engine::Find(L"Cursor"));
			::Safe_Release(pCursor);
			if(((CCursor*)pCursor)->GetItem() != NULL)
				return;

			m_bRBPick = true;
		}
		else
			m_bRClick = false;
	}
}

CUI_Item* CInven_Block::GetItem(void)
{
	return m_pItem;
}

void CInven_Block::SetItem(CUI_Item* pItem)
{
	m_pItem = pItem;
}

void CInven_Block::SetItemPos(void)
{
	if(m_pItem != NULL)
	{
		m_pItem->SetWinPos(_vec2(m_fX, m_fY));
		m_pItem->SetMousePick(false);
	}
}

void CInven_Block::Free(void)
{
	//::Safe_Release(m_pParentWindow);
	CUI::Free();
}
