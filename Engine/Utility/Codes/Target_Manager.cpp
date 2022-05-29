#include "Target_Manager.h"
#include "Target.h"
#include "Shader.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTarget_Manager)

Engine::CTarget_Manager::CTarget_Manager(void)
{

}

Engine::CTarget_Manager::~CTarget_Manager(void)
{
}

HRESULT Engine::CTarget_Manager::Ready_Target(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pTargetTag, const _uint & iSizeX, const _uint & iSizeY, D3DFORMAT Format, D3DXCOLOR Color, RENDERTARGETID eRTID)
{
	CTarget*	pTarget = Find_Target(pTargetTag);
	if (NULL != pTarget) return E_FAIL;

	pTarget = CTarget::Create(pGraphicDev, iSizeX, iSizeY, Format, Color, eRTID);
	if (NULL == pTarget) return E_FAIL;

	m_mapTarget.insert(MAPTARGET::value_type(pTargetTag, pTarget));

	return S_OK;
}

HRESULT Engine::CTarget_Manager::Ready_DebugBuffer(const _tchar * pTargetTag, const _uint & iX, const _uint & iY, const _uint & iSizeX, const _uint & iSizeY)
{
	CTarget*		pTarget = Find_Target(pTargetTag);

	if (NULL == pTarget)	return E_FAIL;

	return pTarget->Ready_DebugBuffer(iX, iY, iSizeX, iSizeY);
}

HRESULT Engine::CTarget_Manager::Ready_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag)
{
	CTarget*	pTarget = Find_Target(pTargetTag);
	if (NULL == pTarget)
		return E_FAIL;

	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);

	if (NULL == pMRTList)
	{
		list<CTarget*>			MRTList;
		MRTList.push_back(pTarget);
		m_mapMRT.insert(MAPMRT::value_type(pMRTTag, MRTList));
	}
	else
		pMRTList->push_back(pTarget);

	return S_OK;
}

HRESULT Engine::CTarget_Manager::Begin_MRT(const _tchar * pMRTTag, _uint iIndex)
{
	// 장치에 타겟들을 셋팅한다.
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (NULL == pMRTList)
		return E_FAIL;

	list<CTarget*>::iterator	iter = pMRTList->begin();
	list<CTarget*>::iterator	iter_end = pMRTList->end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Clear_RenderTarget();
	}

	_ulong		dwIndex = iIndex;

	for (iter = pMRTList->begin(); iter != iter_end; ++iter)
	{
		(*iter)->SetUp_OnGraphicDev(dwIndex++);// 장치에 올린다.
	}

	return S_OK;
}

HRESULT CTarget_Manager::Begin_NoClear_MRT(const _tchar * pMRTTag, _uint iIndex)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (NULL == pMRTList)
		return E_FAIL;

	list<CTarget*>::iterator	iter = pMRTList->begin();
	list<CTarget*>::iterator	iter_end = pMRTList->end();

	_ulong		dwIndex = iIndex;

	for (iter = pMRTList->begin(); iter != iter_end; ++iter)
	{
		(*iter)->SetUp_OnGraphicDev(dwIndex++);// 장치에 올린다.
	}

	return S_OK;
}

HRESULT Engine::CTarget_Manager::End_MRT(const _tchar * pMRTTag, _uint iIndex)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (NULL == pMRTList)
		return E_FAIL;

	list<CTarget*>::iterator	iter = pMRTList->begin();
	list<CTarget*>::iterator	iter_end = pMRTList->end();

	_ulong		dwIndex = iIndex;

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Release_OnGraphicDev(dwIndex++);
	}

	return S_OK;
}

HRESULT CTarget_Manager::MRT_Clear(const _tchar * pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (NULL == pMRTList)
		return E_FAIL;

	list<CTarget*>::iterator	iter = pMRTList->begin();
	list<CTarget*>::iterator	iter_end = pMRTList->end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Clear_RenderTarget();
	}

	return S_OK;
}

HRESULT Engine::CTarget_Manager::Render_DebugBuffer(const _tchar * pMRTTag)
{
	list<CTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (NULL == pMRTList)
		return E_FAIL;

	list<CTarget*>::iterator	iter = pMRTList->begin();
	list<CTarget*>::iterator	iter_end = pMRTList->end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_DubugBuffer();
	}

	return S_OK;
}

HRESULT Engine::CTarget_Manager::SetUp_OnShader(const _tchar * pTargetTag, LPD3DXEFFECT pEffect, const char * pConstantName)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (NULL == pTarget) return E_FAIL;

	return pTarget->SetUp_OnShader(pEffect, pConstantName);
}

CTarget * Engine::CTarget_Manager::Find_Target(const _tchar * pTargetTag)
{
	MAPTARGET::iterator	iter = find_if(m_mapTarget.begin(), m_mapTarget.end(), CTag_Finder(pTargetTag));

	if (iter == m_mapTarget.end())	return NULL;

	return iter->second;
}

list<CTarget*>* Engine::CTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	MAPMRT::iterator	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CTag_Finder(pMRTTag));

	if (iter == m_mapMRT.end())	return NULL;

	return &iter->second;
}

LPDIRECT3DTEXTURE9 Engine::CTarget_Manager::GetTargetTexture(const _tchar * pTargetTag)
{
	CTarget*	pTarget = Find_Target(pTargetTag);

	if (NULL == pTarget) return NULL;

	return pTarget->GetTargetTexture();
}

void Engine::CTarget_Manager::Free(void)
{
	for_each(m_mapTarget.begin(), m_mapTarget.end(), CRelease_Pair());
	m_mapTarget.clear();
}
