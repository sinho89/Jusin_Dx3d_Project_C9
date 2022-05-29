#include "stdafx.h"
#include "Thrown_Arrow.h"
#include "Map_Ice.h"

#include "Export_Engine.h"
//클라 환경 오브젝트

CThrown_Arrow::CThrown_Arrow(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: Engine::CGameObject(pGraphicDev, wstrName)
	, m_pTransCom(NULL)
	, m_pMeshCom(NULL)
	, m_pRendererCom(NULL)
	, m_fAlpha(1.0f)
	, m_fPower(0.f)
	, m_fDestroyCount(0.f)
	, m_bJump(false)
	, m_bStop(false)
	, m_bAttackCol(false)
{

}
CThrown_Arrow::~CThrown_Arrow(void)
{

}

void CThrown_Arrow::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (!m_bAttackCol)
	{
		m_bAttackCol = true;
		m_pSphereColCom->SetScale(5.f);
		m_pSphereColCom->SetPos(_vec3(0.f, 0.f, 0.f));
		Engine::COLINFO tColInfo;
		tColInfo.eColInfo = Engine::COL_PUSH;
		tColInfo.eColDir = Engine::DIR_LOOK;
		tColInfo.fJumpForce = 1.2f;
		tColInfo.fStampForce = 0.f;
		tColInfo.fForce = 3.f;
		tColInfo.vDir = m_vLook;
		tColInfo.iDamage = 50;
		m_pSphereColCom->Set_ColInfo(&tColInfo);
		Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pSphereColCom);
	}

	if (!m_bStop)
	{
		Shot(fTimeDelta);

		LookAtDir(fTimeDelta);
	}

	if (m_pCalculatorCom->Get_IsCol())
	{
		m_bStop = true;
		Engine::Delete_Collider(L"Monster", m_pSphereColCom);
	}

	if (m_pCalculatorCom->Wall_Collision_Check(&m_vDir))
	{
		m_bStop = true;
		Engine::Delete_Collider(L"Monster", m_pSphereColCom);
	}

	if (m_bStop)
	{
		m_fDestroyCount += fTimeDelta;
		if (m_fDestroyCount > 5.f)
		{
			m_eState = Engine::STATE_DESTROY;
		}
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);

	_vec3	vPos;
	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos));

}

void CThrown_Arrow::Render_Object(void)
{
	/*LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect) return;

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
	}

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);

	m_pMeshCom->Render_Mesh(pEffect, iPassIdx);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);*/


	if (m_pSphereColCom)
		m_pSphereColCom->Render_Collider(g_bDebugRender);
}

void CThrown_Arrow::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	switch (iPassIdx)
	{
	case 0:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 2:
		return;
		break;

	case 3:
		return;
		break;

	case 4:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;
	}

	m_pMeshCom->Render_Mesh(pEffect, iPassIdx);
}

void CThrown_Arrow::Shot(const _float& fTimeDelta)
{


	m_vMovingA = m_vMovingA + m_vADir * fTimeDelta * 25.f;
	m_vMovingB = m_vMovingB + m_vBDir * fTimeDelta * 25.f;

	m_vDir = m_vMovingB - m_vMovingA;
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_vPos = m_vPos + m_vDir * fTimeDelta * 30.f;

	m_pTransCom->Set_Position(&m_vPos);

	m_vLook = m_vLastPos - m_vCurvePos[0];
	D3DXVec3Normalize(&m_vLook, &m_vLook);


	//m_vPos = m_vNextPos;

	//m_pTransCom->Set_Position(&m_vNextPos);
	//m_vCurvePos[1] = D3DXVECTOR3((m_vCurvePos[0].x + m_vLastPos.x) * 0.5f, 
	//	(m_vCurvePos[0].y + m_vLastPos.y) * 0.5f + 2.f, 
	//	(m_vCurvePos[0].z + m_vLastPos.z) * 0.5f);

	//m_vCurvePos[2] = m_vLastPos;

	//m_vNextPos.x = pow((1 - fTimeDelta * 30.f), 2) * m_vCurvePos[0].x +
	//	2 * fTimeDelta * 30.f* (1 - fTimeDelta * 30.f) * m_vCurvePos[1].x +
	//	pow(fTimeDelta * 30.f, 2) * m_vCurvePos[2].x;

	//m_vNextPos.y = pow((1 - fTimeDelta * 30.f), 2) * m_vCurvePos[0].y +
	//	2 * fTimeDelta * 30.f * (1 - fTimeDelta * 30.f) * m_vCurvePos[1].y +
	//	pow(fTimeDelta * 30.f, 2) * m_vCurvePos[2].y;

	//m_vNextPos.z = pow((1 - fTimeDelta * 30.f), 2) * m_vCurvePos[0].z +
	//	2 * fTimeDelta * 30.f * (1 - fTimeDelta * 30.f) * m_vCurvePos[1].z +
	//	pow(fTimeDelta * 30.f, 2) * m_vCurvePos[2].z;

	//////m_vLook = m_pTransCom->Get_Now_Look();
	//D3DXVec3Subtract(&m_vDir, &m_vNextPos, &m_vPos);
	//D3DXVec3Normalize(&m_vDir, &m_vDir);

	//float fTheta = atan2f(m_vDir.y, m_vDir.x);
	//_float fAngle = fTheta + D3DX_PI;

	//_vec3 vRight = m_pTransCom->Get_Now_Right();

	//D3DXMATRIX	matScale;
	//D3DXMATRIX	matRot;
	//D3DXMATRIX	matTrans;
	//D3DXMATRIX	matWorld;
	//D3DXMatrixIdentity(&matScale);
	//D3DXMatrixIdentity(&matRot);
	//D3DXMatrixIdentity(&matTrans);
	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	//D3DXMatrixRotationAxis(&matRot, &vRight, fAngle);
	//m_pTransCom->Get_WorldMatrix(&matWorld);
	//matWorld = matWorld * matRot;
	//m_pTransCom->Set_WorldMatrix(&matWorld);

	/////////////////////////////////////////
	//_vec3 vPos = m_pTransCom->Get_Now_Position();
	//m_vLook = m_pTransCom->Get_Now_Look();
	//_vec3 vEndPos = vPos + m_vLook * fTimeDelta * m_fPower;
	//m_pTransCom->Set_Position(&vEndPos);
	/////////////////////////////////////////


}

void CThrown_Arrow::LookAtDir(const _float & fTimeDelta)
{
	_vec3 vLook;
	_vec3 vRight;
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	m_pTransCom->Get_Infomation(Engine::INFO_LOOK, &vLook);
	D3DXVec3Cross(&vRight, &vUp, &vLook);

	_vec3 vDir = m_vMovingB - m_vPos;
	D3DXVec3Normalize(&vDir, &vDir);

	_float fCos = D3DXVec3Dot(&vLook, &vDir);
	_float fRealCos = D3DXVec3Dot(&vLook, &vDir);
	_float fAngle = acosf(fRealCos);
	_float fDegree = D3DXToDegree(fAngle);
	_float fUp = 1.f;
	_float fRotationSpeed = 180.f;

	//m_pTransCom->Rotation(Engine::ROT_X, 90.f);
	//m_pTransCom->Set_Angle(&vDir);

	if (fCos < 0)
	{
		fUp = -1.f;
	}

	if (fDegree > fRotationSpeed * fTimeDelta)
	{
		m_pTransCom->Rotation(Engine::ROT_X, fUp * fAngle);
	
	}

	else
	{
		m_pTransCom->Rotation(Engine::ROT_X, fUp * fRotationSpeed * fTimeDelta);

	}

}


HRESULT CThrown_Arrow::Ready_Object(_vec3* pPos, _vec3* pTargetPos, _vec3* pDir, _float fPower)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(pPos);
	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vRight;
	_vec3 vLook = m_pTransCom->Get_Now_Look();

	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	_float fCos = D3DXVec3Dot(&vRight, pDir);
	_float fRealCos = D3DXVec3Dot(&vLook, pDir);
	_float fAngle = acosf(fRealCos);
	_float fDegree = D3DXToDegree(fAngle);
	_float fRight = 1.f;

	if (fCos < 0)
		fRight = -1.f;

	m_pTransCom->Rotation(Engine::ROT_Y, fRight * fDegree);
	m_fPower = fPower;

	m_vLook = *pTargetPos - *pPos;
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	m_vLook.y = 0.f;
	//중점보정...
	m_vPos = *pPos + m_vLook * 1.f;
	//베지어...
	m_vNextPos = m_vPos;

	//vLook = m_pTransCom->Get_Now_Look();

	m_vLastPos = m_vPos + m_vLook * 75.f;
	// 0 1 2를 구한 상태..
	m_vCurvePos[0] = m_vPos;
	m_vCurvePos[1] = _vec3((m_vPos.x + m_vLastPos.x) * 0.5f,
		(m_vPos.y + m_vLastPos.y) * 0.5f + 10.f,
		(m_vPos.z + m_vLastPos.z) * 0.5f);
	m_vCurvePos[2] = m_vLastPos;

	m_vMovingA = m_vPos;
	m_vMovingB = m_vCurvePos[1];

	m_vADir = m_vCurvePos[1] - m_vMovingA;
	D3DXVec3Normalize(&m_vADir, &m_vADir);
	m_vBDir = m_vCurvePos[2] - m_vMovingB;
	D3DXVec3Normalize(&m_vBDir, &m_vBDir);

	m_pTransCom->Rotation(Engine::ROT_X, -15.f);

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
	if (pGameObject == NULL)
		return E_FAIL;

	if (dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);
		m_ppMapMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(ppMesh[i]->Get_Mesh());
		}
	}

	Engine::Safe_Release(pGameObject);


	return S_OK;
}

HRESULT CThrown_Arrow::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));



	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Thrown_Arrow");
	m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticMesh"));
	if (NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	//Calculator
	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	if (pComponent == NULL)
		return E_FAIL;
	m_pCalculatorCom = (Engine::CCalculator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"2", pComponent));
	m_pCalculatorCom->Set_Transform(m_pTransCom);

	//Rigidbody
	pComponent = Engine::CRigidbody::Create(m_pTransCom, m_pCalculatorCom);
	if (pComponent == NULL)
		return E_FAIL;
	m_pRigidbodyCom = (Engine::CRigidbody*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"1", pComponent));
	m_pRigidbodyCom->Set_Gravity(false);

	//SphereCollider
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Monster", _vec3(0.f, 0.f, 0.f), 10.f);
	if (pComponent == NULL)
		return E_FAIL;
	m_pSphereColCom = (Engine::CSphereCollider*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"SphereCol", pComponent));

	return S_OK;
}

HRESULT CThrown_Arrow::Set_ConstantTable(LPD3DXEFFECT pEffect)
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
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	Engine::Safe_Release(pEffect);

	return S_OK;
}


CThrown_Arrow* CThrown_Arrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName
	, _vec3* pPos, _vec3* pTargetPos, _vec3* pDir, _float fPower)
{
	CThrown_Arrow*		pInstance = new CThrown_Arrow(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object(pPos, pTargetPos, pDir, fPower)))
	{
		MSG_BOX("CThrown_Arrow Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CThrown_Arrow::Free(void)
{
	Engine::CGameObject::Free();

}


