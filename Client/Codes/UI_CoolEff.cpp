#include "stdafx.h"
#include "UI_CoolEff.h"
#include "Export_Engine.h"
#include "PlayerCamera.h"
#include "UI_Skill.h"
#include "UIEffect.h"

CUI_CoolEff::CUI_CoolEff(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
	: CEffect(pGraphicDev, wstrName)
{
}

CUI_CoolEff::~CUI_CoolEff(void)
{
}

void CUI_CoolEff::Update_Object(const _float & fTimeDelta)
{
	m_bRender = m_pParent->GetRender();

	m_fX = *m_pParent->GetfX();
	m_fY = *m_pParent->GetfY();

	if (m_bRender)
	{
		CEffect::Update_Object(fTimeDelta);

		SpriteUpdate(fTimeDelta); //Sprite Update
		Update_RePart(); //렌더 설정
		m_pRendererCom->Add_WindowUI(1, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
	}
	else
	{
		Engine::TPART* pPrt = m_PrtD;
		pPrt->PrsImg = 0.f;
	}
}

void CUI_CoolEff::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect)
		return;

	pEffect->AddRef();

	if (FAILED(SetUp_ConstantTable(pEffect)))
		return;

	_uint				iPassCnt;

	_matrix				matWorld, matView;
	_matrix				matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_matWorld._11 = m_fSizeX;
	m_matWorld._22 = m_fSizeY;
	m_matWorld._33 = 1.f;
	m_matWorld._41 = m_fX - WINSIZEX * 0.5f;
	m_matWorld._42 = -m_fY + WINSIZEY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	pEffect->SetMatrix("g_matWorld", &m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 2);

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pGraphicDev->SetFVF(Engine::VTXFVF_PARTICLE);

	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, m_pVtx, sizeof(Engine::VTXPARTICLE));

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);


	Engine::Safe_Release(pEffect);
}

HRESULT CUI_CoolEff::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = *m_pParent->GetfX();
	m_fY = *m_pParent->GetfY();

	m_fSizeX = 25.f;
	m_fSizeY = 25.f;

	m_fAlpha = 0.75f;
	m_fAlphaTime = 0.f;
	m_bRender = false;

	//===========================================
	m_PrtD = new Engine::TPART;

	m_iVtx = 6;
	m_pVtx = new Engine::VTXPARTICLE[m_iVtx];

	m_iMaxFrame = 100;//스프라이트의 총개수
	m_iRow = 10;  //스프라이트 가로 개수
	m_iColumn = 10; //세로 개수

	switch (m_eSkillId)
	{
	case SKILL_DASH:
		m_fFadeTime = 0.f;
		break;
	case SKILL_LIDDLEKICK:
		m_fFadeTime = 20.f;
		break;
	case SKILL_BLARE:
		m_fFadeTime = 20.f;
		break;
	case SKILL_UPATT:
		m_fFadeTime = 20.f;
		break;
	case SKILL_TRICKEY:
		m_fFadeTime = 20.f;
		break;
	case SKILL_FLENITDANCE:
		m_fFadeTime = 20.f;
		break;
	case SKILL_VIFERKICK:
		m_fFadeTime = 20.f;
		break;
	case SKILL_HILLTURN:
		m_fFadeTime = 20.f;
		break;
	case SKILL_SILUETDANCE:
		m_fFadeTime = 20.f;
		break;
	case SKILL_SPININGHEART:
		m_fFadeTime = 20.f;
		break;
	case SKILL_SOFTLANDING:
		m_fFadeTime = 20.f;
		break;
	}

	m_fSpriteSpeed = m_fFadeTime;//스프라이트 속도

	SetPart();

	return S_OK;
}

CUI_CoolEff * CUI_CoolEff::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CUI_CoolEff*		pInstance = new CUI_CoolEff(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CCoolTime Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

CUI_CoolEff* CUI_CoolEff::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName, CUI_Skill* pParent, _float fFadeTime, SKILL_ID eID)
{
	CUI_CoolEff*		pInstance = new CUI_CoolEff(pGraphicDev, wstrName);

	pInstance->SetParent(pParent);
	pInstance->SetFadeTime(fFadeTime);
	pInstance->SetSkillId(eID);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CCoolTime Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_CoolEff::Free(void)
{
	CEffect::Free();
	Engine::Safe_Delete(m_PrtD);
	Engine::Safe_Delete_Array(m_pVtx);
}

HRESULT CUI_CoolEff::Add_Component(void)
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

HRESULT CUI_CoolEff::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
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

void CUI_CoolEff::Update_RePart(void)
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


	Engine::TPART* pPrt = m_PrtD;

	_vec3 vcP = pPrt->CrnP;
	D3DXCOLOR   xcC = pPrt->dColor;


	_float		 fW = pPrt->PrsW;
	_float       fH = pPrt->PrsH;
	_float       fD = min(fW, fH);


	Engine::VTXPARTICLE* pVtx = &m_pVtx[0];

	//Scaling
	D3DXMatrixScaling(&matS, pPrt->CrnS.x, pPrt->CrnS.y, pPrt->CrnS.z);

	//Transform
	D3DXMatrixTranslation(&matT, pPrt->CrnP.x, pPrt->CrnP.y, pPrt->CrnP.z);


	m_matWorld = matS * matT;

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

void CUI_CoolEff::SpriteUpdate(const _float & fTimeDelta)
{
	//스프라이트 업데이트를 돌려준다.
	Engine::TPART* pPrt = m_PrtD;

	pPrt->PrsImg += fTimeDelta * m_fSpriteSpeed;

	if (pPrt->PrsImg > m_iMaxFrame)
	{
		pPrt->PrsImg = 0.f;
	}

}

void CUI_CoolEff::SetPart(void)
{
	Engine::TPART* pPrt = m_PrtD;

	//초기 위치
	pPrt->IntP.x = 0.f;
	pPrt->IntP.y = 0.f;
	pPrt->IntP.z = 0.f;

	//초기 스케일
	pPrt->CrnS.x = 0.5f;
	pPrt->CrnS.y = 0.5f;
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
