#include "stdafx.h"
#include "Cube.h"

#include "Export_Engine.h"

#include "Map_Store.h"
#include "Map_Ice.h"
#include "Monster.h"
#include "Texture.h"
#include "Cube_Texture.h"



//맵툴 Environment..

CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pBufferCom(NULL)
, m_pRendererCom(NULL)
, m_lMouseMove(0)
, m_fAlpha(0.f)
, m_bStop(false)
, m_bComplete(false)
, m_bCheck(false)
{
	ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}
CCube::~CCube(void)
{

}

void CCube::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	
	_vec3	vScale;
	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));
}

void CCube::Render_Object(void)
{
	if (!g_bDebugRender)
	{
		return;
	}
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

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->Begin(&iPassCnt, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();	

	pEffect->EndPass();
	pEffect->End();
}


HRESULT CCube::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_eLayerType = Engine::LAYER_ENVIRONMENT;
	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);
	m_pTransCom->Set_Scale(5.f, 5.f, 5.f);
	//m_pTransCom->Rotation(Engine::ROT_Y, m_pTransCom->Get_Angle(_vec3(0.f,0.f,0.f)));
	//m_pTransCom->m_vScale = _vec3(100.f, 100.f, 100.f);
	m_fAlpha = 1.f;

	return S_OK;
}

HRESULT CCube::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	wstring::iterator iter = m_wstrName.begin();
	wstring::iterator iter_end = m_wstrName.end();

	TCHAR szName[MAX_PATH] = L"";

	int i = 0;

	for(; iter != iter_end; ++iter)
	{
		szName[i] = (*iter);
		++i;
	}

	// For.Buffer Component
	m_pBufferCom = (Engine::CCube_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_CubeTex");
	pComponent = (Engine::CComponent*)m_pBufferCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_Collider_Blue");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_UI"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	return S_OK;
}

HRESULT CCube::Set_ConstantTable( LPD3DXEFFECT pEffect )
{
	pEffect->AddRef();

	_matrix		matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Find_LightInfo(0, D3DLIGHT_DIRECTIONAL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.0f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", MtrlInfo.Power);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	Engine::Safe_Release(pEffect);

	return S_OK;
}



CCube* CCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CCube*		pInstance = new CCube(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CCube Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CCube::Free(void)
{
	Engine::CGameObject::Free();
}

void CCube::Stop( void )
{
	m_bStop = true;
}

void CCube::Complete( void )
{
	m_bComplete = true;
}



Engine::CTransform* CCube::GetTransCom( void )
{
	return m_pTransCom;
}

_bool CCube::GetChecked( void )
{
	return m_bCheck;
}

void CCube::CheckTrue( void )
{
	m_bCheck = true;
}

void CCube::CheckFalse( void )
{
	m_bCheck = false;
}

OBJ_INFO* CCube::GetObjectInfo( void )
{
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_tInfo.m_vPos);
	m_pTransCom->Get_Angle(&m_tInfo.m_vAngle);
	m_pTransCom->Get_Scale(&m_tInfo.m_vScale);


	wstring::iterator iter = m_wstrName.begin();
	wstring::iterator iter_end = m_wstrName.end();

	int i = 0;

	for(; iter != iter_end; ++iter)
	{
		m_tInfo.m_szName[i] = (*iter);
		++i;
	}

	wstring wstrTag = L"Cube";

	iter = wstrTag.begin();
	iter_end = wstrTag.end();

	i = 0;

	for(; iter != iter_end; ++iter)
	{
		m_tInfo.m_szTag[i] = (*iter);
		++i;
	}




	m_tInfo.m_eLayerType = m_eLayerType;

	return &m_tInfo;
}


void CCube::SetObjectInfo( _vec3* vPos, _vec3* vScale, _vec3* vAngle )
{
	m_pTransCom->Set_Position(vPos);
	m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
}

_bool CCube::CheckRayCol(_float* pOut, _vec3* pPivotPos, _vec3* pRayDir, _float rDist)
{
	_matrix matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);

	Engine::VTXCUBETEX* pVertex = new Engine::VTXCUBETEX[8];

	m_pBufferCom->GetVtxInfo(pVertex);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&pVertex[i].vPosition, &pVertex[i].vPosition, &matWorld);
	}

	float fU, fV, fDist;
	bool bCheck = false;
	_vec3 vCubePos;
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vCubePos);
	_vec3 vCubePlanDir;

	//(Z-)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[1].vPosition,
		&pVertex[0].vPosition,
		&pVertex[2].vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
		//*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{
			vCubePlanDir =  D3DXVECTOR3(0.f, 0.f, -1.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(Z-)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[3].vPosition,
		&pVertex[2].vPosition,
		&pVertex[0].vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 0.f, -1.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(Z+)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[6].vPosition,
		&pVertex[7].vPosition,
		&pVertex[5].vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 0.f, 1.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(Z+)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[4].vPosition,
		&pVertex[6].vPosition,
		&pVertex[7]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
		//*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 0.f, 1.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(Y-)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[2].vPosition,
		&pVertex[3].vPosition,
		&pVertex[6]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, -1.f, 0.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(Y-)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[7].vPosition,
		&pVertex[6].vPosition,
		&pVertex[3]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, -1.f, 0.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}
	//(Y+)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[5].vPosition,
		&pVertex[4].vPosition,
		&pVertex[1]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 1.f, 0.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(Y+)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[0].vPosition,
		&pVertex[1].vPosition,
		&pVertex[4]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(0.f, 1.f, 0.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(x-)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[0].vPosition,
		&pVertex[4].vPosition,
		&pVertex[3]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(-1.f, 0.f, 0.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(x-)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[7].vPosition,
		&pVertex[3].vPosition,
		&pVertex[4]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(-1.f, 0.f, 0.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(x+)앞면 위쪽 삼각
	if(D3DXIntersectTri(&pVertex[5].vPosition,
		&pVertex[1].vPosition,
		&pVertex[6]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(1.f, 0.f, 0.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}

	//(x+)앞면 아래쪽 삼각
	if(D3DXIntersectTri(&pVertex[2].vPosition,
		&pVertex[6].vPosition,
		&pVertex[1]. vPosition,
		pPivotPos,
		pRayDir,
		&fU, &fV, &fDist))
	{
	//	*pOut = *pPivotPos + *pRayDir * fDist;

		if(rDist > fDist)
		{		
			vCubePlanDir =  D3DXVECTOR3(1.f, 0.f, 0.f) + vCubePos;
			rDist = fDist;
			*pOut = fDist;
			bCheck = true;
		}
	}
	Engine::Safe_Delete_Array(pVertex);

	return bCheck;
}



