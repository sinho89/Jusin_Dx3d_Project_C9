#include "Management.h"
#include "Renderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "Shader.h"
#include "Prototype_Manager.h"
#include "Target_Manager.h"
#include "Light_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
: m_pRenderer(CRenderer::GetInstance())
, m_pCurrentScene(NULL)
{
	m_pRenderer->AddRef();
}

Engine::CManagement::~CManagement(void)
{

}

CGameObject* Engine::CManagement::FindWithTag(const _tchar* pObjKey)
{
	Engine::CGameObject* pGameObject = NULL;
	pGameObject = m_pCurrentScene->FindWithTag(pObjKey);

	if(pGameObject != NULL)
		pGameObject->AddRef();

	return pGameObject;
}

CGameObject* Engine::CManagement::Find(const _tchar* pObjKey)
{
	Engine::CGameObject* pGameObject = NULL;
	pGameObject = m_pCurrentScene->Find(pObjKey);

	if(pGameObject != NULL)
		pGameObject->AddRef();

	return pGameObject;
}

list<CGameObject*>* Engine::CManagement::FindList(const _tchar* pObjKey)
{
	return m_pCurrentScene->FindList(pObjKey);
}

Engine::CLayer* Engine::CManagement::FindLayer(WORD eLayerID)
{
	return m_pCurrentScene->FindLayer(eLayerID);
}

_bool CManagement::Get_Delete(void)
{
	if (m_pCurrentScene == NULL)
		return false;

	return m_pCurrentScene->Get_Delete();
}

PSRWLOCK CManagement::Get_Lock(PSRWLOCK * pLock)
{
	if (m_pCurrentScene == NULL)
		return NULL;

	return m_pCurrentScene->Get_Lock(pLock);
}

void Engine::CManagement::AddObject(const WORD& wLayerID, const _tchar* pObjectKey, CGameObject* pGameObject)
{
	if(pGameObject == NULL)
		return;

	m_pCurrentScene->AddObject(wLayerID, pObjectKey, pGameObject);
}

HRESULT Engine::CManagement::Ready_Management_ForClient(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader*		pShader = NULL;

	// For.Sample Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_UI.hpp");
	if(NULL == pShader)	return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_UI", pShader)))		return E_FAIL;

	// For.Sample Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_MiniMap.hpp");
	if(NULL == pShader)	return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_MiniMap", pShader)))	return E_FAIL;

	//For.Player Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Player.hpp");
	if(NULL == pShader) return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Player", pShader)))	return E_FAIL;

	//For.Map Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Object.hpp");
	if(NULL == pShader) return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Object", pShader)))	return E_FAIL;
	
	//For.Particle Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Particle.hpp");
	if(NULL == pShader) return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Particle", pShader))) return E_FAIL;

	//For.DynamicMesh Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_DynamicMesh.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_DynamicMesh", pShader))) return E_FAIL;

	//For.StaticMesh Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_StaticMesh.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_StaticMesh", pShader))) return E_FAIL;

	//For. Effect
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Effect.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Effect", pShader))) return E_FAIL;

	//For. World UI
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_WorldUI.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_WorldUI", pShader))) return E_FAIL;

	//For.Shade Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Shade.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Shade", pShader))) return E_FAIL;

	//For.Blend Shaderddd aaaaa wd  a  as w d wwwwwwwwwwwwwwwwwwwwwwwwwwwwwa    
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Blend.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Blend", pShader))) return E_FAIL;
	
	//For.Trail Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Trail.hpp");
	if(NULL == pShader) return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Trail", pShader))) return E_FAIL;



	//For.EffectParticle Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_ParticleEffect.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_ParticleEffect", pShader))) return E_FAIL;



	//For.Reflect Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Reflect.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Reflect", pShader))) return E_FAIL;

	
	//For.Shadow Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Shadow.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Shadow", pShader))) return E_FAIL;

	//For.HDR Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_HDR.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_HDR", pShader))) return E_FAIL;

	//For.SSAO Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_SSAO.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_SSAO", pShader))) return E_FAIL;

	//For.DOF Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_DOF.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_DOF", pShader))) return E_FAIL;

	//For.Final Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Final.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Final", pShader))) return E_FAIL;

	//For.Outline Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_Outline.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Outline", pShader))) return E_FAIL;

	//For.RadialBlur Shader
	pShader = CShader::Create(pGraphicDev, L"../../Reference/Headers/Shader_RadialBlur.hpp");
	if (NULL == pShader) return E_FAIL;

	if (FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_RadialBlur", pShader))) return E_FAIL;

	CRenderer::GetInstance()->CreateScreenTexture(pGraphicDev);

	if (FAILED(Ready_For_RenderTarget(pGraphicDev))) return E_FAIL;	//렌더타겟 추가 / 디퍼드 쉐이딩을 위함.

	if (FAILED(CLight_Manager::GetInstance()->Ready_ScreenBuffer(pGraphicDev))) return E_FAIL;

	return S_OK;
}

HRESULT CManagement::Ready_For_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev)
{
	D3DVIEWPORT9	ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	//for. RenderTarget : Shadow_DepthMap
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_ShadowMap", ViewPort.Width, ViewPort.Height, D3DFMT_R32F, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), R_TARGET_SHADOWMAP)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_ShadowMap", 1280, 0, 320, 180)))return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_ShadowMap_1", ViewPort.Width, ViewPort.Height, D3DFMT_R32F, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), R_TARGET_SHADOWMAP_1)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_ShadowMap_1", 1280, 180, 320, 180)))return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_ShadowMap_2", ViewPort.Width, ViewPort.Height, D3DFMT_R32F, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), R_TARGET_SHADOWMAP_2)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_ShadowMap_2", 1280, 360, 320, 180)))return E_FAIL;

	//For. Multi Render Target(MRT) : MRT_Deferred_0
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_0", L"Target_ShadowMap")))		return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_CACD_Shadow_1", L"Target_ShadowMap_1")))		return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_CACD_Shadow_2", L"Target_ShadowMap_2")))		return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================
	//for. RenderTarget : Albedo
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Albedo", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_ALBEDO)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Albedo", 0, 0, 320, 180)))	return E_FAIL;
	
	//for. RenderTarget : DepthMap
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_DepthMap", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), R_TARGET_DEPTH)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_DepthMap", 0, 720, 320, 180)))return E_FAIL;

	//For. RenderTarget : Character Sampler
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Char_Sampler", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_CHAR_SAMPLER)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Char_Sampler", 320, 720, 320, 180)))	return E_FAIL;

	//For. RenderTarget : Outline Sampler
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Char_Outline", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_OUTLINE_CHAR)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Char_Outline", 960, 180, 320, 180)))	return E_FAIL;

	//For. Multi Render Target(MRT) : MRT_Deferred_1
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_1", L"Target_Albedo")))			return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_1", L"Target_DepthMap")))		return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_1", L"Target_Char_Sampler")))	return E_FAIL;	
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_1", L"Target_Char_Outline")))	return E_FAIL;
	
	//=====================================================================================================================
	//=====================================================================================================================
	
	//for. RenderTarget : Specular
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_SPECULAR)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Specular", 0, 180, 320, 180)))return E_FAIL;

	//for. RenderTarget : Noraml
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Normal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_NORMAL)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Normal", 0, 360, 320, 180)))	return E_FAIL;

	//for. RenderTarget : Dynamic_Specular
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Dynamic_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_DYNAMIC_SPECULAR)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Dynamic_Specular", 0, 540, 320, 180)))return E_FAIL;
	
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_LimLight", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_LIMLIGHT)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_LimLight", 1280, 720, 320, 180)))	return E_FAIL;

	//For. Multi Render Target(MRT) : MRT_Deferred_2
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_2", L"Target_Normal")))			return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_2", L"Target_Specular")))		return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_2", L"Target_Dynamic_Specular")))		return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_2", L"Target_LimLight")))		return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================
	//For.Only StageMap Depth
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_StageMap_DepthMap", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), R_TARGET_STAGEMAP_DEPTH)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_StageMap_DepthMap", 320, 720, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_3", L"Target_StageMap_DepthMap")))	return E_FAIL;
	
	//======================================================================================================================
	//======================================================================================================================
	
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Deferred_4", L"Target_Char_Outline")))	return E_FAIL;

	//======================================================================================================================
	//======================================================================================================================

	//For.Decal
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Decal", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_DECAL)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Decal", 320, 180, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Decal", L"Target_Decal")))	return E_FAIL;

	//======================================================================================================================
	//======================================================================================================================

	//For.SSAO
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_SSAO", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), R_TARGET_SSAO)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_SSAO", 960, 0, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_SSAO", L"Target_SSAO")))	return E_FAIL;

	//======================================================================================================================
	//======================================================================================================================

	//For. RenderTarget : Effect
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Effect", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), R_TARGET_EFFECT)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Effect", 320, 540, 320, 180)))	return E_FAIL;

	//For. Multi Render Target(MRT) : MRT_Effect
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Effect", L"Target_Effect")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================
	//For. RenderTarget : Directional Light
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Directional_Light", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), R_TARGET_DIRECTIONAL_LIGHT)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Directional_Light", 320, 0, 320, 180)))	return E_FAIL;
	
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Directional_Specular", ViewPort.Width, ViewPort.Height, D3DFMT_A32B32G32R32F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), R_TARGET_DIRECTIONAL_SPEC)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Directional_Light", L"Target_Directional_Light")))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Directional_Light", L"Target_Directional_Specular")))	return E_FAIL;

	//For. RenderTarget : Point Light
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Point_Light", _uint(ViewPort.Width), _uint(ViewPort.Height), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_POINT_LIGHT)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Point_Light", 320, 0, 320, 180)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Point_Specular", _uint(ViewPort.Width), _uint(ViewPort.Height), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_POINT_SPECF)))	return E_FAIL;
	
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Point_Light", L"Target_Point_Light")))			return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Point_Light", L"Target_Point_Specular")))			return E_FAIL;

	//For. RenderTarget : Shade
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_SHADE)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Shade", 320, 0, 320, 180)))			return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_All_Sepcular", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_ALL_SPECULAR)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_All_Sepcular", 320, 180, 320, 180)))	return E_FAIL;

	//For. Multi Render Target(MRT) : MRT_LightAcc
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_LightAcc", L"Target_Shade")))					return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_LightAcc", L"Target_All_Sepcular")))				return E_FAIL;
	
	//For.RenderTarget : Blur_AB
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Light_Blur_A", _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_LIGHT_BLUR_A)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Light_Blur_B", _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_LIGHT_BLUR_B)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Light_Blur_A", L"Target_Light_Blur_A")))			return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Light_Blur_B", L"Target_Light_Blur_B")))			return E_FAIL;

	//For.RenderTarget : BlurDepth_AB
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Light_BlurDepth_A", _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_LIGHT_BLUR_DEPTH_A)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Light_BlurDepth_B", _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_LIGHT_BLUR_DEPTH_B)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Light_BlurDepth_A", L"Target_Light_BlurDepth_A")))			return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Light_BlurDepth_B", L"Target_Light_BlurDepth_B")))			return E_FAIL;

	//For. RenderTarget : Blur
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Light_Blur", _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_LIGHT_BLUR)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Light_Blur", 320, 360, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Light_Blur", L"Target_Light_Blur")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : Outline 
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Outline", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_OUTLINE)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Outline", 960, 720, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Outline", L"Target_Outline")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : Shadow
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Shadow", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), R_TARGET_SHADOW)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Shadow", 1280, 540, 320, 180)))return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Shadow", L"Target_Shadow")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : Char_Sampler_Shade
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Char_Sampler_Shade", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_CHAR_SAMPLER_SHADE)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Char_Sampler_Shade", 960, 180, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Char_Sampler_Shade", L"Target_Char_Sampler_Shade")))			return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Char_Sampler_Shade_HDR", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_CHAR_SAMPLER_SHADE_HDR)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Char_Sampler_Shade_HDR", 320, 720, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Char_Sampler_Shade_HDR", L"Target_Char_Sampler_Shade_HDR")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : Blend
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Blend", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_BLEND)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Blend", 960, 0, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Blend", L"Target_Blend")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : SSAO
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_HDR", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_HDR)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_HDR", L"Target_HDR")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : DOF_BLUR
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_DOF_BLUR", _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_DOF_BLUR)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_DOF_BLUR", L"Target_DOF_BLUR")))			return E_FAIL;

	//For. RenderTarget : DOF
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_DOF", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_DOF)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_DOF", 960, 360, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_DOF", L"Target_DOF")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================
	//For. RenderTarget : Motion_Blur
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Motion_Blur", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_MOTION_BLUR)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Motion_Blur", 320, 720, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Motion_Blur", L"Target_Motion_Blur")))			return E_FAIL;
	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : PostEffect_Blur
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_PostEffect_Blur", _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_POSTEFFECT_BLUR)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_PostEffect_Blur", 960, 180, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_PostEffect_Blur", L"Target_PostEffect_Blur")))			return E_FAIL;

	//For. RenderTarget : PostEffect_Blur
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_All_PostEffect_Blur", _uint(ViewPort.Width / 4.0f), _uint(ViewPort.Height / 4.0f), D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_ALL_POSTEFFECT_BLUR)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_All_PostEffect_Blur", 960, 540, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_All_PostEffect_Blur", L"Target_All_PostEffect_Blur")))			return E_FAIL;

	//For. RenderTarget : PostEffect_Blur_Depth
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_All_PostEffect_Blur_Depth", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_ALL_POSTEFFECT_BLUR_DEPTH)))	return E_FAIL;
	//if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_All_PostEffect_Blur_Depth", 960, 180, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_All_PostEffect_Blur_Depth", L"Target_All_PostEffect_Blur_Depth")))			return E_FAIL;

	//For. RenderTarget : PostEffect
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_PostEffect", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_POSTEFFECT)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_PostEffect", L"Target_PostEffect")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : Final
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_Final", ViewPort.Width, ViewPort.Height, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), R_TARGET_FINAL)))	return E_FAIL;
	if (FAILED(CTarget_Manager::GetInstance()->Ready_DebugBuffer(L"Target_Final", 640, 0, 320, 180)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_Final", L"Target_Final")))			return E_FAIL;

	//=====================================================================================================================
	//=====================================================================================================================

	//For. RenderTarget : MiniMap
	if (FAILED(CTarget_Manager::GetInstance()->Ready_Target(pGraphicDev, L"Target_MiniMap", 200, 200, D3DFMT_A16B16G16R16F, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), R_TARGET_MINIMAP)))	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Ready_MRT(L"MRT_MiniMap", L"Target_MiniMap")))			return E_FAIL;


	return S_OK;
}


HRESULT Engine::CManagement::Ready_Management_ForTool(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader*		pShader = NULL;

	//For.Object Shader
	pShader = CShader::Create(pGraphicDev, L"../../../Reference/Headers/Shader_Object.hpp");
	if(NULL == pShader) return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Object", pShader))) return E_FAIL;

	//For.Player Shader
	pShader = CShader::Create(pGraphicDev, L"../../../Reference/Headers/Shader_Player.hpp");
	if(NULL == pShader) return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Player", pShader))) return E_FAIL;


	//For.Particle Shader
	pShader = CShader::Create(pGraphicDev, L"../../../Reference/Headers/Shader_Particle.hpp");
	if(NULL == pShader) return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_Particle", pShader))) return E_FAIL;

	//For.UI Shader
	pShader = CShader::Create(pGraphicDev, L"../../../Reference/Headers/Shader_UI.hpp");
	if(NULL == pShader) return E_FAIL;

	if(FAILED(CPrototype_Manager::GetInstance()->Ready_Prototype(L"Shader_UI", pShader))) return E_FAIL;



	return S_OK;
}

HRESULT Engine::CManagement::SetUp_CurrentScene(CScene* pCurrentScene)
{
	if (FAILED(SetUp_SendObject(pCurrentScene)))
		return E_FAIL;
	//지우기전에 맵데이터를 넘겨줘야대
	Engine::Safe_Release(m_pCurrentScene);

	m_pRenderer->Clear_RenderGroup();

	m_pCurrentScene = pCurrentScene;

	if (FAILED(m_pCurrentScene->Start_ForScene()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT Engine::CManagement::Return_SelectScene(CScene* pCurrentScene)
{
	Engine::Safe_Release(m_pCurrentScene);

	m_pRenderer->Clear_RenderGroup();

	m_pCurrentScene = pCurrentScene;

	if (FAILED(m_pCurrentScene->Start_ForScene()))
		return E_FAIL;

	return S_OK;
}

void Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if(m_pCurrentScene == NULL)
		return;

	m_pCurrentScene->Update_Scene(fTimeDelta);
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pRenderer->Render_GameObject(pGraphicDev);

	if(m_pCurrentScene == NULL)
		return;

	m_pCurrentScene->Render_Scene();
}

HRESULT CManagement::SetUp_SendObject(CScene * pCurrentScene)
{
	if (m_pCurrentScene == NULL)
		return S_OK;
	if (FAILED(m_pCurrentScene->Set_Object(pCurrentScene)))
		return E_FAIL;
	return S_OK;
}


void Engine::CManagement::SetManagerProjectID(const PROJECTID eProjectID)
{
	m_eProjectID = eProjectID;
	m_pRenderer->SetProjectID(eProjectID);
}

void Engine::CManagement::SafeRelease(void)
{
	m_pCurrentScene->SafeRelease();
}

void Engine::CManagement::Free(void)
{
	//Engine::Safe_Release(m_pCurrentScene);
	Engine::Safe_Release(m_pRenderer);
}
