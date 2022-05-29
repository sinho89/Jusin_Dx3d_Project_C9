#include "stdafx.h"
#include "Map_Ice.h"

#include "Export_Engine.h"

#include "NavigationMesh.h"
#include "Cell.h"

#include "Camera.h"
#include "SnowBall.h"

#include "Sound.h"

//클라 아이스맵..

CMap_Ice::CMap_Ice(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
	: Engine::CGameObject(pGraphicDev, wstrName)
	, m_pTransCom(NULL)
	, m_pRendererCom(NULL)
	, m_fAlpha(1.0f)
	, m_bSnowBall(false)
	, m_fSnowBallTime(false)
{
	for(int i = 0 ; i < 69; ++i)
	{
		m_pMeshCom[i] = NULL;
	}
}
CMap_Ice::~CMap_Ice(void)
{

}

Engine::CStaticMesh** CMap_Ice::GetMapMesh(_int* iSize)
{
	*iSize = 69;
	return m_pMeshCom;
}

HRESULT CMap_Ice::Start_ForScene(void)
{
	m_pLightCam = Engine::GetCamera(Engine::CAMERA_LIGHT);
	m_pCurCam = Engine::GetCurrentCamera();

	Engine::CGameObject* pGameObject = NULL;
	pGameObject = Engine::FindWithTag(L"Player");
	if (pGameObject == NULL)
		return E_FAIL;

	m_pTargetTransCom = (Engine::CTransform*)pGameObject->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);
	if (m_pTargetTransCom == NULL)
		return E_FAIL;

	m_pSoundCom->Set_Sound(L"EarthQuake", L"EarthQuake.wav");

	return S_OK;
}

void CMap_Ice::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if (!m_bSnowBall)
	{
		_vec3 vPlayerPos = m_pTargetTransCom->Get_Now_Position();
		_vec3 vLength = vPlayerPos - m_vSnowBallStart;
		_float fLength = D3DXVec3Length(&vLength);

		if (fLength < 10.f)
		{
			m_pSoundCom->MyPlaySound(L"EarthQuake");
			Engine::Set_Shake(Engine::CAMERA_PLAYER, _vec3(0.f, 1.f, 0.f), 0.3f, 8);
			m_bSnowBall = true;
		}
	}


	if (m_bSnowBall)
	{
		_vec3 vPlayerPos = m_pTargetTransCom->Get_Now_Position();
		_vec3 vLength = vPlayerPos - m_vSnowBallEnd;
		_float fLength = D3DXVec3Length(&vLength);
		if (fLength < 10.f)
		{
			m_pSoundCom->MyStopSound(L"EarthQuake");
			m_bSnowBall = false;
		}

		m_fSnowBallTime += fTimeDelta;

		if (m_fSnowBallTime > 2.f)
		{
			m_fSnowBallTime = 0.f;
			CGameObject* pGameObject = CSnowBall::Create(m_pGraphicDev, L"Mesh_SnowBall");
			Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Monster", pGameObject);

		}
	}

	

	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	//m_pRendererCom->Add_RenderGroup(Engine::RENDER_STAGE_DEPTH, this);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_STATIC, this);

	/*if (m_pRendererCom->GetShadowBuffer())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENEDER_SHADOW, this);
	}*/

	//_vec3	vScale;
	//Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));
}

void CMap_Ice::Render_Object(void)
{
}

void CMap_Ice::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{
	if (g_bDebugRender)
		m_pNaviCom->Render_NaviCell();

	switch (iPassIdx)
	{
	case 0:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 1:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 2:
		if (FAILED(Set_Shadow_ConstanTable(pEffect))) return;
		break;

	case 3:
		if (FAILED(Set_Minimap_ConstanTable(pEffect))) return;
		break;

	case 4:
		if (FAILED(Set_ConstantTable(pEffect))) return;
		break;

	case 6:
		return;
	}

	for (int i = 0; i < 69; ++i)
	{
		m_pMeshCom[i]->Render_Mesh(pEffect, iPassIdx);
	}
}

HRESULT CMap_Ice::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);

	m_vSnowBallStart = _vec3(324.f, 22.f, 207.f);
	m_vSnowBallEnd = _vec3(322.f, 53.f, 23.f);

	return S_OK;
}

HRESULT CMap_Ice::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));

	////Mesh
	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_0");
	m_pMeshCom[0] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_0", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_1");
	m_pMeshCom[1] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_1", pComponent));


	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_2");
	m_pMeshCom[2] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_2", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_3");
	m_pMeshCom[3] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_3", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_4");
	m_pMeshCom[4] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_4", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_5");
	m_pMeshCom[5] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_5", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_6");
	m_pMeshCom[6] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_6", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_7");
	m_pMeshCom[7] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_7", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_8");
	m_pMeshCom[8] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_8", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_9");
	m_pMeshCom[9] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_9", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_10");
	m_pMeshCom[10] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_10", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_11");
	m_pMeshCom[11] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_11", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_12");
	m_pMeshCom[12] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_12", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_13");
	m_pMeshCom[13] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_13", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_14");
	m_pMeshCom[14] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_14", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_15");
	m_pMeshCom[15] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_15", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_16");
	m_pMeshCom[16] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_16", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_17");
	m_pMeshCom[17] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_17", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_18");
	m_pMeshCom[18] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_18", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_19");
	m_pMeshCom[19] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_19", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_20");
	m_pMeshCom[20] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_20", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_21");
	m_pMeshCom[21] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_21", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_22");
	m_pMeshCom[22] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_22", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_23");
	m_pMeshCom[23] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_23", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_24");
	m_pMeshCom[24] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_24", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_25");
	m_pMeshCom[25] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_25", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_26");
	m_pMeshCom[26] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_26", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_27");
	m_pMeshCom[27] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_27", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_28");
	m_pMeshCom[28] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_28", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_29");
	m_pMeshCom[29] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_29", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_30");
	m_pMeshCom[30] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_30", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_31");
	m_pMeshCom[31] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_31", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_32");
	m_pMeshCom[32] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_32", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_33");
	m_pMeshCom[33] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_33", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_34");
	m_pMeshCom[34] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_34", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_35");
	m_pMeshCom[35] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_35", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_36");
	m_pMeshCom[36] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_36", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_37");
	m_pMeshCom[37] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_37", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_38");
	m_pMeshCom[38] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_38", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_39");
	m_pMeshCom[39] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_39", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_40");
	m_pMeshCom[40] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_40", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_41");
	m_pMeshCom[41] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_41", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_42");
	m_pMeshCom[42] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_42", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_43");
	m_pMeshCom[43] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_43", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_44");
	m_pMeshCom[44] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_44", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_45");
	m_pMeshCom[45] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_45", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_46");
	m_pMeshCom[46] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_46", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_47");
	m_pMeshCom[47] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_47", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_48");
	m_pMeshCom[48] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_48", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_49");
	m_pMeshCom[49] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_49", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_50");
	m_pMeshCom[50] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_50", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_51");
	m_pMeshCom[51] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_51", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_52");
	m_pMeshCom[52] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_52", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_53");
	m_pMeshCom[53] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_53", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_54");
	m_pMeshCom[54] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_54", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_55");
	m_pMeshCom[55] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_55", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_56");
	m_pMeshCom[56] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_56", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_57");
	m_pMeshCom[57] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_57", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_58");
	m_pMeshCom[58] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_58", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_59");
	m_pMeshCom[59] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_59", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_60");
	m_pMeshCom[60] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_60", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_61");
	m_pMeshCom[61] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_61", pComponent));


	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_62");
	m_pMeshCom[62] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_62", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_63");
	m_pMeshCom[63] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_63", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_64");
	m_pMeshCom[64] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_64", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_65");
	m_pMeshCom[65] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_65", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_66");
	m_pMeshCom[66] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_66", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_67");
	m_pMeshCom[67] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_67", pComponent));

	pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Map_Ice_68");
	m_pMeshCom[68] = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Mesh_Map_Ice_68", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_StaticMesh"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));


	//Navi
	pComponent = Engine::Clone_Resource(RESOURCE_MESH_ICE, L"Mesh_Navigation");
	if (pComponent == NULL)
		return E_FAIL;
	m_pNaviCom = (Engine::CNavigationMesh*)pComponent;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"NaviCom", pComponent));

	//Sound
	pComponent = CSound::Create(NULL);
	m_pSoundCom = (CSound*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Sound", pComponent));

	return S_OK;
}

HRESULT CMap_Ice::Set_ConstantTable( LPD3DXEFFECT pEffect )
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
	//pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	//pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);
	//pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);

	D3DMATERIAL9	MtrlInfo;
	ZeroMemory(&MtrlInfo, sizeof(D3DMATERIAL9));

	//지형 색 * (빛.Diffuse * 마테리얼.diffuse) + (빛.Ambient * 마테리얼.Ambient)
	MtrlInfo.Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
	//MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//MtrlInfo.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.f);
	//MtrlInfo.Power = 20.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&MtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&MtrlInfo.Ambient);
	//pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&MtrlInfo.Specular);
	pEffect->SetFloat("g_fPower", 50.0f);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPosition", (_vec4*)&matView._41);
	pEffect->SetFloat("g_fAlpha", m_fAlpha);

	float fFar = Engine::GetCurrentCamera()->GetCamFar();
	pEffect->SetFloat("g_fFar", fFar);

	pEffect->SetBool("g_bLimLight", false);
	pEffect->SetBool("g_bSpecularCheck", false);
	pEffect->SetBool("g_bBlurCheck", false);
	pEffect->SetBool("g_bOutline", m_bOutlineCheck);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMap_Ice::Set_Shadow_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = m_pLightCam->GetMatView();
	matLightProj = m_pLightCam->GetMatProj();

	pEffect->SetMatrix("g_matLightView", &matLightView);
	pEffect->SetMatrix("g_matLightProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}

HRESULT CMap_Ice::Set_Minimap_ConstanTable(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix		matWorld;

	m_pTransCom->Get_WorldMatrix(&matWorld);

	pEffect->SetMatrix("g_matWorld", &matWorld);

	_matrix		matLightView, matLightProj;

	matLightView = Engine::GetCamera(Engine::CAMERA_MINIMAP)->GetMatView();
	matLightProj = Engine::GetCamera(Engine::CAMERA_MINIMAP)->GetMatProj();

	pEffect->SetMatrix("g_matView", &matLightView);
	pEffect->SetMatrix("g_matProj", &matLightProj);

	Engine::Safe_Release(pEffect);

	return S_OK;
}



CMap_Ice* CMap_Ice::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CMap_Ice*		pInstance = new CMap_Ice(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CMap_Ice Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMap_Ice::Free(void)
{
  	Engine::CGameObject::Free();
}


