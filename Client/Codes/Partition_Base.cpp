#include "stdafx.h"
#include "Partition_Base.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Player.h"
#include "UI_Item.h"

CPartition_Base::CPartition_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pTextureCom(NULL)
	, m_pItem(NULL)

{
}

CPartition_Base::~CPartition_Base(void)
{

}

HRESULT CPartition_Base::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1) + 100.f;
	m_fY = (WINSIZEY >> 1) - 50.f;

	m_fSizeX = 300.f;
	m_fSizeY = 200.f;

	m_fAlpha = 1.f;
	m_wSortNum = 20;
	m_bRender = false;
	m_eWinType = WINDOW_PARTITION;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CPartition_Base::Start_ForScene(void)
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

void CPartition_Base::Update_Object(const _float& fTimeDelta)
{
	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);
		m_wSortNum = 20;
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		Window_MoveToMouse();
	}
	else
	{
		m_fX = (WINSIZEX >> 1) + 100.f;
		m_fY = (WINSIZEY >> 1) - 50.f;

		m_iPartitionMinCnt = 1;
		m_iPartitionMaxCnt = 0;

		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
	}
}

void CPartition_Base::Render_Object(void)
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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	_tchar		szInfo[128] = L"";
	wsprintf(szInfo, L"%d / %d", m_iPartitionMinCnt, m_iPartitionMaxCnt);	// 원천 능력
	Engine::Render_Font(L"Font_Main_Button", szInfo, &_vec2(m_fX - 70.f, m_fY + 10.f), D3DXCOLOR(1.f, 1.f, 1.f, m_fAlpha));

}

void CPartition_Base::SetItem(CUI_Item* pItem)
{
	m_pItem = pItem;
}

CUI_Item* CPartition_Base::GetItem(void)
{
	return m_pItem;
}

void CPartition_Base::Window_MoveToMouse()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bBarCollision = m_pCalCom->Move_Window_UI(WINSIZEX, WINSIZEY, m_fX, m_fY, m_fSizeX, m_fSizeY, ptMouse);
	m_bCollision = m_pCalCom->Collision_ByMouse(WINSIZEX, WINSIZEY, m_fX, m_fY, m_fSizeX, m_fSizeY, ptMouse);

	list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"UI_Window")->begin();
	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"UI_Window")->end();

	for (; iter != iter_end; ++iter)
	{
		if (this == (*iter))
			continue;
		if (((CUI*)(*iter))->GetWindowProcess())
		{
			//m_bClick = false;
			m_bBarCollision = false;
			m_bCollision = false;
		}
	}

	if (m_bMove)
	{
		m_bMove = false;
		m_fTermX = m_fX - ptMouse.x;
		m_fTermY = m_fY - ptMouse.y;
	}
	if (m_bClick)
	{
		m_bWindowProcess = true;
		m_fX = ptMouse.x + m_fTermX;
		m_fY = ptMouse.y + m_fTermY;
	}

	if (m_bWindowProcess || m_bSelect)
	{
		iter = Engine::FindList(L"UI_Window")->begin();

		for (; iter != iter_end; ++iter)
		{
			if (this == (*iter))
				m_wSortNum = 20;
			else if (((CUI*)(*iter))->GetSelect())
			{
				((CUI*)(*iter))->SetSortNum(this->GetSortNum() - 1);
				((CUI*)(*iter))->SetSelect(false);
			}
			else
				((CUI*)(*iter))->SetSortNum(0);
		}
	}
	if (m_bBarCollision)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if (m_bClick)
				return;
			m_bClick = true;
			m_bMove = true;
		}
		else
		{
			if (m_bClick)
				m_bWindowProcess = false;
			m_bClick = false;
		}
	}

	if (m_bCollision)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			iter = Engine::FindList(L"UI_Window")->begin();

			for (; iter != iter_end; ++iter)
				((CUI*)(*iter))->SetSelect(false);

			m_bSelect = true;
		}
	}

	Overlap_Window();
}

HRESULT CPartition_Base::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Base");
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

CPartition_Base* CPartition_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CPartition_Base*		pInstance = new CPartition_Base(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPartition_Base::Free(void)
{
	
	CUI::Free();
}
