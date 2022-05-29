#include "stdafx.h"
#include "Map_Fire.h"

#include "Export_Engine.h"

#include "Camera_MapTool.h"
#include "MouseCol.h"
#include "Scene_MapTool.h"
#include "NavigationMesh.h"
#include "Cell.h"

//¸ÊÅø ÆÄÀÌ¾î..

CMap_Fire::CMap_Fire(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pRendererCom(NULL)
, m_pResourceMgr(NULL)
, m_pMouseCol(NULL)
, m_pNaviCom(NULL)
, m_bClick(false)
, m_bDrag(false)
{
	//ºÐÇÒ¸Ê..
		for (int i = 0; i < 33; ++i)
		{
			m_pMeshCom[i] = NULL;
		}
}
CMap_Fire::~CMap_Fire(void)
{

}

Engine::CStaticMesh** CMap_Fire::GetMapMesh(_int* iSize)
{
	//ºÐÇÒ¸Ê..
	*iSize = 33;
	return m_pMeshCom;

	////Åë¸Ê..
	//*iSize = 1;
	//return &m_pMeshCom;
}

void CMap_Fire::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	if(g_vMouse.x > 5.f)
	{
		float rDist = 9999.f;

		////ºÐÇÒ¸Ê..
		for(int i = 0; i < 33; ++i)
		{
			m_pMouseCol->PickMapMesh(&g_vMouse, m_pMeshCom[i]->Get_Mesh(), rDist);
		}

		//Åë¸Ê..
	//	m_pMouseCol->PickMapMesh(&g_vMouse, m_pMeshCom->Get_Mesh(), rDist);
		NaviSetting();
	}

	_vec3	vScale;


	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

}

void CMap_Fire::Render_Object(void)
{
	m_pNaviCom->Render_NaviCell();

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect) return;

	pEffect->AddRef();

	if(FAILED(Set_ConstantTable(pEffect))) return;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	for (int i = 0; i < 33; ++i)
	{
		m_pMeshCom[i]->Render_Mesh(pEffect);
	}

	//Åë¸Ê
	//	m_pMeshCom->Render_Mesh(pEffect);


	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

}

HRESULT CMap_Fire::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

	m_pMouseCol = CMouseCol::Create();
	m_eNaviMode = MODE_NONE;
	m_eNaviType = Engine::TYPE_NORMAL;
	m_pTransCom->Set_Scale(1.f, 1.f, 1.f);



	return S_OK;
}

HRESULT CMap_Fire::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	////Mesh
	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_0");
	m_pMeshCom[0] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_0", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_1");
	m_pMeshCom[1] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_1", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_2");
	m_pMeshCom[2] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_02", pComponent));


	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_3");
	m_pMeshCom[3] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_3", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_4");
	m_pMeshCom[4] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_4", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_5");
	m_pMeshCom[5] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_5", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_6");
	m_pMeshCom[6] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_6", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_7");
	m_pMeshCom[7] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_7", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_8");
	m_pMeshCom[8] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_8", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_9");
	m_pMeshCom[9] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_9", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_10");
	m_pMeshCom[10] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_10", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_11");
	m_pMeshCom[11] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_11", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_12");
	m_pMeshCom[12] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_12", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_13");
	m_pMeshCom[13] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_13", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_14");
	m_pMeshCom[14] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_14", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_15");
	m_pMeshCom[15] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_15", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_16");
	m_pMeshCom[16] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_16", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_17");
	m_pMeshCom[17] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_17", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_18");
	m_pMeshCom[18] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_18", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_19");
	m_pMeshCom[19] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_19", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_20");
	m_pMeshCom[20] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_20", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_21");
	m_pMeshCom[21] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_21", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_22");
	m_pMeshCom[22] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_22", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_23");
	m_pMeshCom[23] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_23", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_24");
	m_pMeshCom[24] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_24", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_25");
	m_pMeshCom[25] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_25", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_26");
	m_pMeshCom[26] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_26", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_27");
	m_pMeshCom[27] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_27", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_28");
	m_pMeshCom[28] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_28", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_29");
	m_pMeshCom[29] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_29", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_30");
	m_pMeshCom[30] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_30", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_31");
	m_pMeshCom[31] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_31", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_32");
	m_pMeshCom[32] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_32", pComponent));

	//pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_33");
	//m_pMeshCom[33] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_33", pComponent));

	//pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire_34");
	//m_pMeshCom[34] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire_34", pComponent));


	//Åë¸Ê..
	/*pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Map_Fire");
	m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Fire", pComponent));*/

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Object"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	//Navi
	pComponent = (Engine::CNavigationMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navigation");
	m_pNaviCom = dynamic_cast<Engine::CNavigationMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"NaviMesh", m_pNaviCom));

	return S_OK;
}

HRESULT CMap_Fire::Set_ConstantTable( LPD3DXEFFECT pEffect )
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

	//ÁöÇü »ö * (ºû.Diffuse * ¸¶Å×¸®¾ó.diffuse) + (ºû.Ambient * ¸¶Å×¸®¾ó.Ambient)
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

	Engine::Safe_Release(pEffect);

	return S_OK;
}

void CMap_Fire::NaviSetting( void )
{
	if(m_eNaviMode == MODE_ADD)
	{
		AddCellMode();
	}

	if(m_eNaviMode == MODE_MODIFY)
	{
		ModifyMode();
	}
	if(m_eNaviMode == MODE_CHECK)
	{
		CheckMode();
	}

	if(m_bClick)
	{
		if(!Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
			m_bClick = false;
	}
}


void CMap_Fire::AddCellMode( void )
{
	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
	{
		if(m_bClick)
			return;

		++CScene_MapTool::m_iPointCount;

		vector<Engine::CCell*>::iterator iter = m_pNaviCom->GetVECCELL()->begin();
		vector<Engine::CCell*>::iterator iter_end = m_pNaviCom->GetVECCELL()->end();

		for(iter; iter != iter_end; ++iter)
		{
			for(int i = 0; i < 3; ++i)
			{
				float fDis = 0.f;
				_vec3 vTemp = *(*iter)->Get_Point((Engine::CCell::POINT)i) - g_vViewMouse;
				fDis = abs(D3DXVec3Length(&vTemp));
				if(fDis < 2.5f)
				{
					if(CScene_MapTool::m_iPointCount == 1)
					{
						m_vNaviPos[0] = *(*iter)->Get_Point((Engine::CCell::POINT)i);
						m_bClick = true;
						return;
					}
					else if(CScene_MapTool::m_iPointCount == 2)
					{
						m_vNaviPos[1] = *(*iter)->Get_Point((Engine::CCell::POINT)i);
						m_bClick = true;
						return;
					}

					else if(CScene_MapTool::m_iPointCount > 2)
					{
						m_vNaviPos[2] = *(*iter)->Get_Point((Engine::CCell::POINT)i);
						if(m_eNaviType == Engine::TYPE_NORMAL)
						{
							m_pNaviCom->Add_Cell(&m_vNaviPos[0], &m_vNaviPos[1], &m_vNaviPos[2], Engine::TYPE_NORMAL);
						}
						else
						{
							m_pNaviCom->Add_Cell(&m_vNaviPos[0], &m_vNaviPos[1], &m_vNaviPos[2], Engine::TYPE_FALL);
						}
						CScene_MapTool::m_iPointCount = 0;
						m_bClick =true;
						return;
					}
				}

			}
		}

		if(CScene_MapTool::m_iPointCount == 1)
		{
			m_vNaviPos[0] = g_vViewMouse;
		}
		else if(CScene_MapTool::m_iPointCount == 2)
		{
			m_vNaviPos[1] = g_vViewMouse;
		}
		else if(CScene_MapTool::m_iPointCount > 2)
		{
			m_vNaviPos[2] = g_vViewMouse;
			if(m_eNaviType == Engine::TYPE_NORMAL)
			{
				m_pNaviCom->Add_Cell(&m_vNaviPos[0], &m_vNaviPos[1], &m_vNaviPos[2], Engine::TYPE_NORMAL);
			}
			else
			{
				m_pNaviCom->Add_Cell(&m_vNaviPos[0], &m_vNaviPos[1], &m_vNaviPos[2], Engine::TYPE_FALL);
			}
			CScene_MapTool::m_iPointCount = 0;
		}

		m_bClick = true;
	}

}

void CMap_Fire::ModifyMode( void )
{

	vector<Engine::CCell*>::iterator iter = m_pNaviCom->GetVECCELL()->begin();
	vector<Engine::CCell*>::iterator iter_end = m_pNaviCom->GetVECCELL()->end();
	for(iter; iter != iter_end; ++iter)
	{
		for(int i = 0; i < 3; ++i)
		{
			float fDis = 0.f;
			D3DXVECTOR3 vTemp = *(*iter)->Get_Point((Engine::CCell::POINT)i) - g_vViewMouse;
			fDis = abs(D3DXVec3Length(&vTemp));
			if(fDis < 2.f)
			{
				if(m_bDrag)
				{
					(*iter)->m_bMovePoint[i] = true;
				}
				else if(!m_bDrag)
				{
					(*iter)->m_bMovePoint[i] = false;
				}
			}
		}
	}

	iter =  m_pNaviCom->GetVECCELL()->begin();
	for(iter; iter!= iter_end; ++iter)
	{
		(*iter)->SetMovePoint(&g_vViewMouse);
	}


	iter = m_pNaviCom->GetVECCELL()->begin();
	iter_end = m_pNaviCom->GetVECCELL()->end();

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->ChoicePoint();
	}

	if(g_vMouse.x > 10.f)
	{
		if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if(m_bDrag)
				return ;

			m_bDrag = true;
		}
	}

	if(g_vMouse.x > 10.f)
	{
		if(m_bDrag)
		{
			if(!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
			{
				m_bDrag = false;
			}
		}
	}


	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80)
	{
		vector<Engine::CCell*>::iterator iter = m_pNaviCom->GetVECCELL()->begin();
		vector<Engine::CCell*>::iterator iter_end =	m_pNaviCom->GetVECCELL()->end();

		for(iter; iter!= iter_end; ++iter)
		{
			for(int i = 0; i < 3 ;++i)
			{
				(*iter)->m_bChoosedPoint[i] = false;
			}	
		}
	}


}

_bool CMap_Fire::CheckMode( void )
{
	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB) & 0x80)
	{
		vector<Engine::CCell*>::iterator iter = m_pNaviCom->GetVECCELL()->begin();
		vector<Engine::CCell*>::iterator iter_end =	m_pNaviCom->GetVECCELL()->end();
		_vec3 vPoint[3];

		for(iter; iter!= iter_end; ++iter)
		{
			_bool bCheck[3];
			ZeroMemory(bCheck, sizeof(bool)*3);

			for(int i = 0; i < 3 ;++i)
			{
				vPoint[i] = *(*iter)->Get_Point((Engine::CCell::POINT)i);
			}	
			_vec3 vDirection[3] = 
			{
				vPoint[1] - vPoint[0],
				vPoint[2] - vPoint[1],
				vPoint[0] - vPoint[2],
			};

			_vec3 vNormal[3] =
			{
				_vec3(-vDirection[0].z, 0.f, vDirection[0].x),
				_vec3(-vDirection[1].z, 0.f, vDirection[1].x),
				_vec3(-vDirection[2].z, 0.f, vDirection[2].x),
			};

			//´ÜÀ§ º¤ÅÍ·Î ¸¸µé¾ú¾î...
			for(int i = 0; i < 3; ++i)
				D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

			for(int i = 0; i < 3; ++i)
			{
				D3DXVECTOR3	vDestDir = g_vViewMouse - vPoint[i];

				float fDotResult = D3DXVec3Dot(&vDestDir, &vNormal[i]);

				if(fDotResult > 0.f)
				{
					bCheck[i] = false;				
				}
				else
				{
					bCheck[i] = true;
				}
			}
			if(bCheck[0] && bCheck[1] && bCheck[2])
			{
				vector<Engine::CCell*>::iterator iter_First = m_pNaviCom->GetVECCELL()->begin();
				vector<Engine::CCell*>::iterator iter_First_end =	m_pNaviCom->GetVECCELL()->end();
				for(iter_First; iter_First != iter_First_end; ++iter_First)
				{
					(*iter_First)->SetFirstType();
				}
				((Engine::CCell*)(*iter))->SetType(Engine::TYPE_SELECT);
				return true;
			}
		}
	}

	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB) & 0x80)
	{
		vector<Engine::CCell*>::iterator iter = m_pNaviCom->GetVECCELL()->begin();
		vector<Engine::CCell*>::iterator iter_end =	m_pNaviCom->GetVECCELL()->end();

		for(iter; iter!= iter_end; ++iter)
		{
			for(int i = 0; i < 3 ;++i)
			{
				(*iter)->SetFirstType();
			}	
		}
	}

	return false;

}

Engine::CNavigationMesh* CMap_Fire::GetNaviCom( void )
{
	return m_pNaviCom;
}

Engine::CStaticMesh** CMap_Fire::GetMeshCom( void )
{
	//ºÐÇÒ¸Ê..
	return m_pMeshCom;

	//Åë¸Ê..
	//return &m_pMeshCom;
}

CMap_Fire* CMap_Fire::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CMap_Fire*		pInstance = new CMap_Fire(pGraphicDev, wstrName);

	if (FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CMap_Fire Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMap_Fire::Free(void)
{
	Engine::CGameObject::Free();
	Safe_Delete(m_pMouseCol);
}