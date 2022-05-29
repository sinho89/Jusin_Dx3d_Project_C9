#include "stdafx.h"
#include "FireFrame.h"
#include "Export_Engine.h"
#include "Bardiel.h"
#include "Sprite.h"
#include "FrameProjector.h"

CFireFrame::CFireFrame(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
:CEffect(pGraphicDev, wstrName)
, m_pvecAniInfo(NULL)
, m_pTargetMesh(NULL)
, m_pTargetTransform(NULL)
, m_pTargetAnimator(NULL)
, m_pLeftHandMat(NULL)
, m_pFireProjecter(NULL)
, m_bFireStart(false)
, m_fAngle(90.0f)
, m_bLast(false)
, m_dDustTime(0)
{
	m_bAni = false;
	ZeroMemory(&m_pDust, sizeof(CSprite*) * 5);
}

CFireFrame::~CFireFrame(void)
{

}

void CFireFrame::Update_Object(const _float & fTimeDelta)
{
	if (g_bBardielDead)
		return;

	//FIREFRAME_A : ¿ÞÆÈ¿¡ ±â¸¦ ¸ðÀ¸´Â µ¿ÀÛ
	//FIREFRAME_B : ¿ÞÂÊºÎÅÍ ¿À¸¥ÂÊÀ¸·Î È­¿°¹æ»ç
	//FIREFRAME_C : ¿À¸¥ÂÊ¿¡¼­ ¿ÞÂÊÀ¸·Î È­¿°¹æ»ç
	//FIREFRAME_D : ¿ÞÂÊºÎÅÍ ¿À¸¥ÂÊÀ¸·Î È­¿°¹æ»ç
	//FIREFRAME_E : È­¿°¹æ»ç¸¦ ¸¶Ä¡°í ¿Þ¹ß·Î Âï±â

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::CHANGEPHASE2)
	{
		m_bAni = false;
	}

	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREFRAME_B)
	{
		if (!m_bFireStart)
		{
			m_fAngle = 90.0f;
			m_bAni = true;
			m_bFireStart = true;

			m_pFireProjecter->SetAni(true);
		}

		m_fAngle -= 2.f;

		LookAtAngle(m_fAngle);
	}
	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREFRAME_C)
	{
		m_fAngle += 2.f;
		LookAtAngle(m_fAngle);
	}
	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREFRAME_D)
	{
		m_fAngle -= 2.f;
		LookAtAngle(m_fAngle);
	}
	if (m_pTargetAnimator->Get_AnimationIndex() == CBardiel::BARDIEL::FIREFRAME_E)
	{
		_double dTargetTime = m_pTargetMesh->Get_TrackPos();

		m_fAngle += 2.f;
		LookAtAngle(m_fAngle);

		if (!m_bLast)
			m_bLast = true;


		if(0.1f <= dTargetTime)
		{
			m_dDustTime += fTimeDelta;

			if (m_dDustTime >= 0.1f)
			{
				m_dDustTime = 0;

				for (int i = 0; i < 5; ++i)
				{
					if(m_pDust[i]->GetAni())
						continue;

					SpriteDustStart(i);
					break;
				}

			}


		}

		if (2.4f <= dTargetTime)
		{
			_vec4 vColor = m_pFireProjecter->GetParticleInfo()->m_dColor;
			m_bLast = false;
			m_fAngle = 90.0f;
			m_bFireStart = false;
			m_bLast = false;
			vColor.w = 0.8f;
			m_pFireProjecter->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
			m_pFireProjecter->SetAni(false);
		}
	}

	if (!m_bAni)
		return;

	FireUpdate();

	SpriteDustUpdate();

	m_pFireProjecter->SetTargetDir(m_vFireDir);
	m_pFireProjecter->Update_Object(fTimeDelta);

	for (int i = 0; i < 5; ++i)
	{
		m_pDust[i]->Update_Object(fTimeDelta);
	}
}

void CFireFrame::Render_Object(void)
{
	if (!m_bAni)
		return;

	m_pFireProjecter->Render_Object();

	for (int i = 0; i < 5; ++i)
	{
		m_pDust[i]->Render_Object();
	}
}

HRESULT CFireFrame::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pFireProjecter = CFrameProjector::LoadCreate(m_pGraphicDev, L"FireFrameFire");
	m_pFireProjecter->SetAni(false);

	_vec4 vColor = m_pFireProjecter->GetParticleInfo()->m_dColor;
	vColor.w = 0.8f;
	m_pFireProjecter->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;

	for (int i = 0; i < 5; ++i)
	{
		m_pDust[i] = CSprite::LoadCreate(m_pGraphicDev, L"HanumanHQDust");
		m_pDust[i]->SetAni(false);
		m_pDust[i]->SetRenderCheck(true);
		m_pDust[i]->GetTransCom()->Set_Scale(1.f, 1.f, 1.f);

	}

	return S_OK;
}

CFireFrame * CFireFrame::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName)
{
	CFireFrame*		pInstance = new CFireFrame(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("FireFrame Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CFireFrame::Free(void)
{
	CEffect::Free();
	Engine::Safe_Release(m_pFireProjecter);

	for (int i = 0; i < 5; ++i)
	{
		Engine::Safe_Release(m_pDust[i]);
	}

}

HRESULT CFireFrame::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CFireFrame::SkillStart(void)
{
}

void CFireFrame::EndAniCheck(void)
{
}

HRESULT CFireFrame::Start_ForScene(void)
{
	if (m_bOnceStart)
		return S_OK;

	m_bOnceStart = true;

	Engine::CGameObject* pGameObject = Engine::Find(L"Mesh_Bardiel");
	if (pGameObject == NULL)
		return E_FAIL;

	m_pTargetAnimator = ((CBardiel*)pGameObject)->GetAnimatorInfo();

	//Target Mesh
	m_pTargetMesh = (Engine::CDynamicMesh*)pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC);

	//Bardiel TransCom
	m_pTargetTransform = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	//¿Þ¼Õ ¹Ù´Ú or 41(¿Þ¼Õ ÁßÁö ¸¶µð)
	m_pLeftHandMat = m_pTargetMesh->Get_FrameByName("_36");

	Engine::Safe_Release(pGameObject);

	return S_OK;
}

void CFireFrame::FireUpdate(void)
{
	if (!m_pFireProjecter->GetAni())
		return;


	_vec3 vLocalPos;
	memcpy(&vLocalPos, &m_pLeftHandMat->m[3][0], sizeof(_vec3));

	const _matrix*  matWorld = NULL;

	matWorld = m_pTargetTransform->Get_WorldMatrixPointer();

	D3DXVec3TransformCoord(&vLocalPos, &vLocalPos, matWorld);

	m_pFireProjecter->SetTargetPos(vLocalPos);

	if(m_bLast)
	{
		_vec4 vColor = m_pFireProjecter->GetParticleInfo()->m_dColor;


		if (vColor.w <= 0.f)
		{
			m_pFireProjecter->SetAni(false);
		}
		else
		{
			vColor.w -= 0.02f;
			m_pFireProjecter->GetParticleInfo()->m_dColor = (D3DXCOLOR)vColor;
		}
	}

}

void CFireFrame::LookAtAngle(_float fAngle)
{
	_float	fPhi;		// °¢µµ ¥õ

	fPhi = fAngle;

	fPhi = D3DXToRadian(fPhi);

	_vec3 vLocal;
	_matrix Trans;
	_matrix Result;
	_matrix BardielWorld;
	_vec3   vDir;
	_vec3	BardielPos;
	vLocal.x = 5.f * cosf(fPhi);
	vLocal.z = 5.f * sinf(fPhi);
	vLocal.y = -1.5f;

	D3DXMatrixTranslation(&Trans, vLocal.x, vLocal.y, vLocal.z);

	m_pTargetTransform->Get_WorldMatrix(&BardielWorld);

	Result = Trans * BardielWorld;

	_vec3 vWorldTargetPos;

	memcpy(&vWorldTargetPos, &Result.m[3][0], sizeof(_vec3));


	BardielPos = m_pTargetTransform->Get_Now_Position();

	vDir = vWorldTargetPos - BardielPos;

	D3DXVec3Normalize(&vDir, &vDir);

	m_vFireDir = vDir;
}

void CFireFrame::SpriteDustStart(int iIndex)
{
	m_pDust[iIndex]->SetAni(true);

	_vec3 vLocalPos;
	memcpy(&vLocalPos, &m_pLeftHandMat->m[3][0], sizeof(_vec3));

	const _matrix*  matWorld = NULL;

	matWorld = m_pTargetTransform->Get_WorldMatrixPointer();

	vLocalPos.y += 3.f;

	D3DXVec3TransformCoord(&vLocalPos, &vLocalPos, matWorld);

	m_pDust[iIndex]->GetTransCom()->Set_Position(&vLocalPos);

	m_pDust[iIndex]->GetTransCom()->Set_Scale(4.f, 4.f, 4.f);

}

void CFireFrame::SpriteDustUpdate(void)
{
	for (int i = 0; i < 5; ++i)
	{
		if (!(m_pDust[i]->GetAni()))
			continue;

		_vec3 vPos = m_pDust[i]->GetTransCom()->Get_Now_Position();

		vPos.y += 0.05f;

		m_pDust[i]->GetTransCom()->Set_Position(&vPos);

	}
}
