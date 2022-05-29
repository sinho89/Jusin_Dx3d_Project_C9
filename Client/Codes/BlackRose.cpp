#include "stdafx.h"
#include "BlackRose.h"
#include "Export_Engine.h"
#include "WitchBlade.h"
#include "Sprite.h"
#include "Wave.h"
#include "BlackRoseTrail.h"

CBlackRose::CBlackRose(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pLeftLeg(NULL)
, m_bLeftLegCheck(false)
, m_bRightLegCheck(false)
, m_pRightLeg(NULL)
, m_bLegTrailOff(false)
, m_bDustCheck(false)
, m_bRightSwordCheck(false)
, m_pRightSword(NULL)
, m_bEndTrail(false)
{
	m_bIsStatic = true;
	m_bAni = false;
	ZeroMemory(&m_pDust, sizeof(CSprite*) * 2);
}

CBlackRose::~CBlackRose(void)
{

}

void CBlackRose::Update_Object(const _float & fTimeDelta)
{
	if (m_pPlayerAction[CWitchBlade::ACTION_BLACKROSE] == true)
	{
		m_bAni = true;
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		//왼발 트레일
		if( (0.3f <= dTargetTime ) && (!m_bLeftLegCheck))
		{
			m_bLeftLegCheck = true;
			m_pLeftLeg->SetAni(true);
			m_pLeftLeg->SettingTrail();
		}
		//오른발 트레일
		if ((0.86f <= dTargetTime) && (!m_bRightLegCheck))
		{
			m_bRightLegCheck = true;
			m_pRightLeg->SetAni(true);
			m_pRightLeg->SettingTrail();

			m_pLeftLeg->SetAni(false);
		}
		//발에 트레일을 모두 끈다.
		if((1.313f <= dTargetTime) &&  (!m_bLegTrailOff))
		{
			m_bLegTrailOff = true;
			m_pRightLeg->SetAni(false);
			m_pLeftLeg->SetAni(false);
		}
		//2.22 오른손 트레일
		if ((2.22f <= dTargetTime) && (!m_bRightSwordCheck))
		{
			m_bRightSwordCheck = true;
			m_pRightSword->SetAni(true);
			m_pRightSword->SettingTrail();
		}
		//먼지 터트리기
		if((2.1f <= dTargetTime) && (!m_bDustCheck))
		{
			m_bDustCheck = true;
			SkillStart();
		}
		if((2.794332f <= dTargetTime) && (!m_bEndTrail))
		{
			m_bEndTrail = true;
			m_pRightSword->SetAni(false);
		}

	}
	else
	{
		m_bEndTrail = false;
		m_bRightSwordCheck = false;
		m_bRightLegCheck = false;
		m_bLeftLegCheck = false;
		m_bLegTrailOff = false;
		m_bDustCheck = false;
	}


	if (!m_bAni)
		return;

	EndAniCheck();

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Update_Object(fTimeDelta);
	}

	m_pLeftLeg->Update_Object(fTimeDelta);
	m_pRightLeg->Update_Object(fTimeDelta);
	m_pRightSword->Update_Object(fTimeDelta);
}

void CBlackRose::Render_Object(void)
{
	if (!m_bAni)
		return;

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i]->Render_Object();
	}

	m_pLeftLeg->Render_Object();
	m_pRightLeg->Render_Object();
	m_pRightSword->Render_Object();
}

HRESULT CBlackRose::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec4 Color = _vec4(0.5f, 0.5f, 0.5f, 2.0f);

	for (int i = 0; i < 2; ++i)
	{
		m_pDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"Dust");
		m_pDust[i]->SetAni(false);
		m_pDust[i]->SetRenderCheck(true);
		m_pDust[i]->GetParticleInfo()->m_dColor = (D3DXCOLOR)Color;
		m_pDust[i]->GetTransCom()->Set_Scale(1.5f, 1.5f, 1.5f);
		m_pDust[i]->GetParticleInfo()->m_iPath = 3;
		m_pDust[i]->SetEffectBlurCheck(TRUE);
		m_pDust[i]->SetBlurPass(3);
		m_pDust[i]->SetBlurCnt(2);
		m_pDust[i]->SetBlurPower(1.5f);
		m_pDust[i]->SetBlurValue(128.0f);
		m_pDust[i]->SetBlurType(Engine::BLUR_CROSS);

	}


	return S_OK;
}

CBlackRose * CBlackRose::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CBlackRose*		pInstance = new CBlackRose(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("BlackRose Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackRose::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pLeftLeg);
	Engine::Safe_Release(m_pRightLeg);
	Engine::Safe_Release(m_pRightSword);
	for (int i = 0; i < 2; ++i)
	{
		Engine::Safe_Release(m_pDust[i]);
	}
}

HRESULT CBlackRose::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CBlackRose::SkillStart(void)
{
	_vec3 vPosition, Rotate;

	vPosition = m_pTargetTransform->Get_Now_Position();

	for (int i = 0; i < 2; ++i)
	{
		vPosition.y += i * 0.5f + 0.3f;
		m_pDust[i]->SetAni(true);
		m_pDust[i]->GetTransCom()->Set_Position(&vPosition);
	}
}

void CBlackRose::EndAniCheck(void)
{
	if (m_bAni == true && m_pLeftLeg->GetAni() == false && m_pRightLeg->GetAni() == false && m_pDust[0]->GetAni() == false 
		&& m_pDust[1]->GetAni() == false && m_pRightSword->GetAni() == false)
		m_bAni = false;
}

HRESULT CBlackRose::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Player");

	if (pGameObject == NULL)
		return E_FAIL;

	m_pPlayerAction = ((CWitchBlade*)pGameObject)->Get_IsSkill(PLAYER_FIGHT);

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);


	//애니메이션 정보를 가져온다.
	m_pvecAniInfo = ((CWitchBlade*)pGameObject)->Get_AniInfo();

	//Player TransCom
	Engine::CTransform* pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	m_pTargetTransform = pTargetTransform;

	const _matrix*					m_pmatTarget;

	m_pmatTarget = m_pTargetTransform->Get_WorldMatrixPointer();


	char* LeftBoneUp = "_111";
	char* LeftBoneDown = "_110";

	char* RightBoneUp = "_105";
	char* RIghtBoneDown = "_104";

	const _matrix* m_pmatBoneUp = m_pTargetMesh->Get_FrameByName(LeftBoneUp);
	const _matrix* m_pmatBoneDown = m_pTargetMesh->Get_FrameByName(LeftBoneDown);

	const _matrix* m_pRightmatBoneUp	= m_pTargetMesh->Get_FrameByName(RightBoneUp);
	const _matrix* m_pRightmatBoneDown  = m_pTargetMesh->Get_FrameByName(RIghtBoneDown);


	//왼쪽 발
	m_pLeftLeg = CBlackRoseTrail::Create(m_pGraphicDev, L"BlackRoseTrailLeft", CBlackRoseTrail::DIR_LeftReg);
	m_pLeftLeg->SetAni(false);
	m_pLeftLeg->SetTargetMatrix(m_pmatTarget); //타겟의 월드행렬
	m_pLeftLeg->SetTargetBoneUpMatrix(m_pmatBoneUp);
	m_pLeftLeg->SetTargetBoneDownMatrix(m_pmatBoneDown);
	//==============================================================================

	//오른 발
	m_pRightLeg = CBlackRoseTrail::Create(m_pGraphicDev, L"BlackRoseTrailRight", CBlackRoseTrail::DIR_RightReg);
	m_pRightLeg->SetAni(false);
	m_pRightLeg->SetTargetMatrix(m_pmatTarget); //타겟의 월드행렬
	m_pRightLeg->SetTargetBoneUpMatrix(m_pRightmatBoneUp);
	m_pRightLeg->SetTargetBoneDownMatrix(m_pRightmatBoneDown);
	//===========================================================================

	//오른손 검
	Engine::CGameObject* pSwordObject = Engine::Find(L"SwordL1");
	if (pSwordObject == NULL)
		return E_FAIL;

	Engine::CTransform* pSwordTransform = (Engine::CTransform*)pSwordObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	const _matrix*					m_pmatSword = NULL;

	m_pmatSword = pSwordTransform->Get_WorldMatrixPointer();

	//오른손 검
	m_pRightSword = CBlackRoseTrail::Create(m_pGraphicDev, L"BlackRoseTrailRightSword", CBlackRoseTrail::DIR_RightSword);
	m_pRightSword->SetAni(false);
	m_pRightSword->SetTargetMatrix(m_pmatSword); //타겟의 월드행렬
	//======================================================

	Engine::Safe_Release(pSwordObject);
	Engine::Safe_Release(pGameObject);
	return S_OK;
}
