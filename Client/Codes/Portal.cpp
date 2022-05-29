#include "stdafx.h"
#include "Portal.h"
#include "Export_Engine.h"
#include "Reflect.h"
#include "Wave.h"
#include "SphereParticle.h"
#include "Sound.h"
#include "Player.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
,m_pReflect(NULL)
,m_pWave(NULL)
,m_pParticle(NULL)
,m_pPortalParticle(NULL)
, m_pSoundCom(NULL)
, m_pPlayerTrans(NULL)
{
	ZeroMemory(&m_pTwist, sizeof(CWave*) * 4);

}

CPortal::~CPortal(void)
{
}

HRESULT CPortal::Start_ForScene(void)
{
	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return E_FAIL;

	m_pPlayerTrans = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);


	Engine::Safe_Release(m_pPlayerTrans);
	Engine::Safe_Release(pGameObject);

	return S_OK;
}

void CPortal::Update_Object(const _float & fTimeDelta)
{

	CEffect::Update_Object(fTimeDelta);

	_matrix   matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);


	m_pReflect->Update_Object(fTimeDelta);
	m_pReflect->GetTransCom()->Set_ParentMatrix(&matWorld);

	m_pWave->Update_Object(fTimeDelta);
	m_pWave->GetTransCom()->Set_ParentMatrix(&matWorld);

	for (int i = 0; i < 4; ++i)
	{
		m_pTwist[i]->Update_Object(fTimeDelta);
		m_pTwist[i]->GetTransCom()->Set_ParentMatrix(&matWorld);
	}

	m_pParticle->GetTransCom()->Rotation(Engine::ROT_Z, -1.0f);
	m_pParticle->Update_Object(fTimeDelta);
	m_pParticle->GetTransCom()->Set_ParentMatrix(&matWorld);

	m_pPortalParticle->GetTransCom()->Rotation(Engine::ROT_Z, -0.6f);
	m_pPortalParticle->Update_Object(fTimeDelta);
	m_pPortalParticle->GetTransCom()->Set_ParentMatrix(&matWorld);

	_vec3 vPos;
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
	_vec3 vTargetPos;
	m_pPlayerTrans->Get_Infomation(Engine::INFO_POSITION, &vTargetPos);

	_float fDist = D3DXVec3Length(&(vPos - vTargetPos));


	if (fDist < 40.f)
	{
		m_pSoundCom->MyPlaySound(L"Sound", true);
	}
	else
		m_pSoundCom->MyStopSoundAll();

}

void CPortal::Render_Object(void)
{
	m_pReflect->Render_Object();
	m_pWave->Render_Object();

	for (int i = 0; i < 4; ++i)
	{
		m_pTwist[i]->Render_Object();
	}

	m_pParticle->Render_Object();


	m_pPortalParticle->Render_Object();
}

HRESULT CPortal::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pReflect = CReflect::Create(m_pGraphicDev, L"Reflect");
	m_pWave = CWave::Create(m_pGraphicDev, L"PortalWave");

	for (int i = 0; i < 4; ++i)
	{
		m_pTwist[i] = CWave::Create(m_pGraphicDev, L"Twist");
	}

	const _matrix*	m_matWorld = NULL;
	_matrix* matResultWorld = NULL;

	m_matWorld = m_pTransCom->Get_WorldMatrixPointer();
	matResultWorld = const_cast<_matrix*>(m_matWorld);
	

	m_pParticle = CSphereParticle::Create(m_pGraphicDev, L"SphereParticle", 500);
	m_pParticle->SetBillBoardRotateCheck(true);
	m_pParticle->SetTargetWorld(matResultWorld);


	m_pPortalParticle = CSphereParticle::Create(m_pGraphicDev, L"SphereParticle2", 250);
	m_pPortalParticle->SetBillBoardRotateCheck(true);
	m_pPortalParticle->SetTargetWorld(matResultWorld);

	return S_OK;
}

CPortal * CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CPortal*		pInstance = new CPortal(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("Portal Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortal::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pReflect);
	Engine::Safe_Release(m_pWave);
	Engine::Safe_Release(m_pParticle);
	Engine::Safe_Release(m_pPortalParticle);

	for (int i = 0; i < 4; ++i)
	{
		Engine::Safe_Release(m_pTwist[i]);
	}
}

HRESULT CPortal::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Transform Component
	pComponent = m_pSoundCom = CSound::Create(m_pTransCom);
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Sound", pComponent));
	
	m_pSoundCom->Set_Sound(L"Sound", L"PortalSound.wav");
	return S_OK;
}

//포탈 초기 설정
void CPortal::SkillStart(void)
{
	D3DXCOLOR color = _vec4(0.5f, 0.f, 1.f, 1.f);
	//Wave====================================================
	m_pWave->GetParticleInfo()->m_dColor = color;
	m_pWave->GetParticleInfo()->m_iPath = 2;
	m_pWave->GetParticleInfo()->m_iTextureNumber = 9;

	//Pos
	//_vec3 vWavePos = m_pTransCom->Get_Now_Position();

	_vec3 vWavePos = _vec3(0.f, 0.f, 0.f);

	vWavePos.x = 0.0f;
	vWavePos.z += 1.f;
	m_pWave->GetTransCom()->Set_Position(&vWavePos);

	//Rotate
	m_pWave->SetRotateResult(-1.f);
	m_pWave->SetPattern(CWave::PATTERN_TWO);
	
	//Scale
	_vec3 vScale = _vec3(1.f, 1.f, 1.f);
	m_pWave->SetScale(vScale);


	//Reflect==========================================
	//_vec3 vReflectPos = m_pTransCom->Get_Now_Position();

	_vec3 vReflectPos = _vec3(0.f, 0.f, 0.f);

	vReflectPos.y += 5.f;
	m_pReflect->GetTransCom()->Set_Position(&vReflectPos);
	m_pReflect->GetTransCom()->Set_Scale(3.5f, 3.5f, 3.5f);


	//Twist=========================================
	D3DXCOLOR m_Color = _vec4(0.4f, 0.f, 1.f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		m_pTwist[i]->GetParticleInfo()->m_dColor = m_Color;
		m_pTwist[i]->GetParticleInfo()->m_iPath = 2;
		m_pTwist[i]->GetParticleInfo()->m_iTextureNumber = 12;

		//Pos
		//_vec3 vWavePos = m_pTransCom->Get_Now_Position();
		_vec3 vWavePos = _vec3(0.0f, 0.0f, 0.0f);

		vWavePos.x = 0.0f;
		vWavePos.z += 1.5f;
		m_pTwist[i]->GetTransCom()->Set_Position(&vWavePos);

		//Rotate
		m_pTwist[i]->SetRotateResult(-1.2f);

		//Pattern
		m_pTwist[i]->SetPattern(CWave::PATTERN_THREE);

		//Scale
		_vec3 vScale = _vec3(1.2f + i * 0.6f, 1.2f + i * 0.6f, 1.2f + i * 0.6f);
		m_pTwist[i]->SetScale(vScale);

		//Scale 량
		m_pTwist[i]->SetDecrement(m_Color.a, -0.005f);

		//OriginScale
		//vScale = _vec3(1.8f, 1.8f, 1.8f);
		m_pTwist[i]->SetOriginScale(vScale);
	}

	//Particle=======================================
	//_vec3 vParticlePos = m_pTransCom->Get_Now_Position();
	_vec3 vParticlePos = _vec3(0.f, 0.f, 0.f);
	vParticlePos.x = 0.0f;
	vParticlePos.z += 2.0f;

	m_pParticle->SetPos(vParticlePos);
	m_pParticle->SetGoalResult(0.1f);



	//_vec3 vPortalParticlePos = m_pTransCom->Get_Now_Position();

	_vec3 vPortalParticlePos = _vec3(0.f, 0.f, 0.f);

	vPortalParticlePos.x = 0.0f;
	vPortalParticlePos.z += 2.0f;
	m_pPortalParticle->SetPos(vPortalParticlePos);
	D3DXCOLOR Portalcolor = _vec4(0.2f, 0.f, 1.f, 1.f);
	m_pPortalParticle->SetRenderGroupCheck(true);
	m_pPortalParticle->GetParticleInfo()->m_dColor = Portalcolor;
	m_pPortalParticle->SetGoalResult(2.0f);
}

void CPortal::SetPortalPosition(_vec3 * vPos)
{
	m_pTransCom->Set_Position(vPos);
}
