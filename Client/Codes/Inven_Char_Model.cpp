#include "stdafx.h"
#include "Inven_Char_Model.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Camera.h"
#include "Rect_Texture.h"
#include "WitchBladeWeapon.h"
#include "MisticWeapon.h"
#include "Model_WB_Weapon.h"

CInven_Char_Model::CInven_Char_Model(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CInven_Space(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pMeshCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_fFrame(0.0f)
	, m_iAniIndex(2)
	, m_fRenderZ(0.0f)
{

}

CInven_Char_Model::~CInven_Char_Model(void)
{

}

HRESULT CInven_Char_Model::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 vInfo = m_pParentWindow->GetInfo();

	m_vParentBasePos.x = 0.f;
	m_vParentBasePos.y = 0.f;

	m_fX = vInfo.x - m_vParentBasePos.x;
	m_fY = vInfo.y - m_vParentBasePos.y;

	m_fSizeX = 50.f;
	m_fSizeY = 50.f;

	m_fAngle = 180.f;

	m_fAlpha = 1.f;
	m_bLight = true;

	m_pMeshCom->Set_AnimationSet(m_iAniIndex);
	m_pTransCom->Rotation(Engine::ROT_Y, 180.f);
	m_eWinType = WINDOW_INVEN;



	return S_OK;
}

HRESULT CInven_Char_Model::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;


	if (!m_bType && !m_bOnceStart)
	{
		m_bOnceStart = true;

		CModel_WB_Weapon* pWeaponR = CModel_WB_Weapon::Create(m_pGraphicDev, L"Model_Sword1", m_wstrName.c_str(), L"UI_Sword1R", m_pParentWindow, WB_WEAPON_R);
		if (pWeaponR == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pWeaponR);

		CModel_WB_Weapon* pWeaponL = CModel_WB_Weapon::Create(m_pGraphicDev, L"Model_SwordL1", m_wstrName.c_str(), L"UI_Sword1L", m_pParentWindow, WB_WEAPON_L);
		if (pWeaponL == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pWeaponL);

		CModel_WB_Weapon* pWeaponR_In = CModel_WB_Weapon::Create(m_pGraphicDev, L"Model_Sword1_In", m_wstrName.c_str(), L"UI_Sword1R_In", m_pParentWindow, WB_WEAPON_R_IN);
		if (pWeaponR_In == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pWeaponR_In);

		CModel_WB_Weapon* pWeaponL_In = CModel_WB_Weapon::Create(m_pGraphicDev, L"Model_SwordL1_In", m_wstrName.c_str(), L"UI_Sword1L_In", m_pParentWindow, WB_WEAPON_L_IN);
		if (pWeaponL_In == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pWeaponL_In);

	}
	else if(m_bType && !m_bOnceStart)
	{
		m_bOnceStart = true;

		CModel_WB_Weapon* pWeaponR = CModel_WB_Weapon::Create(m_pGraphicDev, L"Model_MisticWeapon", m_wstrName.c_str(), L"UI_MisticWeapon1", m_pParentWindow, M_WEAPON);
		if (pWeaponR == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pWeaponR);
		
		CModel_WB_Weapon* pWeaponR_In = CModel_WB_Weapon::Create(m_pGraphicDev, L"Model_MisticWeapon_In", m_wstrName.c_str(), L"UI_MisticWeapon1_In", m_pParentWindow, M_WEAPON_IN);
		if (pWeaponR_In == NULL)
			return E_FAIL;
		Engine::AddObject(Engine::LAYER_UI, L"UI_Window", pWeaponR_In);

	}

	return S_OK;
}

void CInven_Char_Model::Update_Object(const _float& fTimeDelta)
{

	if (m_pParentWindow->GetRender())
	{
		m_iAniIndex = 2;

		//m_pTransCom->Rotation(Engine::ROT_Y, 180.f);

		_vec4 vInfo = m_pParentWindow->GetInfo();

		m_fX = vInfo.x - m_vParentBasePos.x;
		m_fY = vInfo.y - m_vParentBasePos.y;

		D3DXMATRIX matView;

		CUI::Update_Object(fTimeDelta);
		m_wSortNum = m_pParentWindow->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 200.0f);

		if (m_pMeshCom->is_AnimationSetEnd())
		{
			m_pMeshCom->Set_AnimationSet(m_iAniIndex);
			m_pMeshCom->Set_TrackPos(0.0);
		}
	}
	else
	{
		m_fAngle = 180.f;
		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
	}

}

void CInven_Char_Model::Render_Object(void)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect) return;

	pEffect->AddRef();

	if (FAILED(Set_ConstantTable(pEffect))) return;

	_matrix				matWorld, matView;
	_matrix				matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pTransCom->Get_WorldMatrix(&matWorld);
	matWorld._11 = 30.f;
	matWorld._22 = 30.f;
	matWorld._33 = 30.f;

	D3DXMATRIX matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fAngle));
	matWorld = matWorld * matRotY;

	matWorld._41 = m_fX - WINSIZEX * 0.5f;
	matWorld._42 = -m_fY + WINSIZEY * 0.5f;
	matWorld._43 = 100.f;

	m_matUITransform = matWorld;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	Engine::Safe_Release(pEffect);

}


HRESULT CInven_Char_Model::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	if (!m_bType)
	{
		pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_WitchBlade"));
		if (NULL == pComponent) return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model", pComponent));
	}
	else
	{
		pComponent = m_pMeshCom = static_cast<Engine::CDynamicMesh*>(Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_Mistic"));
		if (NULL == pComponent) return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model", pComponent));
	}

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_Player");
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

HRESULT CInven_Char_Model::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	//m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	//pEffect->SetMatrix("g_matWorld", &matWorld);
	//pEffect->SetMatrix("g_matView", &matView);
	//pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vLightDir", &_vec4(0.5f, 1.0f, 1.0f, 1.0f));
	pEffect->SetVector("g_vLightDiffuse", &_vec4(1.0f, 1.0f, 1.0f, 1.0f));
	pEffect->SetVector("g_vLightAmbient", &_vec4(0.6f, 0.6f, 0.6f, 1.0f));
	pEffect->SetVector("g_vLightSpecular", &_vec4(0.2f, 0.2f, 0.2f, 1.0f));

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CInven_Char_Model* CInven_Char_Model::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CInven_Char_Model*		pInstance = new CInven_Char_Model(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CInven_Char_Model* CInven_Char_Model::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI* pParentWindow, _bool bType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CInven_Char_Model*		pInstance = new CInven_Char_Model(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetType(bType);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("COpening_Banner Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}
void CInven_Char_Model::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}


void CInven_Char_Model::Free(void)
{
	//::Safe_Release(m_pParentWindow);
	CInven_Space::Free();
}
