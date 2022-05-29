#include "stdafx.h"
#include "World_HP_Gage.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "Monster.h"

CWorld_HP_Gage::CWorld_HP_Gage(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pTextureCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_pUICarCom(NULL)
	, m_fCamDist(0.0f)
	, m_fDepth(0.0f)
	, m_fHpGage(1.0f)
{

}

CWorld_HP_Gage::~CWorld_HP_Gage(void)
{

}

HRESULT CWorld_HP_Gage::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = 0.f;
	m_fY = 0.f;

	m_fSizeX = 145.f;
	m_fSizeY = 10.f;

	m_fAlpha = 1.f;
	m_fDepth = 0.f;

	CUI::Start_ForScene(); // 부모 객체 스타트포씬에서 생성됐을때 여기서 호출
	return S_OK;
}

HRESULT CWorld_HP_Gage::Start_ForScene(void)
{
	return S_OK;
}

void CWorld_HP_Gage::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	if (((CMonster*)m_pParentObject)->GetInfo()->iHp <= 0.f)
		m_eState = Engine::STATE_DESTROY;

	_vec3 vParentPos;
	((Engine::CTransform*)m_pParentObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vParentPos);

	_matrix matTempWorld, matTrans, matView, matProj;
	D3DXMatrixIdentity(&matTempWorld);
	D3DXMatrixTranslation(&matTrans, vParentPos.x, (vParentPos.y + 6.5f), vParentPos.z);

	matTempWorld = matTrans;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	_vec3 vResult = m_pUICarCom->WorldToScreenPos(&matTempWorld, &matView, &matProj, WINSIZEX, WINSIZEY);
	_vec3 vPlayerPos;
	((Engine::CTransform*)m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vPlayerPos);

	m_fCamDist = D3DXVec3Length(&_vec3(vParentPos - vPlayerPos));

	m_fAlpha = (1.f - (m_fCamDist * 0.01f));
	m_fX = vResult.x;
	m_fY = vResult.y;
	m_fDepth = vResult.z;

	m_fHpGage = (float(((CMonster*)m_pParentObject)->GetInfo()->iHp) / float(((CMonster*)m_pParentObject)->GetInfo()->iMaxHp));

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CWorld_HP_Gage::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect)
		return;

	_uint			iPassCnt;

	//SetUp_ConstantTable(pEffect);

	_matrix				matWorld, matView;
	_matrix				matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);
	m_pTransCom->Get_WorldMatrix(&matWorld);

	float m_fFar = static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->GetCamFar();

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINSIZEX * 0.5f;
	matWorld._42 = -m_fY + WINSIZEY * 0.5f;
	matWorld._43 = m_fDepth;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetFloat("g_fAlpha", m_fAlpha);
	pEffect->SetFloat("g_fDepth", m_fDepth);
	pEffect->SetFloat("g_fGage", m_fHpGage);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	pEffect->SetFloat("g_fFar", m_fFar);

	Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}
HRESULT CWorld_HP_Gage::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix	 matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");

	Engine::SetUp_OnShader(L"Target_Albedo", pEffect, "g_AlbedoTexture");

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	float m_fFar = static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->GetCamFar();

	pEffect->SetFloat("g_fFar", m_fFar);

	pEffect->SetVector("g_vAttribute", &_vec4(1.0f / WINSIZEX, 1.0f / WINSIZEY, 0.01f, 0.01f));

	Engine::Safe_Release(pEffect);

	return S_OK;
}
void CWorld_HP_Gage::Alpha_Calculation(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if (m_fAlpha <= 0.f)
		m_bAlpha_Increase = true;
	if (m_fAlpha >= 1.f)
		m_bAlpha_Increase = false;

	if (m_fAlphaTime >= 0.05f)
	{
		m_fAlphaTime = 0.f;

		if (m_bAlpha_Increase)
			m_fAlpha += 0.05f;

	}
}

HRESULT CWorld_HP_Gage::Add_Component(void)
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

	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Monster_Gage");
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

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_WorldUI");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Calculater Component
	pComponent = m_pUICarCom = Engine::CUICalculater::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_UICal", pComponent));

	return S_OK;
}

CWorld_HP_Gage* CWorld_HP_Gage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CWorld_HP_Gage*		pInstance = new CWorld_HP_Gage(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CWorld_HP_Gage* CWorld_HP_Gage::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, Engine::CGameObject* pParentObject, MONSTER_ID eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CWorld_HP_Gage*		pInstance = new CWorld_HP_Gage(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentObject);
	pInstance->SetShopType(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;

}

void CWorld_HP_Gage::Free(void)
{
	CUI::Free();
}
