#include "stdafx.h"

#include "Trap.h"

#include "Export_Engine.h"
#include "VIBuffer.h"

#include "Camera_MapTool.h"
#include "MouseCol.h"
#include "PickingRect.h"
#include "Map_Ice.h"
#include "Map_Fire.h"


#include "Environment.h"
#include "NPC.h"
#include "Player.h"
#include "Cube.h"
#include "Babegazi_Warrior_Ani.h"

//맵툴 Monster

CTrap::CTrap(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
, m_pAnimatorCom(NULL)
, m_pCalculatorCom(NULL)
, m_pRigidbodyCom(NULL)
, m_pResourceMgr(NULL)
, m_pPickingRect(NULL)
, m_pMouseCol(NULL)
, m_pPickingVtx(NULL)
, m_lMouseMove(0)
, m_iAniIdx(0)
, m_iMonsterAniState(0)
, m_fAlpha(0.f)
, m_bStop(false)
, m_bComplete(false)
, m_bCheck(false)
, m_bCreateRect(false)
, m_bCubeRayCol(false)
{
	ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
}
CTrap::~CTrap(void)
{

}

HRESULT CTrap::Start_ForScene(void)
{
	Engine::CGameObject*	pGameObject = NULL;

	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter_map = Engine::FindList(L"Map_Ice")->begin();
		pGameObject = ((CMap_Ice*)(*iter_map));

	}

	if(dynamic_cast<CMap_Ice*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Ice*)pGameObject)->GetMapMesh(&iSize);

		for(_int i = 0; i < iSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(ppMesh[i]->Get_Mesh());
		}
	}
	////////////////////////////////////////////////////////Ice
	if (NULL != Engine::FindList(L"Map_Fire"))
	{
		list<Engine::CGameObject*>::iterator iter_map = Engine::FindList(L"Map_Fire")->begin();
		pGameObject = ((CMap_Fire*)(*iter_map));

	}

	if (dynamic_cast<CMap_Fire*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Fire*)pGameObject)->GetMapMesh(&iSize);

		for (_int i = 0; i < iSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(ppMesh[i]->Get_Mesh());
		}
	}
////////////////////////////////////////////////////////Fire



	return S_OK;
}

void CTrap::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);
	if(!m_bCreateRect)
	{
		m_pPickingRect = CPickingRect::Create(m_pGraphicDev, L"PickingRect");
		m_bCreateRect = true;
	}
	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	_vec3 vPos = m_pTransCom->Get_Now_Position();
	_vec3 vDis;



	SettingWork();
	


	vPos.y += 10.f;
	m_pPickingRect->Set_Positon(&vPos);


	_vec3	vScale;

	m_pPickingRect->Update_Object(fTimeDelta);

	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

}

void CTrap::Render_Object(void)
{
	m_pMeshCom->Play_AnimationSet(0.f);
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	if(NULL == pEffect) return;

	pEffect->AddRef();

	if(FAILED(Set_ConstantTable(pEffect))) return;

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(1);

	m_pMeshCom->Render_Mesh(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Engine::Safe_Release(pEffect);

	m_pPickingRect->Render_Object();

}


HRESULT CTrap::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	if(FAILED(LoadAniInfo(L"Babegazi_Warrior_Ani")))
		return E_FAIL;

	m_iMonsterAniState = MONSTER_NORMAL;

	m_eLayerType = Engine::LAYER_GAMELOGIC;
	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);
	m_eMode = MODE_MOVE;

	m_pMouseCol = CMouseCol::Create();
	m_pPickingVtx = new Engine::VTXTEX[4]; 

	//m_pMeshCom->Set_AnimationSet(STAND_IDLE);

	//애니메이션 상태 준비
	Engine::CAnimationState* pState = NULL;

	//	Engine::CAnimationState* pState = NULL;

	pState = CBabegazi_Warrior_Ani::Create(m_pMeshCom, &m_pmapNextMotion[MONSTER_NORMAL], MONSTER_NORMAL);
	if(pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(MONSTER_NORMAL, pState);

	pState = CBabegazi_Warrior_Ani::Create(m_pMeshCom, &m_pmapNextMotion[MONSTER_FIGHT], MONSTER_FIGHT);
	if(pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(MONSTER_FIGHT, pState);

	m_iAniIdx = STAND_IDLE;

	//애니메이터 세팅
	m_pAnimatorCom->Set_Animator(&m_vecAniInfo);
	m_pAnimatorCom->Set_State(m_iMonsterAniState);
	m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);

	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_vStartPos);

	Start_ForScene();

	return S_OK;
}

HRESULT CTrap::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"0Transform", m_pTransCom));


	//Mesh..
	wstring::iterator iter = m_wstrName.begin();
	wstring::iterator iter_end = m_wstrName.end();

	TCHAR szName[MAX_PATH] = L"";

	int i = 0;

	for(; iter != iter_end; ++iter)
	{
		szName[i] = (*iter);
		++i;
	}

	//pComponent = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, szName);
	//m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"StaticMesh", m_pMeshCom));

	pComponent = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGE, szName);
	m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"DynamicMesh", m_pMeshCom));

	// For.Renderer Component
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	if(NULL == pComponent)
		return E_FAIL;
	pComponent->AddRef();
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Renderer", pComponent));

	//Shader
	pComponent = m_pShaderCom = static_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Player"));
	if(NULL == pComponent) return E_FAIL;
	m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"Com_Shader", pComponent));

	//Animator
	pComponent = Engine::CAnimator::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pAnimatorCom = (Engine::CAnimator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"3Animator", pComponent));

	//Calculator
	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	if(pComponent == NULL)
		return E_FAIL;
	m_pCalculatorCom = (Engine::CCalculator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"2Calculator", pComponent));
	m_pCalculatorCom->Set_Transform(m_pTransCom);

	//Rigidbody
	pComponent = Engine::CRigidbody::Create(m_pTransCom, m_pCalculatorCom);
	if(pComponent == NULL)
		return E_FAIL;
	m_pRigidbodyCom = (Engine::CRigidbody*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"1Rigidbody", pComponent));
	m_pRigidbodyCom->Set_Gravity(false);

	return S_OK;
}

HRESULT CTrap::Set_ConstantTable( LPD3DXEFFECT pEffect )
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

void CTrap::SettingWork( void )
{
	_vec3	vScale;

	if(m_eMode == MODE_MOVE)
	{
		m_fAlpha = 1.f;
		m_pTransCom->Set_Position(&g_vViewMouse);
		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB))
			m_eMode = MODE_ANGLE;
	}
	else if(m_eMode == MODE_ANGLE)
	{
		m_fAlpha = 1.f;
		if(m_lMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_X))
		{
			m_pTransCom->Rotation(Engine::ROT_Y, 90.f * m_lMouseMove * 0.01f);
		}
		//if(m_lMouseMove = Engine::Get_DIMouseMove(Engine::CInput_Device::DIMS_Y))
		//{
		//	m_pTransCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f * m_lMouseMove * 0.01f));
		//}
		if(Engine::Get_DIKeyState(DIK_UP) & 0x80)
		{
			m_pTransCom->Set_Scale((m_pTransCom->Get_Scale(&vScale))->x + 0.01f,
				(m_pTransCom->Get_Scale(&vScale))->y + 0.01f,
				(m_pTransCom->Get_Scale(&vScale))->z + 0.01f);
		}
		if(Engine::Get_DIKeyState(DIK_DOWN) & 0x80)
		{
			m_pTransCom->Set_Scale((m_pTransCom->Get_Scale(&vScale))->x - 0.01f,
				(m_pTransCom->Get_Scale(&vScale))->y - 0.01f,
				(m_pTransCom->Get_Scale(&vScale))->z - 0.01f);
		}

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
		{
			m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_vStartPos);
			m_eMode = MODE_STOP;
		}
	}

	else if(m_eMode == MODE_STOP)
	{
		m_fAlpha = 1.f;
		_matrix*	matRotWorld = NULL;

		if(g_vMouse.x > 15.f)
		{
			float fDist = 999999999.f;
			Engine::VTXTEX* pVtx = NULL;
			((Engine::CVIBuffer*)m_pPickingRect->Get_BufferCom())->GetVtxInfo(m_pPickingVtx);

			_matrix matWorld;
			m_pPickingRect->Get_TransCom()->Get_WorldMatrix(&matWorld);
			if(m_pMouseCol->PickRect(&g_vMouse, m_pPickingVtx, &matWorld, fDist))
			{
				if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_LB))
				{
					Engine::CGameObject* pGameObject = NULL;
					if(NULL != Engine::FindList(L"NPC"))
					{
						list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"NPC")->begin();
						list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"NPC")->end();
						for(iter; iter != iter_end;iter++)
						{
							if(((CNPC*)(*iter))->GetMode() == MODE_MODIFY)
							{
								((CNPC*)(*iter))->SetMode(MODE_STOP);
							}
						}
					}

					if(NULL != Engine::FindList(L"Environment"))
					{
						list<Engine::CGameObject*>::iterator  iter_Env = Engine::FindList(L"Environment")->begin();
						list<Engine::CGameObject*>::iterator iter_Env_end = Engine::FindList(L"Environment")->end();
						for(iter_Env; iter_Env != iter_Env_end;iter_Env++)
						{
							if(((CEnvironment*)(*iter_Env))->GetMode() == MODE_MODIFY)
							{
								((CEnvironment*)(*iter_Env))->SetMode(MODE_STOP);
							}
						}
					}

					if(NULL != Engine::FindList(L"Cube"))
					{
						list<Engine::CGameObject*>::iterator  iter_Env = Engine::FindList(L"Cube")->begin();
						list<Engine::CGameObject*>::iterator iter_Env_end = Engine::FindList(L"Cube")->end();
						for(iter_Env; iter_Env != iter_Env_end;iter_Env++)
						{
							if(((CCube*)(*iter_Env))->GetMode() == MODE_MODIFY)
							{
								((CCube*)(*iter_Env))->SetMode(MODE_STOP);
							}
						}
					}

					if(NULL != Engine::FindList(L"Monster"))
					{
						list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Monster")->begin();
						list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Monster")->end();
						for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
						{
							if(((CTrap*)(*iter_Monster))->GetMode() == MODE_MODIFY)
							{
								((CTrap*)(*iter_Monster))->SetMode(MODE_STOP);
							}
						}
					}

					m_eMode = MODE_MODIFY;
					m_fAlpha = 0.5f;
				}
			}
		}
	}
	else if(m_eMode == MODE_MODIFY)
	{
		if(Engine::Get_DIKeyState(DIK_UP) & 0x80)
		{
			m_pTransCom->Set_Scale((m_pTransCom->Get_Scale(&vScale))->x + 0.01f,
				(m_pTransCom->Get_Scale(&vScale))->y + 0.01f,
				(m_pTransCom->Get_Scale(&vScale))->z + 0.01f);
		}
		if(Engine::Get_DIKeyState(DIK_DOWN) & 0x80)
		{
			m_pTransCom->Set_Scale((m_pTransCom->Get_Scale(&vScale))->x - 0.01f,
				(m_pTransCom->Get_Scale(&vScale))->y - 0.01f,
				(m_pTransCom->Get_Scale(&vScale))->z - 0.01f);
		}

		if(Engine::Get_DIKeyState(DIK_LEFT) & 0x80)
		{
			m_pTransCom->Rotation(Engine::ROT_Y, 90.f * 0.01f);
		}

		if(Engine::Get_DIKeyState(DIK_RIGHT) & 0x80)
		{
			m_pTransCom->Rotation(Engine::ROT_Y, -90.f * 0.01f);
		}

		if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB))
		{
			m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_vStartPos);
			m_eMode = MODE_STOP;
		}
	}

	//else
	//{
	//	if(Engine::Get_DIMouseState(Engine::CInput_Device::DIM_RB))
	//	{
	//		m_eMode = MODE_STOP;
	//	}
	//}

	//mfc 슬라이더 컨트롤 최초 1회만 수정하기 위함.
	if(m_eMode != MODE_MODIFY)
	{
		m_bCheck = false;
	}

}



void CTrap::Stop( void )
{
	m_bStop = true;
}

void CTrap::Complete( void )
{
	m_bComplete = true;
}

OBJECT_MODE CTrap::GetMode( void )
{
	return m_eMode;
}

void CTrap::SetMode( OBJECT_MODE eMode )
{
	m_eMode = eMode;
}

Engine::CTransform* CTrap::GetTransCom( void )
{
	return m_pTransCom;
}

_bool CTrap::GetChecked( void )
{
	return m_bCheck;
}

void CTrap::CheckTrue( void )
{
	m_bCheck = true;
}

void CTrap::CheckFalse( void )
{
	m_bCheck = false;
}

OBJ_INFO* CTrap::GetObjectInfo( void )
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

	wstring wstrTag = L"Trap";

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

void CTrap::SetObjectInfo( _vec3* vPos, _vec3* vScale, _vec3* vAngle )
{
	m_pTransCom->Set_Position(vPos);
	m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
	m_eMode = MODE_STOP;
}


CTrap* CTrap::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CTrap*		pInstance = new CTrap(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGrid Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CTrap::Free(void)
{
	
	Safe_Delete(m_pMouseCol);
	Engine::Safe_Release(m_pPickingRect);
	Engine::Safe_Delete(m_pPickingVtx);

	_uint iSize = m_vecAniInfo.size();
	for (_uint i = 0; i < iSize; i++)
	{
		Engine::Safe_Delete_Array(m_vecAniInfo[i]->pMotionEventInfo);
		Engine::Safe_Delete_Array(m_vecAniInfo[i]->pUse);
		Engine::Safe_Delete(m_vecAniInfo[i]);
	}
	m_vecAniInfo.clear();
	for (_uint i = 0; i < MONSTER_END; i++)
	{
		map<_uint, list<Engine::ANIMATORINFO*>>::iterator iter = m_pmapNextMotion[i].begin();
		map<_uint, list<Engine::ANIMATORINFO*>>::iterator iter_end = m_pmapNextMotion[i].end();

		for (iter; iter != iter_end; iter++)
		{
			list<Engine::ANIMATORINFO*>::iterator iter_list = iter->second.begin();
			list<Engine::ANIMATORINFO*>::iterator iter_list_end = iter->second.end();
			for (iter_list; iter_list != iter_list_end; iter_list++)
			{
				Engine::Safe_Delete((*iter_list));
			}
		}
		m_pmapNextMotion[i].clear();
	}
	Engine::Safe_Delete_Array(m_pmapNextMotion);

	Engine::CGameObject::Free();
}

HRESULT CTrap::LoadAniInfo( const _tchar* pFileName )
{
	if(pFileName == NULL)
		return E_FAIL;

	_tchar szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, L"../../../Client/Bin/Data/MonsterAnimationData/");
	lstrcat(szFullPath, pFileName);
	lstrcat(szFullPath, L".dat");


	DWORD dwByte = 0;
	HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
		return E_FAIL;

	_uint iMaxAniCnt = 0;
	_uint iContainerSize = 0;

	ReadFile(hFile, &iMaxAniCnt, sizeof(_uint), &dwByte, NULL);
	ReadFile(hFile, &iContainerSize, sizeof(_uint), &dwByte, NULL);

	if (iMaxAniCnt == 0)
		return E_FAIL;

	m_vecAniInfo.reserve(iMaxAniCnt);
	m_pmapNextMotion = new	map<_uint, list<Engine::ANIMATORINFO*>>[iContainerSize];

	for (_uint i = 0; i < iMaxAniCnt; i++)
	{
		Engine::ANIINFO* pAniInfo = new Engine::ANIINFO;
		ReadFile(hFile, &pAniInfo->tMotionInfo, sizeof(Engine::MOTIONINFO), &dwByte, NULL);
		_uint iEventSize = 0;
		ReadFile(hFile, &iEventSize, sizeof(_uint), &dwByte, NULL);
		pAniInfo->pMotionEventInfo = new Engine::MOTIONEVENTINFO[iEventSize];
		for (_uint j = 0; j < iEventSize; j++)
		{
			ReadFile(hFile, &pAniInfo->pMotionEventInfo[j], sizeof(Engine::MOTIONEVENTINFO), &dwByte, NULL);
		}
		for (_uint j = 0; j < iContainerSize; j++)
		{
			list<Engine::ANIMATORINFO*> listAnimator;
			_uint iNextMotionSize = 0;
			ReadFile(hFile, &iNextMotionSize, sizeof(_uint), &dwByte, NULL);
			for (_uint k = 0; k < iNextMotionSize; k++)
			{
				Engine::ANIMATORINFO* pAnimatorInfo = new Engine::ANIMATORINFO;
				ReadFile(hFile, pAnimatorInfo, sizeof(Engine::ANIMATORINFO), &dwByte, NULL);
				listAnimator.push_back(pAnimatorInfo);
			}
			m_pmapNextMotion[j].insert(map<_uint, list<Engine::ANIMATORINFO*>>::value_type(i, listAnimator));
		}
		pAniInfo->pUse = new _bool[iContainerSize];
		for (_uint j = 0; j < iContainerSize; j++)
		{
			ReadFile(hFile, &pAniInfo->pUse[j], sizeof(_bool), &dwByte, NULL);
		}

		m_vecAniInfo.push_back(pAniInfo);
	}

	CloseHandle(hFile);

	return S_OK;
}









