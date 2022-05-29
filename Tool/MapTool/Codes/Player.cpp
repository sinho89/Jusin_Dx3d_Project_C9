#include "stdafx.h"

#include "Player.h"

#include "Export_Engine.h"
#include "VIBuffer.h"

#include "Camera_MapTool.h"
#include "MouseCol.h"
#include "PickingRect.h"
#include "NavigationMesh.h"

#include "Environment.h"
#include "NPC.h"
#include "Cube.h"
#include "Map_Ice.h"
#include "Map_Store.h"
#include "Monster.h"

#include "Player_Ani.h"




//맵툴 Player

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
, m_pResourceMgr(NULL)
, m_pPickingRect(NULL)
, m_pMouseCol(NULL)
, m_pPickingVtx(NULL)
, m_pNaviMeshCom(NULL)
, m_pAnimatorCom(NULL)
, m_pCalculatorCom(NULL)
, m_pRigidbodyCom(NULL)
, m_lMouseMove(0)
, m_iAniIdx(0)
, m_iPlayerAniState(0)
, m_fAlpha(0.f)
, m_bStop(false)
, m_bComplete(false)
, m_bCheck(false)
, m_bCreateRect(false)
{
	ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
	ZeroMemory(&m_bPush, sizeof(_bool) * KEY_END);
}
CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Start_ForScene(void)
{
	//Engine::CGameObject* pGameObject = Engine::FindWithTag(L"Map");
	Engine::CGameObject*	pGameObject = NULL;

	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter_map = Engine::FindList(L"Map_Ice")->begin();
		//m_pNaviMeshCom = ((CMap_Ice*)(*iter_map))->GetNaviCom();
		pGameObject = ((CMap_Ice*)(*iter_map));

	}
	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter_map = Engine::FindList(L"Map_Store")->begin();
		//m_pNaviMeshCom = ((CMap_Ice*)(*iter_map))->GetNaviCom();
		pGameObject = ((CMap_Store*)(*iter_map));

	}
	if(pGameObject == NULL)
		return E_FAIL;

	if(dynamic_cast<CMap_Store*>(pGameObject) != NULL)
	{
		_int iSize = 0;
		Engine::CStaticMesh** ppMesh = ((CMap_Store*)pGameObject)->GetMapMesh(&iSize);

		for(_int i = 0; i < iSize; i++)
		{
			m_pCalculatorCom->Set_TargetMesh(ppMesh[i]->Get_Mesh());
		}
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

	//Engine::Safe_Release(pGameObject);
	return S_OK;
}

void CPlayer::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	if(!m_bCreateRect)
	{
		m_pPickingRect = CPickingRect::Create(m_pGraphicDev, L"PickingRect");
		m_bCreateRect = true;
	}

	m_pMeshCom->Play_AnimationSet(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	if(m_eMode == MODE_DEBUG)
	{
		if(m_pCalculatorCom->Get_IsCol())
			m_pRigidbodyCom->Set_Gravity(false);
		else
		{
			m_pRigidbodyCom->Set_Gravity(true);
		}

		DebugWork(fTimeDelta);
	}
	else
	{
		SettingWork();
	}

	_vec3 vPos = *m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
	vPos.y += 10.f;
	m_pPickingRect->Set_Positon(&vPos);


	_vec3	vScale;

	m_pPickingRect->Update_Object(fTimeDelta);

	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

}

void CPlayer::Render_Object(void)
{
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


HRESULT CPlayer::Ready_Object(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	if(FAILED(LoadAniInfo(L"WitchBladeAni")))
		return E_FAIL;
	m_iPlayerAniState = PLAYER_TOWN;

	m_eLayerType = Engine::LAYER_GAMELOGIC;
	m_pTransCom->Set_Position(0.0f, 0.0f, 0.0f);
	//m_pTransCom->Rotation(Engine::ROT_Y, m_pTransCom->Get_Angle(_vec3(0.f,0.f,0.f)));
	//m_pTransCom->m_vScale = _vec3(100.f, 100.f, 100.f);
	m_eMode = MODE_MOVE;
	m_pMouseCol = CMouseCol::Create();
	m_pPickingVtx = new Engine::VTXTEX[4]; 

	Start_ForScene();

	//애니메이션 상태 준비
	Engine::CAnimationState* pState = NULL;
	m_iAniIdx = WITCH_IDLE;
	m_pMeshCom->Set_AnimationSet(WITCH_IDLE);

//	Engine::CAnimationState* pState = NULL;

	pState = CPlayer_Ani::Create(m_pMeshCom, &m_pmapNextMotion[PLAYER_TOWN], PLAYER_TOWN);
	if(pState == NULL)
		return E_FAIL;
	m_pAnimatorCom->Set_State(PLAYER_TOWN, pState);

	//애니메이터 세팅
	m_pAnimatorCom->Set_Animator(&m_vecAniInfo);
	m_pAnimatorCom->Set_State(m_iPlayerAniState);
	m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);



	//m_pNaviMeshCom->Set_CurrentCellIndex(0);

	return S_OK;
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent* pComponent = NULL;

	//Transform
	pComponent = Engine::CTransform::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pTransCom = (Engine::CTransform*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Transform", m_pTransCom));


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

	//Navi
	//pComponent = (Engine::CNavigationMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Navigation");
	//m_pNaviMeshCom = dynamic_cast<Engine::CNavigationMesh*>(pComponent);
	//m_mapComponent[Engine::COMID_STATIC].insert(MAPCOMPONENT::value_type(L"NaviMesh", m_pNaviMeshCom));

	//Animator
	pComponent = Engine::CAnimator::Create();
	if(pComponent == NULL)
		return E_FAIL;
	m_pAnimatorCom = (Engine::CAnimator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Animator", pComponent));

	//Calculator
	pComponent = Engine::CCalculator::Create(m_pGraphicDev);
	if(pComponent == NULL)
		return E_FAIL;
	m_pCalculatorCom = (Engine::CCalculator*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Calculator", pComponent));
	m_pCalculatorCom->Set_Transform(m_pTransCom);

	//Rigidbody
	pComponent = Engine::CRigidbody::Create(m_pTransCom, m_pCalculatorCom);
	if(pComponent == NULL)
		return E_FAIL;
	m_pRigidbodyCom = (Engine::CRigidbody*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"Rigidbody", pComponent));
	m_pRigidbodyCom->Set_Gravity(false);

	return S_OK;
}

HRESULT CPlayer::Set_ConstantTable( LPD3DXEFFECT pEffect )
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

HRESULT CPlayer::LoadAniInfo( const _tchar* pFileName )
{
	if(pFileName == NULL)
		return E_FAIL;

	_tchar szFullPath[MAX_PATH] = L"";
	lstrcpy(szFullPath, L"../../../Client/Bin/Data/PlayerAnimationData/");
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


void CPlayer::SettingWork( void )
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
			m_eMode = MODE_STOP;
	}

	else if(m_eMode == MODE_STOP)
	{
		m_fAlpha = 1.f;
		_matrix*	matRotWorld = NULL;

		if(g_vMouse.x > 15.f)
		{
			float fDist = 9999999999999999.f;
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

					if(NULL != Engine::FindList(L"Monster"))
					{
						list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Monster")->begin();
						list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Monster")->end();
						for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
						{
							if(((CMonster*)(*iter_Monster))->GetMode() == MODE_MODIFY)
							{
								((CMonster*)(*iter_Monster))->SetMode(MODE_STOP);
							}
						}
					}

					if(NULL != Engine::FindList(L"Cube"))
					{
						list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Cube")->begin();
						list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Cube")->end();
						for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
						{
							if(((CCube*)(*iter_Monster))->GetMode() == MODE_MODIFY)
							{
								((CCube*)(*iter_Monster))->SetMode(MODE_STOP);
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

void CPlayer::DebugWork( const _float& fTimeDelta )
{
	
	//CPlayerCamera* pCamera = (CPlayerCamera*)Engine::GetCamera(CAMERA_PLAYER);

	_vec3 vLook;
	m_pTransCom->Get_Infomation(Engine::INFO_LOOK, &vLook);
	_vec3 vPos;
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
	vPos.y += 3.f;
	_vec3 vRayDir = _vec3(0.f, -1.f, 0.f);
	D3DXVec3Normalize(&vRayDir, &vRayDir);

	//플레이어와 맵충돌...
	_float rDist = 9999.f;
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter_map = Engine::FindList(L"Map_Ice")->begin();
		m_pNaviMeshCom = NULL;
		m_pNaviMeshCom = ((CMap_Ice*)(*iter_map))->GetNaviCom();
	}
	MoveCheck(fTimeDelta);

}
void CPlayer::MoveCheck( const _float& fTimeDelta )
{
	const _vec3* pCamPos;
	if(NULL != Engine::FindList(L"Camera"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Camera")->begin();
		pCamPos = &((CCamera_MapTool*)(*iter))->GetCamEye();
	}

	_vec3 vLook;
	m_pTransCom->Get_Infomation(Engine::INFO_LOOK, &vLook);

	if(Engine::Get_DIKeyState(DIK_E) & 0x80)
	{
		Compute_CameraLook(pCamPos);
		m_pRigidbodyCom->Set_Jump(0.7f);
		m_bPush[KEY_E] = true;
		m_iAniIdx = WITCH_JUMPFA;
		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
	}
	else
		m_bPush[KEY_E] = false;



	if(Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		Compute_CameraLook(pCamPos);
		_vec3			vPos, vDir;
		m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
		m_pTransCom->Get_Infomation(Engine::INFO_LOOK, &vDir);
		_ulong PlayerIndex = m_pNaviMeshCom->Get_CurrentCellIndex(&vPos);

		//m_pTransCom->Move_Forward(15.f, fTimeDelta);

		//if(m_pCalculatorCom->Get_IsCol())
		//{
			m_pTransCom->Set_Position(&m_pNaviMeshCom->Move_OnNaviMesh_Tool(&vPos, &vDir, 0.2f, PlayerIndex));
		//}
		//else
		//{	
		//	m_pTransCom->Move_Forward(10.f, fTimeDelta);
		//}

		m_bPush[KEY_W] = true;
		m_iAniIdx = WITCH_RUNF;
		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
	}
	else
	{
		m_iAniIdx = WITCH_IDLE;
		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
		m_bPush[KEY_W] = false;
	}

	if(Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		Compute_CameraLook(pCamPos);
		m_pTransCom->Move_Forward(-(10.f / 3.f), fTimeDelta);
		m_bPush[KEY_S] = true;
		m_iAniIdx = WITCH_WALKB;
		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
	}
	else
		m_bPush[KEY_S] = false;

	if(Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		Compute_CameraLook(pCamPos);
		m_pTransCom->Move_Left(&vLook, 10.f, fTimeDelta);
		m_bPush[KEY_A] = true;
		m_iAniIdx = WITCH_RUNL;
		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
	}
	else
		m_bPush[KEY_A] = false;

	if(Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		Compute_CameraLook(pCamPos);
		m_pTransCom->Move_Right(&vLook, 10.f, fTimeDelta);
		m_bPush[KEY_D] = true;
		m_iAniIdx = WITCH_RUNR;
		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
	}
	else
		m_bPush[KEY_D] = false;

	if(m_pCalculatorCom->Get_IsCol())
	{
		switch(m_pAnimatorCom->Get_AnimationIndex())
		{
		case WITCH_JUMPFSC:
			m_iAniIdx = WITCH_JUMPFSD;
			m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
			break;

		case WITCH_JUMPBC:
			m_iAniIdx = WITCH_JUMPBD;
			m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
			break;

		case WITCH_JUMPLC:
			m_iAniIdx = WITCH_JUMPLD;
			m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
			break;

		case WITCH_JUMPRC:
			m_iAniIdx = WITCH_JUMPRD;
			m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
			break;
		}
	}
}



void CPlayer::Compute_CameraLook(const _vec3* pTargetPos)
{
	_vec3 vPos;
	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);

	_vec3 vDir = vPos - (*pTargetPos);
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3			vDirection;
	_matrix			matWorld;
	m_pTransCom->Get_WorldMatrix(&matWorld);
	memcpy(&vDirection, &matWorld.m[2][0], sizeof(_vec3));
	D3DXVec3Normalize(&vDirection, &vDirection);

	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vRight;

	D3DXVec3Cross(&vRight, &vUp, &vDirection);
	D3DXVec3Normalize(&vRight, &vRight);
	float fCos = D3DXVec3Dot(&vRight, &vDir);
	float fRealCos = D3DXVec3Dot(&vDirection, &vDir);
	float fAngle = acosf(fRealCos);

	fAngle = D3DXToDegree(fAngle);

	if(fAngle > 180.f)
		fAngle = 360.f - fAngle;

	if(fCos >= 0.001f)
		m_pTransCom->Rotation(Engine::ROT_Y, fAngle);
	else if(fCos < -0.001f)
		m_pTransCom->Rotation(Engine::ROT_Y, -fAngle);
}


void CPlayer::Stop( void )
{
	m_bStop = true;
}

void CPlayer::Complete( void )
{
	m_bComplete = true;
}

OBJECT_MODE CPlayer::GetMode( void )
{
	return m_eMode;
}

void CPlayer::SetMode( OBJECT_MODE eMode )
{
	m_eMode = eMode;
}

Engine::CTransform* CPlayer::GetTransCom( void )
{
	return m_pTransCom;
}

_bool CPlayer::GetChecked( void )
{
	return m_bCheck;
}

void CPlayer::CheckTrue( void )
{
	m_bCheck = true;
}

void CPlayer::CheckFalse( void )
{
	m_bCheck = false;
}

OBJ_INFO* CPlayer::GetObjectInfo( void )
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

	wstring wstrTag = L"Monster";

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

void CPlayer::SetObjectInfo( _vec3* vPos, _vec3* vScale, _vec3* vAngle )
{
	m_pTransCom->Set_Position(vPos);
	m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
	m_eMode = MODE_STOP;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CPlayer*		pInstance = new CPlayer(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CPlayer Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free(void)
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
	for (_uint i = 0; i < PLAYER_END; i++)
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

Engine::CNavigationMesh* CPlayer::GetNaviCom( void )
{
	return m_pNaviMeshCom;
}

