#include "stdafx.h"
#include "Wind.h"
#include "Export_Engine.h"
#include "Trail_Texture.h"
#include "Texture.h"
#include "PlayerCamera.h"

CWind::CWind(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, _int iCntX)
:CEffect(pGraphicDev, wstrName)
, m_pTrailVtx(NULL)
, m_pBufferCom(NULL)
, m_iCntZ(0)
, m_iInterval(0)
, m_fRotSpeed(0.0f)
, m_fRadius(0.0f)
, pTargetTransCom(NULL)
, m_fTrailDotY(0.0f)
, m_fIntervalAngle(10.0f)
, m_iCntX(iCntX)
, m_bRenderCheck(true)
{
	m_bAni = false;
	m_bEffectBlurCheck = false;

	m_iBlurCnt = 4;	//Blur 몇번 흔들지
	m_fBlurPower = 1.0f; //BlurPower 수치 Up 더 밝아진다.
	m_fBlurValue = 160.0f; //낮추면 잔상처럼 보여진다.(적당히 낮추면 더 밝아진다.)
	m_eBlurType = Engine::BLUR_HORIZON; //가로로 흔들기
}

CWind::~CWind(void)
{
}

void CWind::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	CEffect::Update_Object(fTimeDelta);


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT, this);

	if (m_bEffectBlurCheck)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);
}

void CWind::Render_Object(void)
{
	if (!m_bAni)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (pEffect == NULL)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	Engine::SetUp_OnShader(L"Target_PostEffect_Blur", pEffect, "g_BlurTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(m_pParticleInfo.m_iPath);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CWind::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->BeginPass(m_iBlurPass);
	m_pBufferCom->Render_Buffer();
	pEffect->EndPass();

	Engine::Safe_Release(pEffect);
}

HRESULT CWind::Ready_Object(void)
{
	m_iCntZ = 2;
	m_iInterval = 1;

	if (FAILED(Engine::Ready_Buffers(m_pGraphicDev, RESOURCE_STAGESTATIC, m_wstrName.c_str(), Engine::BUFFER_TRAILTEX, m_iCntX, m_iCntZ, m_iInterval)))
	{
		MSG_BOX("Buffer_Wind Load Failed");
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

	m_pBufferCom->GetVtxInfo(m_pTrailVtx);


	SettingTrail();

	return S_OK;
}

CWind * CWind::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, _int iCntX)
{
	CWind*		pInstance = new CWind(pGraphicDev, wstrName, iCntX);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Wind Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CWind::Free(void)
{
	CEffect::Free();
	Engine::Safe_Delete_Array(m_pTrailVtx);
}

HRESULT CWind::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	// For.Buffer Component
	m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, m_wstrName.c_str());
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));


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

HRESULT CWind::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix	 matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);

	Engine::SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");

	D3DXVECTOR4 color = (D3DXVECTOR4)m_pParticleInfo.m_dColor;

	pEffect->SetVector("g_vColor", &color);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_pParticleInfo.m_iTextureNumber);

	float m_fFar = static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->GetCamFar();

	pEffect->SetFloat("g_fFar", m_fFar);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

void CWind::SetAni(_bool bAni)
{
	m_bAni = bAni;
}

_bool CWind::GetAni(void)
{
	return m_bAni;
}

void CWind::SetTargetTransCom(Engine::CTransform * TranCom)
{
	pTargetTransCom = TranCom;
}

void CWind::SetRotSpeed(_float fSpeed)
{
	m_fRotSpeed = fSpeed;
}

void CWind::SetTextureNumber(_int iTextureNumber)
{
	m_pParticleInfo.m_iTextureNumber = iTextureNumber;
}

void CWind::SetRadius(_float fRadius)
{
	m_fRadius = fRadius;
}

void CWind::SettingTrail(void)
{
	for (int i = 0; i < m_iCntX; ++i)
	{
		_float fTheta = m_fIntervalAngle * i;

		fTheta = D3DXToRadian(fTheta);

		m_pTrailVtx[i].vPosition.x = m_fRadius * cosf(fTheta);
		m_pTrailVtx[i].vPosition.z = m_fRadius * sinf(fTheta);
		m_pTrailVtx[i].vPosition.y = 0.f;


		m_pTrailVtx[i + m_iCntX].vPosition.x = m_fRadius * cosf(fTheta);
		m_pTrailVtx[i + m_iCntX].vPosition.z = m_fRadius * sinf(fTheta);
		m_pTrailVtx[i + m_iCntX].vPosition.y = m_fTrailDotY;
	}

	m_pBufferCom->SetVtxInfo(m_pTrailVtx);
}

void CWind::SetTrailY(_float fY)
{
	m_fTrailDotY = fY;
}

void CWind::SetIntervalAngle(_float fAngle)
{
	m_fIntervalAngle = fAngle;
}

void CWind::SetRenderCheck(_bool bCheck)
{
	m_bRenderCheck = bCheck;
}
