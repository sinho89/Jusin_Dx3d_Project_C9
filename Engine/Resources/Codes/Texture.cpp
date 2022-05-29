#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CResource(pGraphicDev)
{

}

Engine::CTexture::CTexture(const CTexture& rhs)
: Engine::CResource(rhs)
{
	_uint iContainerSize = rhs.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = rhs.m_vecTexture;
	for (_uint i = 0; i < iContainerSize; ++i)
		m_vecTexture[i]->AddRef();	
}

Engine::CTexture::~CTexture(void)
{

}

void Engine::CTexture::Set_Texture(const _uint& iIndex)
{
	if(m_vecTexture.size() < iIndex)
		return;

	m_pGraphicDev->SetTexture(0, m_vecTexture[iIndex]);
}

void Engine::CTexture::Set_Texture(LPD3DXEFFECT pEffect, const char* pShaderName, const _uint& iIndex)
{
	if(m_vecTexture.size() < iIndex)
		return;

	pEffect->SetTexture(pShaderName, m_vecTexture[iIndex]);	
}

HRESULT Engine::CTexture::Ready_Texture(const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt)
{
	m_vecTexture.reserve(iCnt);

	IDirect3DBaseTexture9*			pTexture = NULL;

	for(_uint i = 0; i < iCnt; ++i)
	{
		_tchar			szFileName[128] = L"";

		wsprintf(szFileName, pPath, i);	

		switch(eType)
		{
		case TEX_NORMAL:
			if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case TEX_CUBE:
			if(FAILED(D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
	}
	return S_OK;
}

CResource* Engine::CTexture::Clone(void)
{
	return new CTexture(*this);
}

CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, TEXTURETYPE eType, const _uint& iCnt /*= 1*/)
{
	CTexture*		pInstance = new CTexture(pGraphicDev);

	if(FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
	{
		MSG_BOX("CTexture Created Failed");
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}

void Engine::CTexture::Free(void)
{	
	Engine::CResource::Free();

	_uint iContainerSize = m_vecTexture.size();

	for (_uint i = 0; i < iContainerSize; ++i)
	{
		Engine::Safe_Release(m_vecTexture[i]);
	}
	m_vecTexture.clear();
}

