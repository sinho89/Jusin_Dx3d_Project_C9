#include "stdafx.h"
#include "SRDust.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"

CSRDust::CSRDust(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_fOriginAlpha(0.0f)
,m_fAlphaDecrement(0.0f)
,m_pTargetTransform(NULL)
,m_iEffectCount(0)
,m_fTickCount(0.0f)
{
	m_bAni = false;
	srand(unsigned(time(NULL)));
}

CSRDust::~CSRDust(void)
{
}

void CSRDust::Update_Object(const _float & fTimeDelta)
{	
	LifeCheck();

	if (!m_bAni)
	{
		m_fTickCount = 0.0f;
		return;
	}

	CEffect::Update_Object(fTimeDelta);

	m_fTickCount += fTimeDelta;

	if (m_fTickCount >= 0.03f)
	{
		if ((m_iEffectCount < m_pParticleInfo.m_PrtN))
		{
			SetPart(m_iEffectCount);
			m_iEffectCount++;
		}
		m_fTickCount = 0.0f;
	}


	MoveUpdate(fTimeDelta); //운동 설정
	Update_RePart(); //움짐임 적용

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
}

void CSRDust::Render_Object(void)
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

void CSRDust::Free(void)
{
	Engine::CEffect::Destroy();
}

HRESULT CSRDust::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Sprite");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

HRESULT CSRDust::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
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

void CSRDust::SetPart(_int nIdx)
{
	Engine::TPART* pPrt = &m_PrtD[nIdx];

	//초기 위치
	pPrt->IntP = m_pTargetTransform->Get_Now_Position();

	pPrt->IntP.x += (rand() % 2) * (((rand() % 2) * 2) - 1);
	pPrt->IntP.y += (rand() % 2) * (((rand() % 2) * 2) - 1);
	pPrt->IntP.z += (rand() % 2) * (((rand() % 2) * 2) - 1);
	
	//초기 스케일

	float Random = (float)(((rand()%13)/14) + 0.3f)  * ( ((rand()%2) * 2) - 1);

	pPrt->CrnS.x = m_pParticleInfo.m_vScale.x + Random;
	pPrt->CrnS.y = m_pParticleInfo.m_vScale.y + Random;
	pPrt->CrnS.z = m_pParticleInfo.m_vScale.z + Random;

	//탄성 계수 설정
	pPrt->fElst = 1.0f;

	//초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;
	pPrt->CrnA = pPrt->IntA;

	//입자의 표현 요소
	pPrt->PrsW = 50.f;
	pPrt->PrsW *= 0.1f;
	pPrt->PrsH = 50.f;
	pPrt->PrsH *= 0.1f;

	pPrt->bLive = true;

	pPrt->PrsImg = 0;

}

void CSRDust::MoveUpdate(const _float & fTimeDelta)
{
	//1.운동 갱신

	_float fTime = m_pParticleInfo.m_fTimeAvg * 0.1f;


	//텍스쳐 애니메이션에 대한 파티클을 갱신한다.

	for (int i = 0; i < m_iEffectCount; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		if (pPrt->bLive == false)
			continue;

		pPrt->PrsImg += fTimeDelta * m_pParticleInfo.m_tSpriteInfo.m_fSpriteSpeed;


		if (pPrt->PrsImg > m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame)
		{
			if (m_pParticleInfo.m_bRoop)
			{
				pPrt->PrsImg = (float)(((_int)(pPrt->PrsImg)) % (m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame));
			}
			else
			{
				pPrt->CrnS = _vec3(0.0f, 0.0f, 0.0f);
				pPrt->bLive = false;
			}
		}
	}
}

void CSRDust::Update_RePart(void)
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

		for (_int i = 0; i < m_iEffectCount; ++i)
		{
			Engine::TPART*  pPrt = &m_PrtD[i];

			_vec3 vcP = pPrt->CrnP;

			_vec3 vcTmp = vcP - m_VCEye;

			//카메라의 Z축과 파티클의 위치와 내적
			pPrt->PrsZ = D3DXVec3Dot(&VcZ, &vcTmp);
		}

	}
	else
	{
		_vec3 vcCamZ(mtView._13, mtView._23, mtView._33);


		for (_int i = 0; i < m_iEffectCount; ++i)
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


	for (_int i = 0; i < m_iEffectCount; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		_vec3 vcP = pPrt->CrnP;
		D3DXCOLOR   xcC = pPrt->dColor;


		_float		 fW = pPrt->PrsW;
		_float       fH = pPrt->PrsH;
		_float       fD = min(fW, fH);


		Engine::VTXPARTICLE* pVtx = &m_pVtx[i * 6];

		//Scaling
		D3DXMatrixScaling(&matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);

		//Transform
		D3DXMatrixTranslation(&matT, pPrt->CrnP.x, pPrt->CrnP.y, pPrt->CrnP.z);


		//World matrix setup
		//mat = matS * matX * matY * matZ;

		//월드 행렬


		if (m_pParticleInfo.m_bBillBoard)
		{
			m_matWorld = matS  * matBill * matT;
		}
		else
		{
			m_matWorld = matS * matT;
		}


		int			nIdxX = ((int)pPrt->PrsImg) % (m_pParticleInfo.m_tSpriteInfo.m_iRow);
		int			nIdxY = ((int)pPrt->PrsImg) / (m_pParticleInfo.m_tSpriteInfo.m_iColumn);
		D3DXVECTOR2 uv00((nIdxX) / (float(m_pParticleInfo.m_tSpriteInfo.m_iRow)), (nIdxY) / (float(m_pParticleInfo.m_tSpriteInfo.m_iColumn)));
		D3DXVECTOR2 uv11((nIdxX + 1) / (float(m_pParticleInfo.m_tSpriteInfo.m_iRow)), (nIdxY + 1) / (float(m_pParticleInfo.m_tSpriteInfo.m_iColumn)));


		//vertex
		//vertex setup
		//x, y 
		m_pVtx[i * 6 + 0] = Engine::VTXPARTICLE(-fW, +fH, 0, uv00.x, uv00.y, xcC);
		m_pVtx[i * 6 + 1] = Engine::VTXPARTICLE(+fW, +fH, 0, uv11.x, uv00.y, xcC);
		m_pVtx[i * 6 + 2] = Engine::VTXPARTICLE(-fW, -fH, 0, uv00.x, uv11.y, xcC);
		m_pVtx[i * 6 + 3] = Engine::VTXPARTICLE(+fW, -fH, 0, uv11.x, uv11.y, xcC);


		D3DXVec3TransformCoord(&(pVtx + 0)->p, &(pVtx + 0)->p, &m_matWorld);
		D3DXVec3TransformCoord(&(pVtx + 1)->p, &(pVtx + 1)->p, &m_matWorld);
		D3DXVec3TransformCoord(&(pVtx + 2)->p, &(pVtx + 2)->p, &m_matWorld);
		D3DXVec3TransformCoord(&(pVtx + 3)->p, &(pVtx + 3)->p, &m_matWorld);


		m_pVtx[i * 6 + 4] = m_pVtx[i * 6 + 2];
		m_pVtx[i * 6 + 5] = m_pVtx[i * 6 + 1];
	}
}

CSRDust * CSRDust::LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CSRDust*		pInstance = new CSRDust(pGraphicDev, wstrName);

	if (FAILED(pInstance->LoadReady_Object()))
	{
		MSG_BOX("SRDust Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSRDust::SetAni(_bool bAni)
{
	m_bAni = bAni;

	if (!m_bAni)
		return;

	SetAvgTime(m_pParticleInfo.m_fTimeAvg);

	for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		SetPart(i);
	}
}

_bool CSRDust::GetAni(void)
{
	return m_bAni;
}

void CSRDust::SetAlphaDecrement(_float fAlpha)
{
	m_fAlphaDecrement = fAlpha;
}

HRESULT CSRDust::LoadReady_Object(void)
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


	_vec4 vColor;
	vColor = (_vec4)m_pParticleInfo.m_dColor;

	m_fOriginAlpha = vColor.w;


	return S_OK;
}

void CSRDust::LifeCheck(void)
{
	int m_iLifeCheck = 0;

	for (int i = 0; i <m_iEffectCount; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		if (pPrt->bLive == true)
			continue;

		m_iLifeCheck++;

		if (m_iLifeCheck == m_pParticleInfo.m_PrtN)
		{
			m_bAni = false;
			m_iEffectCount = 0;
		}
	}
}

void CSRDust::SetTargetTransform(const Engine::CTransform * pTargetTransform)
{
	m_pTargetTransform = pTargetTransform;
}