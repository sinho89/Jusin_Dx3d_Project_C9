#include "stdafx.h"
#include "Icicle.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"
#include "LightCamera.h"
#include "WitchBlade.h"
#include "Sprite.h"
#include "Wave.h"

CIcicle::CIcicle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pMeshCom(NULL)
,m_pTargetTransform(NULL)
, m_bDown(true)
, m_dTimer(0)
, m_fDownTime(0.0f)
, m_pColliderCom(NULL)
, m_pWave(NULL)
{
	m_vDownDir = _vec3(0.f, -1.f, 0.f);
	ZeroMemory(&m_pDust, sizeof(CSprite*) * 2);
}

CIcicle::~CIcicle(void)
{
}

void CIcicle::Update_Object(const _float & fTimeDelta)
{

	//Player 충돌
	if (m_pColliderCom->IsHit())
	{
		m_eState = Engine::STATE_DESTROY;
	}


	if (!m_bDown)
	{
		m_dTimer += fTimeDelta;

		if (m_dTimer >= 3.f) //시간이 지나서 사라지는것
		{
			_vec3 vScale;

			m_pTransCom->Get_Scale(&vScale);

			if (vScale.z <= 0.f)
			{
				m_eState = Engine::STATE_DESTROY;
			}
			else
			{
				vScale.z -= 0.05f;

				m_pTransCom->Set_Scale(vScale.x, vScale.y, vScale.z);
			}
		}
	}


	Falling(); //수직 낙하운동

	WaveUpdate();


	CEffect::Update_Object(fTimeDelta);


	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Update_Object(fTimeDelta);
	}

	m_pWave->Update_Object(fTimeDelta);


	if (m_pRendererCom->GetShadowBuffer() ||
		m_pRendererCom->GetSoftShadow() ||
		m_pRendererCom->GetCACDShadow())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC_SHADOW, this);
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);
}

void CIcicle::Render_Object(void)
{

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Render_Object();
	}
	m_pWave->Render_Object();


	if (m_pColliderCom)
		m_pColliderCom->Render_Collider(g_bDebugRender);
}

void CIcicle::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
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
		if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
		break;

	case 3:
		return;
		break;

	case 4:
		return;
		break;

	case 5:
		if (FAILED(Set_CACD_Shadow_ConstanTable(pEffect))) return;
		break;
	}

	m_pMeshCom->Render_Mesh(pEffect, iPassIdx);

}

void CIcicle::Free(void)
{
	//충돌체 삭제=======================================
	Engine::Delete_Collider(L"Monster", m_pColliderCom);

	CEffect::Free();

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pDust[i]);
	}
	Engine::Safe_Release(m_pWave);
}

HRESULT CIcicle::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Scale(4.f, 4.f, 4.f);


	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanDust");
		m_pDust[i]->SetAni(false);
		m_pDust[i]->GetTransCom()->Set_Scale(0.7f + (i * 0.2f), 0.7f + (i * 0.2f), 0.7f + (i * 0.2f));
		_vec4 Color = _vec4(0.3f, 0.3f, 0.3f, 1.f);
		m_pDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pDust[i]->SetRenderCheck(true);
		m_pDust[i]->GetParticleInfo()->m_tSpriteInfo.m_fSpriteSpeed = 35.f;

		m_pDust[i]->SetBlurCheck(true);
		m_pDust[i]->SetBlurCnt(2);
		m_pDust[i]->SetBlurPass(3);
		m_pDust[i]->SetBlurPower(1.2f);
		m_pDust[i]->SetBlurType(Engine::BLUR_CROSS);
		m_pDust[i]->SetBlurValue(100.0f);
	}

	//Wave
	//==========================================
	m_pWave = CWave::Create(m_pGraphicDev, L"Wave");
	m_pWave->SetAni(false);
	m_pWave->SetPattern(CWave::PATTERN_END);
	_vec4 vColor = _vec4(1.1f, 1.0f, 1.0f, 1.0f);
	m_pWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	m_pWave->GetParticleInfo()->m_iPath = 2;
	m_pWave->GetParticleInfo()->m_iTextureNumber = 7;




	//==========================================
	m_pColliderCom->SetScale(2.f);
	m_pColliderCom->SetPos(_vec3(0.f, 0.f, -2.f));
	Engine::COLINFO tColInfo;
	tColInfo.eColInfo = Engine::COL_PUSH;
	tColInfo.fJumpForce = 0.f;
	tColInfo.fStampForce = 0.f;
	tColInfo.fForce = 10.f;
	tColInfo.vDir = m_vDownDir;
	tColInfo.iDamage = 40;
	m_pColliderCom->Set_ColInfo(&tColInfo);
	Engine::Add_ColliderGroup(Engine::COL_SPHERE, L"Monster", m_pColliderCom);


	return S_OK;
}

HRESULT CIcicle::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));



	//Mesh
	pComponent = Engine::Clone_Resource(RESOURCE_MESH, L"Mesh_Icicle1");
	if (pComponent == NULL)
		return E_FAIL;
	m_pMeshCom = (Engine::CStaticMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh", pComponent));


	//For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent) return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));


	// For.Shader Component
	pComponent = Engine::Clone_Prototype(L"Shader_StaticMesh");
	if (pComponent == NULL)
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	//For.Collider Component
	pComponent = Engine::Create_SphereCollider(m_pGraphicDev, m_pTransCom, L"Monster", _vec3(0.f, 0.f, 0.f), 1.f);
	if (pComponent == NULL)
		return E_FAIL;

	m_pColliderCom = (Engine::CSphereCollider*)pComponent;
	m_pColliderCom->Set_Render(true);
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Collider", pComponent));

	return S_OK;
}

HRESULT CIcicle::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	pEffect->SetFloat("g_fPower", 20.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bBlurCheck", false);
	pEffect->SetBool("g_bSpecularCheck", true);

	pEffect->SetBool("g_bOutline", m_bOutlineCheck);
	pEffect->SetFloat("g_fColorCheck", 0.15f);
	pEffect->SetFloat("g_fOutlineThickness", 0.00045f);

	Engine::Safe_Release(pEffect);
	return S_OK;
}

HRESULT CIcicle::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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

HRESULT CIcicle::Set_CACD_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
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

CIcicle * CIcicle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CIcicle * pInstance = new CIcicle(pGraphicDev, wstrName);
	
	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Icle Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CIcicle::Falling(void)
{
	if (!m_bDown)
		return;


	_vec3 Dir = m_vLimitPos - m_pTransCom->Get_Now_Position();

	_float fLength = D3DXVec3Length(&Dir);

	if (fLength <= 2.f)
	{
		m_bDown = false;

		//Dust생성

		for (int i = 0; i < 2; ++i)
		{
			m_pDust[i]->SetAni(true);
			m_pDust[i]->GetTransCom()->Set_Position(&m_vDustStartPoint);

		}

		//Wave=======================================================
		_vec3 vWavePos = m_vDustStartPoint;
		_vec3 vAngle = _vec3(90.f, 0.f, 0.f);
		vWavePos.y += 0.2f;
		m_pWave->SetAni(true);
		m_pWave->GetTransCom()->Set_Position(&vWavePos);
		m_pWave->GetTransCom()->Set_Scale(0.2f, 0.2f, 0.2f);
		m_pWave->GetTransCom()->Set_Angle(&vAngle);
		_vec4 vColor = _vec4(0.4f, 0.7f, 0.8f, 1.0f);
		m_pWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		//===========================================================

	}
	else
	{
		_vec3 vPos = m_pTransCom->Get_Now_Position();

		m_fDownTime += 0.3f;

		if (m_fDownTime >= 2.7f)
		{
			m_fDownTime = 2.7f;
		}

		vPos.y += (-0.98f * m_fDownTime * m_fDownTime) / 2;

		m_pTransCom->Set_Position(&vPos);
	}
}

void CIcicle::SetLimitPos(_vec3 vLimitPos)
{
	m_vLimitPos = vLimitPos;
}

void CIcicle::SetDustStartPoint(_vec3 vStartPoint)
{
	m_vDustStartPoint = vStartPoint;
}

void CIcicle::WaveUpdate(void)
{
	if (!m_pWave->GetAni())
		return;

	_vec4 vColor = (_vec4)m_pWave->GetParticleInfo()->m_dColor;

	if (vColor.w <= 0.f)
		m_pWave->SetAni(false);
	else
	{
		m_pWave->GetTransCom()->Add_Scale(0.3f);
		vColor.w -= 0.1f;
		m_pWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
	}

}
