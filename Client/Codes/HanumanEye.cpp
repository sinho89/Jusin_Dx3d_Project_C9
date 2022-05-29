#include "stdafx.h"
#include "HanumanEye.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"

CHanumanEye::CHanumanEye(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
: CEffect(pGraphicDev, wstrName)
, m_pmatTarget(NULL)
, m_bRenderCheck(true)
, m_bBlurCheck(false)
{
	m_bAni = false;
}

CHanumanEye::~CHanumanEye(void)
{
}

void CHanumanEye::Update_Object(const _float & fTimeDelta)
{
	if (g_bHanumanDead)
		return;

	if (!m_bAni)
		return;



	CEffect::Update_Object(fTimeDelta);


	m_pTransCom->Set_WorldMatrix(m_pmatTarget);


	MoveUpdate(fTimeDelta); //운동 설정
	Update_RePart(); //움짐임 적용

	if (!m_bRenderCheck)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	else
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT, this);

	if (m_bBlurCheck)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT_BLUR, this);


}

void CHanumanEye::Render_Object(void)
{
	if (g_bHanumanDead)
		return;

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

void CHanumanEye::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
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

HRESULT CHanumanEye::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

CHanumanEye * CHanumanEye::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHanumanEye*		pInstance = new CHanumanEye(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("HanumanEye Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHanumanEye::Free(void)
{
	CEffect::Free();
	Engine::CEffect::Destroy();
}

HRESULT CHanumanEye::Add_Component(void)
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

HRESULT CHanumanEye::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CHanumanEye::SetPart(_int nIdx)
{
	Engine::TPART* pPrt = &m_PrtD[nIdx];


	pPrt->IntP = _vec3(0.0f, 10.f, 5.0f);

	//초기 가속도
	pPrt->IntA = _vec3(0, -0.003f, 0);

	//초기 속도
	pPrt->IntV.x = rand() % 10 / 10.f * 5.f;
	pPrt->IntV.y = -rand() % 10 / 10.f * 5.f;
	pPrt->IntV.z = rand() % 10 / 10.f * 5.f;

	pPrt->IntV *= 0.1f;


	//초기 회전
	pPrt->CrnR.x = 0.f;
	pPrt->CrnR.y = 0.f;
	pPrt->CrnR.z = 0.f;

	//회전 속도
	pPrt->CrnRv.x = (rand() % 91 - 45.5f) *1.2f;
	pPrt->CrnRv.y = (rand() % 91 - 45.5f) *1.2f;
	pPrt->CrnRv.z = (rand() % 91 - 45.5f) *1.2f;

	pPrt->CrnRv *= 0.1f;

	//초기 스케일
	pPrt->CrnS.x = m_pParticleInfo.m_vScale.x;
	pPrt->CrnS.y = m_pParticleInfo.m_vScale.y;
	pPrt->CrnS.z = m_pParticleInfo.m_vScale.z;

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

	pPrt->fFade = (m_pParticleInfo.m_iFade) * 0.0001f;


	pPrt->dColor = m_pParticleInfo.m_dColor;


	//입자의 표현 요소
	pPrt->PrsW = 50.f;
	pPrt->PrsW *= 0.1f;
	pPrt->PrsH = 50.f;
	pPrt->PrsH *= 0.1f;

	pPrt->PrsImg = 0;
}

void CHanumanEye::MoveUpdate(const _float & fTimeDelta)
{

	//텍스쳐 애니메이션에 대한 파티클을 갱신한다.
	_float fTime = m_pParticleInfo.m_fTimeAvg * 0.1f;

	for (int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];


		pPrt->PrsImg += fTimeDelta * m_pParticleInfo.m_tSpriteInfo.m_fSpriteSpeed;


		if (pPrt->PrsImg > m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame)
		{
			if (m_pParticleInfo.m_bRoop)
			{
				pPrt->PrsImg = (float)(((_int)(pPrt->PrsImg)) % (m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame));
			}
			else
			{
				m_bAni = false;
			}
		}
	}


}

void CHanumanEye::Update_RePart(void)
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
			pPrt->PrsZ = D3DXVec3Dot(&VcZ, &vcTmp);
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


		_float		 fW = pPrt->PrsW;
		_float       fH = pPrt->PrsH;
		_float       fD = min(fW, fH);


		Engine::VTXPARTICLE* pVtx = &m_pVtx[i * 6];

		//Rotation
		D3DXMatrixRotationAxis(&matX, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(pPrt->CrnR.x));
		D3DXMatrixRotationAxis(&matY, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(pPrt->CrnR.y));
		D3DXMatrixRotationAxis(&matZ, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(pPrt->CrnR.z));


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

void CHanumanEye::SetAni(_bool bAni)
{
	m_bAni = bAni;

	if (!m_bAni)
		return;

	for (_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		SetPart(i);
	}
}

_bool CHanumanEye::GetAni(void)
{
	return m_bAni;
}

CHanumanEye * CHanumanEye::LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHanumanEye*		pInstance = new CHanumanEye(pGraphicDev, wstrName);


	if (FAILED(pInstance->LoadReady_Object()))
	{
		MSG_BOX("HanumanEye Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CHanumanEye::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::Find(L"Mesh_Hanuman");

	if (pGameObject == NULL)
		return S_OK;

	Engine::CDynamicMesh* pMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);
	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//Hanuman의 월드행렬
	m_pmatTarget = pTransform->Get_WorldMatrixPointer();



	Engine::Safe_Release(pGameObject);
	return S_OK;
}

HRESULT CHanumanEye::LoadReady_Object(void)
{
	Start_ForScene();

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
