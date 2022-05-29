#include "stdafx.h"
#include "Sprite.h"
#include "Export_Engine.h"
#include "VIBuffer.h"
#include "MouseCol.h"
#include "MouseCol.h"
#include "PickingRect.h"
#include "Environment.h"
#include "NPC.h"
#include "Monster.h"

CSprite::CSprite(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: CEffect(pGraphicDev, wstrName)
, m_pPickingRect(NULL)
, m_pMouseCol(NULL)
, m_pPickingVtx(NULL)
, m_lMouseMove(0)
, m_bStop(false)
, m_bComplete(false)
, m_bCheck(false)
, m_bCreateRect(false)
{
	ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}

CSprite::~CSprite(void)
{
	Engine::CEffect::Destroy();
}

void CSprite::Update_Object(const _float& fTimeDelta)
{
	if(!m_bAni)
		return ;

	CEffect::Update_Object(fTimeDelta);

	MoveUpdate(fTimeDelta); //운동 설정
	Update_RePart(); //움짐임 적용

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);


//================================================

	if(!m_bCreateRect)
	{
		m_pPickingRect = CPickingRect::Create(m_pGraphicDev, L"PickingRect");
		m_bCreateRect = true;
	}

	SettingWork();

	_vec3 vPos = *m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
	vPos.y += 10.f;
	m_pPickingRect->Set_Positon(&vPos);

	m_pPickingRect->Update_Object(fTimeDelta);
	//==============================================================
}

void CSprite::Render_Object(void)
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


	Engine::Safe_Release(pEffect);


	//=====================================
	m_pPickingRect->Render_Object();
}

HRESULT CSprite::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;


	//=========================================

	m_eLayerType = Engine::LAYER_GAMELOGIC;
	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);
	m_eMode = MODE_MOVE;
	m_pMouseCol = CMouseCol::Create();
	m_pPickingVtx = new Engine::VTXTEX[4]; 
	//===========================================

	m_pParticleInfo.m_PrtN = 0;

	if(m_pParticleInfo.m_PrtN != 0)
	{
		m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

		m_iVtx = m_pParticleInfo.m_PrtN * 6;
		m_pVtx = new Engine::VTXPARTICLE[m_iVtx];
	}


	InitSprite();

	SetAni();

	return S_OK;
}

CSprite* CSprite::Create(LPDIRECT3DDEVICE9 pGraphicDev , const wstring& wstrName)
{
	CSprite*		pInstance = new CSprite(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Sprite Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;

}

void CSprite::Free(void)
{
	CEffect::Free();
	Safe_Delete(m_pMouseCol);
	Engine::Safe_Release(m_pPickingRect);
	Engine::Safe_Delete(m_pPickingVtx);
}

HRESULT CSprite::Add_Component(void)
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
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Sprite");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

HRESULT CSprite::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", m_pParticleInfo.m_iTextureNumber);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

void CSprite::SetPart(_int nIdx)
{
	Engine::TPART* pPrt = &m_PrtD[nIdx];

	//초기 가속도
	pPrt->IntA = _vec3(0, -0.003f, 0);

	//초기 속도
	pPrt->IntV.x =  rand()%10 / 10.f * 5.f;
	pPrt->IntV.y =  -rand()%10 / 10.f * 5.f;
	pPrt->IntV.z =  rand()%10 / 10.f * 5.f;

	pPrt->IntV *= 0.1f;

	//초기 위치
	pPrt->IntP.x	= 0.f;
	pPrt->IntP.y	= 0.f;
	pPrt->IntP.z	= 0.f;

	//초기 회전
	pPrt->CrnR.x	= 0.f + rand()%360;
	pPrt->CrnR.y	= 0.f + rand()%360;
	pPrt->CrnR.z	= 0.f + rand()%360;

	//회전 속도
	pPrt->CrnRv.x	=(rand()%91-45.5f) *1.2f;
	pPrt->CrnRv.y	=(rand()%91-45.5f) *1.2f;
	pPrt->CrnRv.z	=(rand()%91-45.5f) *1.2f;

	pPrt->CrnRv	*=0.1f;

	//초기 스케일
	pPrt->CrnS.x	= m_pParticleInfo.m_vScale.x;
	pPrt->CrnS.y	= m_pParticleInfo.m_vScale.y;
	pPrt->CrnS.z	= m_pParticleInfo.m_vScale.z;

	//탄성 계수 설정
	pPrt->fElst = 1.0f;

	//공기저항 계수
	pPrt->fDamp = (100 + rand()%101) * 0.00001f;

	//초기 위치, 속도, 가속도를 현재의 값들의 초기 값으로 설정
	pPrt->CrnP = pPrt->IntP;
	pPrt->CrnV = pPrt->IntV;
	pPrt->CrnA = pPrt->IntA;


	//입자의 생명 요소
	pPrt->bLive = TRUE;
	pPrt->fLife = 30.f + rand()%71;
	pPrt->fLife = 30.f + rand()%71;
	pPrt->fLife *= 0.01f;

	pPrt->fFade = (m_pParticleInfo.m_iFade) * 0.0001f;


	pPrt->dColor =	m_pParticleInfo.m_dColor;


	//입자의 표현 요소
	pPrt->PrsW = 50.f;
	pPrt->PrsW *= 0.1f;
	pPrt->PrsH = 50.f;
	pPrt->PrsH *= 0.1f;
	pPrt->PrsImg = 0.f;
}

void CSprite::SetParticleTexuture(_int _Number)
{
	m_pParticleInfo.m_iTextureNumber =  _Number;
}

void CSprite::MoveUpdate(const _float& fTimeDelta)
{

	//1.운동 갱신

	_float fTime = m_pParticleInfo.m_fTimeAvg * 0.1f;


	// 2. 파티클의 생명을 갱신한다.
	for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		if(FALSE == pPrt->bLive)
			continue;

		D3DXCOLOR	xc = pPrt->dColor;

		pPrt->CrnS.x	= m_pParticleInfo.m_vScale.x;
		pPrt->CrnS.y	= m_pParticleInfo.m_vScale.y;
		pPrt->CrnS.z	= m_pParticleInfo.m_vScale.z;


		xc.a	= pPrt->fLife;
		pPrt->dColor	= xc;
	}



	//텍스쳐 애니메이션에 대한 파티클을 갱신한다.


	for(int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		Engine::TPART* pPrt = &m_PrtD[i];

		if(pPrt->bLive == FALSE)
			continue;

		pPrt->PrsImg += fTimeDelta * m_pParticleInfo.m_tSpriteInfo.m_fSpriteSpeed;


		if(pPrt->PrsImg > m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame)
		{
			if(m_pParticleInfo.m_bRoop)
			{
				pPrt->PrsImg = (float)(((_int)(pPrt->PrsImg))%(m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame));
			}
			else
			{
				m_bAni = false;
			}
		}
	}


	//4. 죽은 파티클을 재생

	//for(int i = 0; i< m_pParticleInfo.m_PrtN; ++i)
	//{
	//	Engine::TPART* pPrt = &m_PrtD[i];

	//	if(TRUE == pPrt->bLive)
	//		continue;

	//	this->SetPart(i);
	//}


}

void CSprite::Update_RePart(void)
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

	if(m_pParticleInfo.m_bBillBoard) //빌보드 상태이면
	{
		//빌보드 매트릭스

		_vec3	m_VCEye;

		_vec3	VcZ;

		D3DXMatrixIdentity(&matBill);

		D3DXMatrixInverse(&mtView, 0, &mtView);

		matBill = mtView;

		ZeroMemory(&matBill.m[3][0], sizeof(_vec3)); //빌보드 매트릭스

		VcZ = _vec3(matBill._31,matBill._32, matBill._33);

		m_VCEye = _vec3(mtView._41, mtView._42, mtView._43); //카메라 위치

		for(_int i= 0; i < m_pParticleInfo.m_PrtN; ++i)
		{
			Engine::TPART*  pPrt = &m_PrtD[i];

			_vec3 vcP = pPrt->CrnP;

			_vec3 vcTmp  = vcP - m_VCEye;

			//카메라의 Z축과 파티클의 위치와 내적
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

			//카메라 Z축, 파티클의 위치 내적
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


		_float		 fW = pPrt->PrsW;
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

		//월드 행렬 제작
		if(m_pParticleInfo.m_bBillBoard)
		{
			m_matWorld =  matS * matBill * matT;
		}
		else
		{
			m_matWorld = matS * matX * matY * matZ * matT;
		}


		int			nIdxX  = ((int)pPrt->PrsImg)%(m_pParticleInfo.m_tSpriteInfo.m_iRow);
		int			nIdxY  = ((int)pPrt->PrsImg)/(m_pParticleInfo.m_tSpriteInfo.m_iColumn);
		D3DXVECTOR2 uv00( (nIdxX)/(float(m_pParticleInfo.m_tSpriteInfo.m_iRow)), (nIdxY)/(float(m_pParticleInfo.m_tSpriteInfo.m_iColumn)));
		D3DXVECTOR2 uv11( (nIdxX+1)/(float(m_pParticleInfo.m_tSpriteInfo.m_iRow)), (nIdxY + 1)/(float(m_pParticleInfo.m_tSpriteInfo.m_iColumn)));


		//vertex
		//vertex setup
		//x, y 
		m_pVtx[i*6 + 0]   = Engine::VTXPARTICLE(-fW, +fH, 0, uv00.x, uv00.y, xcC);
		m_pVtx[i*6 + 1]	  =	Engine::VTXPARTICLE(+fW, +fH, 0, uv11.x, uv00.y, xcC);
		m_pVtx[i*6 + 2]   = Engine::VTXPARTICLE(-fW, -fH, 0, uv00.x, uv11.y, xcC);
		m_pVtx[i*6 + 3]   = Engine::VTXPARTICLE(+fW, -fH, 0, uv11.x, uv11.y, xcC);


		D3DXVec3TransformCoord( &(pVtx+0)->p, &(pVtx+0)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+1)->p, &(pVtx+1)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+2)->p, &(pVtx+2)->p, &m_matWorld);
		D3DXVec3TransformCoord( &(pVtx+3)->p, &(pVtx+3)->p, &m_matWorld);


		m_pVtx[i * 6 + 4]  = m_pVtx[i * 6 + 2];
		m_pVtx[i * 6 + 5]  = m_pVtx[i * 6 + 1];


	}
}

void CSprite::SetParticleCount(_int iCount)
{
	Engine::CEffect::Destroy(); //스프라이트 삭제

	m_pParticleInfo.m_PrtN = iCount;
	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	//입자 운동 설정
	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];


	SetAni();
	SetAvgTime(10.0f);


	m_pTransCom->Set_Position(&m_pParticleInfo.m_vStandard);
}

void CSprite::InitSprite(void)
{
	m_pParticleInfo.m_vStandard  = _vec3(0.f, 0.f, 0.f);

	m_pTransCom->Set_Position(&m_pParticleInfo.m_vStandard);


	m_pParticleInfo.m_vScale = _vec3(2, 2, 2);


	m_pParticleInfo.m_dColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.f);

	m_pParticleInfo.m_fWindPower = 2.f;
	m_pParticleInfo.m_vWindDir = _vec3(0.f, -1.f, 0.f);


	m_pParticleInfo.m_vTotalWind = m_pParticleInfo.m_fWindPower * m_pParticleInfo.m_vWindDir;

	m_pParticleInfo.m_fTimeAvg = 10.0f;

	m_pParticleInfo.m_bBillBoard = true;

	m_pParticleInfo.m_eMoveMent = Engine::MOVE_NORMAL;

	m_pParticleInfo.m_vParticleArrange = _vec3(100.f, 300.f, 100.f);

	m_pParticleInfo.m_iFade = 100;
	m_pParticleInfo.m_iFHeight = 30;

	m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame = 1;

	m_pParticleInfo.m_tSpriteInfo.m_iRow	= 1;
	m_pParticleInfo.m_tSpriteInfo.m_iColumn = 1;
	m_pParticleInfo.m_tSpriteInfo.m_fSpriteSpeed = 100.f;

	m_pParticleInfo.m_bRoop = true;

}


void CSprite::SetBillBoardCheck(_bool check)
{
	m_pParticleInfo.m_bBillBoard = check;
}

void CSprite::SetMoveMent(Engine::PARTICLE_MOVEMENT move)
{
	m_pParticleInfo.m_eMoveMent = move;
}

void CSprite::SetAni(_bool bAni/*= TRUE*/)
{
	m_bAni = bAni;

	if(!m_bAni)
		return ;

	for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
	{
		SetPart(i);
	}
}

void CSprite::SetSpriteSpeed(_float speed)
{
	m_pParticleInfo.m_tSpriteInfo.m_fSpriteSpeed = speed;
}



void CSprite::SetSpriteFrame(_int frame, _int select)
{
	switch(select)
	{
	case 0: //Max
		m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame = frame;
		break;

	case 1: //Row
		m_pParticleInfo.m_tSpriteInfo.m_iRow = frame;
		break;

	case 2: //Column
		m_pParticleInfo.m_tSpriteInfo.m_iColumn = frame;
		break;
	}

}

void CSprite::SetStandardPos(_float pos, _int select)
{
	switch(select)
	{
	case 0: //x축
		m_pParticleInfo.m_vStandard.x = pos;
		break;

	case 1: //y축
		m_pParticleInfo.m_vStandard.y = pos;
		break;

	case 2: //z축
		m_pParticleInfo.m_vStandard.z = pos;
		break;
	}

	m_pTransCom->Set_Position(&m_pParticleInfo.m_vStandard);
}



void CSprite::SetSize(_float size, _int select)
{
	switch(select)
	{
	case 0: //x축
		m_pParticleInfo.m_vScale.x = size;
		break;

	case 1: //y축
		m_pParticleInfo.m_vScale.y = size;
		break;

	case 2: //z축
		m_pParticleInfo.m_vScale.z = size;
		break;
	}
}


void CSprite::SetColor(_float color, _int select)
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





void CSprite::LoadSpriteInfo(Engine::PARTICLEINFO* info)
{
	Engine::CEffect::Destroy();

	memcpy(&m_pParticleInfo, info, sizeof(Engine::PARTICLEINFO));

	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	//입자 운동 설정
	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];

	SetAni();

}

void CSprite::SetRoof(_bool roof)
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

void CSprite::ResetAnimation(void)
{
	if(m_pParticleInfo.m_bRoop == false)
	{
		m_bAni = true;

		for(_int i = 0; i < m_pParticleInfo.m_PrtN; ++i)
		{
			SetPart(i);
		}
	}
}

CSprite* CSprite::LoadCreate(LPDIRECT3DDEVICE9 pGraphicDev , const wstring& wstrName)
{

	CSprite*		pInstance = new CSprite(pGraphicDev, wstrName);

	if(FAILED(pInstance->LoadReady_Object()))
	{
		MSG_BOX("Sprite Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;

}

HRESULT CSprite::LoadReady_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;


	wstring tPath = L"../../../Client/Bin/Data/EffectData/" + m_wstrName + L".dat";


	DWORD	dwByte = 0;
	HANDLE hFile = CreateFile(tPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Engine::PARTICLEINFO		m_pTempInfo;

	ZeroMemory(&m_pTempInfo, sizeof(Engine::PARTICLEINFO));


	//Effect 구조체 로드
	ReadFile(hFile, &m_pTempInfo, sizeof(Engine::PARTICLEINFO), &dwByte, NULL);

	memcpy(&m_pParticleInfo, &m_pTempInfo, sizeof(Engine::PARTICLEINFO));

	CloseHandle(hFile);

	//=============================================
	m_eLayerType = Engine::LAYER_GAMELOGIC;
	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);
	m_eMode = MODE_MOVE;
	m_pMouseCol = CMouseCol::Create();
	m_pPickingVtx = new Engine::VTXTEX[4]; 
	//=============================================

	m_PrtD = new Engine::TPART[m_pParticleInfo.m_PrtN];

	m_iVtx = m_pParticleInfo.m_PrtN * 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];

	SetAni();

	return S_OK;
}

void CSprite::Stop(void)
{
	m_bStop = true;
}

void CSprite::Complete(void)
{
	m_bComplete = true;
}

Engine::CTransform* CSprite::GetTransCom(void)
{
	return m_pTransCom;
}

OBJECT_MODE CSprite::GetMode(void)
{
	return m_eMode;
}

void CSprite::SetMode(OBJECT_MODE eMode)
{
	m_eMode = eMode;
}

void CSprite::SetObjectInfo(_vec3* vPos, _vec3* vScale, _vec3* vAngle)
{
	m_pTransCom->Set_Position(vPos);
	//m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	//m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
	m_eMode = MODE_STOP;
}

_bool CSprite::GetChecked(void)
{
		return m_bCheck;
}

void CSprite::CheckTrue(void)
{
	m_bCheck = true;
}

void CSprite::CheckFalse(void)
{
	m_bCheck = false;
}

OBJ_INFO* CSprite::GetObjectInfo(void)
{
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_tInfo.m_vPos);
	m_pTransCom->Get_Angle(&m_tInfo.m_vAngle);
	m_pTransCom->Get_Scale(&m_tInfo.m_vScale);


	wstring::iterator iter = m_wstrName.begin();
	wstring::iterator iter_end = m_wstrName.end();

	int i = 0;

	for(; iter != iter_end; ++iter)
	{
		m_tInfo.m_szName[i] = (*iter);
		++i;
	}

	wstring wstrTag = L"Effect";

	iter = wstrTag.begin();
	iter_end = wstrTag.end();

	i = 0;

	for(; iter != iter_end; ++iter)
	{
		m_tInfo.m_szTag[i] = (*iter);
		++i;
	}

	m_tInfo.m_eLayerType = m_eLayerType;

	return &m_tInfo;

}

void CSprite::SettingWork(void)
{
	_vec3	vScale;

	if(m_eMode == MODE_MOVE)
	{
		m_pTransCom->Set_Position(&g_vViewMouse);

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
			m_eMode = MODE_STOP;

	}
	else if(m_eMode == MODE_STOP)
	{
		_matrix*	matRotWorld = NULL;

		if(g_vMouse.x > 15.f)
		{
			float fDist = 9999999999999999.f;
			Engine::VTXTEX* pVtx = NULL;
			((Engine::CVIBuffer*)m_pPickingRect->Get_BufferCom())->GetVtxInfo(m_pPickingVtx);

			_matrix matWorld;
			m_pPickingRect->Get_TransCom()->Get_WorldMatrix(&matWorld);
			if(m_pMouseCol->PickRect(&g_vMouse, m_pPickingVtx, &matWorld, fDist))
			{
				if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
				{
					Engine::CGameObject* pGameObject = NULL;
					if(NULL != Engine::FindList(L"NPC"))
					{
						list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"NPC")->begin();
						list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"NPC")->end();
						for(iter; iter != iter_end;iter++)
						{
							if(((CNPC*)(*iter))->GetMode() == MODE_MODIFY)
							{
								((CNPC*)(*iter))->SetMode(MODE_STOP);
							}
						}
					}

					if(NULL != Engine::FindList(L"Environment"))
					{
						list<Engine::CGameObject*>::iterator  iter_Env = Engine::FindList(L"Environment")->begin();
						list<Engine::CGameObject*>::iterator iter_Env_end = Engine::FindList(L"Environment")->end();
						for(iter_Env; iter_Env != iter_Env_end;iter_Env++)
						{
							if(((CEnvironment*)(*iter_Env))->GetMode() == MODE_MODIFY)
							{
								((CEnvironment*)(*iter_Env))->SetMode(MODE_STOP);
							}
						}
					}

					if(NULL != Engine::FindList(L"Monster"))
					{
						list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Monster")->begin();
						list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Monster")->end();
						for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
						{
							if(((CMonster*)(*iter_Monster))->GetMode() == MODE_MODIFY)
							{
								((CMonster*)(*iter_Monster))->SetMode(MODE_STOP);
							}
						}
					}

					if(NULL != Engine::FindList(L"Effect"))
					{
						list<Engine::CGameObject*>::iterator iter_Effect = Engine::FindList(L"Effect")->begin();
						list<Engine::CGameObject*>::iterator iter_Effect_end = Engine::FindList(L"Effect")->end();
						for(iter_Effect; iter_Effect != iter_Effect_end; iter_Effect++)
						{
							if(((CSprite*)(*iter_Effect))->GetMode() == MODE_MODIFY)
							{
								((CSprite*)(*iter_Effect))->SetMode(MODE_STOP);
							}
						}
					}

					m_eMode = MODE_MODIFY;
				}
			}
		}
	}


	else if(m_eMode == MODE_MODIFY)
	{
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB))
		{
			m_eMode = MODE_STOP;
		}
	}

	if(m_eMode != MODE_MODIFY)
	{
		m_bCheck = false;
	}
}
