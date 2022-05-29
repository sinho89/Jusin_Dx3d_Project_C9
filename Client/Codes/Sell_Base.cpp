#include "stdafx.h"
#include "Sell_Base.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "UI_Item.h"

CSell_Base::CSell_Base(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pTextureCom(NULL)
	, m_pSheet(NULL)
	, m_pItem(NULL)

{
}

CSell_Base::~CSell_Base(void)
{

}

HRESULT CSell_Base::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = 350.f;
	m_fSizeY = 250.f;

	m_fAlpha = 1.f;
	m_wSortNum = 20;
	m_bRender = false;
	m_eWinType = WINDOW_SELL;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CSell_Base::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN /*&& Engine::GetSceneID() != Engine::SCENE_STAGE_ICE*/)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	return S_OK;
}

void CSell_Base::Update_Object(const _float& fTimeDelta)
{
	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
		m_bSelect = false;
		m_bWindowProcess = false;
		m_bMove = false;
		m_bFoward = false;
	}

}

void CSell_Base::Render_Object(void)
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

	if (m_pItem)
	{
		switch (m_pItem->Get_Item_Info()->m_eItemType)
		{
		case ITEM_BLUE_WEAPON:
			Engine::Render_Font(L"Font_Main_Button", L"������ ���� ��ũ ���̵�", &_vec2(m_fX - 100.f, m_fY - 20.f), D3DXCOLOR(0.2f, 0.2f, 1.f, 1.f));
			break;

		case ITEM_RED_WEAPON:
			Engine::Render_Font(L"Font_Main_Button", L"�̴��� ������ ��ũ ���̵�", &_vec2(m_fX - 100.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			break;

		case ITEM_S_BLUE_WEAPON:
			Engine::Render_Font(L"Font_Main_Button", L"����Ƽ���̼�", &_vec2(m_fX - 40.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_S_RED_WEAPON:
			Engine::Render_Font(L"Font_Main_Button", L"�� ������", &_vec2(m_fX - 30.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.3f, 0.f, 1.f));
			break;

		case ITEM_HELMET:
			Engine::Render_Font(L"Font_Main_Button", L"������ �ϻ��� ���� ��ȥ �Ӹ���", &_vec2(m_fX - 100.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_HELMET:
			Engine::Render_Font(L"Font_Main_Button", L"������ �ǳ���Ų�� ����", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_ARMOR:
			Engine::Render_Font(L"Font_Main_Button", L"���� ���帰�� ���� ����", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_ARMOR:
			Engine::Render_Font(L"Font_Main_Button", L"ȥ ��Ÿ��ī ����", &_vec2(m_fX - 60.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_SHORDER:
			Engine::Render_Font(L"Font_Main_Button", L"��ũ�Ͻ� ��ȥ�� ��ī��", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_SHORDER:
			Engine::Render_Font(L"Font_Main_Button", L"�߸����� ����", &_vec2(m_fX - 40.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_LEG:
			Engine::Render_Font(L"Font_Main_Button", L"���ֹ� �ż��� ��ȥ ����", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_LEG:
			Engine::Render_Font(L"Font_Main_Button", L"���� ������ ���� ����", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_GLOVE:
			Engine::Render_Font(L"Font_Main_Button", L"��ġ ������ ȭ�� �尩", &_vec2(m_fX - 70.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_GLOVE:
			Engine::Render_Font(L"Font_Main_Button", L"�ö��� ������ �尩", &_vec2(m_fX - 60.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_BOOTS:
			Engine::Render_Font(L"Font_Main_Button", L"������ �츣������ �Ź�", &_vec2(m_fX - 70.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_BOOTS:
			Engine::Render_Font(L"Font_Main_Button", L"�� �׸޸��� �Ź�", &_vec2(m_fX - 50.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_AMULET:
			Engine::Render_Font(L"Font_Main_Button", L"���� �ý��� ������ �����", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			break;

		case ITEM_ARMLET:
			Engine::Render_Font(L"Font_Main_Button", L"���̵� ������ ����", &_vec2(m_fX - 60.f, m_fY - 20.f), D3DXCOLOR(0.2f, 0.2f, 1.f, 1.f));
			break;

		case ITEM_BELT:
			Engine::Render_Font(L"Font_Main_Button", L"��ī�Ǿ��� ��Ʈ", &_vec2(m_fX - 50.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.3f, 0.f, 1.f));
			break;

		case ITEM_EARING:
			Engine::Render_Font(L"Font_Main_Button", L"���� �������� ���� �Ͱ���", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.5f, 1.f));
			break;

		case ITEM_RING:
			Engine::Render_Font(L"Font_Main_Button", L"�븣��Ʈ �������� ��ȥ ����", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_RING2:
			Engine::Render_Font(L"Font_Main_Button", L"�дٸ� ������ ����", &_vec2(m_fX - 60.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			break;

		case ITEM_HPPOTION:
			Engine::Render_Font(L"Font_Main_Button", L"HP ����", &_vec2(m_fX - 30.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			break;

		case ITEM_MPPOTION:
			Engine::Render_Font(L"Font_Main_Button", L"MP ����", &_vec2(m_fX - 30.f, m_fY - 20.f), D3DXCOLOR(0.2f, 0.2f, 1.f, 1.f));
			break;

		case ITEM_LEVELPOTION:
			Engine::Render_Font(L"Font_Main_Button", L"������ ����", &_vec2(m_fX - 30.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		}
	}

	Engine::Render_Font(L"Font_Main_Button", L"�Ǹ� �Ͻðڽ��ϱ�?", &_vec2(m_fX - 40.f, m_fY + 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


}


HRESULT CSell_Base::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Sell_Base");
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

CSell_Base* CSell_Base::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CSell_Base*		pInstance = new CSell_Base(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSell_Base::Free(void)
{
	CUI::Free();
}
