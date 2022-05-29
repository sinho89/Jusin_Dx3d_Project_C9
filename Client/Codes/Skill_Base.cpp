#include "stdafx.h"
#include "Skill_Base.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Player.h"

CSkill_Base::CSkill_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_pActionSheet(NULL)
,m_pComandSheet(NULL)
,m_pPassiveSheet(NULL)

{
	for(int i = 0; i < 4; ++i)
		m_pTextureCom[i] = NULL;
}

CSkill_Base::~CSkill_Base(void)
{

}

HRESULT CSkill_Base::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1) + 350.f;
	m_fY = (WINSIZEY >> 1) - 50.f;

	m_fSizeX = 425.f;
	m_fSizeY = 550.f;

	m_fAlpha = 1.f;
	m_bRender = false;
	m_eWinType = WINDOW_SKILL;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CSkill_Base::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	m_pActionSheet = ((CUI*)Engine::Find(L"Skill_Sheet_1"));
	m_pComandSheet = ((CUI*)Engine::Find(L"Skill_Sheet_2"));
	m_pPassiveSheet = ((CUI*)Engine::Find(L"Skill_Sheet_3"));

	::Safe_Release(m_pActionSheet);
	::Safe_Release(m_pComandSheet);
	::Safe_Release(m_pPassiveSheet);
	return S_OK;
}

void CSkill_Base::Update_Object(const _float& fTimeDelta)
{
	if(m_bRender)
	{		
		CUI::Update_Object(fTimeDelta);
		Window_MoveToMouse();
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
		m_bWindowProcess = false;
		m_bSelect = false;
		m_bMove = false;
		m_bFoward = true;
	}
}

void CSkill_Base::Render_Object(void)
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


	if (m_pActionSheet->GetSheetSelect())
		m_pTextureCom[0]->Set_Texture(pEffect, "g_BaseTexture");
	else if (m_pComandSheet->GetSheetSelect())
	{
		if (m_pComandSheet->GetSlotLevel() == 1)
			m_pTextureCom[1]->Set_Texture(pEffect, "g_BaseTexture");
		else if (m_pComandSheet->GetSlotLevel() == 2)
			m_pTextureCom[2]->Set_Texture(pEffect, "g_BaseTexture");
	}
	else if (m_pPassiveSheet->GetSheetSelect())
		m_pTextureCom[3]->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 

}


void CSkill_Base::Window_MoveToMouse()
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
		}

		iter = Engine::FindList(L"UI_Window")->begin();

		for (; iter != iter_end; ++iter)
			((CUI*)(*iter))->SetFoward(false);

		m_bFoward = true;
	}

	Overlap_Window();
}

HRESULT CSkill_Base::Add_Component(void)
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

	if (m_bType)
	{
		// For.Texture Component 
		m_pTextureCom[0] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Window_Skill_Base");
		pComponent = (Engine::CComponent*)m_pTextureCom[0];
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

		m_pTextureCom[1] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Window_Skill_Base_Comend1");
		pComponent = (Engine::CComponent*)m_pTextureCom[1];
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture2", pComponent));

		m_pTextureCom[2] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Window_Skill_Base_Comend2");
		pComponent = (Engine::CComponent*)m_pTextureCom[2];
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture3", pComponent));

		m_pTextureCom[3] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Window_Skill_Base_Passive");
		pComponent = (Engine::CComponent*)m_pTextureCom[3];
		if(NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture4", pComponent));
	}
	else
	{
		// For.Texture Component 
		m_pTextureCom[0] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"M_Skill_Base_Action");
		pComponent = (Engine::CComponent*)m_pTextureCom[0];
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

		m_pTextureCom[1] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"M_Skill_Base_Comend");
		pComponent = (Engine::CComponent*)m_pTextureCom[1];
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture2", pComponent));

		m_pTextureCom[2] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"M_Skill_Base_Comend");
		pComponent = (Engine::CComponent*)m_pTextureCom[2];
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture3", pComponent));

		m_pTextureCom[3] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"M_Skill_Base_Passive");
		pComponent = (Engine::CComponent*)m_pTextureCom[3];
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture4", pComponent));

	}
	

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

CSkill_Base* CSkill_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSkill_Base*		pInstance = new CSkill_Base(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CSkill_Base* CSkill_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _bool bType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSkill_Base*		pInstance = new CSkill_Base(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetType(bType);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Base::Free(void)
{

	CUI::Free();
}
