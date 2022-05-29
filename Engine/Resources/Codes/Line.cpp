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
	//vec2Ÿ�� ����Ʈ ��ŸƮ�� ����Ʈ ���带 ���..
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	//vec2 ���� = ���� - ��ŸƮ
	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	//vec2 ���⿡ ���� ��ֺ��� ����...
	m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	//��ֺ��� ����ȭ.
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	m_vCenterPoint = (m_vPoint[POINT_START] + m_vPoint[POINT_FINISH])/2.f;

	return S_OK;
}

CLine::COMPARE Engine::CLine::Compare(const _vec2* pEndPos)
{
	//vec2 vDest = ���� ������ ����Ʈ ��ŸƮ����..
	_vec2	vDest = *pEndPos - m_vPoint[POINT_START];
	//D3DXVec2Normalize(&vDest, &vDest);
	//vDest�� ����ȭ�� ���Ϳ� ��ֺ��Ͱ� ����..
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