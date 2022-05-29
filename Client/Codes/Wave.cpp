#include "stdafx.h"
#include "Wave.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"



CWave::CWave(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: CEffect(pGraphicDev, wstrName)
, m_fAlphaDecrement(0.0f)
, m_fScaleDecrement(0.0f)
, m_bBilBoardCheck(false)
, m_ePattern(PATTERN_ONE)
, m_fRotateResult(0.0f)
, m_bRendererCheck(false)
, m_bBlurCheck(false)
, m_matTargetWorld(NULL)
, m_bBillboardRotate(false)
{
	m_vOriginScale = _vec3(1.0f, 1.0f, 1.0f);
	m_bAni = false;
	m_bZRotateCheck = false;
	m_fRotateSpeed = 0.0f;

	m_iBlurCnt = 8;
	m_fBlurPower = 1.2f;
	m_fBlurValue = 128.0f;
	m_eBlurType = Engine::BLUR_HORIZON;
}

CWave::~CWave(void)
{
}

void CWave::Update_Object(const _float& fTimeDelta)
{
	if (!m_bAni)
		return;

	SetCheckPattern();

	CEffect::Update_Object(fTimeDelta);

	Update_RePart(); //움짐임 적용

	if (m_bRendererCheck == false)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	else
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT, this);

	if (m_bBlurCheck)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);
}

void CWave::Render_Object(void)
{
	if (!m_bAni)
		return;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();

	if (pEffect == NULL)
		return;

	pEffect->AddRef();


	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	/*if (m_bEffectBlurCheck)
	{
		Engine::SetUp_OnShader(L"Target_PostEffect_Blur", pEffect, "g_BlurTexture");
	}*/

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(m_pParticleInfo.m_iPath);

	m_pGraphicDev->SetFVF(Engine::VTXFVF_PARTICLE);

	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_pParticleInfo.m_PrtN * 2, m_pVtx, sizeof(Engine::VTXPARTICLE));

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

void CWave::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
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

HRESULT CWave::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pParticleInfo.m_PrtN = 1;

	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];

	SetAni();

	return S_OK;
}

void CWave::SetBlurCheck(_bool bCheck)
{
	m_bBlurCheck = bCheck;
}

void CWave::SetPattern(WAVEPATTERN Pattern)
{
	m_ePattern = Pattern;
}

void CWave::SetRotateResult(_float fRotate)
{
	m_fRotateResult = fRotate;
}

void CWave::SetZRotateCheck(_bool bCheck)
{
	m_bZRotateCheck = bCheck;
}

void CWave::SetRotateSpeed(_float fSpeed)
{
	m_fRotateSpeed = fSpeed;
}

void CWave::SetTargetWorld(_matrix * matTarget)
{
	m_matTargetWorld = matTarget;
}

void CWave::SetBillboardRotate(_bool bBheck)
{
	m_bBillboardRotate = bBheck;
}

CWave* CWave::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CWave*		pInstance = new CWave(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Wave Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CWave::Free(void)
{
	CEffect::Free();
	Engine::CEffect::Destroy();
}

HRESULT CWave::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	//For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent) return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));


	// For.Shader Component
	pComponent = Engine::Clone_Prototype(L"Shader_Effect");
	if (pComponent == NULL)
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Wave");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

HRESULT CWave::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	_vec4 color = (_vec4)m_pParticleInfo.m_dColor;

	pEffect->SetVector("g_vColor", &color);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_pParticleInfo.m_iTextureNumber);


	float m_fFar = static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->GetCamFar();

	pEffect->SetFloat("g_fFar", m_fFar);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

void CWave::SetPart(_int nIdx)
{
	Engine::TPART* pPrt = &m_PrtD[nIdx];

	//초기 가속도
	pPrt->IntA = _vec3(0, -0.003f, 0);

	//초기 속도
	pPrt->IntV.x = rand() % 10 / 10.f * 5.f;
	pPrt->IntV.y = -rand() % 10 / 10.f * 5.f;
	pPrt->IntV.z = rand() % 10 / 10.f * 5.f;

	pPrt->IntV *= 0.1f;

	//초기 위치
	pPrt->IntP.x = 0.f;
	pPrt->IntP.y = 0.f;
	pPrt->IntP.z = 0.f;

	//초기 회전
	pPrt->CrnR.x = 0.f;
	pPrt->CrnR.y = 0.f;
	pPrt->CrnR.z = m_fRotateSpeed;

	//회전 속도
	pPrt->CrnRv.x = (rand() % 91 - 45.5f) *1.2f;
	pPrt->CrnRv.y = (rand() % 91 - 45.5f) *1.2f;
	pPrt->CrnRv.z = (rand() % 91 - 45.5f) *1.2f;

	pPrt->CrnRv *= 0.1f;

	//초기 스케일
	pPrt->CrnS.x = 1.f;
	pPrt->CrnS.y = 1.f;
	pPrt->CrnS.z = 1.f;

	//탄성 계수 설정
	pPrt->fElst = 1.0f;

	//공기저항 계수
	pPrt->fDamp = (100 + rand() % 101) * 0.00001f;

	//초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;
	pPrt->CrnA = pPrt->IntA;


	//입자의 생명 요소
	pPrt->bLive = TRUE;
	pPrt->fLife = 30.f + rand() % 71;
	pPrt->fLife = 30.f + rand() % 71;
	pPrt->fLife *= 0.01f;

	pPrt->fFade = (m_pParticleInfo.m_iFade)  * 0.0001f;


	pPrt->dColor = m_pParticleInfo.m_dColor;


	//입자의 표현 요소
	pPrt->PrsW = 50.f;
	pPrt->PrsW *= 0.1f;
	pPrt->PrsH = 50.f;
	pPrt->PrsH *= 0.1f;
	pPrt->PrsImg = 0.0f;
}

void CWave::SetParticleTexuture(_int _Number)
{
	m_pParticleInfo.m_iTextureNumber = _Number;
}

void CWave::MoveUpdate(const _float& fTimeDelta)
{
	//1.운동 갱신
	_float fTime = m_pParticleInfo.m_fTimeAvg * 0.1f;
}

void CWave::Update_RePart(void)
{
	D3DXMATRIX  matX;
	D3DXMATRIX	matY;
	D3DXMATRIX	matZ;
	D3DXMATRIX	matS;
	D3DXMATRIX  matW;
	D3DXMATRIX  matT;

	//4. 출력을 설정

	//카메라 정보

	D3DXMATRIX mtView;
	D3DXMATRIX	matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &mtView);

	if (m_bBilBoardCheck) //빌보드 상태
	{
		//빌보드 매트릭스

		_vec3	m_VCEye;

		_vec3	VcZ;

		D3DXMatrixInverse(&mtView, 0, &mtView);

		matBill = mtView;

		ZeroMemory(&matBill.m[3][0], sizeof(_vec3)); //빌보드 매트릭스


		VcZ = _vec3(matBill._31, matBill._32, matBill._33);



		m_VCEye = _vec3(mtView._41, mtView._42, mtView._43); //카메라 위치

		for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
		{
			Engine::TPART*  pPrt = &m_PrtD[i];

			_vec3 vcP = pPrt->CrnP;

			_vec3 vcTmp = vcP - m_VCEye;

			//카메라의 Z축과 파티클의 위치와 내적
			pPrt->PrsZ = D3DXVec3Dot(&VcZ, &vcTmp);
		}

	}
	else//아닌상태
	{
		_vec3 vcCamZ(mtView._13, mtView._23, mtView._33);


		for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
		{
			Engine::TPART*	pPrt = &m_PrtD[i];

			_vec3	vcP = pPrt->CrnP;

			//카메라 Z축, 파티클의 위치 내적
			pPrt->PrsZ = D3DXVec3Dot(&vcP, &vcCamZ);
		}
	}


	//Sorting
	qsort(m_PrtD, m_pParticleInfo.m_PrtN, sizeof(Engine::TPART)
		, (_int(*) (const void *, const void *))  SortFnc);


	for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		_vec3 vcP = pPrt->CrnP;
		D3DXCOLOR   xcC = pPrt->dColor;


		_float		 fW = pPrt->PrsW;
		_float       fH = pPrt->PrsH;
		_float       fD = min(fW, fH);


		Engine::VTXPARTICLE* pVtx = &m_pVtx[i * 6];


		pPrt->CrnR.z += pPrt->CrnRv.z;

		//Rotation
		
		(&matX, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(pPrt->CrnR.x));
		D3DXMatrixRotationAxis(&matY, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(pPrt->CrnR.y));
		D3DXMatrixRotationAxis(&matZ, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(pPrt->CrnR.z));



		//Scaling
		D3DXMatrixScaling(&matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);

		//Transform
		D3DXMatrixTranslation(&matT, pPrt->CrnP.x, pPrt->CrnP.y, pPrt->CrnP.z);


		//World matrix setup
		//mat = matS * matX * matY * matZ;


		if (m_bBillboardRotate)
		{
			D3DXMatrixInverse(m_matTargetWorld, 0, m_matTargetWorld);

			ZeroMemory(&m_matTargetWorld->m[3][0], sizeof(_vec3)); //빌보드 매트릭스
		}

		
		if (m_bBilBoardCheck)//빌보드 상태이면
		{
			if (m_bZRotateCheck)
			{
				m_matWorld = matS * matZ *matBill * matT;
			}
			else
			{
				if(m_bBillboardRotate)
					m_matWorld = matS  * matBill * (*m_matTargetWorld) * matT;
				else
					m_matWorld = matS  * matBill  * matT;
				
			}
		}
		else
		{
			m_matWorld = matS * matT;
		}


		//vertex
		//vertex setup
		//x, y 
		m_pVtx[i * 6 + 0] = Engine::VTXPARTICLE(-fW, +fH, 0, 0, 0, xcC);
		m_pVtx[i * 6 + 1] = Engine::VTXPARTICLE(+fW, +fH, 0, 1, 0, xcC);
		m_pVtx[i * 6 + 2] = Engine::VTXPARTICLE(-fW, -fH, 0, 0, 1, xcC);
		m_pVtx[i * 6 + 3] = Engine::VTXPARTICLE(+fW, -fH, 0, 1, 1, xcC);


		D3DXVec3TransformCoord(&(pVtx + 0)->p, &(pVtx + 0)->p, &m_matWorld);
		D3DXVec3TransformCoord(&(pVtx + 1)->p, &(pVtx + 1)->p, &m_matWorld);
		D3DXVec3TransformCoord(&(pVtx + 2)->p, &(pVtx + 2)->p, &m_matWorld);
		D3DXVec3TransformCoord(&(pVtx + 3)->p, &(pVtx + 3)->p, &m_matWorld);


		m_pVtx[i * 6 + 4] = m_pVtx[i * 6 + 2];
		m_pVtx[i * 6 + 5] = m_pVtx[i * 6 + 1];


	}
}


void CWave::SetAni(_bool bAni/*= TRUE*/)
{
	m_bAni = bAni;

	if (!m_bAni)
		return;

	for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
		SetPart(i);
}

_bool CWave::GetAni(void)
{
	return m_bAni;
}

void CWave::SetDecrement(_float fAlphaDecrement, _float fScaleDecrement)
{
	m_fAlphaDecrement = fAlphaDecrement, m_fScaleDecrement = fScaleDecrement;
}

void CWave::SetBilBoardCheck(_bool bcheck)
{
	m_bBilBoardCheck = bcheck;
}

void CWave::SetOriginScale(_vec3 vScale)
{
	m_vOriginScale = vScale;
}

void CWave::SetRendererCheck(_bool bCheck)
{
	m_bRendererCheck = bCheck;
}

void CWave::SetCheckPattern(void)
{
	_vec3 vScale;
	m_pTransCom->Get_Scale(&vScale);


	switch (m_ePattern)
	{
	case PATTERN_ONE:
		if (m_pParticleInfo.m_dColor.a < 0.0f)
		{
			m_pParticleInfo.m_dColor.a = 1.0f;
			m_pTransCom->Set_Scale(m_vOriginScale.x, m_vOriginScale.y, m_vOriginScale.z);
			m_bAni = false;
		}
		else
		{
			m_pParticleInfo.m_dColor.a -= m_fAlphaDecrement;

			m_pTransCom->Add_Scale(m_fScaleDecrement);
		}
		break;

	case PATTERN_TWO:
		m_pTransCom->Rotation(Engine::ROT_Z, m_fRotateResult);
		break;

	case PATTERN_THREE:
		m_pTransCom->Rotation(Engine::ROT_Z, m_fRotateResult);

		m_pTransCom->Add_Scale(m_fScaleDecrement);


		if (m_pParticleInfo.m_dColor.a < 1.0f)
		{
			m_pParticleInfo.m_dColor.a += 0.005f;
		}

		if (vScale.x <= 0.6f)
		{
			m_pTransCom->Set_Scale(3.0f, 3.0f, 3.0f);
			m_pParticleInfo.m_dColor.a = 0.f;
		}

		break;

	case PATTERN_FOUR:
		if (vScale.x < 0.f)
			m_bAni = false;
		else
			m_pTransCom->Add_Scale(-0.2f);
		break;

	case PATTERN_FIVE:
		if (m_pParticleInfo.m_dColor.a <= 0.f)
			m_bAni = false;
		else
		{
			m_pTransCom->Add_Scale(0.3f);
			m_pParticleInfo.m_dColor.a -= 0.2f;
		}
		break;


	case PATTERN_SIX:
		if (m_pParticleInfo.m_dColor.a <= 0.f)
			m_bAni = false;
		else
		{
			m_pParticleInfo.m_dColor.a -= 0.1f;
			m_pTransCom->Add_Scale(-0.2f);
		}
		break;


	case PATTERN_SEVEN:
		if (vScale.x < 0.f)
			m_bAni = false;
		else
			m_pTransCom->Add_Scale(-0.1f);
		break;



	case PATTERN_EIGHT:
		if (m_pParticleInfo.m_dColor.a <= 0.f)
			m_bAni = false;
		else
		{
			m_pParticleInfo.m_dColor.a -= 0.05f;
			m_pTransCom->Add_Scale(0.04f);
		}

	case PATTERN_NINE:
		if (m_pParticleInfo.m_dColor.a <= 0.f)
			m_bAni = false;
		else
		{
			m_pParticleInfo.m_dColor.a -= 0.06f;
			m_pTransCom->Add_Scale(0.09f);
		}
		break;
	}


}
