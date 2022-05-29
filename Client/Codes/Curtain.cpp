#include "stdafx.h"
#include "Curtain.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"

CCurtain::CCurtain(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
{
	m_bAni = false;
	m_bYBillBoard = false;

	m_iBlurCnt = 2;
	m_fBlurPower = 1.2f;
	m_fBlurValue = 128.0f;
	m_eBlurType = Engine::BLUR_HORIZON;
}

CCurtain::~CCurtain(void)
{
}

void CCurtain::Update_Object(const _float & fTimeDelta)
{
	if (!m_bAni)
		return;

	CEffect::Update_Object(fTimeDelta);

	MoveUpdate(); //운동 설정
	Update_RePart(); //움짐임 적용

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT, this);
	
	if(m_bEffectBlurCheck)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);
}

void CCurtain::Render_Object(void)
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

void CCurtain::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (!m_bAni)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	pEffect->BeginPass(m_iBlurPass);

	m_pGraphicDev->SetFVF(Engine::VTXFVF_PARTICLE);

	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_pParticleInfo.m_PrtN * 2, m_pVtx, sizeof(Engine::VTXPARTICLE));

	pEffect->EndPass();

	Engine::Safe_Release(pEffect);
}

HRESULT CCurtain::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	return S_OK;
}

CCurtain * CCurtain::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CCurtain*		pInstance = new CCurtain(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Curtain Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

CCurtain * CCurtain::LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CCurtain*		pInstance = new CCurtain(pGraphicDev, wstrName);

	if (FAILED(pInstance->LoadReady_Object()))
	{
		MSG_BOX("Curtain Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CCurtain::Free(void)
{
	CEffect::Free();
	Engine::CEffect::Destroy();
}

HRESULT CCurtain::Add_Component(void)
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

HRESULT CCurtain::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

HRESULT CCurtain::LoadReady_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	wstring tPath = L"../Bin/Data/EffectData/" + m_wstrName + L".dat";


	DWORD	dwByte = 0;
	HANDLE hFile = CreateFile(tPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Engine::PARTICLEINFO		m_pTempInfo;

	ZeroMemory(&m_pTempInfo, sizeof(Engine::PARTICLEINFO));

	//Effect 구조체 로드
	ReadFile(hFile, &m_pTempInfo, sizeof(Engine::PARTICLEINFO), &dwByte, NULL);

	memcpy(&m_pParticleInfo, &m_pTempInfo, sizeof(Engine::PARTICLEINFO));

	CloseHandle(hFile);


	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];

	SetAni();

	return S_OK;
}

void CCurtain::SetPart(_int nIdx)
{
	Engine::TPART* pPrt = &m_PrtD[nIdx];

	FLOAT	fTheta;		// 각도 θ
	FLOAT	fPhi;		// 각도 φ
	FLOAT	fSpdR;		// 속도 크기

						//초기 속도와 위치를 설정하기 위한 변수
	fTheta = _float(rand() % 60);
	fTheta -= 30.f;

	fPhi = _float(rand() % 360);

	fSpdR = 10.f + rand() % 30;
	fSpdR *= 0.03f;

	// 라디안으로 변경
	fTheta = D3DXToRadian(fTheta);
	fPhi = D3DXToRadian(fPhi);


	// 초기 속도
	pPrt->IntV.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->IntV.y = fSpdR * cosf(fTheta);
	pPrt->IntV.z = fSpdR * sinf(fTheta) * cosf(fPhi);

	// 초기 위치		
	pPrt->IntP = _vec3(0.f, 0.f, 0.f) + m_pParticleInfo.m_fFireArrange * _vec3(cosf(fPhi), 0, sinf(fPhi));

	// 초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;


	// 입자의 생명 요소
	pPrt->bLive = TRUE;
	pPrt->fLife = 30.f + rand() % 30;
	pPrt->fLife *= 0.01f;

	pPrt->fFade = (m_pParticleInfo.m_iFade) * 0.0001f;

	D3DXCOLOR color = D3DXCOLOR(m_pParticleInfo.m_dColor.r, m_pParticleInfo.m_dColor.g,
		m_pParticleInfo.m_dColor.b, pPrt->fLife);


	pPrt->dColor = D3DXCOLOR(color.r, color.g, color.b, pPrt->fLife);


	//초기 가속도
	pPrt->IntA = _vec3(0, -0.003f, 0);

	//초기 스케일
	pPrt->CrnS.x = m_pParticleInfo.m_vScale.x;
	pPrt->CrnS.y = m_pParticleInfo.m_vScale.y;
	pPrt->CrnS.z = m_pParticleInfo.m_vScale.z;


	//입자의 표현 요소
	pPrt->PrsW = 50.f;
	pPrt->PrsW *= 0.1f;

	pPrt->PrsH = 50.f;
	pPrt->PrsH *= 0.1f;
}

void CCurtain::MoveUpdate(void)
{
	//1.운동 갱신

	_float fTime = m_pParticleInfo.m_fTimeAvg * 0.1f;


	for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];



		//3. 현재 위치 갱신
		//pPrt->CrnP += pPrt->CrnV * fTime;


		if (pPrt->CrnP.y > m_pParticleInfo.m_iFHeight)
		{
			pPrt->bLive = false;
		}

		if (pPrt->bLive == false)
			continue;

		D3DXCOLOR	xc = pPrt->dColor;

		pPrt->fLife -= pPrt->fFade * fTime;

		if (pPrt->fLife <= 0.f)
		{
			pPrt->bLive = false;
			continue;
		}

		xc.a = pPrt->fLife;
		pPrt->dColor = xc;
	}


	// 3. 죽은 파티클을 재생한다.
	for (_int i = 0; i< m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		if (TRUE == pPrt->bLive)
			continue;

		this->SetPart(i);

	}
}

void CCurtain::Update_RePart(void)
{
	D3DXMATRIX	matS;
	D3DXMATRIX  matW;
	D3DXMATRIX  matT;

	//4. 출력을 설정

	//카메라 정보

	D3DXMATRIX mtView;
	D3DXMATRIX	matBill;
	D3DXMATRIX  matYBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &mtView);

	if (m_pParticleInfo.m_bBillBoard) //빌보드 상태이면
	{
		//빌보드 매트릭스

		_vec3	m_VCEye;

		_vec3	VcZ;

		D3DXMatrixIdentity(&matYBill);

		matYBill._11 = mtView._11;
		matYBill._13 = mtView._13;
		matYBill._31 = mtView._31;
		matYBill._33 = mtView._33;
		D3DXMatrixInverse(&matYBill, 0, &matYBill);

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

		_vec3 vcP = pPrt->CrnP;
		D3DXCOLOR   xcC = pPrt->dColor;


		_float		fW = pPrt->PrsW;
		_float       fH = pPrt->PrsH;
		_float       fD = min(fW, fH);


		Engine::VTXPARTICLE* pVtx = &m_pVtx[i * 6];

		//Scaling
		D3DXMatrixScaling(&matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);


		D3DXMatrixTranslation(&matT, pPrt->CrnP.x, pPrt->CrnP.y, pPrt->CrnP.z);


		//World matrix setup
		//mat = matS * matX * matY * matZ;


		//빌보드 공전
		_matrix TransmatWorld;

		m_pTransCom->Get_WorldMatrix(&TransmatWorld);


		D3DXMatrixInverse(&TransmatWorld, 0, &TransmatWorld);

		ZeroMemory(&TransmatWorld.m[3][0], sizeof(_vec3)); //빌보드 매트릭스



		if (m_pParticleInfo.m_bBillBoard)
		{
			if(!m_bYBillBoard)
				m_matWorld = matS  * matBill * matT;
			else //Y축 방향 BillBoard
				m_matWorld = matS  * matYBill *TransmatWorld * matT;
		}
		else
		{
			m_matWorld = matS *  matT;
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

void CCurtain::SetAni(_bool bAni)
{
	m_bAni = bAni;

	if (!m_bAni)
		return;

	for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		SetPart(i);
	}
}

_bool CCurtain::GetAni(void)
{
	return m_bAni;
}

void CCurtain::SetYBillBoard(_bool bYBillBoard)
{
	m_bYBillBoard = bYBillBoard;
}
