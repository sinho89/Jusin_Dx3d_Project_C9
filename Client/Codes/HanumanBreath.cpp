#include "stdafx.h"
#include "HanumanBreath.h"
#include "Hanuman.h"
#include "Wave.h"
#include "BreathBullet.h"
#include "Sound.h"

CHanumanBreath::CHanumanBreath(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pTargetBoneMatrix(NULL)
, m_pBreathBullet(NULL)
, m_pSoundCom(NULL)
, m_dShootingTimer(0)
, m_iShootingCount(0)
{
	m_bAni = false;
}

CHanumanBreath::~CHanumanBreath(void)
{
}

void CHanumanBreath::Update_Object(const _float & fTimeDelta)
{
	if (g_bHanumanDead)
		return;

	//Breath0 -> 브레스 준비동작
	//Breath1 -> 고개를 돌리면서 브레스 발사

	//Breath0
	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::BREATH_0)
	{
		m_bAni = true;

		_double dTargetTime = m_pTargetMesh->Get_TrackPos();
	}

	//Breath1
	if (m_pTargetAnimator->Get_AnimationIndex() == CHanuman::HANUMAN::BREATH_1)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		m_dShootingTimer += fTimeDelta;

		if(dTargetTime  >= 1.0f) // Bullet을 발사시킬 타임
		{
			if (m_dShootingTimer < 0.1) //Bullet 발사
				return;

			if (m_iShootingCount > 10)
				return;
			
			m_pSoundCom->MyPlaySound(L"Missile", false, true);
			m_dShootingTimer = 0;

			m_fAngle = 200.f - m_iShootingCount * 20.f;

			LookAtAngle(m_fAngle);


			_matrix HanumanWorld;


			m_pTargetTransform->Get_WorldMatrix(&HanumanWorld);


			//하누만의 입 위치에 이펙트를 이동 시킨다.`
			_vec3 HanumanFacePos; //하누만 입 위치의 월드 Pos

			memcpy(&HanumanFacePos, &m_pTargetBoneMatrix->m[3][0], sizeof(_vec3));


			D3DXVec3TransformCoord(&HanumanFacePos, &HanumanFacePos, &HanumanWorld);


			//Bullet 생성=================================================================

			_vec3 vBulletLook = _vec3(0.f, 0.f, 1.f);

			_float Dot = D3DXVec3Dot(&vBulletLook, &vHanumanDir);

			_float Radian = (_float)acos(Dot);
			Radian = D3DXToDegree(Radian);

			_vec3 vBulletDir;

			D3DXVec3Cross(&vBulletDir, &vBulletLook, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

			if (D3DXVec3Dot(&vBulletDir, &vHanumanDir) > 0)
			{
				Radian *= -1.f;
			}

			_vec3 Angle = _vec3(0.f, Radian, 0.0f);

			m_pBreathBullet = CBreathBullet::Create(m_pGraphicDev, L"BreathBullet");

			HanumanFacePos += vHanumanDir * 10.f;

			m_pBreathBullet->GetTransCom()->Set_Position(&HanumanFacePos);
			m_pBreathBullet->SetShootingDir(vHanumanDir);

			_vec3 vRotate = _vec3(0.f, m_fAngle, 0.f);
			m_pBreathBullet->SetRotate(Angle);

			Engine::AddObject(Engine::LAYER_GAMELOGIC, L"SoftEffect", m_pBreathBullet);
			//============================================================================

			m_iShootingCount++;
		}
	}
	else
	{
		m_iShootingCount = 0;
	}
}

void CHanumanBreath::Render_Object(void)
{
	if (g_bHanumanDead)
		return;

	if (!m_bAni)
		return;


}


void CHanumanBreath::LookAtAngle(_float fAngle)
{

	_float	fPhi;		// 각도 φ

	fPhi = fAngle;

	fPhi = D3DXToRadian(fPhi);

	_vec3 vLocal;
	_matrix Trans;
	_matrix Result;
	_matrix HanumanWorld;
	_vec3   vDir;
	_vec3	HanumanPos;
	vLocal.x = 5.f * cosf(fPhi);
	vLocal.z = 5.f * sinf(fPhi);
	vLocal.y = 0.f;

	D3DXMatrixTranslation(&Trans, vLocal.x, vLocal.y, vLocal.z);

	m_pTargetTransform->Get_WorldMatrix(&HanumanWorld);

	Result = Trans * HanumanWorld;

	_vec3 vWorldTargetPos;

	memcpy(&vWorldTargetPos, &Result.m[3][0], sizeof(_vec3));


	HanumanPos = m_pTargetTransform->Get_Now_Position();

	vDir = vWorldTargetPos - HanumanPos;

	D3DXVec3Normalize(&vDir, &vDir);

	vHanumanDir = vDir;
}

HRESULT CHanumanBreath::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	return S_OK;
}

CHanumanBreath * CHanumanBreath::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CHanumanBreath*		pInstance = new CHanumanBreath(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("HanumanBreath Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CHanumanBreath::Free(void)
{
	CEffect::Free();
}

HRESULT CHanumanBreath::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Sound
	pComponent = CSound::Create(m_pTransCom);
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Sound", pComponent));

	return S_OK;
}

void CHanumanBreath::SkillStart(void)
{

}

void CHanumanBreath::EndAniCheck(void)
{

}

HRESULT CHanumanBreath::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_pSoundCom->Set_Sound(L"Missile", L"Hanuman_Missile.wav");

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::Find(L"Mesh_Hanuman");
	if (pGameObject == NULL)
		return E_FAIL;

	m_pTargetAnimator = ((CHanuman*)pGameObject)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//Hanuman TransCom
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//Hanuman Face========================================================
	const _matrix* pTargetBone = m_pTargetMesh->Get_FrameByName("_13");

	m_pTargetBoneMatrix = pTargetBone;

	Engine::Safe_Release(pGameObject);
	return S_OK;
}
