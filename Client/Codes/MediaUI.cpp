#include "stdafx.h"
#include "MediaUI.h"
#include "Export_Engine.h"
#include "MediaObj.h"
#include "Rect_Texture.h"
#include "Opening_Banner.h"

CMediaUI::CMediaUI(LPDIRECT3DDEVICE9 pGraphicDev, 
				   const wstring& wstrName, 
				   const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, 
				   const _bool bIsActive /*= true*/)
: CUI(pGraphicDev, wstrName, eState, bIsActive)
, m_pTransCom(NULL)
, m_pBufferCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
, m_pMediaObj(NULL)
, m_pOpenningBanner(NULL)
, m_bLoop(FALSE)
, m_bPlayButton(FALSE)
{
	
}

CMediaUI::~CMediaUI(void)
{

}

HRESULT CMediaUI::Start_ForScene(void)
{
	m_pOpenningBanner = static_cast<COpening_Banner*>(Engine::Find(L"OpeningBanner"));	

	return S_OK;
}

HRESULT CMediaUI::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(&_vec3(0.0f, 0.04f, 0.0f));
	m_pTransCom->Set_Scale(2.2f, 1.7f, 0.0f);

	return S_OK;
}

void CMediaUI::Update_Object(const _float& fTimeDelta)
{
	if(TRUE == m_pOpenningBanner->GetAlphaIncrease())
	{
		if(FALSE == m_bPlayButton)
			m_pMediaObj->Play();
	}	

	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_PRIORITY, this);
}

void CMediaUI::Render_Object(void)
{
	if(TRUE == m_pOpenningBanner->GetAlphaIncrease())
	{
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		if(NULL == pEffect)
			return;

		_uint			iPassCnt;

		_matrix				matWorld, matView, matProj;

		m_pTransCom->Get_WorldMatrix(&matWorld);
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &matProj);

		pEffect->SetFloat("g_fAlpha", m_fAlpha);

		if(NULL != m_pMediaObj)
			pEffect->SetTexture("g_BaseTexture", m_pMediaObj->GetTexture());

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pBufferCom->Render_Buffer();	

		pEffect->EndPass();
		pEffect->End();
	}	
}

void CMediaUI::Alpha_Calculation(const _float& fTimeDelta)
{
	if(TRUE == m_pOpenningBanner->GetAlphaIncrease())
	{
		m_fAlphaTime += fTimeDelta;

		if(m_fAlphaTime >= 0.05f)
		{
			m_fAlphaTime = 0.f;

			m_fAlpha += 0.03f;
			if(m_fAlpha > 1.0f)
				m_fAlpha = 1.0f;
		}
	}
}

void CMediaUI::Loop(void)
{
	CComPtr<IMediaPosition> pMP = m_pMediaObj->m_pMP;

	if(EndCheck())
	{
		pMP->put_CurrentPosition(0);
	}
}

HRESULT CMediaUI::SetMedia(_tchar* _filePath, _bool _bLoop, _int _iVertexX, _int _iVertexY)
{
	CoInitialize(NULL);

	m_bLoop = _bLoop;

	m_pMediaObj = new Engine::CMediaObj;
	m_pMediaObj->Initialize(m_pGraphicDev, _filePath);

	_float fRatio = m_pMediaObj->GetVidWidth() / (_float)m_pMediaObj->GetVidHeight();

	int iVertexX = m_pMediaObj->GetVidHeight();
	int iVertexY = m_pMediaObj->GetVidHeight();

	if(_iVertexX)
	{
		iVertexX = _iVertexX;

		if(_iVertexX)
			iVertexX = _iVertexX;
		else
			iVertexY = int(iVertexX * fRatio);
	}

	m_pMediaObj->SetSound(-500);
	m_pMediaObj->Pause();

	return S_OK;
}

_bool CMediaUI::EndCheck(void)
{
	CComPtr<IMediaPosition> pMP = m_pMediaObj->m_pMP;

	REFTIME rtNow, rtMax;

	pMP->get_Duration(&rtMax);
	pMP->get_CurrentPosition(&rtNow);

	if(rtNow >= rtMax)
		return TRUE;

	return FALSE;
}

HRESULT CMediaUI::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create(); 
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	// For.Shader Component
	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_UI");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

CMediaUI* CMediaUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
						   const wstring& wstrName, 
						   _tchar* _filePath,
						   _bool _bLoop,
						   _int _iVertexX,
						   _int _iVertexY,
						   const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, 
						   const _bool bIsActive /*= true*/)
{
	CMediaUI*		pInstance = new CMediaUI(pGraphicDev, wstrName, eState, bIsActive);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("MediaUI Created Failed");
		::Safe_Release(pInstance);
	}

	if(FAILED(pInstance->SetMedia(_filePath, _bLoop, _iVertexX, _iVertexY)))
	{
		MSG_BOX("MediaUI Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CMediaUI::Free(void)
{
	::Safe_Release(m_pOpenningBanner);
	::Safe_Delete(m_pMediaObj);

	CoUninitialize();

	CUI::Free();
}
