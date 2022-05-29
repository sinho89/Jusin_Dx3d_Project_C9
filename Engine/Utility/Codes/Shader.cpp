#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
, m_pEffect(NULL)
{
	m_pGraphicDev->AddRef();
}

CShader::CShader(const CShader& rhs)
: m_pGraphicDev(rhs.m_pGraphicDev)
, m_pEffect(rhs.m_pEffect)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	if(FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFilePath, NULL, NULL, D3DXSHADER_DEBUG
		, NULL, &m_pEffect, &m_pErrMsg)))
	{

		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader Error", MB_OK);
		return E_FAIL;
	}

	else if(NULL != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader Warning", MB_OK);
	}

	return S_OK;
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*		pInstance = new CShader(pGraphicDev);

	if(FAILED(pInstance->Ready_Shader(pFilePath)))
	{
		MSG_BOX("CShader Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pEffect);
}

