// MapToolTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MapToolTab.h"
#include "Scene_MapTool.h"
#include "Export_Engine.h"
#include "Camera_MapTool.h"
#include "Map_Store.h"
#include "Map_Ice.h"
#include "Map_Fire.h"
#include "Environment.h"
#include "NPC.h"
#include "Monster.h"
#include "Player.h"
#include "Sprite.h"
#include "Particle.h"
#include "Cube.h"
#include "Trap.h"

// CMapToolTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapToolTab, CDialog)

CMapToolTab::CMapToolTab(CWnd* pParent /*=NULL*/)
	: CDialog(CMapToolTab::IDD, pParent)
{

}

CMapToolTab::~CMapToolTab()
{
}

void CMapToolTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_OBJECT, m_ctrlTreeObject);
	DDX_Control(pDX, IDC_SLIDER_OBJ_X, m_ctrObjXSlider);
	DDX_Control(pDX, IDC_SLIDER_OBJ_Y, m_ctrObjYSlider);
	DDX_Control(pDX, IDC_SLIDER_OBJ_Z, m_ctrObjZSlider);
	DDX_Control(pDX, IDC_SLIDER_CAM_SPEED, m_ctrCamSpeedSlider);
}


BEGIN_MESSAGE_MAP(CMapToolTab, CDialog)
	ON_BN_CLICKED(IDC_MAP_ICE_BTN, &CMapToolTab::OnBnClickedMapIceBtn)
	ON_BN_CLICKED(IDC_MAP_SHOP_BTN, &CMapToolTab::OnBnClickedMapShopBtn)
	ON_BN_CLICKED(IDC_CREATE_BTn, &CMapToolTab::OnBnClickedCreateBtn)
	ON_BN_CLICKED(IDC_DEL_BTN, &CMapToolTab::OnBnClickedDelBtn)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_OBJ_X, &CMapToolTab::OnNMReleasedcaptureSliderObjX)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_EYETOMID, &CMapToolTab::OnBnClickedButtonEyetomid)
	ON_BN_CLICKED(IDC_DELALL_BTN, &CMapToolTab::OnBnClickedDelallBtn)
	ON_BN_CLICKED(IDC_SAVEOBJ_BTN, &CMapToolTab::OnBnClickedSaveobjBtn)
	ON_BN_CLICKED(IDC_LOADOBJ_BTN, &CMapToolTab::OnBnClickedLoadobjBtn)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_OBJECT, &CMapToolTab::OnTvnItemexpandedTreeObject)
	ON_BN_CLICKED(IDC_MAP_FIRE_BTN, &CMapToolTab::OnBnClickedMapFireBtn)
END_MESSAGE_MAP()


// CMapToolTab 메시지 처리기입니다.

BOOL CMapToolTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	HTREEITEM hRoot;

	hRoot = m_ctrlTreeObject.InsertItem(L"Object", 0/* nImage */, 1/* nSelectedImage */, TVI_ROOT, TVI_LAST );


	//구조물...
	HTREEITEM  hStructure;

	hStructure = m_ctrlTreeObject.InsertItem(L"Structure", 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST ) ;

	HTREEITEM  hStore;
	hStore = m_ctrlTreeObject.InsertItem(L"Store", 1/* nImage */, 1/* nSelectedImage */, hStructure, TVI_LAST );

	HTREEITEM  hTorch;

	hTorch = m_ctrlTreeObject.InsertItem(L"Mesh_Torch1", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hTorch2;

	hTorch2 = m_ctrlTreeObject.InsertItem(L"Mesh_Torch2", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST);

	HTREEITEM hStoreBoard;
	hStoreBoard = m_ctrlTreeObject.InsertItem(L"Mesh_StoreBoard", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM hStoreCandle;
	hStoreCandle = m_ctrlTreeObject.InsertItem(L"Mesh_StoreCandle", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hBookShelf;

	hBookShelf = m_ctrlTreeObject.InsertItem(L"Mesh_BookShelf", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hPaperBook;

	hPaperBook = m_ctrlTreeObject.InsertItem(L"Mesh_PaperBook", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStoreBook;

	hStoreBook = m_ctrlTreeObject.InsertItem(L"Mesh_StoreBook", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;


	HTREEITEM  hStorePotion;

	hStorePotion = m_ctrlTreeObject.InsertItem(L"Mesh_StorePotion", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	
	HTREEITEM  hStoreArmor;

	hStoreArmor = m_ctrlTreeObject.InsertItem(L"Mesh_StoreArmor", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStoreSword;

	hStoreSword = m_ctrlTreeObject.InsertItem(L"Mesh_StoreSword", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStoreShield;

	hStoreShield = m_ctrlTreeObject.InsertItem(L"Mesh_StoreShield", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStoreBoots;

	hStoreBoots = m_ctrlTreeObject.InsertItem(L"Mesh_StoreBoots", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;


	HTREEITEM  hStoreDagger;

	hStoreDagger = m_ctrlTreeObject.InsertItem(L"Mesh_StoreDagger", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStoreBow;

	hStoreBow = m_ctrlTreeObject.InsertItem(L"Mesh_StoreBow", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStoreSpear;

	hStoreSpear = m_ctrlTreeObject.InsertItem(L"Mesh_StoreSpear", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStoreTable;

	hStoreTable = m_ctrlTreeObject.InsertItem(L"Mesh_StoreTable", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStoreTable2;

	hStoreTable2 = m_ctrlTreeObject.InsertItem(L"Mesh_StoreTable2", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST ) ;

	HTREEITEM  hStage_Ice;
	hStage_Ice = m_ctrlTreeObject.InsertItem(L"Stage_Ice", 1/* nImage */, 1/* nSelectedImage */, hStructure, TVI_LAST);

	HTREEITEM hCristal_1;
	hCristal_1 = m_ctrlTreeObject.InsertItem(L"Mesh_Cristal_1", 1/* nImage */, 1/* nSelectedImage */, hStage_Ice, TVI_LAST);

	HTREEITEM hCristal_2;
	hCristal_2 = m_ctrlTreeObject.InsertItem(L"Mesh_Cristal_2", 1/* nImage */, 1/* nSelectedImage */, hStage_Ice, TVI_LAST);

	HTREEITEM hIce_Rock_1;
	hIce_Rock_1 = m_ctrlTreeObject.InsertItem(L"Mesh_Ice_Rock_1", 1/* nImage */, 1/* nSelectedImage */, hStage_Ice, TVI_LAST);

	HTREEITEM hLamp_Bone_A;
	hLamp_Bone_A = m_ctrlTreeObject.InsertItem(L"Mesh_Lamp_Bone_A", 1/* nImage */, 1/* nSelectedImage */, hStage_Ice, TVI_LAST);

	HTREEITEM hLamp_Bone_B;
	hLamp_Bone_B = m_ctrlTreeObject.InsertItem(L"Mesh_Lamp_Bone_B", 1/* nImage */, 1/* nSelectedImage */, hStage_Ice, TVI_LAST);

	HTREEITEM hLamp_Bone_C;
	hLamp_Bone_C = m_ctrlTreeObject.InsertItem(L"Mesh_Lamp_Bone_C", 1/* nImage */, 1/* nSelectedImage */, hStage_Ice, TVI_LAST);

	HTREEITEM hGate_A;
	hGate_A = m_ctrlTreeObject.InsertItem(L"Mesh_Gate_A", 1/* nImage */, 1/* nSelectedImage */, hStage_Ice, TVI_LAST);
	//Bardiel Sealed

	HTREEITEM  hBardielSealed;

	hBardielSealed = m_ctrlTreeObject.InsertItem(L"Mesh_Bardiel_Sealed", 1/* nImage */, 1/* nSelectedImage */, hStore, TVI_LAST);


	//NPC

	HTREEITEM	hNPC;
	hNPC = m_ctrlTreeObject.InsertItem(L"NPC", 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST ) ;

	HTREEITEM  hNPCWeapon;

	hNPCWeapon = m_ctrlTreeObject.InsertItem(L"Mesh_NPC_Weapon", 1/* nImage */, 1/* nSelectedImage */, hNPC, TVI_LAST ) ;

	HTREEITEM  hNPCArmor;

	hNPCArmor = m_ctrlTreeObject.InsertItem(L"Mesh_NPC_Armor", 1/* nImage */, 1/* nSelectedImage */, hNPC, TVI_LAST ) ;

	HTREEITEM  hNPCPotion;

	hNPCPotion = m_ctrlTreeObject.InsertItem(L"Mesh_NPC_Potion", 1/* nImage */, 1/* nSelectedImage */, hNPC, TVI_LAST ) ;

	HTREEITEM  hNPCBook;

	hNPCBook = m_ctrlTreeObject.InsertItem(L"Mesh_NPC_Book", 1/* nImage */, 1/* nSelectedImage */, hNPC, TVI_LAST ) ;


	//Monster

	HTREEITEM	hMonster;
	hMonster = m_ctrlTreeObject.InsertItem(L"Monster", 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST ) ;

	HTREEITEM	hBabegaze_Warrior;
	hBabegaze_Warrior = m_ctrlTreeObject.InsertItem(L"Mesh_Babegazi_Warrior", 1/* nImage */, 1/* nSelectedImage */, hMonster, TVI_LAST ) ;

	HTREEITEM	hBabegaze_Axe;
	hBabegaze_Axe = m_ctrlTreeObject.InsertItem(L"Mesh_Babegazi_Axe", 1/* nImage */, 1/* nSelectedImage */, hMonster, TVI_LAST ) ;

	HTREEITEM	hBabegaze_Bow;
	hBabegaze_Bow = m_ctrlTreeObject.InsertItem(L"Mesh_Babegazi_Bow", 1/* nImage */, 1/* nSelectedImage */, hMonster, TVI_LAST ) ;

	HTREEITEM	hKnole_Commander;
	hKnole_Commander = m_ctrlTreeObject.InsertItem(L"Mesh_Knole_Commander", 1/* nImage */, 1/* nSelectedImage */, hMonster, TVI_LAST ) ;

	HTREEITEM	hKnole_Warrior;
	hKnole_Warrior = m_ctrlTreeObject.InsertItem(L"Mesh_Knole_Warrior", 1/* nImage */, 1/* nSelectedImage */, hMonster, TVI_LAST ) ;

	HTREEITEM	hHanuman;
	hHanuman = m_ctrlTreeObject.InsertItem(L"Mesh_Hanuman", 1/* nImage */, 1/* nSelectedImage */, hMonster, TVI_LAST);

	HTREEITEM	hBardiel;
	hBardiel = m_ctrlTreeObject.InsertItem(L"Mesh_Bardiel", 1/* nImage */, 1/* nSelectedImage */, hMonster, TVI_LAST);

	//Trap
	HTREEITEM	hTrap;
	hTrap = m_ctrlTreeObject.InsertItem(L"Trap", 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST);

	HTREEITEM	hSnowBall;
	hSnowBall = m_ctrlTreeObject.InsertItem(L"Mesh_SnowBall", 1/* nImage */, 1/* nSelectedImage */, hTrap, TVI_LAST);

	HTREEITEM	hIceTwister;
	hIceTwister = m_ctrlTreeObject.InsertItem(L"Mesh_IceTwister", 1/* nImage */, 1/* nSelectedImage */, hTrap, TVI_LAST);
	

	//Player
	HTREEITEM hPlayer;
	hPlayer = m_ctrlTreeObject.InsertItem(L"Mesh_Player", 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST ) ;


	//Cube
	HTREEITEM hCube;
	hCube = m_ctrlTreeObject.InsertItem(L"Mesh_Cube", 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST ) ;


	//Effect
	HTREEITEM	hEffect;
	hEffect = m_ctrlTreeObject.InsertItem(L"Effect", 1/* nImage */, 1/* nSelectedImage */, hRoot, TVI_LAST );

	HTREEITEM	hRedFire;
	hRedFire = m_ctrlTreeObject.InsertItem(L"RedFire", 1/* nImage */, 1/* nSelectedImage */, hEffect, TVI_LAST ) ;

	HTREEITEM	hBlueFire;
	hBlueFire = m_ctrlTreeObject.InsertItem(L"BlueFire", 1/* nImage */, 1/* nSelectedImage */, hEffect, TVI_LAST ) ;




	//hNPCQuest = m_ctrlTreeObject.InsertItem(L"NPCQuest", 1/* nImage */, 1/* nSelectedImage */, hNPC, TVI_LAST ) ;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_ctrObjXSlider.SetRange(-100000, 100000);
	m_ctrObjXSlider.SetRangeMin(-100000);
	m_ctrObjXSlider.SetRangeMax(100000);

	m_ctrObjXSlider.SetPos(0);

	m_ctrObjXSlider.SetTicFreq(1);
	m_ctrObjXSlider.SetLineSize(1);

	m_ctrObjYSlider.SetRange(-100000, 100000);
	m_ctrObjYSlider.SetRangeMin(-100000);
	m_ctrObjYSlider.SetRangeMax(100000);

	m_ctrObjYSlider.SetPos(0);

	m_ctrObjYSlider.SetTicFreq(1);
	m_ctrObjYSlider.SetLineSize(1);

	m_ctrObjZSlider.SetRange(-100000, 100000);
	m_ctrObjZSlider.SetRangeMin(-100000);
	m_ctrObjZSlider.SetRangeMax(100000);

	m_ctrObjZSlider.SetPos(0);

	m_ctrObjZSlider.SetTicFreq(1);
	m_ctrObjZSlider.SetLineSize(1);

	m_ctrCamSpeedSlider.SetRange(0, 100);
	m_ctrCamSpeedSlider.SetRangeMin(0);
	m_ctrCamSpeedSlider.SetRangeMax(100);

	m_ctrCamSpeedSlider.SetPos(50);

	m_ctrCamSpeedSlider.SetTicFreq(1);
	m_ctrCamSpeedSlider.SetLineSize(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapToolTab::OnBnClickedMapIceBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	LPDIRECT3DDEVICE9 pGraphicDev = NULL;
	if(FAILED(Engine::Get_GraphicDev(&pGraphicDev)))
		return;


	Engine::CGameObject* pGameObject = NULL;
	//Engine::CLayer* pLayer = NULL;
	pGameObject = Engine::Find(L"Map");

	if(pGameObject == NULL)
	{
		pGameObject = CMap_Ice::Create(pGraphicDev, L"Map");

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Map_Ice", pGameObject);


	}
	else
	{
		pGameObject->SetState(Engine::STATE_DESTROY);
		Engine::Safe_Release(pGameObject);

	}


	Engine::Safe_Release(pGraphicDev);

}

void CMapToolTab::OnBnClickedMapShopBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPDIRECT3DDEVICE9 pGraphicDev = NULL;
	if(FAILED(Engine::Get_GraphicDev(&pGraphicDev)))
		return;


	Engine::CGameObject* pGameObject = NULL;
	//Engine::CLayer* pLayer = NULL;
	pGameObject = Engine::Find(L"Map");

	if(pGameObject == NULL)
	{
		pGameObject = CMap_Store::Create(pGraphicDev, L"Map");

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Map_Store", pGameObject);
	}
	else
	{
		pGameObject->SetState(Engine::STATE_DESTROY);
		Engine::Safe_Release(pGameObject);
	}


	Engine::Safe_Release(pGraphicDev);
}


void CMapToolTab::OnBnClickedMapFireBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPDIRECT3DDEVICE9 pGraphicDev = NULL;
	if (FAILED(Engine::Get_GraphicDev(&pGraphicDev)))
		return;


	Engine::CGameObject* pGameObject = NULL;
	//Engine::CLayer* pLayer = NULL;
	pGameObject = Engine::Find(L"Map");

	if (pGameObject == NULL)
	{
		pGameObject = CMap_Fire::Create(pGraphicDev, L"Map");

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Map_Fire", pGameObject);


	}
	else
	{
		pGameObject->SetState(Engine::STATE_DESTROY);
		Engine::Safe_Release(pGameObject);

	}


	Engine::Safe_Release(pGraphicDev);
}


//Create
void CMapToolTab::OnBnClickedCreateBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	LPDIRECT3DDEVICE9 pGraphicDev = NULL;
	Engine::CGameObject* pGameObject = NULL;

	if(FAILED(Engine::Get_GraphicDev(&pGraphicDev)))
		return;

	HTREEITEM hTreeItem = m_ctrlTreeObject.GetSelectedItem();
	CString strObj = m_ctrlTreeObject.GetItemText(hTreeItem);


	hTreeItem = m_ctrlTreeObject.GetNextItem(hTreeItem, TVGN_PARENT); 

	CString strMeshTypeName = m_ctrlTreeObject.GetItemText(hTreeItem);
	wstring wstrMeshTypeName = (LPCTSTR)strMeshTypeName;


	if(strMeshTypeName == L"Store")
	{
	//	m_wLayerID = LAYER_ENVIRONMENT;
		wstring wstr;

		wstr = strObj.operator LPCWSTR();

		pGameObject = CEnvironment::Create(pGraphicDev, wstr);

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Environment", pGameObject);
	}

	if (strMeshTypeName == L"Stage_Ice")
	{
		//	m_wLayerID = LAYER_ENVIRONMENT;
		wstring wstr;

		wstr = strObj.operator LPCWSTR();

		pGameObject = CEnvironment::Create(pGraphicDev, wstr);

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Environment", pGameObject);
	}

	if(strMeshTypeName == L"NPC")
	{
		wstring wstr;

		wstr = strObj.operator LPCWSTR();

		pGameObject = CNPC::Create(pGraphicDev, wstr);

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"NPC", pGameObject);
	}

	if(strMeshTypeName == L"Monster")
	{
		wstring wstr;

		wstr = strObj.operator LPCWSTR();

		pGameObject = CMonster::Create(pGraphicDev, wstr);

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Monster", pGameObject);
	}

	if (strMeshTypeName == L"Trap")
	{
		wstring wstr;

		wstr = strObj.operator LPCWSTR();

		pGameObject = CTrap::Create(pGraphicDev, wstr);

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Trap", pGameObject);
	}

	if(strObj == L"Mesh_Player")
	{
		wstring wstr;

		wstr = strObj.operator LPCWSTR();

		pGameObject = CPlayer::Create(pGraphicDev, wstr);

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Player", pGameObject);
	}

	if(strObj == L"Mesh_Cube")
	{
		wstring wstr;

		wstr = strObj.operator LPCWSTR();

		pGameObject = CCube::Create(pGraphicDev, wstr);

		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Cube", pGameObject);
	}

	if(strMeshTypeName == L"Effect")
	{
		wstring wstr;

		wstr = strObj.operator LPCWSTR();

		if(wstr == L"RedFire" || wstr == L"BlueFire")
		{
			pGameObject = CSprite::LoadCreate(pGraphicDev, wstr);
		}


		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Effect", pGameObject);

	}

	Engine::Safe_Release(pGraphicDev);

}

void CMapToolTab::OnBnClickedDelBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Engine::CGameObject* pGameObject = NULL;
	//Engine::CLayer* pLayer = NULL;
	if(NULL != Engine::FindList(L"Environment"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Environment")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Environment")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CEnvironment*)(*iter))->GetMode() == MODE_MODIFY)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
				break;
			}
		}
	}

	if(NULL != Engine::FindList(L"NPC"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"NPC")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"NPC")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CNPC*)(*iter))->GetMode() == MODE_MODIFY)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
				break;
			}
		}
	}

	if(NULL != Engine::FindList(L"Monster"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Monster")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Monster")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CMonster*)(*iter))->GetMode() == MODE_MODIFY)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
				break;
			}
		}
	}

	if (NULL != Engine::FindList(L"Trap"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Trap")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Trap")->end();
		for (iter; iter != iter_end; iter++)
		{
			if (((CTrap*)(*iter))->GetMode() == MODE_MODIFY)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
				break;
			}
		}
	}

	if(NULL != Engine::FindList(L"Player"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Player")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Player")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CPlayer*)(*iter))->GetMode() == MODE_MODIFY)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
				break;
			}
		}
	}

	if(NULL != Engine::FindList(L"Cube"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Cube")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Cube")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CCube*)(*iter))->GetMode() == MODE_MODIFY)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
				break;
			}
		}
	}

	if(NULL != Engine::FindList(L"Effect"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Effect")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Effect")->end();
		for(iter; iter != iter_end;iter++)
		{
			if(((CSprite*)(*iter))->GetMode() == MODE_MODIFY)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
				break;
			}
		}
	}


}

void CMapToolTab::OnBnClickedDelallBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Engine::CGameObject* pGameObject = NULL;
	//Engine::CLayer* pLayer = NULL;
	if(NULL != Engine::FindList(L"Environment"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Environment")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Environment")->end();
		for(iter; iter != iter_end;iter++)
		{		
			(*iter)->SetState(Engine::STATE_DESTROY);					
		}
	}

	if(NULL != Engine::FindList(L"NPC"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"NPC")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"NPC")->end();
		for(iter; iter != iter_end;iter++)
		{		
			(*iter)->SetState(Engine::STATE_DESTROY);					
		}
	}

	if(NULL != Engine::FindList(L"Monster"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Monster")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Monster")->end();
		for(iter; iter != iter_end;iter++)
		{		
			(*iter)->SetState(Engine::STATE_DESTROY);					
		}
	}

	if (NULL != Engine::FindList(L"Trap"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Trap")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Trap")->end();
		for (iter; iter != iter_end; iter++)
		{
			(*iter)->SetState(Engine::STATE_DESTROY);
		}
	}

	if(NULL != Engine::FindList(L"Player"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Player")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Player")->end();
		for(iter; iter != iter_end;iter++)
		{		
			(*iter)->SetState(Engine::STATE_DESTROY);					
		}
	}


	if(NULL != Engine::FindList(L"Cube"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Cube")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Cube")->end();
		for(iter; iter != iter_end;iter++)
		{		
			(*iter)->SetState(Engine::STATE_DESTROY);					
		}
	}


	if(NULL != Engine::FindList(L"Effect"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Effect")->begin();
		list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Effect")->end();
		for(iter; iter != iter_end;iter++)
		{		
			(*iter)->SetState(Engine::STATE_DESTROY);					
		}
	}
}

void CMapToolTab::OnNMReleasedcaptureSliderObjX(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CMapToolTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(pScrollBar)
	{

		if(pScrollBar == (CScrollBar*)&m_ctrCamSpeedSlider)
		{
			int nPos = m_ctrCamSpeedSlider.GetPos();
			SetDlgItemInt(IDC_EDIT_CAMSPEED, nPos);
			Engine::CGameObject* pGameObject = NULL;
			pGameObject = Engine::Find(L"MainCamera");
			((CCamera_MapTool*)pGameObject)->SetCamSpeed(nPos);

			Engine::Safe_Release(pGameObject);

		}
	}
	//선택 오브젝트 스크롤바대로 위치 미세 수정..
	if(pScrollBar)
	{
		if(pScrollBar == (CScrollBar*)&m_ctrObjXSlider)
		{
			float fX = (float)m_ctrObjXSlider.GetPos() * 0.01f;
			CString str; 

			str.Format(L"%f", fX); 
			GetDlgItem(IDC_EDIT_OBJ_X)->SetWindowText(str); 
			
			if( Engine::FindList(L"Environment") != NULL )
			{
				list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Environment")->begin();
				list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Environment")->end();
				for(iter; iter != iter_end;iter++)
				{
					if(((CEnvironment*)(*iter))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CEnvironment*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CEnvironment*)(*iter))->GetTransCom()->Set_Position(fX,
							vPos.y, vPos.z);
					}
				}
			}

			if(  Engine::FindList(L"NPC") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_NPC = Engine::FindList(L"NPC")->begin();
				list<Engine::CGameObject*>::iterator iter_NPC_end = Engine::FindList(L"NPC")->end();
				for(iter_NPC; iter_NPC != iter_NPC_end;iter_NPC++)
				{
					if(((CNPC*)(*iter_NPC))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CNPC*)(*iter_NPC))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CNPC*)(*iter_NPC))->GetTransCom()->Set_Position(fX,
							vPos.y, vPos.z);
					}
				}
			}

			if(  Engine::FindList(L"Monster") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Monster")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Monster")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CMonster*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CMonster*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CMonster*)(*iter_Monster))->GetTransCom()->Set_Position(fX,
							vPos.y, vPos.z);
					}
				}
			}

			if (Engine::FindList(L"Trap") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Trap")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Trap")->end();
				for (iter_Monster; iter_Monster != iter_Monster_end; iter_Monster++)
				{
					if (((CTrap*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CTrap*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CTrap*)(*iter_Monster))->GetTransCom()->Set_Position(fX,
							vPos.y, vPos.z);
					}
				}
			}




			if(  Engine::FindList(L"Effect") != NULL)
			{

				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Effect")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Effect")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CSprite*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CSprite*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CSprite*)(*iter_Monster))->GetTransCom()->Set_Position(fX,
							vPos.y, vPos.z);
					}
				}
			}

			if(  Engine::FindList(L"Player") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Player")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Player")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CPlayer*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CPlayer*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CPlayer*)(*iter_Monster))->GetTransCom()->Set_Position(fX,
							vPos.y, vPos.z);
					}
				}
			}

			if(  Engine::FindList(L"Cube") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Cube")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Cube")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CCube*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CCube*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CCube*)(*iter_Monster))->GetTransCom()->Set_Position(fX,
							vPos.y, vPos.z);
					}
				}
			}

		}

		if(pScrollBar == (CScrollBar*)&m_ctrObjYSlider)
		{
			float fY = (float)m_ctrObjYSlider.GetPos() * 0.01f;
			CString str; 

			str.Format(L"%f", fY); 
			GetDlgItem(IDC_EDIT_OBJ_Y)->SetWindowText(str); 

			if( Engine::FindList(L"Environment") != NULL )
			{
				list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Environment")->begin();
				list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Environment")->end();
				for(iter; iter != iter_end;iter++)
				{
					if(((CEnvironment*)(*iter))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CEnvironment*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);
						((CEnvironment*)(*iter))->GetTransCom()->Set_Position(vPos.x,
							fY, vPos.z);
					}
				}
			}

			if(  Engine::FindList(L"NPC") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_NPC = Engine::FindList(L"NPC")->begin();
				list<Engine::CGameObject*>::iterator iter_NPC_end = Engine::FindList(L"NPC")->end();
				for(iter_NPC; iter_NPC != iter_NPC_end;iter_NPC++)
				{
					if(((CNPC*)(*iter_NPC))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CNPC*)(*iter_NPC))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CNPC*)(*iter_NPC))->GetTransCom()->Set_Position(vPos.x,
							fY, vPos.z);
					}
				}
			}

			if(  Engine::FindList(L"Monster") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Monster")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Monster")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CMonster*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CMonster*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CMonster*)(*iter_Monster))->GetTransCom()->Set_Position(vPos.x,
							fY, vPos.z);
					}
				}
			}

			if (Engine::FindList(L"Trap") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Trap")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Trap")->end();
				for (iter_Monster; iter_Monster != iter_Monster_end; iter_Monster++)
				{
					if (((CTrap*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CTrap*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CTrap*)(*iter_Monster))->GetTransCom()->Set_Position(vPos.x,
							fY, vPos.z);
					}
				}
			}

			
			if(  Engine::FindList(L"Effect") != NULL)
			{

				list<Engine::CGameObject*>::iterator iter_Effect = Engine::FindList(L"Effect")->begin();
				list<Engine::CGameObject*>::iterator iter_Effect_end = Engine::FindList(L"Effect")->end();
				for(iter_Effect; iter_Effect != iter_Effect_end; iter_Effect++)
				{
					if(((CSprite*)(*iter_Effect))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CSprite*)(*iter_Effect))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CSprite*)(*iter_Effect))->GetTransCom()->Set_Position(vPos.x,
							fY, vPos.z);
					}
				}
			}



			if(  Engine::FindList(L"Player") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Player")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Player")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CPlayer*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CPlayer*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CPlayer*)(*iter_Monster))->GetTransCom()->Set_Position(vPos.x,
							fY, vPos.z);
					}
				}
			}

			if(  Engine::FindList(L"Cube") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Cube")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Cube")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CCube*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CCube*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CCube*)(*iter_Monster))->GetTransCom()->Set_Position(vPos.x,
							fY, vPos.z);
					}
				}
			}
		}

		if(pScrollBar == (CScrollBar*)&m_ctrObjZSlider)
		{
			float fZ = (float)m_ctrObjZSlider.GetPos() * 0.01f;
			CString str; 

			str.Format(L"%f", fZ); 
			GetDlgItem(IDC_EDIT_OBJ_Z)->SetWindowText(str); 
			if( Engine::FindList(L"Environment") != NULL )
			{
				list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Environment")->begin();
				list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Environment")->end();
				for(iter; iter != iter_end;iter++)
				{
					if(((CEnvironment*)(*iter))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CEnvironment*)(*iter))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);
						((CEnvironment*)(*iter))->GetTransCom()->Set_Position(vPos.x,
							vPos.y, fZ);
					}
				}
			}

			if( Engine::FindList(L"NPC") != NULL )
			{
				list<Engine::CGameObject*>::iterator iter_NPC = Engine::FindList(L"NPC")->begin();
				list<Engine::CGameObject*>::iterator iter_NPC_end = Engine::FindList(L"NPC")->end();
				for(iter_NPC; iter_NPC != iter_NPC_end;iter_NPC++)
				{
					if(((CNPC*)(*iter_NPC))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CNPC*)(*iter_NPC))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CNPC*)(*iter_NPC))->GetTransCom()->Set_Position(vPos.x,
							vPos.y, fZ);
					}
				}
			}
			
			if( Engine::FindList(L"Monster") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Monster")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Monster")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CMonster*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CMonster*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CMonster*)(*iter_Monster))->GetTransCom()->Set_Position(vPos.x,
							vPos.y, fZ);
					}
				}
			}

			if (Engine::FindList(L"Trap") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Trap = Engine::FindList(L"Trap")->begin();
				list<Engine::CGameObject*>::iterator iter_Trap_end = Engine::FindList(L"Trap")->end();
				for (iter_Trap; iter_Trap != iter_Trap_end; iter_Trap++)
				{
					if (((CTrap*)(*iter_Trap))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CTrap*)(*iter_Trap))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CTrap*)(*iter_Trap))->GetTransCom()->Set_Position(vPos.x,
							vPos.y, fZ);
					}
				}
			}


			if( Engine::FindList(L"Effect") != NULL)
			{

				list<Engine::CGameObject*>::iterator iter_Effect = Engine::FindList(L"Effect")->begin();
				list<Engine::CGameObject*>::iterator iter_Effect_end = Engine::FindList(L"Effect")->end();
				for(iter_Effect; iter_Effect != iter_Effect_end; iter_Effect++)
				{
					if(((CSprite*)(*iter_Effect))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CSprite*)(*iter_Effect))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CSprite*)(*iter_Effect))->GetTransCom()->Set_Position(vPos.x,
							vPos.y, fZ);
					}
				}
			}

			if( Engine::FindList(L"Player") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Player")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Player")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CPlayer*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CPlayer*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CPlayer*)(*iter_Monster))->GetTransCom()->Set_Position(vPos.x,
							vPos.y, fZ);
					}
				}
			}

			if( Engine::FindList(L"Cube") != NULL)
			{
				list<Engine::CGameObject*>::iterator iter_Monster = Engine::FindList(L"Cube")->begin();
				list<Engine::CGameObject*>::iterator iter_Monster_end = Engine::FindList(L"Cube")->end();
				for(iter_Monster; iter_Monster != iter_Monster_end;iter_Monster++)
				{
					if(((CCube*)(*iter_Monster))->GetMode() == MODE_MODIFY)
					{
						_vec3 vPos;
						((CCube*)(*iter_Monster))->GetTransCom()->Get_Infomation(Engine::INFO_POSITION, &vPos);

						((CCube*)(*iter_Monster))->GetTransCom()->Set_Position(vPos.x,
							vPos.y, fZ);
					}
				}
			}
		}


	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMapToolTab::OnBnClickedButtonEyetomid()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	Engine::CGameObject* pGameObject = NULL;
	pGameObject = Engine::Find(L"MainCamera");
	((CCamera_MapTool*)pGameObject)->SetCamEye(_vec3(0.f, 10.f, 0.f));

	Engine::Safe_Release(pGameObject);

}



void CMapToolTab::OnBnClickedSaveobjBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DWORD	dwByte = 0;
	HANDLE	hFile = NULL;

	CFileDialog	FileDlg(FALSE, NULL, NULL, OFN_EXPLORER, NULL, NULL);

	TCHAR	szCurDir[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH - 1, szCurDir);
	PathRemoveFileSpec(szCurDir);
	PathRemoveFileSpec(szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"../../../Client/Bin/Data");
	FileDlg.m_ofn.lpstrInitialDir = szCurDir;
	CString strPathName;

	if(FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();
	}

	if(strPathName == L"")
	{
//		MSG_BOX(L"파일 개방 실패");
		return;
	}

	hFile = CreateFile(strPathName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	if(NULL != Engine::FindList(L"Environment"))
	{
		list<Engine::CGameObject*>* pObject = Engine::FindList(L"Environment");

		list<Engine::CGameObject*>::iterator iter =  pObject->begin();
		list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

		for(; iter != iter_end; ++iter)
		{
			WriteFile(hFile, ((CEnvironment*)(*iter))->GetObjectInfo(), sizeof(OBJ_INFO), &dwByte, NULL);
		}

	}

	if(NULL != Engine::FindList(L"NPC"))
	{
		list<Engine::CGameObject*>* pObject = Engine::FindList(L"NPC");

		list<Engine::CGameObject*>::iterator iter =  pObject->begin();
		list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

		for(; iter != iter_end; ++iter)
		{
			WriteFile(hFile, ((CNPC*)(*iter))->GetObjectInfo(), sizeof(OBJ_INFO), &dwByte, NULL);
		}

	}

	if(NULL != Engine::FindList(L"Monster"))
	{
		list<Engine::CGameObject*>* pObject = Engine::FindList(L"Monster");

		list<Engine::CGameObject*>::iterator iter =  pObject->begin();
		list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

		for(; iter != iter_end; ++iter)
		{
			WriteFile(hFile, ((CMonster*)(*iter))->GetObjectInfo(), sizeof(OBJ_INFO), &dwByte, NULL);
		}

	}

	if (NULL != Engine::FindList(L"Trap"))
	{
		list<Engine::CGameObject*>* pObject = Engine::FindList(L"Trap");

		list<Engine::CGameObject*>::iterator iter = pObject->begin();
		list<Engine::CGameObject*>::iterator iter_end = pObject->end();

		for (; iter != iter_end; ++iter)
		{
			WriteFile(hFile, ((CTrap*)(*iter))->GetObjectInfo(), sizeof(OBJ_INFO), &dwByte, NULL);
		}

	}

	if(NULL != Engine::FindList(L"Cube"))
	{
		list<Engine::CGameObject*>* pObject = Engine::FindList(L"Cube");

		list<Engine::CGameObject*>::iterator iter =  pObject->begin();
		list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

		for(; iter != iter_end; ++iter)
		{
			WriteFile(hFile, ((CCube*)(*iter))->GetObjectInfo(), sizeof(OBJ_INFO), &dwByte, NULL);
		}

	}

	if(NULL != Engine::FindList(L"Effect"))
	{
		list<Engine::CGameObject*>* pObject = Engine::FindList(L"Effect");

		list<Engine::CGameObject*>::iterator iter =  pObject->begin();
		list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

		for(; iter != iter_end; ++iter)
		{
			WriteFile(hFile, ((CSprite*)(*iter))->GetObjectInfo(), sizeof(OBJ_INFO), &dwByte, NULL);
		}

	}




	CloseHandle(hFile);

//	MSG_BOX(L"오브젝트 저장 성공!!");

}

void CMapToolTab::OnBnClickedLoadobjBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//list<Engine::CGameObject*>* pObject = &m_ListLandObj;
	//파일 다이얼로그 열기
	CFileDialog	FileDlg(TRUE, NULL, NULL, OFN_EXPLORER, NULL, NULL);

	TCHAR	szCurDir[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH - 1, szCurDir);
	PathRemoveFileSpec(szCurDir);
	PathRemoveFileSpec(szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"../../../Client/Bin/Data");
	FileDlg.m_ofn.lpstrInitialDir = szCurDir;

	CString strPathName;

	if(FileDlg.DoModal() == IDOK)
	{
		DWORD	dwByte = 0;

		strPathName = FileDlg.GetPathName();

		HANDLE hFile = CreateFile(strPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if( Engine::FindList(L"Environment") != NULL )
		{
			list<Engine::CGameObject*>* pObject = Engine::FindList(L"Environment");

			list<Engine::CGameObject*>::iterator iter =  pObject->begin();
			list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

			for(; iter != iter_end; ++iter)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
			}
		}

		if( Engine::FindList(L"NPC") != NULL )
		{
			list<Engine::CGameObject*>* pObject = Engine::FindList(L"NPC");

			list<Engine::CGameObject*>::iterator iter =  pObject->begin();
			list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

			for(; iter != iter_end; ++iter)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
			}
		}

		if( Engine::FindList(L"Monster") != NULL )
		{
			list<Engine::CGameObject*>* pObject = Engine::FindList(L"Monster");

			list<Engine::CGameObject*>::iterator iter =  pObject->begin();
			list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

			for(; iter != iter_end; ++iter)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
			}
		}

		if (Engine::FindList(L"Trap") != NULL)
		{
			list<Engine::CGameObject*>* pObject = Engine::FindList(L"Trap");

			list<Engine::CGameObject*>::iterator iter = pObject->begin();
			list<Engine::CGameObject*>::iterator iter_end = pObject->end();

			for (; iter != iter_end; ++iter)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
			}
		}


		if( Engine::FindList(L"Effect") != NULL )
		{
			list<Engine::CGameObject*>* pObject = Engine::FindList(L"Effect");

			list<Engine::CGameObject*>::iterator iter =  pObject->begin();
			list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

			for(; iter != iter_end; ++iter)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
			}
		}

		if( Engine::FindList(L"Cube") != NULL )
		{
			list<Engine::CGameObject*>* pObject = Engine::FindList(L"Cube");

			list<Engine::CGameObject*>::iterator iter =  pObject->begin();
			list<Engine::CGameObject*>::iterator iter_end =  pObject->end();

			for(; iter != iter_end; ++iter)
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
			}
		}

		Engine::SafeRelease();

		while(true)
		{
			OBJ_INFO*	pInfo = new OBJ_INFO;
			ReadFile(hFile, pInfo, sizeof(OBJ_INFO), &dwByte, NULL);
			Engine::CGameObject* pGameObject = NULL;

			if(dwByte == 0)
			{
				Engine::Safe_Delete(pInfo);
				break;
			}

			LPDIRECT3DDEVICE9 pGraphicDev = NULL;
			Engine::Get_GraphicDev(&pGraphicDev);

			wstring szTemp;

			szTemp = (wchar_t*)pInfo->m_szTag;

			if(szTemp == L"NPC")
				
			{
				pGameObject = CNPC::Create(pGraphicDev, pInfo->m_szName);

				((CNPC*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(Engine::LAYER_GAMELOGIC, L"NPC", pGameObject);
			}
			
			else if(szTemp == L"Environment")
			{
				pGameObject = CEnvironment::Create(pGraphicDev, pInfo->m_szName);

				((CEnvironment*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Environment", pGameObject);
			}

			else if(szTemp == L"Monster")
			{
				pGameObject = CMonster::Create(pGraphicDev, pInfo->m_szName);

				((CMonster*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Monster", pGameObject);
			}

			else if (szTemp == L"Trap")
			{
				pGameObject = CTrap::Create(pGraphicDev, pInfo->m_szName);

				((CTrap*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Trap", pGameObject);
			}

			else if(szTemp == L"Effect")
			{
				pGameObject = CSprite::LoadCreate(pGraphicDev, pInfo->m_szName);

				((CSprite*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Effect", pGameObject);
			}

			else if(szTemp == L"Cube")
			{
				pGameObject = CCube::Create(pGraphicDev, pInfo->m_szName);

				((CCube*)pGameObject)->SetObjectInfo(&pInfo->m_vPos,
					&pInfo->m_vScale,
					&pInfo->m_vAngle);

				Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Cube", pGameObject);
			}


			Engine::Safe_Release(pGraphicDev);

			delete pInfo;
			pInfo = NULL;
		}
		CloseHandle(hFile);
	}

}

void CMapToolTab::OnTvnItemexpandedTreeObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;


	TVITEM  item;

	item.mask = TVIF_HANDLE;
	item.hItem = pNMTreeView->itemNew.hItem;
	//CString strObj = m_ctrlTreeObject.GetItem(&item);            // 아이템 정보를 알아낸다.

	*pResult = 0;
}

