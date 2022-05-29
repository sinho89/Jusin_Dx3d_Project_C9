#include "stdafx.h"
#include "MiniMap.h"
#include "Engine_Defines.h"
#include "MapCamera.h"
#include "Map_Store.h"
#include "PlayerCamera.h"
#include "Texture.h"
#include "Rect_Texture.h"
#include "Map_Ice.h"
#include "Fade_Manager.h"
#include "WitchBlade.h"
#include "Layer.h"
#include "NPC.h"
#include "Monster.h"

CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, const Engine::OBJECTSTATE & eState, const _bool bIsActive)
	: CUI(pGraphicDev, wstrName)
	, m_pRendererCom(NULL)
	, m_pTransCom(NULL)
	, m_pBufferCom(NULL)
	, m_pShaderCom(NULL)
	, m_pMapCamera(NULL)
	, m_pMiniPlayer(NULL)
	, m_pMinimapTexCom(NULL)
	, m_pLogicLayer(NULL)
	, m_pStageBoss(NULL)
	, m_iTxW(0)
	, m_fMiniMapMove_X(0.0f)
	, m_fMiniMapMove_Y(0.0f)
	, m_vOriginalCenterPos(0.0f, 0.0f, 0.0f)
{
}

CMiniMap::~CMiniMap(void)
{
}

HRESULT CMiniMap::Start_ForScene(void)
{
	/*list<Engine::CGameObject*>::iterator iter = m_listMiniUnit.begin();
	list<Engine::CGameObject*>::iterator iter_end = m_listMiniUnit.end();

	for (; iter != iter_end; ++iter)
	{
		::Safe_Release(*iter);
	}

	m_listMiniUnit.clear();*/

	CPlayer* pPlyer = static_cast<CWitchBlade*>(Engine::Find(L"Player"));
	m_pPlayerTrans = static_cast<Engine::CTransform*>(pPlyer->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC));
	::Safe_Release(pPlyer);

	if (Engine::SCENE_TOWN == Engine::GetSceneID())
	{
		m_pMinimapTexCom = (Engine::CTexture*)m_mapComponent[Engine::COMID_STATIC].find(L"Com_TexMiniMap_Town")->second;
		m_vOriginalCenterPos = _vec3(-32.f, 0.f, 0.f);	

		FindNPC();
	}
	else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
	{
		m_pMinimapTexCom = (Engine::CTexture*)m_mapComponent[Engine::COMID_STATIC].find(L"Com_TexMiniMap_Ice")->second;
		m_vOriginalCenterPos = _vec3(-313.f, 3.f, 58.f);

		//StageIceFindMonster();
	}
	else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
	{
		m_pMinimapTexCom = (Engine::CTexture*)m_mapComponent[Engine::COMID_STATIC].find(L"Com_TexMiniMap_Fire")->second;
		m_vOriginalCenterPos = _vec3(235.f, -34.f, -397.f);

		//StageFireFindMonster();
	}

	return S_OK;
}

void CMiniMap::Update_Object(const _float & fTimeDelta)
{
	if (CFade_Manager::GetInstance()->GetFading() == true)
		return;

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_MINIMAP, this);

	//static_cast<CMapCamera*>(m_pMapCamera)->Update_Object(fTimeDelta);
	CUI::Update_Object(fTimeDelta);

	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);

	if (Engine::SCENE_TOWN == Engine::GetSceneID())
	{
		_vec3 vMovingDist = m_pPlayerTrans->Get_Now_Position() - m_vOriginalCenterPos;
		m_fMiniMapMove_X = vMovingDist.x;
		m_fMiniMapMove_Y = vMovingDist.z;
	}
	else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
	{
		_vec3 vMovingDist = m_pPlayerTrans->Get_Now_Position() - m_vOriginalCenterPos;
		m_fMiniMapMove_X = vMovingDist.x;
		m_fMiniMapMove_Y = vMovingDist.z;
	}
	else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
	{
		_vec3 vMovingDist = m_pPlayerTrans->Get_Now_Position() - m_vOriginalCenterPos;
		m_fMiniMapMove_X = vMovingDist.x;
		m_fMiniMapMove_Y = vMovingDist.z;
	}
}

void CMiniMap::Render_Object(void)
{
}

void CMiniMap::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
{	
	_matrix				matWorld, matView;
	_matrix				matOldView, matOldProj;
	_matrix				matRotY;

	if (1 == iPassIdx)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

		if (Engine::SCENE_TOWN == Engine::GetSceneID())
		{
			matWorld._11 = 10000;
			matWorld._22 = 10000;
			matWorld._33 = 1.0f;
			matWorld._41 = -90.0f	-	(m_fMiniMapMove_X * 9.0f);
			matWorld._42 = -800.0f	-	(m_fMiniMapMove_Y * 9.0f);
		}
		else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
		{
			matWorld._11 = 5000;
			matWorld._22 = 5000;
			matWorld._33 = 1.0f;
			matWorld._41 = 1000.0f	-	(m_fMiniMapMove_X * 3.25f);
			matWorld._42 = -275.0f	-	(m_fMiniMapMove_Y * 3.25f);
		}
		else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
		{
			matWorld._11 = 5000;
			matWorld._22 = 5000;
			matWorld._33 = 1.0f;
			matWorld._41 = -600.0f - (m_fMiniMapMove_X * 2.5f);
			matWorld._42 = 950.0f - (m_fMiniMapMove_Y * 2.5f);
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &m_matProj);

		m_pMinimapTexCom->Set_Texture(pEffect, "g_BaseTexture");

		pEffect->CommitChanges();

		m_pBufferCom->Render_Buffer();

		//MiniMap Player

		D3DXMatrixIdentity(&matWorld);
		//D3DXMatrixIdentity(&matView);

		if (Engine::SCENE_TOWN == Engine::GetSceneID())
		{
			matWorld._11 = 100.0f;
			matWorld._22 = 100.0f;
			matWorld._33 = 1.0f;
			//matWorld._41 = 0.0f;
			//matWorld._42 = 0.0f;
		}
		else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
		{
			matWorld._11 = 100.0f;
			matWorld._22 = 100.0f;
			matWorld._33 = 1.0f;
		}
		else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
		{
			matWorld._11 = 100.0f;
			matWorld._22 = 100.0f;
			matWorld._33 = 1.0f;
		}

		D3DXMatrixIdentity(&matRotY);
		float fAngleY = m_pPlayerTrans->Get_Now_Angle().y;
		fAngleY = 360.f - fAngleY;
		D3DXMatrixRotationZ(&matRotY, D3DXToRadian(fAngleY));

		matWorld = matWorld * matRotY;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		
		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &m_matProj);

		m_pMinimapPlayerTexCom->Set_Texture(pEffect, "g_BaseTexture");

		pEffect->CommitChanges();

		m_pBufferCom->Render_Buffer();

		if (Engine::SCENE_TOWN == Engine::GetSceneID())
		{
			DrawTownUnit(pEffect);
		}
		else if (Engine::SCENE_STAGE_ICE == Engine::GetSceneID())
		{
			//DrawIceUnit(pEffect);
		}
		else if (Engine::SCENE_STAGE_FIRE == Engine::GetSceneID())
		{
		}

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
	}
	else
	{
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

		Engine::SetUp_OnShader(L"Target_MiniMap", pEffect, "g_BaseTexture");
		m_pTextureCom->Set_Texture(pEffect, "g_AlphaTexture");

		pEffect->CommitChanges();

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
	}
}

HRESULT CMiniMap::Ready_Object(void)
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fX = 1500.0f;
	m_fY = 100;

	m_fSizeX = 180;
	m_fSizeY = 180;

	m_bIsStatic = true;

	return S_OK;
}

HRESULT CMiniMap::Add_Component(void)
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

	// For.Texture Component 
	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Texture_MiniMap_Alpha");
	pComponent = (Engine::CComponent*)m_pTextureCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TextureAlpha", pComponent));

	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_MiniMap");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if (NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));


	pComponent = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MiniMap_Town");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TexMiniMap_Town", pComponent));

	pComponent = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MiniMap_Ice");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TexMiniMap_Ice", pComponent));

	pComponent = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MiniMap_Fire");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TexMiniMap_Fire", pComponent));

	m_pMinimapPlayerTexCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Texture_MiniMap_Player");
	pComponent = (Engine::CComponent*)m_pMinimapPlayerTexCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TexMiniMap_Player", pComponent));

	m_pMinimapNPCTexCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MiniNpc");
	pComponent = (Engine::CComponent*)m_pMinimapNPCTexCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TexMiniNpc", pComponent));

	m_pMinimapMonsterTexCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MiniMonster");
	pComponent = (Engine::CComponent*)m_pMinimapMonsterTexCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TexMiniMonster", pComponent));

	m_pMinimapBossTexCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"MiniBoss");
	pComponent = (Engine::CComponent*)m_pMinimapBossTexCom;
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TexMiniBoss", pComponent));
	
	return S_OK;
}

CMiniMap * CMiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring & wstrName, const Engine::OBJECTSTATE & eState, const _bool bIsActive)
{
	CMiniMap* pMiniMap = new CMiniMap(pGraphicDev, wstrName);

	if (FAILED(pMiniMap->Ready_Object()))
	{
		MSG_BOX("MiniMap Created Failed");
		::Safe_Release(pMiniMap);
	}

	return pMiniMap;
}

void CMiniMap::Free(void)
{
	if (m_listMiniUnit.size() != 0)
	{
		list<Engine::CGameObject*>::iterator iter = m_listMiniUnit.begin();
		list<Engine::CGameObject*>::iterator iter_end = m_listMiniUnit.end();

		for (; iter != iter_end; ++iter)
		{
			::Safe_Release(*iter);
		}

		m_listMiniUnit.clear();
	}

	CUI::Free();
}

void CMiniMap::FindNPC(void)
{
	if (m_listMiniUnit.size() != 0)
	{
		list<Engine::CGameObject*>::iterator iter = m_listMiniUnit.begin();
		list<Engine::CGameObject*>::iterator iter_end = m_listMiniUnit.end();
		
		for (; iter != iter_end; ++iter)
		{
			::Safe_Release(*iter);
		}

		m_listMiniUnit.clear();
	}

	map<wstring, list<CGameObject*>>* LayerMap = Engine::FindLayer(Engine::LAYER_GAMELOGIC)->Get_MapObject();

	map<wstring, list<CGameObject*>>::iterator iter = LayerMap->begin();
	map<wstring, list<CGameObject*>>::iterator iter_end = LayerMap->end();
	
	for (; iter != iter_end; ++iter)
	{
		if (L"Mesh_NPC_Weapon" == iter->first)
		{
			list<CGameObject*>::iterator iterGO = iter->second.begin();
			list<CGameObject*>::iterator iterGO_end = iter->second.end();

			for (; iterGO != iterGO_end; ++iterGO)
			{
				m_listMiniUnit.push_back(*iterGO);
			}
		}
		else if (L"Mesh_NPC_Armor" == iter->first)
		{
			list<CGameObject*>::iterator iterGO = iter->second.begin();
			list<CGameObject*>::iterator iterGO_end = iter->second.end();

			for (; iterGO != iterGO_end; ++iterGO)
			{
				m_listMiniUnit.push_back(*iterGO);
			}
		}
		else if (L"Mesh_NPC_Potion" == iter->first)
		{
			list<CGameObject*>::iterator iterGO = iter->second.begin();
			list<CGameObject*>::iterator iterGO_end = iter->second.end();

			for (; iterGO != iterGO_end; ++iterGO)
			{
				m_listMiniUnit.push_back(*iterGO);
			}
		}
		else if (L"Mesh_NPC_Book" == iter->first)
		{
			list<CGameObject*>::iterator iterGO = iter->second.begin();
			list<CGameObject*>::iterator iterGO_end = iter->second.end();

			for (; iterGO != iterGO_end; ++iterGO)
			{
				m_listMiniUnit.push_back(*iterGO);
			}
		}
	}

	//::Safe_Release(LayerMap);
}

void CMiniMap::DrawTownUnit(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	_matrix	matWorld, matView;	

	list<Engine::CGameObject*>::iterator iter = m_listMiniUnit.begin();
	list<Engine::CGameObject*>::iterator iter_end = m_listMiniUnit.end();

	for (; iter != iter_end; ++iter)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);

		matWorld._11 = 100.0f;
		matWorld._22 = 100.0f;
		matWorld._33 = 1.0f;

		if (L"Mesh_NPC_Weapon" == (*iter)->GetName())
		{
			matWorld._41 = static_cast<CNPC*>(*iter)->GetTransCom()->Get_Now_Position().x * 6.0f - (m_fMiniMapMove_X * 9.0f);
			matWorld._42 = static_cast<CNPC*>(*iter)->GetTransCom()->Get_Now_Position().z * 9.0f - (m_fMiniMapMove_Y * 9.0f);
		}
		else if (L"Mesh_NPC_Armor" == (*iter)->GetName())
		{
			matWorld._41 = static_cast<CNPC*>(*iter)->GetTransCom()->Get_Now_Position().x * 0.0f - (m_fMiniMapMove_X * 9.0f);
			matWorld._42 = static_cast<CNPC*>(*iter)->GetTransCom()->Get_Now_Position().z * 9.0f - (m_fMiniMapMove_Y * 9.0f);
		}		
		else if (L"Mesh_NPC_Potion" == (*iter)->GetName())
		{
			matWorld._41 = static_cast<CNPC*>(*iter)->GetTransCom()->Get_Now_Position().x * 20.0f - (m_fMiniMapMove_X * 9.0f);
			matWorld._42 = static_cast<CNPC*>(*iter)->GetTransCom()->Get_Now_Position().z * 10.0f - (m_fMiniMapMove_Y * 9.0f);
		}
		else if (L"Mesh_NPC_Book" == (*iter)->GetName())
		{
			matWorld._41 = static_cast<CNPC*>(*iter)->GetTransCom()->Get_Now_Position().x * 12.0f - (m_fMiniMapMove_X * 9.0f);
			matWorld._42 = static_cast<CNPC*>(*iter)->GetTransCom()->Get_Now_Position().z * 7.0f - (m_fMiniMapMove_Y * 9.0f);
		}
		
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

		pEffect->SetMatrix("g_matWorld", &matWorld);
		pEffect->SetMatrix("g_matView", &matView);
		pEffect->SetMatrix("g_matProj", &m_matProj);

		m_pMinimapNPCTexCom->Set_Texture(pEffect, "g_BaseTexture");

		pEffect->CommitChanges();

		m_pBufferCom->Render_Buffer();
	}

	::Safe_Release(pEffect);
}

void CMiniMap::StageIceFindMonster(void)
{
	map<wstring, list<CGameObject*>>* LayerMap = Engine::FindLayer(Engine::LAYER_GAMELOGIC)->Get_MapObject();

	map<wstring, list<CGameObject*>>::iterator iter = LayerMap->begin();
	map<wstring, list<CGameObject*>>::iterator iter_end = LayerMap->end();

	for (; iter != iter_end; ++iter)
	{
		if (L"Monster" == iter->first)
		{
			list<CGameObject*>::iterator iterGO = iter->second.begin();
			list<CGameObject*>::iterator iterGO_end = iter->second.end();

			for (; iterGO != iterGO_end; ++iterGO)
			{
				if (L"Mesh_Hanuman" == (*iterGO)->GetName())
				{
					m_pStageBoss = (*iterGO);
				}				
			}
		}		
	}
}

void CMiniMap::DrawIceUnit(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();
	
	_matrix	matWorld, matView;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = 100.0f;
	matWorld._22 = 100.0f;
	matWorld._33 = 1.0f;

	matWorld._42 = static_cast<CMonster*>(m_pStageBoss)->GetTransCom()->Get_Now_Position().z * 20.0f;// - (m_fMiniMapMove_Y * 3.25f) + 1000.0f;
	matWorld._41 = static_cast<CMonster*>(m_pStageBoss)->GetTransCom()->Get_Now_Position().x * 10.0f;// - (m_fMiniMapMove_X * 3.25f) + -275.0f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	m_pMinimapBossTexCom->Set_Texture(pEffect, "g_BaseTexture");

	pEffect->CommitChanges();

	m_pBufferCom->Render_Buffer();
	

	::Safe_Release(pEffect);
}

void CMiniMap::StageFireFindMonster(void)
{
}

void CMiniMap::DrawFireUnit(LPD3DXEFFECT pEffect)
{
}

//CMiniMap::CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev, 
//				   const wstring& wstrName, 
//				   const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, 
//				   const _bool bIsActive /*= TRUE*/)
//: CUI(pGraphicDev, wstrName)
//, m_pRendererCom(NULL)
//, m_pTransCom(NULL)
//, m_pBufferCom(NULL)
//, m_pShaderCom(NULL)
//, m_pCurBck(NULL)
//, m_pCurDpt(NULL)
//, m_pMapCamera(NULL)
//, m_pMiniPlayer(NULL)
//, m_iTxW(0)
//{
//	memset(&m_RndSf, 0, sizeof(m_RndSf));
//}
//
//CMiniMap::~CMiniMap(void)
//{
//
//}
//
//HRESULT CMiniMap::Ready_Object(void)
//{
//	if(FAILED(Add_Component()))
//		return E_FAIL;
//
//	m_pMiniPlayer = CMiniPlayer::Create(m_pGraphicDev, L"MiniPlayer");
//	if(NULL == m_pMiniPlayer) return E_FAIL;		
//
//	m_fX = 1500.f;
//	m_fY = 100;
//
//	m_fSizeX = 180;
//	m_fSizeY = 180;
//
//	m_bIsStatic = true;
//
//	return S_OK;
//}
//
//HRESULT CMiniMap::Start_ForScene(void)
//{
//
//	if (Engine::GetSceneID() != Engine::SCENE_TOWN && Engine::GetSceneID() != Engine::SCENE_STAGE_ICE && Engine::GetSceneID() != Engine::SCENE_STAGE_FIRE)
//	{
//		m_bIsActive = false;
//		return S_OK;
//	}
//	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
//		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
//		m_bIsActive = true;
//
//	m_pMapCamera = Engine::GetCamera(Engine::CAMERA_MINIMAP);
//	static_cast<CMapCamera*>(m_pMapCamera)->Start_ForScene();
//
//	if (Engine::GetSceneID() == Engine::SCENE_TOWN)
//	{
//		
//		m_pMiniPlayer->Start_ForScene();
//	}
//	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
//	{
//		
//		m_pMiniPlayer->Start_ForScene();
//	}
//	else if (Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
//	{
//		
//		m_pMiniPlayer->Start_ForScene();
//	}
//
//	if (Engine::GetSceneID() == Engine::SCENE_LOADING)
//		m_bIsActive = false;
//	if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE
//		|| Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
//		m_bIsActive = true;
//
//	return S_OK;
//}
//
//void CMiniMap::Update_Object(const _float& fTimeDelta)
//{
//	if (CFade_Manager::GetInstance()->GetFading() == true)
//		return;
//
//	m_pRendererCom->Add_RenderGroup(Engine::RENDER_MINIMAP, this);
//
//	static_cast<CMapCamera*>(m_pMapCamera)->Update_Object(fTimeDelta);
//	CUI::Update_Object(fTimeDelta);
//	
//	D3DXMatrixOrthoLH(&m_matProj, float(WINSIZEX), float(WINSIZEY), 0.0f, 1.0f);
//}
//
//void CMiniMap::Render_Object(void)
//{		
//	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
//	if(NULL == pEffect)			return;
//
//	//_uint			iPassCnt;
//
//	if (Engine::MRT_MINIMAP == m_pRendererCom->GetMRTID())
//	{
//		m_pMiniPlayer->Render_Object();
//	}
//	else if (Engine::MRT_MINIMAP_DRAW == m_pRendererCom->GetMRTID())
//	{
//		_matrix				matWorld, matView;
//		_matrix				matOldView, matOldProj;
//
//		D3DXMatrixIdentity(&matWorld);
//		D3DXMatrixIdentity(&matView);
//
//		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
//		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);
//
//		matWorld._11 = m_fSizeX;
//		matWorld._22 = m_fSizeY;
//		matWorld._33 = 1.f;
//		matWorld._41 = m_fX - WINSIZEX * 0.5f;
//		matWorld._42 = -m_fY + WINSIZEY * 0.5f;
//
//		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
//		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
//		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
//
//		pEffect->SetMatrix("g_matWorld", &matWorld);
//		pEffect->SetMatrix("g_matView", &matView);
//		pEffect->SetMatrix("g_matProj", &m_matProj);
//
//		Engine::SetUp_OnShader(L"Target_MiniMap", pEffect, "g_BaseTexture");
//		m_pTextureCom->Set_Texture(pEffect, "g_AlphaTexture");
//
//		pEffect->Begin(NULL, 0);
//		pEffect->BeginPass(0);
//
//		//RECT	rc	 = { 0, 0, m_iTxW, m_iTxW };
//		//_vec3	vPos = _vec3(1300, 25, 0);//_vec3(1, 0, 0) * m_iTxW;
//
//		//m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
//		//m_pd3dSprite->Draw(m_RndSf.pTx, &rc, NULL, &vPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//
//		//m_pd3dSprite->End();
//
//		m_pBufferCom->Render_Buffer();
//
//		pEffect->EndPass();
//		pEffect->End();
//
//		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
//		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
//	}	
//}
//
//void CMiniMap::Render_Object(LPD3DXEFFECT pEffect, const _uint iPassIdx)
//{
//	if (3 == iPassIdx)
//	{
//		m_pMiniPlayer->Render_Object();
//		
//		if (Engine::GetSceneID() == Engine::SCENE_TOWN || Engine::GetSceneID() == Engine::SCENE_STAGE_FIRE)
//		{
//			
//		}
//		else if (Engine::GetSceneID() == Engine::SCENE_STAGE_ICE)
//		{
//			
//		}
//	}
//	else
//	{
//		_matrix				matWorld, matView;
//		_matrix				matOldView, matOldProj;
//
//		D3DXMatrixIdentity(&matWorld);
//		D3DXMatrixIdentity(&matView);
//
//		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
//		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);
//
//		matWorld._11 = m_fSizeX;
//		matWorld._22 = m_fSizeY;
//		matWorld._33 = 1.f;
//		matWorld._41 = m_fX - WINSIZEX * 0.5f;
//		matWorld._42 = -m_fY + WINSIZEY * 0.5f;
//
//		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
//		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
//		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
//
//		pEffect->SetMatrix("g_matWorld", &matWorld);
//		pEffect->SetMatrix("g_matView", &matView);
//		pEffect->SetMatrix("g_matProj", &m_matProj);
//
//		Engine::SetUp_OnShader(L"Target_MiniMap", pEffect, "g_BaseTexture");
//		m_pTextureCom->Set_Texture(pEffect, "g_AlphaTexture");
//
//		pEffect->CommitChanges();
//		
//		m_pBufferCom->Render_Buffer();
//		
//		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
//		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
//	}
//}
//
//HRESULT CMiniMap::Add_Component(void)
//{
//	Engine::CComponent*				pComponent = NULL;
//
//	// For.Transform Component
//	pComponent = m_pTransCom = Engine::CTransform::Create(); 
//	if(NULL == pComponent)
//		return E_FAIL;
//	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
//
//	// For.Buffer Component
//	m_pBufferCom = (Engine::CRect_Texture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
//	pComponent = (Engine::CComponent*)m_pBufferCom;
//	if(NULL == pComponent)
//		return E_FAIL;
//	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));
//
//	// For.Texture Component 
//	m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGESTATIC, L"Texture_MiniMap_Alpha");
//	pComponent = (Engine::CComponent*)m_pTextureCom;
//	if(NULL == pComponent)
//		return E_FAIL;
//	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_TextureAlpha", pComponent));
//
//	pComponent = m_pShaderCom = (Engine::CShader*)Engine::Clone_Prototype(L"Shader_MiniMap");
//	if(NULL == pComponent)
//		return E_FAIL;
//	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));
//
//	// For.Renderer Component
//	pComponent = m_pRendererCom = Engine::Get_Renderer();
//	if(NULL == pComponent)
//		return E_FAIL;
//	pComponent->AddRef();
//	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));
//
//	return S_OK;
//}
//
//CMiniMap* CMiniMap::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
//						   const wstring& wstrName, 
//						   const Engine::OBJECTSTATE& eState /*= Engine::STATE_DEFAULT*/, 
//						   const _bool bIsActive /*= TRUE*/)
//{
//	CMiniMap* pMiniMap = new CMiniMap(pGraphicDev, wstrName);
//
//	if(FAILED(pMiniMap->Ready_Object()))
//	{
//		MSG_BOX("MiniMap Created Failed");
//		::Safe_Release(pMiniMap);
//	}
//
//	return pMiniMap;
//}
//
//void CMiniMap::Free(void)
//{
//	m_RndSf.Invalidate();
//
//	::Safe_Release(m_pMiniPlayer);
//
//	CUI::Free();
//}

