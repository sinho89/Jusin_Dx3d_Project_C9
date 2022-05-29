#include "stdafx.h"
#include "BlackRoseTrail.h"
#include "Export_Engine.h"
#include "Trail_Texture.h"
#include "Texture.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"


CBlackRoseTrail::CBlackRoseTrail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, TRAILPART eDir)
:CEffect(pGraphicDev, wstrName)
, m_pTrailVtx(NULL)
, m_pBufferCom(NULL)
, m_iCntX(0)
, m_iCntZ(0)
, m_iInterval(0)
, m_pmatTarget(NULL)
, m_iTexture(0)
, m_pmatBoneUp(NULL)
, m_pmatBoneDown(NULL)
{
	m_eDir = eDir;
	m_bIsStatic = true;
	m_bAni = false;
}

CBlackRoseTrail::~CBlackRoseTrail(void)
{
}

void CBlackRoseTrail::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	CEffect::Update_Object(fTimeDelta);

	if (m_eDir == DIR_LeftReg)//왼발
	{
		memcpy(&vStart, &m_pmatBoneUp->m[3][0], sizeof(D3DXVECTOR3));
		memcpy(&vEnd,   &m_pmatBoneDown->m[3][0], sizeof(D3DXVECTOR3));
	}
	else if(m_eDir == DIR_RightReg)//오른발
	{
		memcpy(&vStart, &m_pmatBoneUp->m[3][0], sizeof(D3DXVECTOR3));
		memcpy(&vEnd,	&m_pmatBoneDown->m[3][0], sizeof(D3DXVECTOR3));
	}
	else if (m_eDir == DIR_RightSword) //오른손 검
	{
		vStart = _vec3(0.0f, 0.0f, 0.0f);
		vEnd = _vec3(0.0f, -2.5f, -2.2f);
	}

	D3DXVec3TransformCoord(&vStart, &vStart, m_pmatTarget);
	D3DXVec3TransformCoord(&vEnd, &vEnd, m_pmatTarget);

	for (int i = 0; i < m_iCntX; ++i)
	{
		_vec3 Temp, Temp2;

		if (i == 0)
		{
			Temp = m_pTrailVtx[0].vPosition;
			Temp2 = m_pTrailVtx[m_iCntX].vPosition;


			m_pTrailVtx[0].vPosition = vStart;
			m_pTrailVtx[m_iCntX].vPosition = vEnd;
		}
		else
		{
			_vec3 vTemp, vTemp2;

			vTemp = m_pTrailVtx[i].vPosition;

			vTemp2 = m_pTrailVtx[m_iCntX + i].vPosition;


			m_pTrailVtx[i].vPosition = Temp;

			m_pTrailVtx[m_iCntX + i].vPosition = Temp2;

			Temp = vTemp;
			Temp2 = vTemp2;

		}
	}

	m_pBufferCom->SetVtxInfo(m_pTrailVtx);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT, this);

	if (m_bEffectBlurCheck)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);

}

void CBlackRoseTrail::Render_Object(void)
{
	if (!m_bAni)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == NULL)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(m_iShaderPath);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CBlackRoseTrail::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->BeginPass(m_iBlurPass);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	Engine::Safe_Release(pEffect);
}

HRESULT CBlackRoseTrail::Ready_Obejct(void)
{
	m_iCntX = 20;
	m_iCntZ = 2;
	m_iInterval = 1;

	switch (m_eDir)
	{
	case DIR_LeftReg:
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BlackRoseTrailLeft", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("BlackRoseTrailLeft Load Failed");
			return E_FAIL;
		}

		if (FAILED(Add_Component()))
			return E_FAIL;

		m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

		m_pBufferCom->GetVtxInfo(m_pTrailVtx);

		m_iShaderPath = 0;
		m_iTexture = 5;
		m_vColor = _vec4(1.f, 0.f, 0.f, 2.f);
		break;

	case DIR_RightReg:
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BlackRoseTrailRight", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("BlackRoseTrailRight Load Failed");
			return E_FAIL;
		}

		if (FAILED(Add_Component()))
			return E_FAIL;

		m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

		m_pBufferCom->GetVtxInfo(m_pTrailVtx);

		m_iShaderPath = 0;
		m_iTexture = 5;
		m_vColor = _vec4(1.f, 0.f, 0.f, 1.f);
		break;

	case DIR_RightSword:
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"BlackRoseTrailRightSword", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("BlackRoseTrailRightSword Load Failed");
			return E_FAIL;
		}

		if (FAILED(Add_Component()))
			return E_FAIL;

		m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

		m_pBufferCom->GetVtxInfo(m_pTrailVtx);

		m_iShaderPath = 0;
		m_iTexture = 14;
		m_vColor = _vec4(1.f, 0.f, 0.f, 1.f);
		break;
	}

	return S_OK;
}

CBlackRoseTrail * CBlackRoseTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, TRAILPART eDir)
{
	CBlackRoseTrail*		pInstance = new CBlackRoseTrail(pGraphicDev, wstrName, eDir);

	if (FAILED(pInstance->Ready_Obejct()))
	{
		MSG_BOX("BlackRoseTrail Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackRoseTrail::Free(void)
{
	CEffect::Free();
	Engine::Safe_Delete_Array(m_pTrailVtx);
}

HRESULT CBlackRoseTrail::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	if (m_eDir == DIR_LeftReg)
	{
		// For.Buffer Component
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"BlackRoseTrailLeft");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if(m_eDir == DIR_RightReg)
	{
		// For.Buffer Component
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"BlackRoseTrailRight");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if(m_eDir == DIR_RightSword)
	{
		// For.Buffer Component
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"BlackRoseTrailRightSword");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}


	//For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent) return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));


	// For.Shader Component
	pComponent = Engine::Clone_Prototype(L"Shader_Trail");
	if (pComponent == NULL)
		return E_FAIL;
	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Trail");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

HRESULT CBlackRoseTrail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix				matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);


	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	pEffect->SetVector("g_vColor", &m_vColor);


	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_iTexture);

	float m_fFar = static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->GetCamFar();

	pEffect->SetFloat("g_fFar", m_fFar);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

void CBlackRoseTrail::SettingTrail(void)
{

	if (m_eDir == DIR_LeftReg)//왼발
	{
		memcpy(&vStart, &m_pmatBoneUp->m[3][0], sizeof(D3DXVECTOR3));
		memcpy(&vEnd, &m_pmatBoneDown->m[3][0], sizeof(D3DXVECTOR3));

	}
	else if (m_eDir == DIR_RightReg)//오른발
	{
		memcpy(&vStart, &m_pmatBoneUp->m[3][0], sizeof(D3DXVECTOR3));
		memcpy(&vEnd, &m_pmatBoneDown->m[3][0], sizeof(D3DXVECTOR3));

	}
	else if (m_eDir == DIR_RightSword) //오른손 검
	{
		vStart = _vec3(0.0f, 0.0f, 0.0f);
		vEnd = _vec3(0.0f, -4.5f, -3.0f);
	}

	D3DXVec3TransformCoord(&vStart, &vStart, m_pmatTarget);
	D3DXVec3TransformCoord(&vEnd, &vEnd, m_pmatTarget);

	for (int i = 0; i < m_iCntX; ++i)
	{
		m_pTrailVtx[0 + i].vPosition = vStart;
		m_pTrailVtx[m_iCntX + i].vPosition = vEnd;
	}
}

void CBlackRoseTrail::SetTargetMatrix(const _matrix * pTargetMatrix)
{
	m_pmatTarget = pTargetMatrix;
}

void CBlackRoseTrail::SetTargetBoneUpMatrix(const _matrix * pTargetMatrix)
{
	m_pmatBoneUp = pTargetMatrix;
}

void CBlackRoseTrail::SetTargetBoneDownMatrix(const _matrix * pTargetMatrix)
{
	m_pmatBoneDown = pTargetMatrix;
}



void CBlackRoseTrail::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CBlackRoseTrail::GetAni(void)
{
	return m_bAni;
}

void CBlackRoseTrail::SetTrailDir(TRAILPART Dir)
{
	m_eDir = Dir;
}