#include "stdafx.h"
#include "Cursor.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_Item.h"
#include "UI_Skill.h"
#include "MouseEff.h"

CCursor::CCursor(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
:CUI(pGraphicDev, wstrName, eState, bIsActive)
,m_pTransCom(NULL)
,m_pBufferCom(NULL)
,m_pTextureCom(NULL)
,m_pRendererCom(NULL)
,m_pShaderCom(NULL)
,m_pItem(NULL)
,m_pSkill(NULL)
,m_bDropDelay(false)
{
}

CCursor::~CCursor(void)
{

}

HRESULT CCursor::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = 25.f;
	m_fSizeY = 25.f;

	m_fAlpha = 1.f;

	m_bIsStatic = true;
	m_bIsActive = true;
	m_bEffect = false;
	
	return S_OK;
}

void CCursor::SetItem(CUI_Item* pItem)
{
	m_pItem = pItem;
}

void CCursor::SetSkill(CUI_Skill* pSkill)
{
	m_pSkill = pSkill;
}

void CCursor::SetDropDelay(_bool bDropDelay)
{
	m_bDropDelay = bDropDelay;
}

CUI_Item* CCursor::GetItem(void)
{
	return m_pItem;
}

CUI_Skill* CCursor::GetSkill(void)
{
	return m_pSkill;
}

_bool CCursor::GetDropDelay(void)
{
	return m_bDropDelay;
}

HRESULT CCursor::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN
		&& Engine::GetSceneID() != Engine::SCENE_STAGE_ICE
		&& Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
		m_bRender = false;
		m_bEffect = true;

	return S_OK;
}

void CCursor::Update_Object(const _float& fTimeDelta)
{
	if(m_pSkill != NULL)
		m_pSkill->Update_Object(fTimeDelta);
	CUI::Update_Object(fTimeDelta);
	MoveFollowMousePos();
	SetItemPos();
	SetSkillPos();

	if (!m_bRender)
	{
		if (!m_bEffect && (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_ICE || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE))
		{
			Engine::CGameObject* pGameObject = CMouseEff::Create(m_pGraphicDev, L"Mouse_Eff", this);
			Engine::AddObject(Engine::LAYER_UI, L"UI_Eff", pGameObject);
			m_bEffect = true;
		}
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_CURSOR, this);
	}
	else
		m_bEffect = false;
	
	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CCursor::Render_Object(void)
{
	if(m_pSkill != NULL)
		m_pSkill->Render_Object();
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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 
}


void CCursor::MoveFollowMousePos()
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_fX = (float)ptMouse.x + (m_fSizeX * 0.5f);
	m_fY = (float)ptMouse.y + (m_fSizeY * 0.5f);
}

void CCursor::SetItemPos(void)
{
	if(m_pItem != NULL)
	{
		m_pItem->SetWinPos(_vec2(m_fX, m_fY));
		m_pItem->SetMousePick(true);
	}
}

void CCursor::SetSkillPos(void)
{
	if(m_pSkill != NULL)
	{
		m_pSkill->SetWinPos(_vec2(m_fX, m_fY));
		m_pSkill->SetMousePick(true);
	}
}

HRESULT CCursor::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Cursor");
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


	return S_OK;
}

CCursor* CCursor::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCursor*		pInstance = new CCursor(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CCursor* CCursor::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, _bool bRender, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CCursor*		pInstance = new CCursor(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	pInstance->SetRender(bRender);

	return pInstance;
}

void CCursor::Free(void)
{
	if(!m_pSkill)
		Engine::Safe_Release(m_pSkill);

	CUI::Free();
}
