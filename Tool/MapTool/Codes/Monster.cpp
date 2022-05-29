#include "stdafx.h"

#include "Monster.h"

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
#include "Line.h"
#include "NavigationMesh.h"
#include "Babegazi_Warrior_Ani.h"

#include "AI.h"
#include "AI_Roaming.h"
#include "AI_Discovery.h"
#include "AI_Trace.h"

//맵툴 Monster

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
: Engine::CGameObject(pGraphicDev, wstrName)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_pRendererCom(NULL)
, m_pShaderCom(NULL)
, m_pNaviMeshCom(NULL)
, m_pAnimatorCom(NULL)
, m_pCalculatorCom(NULL)
, m_pRigidbodyCom(NULL)
, m_pResourceMgr(NULL)
, m_pPickingRect(NULL)
, m_pMouseCol(NULL)
, m_pPickingVtx(NULL)
, m_pLine(NULL)
, m_pNaviTraceLine(NULL)
, m_lMouseMove(0)
, m_iAniIdx(0)
, m_iMonsterAniState(0)
, m_fAggroRad(0.f)
, m_fPlayerDis(0.f)
, m_fRayDis(0.f)
, m_fAlpha(0.f)
, m_fRoamingTime(0.f)
, m_bStop(false)
, m_bComplete(false)
, m_bCheck(false)
, m_bCreateRect(false)
, m_bJump(false)
, m_bJumpMove(false)
, m_bCubeRayCol(false)
, m_bAstarMove(false)
, m_bRayMove(false)
, m_bAggro(false)
, m_bRoam(false)
, m_bMove(false)
{
	ZeroMemory(&m_tInfo, sizeof(OBJ_INFO));
	m_eRoamDir = DIR_END;
	m_eCompareRoamDir = DIR_END;
}
CMonster::~CMonster(void)
{

}

HRESULT CMonster::Start_ForScene(void)
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


	Engine::CAI_State* pAI_State = NULL;
	pAI_State = CAI_Roaming::Create(m_pTransCom, m_pAnimatorCom, m_pNaviMeshCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_ROAMING), pAI_State);

	pAI_State = CAI_Discovery::Create(m_pTransCom, m_pAnimatorCom, m_pNaviMeshCom, m_pCalculatorCom, m_pRigidbodyCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_DISCOVERY), pAI_State);


	pAI_State = CAI_Trace::Create(m_pTransCom, m_pAnimatorCom, m_pNaviMeshCom, m_pCalculatorCom, m_pRigidbodyCom);
	if (pAI_State == NULL)
		return E_FAIL;
	m_pAICom->Set_State(_uint(AI_STATE::AI_TRACE), pAI_State);

	m_pAICom->Set_State(AI_ROAMING);
	m_pAICom->Set_AI_Stop();

	list<Engine::CGameObject*>::iterator  iter = Engine::FindList(L"Player")->begin();

	m_pTargetTransCom = ((CPlayer*)(*iter))->GetTransCom();


	return S_OK;
}

void CMonster::Update_Object(const _float& fTimeDelta)
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

	if(m_eMode == MODE_DEBUG)
	{
		list<Engine::CGameObject*>::iterator  iter = Engine::FindList(L"Player")->begin();

		m_pTargetTransCom = ((CPlayer*)(*iter))->GetTransCom();
	
		m_vTargetPos = m_pTargetTransCom->Get_Now_Position();
		m_vPivotPos = *m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_vPivotPos);
		m_vRayDir = m_vTargetPos - m_vPivotPos;
		m_fPlayerDis = D3DXVec3Length(&m_vRayDir);
		D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
		

		m_pAICom->Set_AI_Update();

		if (m_pCalculatorCom->Get_IsCol())
		{
			m_pRigidbodyCom->Set_Gravity(false);
		}
		else
		{
			m_pRigidbodyCom->Set_Gravity(true);
		}

		//몬스터 무빙...(일단 가라로...)
		if (!m_bAggro)
		{
			if (m_fPlayerDis < m_fAggroRad)
			{
				m_pAICom->Set_State(AI_DISCOVERY);
			}

			if (m_pAnimatorCom->Is_AnimationEnd(DISCOVERY))
			{
				m_bAggro = true;
				m_pAnimatorCom->Set_State(MONSTER_FIGHT);
				m_pAICom->Set_State(AI_TRACE);
				m_pAnimatorCom->Set_AnimationSet(MOVE_RUN);
			}
		}

		//if (!m_bAggro)
		//{
		//	DebugWork(fTimeDelta);
		//}	
		//
		////DebugWork에서 점프 트루로 만들어주면..
		////점프는 한번만 들어와라...
		//if(m_bJump && !m_bJumpMove)
		//{
		//	//Set_Force로 점프를하고
		//	m_pRigidbodyCom->Set_Jump(0.7f);
		//	m_iAniIdx = JUMP_A;
		//	m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
		//	m_bJumpMove = true;
		//	m_pCalculatorCom->Set_IsCol(false);
		//}

		//if(m_bJumpMove)
		//{
		//	//점프시 무브로 넘어와서..
		//	//점프 A -> 점프 B
		//	if(m_pAnimatorCom->Is_AnimationEnd(JUMP_A))
		//	{
		//		m_pAnimatorCom->Set_AnimationSet(DISCOVERY);
		//		LookAtDir(&m_vRayDir, fTimeDelta);

		//	}

		//	if (m_pAnimatorCom->Is_AnimationEnd(DISCOVERY))
		//	{
		//		m_bAggro = true;
		//		m_iMonsterAniState = MONSTER_FIGHT;
		//		m_pAnimatorCom->Set_State(m_iMonsterAniState);
		//		//m_iAniIdx = MOVE_RUN;
		//		m_pAnimatorCom->Set_AnimationSet(MOVE_RUN);
		//		m_pAICom->Set_State(AI_TRACE);
		//	}
		//}

		//if(m_pAnimatorCom->Is_AnimationEnd(JUMP_C))
		//{
		//	m_iAniIdx = BATTLE_IDLE;
		//	m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
		//}



		//if(!m_bJump && !m_bJumpMove)eeee
		//{
		//	DebugWork(fTimeDelta);
		//}	
		//
		////DebugWork에서 점프 트루로 만들어주면..
		////점프는 한번만 들어와라...
		//if(m_bJump && !m_bJumpMove)
		//{
		//	//Set_Force로 점프를하고
		//	m_pRigidbodyCom->Set_Force(0.7f);
		//	m_iAniIdx = JUMP_A;
		//	m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
		//	m_bJumpMove = true;
		//	m_pCalculatorCom->Set_IsCol(false);
		//}

		//if(m_bJumpMove)
		//{
		//	//점프시 무브로 넘어와서..
		//	//점프 A -> 점프 B
		//	if(m_pAnimatorCom->Is_AnimationEnd(JUMP_A))
		//	{
		//		m_iAniIdx = JUMP_B;
		//		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);

		//	}
		//	//레이방향으로 이동...
		//	_vec3 vPos = *m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
		//	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
		//	_vec3 vEndPos = vPos + m_vRayDir * 0.3f;
		//	m_pTransCom->Set_Position(&vEndPos);

		//	//지면에충돌하면...
		//	//여기가문제인듯..지면 뛰기도 전에 들어오는경우가 생김

		//	if(m_pCalculatorCom->Get_IsCol())
		//	{
		//		//이거 추가해서 되는듯?
		//		if(m_pAnimatorCom->Get_AnimationIndex() == JUMP_B)
		//		{
		//			m_iAniIdx = JUMP_C;
		//			m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
		//			m_bJump = false;
		//			m_bJumpMove = false;
		//		}
		//	}
		//}

	}


	else
	{
		m_pAICom->Set_AI_Stop();
		SettingWork();
	}


	vPos.y += 10.f;
	m_pPickingRect->Set_Positon(&vPos);


	_vec3	vScale;

	m_pPickingRect->Update_Object(fTimeDelta);

	Compute_ViewZ(m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vScale));

}

void CMonster::Render_Object(void)
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

	if(m_eMode == MODE_DEBUG)
	{
		Render_Ray();
	}

}


HRESULT CMonster::Ready_Object(void)
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

	m_fAggroRad = 30.f;

	m_vPivotPos = m_vTargetPos = _vec3(0.f, 0.f, 0.f);
	m_pLine = Engine::CLine::Create(&_vec2(m_vPivotPos.x, m_vPivotPos.z)
		, &_vec2(m_vTargetPos.x, m_vTargetPos.z));

	if(FAILED(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine)))
		return E_FAIL;

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

HRESULT CMonster::Add_Component(void)
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

	if (NULL != Engine::FindList(L"Map_Fire"))
	{
		list<Engine::CGameObject*>::iterator iter_map = Engine::FindList(L"Map_Fire")->begin();
		m_pNaviMeshCom = ((CMap_Fire*)(*iter_map))->GetNaviCom();
	}

	if (NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter_map = Engine::FindList(L"Map_Ice")->begin();
		m_pNaviMeshCom = ((CMap_Ice*)(*iter_map))->GetNaviCom();
	}



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

	//AI
	pComponent = Engine::CAI::Create();
	if (pComponent == NULL)
		return E_FAIL;
	m_pAICom = (Engine::CAI*)pComponent;
	m_mapComponent[Engine::COMID_DYNAMIC].insert(MAPCOMPONENT::value_type(L"4AI", pComponent));

	return S_OK;
}

HRESULT CMonster::Set_ConstantTable( LPD3DXEFFECT pEffect )
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

void CMonster::SettingWork( void )
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
			RoamPosSet();
			m_eMode = MODE_STOP;
			m_pAICom->Set_StartState();
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
							if(((CMonster*)(*iter_Monster))->GetMode() == MODE_MODIFY)
							{
								((CMonster*)(*iter_Monster))->SetMode(MODE_STOP);
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
			RoamPosSet();
			m_eMode = MODE_STOP;
			m_pAICom->Set_StartState();
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

void CMonster::DebugWork( const _float& fTimeDelta )
{

	//if(NULL != Engine::FindList(L"Player"))
	//{
	//	list<Engine::CGameObject*>::iterator  iter = Engine::FindList(L"Player")->begin();
	//	list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Player")->end();
	//	for(iter; iter != iter_end;iter++)
	//	{
	//		((CPlayer*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &m_vTargetPos);
	//		((CPlayer*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &m_vPlayerPos);
	//	}
	//	
	//	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &m_vPivotPos);
	//	//몬스터에서 플레이어로 쏘는 레이 포인트 수정...
	//	//m_vTargetPos.y += 0.0f;
	//	m_vPivotPos.y += 2.f;
	//	m_vRayDir   = m_vTargetPos - m_vPivotPos;
	//	//플레이어와 나의 직선거리....
	//	m_fPlayerDis = D3DXVec3Length(&m_vRayDir);

	//	//플레이어와 나의 직선거리....

	//	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	//	PlayerRayCheck();
	//	CubeRayCheck();

	//}
	////몬스터 무빙...(일단 가라로...)
	//if(!m_bAggro)
	//{
	//	if(m_fPlayerDis < m_fAggroRad)
	//	{
	//		m_iAniIdx = DISCOVERY;
	//		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
	//		LookAtDir(&m_vRayDir, fTimeDelta);

	//	}
	//	else
	//	{
	//		NormalMove(fTimeDelta);
	//	}

	//	if(m_pAnimatorCom->Is_AnimationEnd(DISCOVERY))
	//	{
	//		m_bAggro = true;
	//		m_iMonsterAniState = MONSTER_FIGHT;
	//		m_pAnimatorCom->Set_State(m_iMonsterAniState);
	//		m_iAniIdx = MOVE_RUN;
	//		m_pAnimatorCom->Set_AnimationSet(m_iAniIdx);
	//	}
	//}


	//if(m_bAggro)
	//{
	//	//레이방향 이동...
	//	_vec3 vPos;
	//	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
	//	_vec3 vEndPos;
	//	//레이가 플레이어와의 직선거리보다 길면...(장애물X)
	//	if(m_fRayDis > m_fPlayerDis)		
	//	{
	//		if(!m_bCubeRayCol)
	//		{
	//			m_bRayMove = true;
	//			m_bAstarMove = false;
	//			vEndPos = vPos + m_vRayDir * 0.1f;
	//			m_pTransCom->Set_Position(&vEndPos);
	//			_ulong dwIdx = m_pNaviMeshCom->Get_CurrentCellIndex(&vPos);

	//			if(!m_bJump && m_pCalculatorCom->Get_IsCol())
	//			{
	//				//여기서 m_bJump를 트루 반환하게 되면..
	//				m_bJump = m_pNaviMeshCom->Jump_Check_NaviSide(&vEndPos, &m_vRayDir, dwIdx);
	//			}
	//		}
	//		else
	//		{
	//			//A스타 이동....
	//			m_bAstarMove = true;
	//			m_bRayMove = false;
	//			m_pNaviMeshCom->FindPath(&vPos, &m_vPlayerPos);
	//			m_pTransCom->Set_Position(&m_pNaviMeshCom->AStarMove(&vPos));
	//		}

	//	}


	//	//레이가 플레이어와의 직선거리보다 짧으면..(장애물O)
	//	else
	//	{
	//		//짧더라도 장애물이 "바닥"일 경우!
	//		if(m_vTargetPos.y < vPos.y + 1.f)
	//		{
	//			if(!m_bCubeRayCol)
	//			{
	//				m_bRayMove = true;
	//				m_bAstarMove = false;

	//				vEndPos = vPos + m_vRayDir * 0.1f;
	//				m_pTransCom->Set_Position(&vEndPos);
	//				_ulong dwIdx = m_pNaviMeshCom->Get_CurrentCellIndex(&vPos);
	//				//cout << "바닥 인덱스 : " << dwIdx << endl;
	//				//m_pTransCom->Set_Position(&m_pNaviMeshCom->Move_OnNaviMesh_Tool(&vPos, &m_vRayDir, 0.1f, dwIdx));

	//				if(!m_bJump && m_pCalculatorCom->Get_IsCol())
	//				{
	//					m_bJump = m_pNaviMeshCom->Jump_Check_NaviSide(&vEndPos, &m_vRayDir, dwIdx);
	//				}
	//			}
	//			else
	//			{
	//				m_bRayMove = false;
	//				m_bAstarMove = true;
	//				//A스타 이동....
	//				m_pNaviMeshCom->FindPath(&vPos, &m_vPlayerPos);
	//				m_pTransCom->Set_Position(&m_pNaviMeshCom->AStarMove(&vPos));
	//			}
	//		}
	//		//장애물이 바닥이 아니고 벽이라면...
	//		else
	//		{
	//			m_bRayMove = false;
	//			m_bAstarMove = true;
	//			//A스타 이동....
	//			m_pNaviMeshCom->FindPath(&vPos, &m_vPlayerPos);
	//			m_pTransCom->Set_Position(&m_pNaviMeshCom->AStarMove(&vPos));		
	//		}
	//	}
	//	if(m_bRayMove)
	//	{
	//		LookAtDir(&m_vRayDir, fTimeDelta);
	//		
	//	}
	//	else
	//	{
	//		LookAtDir(m_pNaviMeshCom->GetAStarDir(), fTimeDelta);
	//	}
}
	


void CMonster::RoamPosSet( void )
{
	//로밍 포인트를 세팅한다..
	for(int i = 0; i < DIR_END; ++i)
	{
		m_vRoamPos[i] = m_vStartPos;
	}
	m_vRoamPos[DIR_EAST].x = m_vStartPos.x - 5.f;
	m_vRoamPos[DIR_EAST].z = m_vStartPos.z - 5.f;

	m_vRoamPos[DIR_WEST].x = m_vStartPos.x + 5.f;
	m_vRoamPos[DIR_WEST].z = m_vStartPos.z - 5.f;

	m_vRoamPos[DIR_SOUTH].x = m_vStartPos.x - 5.f;
	m_vRoamPos[DIR_SOUTH].z = m_vStartPos.z + 5.f;

	m_vRoamPos[DIR_NORTH].x = m_vStartPos.x + 5.f;
	m_vRoamPos[DIR_NORTH].z = m_vStartPos.z + 5.f;

}

void CMonster::NormalMove( const _float& fTimeDelta )
{

	//if(!m_bRoam)
	//{
	//	m_fRoamingTime += fTimeDelta;

	//	if(m_fRoamingTime > 6.f)
	//	{
	//		srand(unsigned int(time(NULL)));
	//		m_eRoamDir = ROAM_DIR(rand() % 4);
	//		if(m_eRoamDir == m_eCompareRoamDir)
	//		{
	//			return;
	//		}
	//		m_bRoam = true;
	//		m_fRoamingTime = 0.f;
	//	}
	//}

	//if(m_bRoam)// m_bRoam == true
	//{
	//	_float fLength = 0.f;
	//	m_eCompareRoamDir = m_eRoamDir;

	//	_vec3 vDir;
	//	_vec3 vPos;
	//	m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
	//	vDir = m_vRoamPos[m_eRoamDir] - vPos;
	//	fLength = D3DXVec3Length(&vDir);

	//	D3DXVec3Normalize(&vDir, &vDir);

	//	LookAtDir(&vDir, fTimeDelta);
	//	_vec3 vEndPos = vPos + vDir * 0.1f;

	//	m_pTransCom->Set_Position(&vEndPos);
	//	m_pAnimatorCom->Set_AnimationSet(WALK);

	//	if(fLength < 1.f)
	//	{
	//		m_bRoam = false;
	//		m_pAnimatorCom->Set_AnimationSet(STAND_IDLE);
	//	}

	//}
}



void CMonster::Render_Ray( void )
{
	_vec3			vPoint[2];

	vPoint[0] = m_vPivotPos;
	vPoint[1] = m_vTargetPos;

	_matrix			matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 2; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);
		if(vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	m_pD3DXLine->SetWidth(3.f);

	m_pGraphicDev->EndScene();

	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();

	_matrix				matTmp;

	m_pD3DXLine->DrawTransform(vPoint, 2, 
		D3DXMatrixIdentity(&matTmp), D3DXCOLOR(1.f, 1.f, 0.f, 1.f));

	m_pD3DXLine->End();
}

void CMonster::PlayerRayCheck( void )
{
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		_float rDist = 99999.f;
		list<Engine::CGameObject*>::iterator iter_map = Engine::FindList(L"Map_Ice")->begin();
	//	m_pNaviMeshCom = ((CMap_Ice*)(*iter_map))->GetNaviCom();
		//네비컴포넌트로 가져오고 인덱스에 꼽기...
		_vec3 vPos;
		m_pTransCom->Get_Infomation(Engine::INFO_POSITION, &vPos);
		//m_pNaviMeshCom->Get_CurrentCellIndex(&vPos);

		for(int i = 0; i < 5; ++i)
		{
			_float	fU, fV, fDist;
			BOOL BHit = false;
			D3DXIntersect(*((CMap_Ice*)(*iter_map))->GetMeshCom()[i]->Get_Mesh(),
				&m_vPivotPos, &m_vRayDir,
				&BHit, NULL, &fU, &fV, &fDist, NULL, NULL);

			if(BHit)
			{
				if(rDist > fDist)
				{
					rDist = fDist;
					D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
					m_vTargetPos = m_vPivotPos + m_vRayDir * rDist;

					//레이의 길이...
					_vec3 vDis = m_vTargetPos - m_vPivotPos;
					m_fRayDis = D3DXVec3Length(&vDis);
				}
			}
		}
	}
}


void CMonster::CubeRayCheck( void )
{
	if(NULL != Engine::FindList(L"Cube"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Cube")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Cube")->end();

		//			_float	fU, fV, fDist;
		_float fDist = 99999.f;
		_float rDist = 99999.f;
		BOOL BHit = false;
		_vec3 vDis;
		for(iter; iter != iter_end; ++iter)
		{
			((CCube*)(*iter))->CheckRayCol(&fDist, &m_vPivotPos, &m_vRayDir, rDist);		
		}
		//플레이어와의 거리 비교..
		if(fDist < m_fPlayerDis)
		{
			m_bCubeRayCol = true;
		}
		else
		{
			m_bCubeRayCol = false;
		}
	}
}

void CMonster::LookAtDir( _vec3* pDir, const _float& fTimeDelta )
{
	_vec3 vLook;
	m_pTransCom->Get_Infomation(Engine::INFO_LOOK, &vLook);

	//float fAngle = acosf(D3DXVec3Dot(&m_vRayDir, &vLook));

	_vec3 vUp = _vec3(0.f, 1.f, 0.f);
	_vec3 vRight;

	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	float fCos = D3DXVec3Dot(&vRight, pDir);
	float fRealCos = D3DXVec3Dot(&vLook, pDir);
	float fAngle = acosf(fRealCos);
	float fRotationSpeed = 180.f;

	fAngle = D3DXToDegree(fAngle);

	//if(fAngle > 180.f)
	//	fAngle = 360.f - fAngle;
	if (fCos < 0.f)
	{
		fRotationSpeed = -fRotationSpeed;
		fAngle = -fAngle;
	}

	if (fabs(fAngle) > fRotationSpeed * fTimeDelta)
	{
		m_pTransCom->Rotation(Engine::ROT_Y, fRotationSpeed * fTimeDelta);
	}
	else
		m_pTransCom->Rotation(Engine::ROT_Y, fAngle);

	//m_pTransCom->Rotation(Engine::ROT_Y, fRotationSpeed * fTimeDelta);

	//if (fCos > 0.1f)
	//{
	//	
	//	m_pTransCom->Rotation(Engine::ROT_Y, fRotationSpeed * fTimeDelta);
	//}
	//else if(fCos < -0.1f)
	//	m_pTransCom->Rotation(Engine::ROT_Y, fRotationSpeed * fTimeDelta);

}




void CMonster::Stop( void )
{
	m_bStop = true;
}

void CMonster::Complete( void )
{
	m_bComplete = true;
}

OBJECT_MODE CMonster::GetMode( void )
{
	return m_eMode;
}

void CMonster::SetMode( OBJECT_MODE eMode )
{
	m_eMode = eMode;
}

Engine::CTransform* CMonster::GetTransCom( void )
{
	return m_pTransCom;
}

_bool CMonster::GetChecked( void )
{
	return m_bCheck;
}

void CMonster::CheckTrue( void )
{
	m_bCheck = true;
}

void CMonster::CheckFalse( void )
{
	m_bCheck = false;
}

OBJ_INFO* CMonster::GetObjectInfo( void )
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

void CMonster::SetObjectInfo( _vec3* vPos, _vec3* vScale, _vec3* vAngle )
{
	m_pTransCom->Set_Position(vPos);
	m_pTransCom->Set_Scale(vScale->x, vScale->y, vScale->z);
	m_pTransCom->Rotation(Engine::ROT_Y, vAngle->y);
	m_eMode = MODE_STOP;
}


CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& wstrName)
{
	CMonster*		pInstance = new CMonster(pGraphicDev, wstrName);

	if(FAILED(pInstance->Ready_Object()))
	{
		MSG_BOX("CGrid Created Failed");
		::Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster::Free(void)
{
	
	Safe_Delete(m_pMouseCol);
	Engine::Safe_Release(m_pPickingRect);
	Engine::Safe_Delete(m_pPickingVtx);
	Engine::Safe_Release(m_pLine);	
	Engine::Safe_Release(m_pD3DXLine);

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

HRESULT CMonster::LoadAniInfo( const _tchar* pFileName )
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









