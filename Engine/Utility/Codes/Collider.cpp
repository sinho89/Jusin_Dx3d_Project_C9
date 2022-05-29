#include "Collider.h"
#include "VIBuffer.h"

USING(Engine)

Engine::CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pBuffer(NULL)
, m_pParentTransform(NULL)
, m_vPos(0.f, 0.f, 0.f)
, m_vScale(1.f, 1.f, 1.f)
, m_eType(COL_END)
, m_wstrTag(L"Collision")
, m_iIndex(0)
, m_bIsRender(false)
{
	D3DXMatrixIdentity(&m_matWorld);
	ZeroMemory(&m_tColInfo, sizeof(COLINFO));
}

Engine::CCollider::~CCollider(void)
{

}

void Engine::CCollider::SetScale(const _vec3& vScale)
{
	m_vScale = vScale;
}

void Engine::CCollider::SetScale(const _float& fRad)
{
	m_vScale.x = fRad;
	m_vScale.y = fRad;
	m_vScale.z = fRad;
}

void Engine::CCollider::SetPos(const _vec3& vPos)
{
	m_vPos = vPos;
}

void CCollider::Change_BoxColor(const _ulong & dwColor)
{
	m_pBuffer->Change_BoxColor(dwColor);
}

void CCollider::Set_ColInfo(const COLINFO * pColInfo)
{
	m_tColInfo = *pColInfo;
}

void CCollider::Clear_Map(void)
{
	m_mapColCheck.clear();
}

void CCollider::Add_IsCol(const _uint & iIndex)
{
	m_mapColCheck.insert(map<_uint, _bool>::value_type(iIndex, true));
}

void CCollider::Set_Render(_bool bRender)
{
	m_bIsRender = bRender;
}

void CCollider::GetScale(_vec3 * pScale) const
{
	*pScale = m_vScale;
}

void CCollider::GetScale(_float * pRad) const
{
	*pRad = m_vScale.x;
}

void CCollider::GetPos(_vec3 * pPos) const
{
	_vec3 vPos;
	memcpy(&vPos, &m_matWorld.m[3][0], sizeof(_vec3));
	*pPos = vPos;
}

void CCollider::GetWorldMatrix(_matrix * pMatrix) const
{
	*pMatrix = m_matWorld;
}

_uint CCollider::Get_Index(void)
{
	return m_iIndex;
}

const COLINFO * CCollider::Get_ColInfo(void) const
{
	return &m_tColInfo;
}

_bool CCollider::Get_IsCol(const _uint & iIndex)
{
	map<_uint, _bool>::iterator iter = m_mapColCheck.find(iIndex);

	if (iter == m_mapColCheck.end())
		return true;

	return false;
}

CTransform * CCollider::Get_ParentTrans(void)
{
	return m_pParentTransform;
}

_bool CCollider::IsHit(void)
{
	if (m_mapColCheck.empty())
		return false;
	return true;
}

void Engine::CCollider::Free(void)
{

}

