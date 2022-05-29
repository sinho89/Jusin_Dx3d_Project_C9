#include "stdafx.h"
#include "IceTwister.h"
#include "Export_Engine.h"


CIceTwister::CIceTwister(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: CTrap(pGraphicDev, wstrName)
{
	//ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}
CIceTwister::~CIceTwister(void)
{

}

void CIceTwister::Update_Object(const _float& fTimeDelta)
{
	CTrap::Update_Object(fTimeDelta);

}

void CIceTwister::Render_Object(void)
{
	//CTrap::Render_Object();
}

void CIceTwister::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	CTrap::Render_Object(pEffect, iPassIdx);
}

HRESULT CIceTwister::Ready_Object(void)
{
	CTrap::Ready_Object();

	m_bOutlineColor = _vec3(1.0f, 0.8549f, 0.7254f);

	return S_OK;
}


void CIceTwister::Free(void)
{
	CTrap::Free();
	//	Safe_Delete(m_pMouseCol);

}

CIceTwister* CIceTwister::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CIceTwister*		pInstance = new CIceTwister(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGrid Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}
