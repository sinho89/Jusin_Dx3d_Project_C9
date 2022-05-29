#include "Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Screen_Texture.h"
#include "Camera.h"
#include "Texture.h"
//#include "Export_Utility.h"
//#include "Export_Resource.h"
#include "Export_Engine.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
	: m_pScreenTexture			(NULL)
	, m_pScreent_Quad_Texture	(NULL)
	, m_pTexRandNormal			(NULL)
	, m_bDebugBuffer			(FALSE)
	, m_bShadowBuffer			(FALSE)
	, m_bSoftShadow				(FALSE)
	, m_bCACDShadow				(FALSE)
	, m_bMotionBlurBuffer		(FALSE)
	, m_bGaussianBlur			(FALSE)
	, m_bGaussianClear			(FALSE)
	, m_bHDR					(FALSE)
	, m_bDOF					(FALSE)
	, m_bSSAO					(FALSE)
	, m_bSSAO_Clear				(FALSE)
	, m_bRandNormal				(FALSE)
	, m_bExpFog					(FALSE)
	, m_bSquareFog				(FALSE)
	, m_bHeightFog				(FALSE)
	, m_bLimLight				(FALSE)
	, m_bRadialBlur				(FALSE)
	, m_iCACD_Cnt				(0)
	, m_fBlurValue				(0.0f)
	, m_fBrightnessValue		(0.0f)
	, m_fLum_Average			(-0.08f)
	, m_fA						(6.20f)
	, m_fB						(0.50f)
	, m_fC						(1.70f)
	, m_fD						(0.06f)
	, m_fDOF_Focus				(0.5f)
	, m_fDOF_Range				(1.0f)
	, m_fRadialBlur_Width		(0.0f)
{
	
}

Engine::CRenderer::~CRenderer(void)
{

}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if(eGroup >= RENDER_END)return;

	m_RenderGroup[eGroup].push_back(pGameObject);

	pGameObject->AddRef();
}

void Engine::CRenderer::Add_WindowUI(WORD wSortNum, CGameObject* pGameObject)
{
	if(pGameObject == NULL)	return;

	m_WindowUI.insert(WINDOWUIMAP::value_type(wSortNum, pGameObject));

	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Render_Priority(pGraphicDev);

	if (PROJECT_CLIENT == m_eProjectID)
	{
		if (SCENE_SELECT	==	GetSceneID() ||
			SCENE_TOWN		==	GetSceneID() ||
			SCENE_STAGE_ICE	==	GetSceneID() ||
			SCENE_STAGE_FIRE == GetSceneID())
		{
			Render_Deferred(pGraphicDev);	//NONALPHA 객체들을 디퍼드 쉐이딩으로 그린다.
			Render_LightAcc(pGraphicDev);	//출력된 노말 텍스쳐로 빛을 연산
			Render_Outline(pGraphicDev);
			Render_Shadow(pGraphicDev);		//그림자를 그려준다.
			Render_SSAO(pGraphicDev);
			Render_Blend(pGraphicDev);		//출력된 텍스쳐를 조합하여 그림 연출.
			Render_DOF(pGraphicDev);
			Render_Final(pGraphicDev);
			Render_RadialBlur(pGraphicDev);
			Render_MotionBlur(pGraphicDev);	//Motion Blur
			Render_PostEffect(pGraphicDev);
			Render_MiniMap(pGraphicDev);
		}

		//Render_Alpha(pGraphicDev);
		Render_UI(pGraphicDev);
		Render_Window(pGraphicDev);
		Render_Cursor(pGraphicDev);
		
		if (m_bDebugBuffer)
		{
			Render_DebugBuffer(L"MRT_Deferred_0");
			Render_DebugBuffer(L"MRT_CACD_Shadow_1");
			Render_DebugBuffer(L"MRT_CACD_Shadow_2");
			Render_DebugBuffer(L"MRT_Deferred_1");
			Render_DebugBuffer(L"MRT_Deferred_2");
			Render_DebugBuffer(L"MRT_Deferred_3");
			//Render_DebugBuffer(L"MRT_Deferred_4");
			Render_DebugBuffer(L"MRT_Decal");
			Render_DebugBuffer(L"MRT_Outline");
			Render_DebugBuffer(L"MRT_SSAO");
			Render_DebugBuffer(L"MRT_DOF");
			Render_DebugBuffer(L"MRT_Effect");
			Render_DebugBuffer(L"MRT_Shadow");
			Render_DebugBuffer(L"MRT_LightAcc");
			Render_DebugBuffer(L"MRT_Char_Sampler_Shade");
			Render_DebugBuffer(L"MRT_Char_Sampler_Shade_HDR");
			//Render_DebugBuffer(L"MRT_Blend");
			Render_DebugBuffer(L"MRT_Final");
			//Render_DebugBuffer(L"MRT_Motion_Blur");
			Render_DebugBuffer(L"MRT_Light_Blur");
			Render_DebugBuffer(L"MRT_All_PostEffect_Blur");
			//Render_DebugBuffer(L"MRT_All_PostEffect_Blur_Depth");
		}
	}
	else if (PROJECT_ANIMATION	== m_eProjectID ||		//Tool . ver
			 PROJECT_MAP		== m_eProjectID ||
			 PROJECT_EFFECT		== m_eProjectID)
	{
		Render_NoneAlpha(pGraphicDev);
		Render_Alpha(pGraphicDev);
		Render_UI(pGraphicDev);
		Render_Window(pGraphicDev);
		Render_Cursor(pGraphicDev);
	}		

	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for(_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CRelease_Single());
		m_RenderGroup[i].clear();
	}

	for_each(m_WindowUI.begin(), m_WindowUI.end(), CRelease_Pair());
	m_WindowUI.clear();
}

void Engine::CRenderer::Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_PRIORITY].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_PRIORITY].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_ShadowMap(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_DynamicMesh");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;
	
	pEffect->AddRef();

	m_eMRTID = MRT_DRAW_DYNAMIC_SHADOWMAP;
	_uint iPassIdx = 3;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);

	RENDERGROUP::iterator	iter = m_RenderGroup[RENEDER_DYANAMIC_SHADOW].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENEDER_DYANAMIC_SHADOW].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object(pEffect, iPassIdx);
	}

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	pShader = (Engine::CShader*)Clone_Prototype(L"Shader_StaticMesh");
	if (NULL == pShader) return;
	pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	pEffect->AddRef();

	m_eMRTID = MRT_DRAW_STATIC_SHADOWMAP;
	iPassIdx = 2;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);

	iter = m_RenderGroup[RENDER_STATIC_SHADOW].begin();
	iter_end = m_RenderGroup[RENDER_STATIC_SHADOW].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object(pEffect, iPassIdx);
	}

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);
}

void CRenderer::Render_CACD_ShadowMap(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader* pDynamic_Shader = (CShader*)Clone_Prototype(L"Shader_DynamicMesh");
	if (NULL == pDynamic_Shader) return;
	LPD3DXEFFECT pDynamic_Effect = pDynamic_Shader->Get_EffectHandle();
	if (NULL == pDynamic_Effect) return;

	CShader* pStatic_Shader = (Engine::CShader*)Clone_Prototype(L"Shader_StaticMesh");
	if (NULL == pStatic_Shader) return;
	LPD3DXEFFECT pStatic_Effect = pStatic_Shader->Get_EffectHandle();
	if (NULL == pStatic_Effect) return;

	//=================================================
	m_iCACD_Cnt = 0;
	Begin_MRT(L"MRT_Deferred_0");
	
	pDynamic_Effect->AddRef();
	pStatic_Effect->AddRef();

	m_eMRTID = MRT_DRAW_DYNAMIC_SHADOWMAP;

	_uint iPassIdx = 4;

	pDynamic_Effect->Begin(NULL, 0);
	pDynamic_Effect->BeginPass(iPassIdx);

	RENDERGROUP::iterator	DY_iter = m_RenderGroup[RENEDER_DYANAMIC_SHADOW].begin();
	RENDERGROUP::iterator	DY_iter_end = m_RenderGroup[RENEDER_DYANAMIC_SHADOW].end();

	for (; DY_iter != DY_iter_end; ++DY_iter)
	{
		(*DY_iter)->Render_Object(pDynamic_Effect, iPassIdx);
	}

	pDynamic_Effect->EndPass();
	pDynamic_Effect->End();

	m_eMRTID = MRT_DRAW_STATIC_SHADOWMAP;
	iPassIdx = 5;

	pStatic_Effect->Begin(NULL, 0);
	pStatic_Effect->BeginPass(iPassIdx);

	RENDERGROUP::iterator	ST_iter = m_RenderGroup[RENDER_STATIC_SHADOW].begin();
	RENDERGROUP::iterator	ST_iter_end = m_RenderGroup[RENDER_STATIC_SHADOW].end();

	for (; ST_iter != ST_iter_end; ++ST_iter)
	{
		(*ST_iter)->Render_Object(pStatic_Effect, iPassIdx);
	}

	pStatic_Effect->EndPass();
	pStatic_Effect->End();

	End_MRT(L"MRT_Deferred_0");

	//=================================================
	m_iCACD_Cnt = 1;
	Begin_MRT(L"MRT_CACD_Shadow_1");

	m_eMRTID = MRT_DRAW_DYNAMIC_SHADOWMAP;
	iPassIdx = 4;

	pDynamic_Effect->Begin(NULL, 0);
	pDynamic_Effect->BeginPass(iPassIdx);

	DY_iter = m_RenderGroup[RENEDER_DYANAMIC_SHADOW].begin();
	for (; DY_iter != DY_iter_end; ++DY_iter)
	{
		(*DY_iter)->Render_Object(pDynamic_Effect, iPassIdx);
	}

	pDynamic_Effect->EndPass();
	pDynamic_Effect->End();

	m_eMRTID = MRT_DRAW_STATIC_SHADOWMAP;
	iPassIdx = 5;

	pStatic_Effect->Begin(NULL, 0);
	pStatic_Effect->BeginPass(iPassIdx);

	ST_iter = m_RenderGroup[RENDER_STATIC_SHADOW].begin();
	for (; ST_iter != ST_iter_end; ++ST_iter)
	{
		(*ST_iter)->Render_Object(pStatic_Effect, iPassIdx);
	}

	pStatic_Effect->EndPass();
	pStatic_Effect->End();
	
	End_MRT(L"MRT_CACD_Shadow_1");

	//=================================================
	m_iCACD_Cnt = 2;
	Begin_MRT(L"MRT_CACD_Shadow_2");

	m_eMRTID = MRT_DRAW_DYNAMIC_SHADOWMAP;
	iPassIdx = 4;

	pDynamic_Effect->Begin(NULL, 0);
	pDynamic_Effect->BeginPass(iPassIdx);

	DY_iter = m_RenderGroup[RENEDER_DYANAMIC_SHADOW].begin();
	for (; DY_iter != DY_iter_end; ++DY_iter)
	{
		(*DY_iter)->Render_Object(pDynamic_Effect, iPassIdx);
	}

	pDynamic_Effect->EndPass();
	pDynamic_Effect->End();

	m_eMRTID = MRT_DRAW_STATIC_SHADOWMAP;
	iPassIdx = 5;

	pStatic_Effect->Begin(NULL, 0);
	pStatic_Effect->BeginPass(iPassIdx);

	ST_iter = m_RenderGroup[RENDER_STATIC_SHADOW].begin();
	for (; ST_iter != ST_iter_end; ++ST_iter)
	{
		(*ST_iter)->Render_Object(pStatic_Effect, iPassIdx);
	}

	pStatic_Effect->EndPass();
	pStatic_Effect->End();
	
	End_MRT(L"MRT_CACD_Shadow_2");

	//=================================================

	Engine::Safe_Release(pDynamic_Effect);
	Engine::Safe_Release(pStatic_Effect);
	Engine::Safe_Release(pDynamic_Shader);
	Engine::Safe_Release(pStatic_Shader);
}

void CRenderer::Render_SSAO(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (m_bSSAO)
	{
		if (FALSE == m_bRandNormal)
		{
			m_bRandNormal = TRUE;

			m_pTexRandNormal = (CTexture*)Clone_Resource(2, L"Random_NomalMap");
		}

		Begin_MRT(L"MRT_SSAO");

		CShader* pShader = (CShader*)Clone_Prototype(L"Shader_SSAO");
		if (NULL == pShader) return;
		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
		if (NULL == pEffect) return;

		pEffect->AddRef();
		
		CCamera* CurCam = GetCurrentCamera();

		_float FarY = (_float)tan(CurCam->GetFovy() * 0.5f) * CurCam->GetCamFar();
		_float FarX = FarY * CurCam->GetAspect();

		_vec3 vCornerFrustum;
		vCornerFrustum.x = FarX;
		vCornerFrustum.y = FarY;
		vCornerFrustum.z = CurCam->GetCamFar();
		pEffect->SetVector("cornerFrustum", &_vec4(vCornerFrustum, 1.0f));

		pEffect->SetMatrix("g_matProj", &CurCam->GetMatProj());

		SetUp_OnShader(L"Target_DepthMap", pEffect, "DepthTexture");
		//SetUp_OnShader(L"Target_Normal", pEffect, "RandNoramlTexture");
		m_pTexRandNormal->Set_Texture(pEffect, "RandNoramlTexture");

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(0);
		m_pScreenTexture->Render_Buffer();
		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);
		Safe_Release(pShader);

		End_MRT(L"MRT_SSAO");
		m_bSSAO_Clear = FALSE;
	}
	else
	{
		if (FALSE == m_bSSAO_Clear)
		{
			m_bSSAO_Clear = TRUE;
			MRT_Clear(L"MRT_SSAO");
		}
	}
}

void CRenderer::Render_Dynamic(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_DynamicMesh");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	_uint iPassIdx = 0;
	switch (m_eMRTID)
	{
	case MRT_DEFERRED_1:
		pEffect->SetFloat("g_fBrightness", m_fBrightnessValue);
		iPassIdx = 0;
		break;

	case MRT_DEFERRED_2:
		iPassIdx = 1;
		break;

	case MRT_DEFERRED_4:
		SetUp_OnShader(L"Target_StageMap_DepthMap", pEffect, "g_Depth_Texture");
		iPassIdx = 5;
		break;

	case MRT_MOTION_BLUR:
		iPassIdx = 2;
		break;

	default:
		return;
	}

	pEffect->AddRef();

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);

	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_DYNAMIC].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_DYNAMIC].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object(pEffect, iPassIdx);
	}

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);

	iter = m_RenderGroup[RENDER_DYNAMIC].begin();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}
}

void CRenderer::Render_Static(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_StaticMesh");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	_uint iPassIdx = 0;
	switch (m_eMRTID)
	{
	case MRT_DEFERRED_1:
		pEffect->SetFloat("g_fBrightness", m_fBrightnessValue);
		iPassIdx = 0;
		break;

	case MRT_DEFERRED_2:
		iPassIdx = 1;
		break;

	case MRT_DEFERRED_3:
		iPassIdx = 4;
		break;

	case MRT_DEFERRED_4:
		SetUp_OnShader(L"Target_StageMap_DepthMap", pEffect, "g_Depth_Texture");
		iPassIdx = 6;
		break;

	case MRT_MINIMAP:
		iPassIdx = 3;
		break;

	default:
		return;
	}

	pEffect->AddRef();

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);

	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_STATIC].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_STATIC].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object(pEffect, iPassIdx);
	}

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);

	iter = m_RenderGroup[RENDER_STATIC].begin();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}
}

void CRenderer::Render_Decal(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_Effect");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	pEffect->AddRef();

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(5);

	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_DECAL].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_DECAL].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object(pEffect, 5);
		m_pScreenTexture->Render_Buffer();
	}

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);
}

void Engine::CRenderer::Render_NoneAlpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_NONALPHA].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_NONALPHA].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}
}

_bool Compare_Z(CGameObject* pDest, CGameObject* pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}

void Engine::CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_ALPHA].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_ALPHA].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void Engine::CRenderer::Render_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_UI].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_UI].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
void Engine::CRenderer::Render_Window(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	WINDOWUIMAP::iterator	iter = m_WindowUI.begin();
	WINDOWUIMAP::iterator	iter_end = m_WindowUI.end();

	for (; iter != iter_end; ++iter)
	{
		if(iter->second->GetLight())
		{
			pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
			pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
			pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		}
		iter->second->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_Deferred(LPDIRECT3DDEVICE9 pGraphicDev)
{
	//Shadow=========================================================
	if (m_bShadowBuffer || m_bSoftShadow)
	{
		Begin_MRT(L"MRT_Deferred_0");

		m_eMRTID = MRT_DEFERRED_0;
		Render_ShadowMap(pGraphicDev);
		//Render_Dynamic(pGraphicDev);
		//Render_Static(pGraphicDev);

		End_MRT(L"MRT_Deferred_0");
	}
	else if (m_bCACDShadow)
	{
		m_eMRTID = MRT_DEFERRED_0;
		Render_CACD_ShadowMap(pGraphicDev);
	}
	//================================================================

	//Albedo, DepthMap, Character_Sampler=============================
	Begin_MRT(L"MRT_Deferred_1");

	m_eMRTID = MRT_DEFERRED_1;
	//Render_NoneAlpha(pGraphicDev);
	Render_Dynamic(pGraphicDev);
	Render_Static(pGraphicDev);

	End_MRT(L"MRT_Deferred_1");
	//================================================================

	//Normal, Specular================================================
	Begin_MRT(L"MRT_Deferred_2");

	m_eMRTID = MRT_DEFERRED_2;
	//Render_NoneAlpha(pGraphicDev);
	Render_Dynamic(pGraphicDev);
	Render_Static(pGraphicDev);

	End_MRT(L"MRT_Deferred_2");
	//================================================================

	//StageMap Depth==================================================
	Begin_MRT(L"MRT_Deferred_3");

	m_eMRTID = MRT_DEFERRED_3;
	Render_Static(pGraphicDev);
	/*RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_STAGE_DEPTH].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_STAGE_DEPTH].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}*/

	End_MRT(L"MRT_Deferred_3");
	//================================================================

	//StageMap Depth==================================================
	/*Begin_MRT(L"MRT_Deferred_4");

	m_eMRTID = MRT_DEFERRED_4;
	Render_Dynamic(pGraphicDev);
	Render_Static(pGraphicDev);
	
	End_MRT(L"MRT_Deferred_4");*/
	//================================================================

	//Decal===========================================================
	Begin_MRT(L"MRT_Decal");

	m_eMRTID = MRT_DECAL;
	Render_Decal(pGraphicDev);

	End_MRT(L"MRT_Decal");
	//================================================================

	//Effect==========================================================
	Begin_MRT(L"MRT_Effect");

	m_eMRTID = MRT_EFFECT;
	Render_Alpha(pGraphicDev);

	End_MRT(L"MRT_Effect");
	//================================================================
}

void CRenderer::Render_LightAcc(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_Shade");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	pEffect->AddRef();

	float fFar = GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);	

	SetUp_OnShader(L"Target_Normal", pEffect, "g_NormalTexture");
	SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");

	pEffect->Begin(NULL, 0);

	//Shadow
	if (m_bShadowBuffer || m_bSoftShadow)
	{
		SetUp_OnShader(L"Target_ShadowMap", pEffect, "g_ShadowTexture");
	}

	//Directional Light======================================================
	Begin_MRT(L"MRT_Directional_Light");

	m_eMRTID = MRT_DIRECTIONAL_LIGHT;
	Render_Directional_Light(pGraphicDev, pEffect, 1);

	End_MRT(L"MRT_Directional_Light");
	//=======================================================================

	//Point Light============================================================
	Begin_MRT(L"MRT_Point_Light");

	m_eMRTID = MRT_POINT_LIGHT;
	Render_Point_Light(pGraphicDev, pEffect, 2);

	End_MRT(L"MRT_Point_Light");
	//=======================================================================

	//LgithAcc===============================================================
	Begin_MRT(L"MRT_LightAcc");
	
	m_eMRTID = MRT_SHADE;		

	SetUp_OnShader(L"Target_Directional_Light", pEffect, "g_DirectionalTexture");
	SetUp_OnShader(L"Target_Point_Light", pEffect, "g_PointTexture");
	//SetUp_OnShader(L"Target_Dynamic_Specular", pEffect, "g_Specular_Direc_Texture");
	SetUp_OnShader(L"Target_Point_Specular", pEffect, "g_Specular_Point_Texture");
	
	pEffect->BeginPass(3);
	m_pScreenTexture->Render_Buffer();
	pEffect->EndPass();
	pEffect->End();

	End_MRT(L"MRT_LightAcc");
	//=======================================================================
	
	if (m_bGaussianBlur)
	{
		//Blur========= Gaussian=================================================
		Begin_MRT(L"MRT_Light_Blur");
		
		m_eMRTID = MRT_LIGHT_BLUR;

		//Target_Shade
		//Target_All_Sepcular

		_int		iCnt = 0;
		_float		fPower = 0.0f;
		BLURTYPE	eBlurType = BLUR_END;

		switch (GetSceneID())
		{
		case SCENE_SELECT:
			iCnt = 4;
			fPower = 1.1f;
			eBlurType = BLUR_VERTICAL;
			break;

		case SCENE_TOWN:
			iCnt = 2;
			fPower = 1.1f;
			eBlurType = BLUR_VERTICAL;
			break;

		case SCENE_STAGE_ICE:
			iCnt = 4;
			fPower = 1.2f;
			eBlurType = BLUR_VERTICAL;
			break;

		case SCENE_STAGE_FIRE:
			iCnt = 4;
			fPower = 1.2f;
			eBlurType = BLUR_VERTICAL;
			break;
		}

		if(Render_Blur_Work(pEffect, iCnt, L"Target_All_Sepcular", fPower, -m_fBlurValue, eBlurType))
			SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BaseTexture");
		else
			SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BaseTexture");
				
		/*SetUp_OnShader(L"Target_All_Sepcular", pEffect, "g_SpecularTexture");

		pEffect->BeginPass(4);
		m_pScreent_Quad_Texture->Render_Buffer();
		pEffect->EndPass();

		SetUp_OnShader(L"Target_Light_Blur", pEffect, "g_SpecularTexture");
		pEffect->CommitChanges();*/

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(6);
		m_pScreent_Quad_Texture->Render_Buffer();
		pEffect->EndPass();
		pEffect->End();

		End_MRT(L"MRT_Light_Blur");
		//=======================================================================
		m_bGaussianClear = FALSE;
	}
	else
	{
		if (FALSE == m_bGaussianClear)
		{
			m_bGaussianClear = TRUE;
			MRT_Clear(L"MRT_Light_Blur");
		}		
	}
	
	Begin_NoClear_MRT(L"MRT_LightAcc");

	m_eMRTID = MRT_SHADE;
	
	if (SCENE_STAGE_ICE == GetSceneID() || SCENE_STAGE_FIRE == GetSceneID())
	{
		SetUp_OnShader(L"Target_Specular", pEffect, "g_BaseTexture");
	}
	else
	{
		SetUp_OnShader(L"Target_Dynamic_Specular", pEffect, "g_BaseTexture");
	}

	//SetUp_OnShader(L"Target_Specular", pEffect, "g_Blur_Texture");
	//SetUp_OnShader(L"Target_All_Sepcular", pEffect, "g_SpecularTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(15);
	m_pScreenTexture->Render_Buffer();
	pEffect->EndPass();
	pEffect->End();
	
	if (m_bLimLight)
	{
		SetUp_OnShader(L"Target_LimLight", pEffect, "g_BaseTexture");
		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(15);
		m_pScreenTexture->Render_Buffer();
		pEffect->EndPass();
		pEffect->End();
	}	

	End_MRT(L"MRT_LightAcc");
	
	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);
}

void CRenderer::Render_Outline(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_Outline");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	/*CShader* pShader_Shade = (CShader*)Clone_Prototype(L"Shader_Shade");
	if (NULL == pShader_Shade) return;
	LPD3DXEFFECT pEffect_Shade = pShader_Shade->Get_EffectHandle();
	if (NULL == pEffect_Shade) return;*/

	Begin_MRT(L"MRT_Outline");

	pEffect->AddRef();

	//pEffect->SetVector("g_vCamPosition", &_vec4(GetCurrentCamera()->GetEyePosition(), 0.0f));
	//pEffect->SetFloat("g_fFar", GetCurrentCamera()->GetCamFar());

	SetUp_OnShader(L"Target_Char_Outline", pEffect, "g_BaseTexture");
	//SetUp_OnShader(L"Target_Normal", pEffect, "g_NormalTexture");
	//SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");
	//SetUp_OnShader(L"Target_StageMap_DepthMap", pEffect, "g_MapDepthTexture");

	/*_matrix		matView, matProj;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	pEffect->SetMatrix("g_matViewInv", &matView);
	pEffect->SetMatrix("g_matProjInv", &matProj);*/

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);
	m_pScreenTexture-> Render_Buffer();
	pEffect->EndPass();
	pEffect->End();	

	/*SetUp_OnShader(L"Target_Outline", pEffect, "g_OutlineTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);
	m_pScreenTexture->Render_Buffer();
	pEffect->EndPass();
	pEffect->End();*/

	End_MRT(L"MRT_Outline");

	/*if(Render_Blur_Work(pEffect_Shade, 8, L"Target_Outline", 1.2f, 400.0f, BLUR_HORIZON))
		SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BlurTexture");
	else
		SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BlurTexture");

	Begin_MRT(L"MRT_Outline");

	SetUp_OnShader(L"Target_Char_Sampler", pEffect, "g_BaseTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(1);
	m_pScreenTexture->Render_Buffer();
	pEffect->EndPass();
	pEffect->End();

	End_MRT(L"MRT_Outline");
*/
	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);	
	//Engine::Safe_Release(pEffect_Shade);
	//Engine::Safe_Release(pShader_Shade);
}

void CRenderer::Render_Shadow(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (m_bShadowBuffer || m_bSoftShadow)
	{
		Begin_MRT(L"MRT_Shadow");

		m_eMRTID = MRT_SHADOW;

		CShader* pShader = (CShader*)Clone_Prototype(L"Shader_Shadow");
		if (NULL == pShader) return;
		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
		if (NULL == pEffect) return;

		pEffect->AddRef();

		SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");	//캐릭터 몸에도 음영이 지도록 한다.
		SetUp_OnShader(L"Target_StageMap_DepthMap", pEffect, "g_StageMap_DepthTexture");	//지형에만 음영이 지도록 한다.
		SetUp_OnShader(L"Target_ShadowMap", pEffect, "g_ShadowTexture");

		float fFar = GetCurrentCamera()->GetCamFar();
		pEffect->SetFloat("g_fFar", fFar);

		_matrix		matView, matProj;
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixInverse(&matProj, NULL, &matProj);

		pEffect->SetMatrix("g_matViewInv", &matView);
		pEffect->SetMatrix("g_matProjInv", &matProj);

		pEffect->SetMatrix("g_matLightView", &GetCamera(CAMERA_LIGHT)->GetMatView());
		pEffect->SetMatrix("g_matLightProj", &GetCamera(CAMERA_LIGHT)->GetMatProj());

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(1);

		m_pScreenTexture->Render_Buffer();

		pEffect->EndPass();

		if (m_bSoftShadow)
		{
			SetUp_OnShader(L"Target_Shadow", pEffect, "g_BaseTexture");

			pEffect->BeginPass(2);
			m_pScreenTexture->Render_Buffer();
			pEffect->EndPass();
		}

		pEffect->End();

		End_MRT(L"MRT_Shadow");

		Engine::Safe_Release(pEffect);
		Engine::Safe_Release(pShader);
	}
	else if (m_bCACDShadow)
	{
		Begin_MRT(L"MRT_Shadow");

		m_eMRTID = MRT_SHADOW;

		CShader* pShader = (CShader*)Clone_Prototype(L"Shader_Shadow");
		if (NULL == pShader) return;
		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
		if (NULL == pEffect) return;

		pEffect->AddRef();

		SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");	//캐릭터 몸에도 음영이 지도록 한다.
		SetUp_OnShader(L"Target_StageMap_DepthMap", pEffect, "g_StageMap_DepthTexture");	//지형에만 음영이 지도록 한다.

		float fFar = GetCurrentCamera()->GetCamFar();
		pEffect->SetFloat("g_fFar", fFar);

		SetUp_OnShader(L"Target_ShadowMap", pEffect, "g_CACD_Shadow_0_Texture");
		SetUp_OnShader(L"Target_ShadowMap_1", pEffect, "g_CACD_Shadow_1_Texture");
		SetUp_OnShader(L"Target_ShadowMap_2", pEffect, "g_CACD_Shadow_2_Texture");

		_matrix		matView, matProj;
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixInverse(&matProj, NULL, &matProj);

		pEffect->SetMatrix("g_matViewInv", &matView);
		pEffect->SetMatrix("g_matProjInv", &matProj);

		pEffect->SetMatrix("g_matLightView", &GetCamera(CAMERA_LIGHT)->GetMatView());

		pEffect->SetMatrix("g_matLightProj", &GetCamera(CAMERA_LIGHT)->Get_CACD_Proj(0));
		pEffect->SetMatrix("g_matLightProj_1", &GetCamera(CAMERA_LIGHT)->Get_CACD_Proj(1));
		pEffect->SetMatrix("g_matLightProj_2", &GetCamera(CAMERA_LIGHT)->Get_CACD_Proj(2));

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(3);

		m_pScreenTexture->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();

		End_MRT(L"MRT_Shadow");

		Engine::Safe_Release(pEffect);
		Engine::Safe_Release(pShader);
	}
}

void CRenderer::Render_Blend(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Begin_MRT(L"MRT_Blend");

	m_eMRTID = MRT_BLEND;
	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_Blend");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	pEffect->AddRef();

	SetUp_OnShader(L"Target_Albedo"		, pEffect, "g_AlbedoTexture");
	SetUp_OnShader(L"Target_Shade", pEffect, "g_ShadeTexture");
	SetUp_OnShader(L"Target_All_Sepcular", pEffect, "g_SpecularTexture");

	if (m_bGaussianBlur)
	{
		SetUp_OnShader(L"Target_Light_Blur", pEffect, "g_BlurTexture");
		pEffect->SetBool("m_bGaussianBlur", true);
	}
	else
		pEffect->SetBool("m_bGaussianBlur", false);

	//SSAO
	if (m_bSSAO)
	{
		SetUp_OnShader(L"Target_SSAO", pEffect, "g_SSAOTexture");
		pEffect->SetBool("g_bSSAO", true);
	}
	else
		pEffect->SetBool("g_bSSAO", false);
		
	SetUp_OnShader(L"Target_DepthMap"	, pEffect, "g_DepthTexture");
	SetUp_OnShader(L"Target_Effect"		, pEffect, "g_EffectTexture");
	
	_uint iPassIdx = 0;
	//Shadow
	if (m_bShadowBuffer || m_bSoftShadow || m_bCACDShadow)
	{
		SetUp_OnShader(L"Target_Shadow", pEffect, "g_ShadowTexture");

		//if (m_bHDR) iPassIdx = 4;
		iPassIdx = 4;
	}
	else
	{
		//iPassIdx = 1;
		//if (m_bHDR) iPassIdx = 5;
		iPassIdx = 5;
	}

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);
	m_pScreenTexture->Render_Buffer();
	pEffect->EndPass();

	End_MRT(L"MRT_Blend");

	if(m_bMotionBlurBuffer)
	{
		SetUp_OnShader(L"Target_Char_Sampler", pEffect, "g_CharTexture");

		//블러를 위한 샘플러에다 노말과 스펙큘러를 매핑하여 퀄을 상승시킨다.
		Begin_MRT(L"MRT_Char_Sampler_Shade");
		iPassIdx = 2;
		pEffect->BeginPass(iPassIdx);
		m_pScreenTexture->Render_Buffer();
		pEffect->EndPass();
		End_MRT(L"MRT_Char_Sampler_Shade");
	}

	pEffect->End();

	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);

	if (m_bHDR)
	{
		Begin_MRT(L"MRT_HDR");

		pShader = (CShader*)Clone_Prototype(L"Shader_HDR");
		if (NULL == pShader) return;
		pEffect = pShader->Get_EffectHandle();
		if (NULL == pEffect) return;

		pEffect->AddRef();

		//SetUp_OnShader(L"Target_All_Sepcular", pEffect, "g_BlurTexture");
		SetUp_OnShader(L"Target_Blend", pEffect, "g_BaseTexture");
				
		/*pEffect->SetFloat("A", m_fA);
		pEffect->SetFloat("B", m_fB);
		pEffect->SetFloat("C", m_fC);
		pEffect->SetFloat("D", m_fD);*/

		switch (GetSceneID())
		{
		case SCENE_SELECT:
			pEffect->SetFloat("g_fLum_Avg", -0.15f);
			break;

		case SCENE_TOWN:
			pEffect->SetFloat("g_fLum_Avg", -0.08f);
			break;

		case SCENE_STAGE_ICE:
			pEffect->SetFloat("g_fLum_Avg", -0.126f);
			break;

		case SCENE_STAGE_FIRE:
			pEffect->SetFloat("g_fLum_Avg", -0.13f);
			break;
		}
		//pEffect->SetFloat("g_fLum_Avg", m_fLum_Average);
		//Town = -0.08f
		//ICE = -0.126f

		pEffect->Begin(NULL, 0);

		pEffect->BeginPass(0);
		m_pScreenTexture->Render_Buffer();
		pEffect->EndPass();
		pEffect->End();

		End_MRT(L"MRT_HDR");

		/*Begin_MRT(L"MRT_Char_Sampler_Shade_HDR");

		SetUp_OnShader(L"Target_Char_Sampler_Shade", pEffect, "g_BaseTexture");
		pEffect->SetFloat("g_fLum_Avg", m_fLum_Average);
		pEffect->CommitChanges();

		pEffect->BeginPass(3);
		m_pScreenTexture->Render_Buffer();
		pEffect->EndPass();

		pEffect->End();

		End_MRT(L"MRT_Char_Sampler_Shade_HDR");*/

		Engine::Safe_Release(pEffect);
		Engine::Safe_Release(pShader);
	}
}

void CRenderer::Render_DOF(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (m_bDOF)
	{
		CShader* pShader = (CShader*)Clone_Prototype(L"Shader_DOF");
		if (NULL == pShader) return;
		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
		if (NULL == pEffect) return;

		CShader* pShader_Shade = (CShader*)Clone_Prototype(L"Shader_Shade");
		if (NULL == pShader_Shade) return;
		LPD3DXEFFECT pEffect_Shade = pShader_Shade->Get_EffectHandle();
		if (NULL == pEffect_Shade) return;

		pEffect->AddRef();

		if (m_bHDR)
			SetUp_OnShader(L"Target_HDR", pEffect, "g_BaseTexture");
		else
			SetUp_OnShader(L"Target_Blend", pEffect, "g_BaseTexture");

		//pEffect->SetFloat("fFocus", m_fDOF_Focus);
		//pEffect->SetFloat("fRange", m_fDOF_Range);

		pEffect->SetFloat("g_fFar", GetCurrentCamera()->GetCamFar());

		//float fNear = GetCurrentCamera()->GetNear();
		//float fAt = GetCurrentCamera()->GetAtPosition().z;
		//float fFar = GetCurrentCamera()->GetCamFar();

		//_vec4 vDofParams = _vec4(fNear, fAt, fFar, (fFar - 20.0f));

		//pEffect->SetVector("vDofParams", &vDofParams);
		//pEffect->SetFloat("fDofFocus", fAt);

		SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");

		//Begin_MRT(L"MRT_DOF_BLUR");

		//pEffect->Begin(NULL, 0);
		//pEffect->BeginPass(0);
		//m_pScreent_Quad_Texture->Render_Buffer();
		//pEffect->EndPass();

		//End_MRT(L"MRT_DOF_BLUR");

		if (m_bHDR)
		{
			if (Render_Blur_Work(pEffect_Shade, 4, L"Target_HDR", 1.01f, 450.0f, BLUR_STAR))
				SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BlendBlurTexture");
			else
				SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BlendBlurTexture");
		}
		else
		{
			if (Render_Blur_Work(pEffect_Shade, 4, L"Target_Blend", 1.01f, 450.0f, BLUR_STAR))
				SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BlendBlurTexture");
			else
				SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BlendBlurTexture");
		}
		
		Begin_MRT(L"MRT_DOF");
		
		//SetUp_OnShader(L"Target_DOF_BLUR", pEffect, "g_BlendBlurTexture");
		float fDOF_Range = 0.0210f - (GetCurrentCamera()->Get_Gap() * 0.001f);
		if (fDOF_Range < 0) fDOF_Range = 0.0f;
		pEffect->SetFloat("fRange", fDOF_Range);

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(1);
		m_pScreenTexture->Render_Buffer();
		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
		Engine::Safe_Release(pShader);
		Engine::Safe_Release(pShader_Shade);

		End_MRT(L"MRT_DOF");
	}	
}

void CRenderer::Render_Final(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Begin_MRT(L"MRT_Final", 1);

	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_Final");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	pEffect->AddRef();

	if(m_bHDR && m_bDOF)
		SetUp_OnShader(L"Target_DOF", pEffect, "g_BaseTexture");
	else if (m_bHDR)
		SetUp_OnShader(L"Target_HDR", pEffect, "g_BaseTexture");
	else if(m_bDOF)
		SetUp_OnShader(L"Target_DOF", pEffect, "g_BaseTexture");
	else
		SetUp_OnShader(L"Target_Blend", pEffect, "g_BaseTexture");

	pEffect->SetFloat("g_fCamFar", GetCurrentCamera()->GetCamFar());
	SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");
	SetUp_OnShader(L"Target_Decal", pEffect, "g_DecalTexture");
	SetUp_OnShader(L"Target_Outline", pEffect, "g_OutlineTexture");

	_uint iPassIdx = 0;

	/*if (m_bExpFog && m_bHeightFog)
	{
		_matrix		matView, matProj;
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixInverse(&matProj, NULL, &matProj);

		pEffect->SetMatrix("g_matViewInv", &matView);
		pEffect->SetMatrix("g_matProjInv", &matProj);

		pEffect->SetVector("g_vCamPosition", &_vec4(GetCurrentCamera()->GetEyePosition(), 0.0f));

		iPassIdx = 4;
	}
	else if (m_bSquareFog && m_bHeightFog)
	{
		_matrix		matView, matProj;
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixInverse(&matProj, NULL, &matProj);

		pEffect->SetMatrix("g_matViewInv", &matView);
		pEffect->SetMatrix("g_matProjInv", &matProj);

		pEffect->SetVector("g_vCamPosition", &_vec4(GetCurrentCamera()->GetEyePosition(), 0.0f));

		iPassIdx = 5;
	}
	else */if (m_bExpFog)
	{
		switch (GetSceneID())
		{
		case SCENE_SELECT:
			pEffect->SetVector("g_vColorFog", &_vec4(0.2f, 0.2f, 0.2f, 0.0f));			
			break;

		case SCENE_TOWN:
			pEffect->SetVector("g_vColorFog", &_vec4(0.2f, 0.2f, 0.2f, 0.0f));
			break;

		case SCENE_STAGE_ICE:
			pEffect->SetVector("g_vColorFog", &_vec4(0.4f, 0.4f, 0.4f, 0.0f));
			break;

		case SCENE_STAGE_FIRE:
			pEffect->SetVector("g_vColorFog", &_vec4(0.4f, 0.4f, 0.4f, 0.0f));
			break;
		}

		iPassIdx = 1;
	}
	else if (m_bSquareFog)
	{
		switch (GetSceneID())
		{
		case SCENE_SELECT:
			pEffect->SetVector("g_vColorFog", &_vec4(0.2f, 0.2f, 0.2f, 0.0f));
			break;

		case SCENE_TOWN:
			pEffect->SetVector("g_vColorFog", &_vec4(0.2f, 0.2f, 0.2f, 0.0f));
			break;

		case SCENE_STAGE_ICE:
			pEffect->SetVector("g_vColorFog", &_vec4(0.4f, 0.4f, 0.4f, 0.0f));
			break;

		case SCENE_STAGE_FIRE:
			pEffect->SetVector("g_vColorFog", &_vec4(0.4f, 0.4f, 0.4f, 0.0f));
			break;
		}

		iPassIdx = 2;
	}
	else if (m_bHeightFog)
	{
		switch (GetSceneID())
		{
		case SCENE_SELECT:
			pEffect->SetVector("g_vColorFog", &_vec4(0.2f, 0.2f, 0.2f, 0.0f));
			break;

		case SCENE_TOWN:
			pEffect->SetVector("g_vColorFog", &_vec4(0.2f, 0.2f, 0.2f, 0.0f));
			break;

		case SCENE_STAGE_ICE:
			pEffect->SetVector("g_vColorFog", &_vec4(0.5607843f * 0.1f, 0.0f, 0.8274509803f * 0.1f, 0.0f));
			pEffect->SetVector("g_vHeightFog", &_vec4(0.275f, 1000.0f, -10.0f, 0.0f));
			break;

		case SCENE_STAGE_FIRE:
			pEffect->SetVector("g_vColorFog", &_vec4(0.73725f * 0.1f, 0.5607843f * 0.1f, 0.5607843f * 0.1f, 0.0f));
			pEffect->SetVector("g_vHeightFog", &_vec4(0.275f, 1000.0f, -50.0f, 0.0f));
			break;
		}

		_matrix		matView, matProj;
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, NULL, &matView);

		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixInverse(&matProj, NULL, &matProj);

		pEffect->SetMatrix("g_matViewInv", &matView);
		pEffect->SetMatrix("g_matProjInv", &matProj);

		pEffect->SetVector("g_vCamPosition", &_vec4(GetCurrentCamera()->GetEyePosition(), 0.0f));

		iPassIdx = 3;
	}
	else					
		iPassIdx = 0;
	
	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);
	m_pScreenTexture->Render_Buffer();
	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);

	End_MRT(L"MRT_Final", 1);
}

void CRenderer::Render_RadialBlur(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (m_bRadialBlur)
	{
		CShader* pShader = (CShader*)Clone_Prototype(L"Shader_RadialBlur");
		if (NULL == pShader) return;
		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
		if (NULL == pEffect) return;

		pEffect->AddRef();

		SetUp_OnShader(L"Target_Final", pEffect, "g_BaseTexture");

		pEffect->SetFloat("fBlurStar", 1.0f);
		pEffect->SetFloat("fBlurWidth", m_fRadialBlur_Width);//-0.25f);
		pEffect->SetInt("iNsamples", 10);

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(0);
		m_pScreenTexture->Render_Buffer();
		pEffect->EndPass();
		pEffect->End();

		Engine::Safe_Release(pEffect);
		Engine::Safe_Release(pShader);
	}
}

void CRenderer::Render_MotionBlur(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (m_bMotionBlurBuffer)
	{
		CShader* pShader = (CShader*)Clone_Prototype(L"Shader_DynamicMesh");
		if (NULL == pShader) return;
		LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
		if (NULL == pEffect) return;

		pEffect->AddRef();

		_uint iPassIdx = 2;

		Begin_MRT(L"MRT_Motion_Blur");

		m_eMRTID = MRT_MOTION_BLUR;

		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(iPassIdx);

		RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_MOTION_BLUR].begin();
		RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_MOTION_BLUR].end();

		for (; iter != iter_end; ++iter)
		{
			(*iter)->Render_Object(pEffect, iPassIdx);
		}

		pEffect->EndPass();
		pEffect->End();

		End_MRT(L"MRT_Motion_Blur");

		Safe_Release(pEffect);
		Safe_Release(pShader);

		pShader = (Engine::CShader*)Clone_Prototype(L"Shader_Blend");
		if (NULL == pShader) return;
		pEffect = pShader->Get_EffectHandle();
		if (NULL == pEffect) return;

		pEffect->AddRef();

		iPassIdx = 3;
		SetUp_OnShader(L"Target_Char_Sampler", pEffect, "g_CharTexture");
		SetUp_OnShader(L"Target_Motion_Blur", pEffect, "g_MotionBlurTexture");
		pEffect->Begin(NULL, 0);
		pEffect->BeginPass(iPassIdx);
		m_pScreenTexture->Render_Buffer();
		pEffect->EndPass();
		pEffect->End();

		Safe_Release(pEffect);
		Safe_Release(pShader);
	}	
}

void CRenderer::Render_PostEffect(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_Effect");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	CShader* pShader_Shade = (CShader*)Clone_Prototype(L"Shader_Shade");
	if (NULL == pShader_Shade) return;
	LPD3DXEFFECT pEffect_Shade = pShader_Shade->Get_EffectHandle();
	if (NULL == pEffect_Shade) return;

	MRT_Clear(L"MRT_All_PostEffect_Blur");
	//MRT_Clear(L"MRT_All_PostEffect_Blur_Depth");

	pEffect->AddRef();
	pEffect_Shade->AddRef();

	Begin_MRT(L"MRT_PostEffect");

	m_eMRTID = MRT_POSTEFFECT;

	RENDERGROUP::iterator iter = m_RenderGroup[RENDER_POSTEFFECT].begin();
	RENDERGROUP::iterator iter_end = m_RenderGroup[RENDER_POSTEFFECT].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}

	End_MRT(L"MRT_PostEffect");

	iter = m_RenderGroup[RENDER_POSTEFFECT_BLUR].begin();
	iter_end = m_RenderGroup[RENDER_POSTEFFECT_BLUR].end();

	m_eMRTID = MRT_POSTEFFECT_BLUR;

	for (; iter != iter_end; ++iter)
	{
		Begin_MRT(L"MRT_PostEffect_Blur");
		pEffect->Begin(NULL, 0);
		(*iter)->Render_Object(pEffect, 0);
		pEffect->End();
		End_MRT(L"MRT_PostEffect_Blur");
		
		Begin_NoClear_MRT(L"MRT_All_PostEffect_Blur");

		if (Render_Blur_Work(pEffect_Shade, (*iter)->GetBlurCnt(), L"Target_PostEffect_Blur", (*iter)->GetBlurPower(), (*iter)->GetBlurValue(), (*iter)->GetBlurType()))
			SetUp_OnShader(L"Target_Light_Blur_A", pEffect_Shade, "g_BaseTexture");
		else
			SetUp_OnShader(L"Target_Light_Blur_B", pEffect_Shade, "g_BaseTexture");		

		pEffect_Shade->Begin(NULL, 0);
		pEffect_Shade->BeginPass(6);
		m_pScreent_Quad_Texture->Render_Buffer();
		pEffect_Shade->EndPass();	
		pEffect_Shade->End();

		End_MRT(L"MRT_All_PostEffect_Blur");

		/*Begin_NoClear_MRT(L"MRT_All_PostEffect_Blur");

		if (Render_Blur_Depth_Work(pEffect_Shade, (*iter)->GetBlurCnt(), L"Target_PostEffect_Blur", (*iter)->GetBlurPower(), (*iter)->GetBlurValue(), (*iter)->GetBlurType()))
			SetUp_OnShader(L"Target_Light_BlurDepth_A", pEffect_Shade, "g_BaseTexture");
		else
			SetUp_OnShader(L"Target_Light_BlurDepth_B", pEffect_Shade, "g_BaseTexture");

		pEffect_Shade->Begin(NULL, 0);
		pEffect_Shade->BeginPass(6);
		m_pScreent_Quad_Texture->Render_Buffer();
		pEffect_Shade->EndPass();
		pEffect_Shade->End();

		End_MRT(L"MRT_All_PostEffect_Blur");*/
	}
				
	Safe_Release(pEffect);
	Safe_Release(pShader);

	//===================================================================

	//pShader = (CShader*)Clone_Prototype(L"Shader_Shade");
	//if (NULL == pShader) return;
	//pEffect = pShader->Get_EffectHandle();
	//if (NULL == pEffect) return;

	//pEffect->AddRef();

	//if (Render_Blur_Work(pEffect, 8, L"Target_PostEffect_Blur", 1.2f, 128.0f))
	//	SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BaseTexture");
	//else
	//	SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BaseTexture");

	//Begin_MRT(L"MRT_PostEffect_Blur");

	//pEffect->Begin(NULL, 0);
	//pEffect->BeginPass(6);
	//m_pScreent_Quad_Texture->Render_Buffer();
	//pEffect->EndPass();
	//pEffect->End();

	//End_MRT(L"MRT_PostEffect_Blur");
	
	SetUp_OnShader(L"Target_PostEffect", pEffect_Shade, "g_BaseTexture");
	SetUp_OnShader(L"Target_All_PostEffect_Blur", pEffect_Shade, "g_Blur_Texture");
	//SetUp_OnShader(L"Target_All_PostEffect_Blur_Depth", pEffect_Shade, "g_BlurDepth_All_Texture");
	//SetUp_OnShader(L"Target_DepthMap", pEffect, "g_DepthTexture");

	pEffect_Shade->Begin(NULL, 0);
	pEffect_Shade->BeginPass(7);
	m_pScreenTexture->Render_Buffer();
	pEffect_Shade->EndPass();
	pEffect_Shade->End();

	Safe_Release(pEffect_Shade);
	Safe_Release(pShader_Shade);
}

void CRenderer::Render_MiniMap(LPDIRECT3DDEVICE9 pGraphicDev)
{
	/*CShader* pShader = (CShader*)Clone_Prototype(L"Shader_StaticMesh");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;*/

	CShader* pShader = (CShader*)Clone_Prototype(L"Shader_MiniMap");
	if (NULL == pShader) return;
	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;

	pEffect->AddRef();

	_uint iPassIdx = 1;

	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_MINIMAP].begin();

	Begin_MRT(L"MRT_MiniMap");

	pEffect->AddRef();

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);
	
	m_eMRTID = MRT_MINIMAP;
	if(m_RenderGroup[RENDER_MINIMAP].end() != iter)
		(*iter)->Render_Object(pEffect, iPassIdx);

	pEffect->EndPass();
	pEffect->End();

	End_MRT(L"MRT_MiniMap");
	
	//Engine::Safe_Release(pEffect);

	/*pShader = (CShader*)Clone_Prototype(L"Shader_MiniMap");
	if (NULL == pShader) return;
	pEffect = pShader->Get_EffectHandle();
	if (NULL == pEffect) return;*/
	
	//pEffect->AddRef();

	m_eMRTID = MRT_MINIMAP_DRAW;

	iPassIdx = 0;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(iPassIdx);

	if (m_RenderGroup[RENDER_MINIMAP].end() != iter)
		(*iter)->Render_Object(pEffect, 0);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);
	Engine::Safe_Release(pShader);
}

bool CRenderer::Render_Blur_Work(LPD3DXEFFECT pEffect, _int iCnt, const _tchar* pTargetTag, _float fBlurPower, _float fBlurValue, BLURTYPE eBlurType)
{
	pEffect->AddRef();

	SetUp_OnShader(pTargetTag, pEffect, "g_BaseTexture");	

	pEffect->Begin(NULL, 0);

	Begin_MRT(L"MRT_Light_Blur_B");

	pEffect->BeginPass(6);
	m_pScreent_Quad_Texture->Render_Buffer();
	pEffect->EndPass();

	End_MRT(L"MRT_Light_Blur_B");	
	
	bool m_bSort = FALSE;
	switch (eBlurType)
	{
	case BLUR_HORIZON:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 2)
			{
			case 0:
				Begin_MRT(L"MRT_Light_Blur_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(5);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"MRT_Light_Blur_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(5);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;

	case BLUR_VERTICAL:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 2)
			{
			case 0:
				Begin_MRT(L"MRT_Light_Blur_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(4);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"MRT_Light_Blur_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(4);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;

	case BLUR_CROSS:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 2)
			{
			case 0:
				Begin_MRT(L"MRT_Light_Blur_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(5);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"MRT_Light_Blur_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(4);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;

	case BLUR_X:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 2)
			{
			case 0:
				Begin_MRT(L"MRT_Light_Blur_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(8);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"MRT_Light_Blur_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(9);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;

	case BLUR_STAR:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 4)
			{
			case 0:
				Begin_MRT(L"MRT_Light_Blur_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(5);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"MRT_Light_Blur_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(4);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_B");
				m_bSort = FALSE;
				break;

			case 2:
				Begin_MRT(L"MRT_Light_Blur_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(8);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_A");
				m_bSort = TRUE;
				break;

			case 3:
				Begin_MRT(L"MRT_Light_Blur_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_Blur_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(9);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"MRT_Light_Blur_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;
	}	

	pEffect->End();

	Safe_Release(pEffect);

	return m_bSort;
}

bool CRenderer::Render_Blur_Depth_Work(LPD3DXEFFECT pEffect, _int iCnt, const _tchar * pTargetTag, _float fBlurPower, _float fBlurValue, BLURTYPE eBlurType)
{
	pEffect->AddRef();
		
	SetUp_OnShader(pTargetTag, pEffect, "g_BaseTexture");

	pEffect->Begin(NULL, 0);

	Begin_MRT(L"Target_Light_BlurDepth_B");

	pEffect->BeginPass(6);
	m_pScreent_Quad_Texture->Render_Buffer();
	pEffect->EndPass();

	End_MRT(L"Target_Light_BlurDepth_B");

	bool m_bSort = FALSE;
	switch (eBlurType)
	{
	case BLUR_HORIZON:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 2)
			{
			case 0:
				Begin_MRT(L"Target_Light_BlurDepth_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_B", pEffect, "g_BaseTexture");
				//SetUp_OnShader(L"Target_Dynamic_DepthMap", pEffect, "g_DepthTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(11);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"Target_Light_BlurDepth_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_A", pEffect, "g_BaseTexture");
				//SetUp_OnShader(L"Target_Dynamic_DepthMap", pEffect, "g_DepthTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(11);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;

	case BLUR_VERTICAL:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 2)
			{
			case 0:
				Begin_MRT(L"Target_Light_BlurDepth_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_B", pEffect, "g_BaseTexture");
				//SetUp_OnShader(L"Target_Dynamic_DepthMap", pEffect, "g_DepthTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(10);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"Target_Light_BlurDepth_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_A", pEffect, "g_BaseTexture");
				//SetUp_OnShader(L"Target_Dynamic_DepthMap", pEffect, "g_DepthTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(10);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;

	case BLUR_CROSS:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 2)
			{
			case 0:
				Begin_MRT(L"Target_Light_BlurDepth_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(11);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"Target_Light_BlurDepth_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(10);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;

	case BLUR_X:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 2)
			{
			case 0:
				Begin_MRT(L"Target_Light_BlurDepth_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(12);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"Target_Light_BlurDepth_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(13);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;

	case BLUR_STAR:
		for (int i = 0; i < iCnt; ++i)
		{
			switch (i % 4)
			{
			case 0:
				Begin_MRT(L"Target_Light_BlurDepth_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(11);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_A");
				m_bSort = TRUE;
				break;

			case 1:
				Begin_MRT(L"Target_Light_BlurDepth_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(10);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_B");
				m_bSort = FALSE;
				break;

			case 2:
				Begin_MRT(L"Target_Light_BlurDepth_A");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_B", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(12);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_A");
				m_bSort = TRUE;
				break;

			case 3:
				Begin_MRT(L"Target_Light_BlurDepth_B");

				pEffect->SetFloat("g_fBlurPower", fBlurPower);
				pEffect->SetFloat("g_fBlurValue", fBlurValue);
				SetUp_OnShader(L"Target_Light_BlurDepth_A", pEffect, "g_BaseTexture");
				pEffect->CommitChanges();

				pEffect->BeginPass(13);
				m_pScreent_Quad_Texture->Render_Buffer();
				pEffect->EndPass();

				End_MRT(L"Target_Light_BlurDepth_B");
				m_bSort = FALSE;
				break;
			}
		}
		break;
	}

	pEffect->End();

	Safe_Release(pEffect);

	return m_bSort;
}

void Engine::CRenderer::Render_Cursor(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	RENDERGROUP::iterator	iter = m_RenderGroup[RENDER_CURSOR].begin();
	RENDERGROUP::iterator	iter_end = m_RenderGroup[RENDER_CURSOR].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render_Object();
	}

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CRenderer::CreateScreenTexture(LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DVIEWPORT9			ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	m_pScreenTexture = CScreen_Texture::Create(pGraphicDev, 0, 0, ViewPort.Width, ViewPort.Height);
	m_pScreent_Quad_Texture = CScreen_Texture::Create(pGraphicDev, 0, 0, _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f));

	if (NULL == m_pScreenTexture)
		return E_FAIL;

	return S_OK;
}

void Engine::CRenderer::Free(void)
{
	if (PROJECT_CLIENT == m_eProjectID)
	{
		Engine::Safe_Release(m_pScreenTexture);
		Engine::Safe_Release(m_pScreent_Quad_Texture);
	}

	Clear_RenderGroup();
}


