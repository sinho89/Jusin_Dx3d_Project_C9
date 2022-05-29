#include "stdafx.h"
#include "Particle.h"
#include "Export_Engine.h"


CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: CEffect(pGraphicDev,wstrName)
{

}

CParticle::~CParticle(void)
{
	Engine::CEffect::Destroy();
}

void CParticle::Update_Object(const _float& fTimeDelta)
{
	if(!m_bAni)
		return ;

	CEffect::Update_Object(fTimeDelta);


	switch(m_pParticleInfo.m_eMoveMent)
	{
	case Engine::MOVE_NORMAL:
		{
			MoveUpdate(); //� ����
			Update_RePart(); //������ ����
		}
		break;

	case Engine::MOVE_FIRE:
		{
			FireMoveUpdate(); //� ����
			FireUpdate_RePart(); //������ ����
		}
		break;
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
}

void CParticle::Render_Object(void)
{
	if(!m_bAni)
		return ;

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if(pEffect == NULL)
		return ;

	pEffect->AddRef();


	if(FAILED(SetUp_ConstantTable(pEffect)))
		return ;


	pEffect->Begin(0, 0);
	pEffect->BeginPass(m_pParticleInfo.m_iPath);

	m_pGraphicDev->SetFVF(Engine::VTXFVF_PARTICLE);

	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_pParticleInfo.m_PrtN * 2, m_pVtx, sizeof(Engine::VTXPARTICLE));

	pEffect->EndPass();
	pEffect->End();


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);


	Engine::Safe_Release(pEffect);

}

HRESULT CParticle::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pParticleInfo.m_PrtN = 0;

	if(m_pParticleInfo.m_PrtN != 0)
	{
		m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

		//���� � ����
		m_iVtx = m_pParticleInfo.m_PrtN * 6;
		m_pVtx = new Engine::VTXPARTICLE[m_iVtx];
	}


	InitParticle();

	SetAni();

	return S_OK;
}

CParticle* CParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev , const wstring& wstrName)
{
	CParticle*		pInstance = new CParticle(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Particle Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticle::Free(void)
{
	CEffect::Free();
}

HRESULT CParticle::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	//For.Renderer Component
	pComponent = m_pRendererCom	= Engine::Get_Renderer();
	if(NULL == pComponent) return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));


	// For.Shader Component
	pComponent = Engine::Clone_Prototype(L"Shader_Particle");
	if(pComponent == NULL)
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Particle");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

HRESULT CParticle::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix	 matWorld, matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXVECTOR4 color  = (D3DXVECTOR4)m_pParticleInfo.m_dColor;

	pEffect->SetVector("g_vColor", &color);	
	pEffect->SetBool("g_bCheck", m_pParticleInfo.m_iPath);


	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_pParticleInfo.m_iTextureNumber);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

void CParticle::SetPart(_int nIdx)
{
	Engine::TPART* pPrt = &m_PrtD[nIdx];

	//�ʱ� ���ӵ�
	pPrt->IntA = _vec3(0, -0.003f, 0);

	//�ʱ� �ӵ�
	pPrt->IntV.x =  rand()%10 / 10.f * 5.f;
	pPrt->IntV.y =  -rand()%10 / 10.f * 5.f;
	pPrt->IntV.z =  rand()%10 / 10.f * 5.f;

	pPrt->IntV *= 0.1f;

	//�ʱ� ��ġ


	pPrt->IntP.x	= 0.f + rand()%(int)m_pParticleInfo.m_vParticleArrange.x;
	pPrt->IntP.y	= 0.f + rand()%(int)m_pParticleInfo.m_vParticleArrange.y;
	pPrt->IntP.z	= 0.f + rand()%(int)m_pParticleInfo.m_vParticleArrange.z;

	//�ʱ� ȸ��
	pPrt->CrnR.x	= 0.f + rand()%360;
	pPrt->CrnR.y	= 0.f + rand()%360;
	pPrt->CrnR.z	= 0.f + rand()%360;

	//ȸ�� �ӵ�
	pPrt->CrnRv.x	=(rand()%91-45.5f) *1.2f;
	pPrt->CrnRv.y	=(rand()%91-45.5f) *1.2f;
	pPrt->CrnRv.z	=(rand()%91-45.5f) *1.2f;

	pPrt->CrnRv	*=0.1f;

	//�ʱ� ������
	pPrt->CrnS.x	= m_pParticleInfo.m_vScale.x;
	pPrt->CrnS.y	= m_pParticleInfo.m_vScale.y;
	pPrt->CrnS.z	= m_pParticleInfo.m_vScale.z;

	//ź�� ��� ����
	pPrt->fElst = 1.0f;

	//�������� ���
	pPrt->fDamp = (100 + rand()%101) * 0.00001f;

	//�ʱ� ��ġ, �ӵ�, ���ӵ��� ������ ������ �ʱ� ������ ����
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;
	pPrt->CrnA = pPrt->IntA;


	//������ ���� ���
	pPrt->bLive = TRUE;
	pPrt->fLife = 30.f + rand()%71;
	pPrt->fLife = 30.f + rand()%71;
	pPrt->fLife *= 0.01f;

	pPrt->fFade = (m_pParticleInfo.m_iFade) * 0.0001f;


	pPrt->dColor =	m_pParticleInfo.m_dColor;


	//������ ǥ�� ���
	pPrt->PrsW = 50.f;
	pPrt->PrsW *= 0.1f;

	pPrt->PrsH = 50.f;
	pPrt->PrsH *= 0.1f;
}

void CParticle::SetParticleTexuture(_int _Number)
{
	m_pParticleInfo.m_iTextureNumber =  _Number;
}

void CParticle::MoveUpdate(void)
{
	m_pParticleInfo.m_vTotalWind  = m_pParticleInfo.m_vWindDir * m_pParticleInfo.m_fWindPower;
	_vec3 vcWind;

	//1.� ����

	_float fTime = m_pParticleInfo.m_fTimeAvg * 0.1f;

	for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		//���� ������ ���Ѵ�.
		_vec3 vcAirR = pPrt->CrnV; //���������� ���� ����
		_float fLenY = D3DXVec3LengthSq(&vcAirR); //�ӵ��� ���� ũ�� ����

		//���� ������ ���� ���͸� ���Ѵ�.
		D3DXVec3Normalize(&vcAirR, &vcAirR);


		//�̵��ӵ��� �ݴ�� ����
		vcAirR *= -1.0f;

		//�ӷ����� * ���� ���� ����� ����
		vcAirR *= fLenY * pPrt->fDamp;

		//�ٶ��� ���� perturbation�� ���Ѵ�.

		vcWind.x = m_pParticleInfo.m_vTotalWind.x * (10 + rand()%11)/20.f * (1 + sinf(D3DXToRadian(pPrt->CrnR.x)));
		vcWind.y = m_pParticleInfo.m_vTotalWind.y * (10 + rand()%11)/20.f * (1 + sinf(D3DXToRadian(pPrt->CrnR.y)));
		vcWind.z = m_pParticleInfo.m_vTotalWind.z * (10 + rand()%11)/20.f * (1 + sinf(D3DXToRadian(pPrt->CrnR.z)));


		//ȸ�� ��Ҹ� ÷���Ѵ�.
		vcWind.x += 1.8f * sinf(D3DXToRadian(pPrt->CrnR.x));
		vcWind.y += 1.8f * sinf(D3DXToRadian(pPrt->CrnR.y));
		vcWind.z += 1.8f * sinf(D3DXToRadian(pPrt->CrnR.z));

		vcWind *= .8f;

		//1. ���ӵ��� ���������� ���Ѵ�.
		pPrt->CrnA = pPrt->IntA + vcAirR;


		//2. ���� �ӵ� ����
		pPrt->CrnV += pPrt->CrnA * fTime;

		//3. ���� ��ġ ����
		pPrt->CrnP += pPrt->CrnV * fTime;
		pPrt->CrnP += vcWind * fTime;

		//ȸ��
		pPrt->CrnR += pPrt->CrnRv * fTime;

		//4. ���� ��ƼŬ�� ���

		if(pPrt->CrnP.y < 0.0f)
		{
			this->SetPart(i);
		}
	}
}

void CParticle::Update_RePart(void)
{
	D3DXMATRIX  matX;
	D3DXMATRIX	matY;
	D3DXMATRIX	matZ;
	D3DXMATRIX	matS;
	D3DXMATRIX  matW;
	D3DXMATRIX  matT;

	//4. ����� ����

	//ī�޶� ����

	D3DXMATRIX mtView;
	D3DXMATRIX	matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &mtView);

	if(m_pParticleInfo.m_bBillBoard) //������ �����̸�
	{
		//������ ��Ʈ����

		_vec3	m_VCEye;

		_vec3	VcZ;

		D3DXMatrixInverse(&mtView, 0, &mtView);

		matBill = mtView;

		ZeroMemory(&matBill.m[3][0], sizeof(_vec3)); //������ ��Ʈ����


		VcZ = _vec3(matBill._31,matBill._32, matBill._33);

		m_VCEye = _vec3(mtView._41, mtView._42, mtView._43); //ī�޶� ��ġ

		for(_int i= 0; i < m_pParticleInfo.m_PrtN; ++i)
		{
			Engine::TPART*  pPrt = &m_PrtD[i];

			_vec3 vcP = pPrt->CrnP;

			_vec3 vcTmp  = vcP - m_VCEye;

			//ī�޶��� Z��� ��ƼŬ�� ��ġ�� ����
			pPrt->PrsZ = D3DXVec3Dot(&VcZ, &vcTmp);
		}

	}
	else
	{
		_vec3 vcCamZ(mtView._13, mtView._23, mtView._33);


		for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
		{
			Engine::TPART*	pPrt = &m_PrtD[i];

			_vec3	vcP	= pPrt->CrnP;

			//ī�޶� Z��, ��ƼŬ�� ��ġ ����
			pPrt->PrsZ = D3DXVec3Dot(&vcP, &vcCamZ);
		}
	}


	//Sorting
	qsort(m_PrtD, m_pParticleInfo.m_PrtN, sizeof(Engine::TPART)
		,(_int(*) (const void *, const void *))  SortFnc);


	for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		_vec3 vcP = pPrt->CrnP;
		D3DXCOLOR   xcC = pPrt->dColor;


		_float		fW = pPrt->PrsW;
		_float       fH = pPrt->PrsH;
		_float       fD = min(fW, fH);


		Engine::VTXPARTICLE* pVtx = &m_pVtx[i * 6];

		//Rotation
		D3DXMatrixRotationAxis(&matX, &_vec3(1.f,0.f,0.f),D3DXToRadian(pPrt->CrnR.x));
		D3DXMatrixRotationAxis(&matY, &_vec3(0.f,1.f,0.f),D3DXToRadian(pPrt->CrnR.y));
		D3DXMatrixRotationAxis(&matZ, &_vec3(0.f,0.f,1.f),D3DXToRadian(pPrt->CrnR.z));


		//Scaling
		D3DXMatrixScaling(&matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);


		D3DXMatrixTranslation(&matT, pPrt->CrnP.x,pPrt->CrnP.y, pPrt->CrnP.z);


		//World matrix setup
		//mat = matS * matX * matY * matZ;

		//���� ��� ����

		if(m_pParticleInfo.m_bBillBoard)
		{
			m_matWorld =  matS  * matBill * matT;
		}
		else
		{
			m_matWorld = matS * matX * matY * matZ * matT;
		}


		//vertex
		//vertex setup
		//x, y 
		m_pVtx[i*6 + 0]   = Engine::VTXPARTICLE(-fW, +fH, 0, 0, 0, xcC);
		m_pVtx[i*6 + 1]	  =	Engine::VTXPARTICLE(+fW, +fH, 0, 1, 0, xcC);
		m_pVtx[i*6 + 2]   = Engine::VTXPARTICLE(-fW, -fH, 0, 0, 1, xcC);
		m_pVtx[i*6 + 3]   = Engine::VTXPARTICLE(+fW, -fH, 0, 1, 1, xcC);


		D3DXVec3TransformCoord( &(pVtx+0)->p, &(pVtx+0)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+1)->p, &(pVtx+1)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+2)->p, &(pVtx+2)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+3)->p, &(pVtx+3)->p, &m_matWorld);


		m_pVtx[i * 6 + 4]  = m_pVtx[i * 6 + 2];
		m_pVtx[i * 6 + 5]  = m_pVtx[i * 6 + 1];


	}

}

void CParticle::SetParticleCount(_int iCount)
{
	Engine::CEffect::Destroy(); //��ƼŬ ����

	m_pParticleInfo.m_PrtN = iCount;
	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	//���� � ����
	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];


	SetAni();
	SetAvgTime(10.0f);


	m_pTransCom->Set_Position(&m_pParticleInfo.m_vStandard);
}

void CParticle::SetStandardPos(_float pos, _int select)
{
	switch(select)
	{
	case 0: //x��
		m_pParticleInfo.m_vStandard.x = pos;
		break;

	case 1: //y��
		m_pParticleInfo.m_vStandard.y = pos;
		break;

	case 2: //z��
		m_pParticleInfo.m_vStandard.z = pos;
		break;
	}

	m_pTransCom->Set_Position(&m_pParticleInfo.m_vStandard);
}

void CParticle::SetSize(_float size, _int select)
{
	switch(select)
	{
	case 0: //x��
		m_pParticleInfo.m_vScale.x = size;
		break;

	case 1: //y��
		m_pParticleInfo.m_vScale.y = size;
		break;

	case 2: //z��
		m_pParticleInfo.m_vScale.z = size;
		break;
	}
}

void CParticle::SetColor(_float color, _int select)
{
	switch(select)
	{
	case 0: //R
		m_pParticleInfo.m_dColor.r = color;
		break;

	case 1: //G
		m_pParticleInfo.m_dColor.g = color;
		break;

	case 2: //B
		m_pParticleInfo.m_dColor.b = color;
		break;

	case 3: //A
		m_pParticleInfo.m_dColor.a = color;
		break;
	}
}

void CParticle::InitParticle(void)
{
	m_pParticleInfo.m_vStandard  = _vec3(0.f, 0.f, 0.f);

	m_pTransCom->Set_Position(&m_pParticleInfo.m_vStandard);


	m_pParticleInfo.m_vScale = _vec3( (5 + rand()%10)/50.f,
		(10 + rand()%10) /50.f, (5 + rand()%10) /50.f);


	m_pParticleInfo.m_dColor = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.f);

	m_pParticleInfo.m_fWindPower = 2.f;
	m_pParticleInfo.m_vWindDir = _vec3(0.f, -1.f, 0.f);


	m_pParticleInfo.m_vTotalWind = m_pParticleInfo.m_fWindPower * m_pParticleInfo.m_vWindDir;

	m_pParticleInfo.m_fTimeAvg = 10.0f;

	m_pParticleInfo.m_bBillBoard = false;

	m_pParticleInfo.m_eMoveMent = Engine::MOVE_NORMAL;

	m_pParticleInfo.m_vParticleArrange = _vec3(100.f, 300.f, 100.f);

	m_pParticleInfo.m_iFade = 100;
	m_pParticleInfo.m_iFHeight = 30;
}

void CParticle::SetWindControl(_float wind, _int select)
{
	switch(select)
	{
	case 0: //Wind Dir X
		m_pParticleInfo.m_vWindDir.x = wind;
		break;

	case 1: //Wind Dir Y
		m_pParticleInfo.m_vWindDir.y = wind;
		break;

	case 2: //Wind Dir Z
		m_pParticleInfo.m_vWindDir.z = wind;
		break;

	case 3: //Wind Power
		m_pParticleInfo.m_fWindPower = wind;
		break;
	}
}

void CParticle::SetBillBoardCheck(_bool check)
{
	m_pParticleInfo.m_bBillBoard = check;
}

void CParticle::SetMoveMent(Engine::PARTICLE_MOVEMENT move)
{
	m_pParticleInfo.m_eMoveMent = move;
}

void CParticle::FireMoveUpdate(void)
{
	//1.� ����

	_float fTime = m_pParticleInfo.m_fTimeAvg * 0.1f;


	for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];


		//3. ���� ��ġ ����
		pPrt->CrnP += pPrt->CrnV * fTime;


		if(pPrt->CrnP.y > m_pParticleInfo.m_iFHeight)
		{
			pPrt->bLive	= false;
		}

		if(pPrt->bLive == false)
			continue;

		D3DXCOLOR	xc = pPrt->dColor;

		pPrt->fLife -=pPrt->fFade * fTime;

		if(pPrt->fLife <= 0.f)
		{
			pPrt->bLive	= false;
			continue;
		}

		xc.a	= pPrt->fLife;
		pPrt->dColor	= xc;
	}


	// 3. ���� ��ƼŬ�� ����Ѵ�.
	for(_int i=0; i< m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		if(TRUE == pPrt->bLive)
			continue;

		this->FireSetPart(i);
	}
}

void CParticle::FireUpdate_RePart(void)
{
	D3DXMATRIX	matS;
	D3DXMATRIX  matW;
	D3DXMATRIX  matT;

	//4. ����� ����

	//ī�޶� ����

	D3DXMATRIX mtView;
	D3DXMATRIX	matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &mtView);

	if(m_pParticleInfo.m_bBillBoard) //������ �����̸�
	{
		//������ ��Ʈ����

		_vec3	m_VCEye;

		_vec3	VcZ;

		D3DXMatrixInverse(&mtView, 0, &mtView);

		matBill = mtView;

		ZeroMemory(&matBill.m[3][0], sizeof(_vec3)); //������ ��Ʈ����


		VcZ = _vec3(matBill._31, matBill._32, matBill._33);

		m_VCEye = _vec3(mtView._41, mtView._42, mtView._43); //ī�޶� ��ġ

		for(_int i= 0; i < m_pParticleInfo.m_PrtN; ++i)
		{
			Engine::TPART*  pPrt = &m_PrtD[i];

			_vec3 vcP = pPrt->CrnP;

			_vec3 vcTmp  = vcP - m_VCEye;

			//ī�޶��� Z��� ��ƼŬ�� ��ġ�� ����
			pPrt->PrsZ = D3DXVec3Dot(&VcZ, &vcTmp);
		}

	}
	else
	{
		_vec3 vcCamZ(mtView._13, mtView._23, mtView._33);


		for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
		{
			Engine::TPART*	pPrt = &m_PrtD[i];

			_vec3	vcP	= pPrt->CrnP;

			//ī�޶� Z��, ��ƼŬ�� ��ġ ����
			pPrt->PrsZ = D3DXVec3Dot(&vcP, &vcCamZ);
		}
	}


	//Sorting
	qsort(m_PrtD, m_pParticleInfo.m_PrtN, sizeof(Engine::TPART)
		,(_int(*) (const void *, const void *))  SortFnc);


	for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
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


		D3DXMatrixTranslation(&matT, pPrt->CrnP.x,pPrt->CrnP.y, pPrt->CrnP.z);


		//World matrix setup
		//mat = matS * matX * matY * matZ;

		//���� ��� ����


		if(m_pParticleInfo.m_bBillBoard)
		{
			m_matWorld =  matS  * matBill * matT;
		}
		else
		{
			m_matWorld = matS *  matT;
		}


		//vertex
		//vertex setup
		//x, y 
		m_pVtx[i*6 + 0]   = Engine::VTXPARTICLE(-fW, +fH, 0, 0, 0, xcC);
		m_pVtx[i*6 + 1]	  =	Engine::VTXPARTICLE(+fW, +fH, 0, 1, 0, xcC);
		m_pVtx[i*6 + 2]   = Engine::VTXPARTICLE(-fW, -fH, 0, 0, 1, xcC);
		m_pVtx[i*6 + 3]   = Engine::VTXPARTICLE(+fW, -fH, 0, 1, 1, xcC);


		D3DXVec3TransformCoord( &(pVtx+0)->p, &(pVtx+0)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+1)->p, &(pVtx+1)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+2)->p, &(pVtx+2)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+3)->p, &(pVtx+3)->p, &m_matWorld);


		m_pVtx[i * 6 + 4]  = m_pVtx[i * 6 + 2];
		m_pVtx[i * 6 + 5]  = m_pVtx[i * 6 + 1];


	}
}

void CParticle::FireSetPart(_int nIdx)
{
	Engine::TPART* pPrt = &m_PrtD[nIdx];

	FLOAT	fTheta;		// ���� ��
	FLOAT	fPhi;		// ���� ��
	FLOAT	fSpdR;		// �ӵ� ũ��

	//�ʱ� �ӵ��� ��ġ�� �����ϱ� ���� ����
	fTheta	= _float(rand()%61);
	fTheta	-= 30.f;

	fPhi	= _float(rand()%360);

	fSpdR =  10.f + rand()%30;
	fSpdR *= 0.03f;

	// �������� ����
	fTheta	= D3DXToRadian(fTheta);
	fPhi	= D3DXToRadian(fPhi);


	// �ʱ� �ӵ�
	pPrt->IntV.x = fSpdR * sinf(fTheta) * sinf(fPhi);
	pPrt->IntV.y = fSpdR * cosf(fTheta);
	pPrt->IntV.z = fSpdR * sinf(fTheta) * cosf(fPhi);

	// �ʱ� ��ġ		
	pPrt->IntP = _vec3(0.f, 0.f, 0.f) + m_pParticleInfo.m_fFireArrange * _vec3( cosf(fPhi), 0, sinf(fPhi));

	// �ʱ� ��ġ, �ӵ�, ���ӵ��� ������ ������ �ʱ� ������ ����
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;


	// ������ ���� ���
	pPrt->bLive	= TRUE;
	pPrt->fLife	= 30.f + rand()%30;
	pPrt->fLife	*= 0.01f;

	pPrt->fFade	= (m_pParticleInfo.m_iFade) * 0.0001f;

	D3DXCOLOR color =D3DXCOLOR(m_pParticleInfo.m_dColor.r, m_pParticleInfo.m_dColor.g, 
		m_pParticleInfo.m_dColor.b, pPrt->fLife);


	pPrt->dColor =	D3DXCOLOR(color.r, color.g, color.b, pPrt->fLife);


	//�ʱ� ���ӵ�
	pPrt->IntA = _vec3(0, -0.003f, 0);

	//�ʱ� ������
	pPrt->CrnS.x	= m_pParticleInfo.m_vScale.x;
	pPrt->CrnS.y	= m_pParticleInfo.m_vScale.y;
	pPrt->CrnS.z	= m_pParticleInfo.m_vScale.z;


	//������ ǥ�� ���
	pPrt->PrsW = 50.f;
	pPrt->PrsW *= 0.1f;

	pPrt->PrsH = 50.f;
	pPrt->PrsH *= 0.1f;
}

void CParticle::ChangingMoveMent(Engine::PARTICLE_MOVEMENT move)
{
	Engine::CEffect::Destroy();

	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	//���� � ����
	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];

	m_pParticleInfo.m_eMoveMent = move;

	SetAni();
	m_pTransCom->Set_Position(&m_pParticleInfo.m_vStandard);
}

void CParticle::ChangingArrange(_float range, _int select)
{
	switch(select)
	{
		//arrangeX
	case 0:
		m_pParticleInfo.m_vParticleArrange.x = range;
		break;

		//arrangeY
	case 1:
		m_pParticleInfo.m_vParticleArrange.y = range;
		break;

		//arrangeZ
	case 2:
		m_pParticleInfo.m_vParticleArrange.z = range;
		break;

		//FireArrange
	case 3:
		m_pParticleInfo.m_fFireArrange = range;
		break;

	}
}

void CParticle::ChangingFadeArrange(_int range)
{
	m_pParticleInfo.m_iFade  = range;
}

void CParticle::ChangingFHeight(_int hegiht)
{
	m_pParticleInfo.m_iFHeight = hegiht;
}

void CParticle::LoadParticleInfo(Engine::PARTICLEINFO* info)
{
	Engine::CEffect::Destroy();

	memcpy(&m_pParticleInfo, info, sizeof(Engine::PARTICLEINFO));

	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	//���� � ����
	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];

	SetAni();

}

void CParticle::SetAni(_bool bAni/*= TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return ;

	for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		switch(m_pParticleInfo.m_eMoveMent)
		{
		case Engine::MOVE_NORMAL:
			SetPart(i);
			break;
		case Engine::MOVE_FIRE:
			FireSetPart(i);
			break;
		}
	}
}

//���� ����
void CParticle::SetRoof(_bool roof)
{
	if(roof)
	{
		m_pParticleInfo.m_bRoop = true;
		m_bAni = true;
	}
	else
	{
		m_pParticleInfo.m_bRoop = false;

	}

}
