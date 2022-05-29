#include "stdafx.h"
#include "DunGeon_Base.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Effect.h"
#include "Player.h"
#include "Portal.h"

CDunGeon_Base::CDunGeon_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pTextureCom(NULL)
	, m_pPortal(NULL)

{
	for (int i = 0; i < 4; ++i)
		m_bDirection[i] = false;
}

CDunGeon_Base::~CDunGeon_Base(void)
{

}

HRESULT CDunGeon_Base::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = 1810.f;
	m_fSizeY = 1803.f;

	m_fAlpha = 1.f;
	m_wSortNum = 90;
	m_bRender = false;
	m_eWinType = WINDOW_DUNGEON;

	m_bIsStatic = true;
	return S_OK;
}

void CDunGeon_Base::PotalCollision(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN)
		return;
	_vec3 vPlayerPos, vPortalPos;
	((Engine::CTransform*)m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);
	((Engine::CTransform*)m_pPortal->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vPortalPos);


	float fLength = D3DXVec3Length(&(vPlayerPos - vPortalPos));

	if ((fLength < 15.f) && m_bAlive)
	{
		m_bRender = true;
		m_bAlive = false;
	}
	else if(fLength > 15.f)
	{
		m_bAlive = true;
	}
}

void CDunGeon_Base::MoveToMouse(void)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (ptMouse.x <= 10.f)
	{
		if (m_fX >= 900.f)
		{
		}
		else
			m_fX += 10.f;
	}
	if ((ptMouse.x >= WINSIZEX - 250) && (ptMouse.x < WINSIZEX - 200))
	{
		if (m_fX <= 700.f)
		{
		}
		else
			m_fX -= 10.f;
	}
	if (ptMouse.y <= 10)
	{
		if (m_fY >= 900.f)
		{
		}
		else
			m_fY += 10.f;
	}
	if (ptMouse.y >= WINSIZEY - 50)
	{
		if (m_fY <= 0.f)
		{
		}
		else
			m_fY -= 10.f;
	}
}

HRESULT CDunGeon_Base::Start_ForScene(void)
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
	m_pPortal = ((CPortal*)(Engine::CEffect*)Engine::Find(L"Portal"));
	::Safe_Release(m_pPortal);
	return S_OK;
}

void CDunGeon_Base::Update_Object(const _float& fTimeDelta)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN)
		return;
	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
		MoveToMouse();
	}
	else
	{
		m_fX = (WINSIZEX >> 1);
		m_fY = (WINSIZEY >> 1);
	}

	PotalCollision();
}

void CDunGeon_Base::Render_Object(void)
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
}

HRESULT CDunGeon_Base::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"DunGeon_Base");
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

CDunGeon_Base* CDunGeon_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CDunGeon_Base*		pInstance = new CDunGeon_Base(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDunGeon_Base::Free(void)
{

	CUI::Free();
}
