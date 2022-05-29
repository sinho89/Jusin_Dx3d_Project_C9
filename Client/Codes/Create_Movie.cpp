#include "stdafx.h"
#include "Create_Movie.h"
#include "Export_Engine.h"
#include "MediaObj.h"
#include "Rect_Texture.h"

CCreate_Movie::CCreate_Movie(LPDIRECT3DDEVICE9 pGraphicDev, 
							 const wstring& wstrName, 
							 const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/,
							 const _bool bIsActive /*= true*/)
: CUI(pGraphicDev, wstrName, eState, bIsActive)
, m_pTransCom(NULL)
, m_pBufferCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
, m_pMediaObj(NULL)
, m_bLoop(FALSE)
, m_bRender(FALSE)
{

}

CCreate_Movie::~CCreate_Movie(void)
{

}

HRESULT CCreate_Movie::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransCom->Set_Position(&_vec3(0.0f, 0.04f, 0.0f));
	//m_pTransCom->Set_Scale(2.2f, 1.7f, 0.0f);

	m_fX = (WINSIZEX >> 1);
	m_fY = (WINSIZEY >> 1) + 300;

	m_fSizeX = 380;
	m_fSizeY = 230;

	return S_OK;
}

HRESULT CCreate_Movie::Start_ForScene(void)
{
	return S_OK;
}

void CCreate_Movie::Update_Object(const _float& fTimeDelta)
{
	CUI::Update_Object(fTimeDelta);
	Alpha_Calculation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
}

void CCreate_Movie::Render_Object(void)
{
	if(TRUE == m_bRender)
	{
		LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
		if(NULL == pEffect)
			return;

		_uint			iPassCnt;

		_matrix				matWorld, matView;
		_matrix				matOldView, matOldProj;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

		matWorld._11 = m_fSizeX;
		matWorld._22 = m_fSizeY;
		matWorld._33 = 1.f;
		matWorld._41 = m_fX - WINSIZEX * 0.5f;
		matWorld._42 = -m_fY + WINSIZEY * 0.5f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld); 
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView); 
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj); 

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &m_matProj);

		pEffect->SetFloat("g_fAlpha", m_fAlpha);

		if(NULL != m_pMediaObj)
			pEffect->SetTexture("g_BaseTexture", m_pMediaObj->GetTexture());

		pEffect->Begin(&iPassCnt, 0);
		pEffect->BeginPass(0);

		m_pBufferCom->Render_Buffer();	

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView); 
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj); 
	}
}

void CCreate_Movie::Alpha_Calculation(const _float& fTimeDelta)
{
	m_fAlphaTime += fTimeDelta;

	if(m_fAlpha <= 0.f)
		m_bAlpha_Increase = true;
	if(m_fAlpha >= 1.f)
		m_bAlpha_Increase = false;

	if(m_fAlphaTime >= 0.005f)
	{
		m_fAlphaTime = 0.f;

		if(m_bAlpha_Increase)
			m_fAlpha += 0.01f;
	}
}

_bool CCreate_Movie::EndCheck(void)
{
	CComPtr<IMediaPosition> pMP = m_pMediaObj->m_pMP;

	REFTIME rtNow, rtMax;

	pMP->get_Duration(&rtMax);
	pMP->get_CurrentPosition(&rtNow);

	if(rtNow >= rtMax)
		return TRUE;

	return FALSE;
}

void CCreate_Movie::Loop(void)
{
	CComPtr<IMediaPosition> pMP = m_pMediaObj->m_pMP;

	if(EndCheck())
	{
		pMP->put_CurrentPosition(0);
	}
}

void CCreate_Movie::ReStart(void)
{
	m_pMediaObj->SetTime(0.0);
	m_pMediaObj->Play();
}

HRESULT CCreate_Movie::SetMedia(_tchar* _filePath, _bool _bLoop, _int _iVertexX, _int _iVertexY)
{
	CoInitialize(NULL);

	m_bLoop = _bLoop;

	m_pMediaObj = new Engine::CMediaObj;
	m_pMediaObj->Initialize(m_pGraphicDev, _filePath);

	_float fRatio = m_pMediaObj->GetVidWidth() / (_float)m_pMediaObj->GetVidHeight();

	int iVertexX = m_pMediaObj->GetVidWidth();
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

HRESULT CCreate_Movie::Add_Component(void)
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

CCreate_Movie* CCreate_Movie::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									 const wstring& wstrName, 
									 _tchar* _filePath, 
									 _bool _bLoop, _int 
									 _iVertexX /*= 0*/,
									 _int _iVertexY /*= 0*/, 
									 const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, 
									 const _bool bIsActive /*= true*/)
{
	CCreate_Movie*		pInstance = new CCreate_Movie(pGraphicDev, wstrName, eState, bIsActive);

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

void CCreate_Movie::Free(void)
{
	::Safe_Delete(m_pMediaObj);

	CUI::Free();
}

void CCreate_Movie::Play(void)
{
	m_pMediaObj->Play();
}

_bool& CCreate_Movie::RenderCheck(void)
{
	return m_bRender;
}

