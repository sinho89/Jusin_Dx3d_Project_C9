#include "Collision_Manager.h"

#include "BoxCollider.h"
#include "SphereCollider.h"

#include "Transform.h"

USING(Engine)
IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager(void)
{
	m_iColliderIndex = 0;
}

CCollision_Manager::~CCollision_Manager(void)
{
}

_bool CCollision_Manager::IsCol_ForOBB(const _tchar * pTargetTag, const CCollider * pMyCollider, const _vec3* pNextPos)
{
	if (pMyCollider == NULL)
		return false;

	MAPCOLLIDER::iterator iter = m_mapCollider[COL_BOX].find(pTargetTag);

	if (iter == m_mapCollider[COL_BOX].end())
		return false;

	list<CCollider*>::iterator iter_list = iter->second.begin();
	list<CCollider*>::iterator iter_list_end = iter->second.end();

	for (iter_list; iter_list != iter_list_end; iter_list++)
	{
		_vec3 vDestPos;
		pMyCollider->GetPos(&vDestPos);
		_vec3 vSourPos;
		(*iter_list)->GetPos(&vSourPos);
		_float fDest = D3DXVec3Length(&(vDestPos - vSourPos));
		//거리가 멀면 넘어가 검사 하지말고
		if (fDest > 30.f)
			continue;

		_vec3 vDestMin = _vec3(-0.5f, -0.5f, -0.5f);
		_vec3 vDestMax = _vec3(0.5f, 0.5f, 0.5f);
		_matrix matDest;
		pMyCollider->GetWorldMatrix(&matDest);

		_vec3 vSourMin = _vec3(-0.5f, -0.5f, -0.5f);
		_vec3 vSourMax = _vec3(0.5f, 0.5f, 0.5f);
		_matrix matSour;
		(*iter_list)->GetWorldMatrix(&matSour);

		if (Collision_OBB(&vDestMin, &vDestMax, &matDest, &vSourMin, &vSourMax, &matSour))
		{
			matDest._41 += pNextPos->x * 2.f;
			matDest._42 += pNextPos->y * 2.f;
			matDest._43 += pNextPos->z * 2.f;

			if (Collision_OBB(&vDestMin, &vDestMax, &matDest, &vSourMin, &vSourMax, &matSour))
			{
				const_cast<CCollider*>(pMyCollider)->Change_BoxColor(D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
				(*iter_list)->Change_BoxColor(D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
				return true;
			}
			else
			{
				const_cast<CCollider*>(pMyCollider)->Change_BoxColor(D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
				(*iter_list)->Change_BoxColor(D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
			}
			
		}
		else
		{
			const_cast<CCollider*>(pMyCollider)->Change_BoxColor(D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
			(*iter_list)->Change_BoxColor(D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
		}
	}

	return false;
}

_bool CCollision_Manager::IsCollision(const _tchar * pTargetTag, COLINFO * pColInfo, const CCollider* pMyCollider)
{
	if (pMyCollider == NULL)
		return false;

	if (pColInfo == NULL)
		return false;

	MAPCOLLIDER::iterator iter = m_mapCollider[COL_SPHERE].find(pTargetTag);
	if (iter == m_mapCollider[COL_SPHERE].end())
		return false;

	list<CCollider*>::iterator iter_list = iter->second.begin();
	list<CCollider*>::iterator iter_list_end = iter->second.end();

	for (iter_list; iter_list != iter_list_end; iter_list++)
	{
		_vec3 vDestPos;
		_vec3 vSourPos;
		_vec3 vScale;
		pMyCollider->GetPos(&vDestPos);
		pMyCollider->GetScale(&vScale);
		(*iter_list)->GetPos(&vSourPos);
		_vec3 vDest = vDestPos - vSourPos;
		_float fDist = D3DXVec3Length(&vDest);
		_float fRad;
		(*iter_list)->GetScale(&fRad);
		fRad += vScale.x;
		if (fDist > fRad)
			continue;
		_uint iIndex = const_cast<CCollider*>(pMyCollider)->Get_Index();

		if (!(*iter_list)->Get_IsCol(iIndex))
			continue;

		//충돌 했어
		const COLINFO* pSourColInfo = (*iter_list)->Get_ColInfo();

		vDest.y = 0.f;

		pColInfo->eColInfo = pSourColInfo->eColInfo;
		pColInfo->eColDir = pSourColInfo->eColDir;
		pColInfo->vDest = vDest;
		pColInfo->vDir = pSourColInfo->vDir;
		pColInfo->fForce = pSourColInfo->fForce;
		pColInfo->fJumpForce = pSourColInfo->fJumpForce;
		pColInfo->fStampForce = pSourColInfo->fStampForce;
		pColInfo->dwSkillID = pSourColInfo->dwSkillID;
		pColInfo->iDamage = pSourColInfo->iDamage;
		pColInfo->bCritical = pSourColInfo->bCritical;
		
		(*iter_list)->Add_IsCol(iIndex);
		return true;
	}

	return false;
}

_bool CCollision_Manager::IsCapture(const _tchar * pTargetTag, CCollider* pCollider)
{
	if (pCollider == NULL)
		return false;

	MAPCOLLIDER::iterator iter = m_mapCollider[COL_BOX].find(pTargetTag);
	if (iter == m_mapCollider[COL_BOX].end())
		return false;


	list<CCollider*>::iterator iter_list = iter->second.begin();
	list<CCollider*>::iterator iter_list_end = iter->second.end();

	for (iter_list; iter_list != iter_list_end; iter_list++)
	{
		_vec3 vDestPos;
		_vec3 vSourPos;
		pCollider->GetPos(&vDestPos);
		(*iter_list)->GetPos(&vSourPos);

		_float fDist = D3DXVec3Length(&(vDestPos - vSourPos));

		if (fDist < 4.f)
			return true;
	}

	return false;
}

void CCollision_Manager::Add_ColliderGroup(COL_TYPE eType, const _tchar* pTag, CCollider * pCollider)
{
	if (pCollider == NULL)
		return;

	MAPCOLLIDER::iterator iter = m_mapCollider[eType].find(pTag);

	if (iter == m_mapCollider[eType].end())
	{
		m_mapCollider[eType].insert(MAPCOLLIDER::value_type(pTag, list<CCollider*>()));
		iter = m_mapCollider[eType].find(pTag);
	}

	list<CCollider*>::iterator iter_list = iter->second.begin();
	list<CCollider*>::iterator iter_list_end = iter->second.end();

	for (iter_list; iter_list != iter_list_end; iter_list++)
	{
		if ((*iter_list) == pCollider)
		{
			pCollider->Clear_Map();
			pCollider->Set_Render(true);
			return;
		}
	}

	pCollider->Clear_Map();
	pCollider->Set_Render(true);
	iter->second.push_back(pCollider);
}

void CCollision_Manager::Clear_ColliderGroup(void)
{
	MAPCOLLIDER::iterator iter = m_mapCollider[COL_BOX].begin();
	MAPCOLLIDER::iterator iter_end = m_mapCollider[COL_BOX].end();

	for (iter; iter != iter_end; iter++)
		iter->second.clear();

	m_mapCollider[COL_BOX].clear();
}

void CCollision_Manager::Clear_All(void)
{
	for (_uint i = 0; i < COL_END; i++)
	{
		MAPCOLLIDER::iterator iter = m_mapCollider[i].begin();
		MAPCOLLIDER::iterator iter_end = m_mapCollider[i].end();

		for (iter; iter != iter_end; iter++)
			iter->second.clear();

		m_mapCollider[i].clear();
	}
}
void CCollision_Manager::Delete_Collider(const _tchar* pTag, const CCollider * pMyCollider)
{
	if (pMyCollider == NULL)
		return;

	MAPCOLLIDER::iterator iter = m_mapCollider[COL_SPHERE].find(pTag);

	if (iter == m_mapCollider[COL_SPHERE].end())
		return;

	list<CCollider*>::iterator iter_list = iter->second.begin();
	list<CCollider*>::iterator iter_list_end = iter->second.end();

	for (iter_list; iter_list != iter_list_end;)
	{
		if ((*iter_list) == pMyCollider)
		{
			(*iter_list)->Set_Render(false);
			(*iter_list)->Clear_Map();
			iter_list = iter->second.erase(iter_list);
		}
		else
			iter_list++;
	}
	
}

CBoxCollider* CCollision_Manager::Create_BoxCollider(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, const wstring& wstrTag,
													const _vec3& vPos, const _vec3& vScale, const _ulong& dwColor)
{
	return CBoxCollider::Create(pGraphicDev, pParentTransform, wstrTag, vPos, vScale, dwColor, m_iColliderIndex++);
}
CSphereCollider* CCollision_Manager::Create_SphereCollider(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform,
									const wstring& wstrTag, const _vec3& vPos, const _float& fRad)
{
	return CSphereCollider::Create(pGraphicDev, pParentTransform, wstrTag, vPos, fRad, m_iColliderIndex++);
}

void CCollision_Manager::Free(void)
{
	for (_uint i = 0; i < COL_END; i++)
	{
		MAPCOLLIDER::iterator iter = m_mapCollider[i].begin();
		MAPCOLLIDER::iterator iter_end = m_mapCollider[i].end();

		for (iter; iter != iter_end; iter++)
			iter->second.clear();

		m_mapCollider[i].clear();
	}
}

_bool CCollision_Manager::Collision_OBB(const _vec3 * pDestMin, const _vec3 * pDestMax, const _matrix * pDestWorld, 
										const _vec3 * pSourMin, const _vec3 * pSourMax, const _matrix * pSourWorld)
{
	OBB				tObb[2];
	ZeroMemory(&tObb, sizeof(OBB) * 2);

	Set_Point(tObb, pDestMin, pDestMax);
	Set_Point(tObb + 1, pSourMin, pSourMax);

	for (_int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&tObb[0].vPoint[i], &tObb[0].vPoint[i], pDestWorld);
		D3DXVec3TransformCoord(&tObb[1].vPoint[i], &tObb[1].vPoint[i], pSourWorld);
	}
	D3DXVec3TransformCoord(&tObb[0].vCenter, &tObb[0].vCenter, pDestWorld);
	D3DXVec3TransformCoord(&tObb[1].vCenter, &tObb[1].vCenter, pSourWorld);

	for (_int i = 0; i < 2; ++i)
		Set_Axis(tObb + i);


	_float			fDistance[3];

	for (_int i = 0; i < 2; ++i)
	{
		for (_int j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(D3DXVec3Dot(&tObb[0].vProjAxis[0], &tObb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&tObb[0].vProjAxis[1], &tObb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&tObb[0].vProjAxis[2], &tObb[i].vAxis[j]));

			fDistance[1] = fabs(D3DXVec3Dot(&tObb[1].vProjAxis[0], &tObb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&tObb[1].vProjAxis[1], &tObb[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&tObb[1].vProjAxis[2], &tObb[i].vAxis[j]));

			fDistance[2] = fabs(D3DXVec3Dot(&(tObb[1].vCenter - tObb[0].vCenter), &tObb[i].vAxis[j]));

			if (fDistance[0] + fDistance[1] < fDistance[2])
				return false;
		}
	}


	return true;
}

void CCollision_Manager::Set_Point(OBB * pObb, const _vec3 * pMin, const _vec3 * pMax)
{
	pObb->vPoint[0] = _vec3(pMin->x, pMax->y, pMin->z);
	pObb->vPoint[1] = _vec3(pMax->x, pMax->y, pMin->z);
	pObb->vPoint[2] = _vec3(pMax->x, pMin->y, pMin->z);
	pObb->vPoint[3] = _vec3(pMin->x, pMin->y, pMin->z);

	pObb->vPoint[4] = _vec3(pMin->x, pMax->y, pMax->z);
	pObb->vPoint[5] = _vec3(pMax->x, pMax->y, pMax->z);
	pObb->vPoint[6] = _vec3(pMax->x, pMin->y, pMax->z);
	pObb->vPoint[7] = _vec3(pMin->x, pMin->y, pMax->z);

	pObb->vCenter = (*pMin + *pMax) * 0.5f;
}

void  CCollision_Manager::Set_Axis(OBB * pObb)
{
	pObb->vProjAxis[0] = (pObb->vPoint[2] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
	pObb->vProjAxis[1] = (pObb->vPoint[0] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;
	pObb->vProjAxis[2] = (pObb->vPoint[7] + pObb->vPoint[5]) * 0.5f - pObb->vCenter;

	pObb->vAxis[0] = pObb->vPoint[2] - pObb->vPoint[3];
	pObb->vAxis[1] = pObb->vPoint[0] - pObb->vPoint[3];
	pObb->vAxis[2] = pObb->vPoint[7] - pObb->vPoint[3];

	for (_int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pObb->vAxis[i], &pObb->vAxis[i]);
}


