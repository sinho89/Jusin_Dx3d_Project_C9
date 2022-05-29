#include "stdafx.h"
#include "Model_WB_Weapon.h"
#include "Export_Engine.h"
#include "Texture.h"
#include "Camera.h"
#include "Rect_Texture.h"
#include "WitchBladeWeapon.h"
#include "MisticWeapon.h"
#include "Inven_Char_Model.h"
#include "Player.h"

CModel_WB_Weapon::CModel_WB_Weapon(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
	:CInven_Space(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pMeshCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_fFrame(0.0f)
	, m_iAniIndex(2)
	, m_fRenderZ(0.0f)
	, m_pmatModelTarget(NULL)
	, m_pmatModelWeapon(NULL)
	, m_pWorldPlayer(NULL)
	, m_pPlayerAniCom(NULL)
	, m_bUpdate(false)
{

}

CModel_WB_Weapon::~CModel_WB_Weapon(void)
{

}

HRESULT CModel_WB_Weapon::Ready_Object(const _tchar* pTargetName, const _tchar* pFilePath)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	LoadModelWeaponData(pTargetName, pFilePath);

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

	m_eWinType = WINDOW_INVEN;
	m_bRender = false;
	m_bIsStatic = true;
	return S_OK;
}

HRESULT CModel_WB_Weapon::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	m_pTargetUI = ((CUI*)Engine::Find(L"Inven_Model"));
	::Safe_Release(m_pTargetUI);

	m_pWorldPlayer = ((CPlayer*)Engine::Find(L"Player"));
	::Safe_Release(m_pWorldPlayer);

	return S_OK;
}

void CModel_WB_Weapon::Update_Object(const _float& fTimeDelta)
{
	
	m_pPlayerAniCom = ((Engine::CAnimator*)((Engine::CGameObject*)m_pWorldPlayer)->GetComponent<Engine::CAnimator>(Engine::COMID_DYNAMIC));

	if (m_pWorldPlayer->GetPlayerInfo()->m_eClassID == CLASS_WITHBLADE)
	{
		if (m_pPlayerAniCom->Get_State() == PLAYER_FIGHT)
		{
			if (m_eMeshType == WB_WEAPON_L || m_eMeshType == WB_WEAPON_R)
				m_bUpdate = true;
			else
				m_bUpdate = false;
		}
		else
		{
			if (m_eMeshType == WB_WEAPON_L_IN || m_eMeshType == WB_WEAPON_R_IN)
				m_bUpdate = true;
			else
				m_bUpdate = false;
		}
	}
	else
	{
		if (m_pPlayerAniCom->Get_State() == PLAYER_FIGHT)
		{
			if (m_eMeshType == M_WEAPON)
				m_bUpdate = true;
			else
				m_bUpdate = false;
		}
		else
		{
			if (m_eMeshType == M_WEAPON_IN)
				m_bUpdate = true;
			else
				m_bUpdate = false;
		}
	}
	

	if (m_pParentWindow->GetRender() && m_bRender && m_bUpdate)
	{
		m_iAniIndex = 2;

		//m_pTransCom->Rotation(Engine::ROT_Y, 180.f);

		_vec4 vInfo = m_pParentWindow->GetInfo();

		m_fX = vInfo.x - m_vParentBasePos.x;
		m_fY = vInfo.y - m_vParentBasePos.y;

		D3DXMATRIX matView;

		CUI::Update_Object(fTimeDelta);
		m_pTransCom->Set_ParentMatrix(m_pmatModelWeapon);
		//m_pTransCom->Set_ParentMatrix(m_pmatTarget);

		m_wSortNum = m_pParentWindow->GetSortNum();
		m_pRendererCom->Add_WindowUI(m_wSortNum, this);

		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 200.0f);

	}
	else
	{
		m_fAngle = 180.f;
		m_bBarCollision = false;
		m_bCollision = false;
		m_bClick = false;
	}

	Engine::Safe_Release(m_pPlayerAniCom);

}

void CModel_WB_Weapon::Render_Object(void)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect) return;

	pEffect->AddRef();

	if (FAILED(Set_ConstantTable(pEffect))) return;

	_matrix				matWorld, matView, matUITransForm;
	_matrix				matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matUITransForm = *(((CInven_Char_Model*)m_pTargetUI)->GetUITransForm());
	m_pTransCom->Get_WorldMatrix(&matWorld);
	
	matWorld *= matUITransForm;
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

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


HRESULT CModel_WB_Weapon::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	switch (m_eMeshType)
	{
	case WB_WEAPON_L:
		pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_UI_SwordL1"));
		if (NULL == pComponent) return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model", pComponent));
		break;
	case WB_WEAPON_R:
		pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_UI_Sword1"));
		if (NULL == pComponent) return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model", pComponent));
		break;
	case WB_WEAPON_L_IN:
		pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_UI_SwordL1"));
		if (NULL == pComponent) return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model", pComponent));
		break;
	case WB_WEAPON_R_IN:
		pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_UI_Sword1"));
		if (NULL == pComponent) return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model", pComponent));
		break;
	case M_WEAPON:
		pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_MisticWeapon"));
		if (NULL == pComponent) return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model", pComponent));
		break;
	case M_WEAPON_IN:
		pComponent = m_pMeshCom = static_cast<Engine::CStaticMesh*>(Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_MisticWeapon"));
		if (NULL == pComponent) return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Model", pComponent));
		break;
	}

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_Object");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
	
	return S_OK;
}

HRESULT CModel_WB_Weapon::Set_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CModel_WB_Weapon::LoadModelWeaponData(const _tchar* pTargetName, const _tchar* pFilePath)
{
	if (pTargetName == NULL)
		return E_FAIL;

	if (pFilePath == NULL)
		return E_FAIL;

	_tchar szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, L"../Bin/Data/WeaponData/");
	lstrcat(szFullPath, pFilePath);
	lstrcat(szFullPath, L".dat");

	DWORD dwByte = 0;
	HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL)
		return E_FAIL;

	while (true)
	{
		WEAPONINFO* tWeaponInfo = new WEAPONINFO;
		ReadFile(hFile, tWeaponInfo, sizeof(WEAPONINFO), &dwByte, NULL);
		if (dwByte == 0)
		{
			Engine::Safe_Delete(tWeaponInfo);
			break;
		}

		Engine::CGameObject* pGameObject = NULL;
		pGameObject = Engine::Find(pTargetName);

		if (pGameObject == NULL)
		{
			Engine::Safe_Delete(tWeaponInfo);
			return E_FAIL;
		}
		
		Engine::CTransform* pTransform = NULL;
		pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

		if (pTransform == NULL)
		{
			Engine::Safe_Delete(tWeaponInfo);
			Engine::Safe_Release(pGameObject);
			return E_FAIL;
		}

		m_pmatModelTarget = pTransform->Get_WorldMatrixPointer();

		Engine::CDynamicMesh* pMesh = NULL;
		pMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

		if (pMesh == NULL)
		{
			Engine::Safe_Delete(tWeaponInfo);
			Engine::Safe_Release(pTransform);
			Engine::Safe_Release(pGameObject);
		}

		m_pmatModelWeapon = pMesh->Get_FrameByName(tWeaponInfo->szBoneName);

		m_pTransCom->Set_Position(&tWeaponInfo->vPos);
		m_pTransCom->Set_Angle(&tWeaponInfo->vRotate);
		m_pTransCom->Set_Scale(tWeaponInfo->vScale.x, tWeaponInfo->vScale.y, tWeaponInfo->vScale.z);

 		Engine::Safe_Release(pMesh);
		Engine::Safe_Release(pTransform);
		Engine::Safe_Release(pGameObject);
		Engine::Safe_Delete(tWeaponInfo);
	}
	CloseHandle(hFile);

	return S_OK;
}


CModel_WB_Weapon* CModel_WB_Weapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, const _tchar* pTargetName, const _tchar* pFilePath, CUI* pParentWindow, MODEL_ID eType, const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, const _bool bIsActive /*= true*/)
{
	CModel_WB_Weapon*		pInstance = new CModel_WB_Weapon(pGraphicDev, wstrName, eState, bIsActive);

	pInstance->SetParent(pParentWindow);
	pInstance->SetType(eType);

	if (FAILED(pInstance->Ready_Object(pTargetName, pFilePath)))
	{
		MSG_BOX("CModel_WB_Weapon Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}
void CModel_WB_Weapon::SetParent(CUI* pParentWindow)
{
	m_pParentWindow = pParentWindow;
}


void CModel_WB_Weapon::Free(void)
{
	//::Safe_Release(m_pParentWindow);
	CInven_Space::Free();
}
