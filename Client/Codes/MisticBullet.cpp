#include "stdafx.h"
#include "MisticBullet.h"

CMisticBullet::CMisticBullet(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: Engine::CGameObject(pGraphicDev, wstrName, Engine::STATE_DEFAULT, false)
, m_vDir(0.f, 0.f, 0.f)
, m_vOldPos(0.f, 0.f, 0.f)
, m_fSpeed(0.f)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
, m_pColliderCom(NULL)
, m_pMesh(NULL)
{
	ZeroMemory(&m_tColInfo, sizeof(Engine::COLINFO));
	m_bIsStatic = true;
}

CMisticBullet::~CMisticBullet(void)
{

}

void CMisticBullet::Shot_Bullet(const _vec3 & vPos, const _vec3 & vDir, const Engine::COLINFO & tColInfo)
{
	m_bIsActive = true;
	m_vOldPos = vPos;
	m_pTransCom->Set_Position(&vPos);
	m_vDir = vDir;
	memcpy(&m_tColInfo, &tColInfo, sizeof(Engine::COLINFO));
	m_pColliderCom->Set_ColInfo(&m_tColInfo);
	Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Player", m_pColliderCom);
}

void CMisticBullet::Set_MapMesh(LPD3DXMESH * pMesh)
{
	if (pMesh == NULL)
		return;

	m_mapMesh.insert(MAPMESH::value_type(m_iMeshSize, pMesh));
	m_iMeshSize = m_mapMesh.size();
}

void CMisticBullet::ClearMap(void)
{
	m_mapMesh.clear();
	m_iMeshSize = 0;
}

void CMisticBullet::Update_Object(const _float & fTimeDelta)
{
	LimitDistance();
	Collision();

	_vec3 vPos = m_vDir * m_fSpeed * fTimeDelta;
	m_pTransCom->Move_Position(&vPos);

	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);
}

void CMisticBullet::Render_Object(void)
{
	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

void CMisticBullet::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	switch (iPassIdx)
	{
	case 0:
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
		break;

	case 2:
		return;
		break;

	case 3:
		return;
		break;

	case 4:
		if (FAILED(SetUp_ConstantTable(pEffect))) return;
		break;
	}

	m_pMesh->Render_Mesh(pEffect, iPassIdx);
}

HRESULT CMisticBullet::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSpeed = 60.f;
	m_bIsActive = false;
	m_pTransCom->Set_Scale(0.5f, 0.5f, 0.5f);
	return S_OK;
}

HRESULT CMisticBullet::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"0", pComponent));

	//Renderer
	pComponent = Engine::Get_Renderer();
	if (pComponent == NULL)
		return E_FAIL;
	m_pRendererCom = (Engine::CRenderer*)pComponent;
	m_pRendererCom->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));

	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_MisticBullet");
	if (pComponent == NULL)
		return E_FAIL;
	m_pMesh = (Engine::CStaticMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	//Shader
	pComponent = Engine::Clone_Prototype(L"Shader_StaticMesh");
	if (pComponent == NULL)
		return E_FAIL;
	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	//Collider
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Player", _vec3(0.f, 0.f, 0.f), 0.5f);
	if (pComponent == NULL)
		return E_FAIL;
	m_pColliderCom = (Engine::CSphereCollider*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"SphereCol", pComponent));
	m_pColliderCom->SetScale(3.f);
	return S_OK;
}

HRESULT CMisticBullet::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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


	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 20.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMisticBullet::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	return S_OK;
}

HRESULT CMisticBullet::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	return S_OK;
}

void CMisticBullet::LimitDistance(void)
{
	_vec3 vPos; 
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);

	_float fDist = D3DXVec3Length(&(vPos - m_vOldPos));

	if (fDist > 100.f)
	{
		m_bIsActive = false;
		Engine::Delete_Collider(L"Player", m_pColliderCom);
	}
}

void CMisticBullet::Collision(void)
{

}

CMisticBullet * CMisticBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CMisticBullet* pInstance = new CMisticBullet(pGraphicDev, wstrName);
	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CMisticBullet Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMisticBullet::Free(void)
{
	Engine::Delete_Collider(L"Player", m_pColliderCom);
	Engine::CGameObject::Free();
}
