#include "CameraMgr.h"
#include "Camera.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCameraMgr)

Engine::CCameraMgr::CCameraMgr(void)
: m_pCurrentCamera(NULL)
{

}

Engine::CCameraMgr::~CCameraMgr(void)
{

}

void Engine::CCameraMgr::DefineCurrentCamera(WORD eCameraID)
{
	m_eCameraID = eCameraID;
	m_pCurrentCamera = m_mapCamera.find(m_eCameraID)->second;
	m_pCurrentCamera->Ready_Object();
}

CCamera* Engine::CCameraMgr::GetCurrentCamera(void)
{
	if (NULL == m_pCurrentCamera)
	{
		MSG_BOX("Not Define Current Camera!");
		return NULL;
	}

	return m_pCurrentCamera;
}

CCamera* Engine::CCameraMgr::GetCamera(WORD eCameraID)
{
	MAPCAMERA::iterator	iter = m_mapCamera.find(eCameraID);

	if (iter == m_mapCamera.end())
		return NULL;

	return iter->second;
}

WORD& Engine::CCameraMgr::GetCurrentCameraID(void)
{
	return m_eCameraID;
}

void CCameraMgr::Set_Shake(const WORD & eCameraID, const _vec3 & vShakeDir, const _float & fPower, const _int & iCnt, const _int& iDelay)
{
	MAPCAMERA::iterator	iter = m_mapCamera.find(eCameraID);

	if (iter == m_mapCamera.end())
		return;

	iter->second->Set_Shake(vShakeDir, fPower, iCnt, iDelay);
}

HRESULT Engine::CCameraMgr::ReadyCamera(CCamera* pCamera, WORD eCameraID)
{
	if(NULL == pCamera)
		return E_FAIL;

	m_mapCamera.insert(MAPCAMERA::value_type(eCameraID, pCamera));

	return S_OK;
}

void Engine::CCameraMgr::Free(void)
{
	for_each(m_mapCamera.begin(), m_mapCamera.end(), CRelease_Pair());
	m_mapCamera.clear();
}