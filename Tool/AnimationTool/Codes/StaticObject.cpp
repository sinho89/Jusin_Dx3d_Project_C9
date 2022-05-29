#include "stdafx.h"
#include "StaticObject.h"

CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pMeshCom(NULL)
, m_pShaderCom(NULL)
, m_pCalculatorCom(NULL)
, m_pColliderCom(NULL)
, m_pmatTarget(NULL)
, m_pmatWeapon(NULL)
, m_bIsSelect(false)
, m_fAlpha(1.f)
{

}

CStaticObject::~CStaticObject(void)
{

}

_bool CStaticObject::GetMouseCol(POINT ptMouse)
{
	if(m_pCalculatorCom->Picking_Mesh(ptMouse, WINSIZEX, WINSIZEY, m_pMeshCom->Get_Mesh(),
		m_pTransCom->Get_WorldMatrixPointer()))
	{
		m_bIsSelect = true;
		return true;
	}
	return false;
}

_bool CStaticObject::IsSelected(void)
{
	return m_bIsSelect;
}

const _vec3* CStaticObject::Get_Angle(_vec3* pAngle)
{
	return m_pTransCom->Get_Angle(pAngle);
}

void CStaticObject::SetSelected(_bool bIsSelect)
{
	m_bIsSelect = bIsSelect;
}

void CStaticObject::SetBone(Engine::CGameObject* pGameObject, const char* pBoneName)
{
	if(pGameObject == NULL)
		return;

	if(pBoneName == NULL)
		return;

	Engine::CDynamicMesh* pMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);
	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pmatTarget = pTransform->Get_WorldMatrixPointer();
	m_pmatWeapon = pMesh->Get_FrameByName(pBoneName);
	
	Engine::Safe_Release(pTransform);
	Engine::Safe_Release(pMesh);
}

void CStaticObject::SetPos(const _vec3& vPos)
{
	m_pTransCom->Set_Position(&vPos);
}

void CStaticObject::SetScale(const _vec3& vScale)
{
	m_pTransCom->Set_Scale(vScale.x, vScale.y, vScale.z);
}

void CStaticObject::SetRot(const _vec3& vRot)
{
	m_pTransCom->Rotation(Engine::ROT_X, vRot.x);
	m_pTransCom->Rotation(Engine::ROT_Y, vRot.y);
	m_pTransCom->Rotation(Engine::ROT_Z, vRot.z);
}

void CStaticObject::Set_Collider(const _uint& iType)
{
	if (m_pColliderCom != NULL)
		return;

	Engine::CComponent* pComponent = NULL;

	switch (iType)
	{
	case Engine::COL_SPHERE:
		pComponent = Engine::CSphereCollider::Create(m_pGraphicDev, m_pTransCom, L"Player", _vec3(0.f, 0.f, 0.f), 1.f);
		if (pComponent == NULL)
			return;
		m_pColliderCom = (Engine::CSphereCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));
		break;

	case Engine::COL_BOX:
		pComponent = Engine::CBoxCollider::Create(m_pGraphicDev, m_pTransCom, L"Player", _vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if (pComponent == NULL)
			return;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));
		break;
	}
}

void CStaticObject::Delete_Collider(void)
{
	MAPCOMPONENT::iterator iter = find_if(m_mapComponent[Engine::COMID_DYNAMIC].begin(), m_mapComponent[Engine::COMID_DYNAMIC].end(), Engine::CTag_Finder(L"Collider"));

	if (iter == m_mapComponent[Engine::COMID_DYNAMIC].end())
		return;

	Engine::Safe_Release(iter->second);
	m_pColliderCom = NULL;
	iter = m_mapComponent[Engine::COMID_DYNAMIC].erase(iter);
}

void CStaticObject::Set_ColliderPos(const _vec3& vPos)
{
	if (m_pColliderCom == NULL)
		return;
	m_pColliderCom->SetPos(vPos);
}

void CStaticObject::Set_ColliderSize(const _vec3& vSize)
{
	if (m_pColliderCom == NULL)
		return;
	m_pColliderCom->SetScale(vSize);
}

void CStaticObject::Set_ColliderSize(const _float& fRad)
{
	if (m_pColliderCom == NULL)
		return;
	m_pColliderCom->SetScale(fRad);
}

void CStaticObject::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	_vec3 vPos;
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
	Compute_ViewZ(&vPos);

	m_pTransCom->Set_ParentMatrix(m_pmatWeapon);
	m_pTransCom->Set_ParentMatrix(m_pmatTarget);

	if(m_bIsSelect)
		m_fAlpha = 0.5f;
	else
		m_fAlpha = 1.f;
}

void CStaticObject::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect)
		return;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	if (m_pColliderCom)
		m_pColliderCom->Render_Collider();
}

HRESULT CStaticObject::Ready_Object(const wstring& wstrName)
{
	if(FAILED(Add_Component(wstrName)))
		return E_FAIL;


	return S_OK;
}

HRESULT CStaticObject::Add_Component(const wstring& wstrName)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_STAGE, wstrName.c_str());
	if(pComponent == NULL)
		return E_FAIL;
	m_pMeshCom = (Engine::CStaticMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	//Renderer
	pComponent = Engine::Get_Renderer();
	if(pComponent == NULL)
		return E_FAIL;
	m_pRendererCom = (Engine::CRenderer*)pComponent;
	m_pRendererCom->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));

	//Shader
	pComponent = Engine::Clone_Prototype(L"Shader_Object");
	if(pComponent == NULL)
		return E_FAIL;
	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	//Calculator
	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	if(pComponent == NULL)
		return E_FAIL;
	m_pCalculatorCom = (Engine::CCalculator*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Calculator", pComponent));

	return S_OK;
}

HRESULT CStaticObject::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

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

	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

CStaticObject* CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CStaticObject*		pInstance = new CStaticObject(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object(wstrName)))
	{
		MSG_BOX("CStaticObject Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticObject::Free(void)
{
	Engine::CGameObject::Free();
}

