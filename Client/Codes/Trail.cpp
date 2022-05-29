#include "stdafx.h"
#include "Trail.h"
#include "Export_Engine.h"
#include "Trail_Texture.h"
#include "Texture.h"
#include "WitchBlade.h"
#include "PlayerCamera.h"

CTrail::CTrail(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pTrailVtx(NULL)
, m_pBufferCom(NULL)
, m_iCntX(0)
, m_iCntZ(0)
, m_iInterval(0)
, m_pmatTarget(NULL)
, m_iTexture(0)
, m_pPlayerAction(NULL)
, m_iShaderPath(0)
, m_iLocalCoordinates(0)
, m_bPatternSilhouette(false)
{
	m_bIsStatic = true;

}

CTrail::~CTrail(void)
{

}

void CTrail::Update_Object(const _float& fTimeDelta)
{
	CEffect::Update_Object(fTimeDelta);

	ChangingTrailSetting(); //Trail 속성 변경함수


	if (m_strSwordName == L"Sword1") //왼손 검
	{
		if (m_iLocalCoordinates == 0) //Normal 상태의 검의 로컬 좌표계
		{
			vStart = _vec3(0.0f, 0.0f, 0.0f);
			vEnd = _vec3(0.2f, -3.0f, -2.0f);
		}
		else if (m_iLocalCoordinates == 1)
		{
			vStart = _vec3(0.0f, -3.0f, 0.0f);
			vEnd = _vec3(0.2f,  -5.0f, -2.0f);
		}
	}
	else  //오른손
	{
		if (m_iLocalCoordinates == 0)
		{
			vStart = _vec3(0.0f, 0.0f, 0.0f);
			vEnd = _vec3(0.0f, -2.5f, -2.2f);
		}
		else if (m_iLocalCoordinates == 1)
		{
			vStart = _vec3(0.0f, -3.0f, 0.0f);
			vEnd = _vec3(0.0f, -5.5f, -2.2f);
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


	if (m_pPlayerAction[CWitchBlade::ACTION_SILHOUETTEDANCER] ||
		m_pPlayerAction[CWitchBlade::ACTION_SILHOUETTEDANCEL])
	{
		m_vColor.w = max(m_vColor.w - 0.035f, 0.0f);
	}
	else
	{
		m_vColor.w = 1.0f;
		m_bPatternSilhouette = false;
	}


	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	if (m_bEffectBlurCheck)
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);
	}
}

void CTrail::Render_Object(void)
{

	if (m_strSwordName == L"Sword1") //왼손 검
	{
		if (!((m_pPlayerAction[CWitchBlade::ACTION_JUMPATTACKR]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_PLANETDANCE]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_ATTACK1]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_ATTACK2]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_ATTACK4]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_ATTACK5])))
			return;
	}
	else //오른손 검
	{
		if (!((m_pPlayerAction[CWitchBlade::ACTION_JUMPATTACKL]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_PLANETDANCE]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_ATTACK2]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_ATTACK3]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_ATTACK4]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_ATTACK5]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_SILHOUETTEDANCEL]) ||
			(m_pPlayerAction[CWitchBlade::ACTION_SILHOUETTEDANCER])))
			return;
	}
	


	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(pEffect == NULL)
		return ;

	pEffect->AddRef();

	if(FAILED(SetUp_ConstantTable(pEffect)))
		return ;

	/*if (m_bEffectBlurCheck)
	{
		Engine::SetUp_OnShader(L"Target_PostEffect_Blur", pEffect, "g_BlurTexture");
	}*/

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(m_iShaderPath);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CTrail::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->BeginPass(m_iBlurPass);
	m_pGraphicDev->SetFVF(Engine::VTXFVF_PARTICLE);
	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_pParticleInfo.m_PrtN * 2, m_pVtx, sizeof(Engine::VTXPARTICLE));
	pEffect->EndPass();

	Engine::Safe_Release(pEffect);
}

HRESULT CTrail::Ready_Obejct(void)
{
	m_iCntX = 10;
	m_iCntZ = 2;
	m_iInterval = 1;


	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTrailVtx = new Engine::VTXTEX[m_iCntX * m_iCntZ];

	m_pBufferCom->GetVtxInfo(m_pTrailVtx);


	return S_OK;
}

CTrail* CTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev , const wstring& wstrName, wstring& wstrTagName)
{
	CTrail*		pInstance = new CTrail(pGraphicDev, wstrName);

	if(wstrTagName == L"Sword1")
	{
		pInstance->SetBufferName(L"Buffer_Trail");
	}
	else
	{
		pInstance->SetBufferName(L"Buffer_Trail2");

	}

	pInstance->SetName(wstrTagName);


	if(FAILED(pInstance->Ready_Obejct()))
	{
		MSG_BOX("Trail Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrail::Free(void)
{
	CEffect::Free();
	Engine::Safe_Delete_Array(m_pTrailVtx);
}


HRESULT	 CTrail::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create(); 
	if(pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	// For.Buffer Component
	m_pBufferCom = (Engine::CTrail_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, m_strBufferName.c_str());
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));


	//For.Renderer Component
	pComponent = m_pRendererCom	= Engine::Get_Renderer();
	if(NULL == pComponent) return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));


	// For.Shader Component
	pComponent = Engine::Clone_Prototype(L"Shader_Trail");
	if(pComponent == NULL)
		return E_FAIL;
	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Trail");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}


HRESULT		CTrail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

Engine::VTXTEX* CTrail::GetTrail(void)
{
	return m_pTrailVtx;
}

HRESULT CTrail::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;
	
	Engine::CGameObject* pGameObject = Engine::Find(m_strSwordName.c_str());
	Engine::CGameObject* pPlayerObject = Engine::FindWithTag(L"Player");
	
	if (pGameObject == NULL || pPlayerObject == NULL)
		return S_OK;
	
	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//검의 월드행렬
	m_pmatTarget = pTransform->Get_WorldMatrixPointer();

	//Player 동작 확인 _bool 변수 확인
	m_pPlayerAction = ((CWitchBlade*)pPlayerObject)->Get_IsSkill(PLAYER_FIGHT);
	
	Engine::Safe_Release(pTransform);
	Engine::Safe_Release(pGameObject);
	Engine::Safe_Release(pPlayerObject);
	
	return S_OK;
}

void CTrail::SetName(wstring Name)
{
	m_strSwordName = Name;
}

void CTrail::SetBufferName(wstring Name)
{
	m_strBufferName = Name;
}

void CTrail::ChangingTrailSetting(void)
{

	if ( m_pPlayerAction[CWitchBlade::ACTION_SILHOUETTEDANCER] ||
		m_pPlayerAction[CWitchBlade::ACTION_SILHOUETTEDANCEL])
	{
		if (m_bPatternSilhouette)
			return;

		m_iShaderPath = 1;
		m_iTexture = 13;
		m_vColor = _vec4(0.6f, 0.0f, 9.0f, 1.0f);
		m_iLocalCoordinates = 1;
		m_bPatternSilhouette = true;
	}
	else
	{
		m_iShaderPath = 0;
		m_iTexture = 0;
		m_iLocalCoordinates = 0;
	}

}
