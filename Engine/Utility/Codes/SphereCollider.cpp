#include "SphereCollider.h"

#include "Transform.h"
#include "Sphere_Color.h"

USING(Engine)

Engine::CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev)
: CCollider(pGraphicDev)
{

}

Engine::CSphereCollider::~CSphereCollider(void)
{

}

void Engine::CSphereCollider::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);
	
	_matrix matParent;
	m_pParentTransform->Get_NScaleWorldMatrix(&matParent);
	
	_matrix matTrans, matScale;
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	m_matWorld = matScale * matTrans * matParent;
}

void Engine::CSphereCollider::Render_Collider(_bool bIsRender /*= true*/)
{
	if(!m_bIsRender || !bIsRender)
		return;

	m_pBuffer->Render_Buffer();
}

HRESULT Engine::CSphereCollider::Ready_SphereCol(CTransform* pParentTransform, const wstring& wstrTag, const _vec3& vPos, const _float& fRad, _uint iIndex)
{
	if(pParentTransform == NULL)
		return E_FAIL;

	m_pParentTransform = pParentTransform;
	m_wstrTag = wstrTag;
	m_vPos = vPos;
	SetScale(fRad);
	m_eType = COL_SPHERE;

	m_pBuffer = CSphere_Color::Create(m_pGraphicDev, &m_matWorld);
	if(m_pBuffer == NULL)
		return E_FAIL;

	m_iIndex = iIndex;

	return S_OK;
}

CSphereCollider* Engine::CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, 
												 const wstring& wstrTag, const _vec3& vPos, const _float& fRad, _uint iIndex)
{
	CSphereCollider* pInstance = new CSphereCollider(pGraphicDev);
	if(FAILED(pInstance->Ready_SphereCol(pParentTransform, wstrTag, vPos, fRad, iIndex)))
	{
		MSG_BOX("CSphereCollider Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CSphereCollider::Free(void)
{
	Engine::Safe_Release(m_pBuffer);
}

