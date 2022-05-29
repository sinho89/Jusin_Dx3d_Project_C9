#include "stdafx.h"
#include "RiddleKick.h"
#include "Export_Engine.h"
#include "Sprite.h"
#include "Wave.h"
#include "WitchBlade.h"
#include "ReflectMesh.h"


CRiddleKick::CRiddleKick(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: CEffect(pGraphicDev, wstrName)
, m_pDustWave(NULL) 
, m_pReflect(NULL)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_bSkillCheck(false)
{
	ZeroMemory(&m_pDust, sizeof(CSprite*) * 2);

	m_bIsStatic = true;
	m_bAni = false;
}

CRiddleKick::~CRiddleKick(void)
{

}

void CRiddleKick::Update_Object(const _float& fTimeDelta)
{
	if (m_pPlayerAction[CWitchBlade::ACTION_RIDDLEKICK] == true)
	{
		_double dTime = (*m_pvecAniInfo)[CWitchBlade::WITCH_GROUNDKICK]->pMotionEventInfo[EVENT_EFFECT].dStartTime;

		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		if (dTargetTime >= dTime && (!m_bSkillCheck))
		{
			SkillStart();
			m_bSkillCheck = true;
		}
	}
	else
	{
		m_bSkillCheck = false;
	}

	if (!m_bAni)
		return;

	EndAniCheck();


	WaveUpdate();


	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Update_Object(fTimeDelta);
	}

	m_pDustWave->Update_Object(fTimeDelta);

	m_pReflect->Update_Object(fTimeDelta);
}

void CRiddleKick::Render_Object(void)
{
	if(!m_bAni)
		return ;

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Render_Object();
	}

	m_pDustWave->Render_Object();
	m_pReflect->Render_Object();
}

HRESULT CRiddleKick::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pDustWave = CWave::Create(m_pGraphicDev, L"DustWave");
	m_pDustWave->SetAni(false);
	m_pDustWave->SetPattern(CWave::PATTERN_END);
	m_pDustWave->SetBilBoardCheck(false);
	m_pDustWave->SetRendererCheck(true);
	m_pDustWave->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
	_vec4 Color = _vec4(1.0f, 1.0f, 1.0f, 0.8f);
	m_pDustWave->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
	m_pDustWave->GetParticleInfo()->m_iPath = 2;
	m_pDustWave->GetParticleInfo()->m_iTextureNumber = 7;
	m_pDustWave->SetZRotateCheck(false);




	m_pReflect = CReflectMesh::Create(m_pGraphicDev, L"Reflect");
	
	Color = _vec4(0.5f, 0.5f, 0.5f, 2.0f);

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"Dust");
		m_pDust[i]->SetAni(false);
		m_pDust[i]->SetRenderCheck(true);
		m_pDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pDust[i]->GetTransCom()->Set_Scale(2.f, 2.f, 2.f);
		m_pDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pDust[i]->SetEffectBlurCheck(TRUE);
		m_pDust[i]->SetBlurPass(3);
		m_pDust[i]->SetBlurCnt(2);
		m_pDust[i]->SetBlurPower(1.5f);
		m_pDust[i]->SetBlurValue(128.0f);
		m_pDust[i]->SetBlurType(Engine::BLUR_CROSS);

	}

	m_pDustWave->SetAni(false);
	m_pReflect->SetAni(false);

	//Reflect========================================
	m_pReflect->SetPattern(CReflectMesh::PATTERN_ONE);
	m_pReflect->GetTransCom()->Set_Scale(0.2f, 0.05f, 0.2f);
	

	return S_OK;
}

CRiddleKick* CRiddleKick::Create(LPDIRECT3DDEVICE9 pGraphicDev , const wstring& wstrName)
{

	CRiddleKick*		pInstance = new CRiddleKick(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("RiddleKick Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CRiddleKick::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pDustWave);
	Engine::Safe_Release(m_pReflect);

	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pDust[i]);
	}

}

HRESULT CRiddleKick::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CRiddleKick::SkillStart(void)
{
	if(m_bAni)
		return ;

	Engine::CGameObject*	pGameObject = NULL;


	pGameObject = Engine::FindWithTag(L"Player");


	Engine::CTransform* pTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);


	if(pGameObject == NULL)
		return ;

	_vec3 Position, Rotate;

	Position = pTransform->Get_Now_Position();
	Position.y += 0.1f;


	Rotate = _vec3(90.0f, 0.0f, 0.0f);

	m_pDustWave->SetAni(true);

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->SetAni(true);
	}


	D3DXCOLOR color = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	//DustWave
	m_pDustWave->GetParticleInfo()->m_dColor = color;
	m_pDustWave->GetParticleInfo()->m_iPath = 2;
	m_pDustWave->GetParticleInfo()->m_iTextureNumber = 7;
	m_pDustWave->SetDecrement(0.05f, 0.05f); //알파값 , 스케일값 감소량 설정
	m_pDustWave->GetTransCom()->Set_Position(&Position);
	m_pDustWave->GetTransCom()->Set_Angle(&Rotate);

	for (int i = 0; i < 2; ++i)
	{
		_vec3 Pos = Position;

		Pos.y += i * 0.5f + 0.3f;

		m_pDust[i]->GetTransCom()->Set_Position(&Pos);
	}


	//Reflect
	m_pReflect->GetTransCom()->Set_Position(&pTransform->Get_Now_Position());
	m_pReflect->SetAni(true);
	m_pReflect->GetTransCom()->Set_Scale(0.2f, 0.05f, 0.2f);

	Engine::Safe_Release(pTransform);
	Engine::Safe_Release(pGameObject);


	m_bAni = true;

}

void CRiddleKick::EndAniCheck(void)
{
	if (m_bAni == true && m_pDustWave->GetAni() == false && m_pDust[0]->GetAni() == false && 
		m_pDust[1]->GetAni() == false  && m_pReflect->GetAni() == false)
		m_bAni = false;
}

HRESULT CRiddleKick::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return S_OK;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();


	Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CRiddleKick::WaveUpdate(void)
{
	if (m_pDustWave->GetAni() == true)
	{

		if (m_pDustWave->GetParticleInfo()->m_dColor.a < 0.0f)
		{
			m_pDustWave->GetParticleInfo()->m_dColor.a = 0.8f;
			
			m_pDustWave->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
			m_pDustWave->SetAni(false);
		}
		else
		{
			m_pDustWave->GetParticleInfo()->m_dColor.a -= 0.05f;

			m_pDustWave->GetTransCom()->Add_Scale(0.05f);
		}
	}
}
