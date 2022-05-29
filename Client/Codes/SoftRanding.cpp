#include "stdafx.h"
#include "SoftRanding.h"
#include "Export_Engine.h"
#include "WitchBlade.h"
#include "Wave.h"
#include "SRParticle.h"
#include "SRDust.h"

CSoftRanding::CSoftRanding(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pTargetTransform(NULL)
,m_pLight(NULL)
,m_pSRDust(NULL)
,m_pPlayer(NULL)
, m_pSword_L(NULL)
, m_pSword_R(NULL)
{
	m_bAni = false;
	m_bIsStatic = true;

	for(int i = 0; i < END_STATE; ++i)
		m_pSRParticle[i] = NULL;
}

CSoftRanding::~CSoftRanding(void)
{

}

void CSoftRanding::Update_Object(const _float & fTimeDelta)
{
	/*ACTION_SOFTRANDINGF  : 전방이동
	ACTION_SOFTRANDINGB  : 후방이동
	ACTION_SOFTRANDINGR  : 왼쪽이동
	ACTION_SOFTRANDINGL  : 오른쪽이동*/


	if ((m_pPlayerAction[CWitchBlade::ACTION_SOFTRANDINGF] ||
		m_pPlayerAction[CWitchBlade::ACTION_SOFTRANDINGB] ||
		m_pPlayerAction[CWitchBlade::ACTION_SOFTRANDINGR] ||
		m_pPlayerAction[CWitchBlade::ACTION_SOFTRANDINGL]))
	{
		m_pPlayer->SetOutlineCheck(TRUE);

		m_pSword_L->SetOutlineCheck(TRUE);
		m_pSword_R->SetOutlineCheck(TRUE);

		m_pPlayer->SetOutlineColor(_vec3(1.0f, 1.0f, 1.0f));
		SkillStart();
	}
	else
	{
		m_pPlayer->SetOutlineCheck(FALSE);
		m_pSword_L->SetOutlineCheck(FALSE);
		m_pSword_R->SetOutlineCheck(FALSE);
	}
	
	if(!m_bAni)
		return;

	EndAniCheck();

	m_pLight->Update_Object(fTimeDelta);

	for (int i = 0; i < END_STATE; ++i)
		m_pSRParticle[i]->Update_Object(fTimeDelta);

	m_pSRDust->Update_Object(fTimeDelta);


}

void CSoftRanding::Render_Object(void)
{
	if (!m_bAni)
		return;
	m_pLight->Render_Object();

	for (int i = 0; i < END_STATE; ++i)
		m_pSRParticle[i]->Render_Object();

	m_pSRDust->Render_Object();

}

HRESULT CSoftRanding::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//순간 섬광
	m_pLight = CWave::Create(m_pGraphicDev, L"EffectLight");
	m_pLight->SetAni(false);
	m_pLight->SetBilBoardCheck(true);
	D3DXCOLOR color = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	m_pLight->GetParticleInfo()->m_dColor = color;
	m_pLight->GetParticleInfo()->m_iPath = 2;
	m_pLight->GetParticleInfo()->m_iTextureNumber = 8;
	m_pLight->SetDecrement(0.1f, 0.08f); //알파값 , 스케일값 감소량 설정

	//크기조절
	_vec3 vScale = _vec3(0.2f, 0.2f, 0.2f);
	m_pLight->SetOriginScale(vScale);
	m_pLight->GetTransCom()->Set_Scale(vScale.x, vScale.y, vScale.z);

	//발목 파티클
	for (int i = 0; i < END_STATE; ++i)
	{
		m_pSRParticle[i] = CSRParticle::LoadCreate(m_pGraphicDev, L"SoftRandingAnkleParticle");
		m_pSRParticle[i]->SetAni(false);
		m_pSRParticle[i]->GetTransCom()->Set_Scale(0.15f, 0.15f, 0.15f);
		m_pSRParticle[i]->SetAlphaDecrement(0.015f);

	}

	//Dust
	m_pSRDust = CSRDust::LoadCreate(m_pGraphicDev, L"SoftRandingDust");
	m_pSRDust->SetAni(false);

	return S_OK;
}

CSoftRanding * CSoftRanding::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CSoftRanding* pInstance = new CSoftRanding(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("SoftRanding Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSoftRanding::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pLight);

	for (int i = 0; i < END_STATE; ++i)
		Engine::Safe_Release(m_pSRParticle[i]);

	Engine::Safe_Release(m_pSRDust);
}

HRESULT CSoftRanding::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CSoftRanding::SkillStart(void)
{
	if (m_bAni) //애니메이션 동작 상태이면 실행하지 않는다.
		return;

	_vec3 vPos = m_pTargetTransform->Get_Now_Position();

	m_pLight->GetTransCom()->Set_Position(&vPos);

	m_pLight->SetAni(true);

	for (int i = 0; i < END_STATE; ++i)
		m_pSRParticle[i]->SetAni(true);

	m_pSRDust->SetAni(true);

	m_bAni = true;
}

void CSoftRanding::EndAniCheck(void)
{
	if (m_bAni == true && m_pLight->GetAni() == false && 
		m_pSRParticle[LEFT]->GetAni() == false && m_pSRParticle[RIGHT]->GetAni() == false && m_pSRDust->GetAni() == false)
		m_bAni = false;
}

HRESULT CSoftRanding::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;
	
	//Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");
	m_pPlayer = Engine::FindWithTag(L"Player");

	m_pSword_L = Engine::Find(L"SwordL1");
	m_pSword_R = Engine::Find(L"Sword1");

	Engine::CDynamicMesh* pMesh = (Engine::CDynamicMesh*)m_pPlayer->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);
	Engine::CTransform* pTransform = (Engine::CTransform*)m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	if (m_pPlayer == NULL || pTransform == NULL || pMesh == NULL)
		return S_OK;

	m_pPlayerAction = ((CWitchBlade*)m_pPlayer)->Get_IsSkill(PLAYER_FIGHT);


	m_pTargetTransform = pTransform;

	////왼쪽발목 파티클
	const _matrix* m_pTargetWorld = NULL;
	const _matrix* m_pmatBone = NULL;

	m_pTargetWorld = pTransform->Get_WorldMatrixPointer(); //Target의 월드행렬


	m_pmatBone = pMesh->Get_FrameByName("_109"); //왼쪽 발목 본
	m_pSRParticle[LEFT]->SetBone(m_pTargetWorld, m_pmatBone);

	m_pmatBone = pMesh->Get_FrameByName("_103"); //오른쪽 발목 본
	m_pSRParticle[RIGHT]->SetBone(m_pTargetWorld, m_pmatBone);


	m_pSRDust->SetTargetTransform(pTransform);


	Engine::Safe_Release(pMesh);
	Engine::Safe_Release(pTransform);
	Engine::Safe_Release(m_pPlayer);

	return S_OK;
}
