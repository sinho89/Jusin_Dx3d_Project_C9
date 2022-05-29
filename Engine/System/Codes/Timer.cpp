#include "Timer.h"

USING(Engine)

Engine::CTimer::CTimer(void)
: m_fTimeDelta(0.f)
{

}

Engine::CTimer::~CTimer(void)
{

}

Engine::_float Engine::CTimer::Get_TimeDelta(void) const
{
	return m_fTimeDelta;
}

HRESULT Engine::CTimer::Ready_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void Engine::CTimer::SetUp_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_LastTime.QuadPart) / _float(m_CpuTick.QuadPart);

	m_LastTime = m_FrameTime;
}

CTimer* Engine::CTimer::Create(void)
{
	CTimer* pInstance = new CTimer;

	if(FAILED(pInstance->Ready_Timer()))
	{
		MSG_BOX("CTimer Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CTimer::Free(void)
{

}

