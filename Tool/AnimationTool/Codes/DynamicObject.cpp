#include "stdafx.h"
#include "DynamicObject.h"

#include "Export_Engine.h"

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pMeshCom(NULL)
, m_pShaderCom(NULL)
, m_pColliderCom(NULL)
, m_fTimeDelta(0.f)
, m_dAniOldPosition(0.0)
, m_iAniIdx(0)
, m_bIsPlaying(false)
{

}

CDynamicObject::~CDynamicObject(void)
{

}

_double CDynamicObject::Get_TrackPos(void)
{
	if(m_pMeshCom == NULL)
		return 0.0;
	return m_pMeshCom->Get_TrackPos();
}

_double CDynamicObject::Get_Period(void)
{
	if(m_pMeshCom == NULL)
		return 0.0;
	return m_pMeshCom->Get_Period();
}

_double CDynamicObject::Get_Period(const _uint& iIndex)
{
	if(m_pMeshCom == NULL)
		return 0.0;

	return m_pMeshCom->Get_Period(iIndex);
}

_bool CDynamicObject::Get_Playing(void)
{
	return m_bIsPlaying;
}

_bool CDynamicObject::Is_AnimationEnd(_double dMagicNumber)
{
	return m_pMeshCom->is_AnimationSetEnd(dMagicNumber);
}

void CDynamicObject::Set_AnimationSet(const _uint& iIdx, const Engine::MOTIONINFO* pMotionInfo, _double dMagicNumber)
{
	if(m_pMeshCom == NULL)
		return;
	m_pMeshCom->Set_AnimationSet(iIdx, pMotionInfo, dMagicNumber);
}

void CDynamicObject::Set_AniTrackPos(const _double& dPosition)
{
	if(m_pMeshCom == NULL)
		return;
	m_dAniOldPosition = dPosition;
	m_pMeshCom->Set_TrackPos(dPosition);
	m_pMeshCom->Play_AnimationSet(m_fTimeDelta);
}

_double CDynamicObject::Get_OldTrackPos(void)
{
	return m_dAniOldPosition;
}

void CDynamicObject::Set_Playing(_bool bIsPlaying)
{
	m_bIsPlaying = bIsPlaying;
}

void CDynamicObject::Set_TrackSpeed(const _float& fSpeed)
{
	if(m_pMeshCom == NULL)
		return;
	m_pMeshCom->Set_TrackSpeed(fSpeed);
}

void CDynamicObject::Set_AniInfo(const Engine::MOTIONINFO* pMotionInfo)
{
	if(m_pMeshCom == NULL)
		return;
	m_pMeshCom->Set_AniInfo(pMotionInfo);
}

void CDynamicObject::Set_Collider(const _uint& iType)
{
	if(m_pColliderCom != NULL)
		return;

	Engine::CComponent* pComponent = NULL;

	switch(iType)
	{
	case Engine::COL_SPHERE:
		pComponent = Engine::CSphereCollider::Create(m_pGraphicDev, m_pTransCom, L"Player", _vec3(0.f, 0.f, 0.f), 1.f);
		if(pComponent == NULL)
			return;
		m_pColliderCom = (Engine::CSphereCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));
		break;

	case Engine::COL_BOX:
		pComponent = Engine::CBoxCollider::Create(m_pGraphicDev, m_pTransCom, L"Player", _vec3(0.f, 0.f, 0.f), _vec3(1.f, 1.f, 1.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		if(pComponent == NULL)
			return;
		m_pColliderCom = (Engine::CBoxCollider*)pComponent;
		m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));
		break;
	}
	m_pColliderCom->Set_Render(true);
}

void CDynamicObject::Delete_Collider(void)
{
	MAPCOMPONENT::iterator iter = find_if(m_mapComponent[Engine::COMID_DYNAMIC].begin(), m_mapComponent[Engine::COMID_DYNAMIC].end(), Engine::CTag_Finder(L"Collider"));

	if(iter == m_mapComponent[Engine::COMID_DYNAMIC].end())
		return;

	Engine::Safe_Release(iter->second);
	m_pColliderCom = NULL;
	iter = m_mapComponent[Engine::COMID_DYNAMIC].erase(iter);
}

void CDynamicObject::Set_ColliderPos(const _vec3& vPos)
{
	if(m_pColliderCom == NULL)
		return;
	m_pColliderCom->SetPos(vPos);
}

void CDynamicObject::Set_ColliderSize(const _vec3& vSize)
{
	if(m_pColliderCom == NULL)
		return;
	m_pColliderCom->SetScale(vSize);
}

void CDynamicObject::Set_ColliderSize(const _float& fRad)
{
	if(m_pColliderCom == NULL)
		return;
	m_pColliderCom->SetScale(fRad);
}

void CDynamicObject::Update_Object(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;
	Engine::CGameObject::Update_Object(fTimeDelta);

	if(m_bIsPlaying)
		m_pMeshCom->Play_AnimationSet(fTimeDelta);
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
}

void CDynamicObject::Render_Object(void)
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

	if(m_pColliderCom)
		m_pColliderCom->Render_Collider();
}

HRESULT CDynamicObject::Ready_Object(const wstring& wstrName)
{
	if(FAILED(Add_Component(wstrName)))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(m_iAniIdx);
	m_pTransCom->Rotation(Engine::ROT_Y, 180.f);

	return S_OK;
}

HRESULT CDynamicObject::Add_Component(const wstring& wstrName)
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
	m_pMeshCom = (Engine::CDynamicMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));

	//Renderer
	pComponent = Engine::Get_Renderer();
	if(pComponent == NULL)
		return E_FAIL;
	m_pRendererCom = (Engine::CRenderer*)pComponent;
	m_pRendererCom->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Renderer", pComponent));

 	//Shader
 	pComponent = Engine::Clone_Prototype(L"Shader_Player");
 	if(pComponent == NULL)
 		return E_FAIL;
 	m_pShaderCom = (Engine::CShader*)pComponent;
 	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

HRESULT CDynamicObject::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	Engine::Safe_Release(pEffect);

	return S_OK;
}


CDynamicObject* CDynamicObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CDynamicObject*		pInstance = new CDynamicObject(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object(wstrName)))
	{
		MSG_BOX("CDynamicObject Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamicObject::Free(void)
{
	Engine::CGameObject::Free();
}
