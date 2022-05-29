#include "stdafx.h"
#include "SummonParticle.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"

CSummonParticle::CSummonParticle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, _int Count)
:CEffect(pGraphicDev, wstrName)
, m_bRenderGroupCheck(true)
, m_fGoalResult(0.1f)
, m_fRadius(60.f)
{
	srand(unsigned(time(NULL)));
	m_bAni = false;
	m_pParticleInfo.m_PrtN = Count;
}

CSummonParticle::~CSummonParticle(void)
{
}

void CSummonParticle::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	CEffect::Update_Object(fTimeDelta);


	MoveUpdate(); //운동 설정
	Update_RePart(); //움짐임 적용

	if (m_bRenderGroupCheck == false)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	else
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT, this);

	if (m_bEffectBlurCheck)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);
}

void CSummonParticle::Render_Object(void)
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
	pEffect->BeginPass(m_pParticleInfo.m_iPath);

	m_pGraphicDev->SetFVF(Engine::VTXFVF_PARTICLE);

	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_pParticleInfo.m_PrtN * 2, m_pVtx, sizeof(Engine::VTXPARTICLE));

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CSummonParticle::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	//입자 운동 설정
	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];

	//파티클 속성 세팅
	InitSphereParticle();

	//파티클 속성 적용
	SetAni();
	_vec3 vPos = _vec3(0.f, 3.f, 0.f);
	m_pTransCom->Set_Position(&vPos);
	return S_OK;
}

CSummonParticle * CSummonParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, _int Count)
{
	CSummonParticle*		pInstance = new CSummonParticle(pGraphicDev, wstrName, Count);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("SummonParticle Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CSummonParticle::Free(void)
{
	CEffect::Free();
	Engine::CEffect::Destroy();
}

HRESULT CSummonParticle::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Particle");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

HRESULT CSummonParticle::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CSummonParticle::SetRenderGroupCheck(_bool bCheck)
{
	m_bRenderGroupCheck = bCheck;
}

void CSummonParticle::SetGoalResult(_float fResult)
{
	m_fGoalResult = fResult;
}

void CSummonParticle::SetPart(_int nIdx)
{
	Engine::TPART* pPrt = &m_PrtD[nIdx];


	//초기 가속도
	pPrt->IntA = _vec3(0, -0.003f, 0);


	//초기 위치
	_float	fPhiX, fPhiY;
	// 각도 φ
	fPhiX = (_float)(rand() % 360);
	fPhiY = (_float)(rand() % 360);
	// 라디안으로 변경
	fPhiX = D3DXToRadian(fPhiX);
	fPhiY = D3DXToRadian(fPhiY);


	pPrt->IntP.x = (_float)(m_fRadius * sinf(fPhiX) * cosf(fPhiY));
	pPrt->IntP.y = (_float)(m_fRadius * cosf(fPhiX));
	pPrt->IntP.z = (_float)(m_fRadius * sinf(fPhiX) * sinf(fPhiY));


	//초기 속도
	pPrt->IntV.x = rand() % 10 / 10.f * 5.f;
	pPrt->IntV.y = -rand() % 10 / 10.f * 5.f;
	pPrt->IntV.z = rand() % 10 / 10.f * 5.f;

	pPrt->IntV *= 0.1f;


	//초기 회전
	pPrt->CrnR.x = 0.f + rand() % 360;
	pPrt->CrnR.y = 0.f + rand() % 360;
	pPrt->CrnR.z = 0.f + rand() % 360;

	//회전 속도
	pPrt->CrnRv.x = (rand() % 91 - 45.5f) *1.2f;
	pPrt->CrnRv.y = (rand() % 91 - 45.5f) *1.2f;
	pPrt->CrnRv.z = (rand() % 91 - 45.5f) *1.2f;

	pPrt->CrnRv *= 0.1f;

	//초기 스케일


	pPrt->CrnS.x = 0.f;
	pPrt->CrnS.y = 0.f;
	pPrt->CrnS.z = 0.f;

	//탄성 계수 설정
	pPrt->fElst = ((rand() % 30) + 50.f) * 0.009f;

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
	pPrt->fFade = (m_pParticleInfo.m_iFade) * 0.0001f;
	pPrt->dColor = m_pParticleInfo.m_dColor;


	//입자의 표현 요소
	pPrt->PrsW = 50.f;
	pPrt->PrsW *= 0.1f;

	pPrt->PrsH = 50.f;
	pPrt->PrsH *= 0.1f;

	pPrt->bLive = true;
}

void CSummonParticle::MoveUpdate(void)
{
	_vec3 Center = _vec3(0.0f, 0.0f, 0.0f);

	//1.운동 갱신

	_float fTime = m_pParticleInfo.m_fTimeAvg * 0.1f;

	for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		if (!pPrt->bLive)
			continue;

		if (pPrt->CrnS.x <= 0.9f)
		{
			pPrt->CrnS += _vec3(0.002f, 0.002f, 0.002f);
		}


		_vec3 vDir = Center - pPrt->CrnP;

		_float fDistance = D3DXVec3Length(&vDir);

		D3DXVec3Normalize(&vDir, &vDir);

		pPrt->CrnP += vDir * fTime * pPrt->fElst;

		if (fDistance <= m_fGoalResult)
		{
			pPrt->bLive = false;
		}

	}
}

void CSummonParticle::Update_RePart(void)
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

	if (m_pParticleInfo.m_bBillBoard) //빌보드 상태이면
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
			pPrt->PrsZ = D3DXVec3Dot(&vcTmp, &vcTmp);
		}

	}
	else
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

		if (pPrt->bLive == false)
			continue ;

		_vec3 vcP = pPrt->CrnP;
		D3DXCOLOR   xcC = pPrt->dColor;


		_float		fW = pPrt->PrsW;
		_float       fH = pPrt->PrsH;
		_float       fD = min(fW, fH);


		Engine::VTXPARTICLE* pVtx = &m_pVtx[i * 6];

		//Rotation
		D3DXMatrixRotationAxis(&matX, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(pPrt->CrnR.x));
		D3DXMatrixRotationAxis(&matY, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(pPrt->CrnR.y));
		D3DXMatrixRotationAxis(&matZ, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(pPrt->CrnR.z));


		//Scaling
		D3DXMatrixScaling(&matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);


		D3DXMatrixTranslation(&matT, pPrt->CrnP.x, pPrt->CrnP.y, pPrt->CrnP.z);


		//World matrix setup
		//mat = matS * matX * matY * matZ;


		if (m_pParticleInfo.m_bBillBoard)
		{
			m_matWorld = matS  * matBill * matT;
		}
		else
		{
			m_matWorld = matS  * matT;
		}


		//vertex
		//vertex setup
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

void CSummonParticle::SetAni(_bool bAni)
{
	m_bAni = bAni;

	if (!m_bAni)
		return;

	for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		SetPart(i);
	}
}

_bool CSummonParticle::GetAni(void)
{
	return m_bAni;
}

void CSummonParticle::InitSphereParticle(void)
{
	//scale
	m_pParticleInfo.m_vScale = _vec3(0.f, 0.0f, 0.0f);

	//색깔
	m_pParticleInfo.m_dColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.f);

	m_pParticleInfo.m_fWindPower = 2.f;
	m_pParticleInfo.m_vWindDir = _vec3(0.f, -1.f, 0.f);


	m_pParticleInfo.m_vTotalWind = m_pParticleInfo.m_fWindPower * m_pParticleInfo.m_vWindDir;

	m_pParticleInfo.m_fTimeAvg = 10.0f;

	m_pParticleInfo.m_bBillBoard = true;

	m_pParticleInfo.m_eMoveMent = Engine::MOVE_END;

	m_pParticleInfo.m_vParticleArrange = _vec3(100.f, 300.f, 100.f);

	m_pParticleInfo.m_iFade = 100;
	m_pParticleInfo.m_iFHeight = 30;

	m_pParticleInfo.m_iPath = 2;

	m_pParticleInfo.m_iTextureNumber = 12;
}

void CSummonParticle::SetRadius(_float fRadius)
{
	m_fRadius = fRadius;
}
