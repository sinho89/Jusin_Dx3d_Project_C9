#include "stdafx.h"
#include "Culling_Manager.h"
#include "Export_Engine.h"
#include "Camera.h"
#include "Light.h"


IMPLEMENT_SINGLETON(CCulling_Manager)

CCulling_Manager::CCulling_Manager(void)
: m_hThread(NULL)
, m_pmapObject(NULL)
, m_pmapPointLight(NULL)
, m_pmapSpotLight(NULL)
, m_pMainCamera(NULL)
, m_pPlayer(NULL)
, m_pPlayerTransform(NULL)
, m_iLoop(1)
, m_fSphereRad(0.f)
{
	ZeroMemory(&m_Crt, sizeof(CRITICAL_SECTION));
}

CCulling_Manager::~CCulling_Manager(void)
{
}

CRITICAL_SECTION * CCulling_Manager::Get_CriticalSection(void)
{
	return &m_Crt;
}

_uint CCulling_Manager::Get_IsLoop(void)
{
	return m_iLoop;
}

void CCulling_Manager::StopThread(void)
{
	m_iLoop = 0;
	WaitForSingleObject(m_hThread, INFINITE);
}

HRESULT CCulling_Manager::Ready_Culling(map<wstring, list<Engine::CGameObject*>>* pmapObject, const _float& fSphereRad)
{
	if (pmapObject == NULL)
		return E_FAIL;
	m_pmapObject = pmapObject;

	m_pmapPointLight = Engine::Get_PointLightMap();
	if (m_pmapPointLight == NULL)
		return E_FAIL;
	m_pmapSpotLight = Engine::Get_SpotLightMap();
	if (m_pmapSpotLight == NULL)
		return E_FAIL;

	m_pMainCamera = Engine::GetCamera(Engine::CAMERA_PLAYER);
	if (m_pMainCamera == NULL)
		return E_FAIL;

	m_fSphereRad = fSphereRad;

	m_pPlayer = Engine::FindWithTag(L"Player");
	if (m_pPlayer == NULL)
		return E_FAIL;

	m_pPlayerTransform = (Engine::CTransform*)m_pPlayer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);

	Engine::Safe_Release(m_pPlayerTransform);
	Engine::Safe_Release(m_pPlayer);


	D3DXMatrixIdentity(&m_matProj);
	m_matProj = m_pMainCamera->GetMatProj();
	D3DXMatrixInverse(&m_matProj, NULL, &m_matProj);


	/*WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	m_iLoop = 1;
	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);*/
	return S_OK;
}

unsigned int CCulling_Manager::Thread_Main(void * pArg)
{
	CCulling_Manager* pCulling = (CCulling_Manager*)pArg;

	_uint		iFlag = 0;

	while (pCulling->Get_IsLoop())
	{
		EnterCriticalSection(pCulling->Get_CriticalSection());

		iFlag = pCulling->Start_Culling();

		LeaveCriticalSection(pCulling->Get_CriticalSection());
	}

	return iFlag;
}

_uint CCulling_Manager::Start_Culling(void)
{
	SetUp_Frustum();
	Object_Culling();
	PointLight_Culling();
	return 0;
}

void CCulling_Manager::Object_Culling(void)
{
	map<wstring, list<Engine::CGameObject*>>::iterator iter = m_pmapObject->begin();
	map<wstring, list<Engine::CGameObject*>>::iterator iter_end = m_pmapObject->end();

	map<wstring, list<Engine::CGameObject*>>::iterator iter_weapon = m_pmapObject->find(L"Weapon");

	map<wstring, list<Engine::CGameObject*>>::iterator iter_Effect = m_pmapObject->find(L"SoftEffect");


	for (iter; iter != iter_end; iter++)
	{
	if (iter == iter_weapon || iter == iter_Effect)
			continue;

		list<Engine::CGameObject*>::iterator iter_list = iter->second.begin();
		list<Engine::CGameObject*>::iterator iter_list_end = iter->second.end();

		for (iter_list; iter_list != iter_list_end; iter_list++)
		{	
			if (Engine::Get_Delete())
				continue;

			Engine::CTransform* pTransform = (Engine::CTransform*)((*iter_list)->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC));
			if (pTransform == NULL)
				continue;
			_vec3 vPos;
			pTransform->Get_Infomation(Engine::INFO_POSITION, &vPos);

			if ((IsIn_Frustum(&vPos, 5.f)) || (IsIn_Sphere(&vPos)))
				(*iter_list)->SetActive(true);
			else
				(*iter_list)->SetActive(false);
				

			Engine::Safe_Release(pTransform);
		}
	}
}

void CCulling_Manager::PointLight_Culling(void)
{
	map<const _uint, Engine::CLight*>::iterator iter = m_pmapPointLight->begin();
	map<const _uint, Engine::CLight*>::iterator iter_end = m_pmapPointLight->end();

	for (iter; iter != iter_end; iter++)
	{
		_vec3 vPos = iter->second->Get_LightInfo()->Position;
		if ((IsIn_Frustum(&vPos, iter->second->Get_LightInfo()->Range)) || (IsIn_Sphere(&vPos)))
			iter->second->SetLightRender(true);
		else
			iter->second->SetLightRender(false);
	}
}

void CCulling_Manager::SetUp_Frustum(void)
{
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	D3DXMatrixIdentity(&m_matView);
	m_matView = m_pMainCamera->GetMatView();
	D3DXMatrixInverse(&m_matView, NULL, &m_matView);

	for (_uint i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &m_matProj);

	for (_uint i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &m_matView);

	Make_Plane();
}

void CCulling_Manager::Make_Plane(void)
{
	// Make Plane
	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);
}

_bool CCulling_Manager::IsIn_Frustum(const _vec3 * pPosition, const _float & fRadius)
{
	//true 절두체 안에 있다 false 밖에잇다

	for (_int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_Plane[i], pPosition) > fRadius)
			return false;
	}

	return true;
}

_bool CCulling_Manager::IsIn_Sphere(const _vec3 * pPosition)
{
	//true 반경 안에있다 false 반경 밖에잇다

	_vec3 vCenterPos = Engine::GetCamera(Engine::CAMERA_PLAYER)->GetEyePosition();;
	
	_vec3 vDist = vCenterPos - *pPosition;
	_float fDist = D3DXVec3Length(&vDist);

	if (fDist > m_fSphereRad)
		return false;

	return true;
}

void CCulling_Manager::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);
}
