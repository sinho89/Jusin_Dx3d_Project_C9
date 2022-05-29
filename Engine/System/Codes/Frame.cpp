#include "Frame.h"

USING(Engine)

Engine::CFrame::CFrame(void)
: m_fCallLimit(0.f)
, m_fAccTimeDelta(0.f)
{

}

Engine::CFrame::~CFrame(void)
{

}

Engine::_bool Engine::CFrame::isPermit_Call(const _float& fTimeDelta)
{
	m_fAccTimeDelta += fTimeDelta;

	if(m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fAccTimeDelta = 0.f;
		return true;
	}

	return false;
}

HRESULT Engine::CFrame::Ready_Frame(const _float& fCallLimit)
{
	m_fCallLimit = 1.f / fCallLimit;

	return S_OK;
}

CFrame* Engine::CFrame::Create(const _float& fCallLimit)
{
	CFrame*			pInstance = new CFrame;

	if(FAILED(pInstance->Ready_Frame(fCallLimit)))
	{
		MSG_BOX("CFrame Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CFrame::Free(void)
{

}
