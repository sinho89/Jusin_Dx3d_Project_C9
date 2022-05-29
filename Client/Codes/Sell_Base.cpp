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
			Engine::Render_Font(L"Font_Main_Button", L"전설의 사라드 아크 블레이드", &_vec2(m_fX - 100.f, m_fY - 20.f), D3DXCOLOR(0.2f, 0.2f, 1.f, 1.f));
			break;

		case ITEM_RED_WEAPON:
			Engine::Render_Font(L"Font_Main_Button", L"이단자 제시의 아크 블레이드", &_vec2(m_fX - 100.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			break;

		case ITEM_S_BLUE_WEAPON:
			Engine::Render_Font(L"Font_Main_Button", L"데스티네이션", &_vec2(m_fX - 40.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_S_RED_WEAPON:
			Engine::Render_Font(L"Font_Main_Button", L"헬 버스터", &_vec2(m_fX - 30.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.3f, 0.f, 1.f));
			break;

		case ITEM_HELMET:
			Engine::Render_Font(L"Font_Main_Button", L"마쉬아 암살자 쑨의 영혼 머리띠", &_vec2(m_fX - 100.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_HELMET:
			Engine::Render_Font(L"Font_Main_Button", L"마스터 피나르킨의 투구", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_ARMOR:
			Engine::Render_Font(L"Font_Main_Button", L"소피 샹드린의 물결 상의", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_ARMOR:
			Engine::Render_Font(L"Font_Main_Button", L"혼 스타니카 상의", &_vec2(m_fX - 60.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_SHORDER:
			Engine::Render_Font(L"Font_Main_Button", L"뤼크니스 영혼의 스카프", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_SHORDER:
			Engine::Render_Font(L"Font_Main_Button", L"발리도르 견장", &_vec2(m_fX - 40.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_LEG:
			Engine::Render_Font(L"Font_Main_Button", L"이주민 신성한 영혼 하의", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_LEG:
			Engine::Render_Font(L"Font_Main_Button", L"사라드 명장의 정령 하의", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_GLOVE:
			Engine::Render_Font(L"Font_Main_Button", L"핀치 일족의 화염 장갑", &_vec2(m_fX - 70.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_GLOVE:
			Engine::Render_Font(L"Font_Main_Button", L"올라프 부족의 장갑", &_vec2(m_fX - 60.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_BOOTS:
			Engine::Render_Font(L"Font_Main_Button", L"마스터 우르슬라의 신발", &_vec2(m_fX - 70.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;
		case ITEM_S_BOOTS:
			Engine::Render_Font(L"Font_Main_Button", L"드 네메리아 신발", &_vec2(m_fX - 50.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_AMULET:
			Engine::Render_Font(L"Font_Main_Button", L"스톰 컬스가 전유의 목걸이", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			break;

		case ITEM_ARMLET:
			Engine::Render_Font(L"Font_Main_Button", L"루이드 교단의 팔찌", &_vec2(m_fX - 60.f, m_fY - 20.f), D3DXCOLOR(0.2f, 0.2f, 1.f, 1.f));
			break;

		case ITEM_BELT:
			Engine::Render_Font(L"Font_Main_Button", L"오카피아의 벨트", &_vec2(m_fX - 50.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.3f, 0.f, 1.f));
			break;

		case ITEM_EARING:
			Engine::Render_Font(L"Font_Main_Button", L"랜드 블래스가의 전승 귀걸이", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.5f, 1.f));
			break;

		case ITEM_RING:
			Engine::Render_Font(L"Font_Main_Button", L"노르간트 점령지의 영혼 반지", &_vec2(m_fX - 80.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		case ITEM_RING2:
			Engine::Render_Font(L"Font_Main_Button", L"론다르 결사대의 반지", &_vec2(m_fX - 60.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			break;

		case ITEM_HPPOTION:
			Engine::Render_Font(L"Font_Main_Button", L"HP 물약", &_vec2(m_fX - 30.f, m_fY - 20.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
			break;

		case ITEM_MPPOTION:
			Engine::Render_Font(L"Font_Main_Button", L"MP 물약", &_vec2(m_fX - 30.f, m_fY - 20.f), D3DXCOLOR(0.2f, 0.2f, 1.f, 1.f));
			break;

		case ITEM_LEVELPOTION:
			Engine::Render_Font(L"Font_Main_Button", L"레벨업 물약", &_vec2(m_fX - 30.f, m_fY - 20.f), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
			break;

		}
	}

	Engine::Render_Font(L"Font_Main_Button", L"판매 하시겠습니까?", &_vec2(m_fX - 40.f, m_fY + 10.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


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
