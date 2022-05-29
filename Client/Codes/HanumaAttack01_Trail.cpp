#include "stdafx.h"
#include "HanumaAttack01_Trail.h"
#include "Export_Engine.h"
#include "Trail_Texture.h"
#include "Texture.h"
#include "Hanuman.h"
#include "PlayerCamera.h"

CHanumaAttack01_Trail::CHanumaAttack01_Trail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pTrailVtx(NULL)
	, m_pBufferCom(NULL)
	, m_iCntX(0)
	, m_iCntZ(0)
	, m_iInterval(0)
	, m_pTarget_Start_Mat(NULL)
	, m_pTarget_End_Mat(NULL)
	, m_iTexture(0)
{
	//m_bIsStatic = true;
	m_bAni = false;

	m_iBlurCnt = 4;
	m_fBlurPower = 1.5f;
	m_fBlurValue = 128.0f;
	m_eBlurType = Engine::BLUR_CROSS;
}

CHanumaAttack01_Trail::~CHanumaAttack01_Trail(void)
{
}

void CHanumaAttack01_Trail::Update_Object(const _float & fTimeDelta)
{
	//if (!m_bAni)
	//	return;

	CEffect::Update_Object(fTimeDelta);

	vStart = _vec3(0.0f, 0.0f, 0.0f);
	vEnd = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vStart, &vStart, m_pTarget_Start_Mat);
	D3DXVec3TransformCoord(&vEnd, &vEnd, m_pTarget_End_Mat);

	D3DXVec3TransformCoord(&vStart, &vStart, m_pTargetWorldMat);
	D3DXVec3TransformCoord(&vEnd, &vEnd, m_pTargetWorldMat);

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
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);
}

void CHanumaAttack01_Trail::Render_Object(void)
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

void CHanumaAttack01_Trail::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->BeginPass(3);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	Engine::Safe_Release(pEffect);
}

HRESULT CHanumaAttack01_Trail::Ready_Obejct(void)
{
	m_iCntX = 25;
	m_iCntZ = 2;
	m_iInterval = 1;

	if (L"Hanuman_Att01_0_Left_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Att01_0_Left_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Att01_0_Left_Trail Load Failed");
			return E_FAIL;
		}
	}
	else if (L"Hanuman_Att01_1_Left_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Att01_1_Left_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Att01_1_Left_Trail Load Failed");
			return E_FAIL;
		}
	}
	else if (L"Hanuman_Att01_2_Left_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Att01_2_Left_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Att01_2_Left_Trail Load Failed");
			return E_FAIL;
		}
	}
	else if (L"Hanuman_Att01_0_Right_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Att01_0_Right_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Att01_0_Right_Trail Load Failed");
			return E_FAIL;
		}
	}
	else if (L"Hanuman_Att01_1_Right_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Att01_1_Right_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Att01_1_Right_Trail Load Failed");
			return E_FAIL;
		}
	}
	else if (L"Hanuman_Att01_2_Right_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Att01_2_Right_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Att01_2_Right_Trail Load Failed");
			return E_FAIL;
		}
	}
	else if (L"Hanuman_Combo_0_Left_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Combo_0_Left_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Combo_0_Left_Trail Load Failed");
			return E_FAIL;
		}
	}
	else if (L"Hanuman_Combo_1_Left_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Combo_1_Left_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Combo_1_Left_Trail Load Failed");
			return E_FAIL;
		}
	}
	else if (L"Hanuman_Combo_2_Left_Trail" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"Hanuman_Combo_2_Left_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Hanuman_Combo_2_Left_Trail Load Failed");
			return E_FAIL;
		}
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

	m_pBufferCom->GetVtxInfo(m_pTrailVtx);

	m_iShaderPath = 3;
	m_iTexture = 10;
	m_vColor = _vec4(0.0f, 0.9333f, 0.9333f, 1.f);

	return S_OK;
}

CHanumaAttack01_Trail * CHanumaAttack01_Trail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHanumaAttack01_Trail*		pInstance = new CHanumaAttack01_Trail(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Obejct()))
	{
		MSG_BOX("TrickyRabbitTrail Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHanumaAttack01_Trail::Free(void)
{
	Engine::Safe_Delete_Array(m_pTrailVtx);
	CEffect::Free();
}

HRESULT CHanumaAttack01_Trail::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	if (L"Hanuman_Att01_0_Left_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Att01_0_Left_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if (L"Hanuman_Att01_1_Left_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Att01_1_Left_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if (L"Hanuman_Att01_2_Left_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Att01_2_Left_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if (L"Hanuman_Att01_0_Right_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Att01_0_Right_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if (L"Hanuman_Att01_1_Right_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Att01_1_Right_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if (L"Hanuman_Att01_2_Right_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Att01_2_Right_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	//Combo
	else if (L"Hanuman_Combo_0_Left_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Combo_0_Left_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if (L"Hanuman_Combo_1_Left_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Combo_1_Left_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else if (L"Hanuman_Combo_2_Left_Trail" == m_wstrName)
	{
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Combo_2_Left_Trail");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}



	/*
	m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Hanuman_Att01_0_Trail");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if (NULL == pComponent)
	return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));*/

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

HRESULT CHanumaAttack01_Trail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CHanumaAttack01_Trail::SettingTrail(void)
{
	vStart = _vec3(0.0f, 0.0f, 0.0f);
	vEnd = _vec3(0.0f, 0.0f, 0.0f);

	D3DXVec3TransformCoord(&vStart, &vStart, m_pTarget_Start_Mat);
	D3DXVec3TransformCoord(&vEnd, &vEnd, m_pTarget_End_Mat);

	D3DXVec3TransformCoord(&vStart, &vStart, m_pTargetWorldMat);
	D3DXVec3TransformCoord(&vEnd, &vEnd, m_pTargetWorldMat);

	for (int i = 0; i < m_iCntX; ++i)
	{
		m_pTrailVtx[0 + i].vPosition = vStart;
		m_pTrailVtx[m_iCntX + i].vPosition = vEnd;
	}

	m_vColor = _vec4(0.0f, 0.9333f, 0.9333f, 1.f);
}

void CHanumaAttack01_Trail::SetTargetMatrix(const _matrix* pTarget_Start_Mat, const _matrix* pTarget_End_Mat)
{
	m_pTarget_Start_Mat = pTarget_Start_Mat;
	m_pTarget_End_Mat = pTarget_End_Mat;
}

void CHanumaAttack01_Trail::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CHanumaAttack01_Trail::GetAni(void)
{
	return m_bAni;
}
