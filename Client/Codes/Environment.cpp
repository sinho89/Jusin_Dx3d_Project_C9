#include "stdafx.h"
#include "Environment.h"

#include "Export_Engine.h"
#include "LightCamera.h"
#include "Map_Store.h"

#include "Bardiel.h"
//클라 환경 오브젝트

CEnvironment::CEnvironment(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_pRendererCom(NULL)
, m_pResourceMgr(NULL)
, m_pColliderCom(NULL)
, m_pTargetAnimator(NULL)
, m_pTargetMesh(NULL)
, m_lMouseMove(0)
, m_fAlpha(1.0f)
, m_bStop(false)
, m_bComplete(false)
, m_bCheck(false)
, m_bBardielObject(FALSE)
{
	ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}
CEnvironment::~CEnvironment(void)
{

}

HRESULT CEnvironment::Start_ForScene(void)
{
	if (L"Mesh_Bardiel_Sealed" == m_wstrName)
	{
		Engine::CGameObject* pGameObject = Engine::Find(L"Mesh_Bardiel");
		if (pGameObject == NULL)
			return E_FAIL;

		m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);
		m_pTargetAnimator = ((CBardiel*)pGameObject)->GetAnimatorInfo();
	}	

	return S_OK;
}

void CEnvironment::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_ColliderGroup(Engine::COL_BOX, L"Static", m_pColliderCom);
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (m_pRendererCom->GetShadowBuffer() ||
		m_pRendererCom->GetSoftShadow() ||
		m_pRendererCom->GetCACDShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC_SHADOW, this);
	}

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_STAGE_DEPTH, this);

	if (m_wstrName == L"Ice_Rock_1")
	{

	}
	_vec3	vPos;
	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos));

}

void CEnvironment::Render_Object(void)
{
	/*LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect) return;

	pEffect->AddRef();

	_uint iPassIdx = 0;

	switch (Engine::GetMRTID())
	{
	case Engine::MRT_DEFERRED_1:
		iPassIdx = 0;
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case Engine::MRT_DEFERRED_2:
		iPassIdx = 1;
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case Engine::MRT_DEFERRED_3:
		iPassIdx = 4;
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case Engine::MRT_DEFERRED_4:
		return;
	}

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);

	m_pMeshCom->Render_Mesh(pEffect, iPassIdx);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);*/
	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

void CEnvironment::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (m_pMeshCom == NULL)
		return;

	switch (iPassIdx)
	{
	case 0:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 2:
		if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
		break;

	case 3:
		return;

	case 4:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 5:
		if (FAILED(Set_CACD_Shadow_ConstanTable(pEffect))) return;
		break;

	case 6:
		return;
	}

	if (L"Mesh_Bardiel_Sealed" == m_wstrName)
	{
		if (!m_bBardielObject)
		{
			if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::WAKEUP)
			{
				_double dTargetTime = m_pTargetMesh->Get_TrackPos();

				if (1.0f < dTargetTime)
					m_bBardielObject = TRUE;
			}
		}
		else
			return;
	}

	m_pMeshCom->Render_Mesh(pEffect, iPassIdx);
}


HRESULT CEnvironment::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

	return S_OK;
}

HRESULT CEnvironment::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//m_wstrName == L"Mesh_Cristal_1"

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	TCHAR szName[MAX_PATH] = L"";

	wstring::iterator iter = m_wstrName.begin();
	wstring::iterator iter_end = m_wstrName.end();

	int i = 0;

	for(; iter != iter_end; ++iter)
	{
		szName[i] = (*iter);
		++i;
	}

	if (m_wstrName == L"Mesh_Torch1")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}
	else if (m_wstrName == L"Mesh_Torch2")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}
	else if (m_wstrName == L"Mesh_Cristal_1")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}
	else if (m_wstrName == L"Mesh_Cristal_2")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}
	else if (m_wstrName == L"Mesh_Ice_Rock_1")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}
	else if (m_wstrName == L"Mesh_Lamp_Bone_A")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}
	else if (m_wstrName == L"Mesh_Lamp_Bone_B")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}
	else if (m_wstrName == L"Mesh_Lamp_Bone_C")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}
	else if (m_wstrName == L"Mesh_Bardiel_Sealed")
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_FIRE, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));		
	}

	else
	{
		pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_TOWN, szName);
		m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));
	}

	if (m_wstrName == L"Mesh_BookShelf")
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 7.f, 0.f), _vec3(6.f, 15.f, 3.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	else if (m_wstrName == L"Mesh_StoreTable")
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 2.f, 0.f), _vec3(8.f, 4.f, 5.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	else if (m_wstrName == L"Mesh_StoreTable2")
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 2.f, 0.f), _vec3(5.f, 3.f, 7.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	else if (m_wstrName == L"Mesh_Cristal_1")
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 3.f, 0.f), _vec3(2.f, 7.f, 2.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	else if (m_wstrName == L"Mesh_Cristal_2")
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 7.f, 0.f), _vec3(7.f, 13.f, 7.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	else if (m_wstrName == L"Mesh_Lamp_Bone_B")
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 7.f, 0.f), _vec3(9.f, 13.f, 9.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	else if (m_wstrName == L"Mesh_Lamp_Bone_C")
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 6.f, 0.f), _vec3(10.f, 10.f, 10.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	else if (m_wstrName == L"Mesh_Ice_Rock_1")
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 15.f, 0.f), _vec3(22.f, 30.f, 15.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	else
	{
		pComponent = Engine::Create_BoxCollider(m_pGraphicDev, m_pTransCom, L"Static", _vec3(0.f, 0.f, 0.f), _vec3(0.0001f, 0.0001f, 0.0001f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL) return E_FAIL;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"BoxCollider", pComponent));
	}
	
	if (m_pMeshCom == NULL)
	{
		m_wstrName;
		MSG_BOX("Environent Created Failed");
		return E_FAIL;
	}

	m_pColliderCom->Set_Render(true);

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticMesh"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

HRESULT CEnvironment::Set_ConstantTable( LPD3DXEFFECT pEffect )
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vLightDir", &_vec4(1.0f, -1.0f, 1.0f, 0.0f));

	//const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	//pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 70.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);
		
	pEffect->SetBool("g_bSpecularCheck", true);

	pEffect->SetBool("g_bOutline", m_bOutlineCheck);

	pEffect->SetBool("g_bLimLight", true);

	if (Engine::GetLimLight())
	{
		if (m_wstrName == L"Mesh_Cristal_1")
			pEffect->SetFloat("g_bOutlinePower", 2.0f);
		else
			pEffect->SetFloat("g_bOutlinePower", 20.0f);

		if (Engine::SCENE_SELECT == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 1.0f, 1.0f, 0.0f));
		else if (Engine::SCENE_TOWN == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 0.8549f, 0.7254f, 0.0f));
		else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
		{
			if (m_wstrName == L"Mesh_Cristal_1")
				pEffect->SetVector("g_vColor", &_vec4(0.8549f, 0.4392f, 0.8392f, 0.0f));
			else
				pEffect->SetVector("g_vColor", &_vec4(0.5294f, 0.8078f, 0.9215f, 0.0f));
		}
		else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
			pEffect->SetVector("g_vColor", &_vec4(1.0f, 0.6274f, 0.4784f, 0.0f));
	}

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CEnvironment::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = Engine::GetCamera(Engine::CAMERA_LIGHT)->GetMatView();
	matLightProj = Engine::GetCamera(Engine::CAMERA_LIGHT)->GetMatProj();

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CEnvironment::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	CLightCamera* pLightCam = (CLightCamera*)Engine::GetCamera(Engine::CAMERA_LIGHT);

	pEffect->SetMatrix("g_matLightView", &pLightCam->GetMatView());

	if (0 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(0));
	else if (1 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(1));
	else if (2 == m_pRendererCom->GetCACDCount())
		pEffect->SetMatrix("g_matLightProj", &pLightCam->Get_CACD_Proj(2));

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CEnvironment* CEnvironment::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CEnvironment*		pInstance = new CEnvironment(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CEnvironment Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnvironment::Free(void)
{
	Engine::CGameObject::Free();

}

void CEnvironment::Stop( void )
{
	m_bStop = true;
}

void CEnvironment::Complete( void )
{
	m_bComplete = true;
}


Engine::CTransform* CEnvironment::GetTransCom( void )
{
	return m_pTransCom;
}

_bool CEnvironment::GetChecked( void )
{
	return m_bCheck;
}


OBJ_INFO* CEnvironment::GetObjectInfo( void )
{
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_tInfo.m_vPos);
	m_pTransCom->Get_Angle(&m_tInfo.m_vAngle);
	m_pTransCom->Get_Scale(&m_tInfo.m_vScale);


	wstring::iterator iter = m_wstrName.begin();
	wstring::iterator iter_end = m_wstrName.end();

	int i = 0;

	for(; iter != iter_end; ++iter)
	{
		m_tInfo.m_szName[i] = (*iter);
		++i;
	}
	
	return &m_tInfo;
}

void CEnvironment::SetObjectInfo( _vec3* vPos, _vec3* vScale, _vec3* vAngle )
{
	m_pTransCom->Set_Position(vPos);
	m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
}


