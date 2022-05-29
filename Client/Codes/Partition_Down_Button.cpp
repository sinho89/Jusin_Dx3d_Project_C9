#include "stdafx.h"
#include "Partition_Down_Button.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Sound.h"

CPartition_Down_Button::CPartition_Down_Button(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pNCTextureCom(NULL)
	, m_pCTextureCom(NULL)
	, m_pClickTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
{

}

CPartition_Down_Button::~CPartition_Down_Button(void)
{

}

HRESULT CPartition_Down_Button::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = m_pParent->GetInfo().x + 30.f;
	m_fY = m_pParent->GetInfo().y + 40.f;

	m_fSizeX = 20.f;
	m_fSizeY = 20.f;

	m_fAlpha = 1.f;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CPartition_Down_Button::Start_ForScene(void)
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

void CPartition_Down_Button::Update_Object(const _float& fTimeDelta)
{
	m_bRender = m_pParent->GetRender();

	if (m_bRender)
	{
		m_fX = m_pParent->GetInfo().x + 50.f;
		m_fY = m_pParent->GetInfo().y + 20.f;

		CUI::Update_Object(fTimeDelta);
		PtInRect_Collision(fTimeDelta);
		m_wSortNum = m_pParent->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}

}

void CPartition_Down_Button::Render_Object(void)
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

	if (m_bKeyBoard)
	{
		m_pClickTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}
	else
	{
		if (m_bCollision)
			m_pCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
		else
			m_pNCTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	}

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}


HRESULT CPartition_Down_Button::Add_Component(void)
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
	m_pNCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Down_NC");
	pComponent = (Engine::CComponent*)m_pNCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_NC", pComponent));

	m_pCTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Down_C");
	pComponent = (Engine::CComponent*)m_pCTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_C", pComponent));

	m_pClickTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Partition_Down_Click");
	pComponent = (Engine::CComponent*)m_pClickTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture_Click", pComponent));

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

	// Sound
	pComponent = m_pSound = CSound::Create(NULL);
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	m_pSound->Set_Sound(L"ButtonPick", L"Pick.wav");

	return S_OK;
}
void CPartition_Down_Button::PtInRect_Collision(const _float& fTimeDelta)
{
	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_bCollision = m_pCalCom->Collision_ByMouse(WINSIZEX
		, WINSIZEY
		, m_fX
		, m_fY
		, m_fSizeX
		, m_fSizeY
		, ptMouse);


	if (m_bCollision)
	{
		if (Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
		{
			if (m_bKeyBoard)
				return;
			m_bKeyBoard = true;
			m_pSound->MyPlaySound(L"ButtonPick");

			if(*(m_pParent->GetPartitionMinCnt()) > 1)
			 --(*(m_pParent->GetPartitionMinCnt()));
		}
		else
			m_bKeyBoard = false;
	}

}

void CPartition_Down_Button::SetParent(CUI* pParent)
{
	m_pParent = pParent;
}

CPartition_Down_Button* CPartition_Down_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CPartition_Down_Button*		pInstance = new CPartition_Down_Button(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CPartition_Down_Button* CPartition_Down_Button::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParent, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CPartition_Down_Button*		pInstance = new CPartition_Down_Button(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParent);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPartition_Down_Button::Free(void)
{
	CUI::Free();
}
