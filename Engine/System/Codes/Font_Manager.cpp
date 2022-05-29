#include "Font_Manager.h"
#include "Font.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFont_Manager)

Engine::CFont_Manager::CFont_Manager(void)
{

}

Engine::CFont_Manager::~CFont_Manager(void)
{

}

HRESULT Engine::CFont_Manager::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontPath, const _tchar* pFontTag , const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont*			pFont = NULL;

	pFont = Find_Font(pFontTag);
	if(pFont != NULL)
		return E_FAIL;

	pFont = CFont::Create(pGraphicDev, pFontPath, pFontType, iWidth, iHeight, iWeight);

	if(pFont == NULL)
		return E_FAIL;

	m_mapFont.insert(MAPFONT::value_type(pFontTag, pFont));

	return S_OK;	
}

void Engine::CFont_Manager::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPosition, D3DXCOLOR Color)
{
	CFont*	pFont = Find_Font(pFontTag);

	if(pFont == NULL)
		return;

	pFont->Render_Font(pString, pPosition, Color);
}

CFont* Engine::CFont_Manager::Find_Font(const _tchar* pFontTag)
{
	MAPFONT::iterator	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(pFontTag));
	if(iter == m_mapFont.end())
		return NULL;

	return iter->second;
}

void Engine::CFont_Manager::Free(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CRelease_Pair());
	m_mapFont.clear();
}

