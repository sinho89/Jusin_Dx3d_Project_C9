#include "stdafx.h"
#include "ObjectName.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"
#include "Player.h"

CObjectName::CObjectName(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	: CEffect(pGraphicDev, wstrName)
{
}

CObjectName::~CObjectName(void)
{
}

void CObjectName::Update_Object(const _float & fTimeDelta)
{
	CEffect::Update_Object(fTimeDelta);

	SpriteUpdate(fTimeDelta); //Sprite Update
	Update_RePart(); //렌더 설정

					 //원하시는 렌더러 그룹에 넣어주세요 
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_POSTEFFECT, this);

}

void CObjectName::Render_Object(void)
{

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect)
		return;

	pEffect->AddRef();

	_uint				iPassCnt;
	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;
	
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 2);


	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(2);

	m_pGraphicDev->SetFVF(Engine::VTXFVF_PARTICLE);

	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, m_pVtx, sizeof(Engine::VTXPARTICLE));

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
}

HRESULT CObjectName::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = 20.f;
	m_fSizeY = 20.f;

	m_fAlpha = 0.75f;

	m_bIsStatic = true;


	//===========================================
	m_PrtD = new Engine::TPART;

	m_iVtx = 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];


	m_iMaxFrame = 100;//스프라이트의 총개수
	m_iRow = 10;  //스프라이트 가로 개수
	m_iColumn = 10; //세로 개수
	m_fSpriteSpeed = 30.f;//스프라이트 속도

	m_bBillBoard = true;
	SetPart();

	return S_OK;
}

CObjectName * CObjectName::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CObjectName*		pInstance = new CObjectName(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CObjectName Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CObjectName* CObjectName::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, Engine::CGameObject* pParentObject)
{
	CObjectName*		pInstance = new CObjectName(pGraphicDev, wstrName);

	pInstance->SetParent(pParentObject);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("UI_Sprite Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CObjectName::Free(void)
{
	CEffect::Free();
	Engine::Safe_Delete(m_PrtD);
	Engine::Safe_Delete_Array(m_pVtx);
}

HRESULT CObjectName::Add_Component(void)
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
	pComponent = Engine::Clone_Prototype(L"Shader_UI");
	if (pComponent == NULL)
		return E_FAIL;

	m_pShaderCom = (Engine::CShader*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_UI");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));


	return S_OK;
}

HRESULT CObjectName::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

	//Texture
	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 6);

	float m_fFar = static_cast<CPlayerCamera*>(Engine::GetCurrentCamera())->GetCamFar();

	pEffect->SetFloat("g_fFar", m_fFar);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

void CObjectName::Update_RePart(void)
{
	D3DXMATRIX  matX;
	D3DXMATRIX	matY;
	D3DXMATRIX	matZ;
	D3DXMATRIX	matS;
	D3DXMATRIX  matW;
	D3DXMATRIX  matT;

	D3DXMATRIX mtView;
	D3DXMATRIX	matBill;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &mtView);

	if (m_bBillBoard) //빌보드 상태이면
	{
		//빌보드 매트릭스

		_vec3	m_VCEye;

		_vec3	VcZ;

		D3DXMatrixInverse(&mtView, 0, &mtView);

		matBill = mtView;

		ZeroMemory(&matBill.m[3][0], sizeof(_vec3)); //빌보드 매트릭스

		VcZ = _vec3(matBill._31, matBill._32, matBill._33);

		m_VCEye = _vec3(mtView._41, mtView._42, mtView._43); //카메라 위치
		
		Engine::TPART*  pPrt = &m_PrtD[0];

		_vec3 vcP = pPrt->CrnP;

		_vec3 vcTmp = vcP - m_VCEye;

		//카메라의 Z축과 파티클의 위치와 내적
		pPrt->PrsZ = D3DXVec3Dot(&VcZ, &vcTmp);
		
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


	Engine::TPART* pPrt = m_PrtD;

	_vec3 vcP = pPrt->CrnP;
	D3DXCOLOR   xcC = pPrt->dColor;


	_float		 fW = pPrt->PrsW;
	_float       fH = pPrt->PrsH;
	_float       fD = min(fW, fH);


	Engine::VTXPARTICLE* pVtx = &m_pVtx[0];

	//Scaling
	D3DXMatrixScaling(&matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);


	//Rotation
	
	(&matX, &_vec3(1.f, 0.f, 0.f), D3DXToRadian(pPrt->CrnR.x));
	D3DXMatrixRotationAxis(&matY, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(pPrt->CrnR.y));
	D3DXMatrixRotationAxis(&matZ, &_vec3(0.f, 0.f, 1.f), D3DXToRadian(pPrt->CrnR.z));

	//Transform

	D3DXMatrixTranslation(&matT, pPrt->CrnP.x, pPrt->CrnP.y, pPrt->CrnP.z);


	m_matWorld = matS  * matBill * matT;


	//=======UV계산=====================================================================//
	int			nIdxX = ((int)pPrt->PrsImg) % (m_iRow);
	int			nIdxY = ((int)pPrt->PrsImg) / (m_iColumn);
	D3DXVECTOR2 uv00((nIdxX) / (float(m_iRow)), (nIdxY) / (float(m_iColumn)));
	D3DXVECTOR2 uv11((nIdxX + 1) / (float(m_iRow)), (nIdxY + 1) / (float(m_iColumn)));
	//=====================================================================================//

	//vertex
	//vertex setup
	//x, y 
	m_pVtx[0] = Engine::VTXPARTICLE(-fW, +fH, 0, uv00.x, uv00.y, xcC);
	m_pVtx[1] = Engine::VTXPARTICLE(+fW, +fH, 0, uv11.x, uv00.y, xcC);
	m_pVtx[2] = Engine::VTXPARTICLE(-fW, -fH, 0, uv00.x, uv11.y, xcC);
	m_pVtx[3] = Engine::VTXPARTICLE(+fW, -fH, 0, uv11.x, uv11.y, xcC);


	D3DXVec3TransformCoord(&(pVtx + 0)->p, &(pVtx + 0)->p, &m_matWorld);
	D3DXVec3TransformCoord(&(pVtx + 1)->p, &(pVtx + 1)->p, &m_matWorld);
	D3DXVec3TransformCoord(&(pVtx + 2)->p, &(pVtx + 2)->p, &m_matWorld);
	D3DXVec3TransformCoord(&(pVtx + 3)->p, &(pVtx + 3)->p, &m_matWorld);

	m_pVtx[4] = m_pVtx[2];
	m_pVtx[5] = m_pVtx[1];



}

void CObjectName::SpriteUpdate(const _float & fTimeDelta)
{
	//스프라이트 업데이트를 돌려준다.
	Engine::TPART* pPrt = m_PrtD;

	pPrt->PrsImg += fTimeDelta * m_fSpriteSpeed;

	if (pPrt->PrsImg > m_iMaxFrame)
		pPrt->PrsImg = (float)(((_int)(pPrt->PrsImg)) % (m_iMaxFrame));

}

void CObjectName::SetPart(void)
{
	Engine::TPART* pPrt = m_PrtD;

	//초기 위치
	/*if (m_pParentObject->GetName() == L"Mesh_NPC_Weapon" || m_pParentObject->GetName() == L"Mesh_NPC_Armor" || m_pParentObject->GetName() == L"Mesh_NPC_Potion" || m_pParentObject->GetName() == L"Mesh_NPC_Book")
	{
		_vec3 vParentPos;

		((Engine::CTransform*)m_pParentObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC))->Get_Infomation(Engine::INFO_POSITION, &vParentPos);

		pPrt->CrnP.x = vParentPos.x;
		pPrt->CrnP.y = vParentPos.y;
		pPrt->CrnP.z = vParentPos.z;
	}*/
	pPrt->CrnP.x = 0.f;
	pPrt->CrnP.y = 0.f;
	pPrt->CrnP.z = 0.f;
	//초기 스케일
	pPrt->CrnS.x = 1.f;
	pPrt->CrnS.y = 1.f;
	pPrt->CrnS.z = 1.f;

	//초기 위치 현재의 값들의 초기 값으로 설정
	pPrt->CrnP = pPrt->IntP;

	//입자의 표현 요소
	pPrt->PrsW = 1.f;
	//pPrt->PrsW *= 0.1f;
	pPrt->PrsH = 1.f;
	//pPrt->PrsH *= 0.1f;

	//스프라이트 처음 위치
	pPrt->PrsImg = 0.0f;

	//랜덤위치를 원한다면
	//pPrt->PrsImg = (_float)(rand() % (m_pParticleInfo.m_tSpriteInfo.m_iMaxFrame));
}
