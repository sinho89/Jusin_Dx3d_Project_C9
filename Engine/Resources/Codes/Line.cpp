#include "Line.h"

USING(Engine)

Engine::CLine::CLine(void)
{

}

Engine::CLine::~CLine(void)
{

}

HRESULT Engine::CLine::Ready_Line(const _vec2* pPointA, const _vec2* pPointB)
{
	//vec2타입 포인트 스타트와 포인트 엔드를 찍고..
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	//vec2 방향 = 엔드 - 스타트
	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	//vec2 방향에 대한 노멀벡터 구함...
	m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	//노멀벡터 정규화.
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	m_vCenterPoint = (m_vPoint[POINT_START] + m_vPoint[POINT_FINISH])/2.f;

	return S_OK;
}

CLine::COMPARE Engine::CLine::Compare(const _vec2* pEndPos)
{
	//vec2 vDest = 찍은 점에서 포인트 스타트를뺌..
	_vec2	vDest = *pEndPos - m_vPoint[POINT_START];
	//D3DXVec2Normalize(&vDest, &vDest);
	//vDest의 정규화한 벡터와 노멀벡터간 내적..
	_float fResult = D3DXVec2Dot(&vDest, &m_vNormal);

	if(fResult > 0)	
		return COMPARE_LEFT;
	else 
		return COMPARE_RIGHT;
}

CLine* Engine::CLine::Create(const _vec2* pPointA, const _vec2* pPointB)
{
	CLine*	pInstance = new CLine();

	if(FAILED(pInstance->Ready_Line(pPointA, pPointB)))
	{
		MSG_BOX("CLine Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CLine::Free(void)
{

}