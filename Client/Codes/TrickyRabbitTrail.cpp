#include "stdafx.h"
#include "TrickyRabbitTrail.h"
#include "Export_Engine.h"
#include "Trail_Texture.h"
#include "Texture.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"


CTrickyRabbitTrail::CTrickyRabbitTrail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, TRAILDIR eDir)
:CEffect(pGraphicDev, wstrName)
, m_pTrailVtx(NULL)
, m_pBufferCom(NULL)
, m_iCntX(0)
, m_iCntZ(0)
, m_iInterval(0)
, m_pmatTarget(NULL)
, m_iTexture(0)
, m_bLightPatternCheck(false)
{
	m_eDir = eDir;
	m_bIsStatic = true;
	m_bAni = false;
}

CTrickyRabbitTrail::~CTrickyRabbitTrail(void)
{
}

void CTrickyRabbitTrail::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	CEffect::Update_Object(fTimeDelta);

	if (m_eDir == DIR_LEFT)//¿Þ¼Õ°Ë
	{
		vStart = _vec3(0.0f, 0.0f, 0.0f);
		vEnd = _vec3(0.2f, -3.0f, -2.0f);
	}
	else //¿À¸¥¼Õ
	{
		if (!m_bLightPatternCheck)
		{
			vStart = _vec3(0.0f, 0.0f, 0.0f);
			vEnd = _vec3(0.0f, -4.5f, -4.2f);
		}
		else
		{
			vStart = _vec3(0.0f, 0.0f, 0.0f);
			vEnd = _vec3(0.0f, -2.5f, -2.2f);
		}
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

	if(m_bEffectBlurCheck)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);
}

void CTrickyRabbitTrail::Render_Object(void)
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

void CTrickyRabbitTrail::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->BeginPass(m_iBlurPass);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	Engine::Safe_Release(pEffect);
}

HRESULT CTrickyRabbitTrail::Ready_Obejct(void)
{
	if (m_eDir == DIR_LEFT) //¿Þ¼Õ
	{
		m_iCntX = 20;
		m_iCntZ = 2;
		m_iInterval = 1;



		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"TrickyRabbitTrailLeft", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("TrickyRabbitTrailLeft Load Failed");
			return E_FAIL;
		}

		if (FAILED(Add_Component()))
			return E_FAIL;

		m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

		m_pBufferCom->GetVtxInfo(m_pTrailVtx);

		m_iShaderPath = 0;
		m_iTexture = 19;
		m_vColor = _vec4(1.f, 0.f, 0.f, 1.f);
	}
	else
	{
		m_iCntX = 20;
		m_iCntZ = 2;
		m_iInterval = 1;



		if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, L"TrickyRabbitTraiRight", Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
		{
			MSG_BOX("TrickyRabbitTraiRight Load Failed");
			return E_FAIL;
		}

		if (FAILED(Add_Component()))
			return E_FAIL;

		m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

		m_pBufferCom->GetVtxInfo(m_pTrailVtx);

		m_iShaderPath = 0;
		m_iTexture = 8;
		m_vColor = _vec4(1.f, 0.f, 0.f, 1.f);

	}

	return S_OK;
}

CTrickyRabbitTrail * CTrickyRabbitTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, TRAILDIR eDir)
{
	CTrickyRabbitTrail*		pInstance = new CTrickyRabbitTrail(pGraphicDev, wstrName, eDir);

	if (FAILED(pInstance->Ready_Obejct()))
	{
		MSG_BOX("TrickyRabbitTrail Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrickyRabbitTrail::Free(void)
{
	CEffect::Free();
	Engine::Safe_Delete_Array(m_pTrailVtx);
}

HRESULT CTrickyRabbitTrail::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	if (m_eDir == DIR_LEFT)
	{
		// For.Buffer Component
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"TrickyRabbitTrailLeft");
		pComponent = (Engine::CComponent*)m_pBufferCom;
		if (NULL == pComponent)
			return E_FAIL;
		m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
	}
	else
	{
		// For.Buffer Component
		m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"TrickyRabbitTraiRight");
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

HRESULT CTrickyRabbitTrail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CTrickyRabbitTrail::SettingTrail(void)
{
	if (m_eDir == DIR_LEFT)//¿Þ¼Õ°Ë
	{
		vStart = _vec3(0.0f, 0.0f, 0.0f);
		vEnd = _vec3(0.2f, -3.0f, -2.0f);
	}
	else //¿À¸¥¼Õ
	{
		if (!m_bLightPatternCheck)
		{
			m_iTexture = 8;
			m_vColor = _vec4(1.f, 0.f, 0.f, 1.f);

			vStart = _vec3(0.0f, 0.0f, 0.0f);
			vEnd = _vec3(0.0f, -4.5f, -4.2f);
		}
		else
		{
			m_iTexture = 19;

			vStart = _vec3(0.0f, 0.0f, 0.0f);
			vEnd = _vec3(0.0f, -2.5f, -2.2f);
		}
	}

	D3DXVec3TransformCoord(&vStart, &vStart, m_pmatTarget);
	D3DXVec3TransformCoord(&vEnd, &vEnd, m_pmatTarget);

	for (int i = 0; i < m_iCntX; ++i)
	{
		m_pTrailVtx[0 + i].vPosition = vStart;
		m_pTrailVtx[m_iCntX + i].vPosition = vEnd;
	}
}

void CTrickyRabbitTrail::SetTargetMatrix(const _matrix * pTargetMatrix)
{
	m_pmatTarget = pTargetMatrix;
}

void CTrickyRabbitTrail::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CTrickyRabbitTrail::GetAni(void)
{
	return m_bAni;
}

void CTrickyRabbitTrail::SetTrailDir(TRAILDIR Dir)
{
	m_eDir = Dir;
}

void CTrickyRabbitTrail::SetLightPatternCheck(bool bCheck)
{
	m_bLightPatternCheck = bCheck;
}
