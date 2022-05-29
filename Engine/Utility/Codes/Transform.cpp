#include "Transform.h"

USING(Engine)

Engine::CTransform::CTransform(void)
: m_vScale(1.f, 1.f, 1.f)
, m_vOutlineScale(1.0f, 1.0f, 1.0f)
, m_vAngle(0.f, 0.f, 0.f)
{

}

Engine::CTransform::~CTransform(void)
{

}

const _vec3* Engine::CTransform::Get_Infomation(INFO eType, _vec3* pInfomation)
{
	*pInfomation = m_vInfo[eType];
	return pInfomation;
}

void Engine::CTransform::Get_WorldMatrix(_matrix* pWorldMatrix) const
{
	*pWorldMatrix = m_matWorld;
}

const _matrix* Engine::CTransform::Get_WorldMatrixPointer(void) const
{
	return &m_matWorld;
}

const _vec3* Engine::CTransform::Get_Scale(_vec3* pScale)
{
	*pScale = m_vScale;
	return pScale;
}

const _vec3 * CTransform::Get_OutlineScale(_vec3 * pScale)
{
	*pScale = m_vOutlineScale;
	return pScale;
}

const _vec3* Engine::CTransform::Get_Angle( _vec3* pAngle )
{
	*pAngle = m_vAngle;
	return &m_vAngle;
}

const _matrix* Engine::CTransform::Get_NRotWorldMatrix(_matrix* pWorldMatrix) const
{
	if(NULL != pWorldMatrix)
		*pWorldMatrix = m_matNRotWorld;	
	return &m_matNRotWorld;
}

const _matrix* Engine::CTransform::Get_NScaleWorldMatrix(_matrix* pWorldMatrix) const
{
	if(NULL != pWorldMatrix)
		*pWorldMatrix = m_matNScaleWorld;	
	return &m_matNScaleWorld;
}

void Engine::CTransform::Set_Transform(LPDIRECT3DDEVICE9 pGraphicDev) const
{
	pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
}

void Engine::CTransform::Set_Scale(const _float& fX, const _float& fY, const _float& fZ)
{	
	m_vScale.x = fX, m_vScale.y = fY, m_vScale.z = fZ;
}

void CTransform::Set_OutlineScale(const _float & fX, const _float & fY, const _float & fZ)
{
	m_vOutlineScale.x = fX, m_vOutlineScale.y = fY, m_vOutlineScale.z = fZ;
}


void Engine::CTransform::Add_Scale(const _float& size)
{
	m_vScale.x += size; 
	m_vScale.z += size; 
	m_vScale.y += size;
}

void Engine::CTransform::Add_NotYScale(const _float& size)
{
	m_vScale.x += size;
	m_vScale.z += size;
}



void Engine::CTransform::Set_Position(const _float& fX, const _float& fY, const _float& fZ)
{
	m_vInfo[INFO_POSITION].x = fX; m_vInfo[INFO_POSITION].y = fY; m_vInfo[INFO_POSITION].z = fZ;
}

void Engine::CTransform::Set_Position(const _vec3* pPosition)
{
	m_vInfo[INFO_POSITION] = *pPosition;
}

void Engine::CTransform::Set_AccMotion(const _vec3* pAccMotion)
{
	m_vInfo[INFO_POSITION] += *pAccMotion;
}

void Engine::CTransform::Set_Gravity(const _float& fPosY)
{
	m_vInfo[INFO_POSITION].y += fPosY;
}

void Engine::CTransform::Set_ParentMatrix(const _matrix* pParentMatrix)
{
	if(pParentMatrix == NULL)
		return;
	m_matWorld *= *pParentMatrix;
}

void Engine::CTransform::Set_Angle(const _vec3* fAngle)
{
	m_vAngle = *fAngle;
}
void Engine::CTransform::Set_WorldMatrix(const _matrix* pWorldMatrix)
{
	m_matWorld = *pWorldMatrix;
}

HRESULT Engine::CTransform::Ready_Transform(void)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);
	D3DXMatrixIdentity(&m_matNScaleWorld);

	for (_int i = 0; i < 4; ++i)
	{
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	}

	return S_OK;
}

void Engine::CTransform::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matNRotWorld);
	D3DXMatrixIdentity(&m_matNScaleWorld);

	_matrix matScale, matOutlineScale, matRot[ROT_END], matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixScaling(&matOutlineScale, m_vOutlineScale.x, m_vOutlineScale.y, m_vOutlineScale.z);
	D3DXMatrixRotationX(&matRot[ROT_X], D3DXToRadian(m_vAngle.x));
	D3DXMatrixRotationY(&matRot[ROT_Y], D3DXToRadian(m_vAngle.y));
	D3DXMatrixRotationZ(&matRot[ROT_Z], D3DXToRadian(m_vAngle.z));
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POSITION].x, m_vInfo[INFO_POSITION].y, m_vInfo[INFO_POSITION].z);


	m_matWorld = matScale * matRot[ROT_X] * matRot[ROT_Y] * matRot[ROT_Z] * matTrans;
	m_matNRotWorld = matScale * matTrans;
	m_matScaleRot = matScale * matRot[ROT_X] * matRot[ROT_Y] * matRot[ROT_Z];
	m_matNScaleWorld = matRot[ROT_X] * matRot[ROT_Y] * matRot[ROT_Z] * matTrans;
	m_matOutlineWorld = matOutlineScale * matRot[ROT_X] * matRot[ROT_Y] * matRot[ROT_Z] * matTrans;

	for (_int i = 0; i < 4; ++i)
	{
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));
	}
}

void Engine::CTransform::Move_Position(const _vec3* const pDirection)
{
	m_vInfo[INFO_POSITION] += *pDirection;
}

void Engine::CTransform::Move_TargetPos(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3			vDir = *pTargetPos - m_vInfo[INFO_POSITION];
	m_vInfo[INFO_POSITION] += *D3DXVec3Normalize(&vDir, &vDir) * fSpeed * fTimeDelta;
}

void Engine::CTransform::Move_Forward(const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3			vDirection;
	memcpy(&vDirection, &m_matWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vDirection, &vDirection);
	vDirection.y = 0.f;
	m_vInfo[INFO_POSITION] += vDirection * fSpeed * fTimeDelta;
}

void Engine::CTransform::Move_Right(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3			vDirection;
	D3DXVec3Cross(&vDirection, &_vec3(0.0f, 1.0f, 0.0f), pDir);
	vDirection.y = 0.f;
	m_vInfo[INFO_POSITION] += *D3DXVec3Normalize(&vDirection, &vDirection) * fTimeDelta * fSpeed;		
}

void Engine::CTransform::Move_Left(const _vec3* pDir, const _float& fSpeed, const _float& fTimeDelta)
{
	_vec3			vDirection;
	D3DXVec3Cross(&vDirection, pDir, &_vec3(0.0f, 1.0f, 0.0f));
	vDirection.y = 0.f;
	m_vInfo[INFO_POSITION] += *D3DXVec3Normalize(&vDirection, &vDirection) * fTimeDelta * fSpeed;
}

void Engine::CTransform::Rotation(ROTATION eType, const _float& fAngle)
{
	*(((_float*)&m_vAngle) + eType) += fAngle;
}

void Engine::CTransform::Chase_Target(const _vec3* pTargetPos, const _float& fSpeed)
{
	// 
	_vec3				vDirection = *pTargetPos - m_vInfo[INFO_POSITION];

	m_vInfo[INFO_POSITION] += *D3DXVec3Normalize(&vDirection, &vDirection) * fSpeed;

	// 월드행렬 생성
	_matrix			matRot = *Compute_LookAtTarget(pTargetPos);
	_matrix			matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matTrans, m_vInfo[INFO_POSITION].x, m_vInfo[INFO_POSITION].y, m_vInfo[INFO_POSITION].z);

	m_matWorld = matScale * matRot * matTrans;
}

const _matrix* Engine::CTransform::Compute_LookAtTarget(const _vec3* pTargetPos)
{
	_vec3				vDirection = *pTargetPos - m_vInfo[INFO_POSITION];

	_vec3				vAxis = *D3DXVec3Cross(&vAxis, &m_vInfo[INFO_UP], &vDirection);

	_vec3				vUp;

	_matrix				matRot;

	return D3DXMatrixRotationAxis(&matRot, &vAxis, acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDirection, &vDirection), D3DXVec3Normalize(&vUp, &m_vInfo[INFO_UP]))));
}


CTransform* Engine::CTransform::Create(void)
{
	CTransform*		pInstance = new CTransform;

	if(FAILED(pInstance->Ready_Transform()))
	{
		MSG_BOX("CTransform Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CTransform::Free(void)
{

}





