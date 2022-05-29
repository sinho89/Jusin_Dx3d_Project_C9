#include "stdafx.h"
#include "Skill_Avi.h"
#include "Export_Engine.h"
#include "MediaObj.h"
#include "Rect_Texture.h"

CSkill_Avi::CSkill_Avi(LPDIRECT3DDEVICE9 pGraphicDev,
	const wstring& wstrName,
	const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/,
	const _bool bIsActive /*= true*/)
	: CUI(pGraphicDev, wstrName, eState, bIsActive)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pRendererCom(NULL)
	, m_pShaderCom(NULL)
	, m_bLoop(FALSE)
{
	for (int i = 0; i < SKILL_END; ++i)
		m_pMediaObj[i] = NULL;
}

CSkill_Avi::~CSkill_Avi(void)
{

}

HRESULT CSkill_Avi::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransCom->Set_Position(&_vec3(0.0f, 0.04f, 0.0f));
	//m_pTransCom->Set_Scale(2.2f, 1.7f, 0.0f);

	m_fX = (WINSIZEX >> 1) - 200;
	m_fY = (WINSIZEY >> 1);

	m_fSizeX = 250;
	m_fSizeY = 150;

	m_bRender = false;
	m_eType = SKILL_END;
	m_fAlpha = 1.f;

	m_bIsStatic = true;
	return S_OK;
}

HRESULT CSkill_Avi::Start_ForScene(void)
{
	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
	{
		m_bIsActive = false;
		return S_OK;
	}
	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
		m_bIsActive = true;

	return S_OK;
}

void CSkill_Avi::Update_Object(const _float& fTimeDelta)
{
	if (m_eType == SKILL_NONE)
		return;

	if (m_bRender)
	{
		CUI::Update_Object(fTimeDelta);
		m_wSortNum = 8;
		if (m_eType != SKILL_END)
			m_pRendererCom->Add_WindowUI(m_wSortNum, this);
		D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

	}
	else
	{
		if(m_eType != SKILL_END)
			ReStart();
		m_eType = SKILL_END;
	}
}

void CSkill_Avi::Render_Object(void)
{

	if (EndCheck())
		ReStart();

	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	if (NULL == pEffect)
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

	if (NULL != m_pMediaObj[m_eType])
		pEffect->SetTexture("g_BaseTexture", m_pMediaObj[m_eType]->GetTexture());

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
	
}



_bool CSkill_Avi::EndCheck(void)
{
	CComPtr<IMediaPosition> pMP = m_pMediaObj[m_eType]->m_pMP;

	REFTIME rtNow, rtMax;

	pMP->get_Duration(&rtMax);
	pMP->get_CurrentPosition(&rtNow);

	if (rtNow >= rtMax)
		return TRUE;

	return FALSE;
}

void CSkill_Avi::Loop(void)
{
	CComPtr<IMediaPosition> pMP = m_pMediaObj[m_eType]->m_pMP;

	if (EndCheck())
	{
		pMP->put_CurrentPosition(0);
	}
}

void CSkill_Avi::ReStart(void)
{
	if (m_eType == SKILL_END)
		return;

	m_pMediaObj[m_eType]->SetTime(0.0);
	m_pMediaObj[m_eType]->Play();
}

void CSkill_Avi::ReSet(void)
{
	if (m_eType == SKILL_END)
		return;

	m_pMediaObj[m_eType]->SetTime(0.0);
}

HRESULT CSkill_Avi::SetMedia(_bool _bLoop, _int _iVertexX, _int _iVertexY)
{

	for (int i = 0; i < SKILL_END; ++i)
	{
		TCHAR* szPath;


		switch(i)
		{
		case SKILL_RUSH:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_FLYDANCE:
			szPath = L"../Bin/Resource/UI/ToolTip/FlyDance.avi";
			break;
		case SKILL_FLEDGE_BLADE:
			szPath = L"../Bin/Resource/UI/ToolTip/FledgeBlade.avi";
			break;
		case SKILL_STUNPIN:
			szPath = L"../Bin/Resource/UI/ToolTip/SpinTurn.avi";
			break;
		case SKILL_VIOLET:
			szPath = L"../Bin/Resource/UI/ToolTip/Violet.avi";
			break;
		case SKILL_CROSS:
			szPath = L"../Bin/Resource/UI/ToolTip/Cross.avi";
			break;
		case SKILL_SPIRALDANCE:
			szPath = L"../Bin/Resource/UI/ToolTip/Sprier.avi";
			break;
		case SKILL_DARKPOLLING:
			szPath = L"../Bin/Resource/UI/ToolTip/DarkPoling.avi";
			break;
		case SKILL_DASH:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_LIDDLEKICK:
			szPath = L"../Bin/Resource/UI/ToolTip/RiddleKick.avi";
			break;
		case SKILL_EVADEATT:
			szPath = L"../Bin/Resource/UI/ToolTip/Evade.avi";
			break;
		case SKILL_TECHDOWN:
			szPath = L"../Bin/Resource/UI/ToolTip/TechDown.avi";
			break;
		case SKILL_BLARE:
			szPath = L"../Bin/Resource/UI/ToolTip/BlareSiluet.avi";
			break;
		case SKILL_UPATT:
			szPath = L"../Bin/Resource/UI/ToolTip/UpAtt.avi";
			break;
		case SKILL_VIFERKICK:
			szPath = L"../Bin/Resource/UI/ToolTip/Viper.avi";
			break;
		case SKILL_SPININGHEART:
			szPath = L"../Bin/Resource/UI/ToolTip/Spining.avi";
			break;
		case SKILL_BLACKROSE:
			szPath = L"../Bin/Resource/UI/ToolTip/BlackRose.avi";
			break;
		case SKILL_DRAGONDANCE:
			szPath = L"../Bin/Resource/UI/ToolTip/DragonDance.avi";
			break;
		case SKILL_SKINNING:
			szPath = L"../Bin/Resource/UI/ToolTip/Spining.avi";
			break;
		case SKILL_TRICKEY:
			szPath = L"../Bin/Resource/UI/ToolTip/Tricky.avi";
			break;
		case SKILL_FLENITDANCE:
			szPath = L"../Bin/Resource/UI/ToolTip/Planit.avi";
			break;
		case SKILL_PLORA:
			szPath = L"../Bin/Resource/UI/ToolTip/Plora.avi";
			break;
		case SKILL_SOFTLANDING:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_SILUETDANCE:
			szPath = L"../Bin/Resource/UI/ToolTip/Siluet.avi";
			break;
		case SKILL_HILLTURN:
			szPath = L"../Bin/Resource/UI/ToolTip/HillTurn.avi";
			break;
		case SKILL_URBANBREAT:
			szPath = L"../Bin/Resource/UI/ToolTip/UrbanBreak.avi";
			break;
		case SKILL_MAXHP:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_MAXMP:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_CRITICAL:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_ATTMASTER:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_BLOODAGIL:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_SPINCRASH:
			szPath = L"../Bin/Resource/UI/ToolTip/SpinCrash_Avi.avi";
			break;
		case SKILL_LOCKON:
			szPath = L"../Bin/Resource/UI/ToolTip/LockOn_Avi.avi";
			break;
		case SKILL_SHOTJUMP:
			szPath = L"../Bin/Resource/UI/ToolTip/ShotJump_Avi.avi";
			break;
		case SKILL_UPPERKICK:
			szPath = L"../Bin/Resource/UI/ToolTip/UpperKick_Avi.avi";
			break;
		case SKILL_FIREM:
			szPath = L"../Bin/Resource/UI/ToolTip/FireM_Avi.avi";
			break;
		case SKILL_DOWNKICK:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_JUMPDOWNKICK:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_SHELLBUSTER:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_HOLDSHOT:
			szPath = L"../Bin/Resource/UI/ToolTip/HoldShot_Avi.avi";
			break;
		case SKILL_GRAVITY:
			szPath = L"../Bin/Resource/UI/ToolTip/Gravity_Avi.avi";
			break;
		case SKILL_ESCAPE:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_GEILSHOT:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		case SKILL_MOVINGCANNON:
			szPath = L"../Bin/Resource/UI/ToolTip/MovingCannon_Avi.avi";
			break;
		case SKILL_NONE:
			szPath = L"../Bin/Resource/UI/ToolTip/Rush.avi";
			break;
		}

		CoInitialize(NULL);

		m_bLoop = _bLoop;

		m_pMediaObj[i] = new Engine::CMediaObj;
		m_pMediaObj[i]->Initialize(m_pGraphicDev, szPath);

		_float fRatio = m_pMediaObj[i]->GetVidWidth() / (_float)m_pMediaObj[i]->GetVidHeight();

		int iVertexX = m_pMediaObj[i]->GetVidWidth();
		int iVertexY = m_pMediaObj[i]->GetVidHeight();

		if (_iVertexX)
		{
			iVertexX = _iVertexX;

			if (_iVertexX)
				iVertexX = _iVertexX;
			else
				iVertexY = int(iVertexX * fRatio);
		}

		m_pMediaObj[i]->SetSound(-500);
		m_pMediaObj[i]->Pause();

	}

	return S_OK;
}

void CSkill_Avi::SetType(SKILL_ID eType)
{
	m_eType = eType;
}

HRESULT CSkill_Avi::Add_Component(void)
{
	Engine::CComponent*				pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Buffer Component
	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	// For.Shader Component
	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_UI");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

CSkill_Avi* CSkill_Avi::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const wstring& wstrName,
	_bool _bLoop, _int
	_iVertexX /*= 0*/,
	_int _iVertexY /*= 0*/,
	const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/,
	const _bool bIsActive /*= true*/)
{
	CSkill_Avi*		pInstance = new CSkill_Avi(pGraphicDev, wstrName, eState, bIsActive);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("MediaUI Created Failed");
		::Safe_Release(pInstance);
	}

	if (FAILED(pInstance->SetMedia(_bLoop, _iVertexX, _iVertexY)))
	{
		MSG_BOX("MediaUI Created Failed");
		::Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Avi::Free(void)
{
	for(int i = 0; i < SKILL_END; ++i)
		::Safe_Delete(m_pMediaObj[i]);

	CUI::Free();
}

void CSkill_Avi::Play(void)
{
	if (m_eType == SKILL_END)
		return;

	m_pMediaObj[m_eType]->Play();
}

_bool& CSkill_Avi::RenderCheck(void)
{
	return m_bRender;
}

