#include "stdafx.h"
#include "Export_Engine.h"
#include "Trail_Texture.h"
#include "Texture.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"
#include "SprialDance_Out_Trail.h"

CSprialDance_Out_Trail::CSprialDance_Out_Trail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	:CEffect(pGraphicDev, wstrName)
	, m_pTrailVtx(NULL)
	, m_pBufferCom(NULL)
	, m_iCntX(0)
	, m_iCntZ(0)
	, m_iInterval(0)
	, m_pmat_Sword(NULL)
	, m_iTexture(0)
{
	m_bIsStatic = true;
	m_bAni = false;

	m_iBlurCnt = 2;
	m_fBlurPower = 1.01f;
	m_fBlurValue = 500.0f;
	m_eBlurType = Engine::BLUR_VERTICAL;
}

CSprialDance_Out_Trail::~CSprialDance_Out_Trail(void)
{
}

void CSprialDance_Out_Trail::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	CEffect::Update_Object(fTimeDelta);

	//왼손
	if (L"Out_LeftSword" == m_wstrName)
	{
		vStart = _vec3(0.2f * 1.5f, -3.0f * 1.5f, -2.0f * 1.5f);
		vEnd = _vec3(0.2f * 2.5f, -3.0f * 2.5f, -2.0f * 2.5f);

		D3DXVec3TransformCoord(&vStart, &vStart, m_pmat_Sword);
		D3DXVec3TransformCoord(&vEnd, &vEnd, m_pmat_Sword);
	}
	else if (L"Out_RightSword" == m_wstrName)
	{
		//오른손
		vStart = _vec3(0.0f, -2.5f * 1.5f, -2.2f * 1.5f);
		vEnd = _vec3(0.0f, -2.5f * 2.5f, -2.2f * 2.5f);

		D3DXVec3TransformCoord(&vStart, &vStart, m_pmat_Sword);
		D3DXVec3TransformCoord(&vEnd, &vEnd, m_pmat_Sword);
	}

	for (int i = 0; i < m_iCntX; ++i)
	{
		_vec3 Temp, Temp2;

		if (i == 0)
		{
			//Left
			Temp = m_pTrailVtx[0].vPosition;
			Temp2 = m_pTrailVtx[m_iCntX].vPosition;

			m_pTrailVtx[0].vPosition = vStart;
			m_pTrailVtx[m_iCntX].vPosition = vEnd;
		}
		else
		{
			_vec3 vTemp, vTemp2;

			//Left
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
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);
}

void CSprialDance_Out_Trail::Render_Object(void)
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

void CSprialDance_Out_Trail::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->BeginPass(10);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	Engine::Safe_Release(pEffect);
}

HRESULT CSprialDance_Out_Trail::Ready_Obejct(void)
{
	m_iCntX = 15;
	m_iCntZ = 2;
	m_iInterval = 1;

	if (L"Out_LeftSword" == m_wstrName)
	{
		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"SprialDance_Out_Trail", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("Buffer_SprialDance_Out_Trail Failed");
			return E_FAIL;
		}
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

	m_pBufferCom->GetVtxInfo(m_pTrailVtx);

	m_iShaderPath = 1;
	m_iTexture = 24;
	m_vColor = _vec4(-1.f, -0.1f,  1.0f, 1.0f);

	return S_OK;
}

CSprialDance_Out_Trail * CSprialDance_Out_Trail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CSprialDance_Out_Trail*		pInstance = new CSprialDance_Out_Trail(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Obejct()))
	{
		MSG_BOX("CSprialDance_Out_Trail Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSprialDance_Out_Trail::Free(void)
{
	CEffect::Free();
	Engine::Safe_Delete_Array(m_pTrailVtx);
}

HRESULT CSprialDance_Out_Trail::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"SprialDance_Out_Trail");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if (NULL == pComponent)
		return E_FAIL;

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

HRESULT CSprialDance_Out_Trail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CSprialDance_Out_Trail::SettingTrail(void)
{
	if (L"Out_LeftSword" == m_wstrName)
	{
		vStart = _vec3(0.2f, -3.0f, -2.0f);
		vEnd = _vec3(0.2f * 2.0f, -3.0f * 2.0f, -2.0f * 2.0f);

		D3DXVec3TransformCoord(&vStart, &vStart, m_pmat_Sword);
		D3DXVec3TransformCoord(&vEnd, &vEnd, m_pmat_Sword);
	}
	else if (L"Out_RightSword" == m_wstrName)
	{
		//오른손
		vStart = _vec3(0.0f, -2.5f, -2.2f);
		vEnd = _vec3(0.0f, -2.5f * 2.0f, -2.2f * 2.0f);

		D3DXVec3TransformCoord(&vStart, &vStart, m_pmat_Sword);
		D3DXVec3TransformCoord(&vEnd, &vEnd, m_pmat_Sword);
	}

	for (int i = 0; i < m_iCntX; ++i)
	{
		m_pTrailVtx[0 + i].vPosition = vStart;
		m_pTrailVtx[m_iCntX + i].vPosition = vEnd;
	}

	m_vColor = _vec4(-1.f, -0.1f, 1.0f, 1.0f);
}

void CSprialDance_Out_Trail::SetTargetMatrix(const _matrix* pSwordMat)
{
	m_pmat_Sword = pSwordMat;
}

void CSprialDance_Out_Trail::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CSprialDance_Out_Trail::GetAni(void)
{
	return m_bAni;
}
