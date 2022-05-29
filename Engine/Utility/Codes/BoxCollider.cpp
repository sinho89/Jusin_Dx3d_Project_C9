#include "BoxCollider.h"

#include "Transform.h"
#include "Cube_Color.h"

USING(Engine)

Engine::CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 pGraphicDev)
: CCollider(pGraphicDev)
{

}

Engine::CBoxCollider::~CBoxCollider(void)
{

}

void Engine::CBoxCollider::Update_Component(const _float& fTimeDelta)
{
	D3DXMatrixIdentity(&m_matWorld);

	_matrix matParent;
	m_pParentTransform->Get_NScaleWorldMatrix(&matParent);

	_matrix matTrans, matScale;
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	m_matWorld = matScale * matTrans * matParent;
}

void Engine::CBoxCollider::Render_Collider(_bool bIsRender /*= true*/)
{
	if(!bIsRender)
		return;

	m_pBuffer->Render_Buffer();
}

HRESULT Engine::CBoxCollider::Ready_BoxCol(CTransform* pParentTransform, const wstring& wstrTag, 
								const _vec3& vPos, const _vec3& vScale, const _ulong& dwColor, _uint iIndex)
{
	if(pParentTransform == NULL)
		return E_FAIL;

	m_pParentTransform = pParentTransform;
	m_wstrTag = wstrTag;
	m_vPos = vPos;
	SetScale(vScale);
	m_eType = COL_BOX;

	m_pBuffer = CCube_Color::Create(m_pGraphicDev, &m_matWorld, dwColor);
	if(m_pBuffer == NULL)
		return E_FAIL;

	m_iIndex = iIndex;

	return S_OK;
}

CBoxCollider* Engine::CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTransform, 
											const wstring& wstrTag, const _vec3& vPos, const _vec3& vScale, 
											const _ulong& dwColor, _uint iIndex)
{
	CBoxCollider* pInstance = new CBoxCollider(pGraphicDev);
	if(FAILED(pInstance->Ready_BoxCol(pParentTransform, wstrTag, vPos, vScale, dwColor, iIndex)))
	{
		MSG_BOX("CBoxCollider Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CBoxCollider::Free(void)
{
	Engine::Safe_Release(m_pBuffer);
}

