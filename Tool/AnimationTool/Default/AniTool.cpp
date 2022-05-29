// AniTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "AniTool.h"
#include "afxdialogex.h"

#include "Export_Engine.h"
#include "DynamicObject.h"
#include "StaticObject.h"

// CAniTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAniTool, CDialog)

CAniTool::CAniTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ANITOOL, pParent)
	, m_iMeshType(0)
	, m_iDynamicMeshType(0)
	, m_eMeshType(Engine::MESH_STATIC)
	, m_eDynamicMeshType(MESH_PLAYER)
	, m_iAniCount(0)
	, m_pmapAnimatorInfo(NULL)
	, m_iNextMotionContainerSize(0)
	, m_bIsChange(false)
{

}

CAniTool::~CAniTool()
{
	Release_Dynamic();
	Release_Static();
}

void CAniTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XFILELISTBOX, m_XFileListBox);
	DDX_Control(pDX, IDC_MESHCOMBOBOX, m_ChoiceComboBox);
	DDX_Radio(pDX, IDC_STATICMESHRADIO, m_iMeshType);
	DDX_Radio(pDX, IDC_PLAYERRADIO, m_iDynamicMeshType);
	DDX_Control(pDX, IDC_ANILISTBOX, m_AniListBox);
	DDX_Control(pDX, IDC_TIMESLIDER, m_TimeSlider);
	DDX_Control(pDX, IDC_LOOPCHECK, m_LoopCheck);
	DDX_Control(pDX, IDC_EVENTCHECK, m_EventCheck);
	DDX_Control(pDX, IDC_EVENTCOMBOBOX, m_EventComboBox);
	DDX_Control(pDX, IDC_STATECOMBOBOX, m_StateComboBox);
	DDX_Control(pDX, IDC_NEXTMOTIONCOMBOBOX, m_NextMotionComboBox);
	DDX_Control(pDX, IDC_NEXTMOTIONLIST, m_NextMotionList);
	DDX_Control(pDX, IDC_BONELIST, m_BoneList);
	DDX_Control(pDX, IDC_USEANIMATIONCHECK, m_AnimationCheck);
}


BEGIN_MESSAGE_MAP(CAniTool, CDialog)
	ON_WM_DROPFILES()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_XFILEDELBTN, &CAniTool::OnBnClickedXfiledelbtn)
	ON_BN_CLICKED(IDC_MESHCREATEBTN, &CAniTool::OnBnClickedMeshcreatebtn)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_STATICMESHRADIO, IDC_DYNAMICMESHRADIO, CAniTool::SetMeshTypeRadioStatus)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_PLAYERRADIO, IDC_MONSTERRADIO, CAniTool::SetDynamicMeshTypeRadioStatus)
	ON_BN_CLICKED(IDC_MESHDELBTN, &CAniTool::OnBnClickedMeshdelbtn)
	ON_LBN_SELCHANGE(IDC_ANILISTBOX, &CAniTool::OnLbnSelchangeAnilistbox)
	ON_BN_CLICKED(IDC_PLAYBTN, &CAniTool::OnBnClickedPlaybtn)
	ON_BN_CLICKED(IDC_STOPBTN, &CAniTool::OnBnClickedStopbtn)
	ON_BN_CLICKED(IDC_STARTTIMESETBTN, &CAniTool::OnBnClickedStarttimesetbtn)
	ON_BN_CLICKED(IDC_ENDTiMESETBTN, &CAniTool::OnBnClickedEndtimesetbtn)
	ON_BN_CLICKED(IDC_MOTIONSETBTN, &CAniTool::OnBnClickedMotionsetbtn)
	ON_CBN_SELCHANGE(IDC_EVENTCOMBOBOX, &CAniTool::OnCbnSelchangeEventcombobox)
	ON_BN_CLICKED(IDC_EVENTSETBTN, &CAniTool::OnBnClickedEventsetbtn)
	ON_CBN_SELCHANGE(IDC_STATECOMBOBOX, &CAniTool::OnCbnSelchangeStatecombobox)
	ON_BN_CLICKED(IDC_ADDMOTIONBTN, &CAniTool::OnBnClickedAddmotionbtn)
	ON_LBN_SELCHANGE(IDC_NEXTMOTIONLIST, &CAniTool::OnLbnSelchangeNextmotionlist)
	ON_BN_CLICKED(IDC_NEXTMOTIONDELBTN, &CAniTool::OnBnClickedNextmotiondelbtn)
	ON_BN_CLICKED(IDC_NEXTMOTIONSET, &CAniTool::OnBnClickedNextmotionset)
	ON_BN_CLICKED(IDC_ANISAVEBTN, &CAniTool::OnBnClickedAnisavebtn)
	ON_BN_CLICKED(IDC_ANILOADBTN, &CAniTool::OnBnClickedAniloadbtn)
	ON_LBN_SELCHANGE(IDC_BONELIST, &CAniTool::OnLbnSelchangeBonelist)
	ON_BN_CLICKED(IDC_STATICMESHINFOSETBTN, &CAniTool::OnBnClickedStaticmeshinfosetbtn)
	ON_BN_CLICKED(IDC_SAVESTATICBTN, &CAniTool::OnBnClickedSavestaticbtn)
	ON_BN_CLICKED(IDC_LOADSTATICBTN, &CAniTool::OnBnClickedLoadstaticbtn)
	ON_BN_CLICKED(IDC_CANCELTIMEBTN, &CAniTool::OnBnClickedCanceltimebtn)
	ON_BN_CLICKED(IDC_ANIMATIONUSESETBTN, &CAniTool::OnBnClickedAnimationusesetbtn)
END_MESSAGE_MAP()


// CAniTool 메시지 처리기입니다.
BOOL CAniTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	BaseAniSetting();
	BaseStaticSetting();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAniTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);

	_int iFileCnt = DragQueryFile(hDropInfo, -1, NULL, 0);

	for (_int i = 0; i < iFileCnt; i++)
	{
		_tchar szPath[MAX_PATH] = L"";
		_tchar szName[MAX_PATH] = L"";
		_tchar szTag[MAX_PATH] = L"";
		_tchar szRelativePath[MAX_PATH] = L"";
		_tchar szDirectoryPath[MAX_PATH] = L"";
		DragQueryFile(hDropInfo, i, szPath, MAX_PATH);
		GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);
		PathRelativePathTo(szRelativePath, szDirectoryPath, FILE_ATTRIBUTE_DIRECTORY,
			szPath, FILE_ATTRIBUTE_NORMAL);
		//경로 얻기
		lstrcpy(szPath, szRelativePath);
		PathRemoveFileSpec(szPath);
		lstrcat(szPath, L"\\");

		//파일명 얻기
		lstrcpy(szName, szRelativePath);
		PathStripPath(szName);

		//태그 얻어오기
		lstrcpy(szTag, szName);
		PathRemoveExtension(szTag);
		//lstrcat(L"Mesh_", szTag);

		//리소스 로드
		LPDIRECT3DDEVICE9 pGraphicDev = NULL;
		Engine::Get_GraphicDev(&pGraphicDev);

		if (FAILED(Engine::Ready_Mesh(pGraphicDev, RESOURCE_STAGE, szTag, m_eMeshType, szPath, szName)))
		{
			Engine::Safe_Release(pGraphicDev);
			return;
		}
		//리스트 박스에 추가
		m_XFileListBox.AddString(szTag);
		//콤보 박스에 추가
		m_ChoiceComboBox.AddString(szTag);
		Engine::Safe_Release(pGraphicDev);
	}

	UpdateData(FALSE);

	CDialog::OnDropFiles(hDropInfo);
}

void CAniTool::BaseAniSetting(void)
{
	m_TimeSlider.SetRange(0, 100);
	m_TimeSlider.SetPos(0);

	SetDlgItemText(IDC_CURANITIMEEDIT, L"0.0000000");
	SetDlgItemText(IDC_STARTTIMEEDIT, L"0.0000000");
	SetDlgItemText(IDC_ENDTIMEEDIT, L"0.0000000");
	SetDlgItemText(IDC_SPEEDEDIT, L"0.00");
	SetDlgItemText(IDC_EVENTTiMEEDIT, L"0.0000000");
	SetDlgItemText(IDC_MAGICNUMBEREDIT, L"0.0000000");
	SetDlgItemText(IDC_CANCELTIMEEDIT, L"0.0000000");
	m_LoopCheck.SetCheck(false);
	m_EventCheck.SetCheck(false);
	m_AnimationCheck.SetCheck(true);
	m_AniListBox.ResetContent();
	m_NextMotionList.ResetContent();
	m_EventComboBox.ResetContent();
	m_StateComboBox.ResetContent();
	m_NextMotionComboBox.ResetContent();
	m_BoneList.ResetContent();
	m_vecBone.clear();
}

void CAniTool::BaseStaticSetting(void)
{
	SetDlgItemText(IDC_POSXEDIT, L"0.00");
	SetDlgItemText(IDC_POSYEDIT, L"0.00");
	SetDlgItemText(IDC_POSZEDIT, L"0.00");
	SetDlgItemText(IDC_SCALEXEDIT, L"0.00");
	SetDlgItemText(IDC_SCALEYEDIT, L"0.00");
	SetDlgItemText(IDC_SCALEZEDIT, L"0.00");
	SetDlgItemText(IDC_ROTXEDIT, L"0.00");
	SetDlgItemText(IDC_ROTYEDIT, L"0.00");
	SetDlgItemText(IDC_ROTZEDIT, L"0.00");
}

void CAniTool::Release_Dynamic(void)
{
	//기본동작들 지우기
	for_each(m_mapMotionInfo.begin(), m_mapMotionInfo.end(), Engine::CDelete_Pair());
	m_mapMotionInfo.clear();

	//애니메이터 지우기
	for (_uint i = 0; i < m_iNextMotionContainerSize; i++)
	{
		MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[i].begin();
		MAPANIMATORINFO::iterator iter_end = m_pmapAnimatorInfo[i].end();

		for (iter; iter != iter_end; iter++)
		{
			for_each(iter->second.begin(), iter->second.end(), Engine::CDelete_Single());
			iter->second.clear();
		}
	}
	Engine::Safe_Delete_Array(m_pmapAnimatorInfo);
	MAPMOTIONEVENTINFO::iterator iter_event = m_mapMotionEventInfo.begin();
	MAPMOTIONEVENTINFO::iterator iter_event_end = m_mapMotionEventInfo.end();
	for (iter_event; iter_event != iter_event_end; iter_event++)
	{
		for_each(iter_event->second.begin(), iter_event->second.end(), Engine::CDelete_Single());
		iter_event->second.clear();
	}

	//애니메이션 사용여부 지우기
	MAPUSE::iterator iter_use = m_mapUse.begin();
	MAPUSE::iterator iter_use_end = m_mapUse.end();
	for (iter_use; iter_use != iter_use_end; iter_use++)
	{
		for_each(iter_use->second.begin(), iter_use->second.end(), Engine::CDelete_Single());
		iter_use->second.clear();
	}
}

void CAniTool::Release_Static(void) 
{
	for_each(m_mapWeapon.begin(), m_mapWeapon.end(), Engine::CDelete_Pair());
	m_mapWeapon.clear();
}

void CAniTool::SetAnimationData(const _int & iIndex)
{
	CString strData;
	_tchar szData[MAX_PATH] = L"";

	m_TimeSlider.SetPos(0);
	SetDlgItemText(IDC_CURANITIMEEDIT, L"0.000000");

	//기본 정보 셋팅
	m_LoopCheck.SetCheck(m_mapMotionInfo[iIndex]->bIsLoop);
	strData.Format(_T("%.2f"), m_mapMotionInfo[iIndex]->fSpeed);
	lstrcpy(szData, strData.GetString());
	SetDlgItemText(IDC_SPEEDEDIT, szData);

	strData.Format(_T("%.7f"), m_mapMotionInfo[iIndex]->dStartTime);
	lstrcpy(szData, strData.GetString());
	SetDlgItemText(IDC_STARTTIMEEDIT, szData);

	strData.Format(_T("%.7f"), m_mapMotionInfo[iIndex]->dEndTime);
	lstrcpy(szData, strData.GetString());
	SetDlgItemText(IDC_ENDTIMEEDIT, szData);

	//이벤트
	m_EventComboBox.SetCurSel(0);
	MAPMOTIONEVENTINFO::iterator iter_event = m_mapMotionEventInfo.find(iIndex);
	MOTIONEVENTLIST::iterator iter_event_list = iter_event->second.begin();
	
	m_EventCheck.SetCheck((*iter_event_list)->bUse);
	strData.Format(_T("%.7f"), (*iter_event_list)->dStartTime);
	lstrcpy(szData, strData.GetString());
	SetDlgItemText(IDC_EVENTTiMEEDIT, szData);

	

	//다음 동작들의 정보 셋팅
	m_StateComboBox.SetCurSel(0);
	m_NextMotionList.ResetContent();
	
	MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[0].find(iIndex);
	
	if (iter == m_pmapAnimatorInfo[0].end())
		return;

	ANIMATORLIST::iterator iter_list = iter->second.begin();
	ANIMATORLIST::iterator iter_list_end = iter->second.end();

	for (iter_list; iter_list != iter_list_end; iter_list++)
	{
		CString strAniName;
		m_AniListBox.GetText((*iter_list)->iNextIndex, strAniName);
		m_NextMotionList.AddString(strAniName.GetString());
	}
	//애니메이션 사용 여부
	MAPUSE::iterator iter_use = m_mapUse.find(iIndex);
	LISTUSE::iterator use_list = iter_use->second.begin();

	m_AnimationCheck.SetCheck(*(*use_list));

	//다음 동작 콤보박스 0번째꺼 선택
	m_NextMotionComboBox.SetCurSel(0);
	//다음동작 데이터들 초기화
	SetDlgItemText(IDC_MAGICNUMBEREDIT, L"0.0000000");
	SetDlgItemText(IDC_CANCELTIMEEDIT, L"0.0000000");
}

void CAniTool::Set_AnimationEdit(void)
{
	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;

	_double dPosition = ((CDynamicObject*)pGameObject)->Get_TrackPos();
	_double dPeriod = ((CDynamicObject*)pGameObject)->Get_Period();
	_double dPos = (dPosition / dPeriod) * 100.0;
	_bool bSet = false;

	m_TimeSlider.SetPos(_int(dPos));

	CString strPos;
	strPos.Format(_T("%.10f"), dPosition);

	_tchar szPos[MAX_PATH] = L"";
	lstrcpy(szPos, strPos.GetString());
	SetDlgItemText(IDC_CURANITIMEEDIT, szPos);

	_int iIndex = m_AniListBox.GetCurSel();
	_int iStateIndex = m_StateComboBox.GetCurSel();
	_int iNextMotionIndex = m_NextMotionList.GetCurSel();

	if ((iIndex < 0) || (iStateIndex < 0) || (iNextMotionIndex < 0))
	{
		Engine::Safe_Release(pGameObject);
		return;
	}

	MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[iStateIndex].find(iIndex);
	ANIMATORLIST::iterator iter_list = iter->second.begin();
	for (_int i = 0; i < iNextMotionIndex; i++)
		iter_list++;

	if (!m_bIsChange)
	{
		_double dMagicNumber = max((m_mapMotionInfo[iIndex]->dEndTime - (*iter_list)->dCancelTime), 0.0);
		if (((CDynamicObject*)pGameObject)->Is_AnimationEnd(dMagicNumber))
		{
			((CDynamicObject*)pGameObject)->Set_Playing(false);
			((CDynamicObject*)pGameObject)->Set_AniTrackPos(m_mapMotionInfo[iIndex]->dStartTime);
			bSet = true;
			m_bIsChange = true;
		}
	}

	if (m_bIsChange && bSet)
	{
		((CDynamicObject*)pGameObject)->Set_AnimationSet((*iter_list)->iNextIndex, m_mapMotionInfo[(*iter_list)->iNextIndex], (*iter_list)->dMagicNumber);
		((CDynamicObject*)pGameObject)->Set_Playing(true);
	}

	if (m_bIsChange)
	{
		if (((CDynamicObject*)pGameObject)->Is_AnimationEnd())
		{
			((CDynamicObject*)pGameObject)->Set_Playing(false);
			((CDynamicObject*)pGameObject)->Set_AniTrackPos(m_mapMotionInfo[(*iter_list)->iNextIndex]->dStartTime);
			((CDynamicObject*)pGameObject)->Set_AnimationSet(iIndex, m_mapMotionInfo[iIndex]);
			m_bIsChange = false;
		}
	}
	

	Engine::Safe_Release(pGameObject);
}

void CAniTool::MouseClick(POINT ptMouse)
{
	list<Engine::CGameObject*>* pList = Engine::FindList(L"StaticObject");

	if (pList == NULL)
		return;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for (iter; iter != iter_end; iter++)
	{
		((CStaticObject*)(*iter))->SetSelected(false);
	}

	iter = pList->begin();

	for (iter; iter != iter_end; iter++)
	{
		if (((CStaticObject*)(*iter))->GetMouseCol(ptMouse))
			return;
	}

}

void CAniTool::OnBnClickedXfiledelbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iIndex = -1;
	_tchar szTag[MAX_PATH] = L"";

	iIndex = m_XFileListBox.GetCurSel();

	if (iIndex < 0)
		return;

	m_XFileListBox.GetText(iIndex, szTag);
	m_XFileListBox.DeleteString(iIndex);

	iIndex = m_ChoiceComboBox.FindString(0, szTag);
	m_ChoiceComboBox.DeleteString(iIndex);
	if (iIndex > 0)
		m_ChoiceComboBox.SetCurSel(iIndex - 1);

	Engine::Delete_Resource(RESOURCE_STAGE, szTag);

	UpdateData(FALSE);
}

void CAniTool::OnBnClickedMeshcreatebtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_eMeshType == Engine::MESH_STATIC)
	{
		Engine::CGameObject* pGameObject = NULL;

		_int iIndex = -1;
		iIndex = m_ChoiceComboBox.GetCurSel();

		if (iIndex < 0)
			return;
		_tchar szTag[MAX_PATH] = L"";
		m_ChoiceComboBox.GetLBText(iIndex, szTag);

		LPDIRECT3DDEVICE9 pGraphicDev = NULL;
		Engine::Get_GraphicDev(&pGraphicDev);
		pGameObject = CStaticObject::Create(pGraphicDev, szTag);
		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"StaticObject", pGameObject);

		WEAPONINFO* pWeaponInfo = new WEAPONINFO;

		ZeroMemory(pWeaponInfo->szBoneName, sizeof(char) * 128);
		pWeaponInfo->vPos = _vec3(0.f, 0.f, 0.f);
		pWeaponInfo->vScale = _vec3(1.f, 1.f, 1.f);
		pWeaponInfo->vRotate = _vec3(0.f, 0.f, 0.f);

		m_mapWeapon.insert(MAPWEAPONINFO::value_type(szTag, pWeaponInfo));

		SetDlgItemText(IDC_POSXEDIT, L"0.00");
		SetDlgItemText(IDC_POSYEDIT, L"0.00");
		SetDlgItemText(IDC_POSZEDIT, L"0.00");
		SetDlgItemText(IDC_SCALEXEDIT, L"1.00");
		SetDlgItemText(IDC_SCALEYEDIT, L"1.00");
		SetDlgItemText(IDC_SCALEZEDIT, L"1.00");
		SetDlgItemText(IDC_ROTXEDIT, L"0.00");
		SetDlgItemText(IDC_ROTYEDIT, L"0.00");
		SetDlgItemText(IDC_ROTZEDIT, L"0.00");

		Engine::Safe_Release(pGraphicDev);
	}

	if (m_eMeshType == Engine::MESH_DYNAMIC)
	{
		//하나만 생성 가능해서 다이나믹 오브젝트가 잇는지 찾아
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = Engine::FindWithTag(L"DynamicObject");
		//있으면 리턴
		if (pGameObject != NULL)
		{
			Engine::Safe_Release(pGameObject);
			return;
		}
		//선택된 셀을 찾아
		_int iIndex = -1;
		iIndex = m_ChoiceComboBox.GetCurSel();
		//없으면 리턴
		if (iIndex < 0)
			return;
		//선택된 셀에서 생성할 메쉬 태그를 가저와
		_tchar szTag[MAX_PATH] = L"";
		m_ChoiceComboBox.GetLBText(iIndex, szTag);
		//객체를 생성하기위한 디바이스
		LPDIRECT3DDEVICE9 pGraphicDev = NULL;
		Engine::Get_GraphicDev(&pGraphicDev);
		//객체생성 없으면 리턴
		pGameObject = CDynamicObject::Create(pGraphicDev, szTag);
		if (pGameObject == NULL)
			return;
		Engine::AddObject(Engine::LAYER_GAMELOGIC, L"DynamicObject", pGameObject);

		//플레이어와 몬스터의 상태
		switch (m_eDynamicMeshType)
		{
		case MESH_PLAYER:
			m_StateComboBox.AddString(L"Town");				//마을
			m_StateComboBox.AddString(L"Dungeon");			//던전에서 칼을 집어넣었을때
			m_StateComboBox.AddString(L"Fight");			//칼뽑고 있을때
			m_pmapAnimatorInfo = new MAPANIMATORINFO[PLAYER_END];
			m_iNextMotionContainerSize = PLAYER_END;
			break;

		case MESH_MONSTER:
			m_StateComboBox.AddString(L"Normal");			//배회
			m_StateComboBox.AddString(L"Fight");			//전투가능?
			m_pmapAnimatorInfo = new MAPANIMATORINFO[MONSTER_END];
			m_iNextMotionContainerSize = MONSTER_END;
			break;
		}

		//다이나믹 메쉬에서 애니메이션 갯수 가저와
		Engine::CDynamicMesh* pMesh = (Engine::CDynamicMesh*)(pGameObject->GetComponent<Engine::CDynamicMesh>(Engine::COMID_STATIC));
		_uint iMaxIndex = pMesh->Get_MaxAnimationSet();
		m_iAniCount = iMaxIndex;

		for (_uint i = 0; i < m_iAniCount; i++)
		{
			_tchar szAniName[MAX_PATH] = L"";
			const char* pAniName = NULL;
			pAniName = pMesh->Get_AnimationName(i);
			MultiByteToWideChar(CP_ACP, 0, pAniName, strlen(pAniName), szAniName, 128);
			m_AniListBox.AddString(szAniName);
			m_NextMotionComboBox.AddString(szAniName);
			//애니메이션 기본동작
			Engine::MOTIONINFO* pMotionInfo = new Engine::MOTIONINFO;
			pMotionInfo->iIndex = i;
			pMotionInfo->bIsLoop = false;
			pMotionInfo->fSpeed = 1.f;
			pMotionInfo->dStartTime = 0.0000000;
			pMotionInfo->dEndTime = pMesh->Get_Period(i);
			m_mapMotionInfo.insert(MAPMOTIONINFO::value_type(i, pMotionInfo));
			//애니메이션 이벤트
			MOTIONEVENTLIST listMotionEvent;
			for (_uint j = 0; j < EVENT_END; j++)
			{
				Engine::MOTIONEVENTINFO* pMotionEventInfo = new Engine::MOTIONEVENTINFO;
				pMotionEventInfo->bUse = false;
				pMotionEventInfo->iEvent = j;
				pMotionEventInfo->dStartTime = 0.0000000;
				listMotionEvent.push_back(pMotionEventInfo);
			}
			m_mapMotionEventInfo.insert(MAPMOTIONEVENTINFO::value_type(i, listMotionEvent));
			//다음 모션에 빈 리스트 집어 넣음.
			if (m_eDynamicMeshType == MESH_PLAYER)
			{
				LISTUSE UseList;
				for (_uint j = 0; j < PLAYER_END; j++)
				{
					ANIMATORLIST list = ANIMATORLIST();
					m_pmapAnimatorInfo[j].insert(MAPANIMATORINFO::value_type(i, list));
					_bool* pUse = new _bool;
					*pUse = true;
					UseList.push_back(pUse);
				}
				m_mapUse.insert(MAPUSE::value_type(i, UseList));
			}

			if (m_eDynamicMeshType == MESH_MONSTER)
			{
				LISTUSE UseList;
				for (_uint j = 0; j < MONSTER_END; j++)
				{
					ANIMATORLIST list = ANIMATORLIST();
					m_pmapAnimatorInfo[j].insert(MAPANIMATORINFO::value_type(i, list));
					_bool* pUse = new _bool;
					*pUse = true;
					UseList.push_back(pUse);
				}
				m_mapUse.insert(MAPUSE::value_type(i, UseList));
			}
		}
		CString strPos;
		CString strAniName;
		_tchar szPos[MAX_PATH] = L"";
		m_AniListBox.SetCurSel(0);
		m_AniListBox.GetText(0, strAniName);
		SetDlgItemText(IDC_SELANINAME, strAniName.GetString());
		//루프
		m_LoopCheck.SetCheck(m_mapMotionInfo[0]->bIsLoop);
		//스피드 시벌탱
		strPos.Format(_T("%.2f"), m_mapMotionInfo[0]->fSpeed);
		lstrcpy(szPos, strPos.GetString());
		SetDlgItemText(IDC_SPEEDEDIT, szPos);
		//애니메이션 시작시간
		strPos.Format(_T("%.7f"), m_mapMotionInfo[0]->dStartTime);
		lstrcpy(szPos, strPos.GetString());
		SetDlgItemText(IDC_STARTTIMEEDIT, szPos);
		//애니메이션 끝나는시간
		strPos.Format(_T("%.7f"), m_mapMotionInfo[0]->dEndTime);
		lstrcpy(szPos, strPos.GetString());
		SetDlgItemText(IDC_ENDTIMEEDIT, szPos);

		m_NextMotionComboBox.SetCurSel(0);
		//이벤트 추가
		m_EventComboBox.AddString(L"Effect");
		m_EventComboBox.AddString(L"Sound");
		m_EventComboBox.SetCurSel(0);
		MOTIONEVENTLIST::iterator iter_event = m_mapMotionEventInfo[0].begin();
		m_EventCheck.SetCheck((*iter_event)->bUse);
		strPos.Format(_T("%.7f"), (*iter_event)->dStartTime);
		lstrcpy(szPos, strPos.GetString());
		SetDlgItemText(IDC_EVENTTiMEEDIT, szPos);

		m_StateComboBox.SetCurSel(0);

		LISTUSE::iterator iter_use = m_mapUse[0].begin();
		m_AnimationCheck.SetCheck(*(*iter_use));

		((CDynamicObject*)pGameObject)->Set_AnimationSet(0, m_mapMotionInfo[0]);

		m_vecBone.clear();
		m_vecBone.reserve(50);

		pMesh->Get_BoneName(&m_vecBone);

		_uint iSize = m_vecBone.size();

		for (_uint i = 0; i < iSize; i++)
		{
			_tchar szBoneName[MAX_PATH] = L"";

			MultiByteToWideChar(CP_ACP, 0, m_vecBone[i], strlen(m_vecBone[i]), szBoneName, 128);
			m_BoneList.AddString(szBoneName);

			//==이펙트툴에도 본리스트를 띄우기위함==//
			m_pEffectTool->m_BoneList.AddString(szBoneName);
		}

		//==이펙트툴 벡터 본 포인터 입력=======//
		m_pEffectTool->m_pvecBone = &m_vecBone;
		//====================================//

		Engine::Safe_Release(pMesh);
		Engine::Safe_Release(pGraphicDev);
	}
	UpdateData(FALSE);
}

void CAniTool::SetMeshTypeRadioStatus(UINT value)
{
	UpdateData(TRUE);

	switch (m_iMeshType)
	{
	case 0:
		m_eMeshType = Engine::MESH_STATIC;
		break;

	case 1:
		m_eMeshType = Engine::MESH_DYNAMIC;
		break;
	}

	UpdateData(FALSE);
}

void CAniTool::SetDynamicMeshTypeRadioStatus(UINT value)
{
	UpdateData(TRUE);

	switch (m_iMeshType)
	{
	case 0:
		m_eDynamicMeshType = MESH_PLAYER;
		break;

	case 1:
		m_eDynamicMeshType = MESH_MONSTER;
		break;
	}

	UpdateData(FALSE);
}

void CAniTool::OnBnClickedMeshdelbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if(m_eMeshType == Engine::MESH_STATIC)
	{
		list<Engine::CGameObject*>* pList = NULL;
		pList = Engine::FindList(L"StaticObject");
		if (pList == NULL)
			return;

		list<Engine::CGameObject*>::iterator iter = pList->begin();
		list<Engine::CGameObject*>::iterator iter_end = pList->end();
		for (iter; iter != iter_end; iter++)
		{
			if (((CStaticObject*)(*iter))->IsSelected())
			{
				(*iter)->SetState(Engine::STATE_DESTROY);
				break;
			}
		}
	}

	if (m_eMeshType == Engine::MESH_DYNAMIC)
	{
		Engine::CGameObject* pGameObject = NULL;
		pGameObject = Engine::FindWithTag(L"DynamicObject");
		if (pGameObject == NULL)
			return;

		pGameObject->SetState(Engine::STATE_DESTROY);
		Engine::Safe_Release(pGameObject);

		Release_Dynamic();
		BaseAniSetting();
		//==이펙트 리스트 박스 삭제===============//
		m_pEffectTool->m_BoneList.ResetContent();

		m_pEffectTool->m_pvecBone = NULL;
		//=======================================//
	}
	UpdateData(FALSE);
}

void CAniTool::OnLbnSelchangeAnilistbox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_eMeshType == Engine::MESH_STATIC)
		return;
	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;

	Engine::CGameObject* pGameObject = NULL;
	pGameObject = Engine::FindWithTag(L"DynamicObject");
	if (pGameObject == NULL)
		return;

	m_bIsChange = false;
	
	_tchar szAniName[MAX_PATH] = L"";
	m_AniListBox.GetText(iIndex, szAniName);
	SetDlgItemText(IDC_SELANINAME, szAniName);

	((CDynamicObject*)pGameObject)->Set_AnimationSet(iIndex, m_mapMotionInfo[iIndex]);

	SetAnimationData(iIndex);
	
	Engine::Safe_Release(pGameObject);
	UpdateData(FALSE);
}

void CAniTool::OnBnClickedPlaybtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_eMeshType == Engine::MESH_STATIC)
		return;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;


	((CDynamicObject*)pGameObject)->Set_Playing(true);

	Engine::Safe_Release(pGameObject);
}

void CAniTool::OnBnClickedStopbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_eMeshType == Engine::MESH_STATIC)
		return;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;


	((CDynamicObject*)pGameObject)->Set_Playing(false);

	Engine::Safe_Release(pGameObject);
}

void CAniTool::OnBnClickedStarttimesetbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_eMeshType == Engine::MESH_STATIC)
		return;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;

	_double dPosition = ((CDynamicObject*)pGameObject)->Get_OldTrackPos();

	CString strPos;
	strPos.Format(_T("%.7f"), dPosition);

	_tchar szPos[MAX_PATH] = L"";
	lstrcpy(szPos, strPos.GetString());
	SetDlgItemText(IDC_STARTTIMEEDIT, szPos);

	Engine::Safe_Release(pGameObject);
	UpdateData(FALSE);
}

void CAniTool::OnBnClickedEndtimesetbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_eMeshType == Engine::MESH_STATIC)
		return;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;

	_double dPosition = ((CDynamicObject*)pGameObject)->Get_OldTrackPos();

	CString strPos;
	strPos.Format(_T("%.10f"), dPosition);

	_tchar szPos[MAX_PATH] = L"";
	lstrcpy(szPos, strPos.GetString());
	SetDlgItemText(IDC_ENDTIMEEDIT, szPos);

	Engine::Safe_Release(pGameObject);
	UpdateData(FALSE);
}

void CAniTool::OnBnClickedMotionsetbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_eMeshType == Engine::MESH_STATIC)
		return;
	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;

	Engine::CGameObject* pGameObject = NULL;
	pGameObject = Engine::FindWithTag(L"DynamicObject");
	if (pGameObject == NULL)
		return;

	_float fSpeed = 0.f;
	_double dStartTime = 0.0;
	_double dEndTime = 0.0;
	_bool bIsLoop = false;

	if (m_LoopCheck.GetCheck())
		bIsLoop = true;
	else
		bIsLoop = false;

	CString strData;
	GetDlgItemText(IDC_SPEEDEDIT, strData);
	fSpeed = (_float)_tstof(strData.GetString());

	GetDlgItemText(IDC_STARTTIMEEDIT, strData);
	dStartTime = (_double)_tstof(strData.GetString());

	GetDlgItemText(IDC_ENDTIMEEDIT, strData);
	dEndTime = (_double)_tstof(strData.GetString());

	m_mapMotionInfo[iIndex]->bIsLoop = bIsLoop;
	m_mapMotionInfo[iIndex]->fSpeed = fSpeed;
	m_mapMotionInfo[iIndex]->dStartTime = dStartTime;
	m_mapMotionInfo[iIndex]->dEndTime = dEndTime;

	((CDynamicObject*)pGameObject)->Set_AniInfo(m_mapMotionInfo[iIndex]);

	Engine::Safe_Release(pGameObject);
	UpdateData(FALSE);
}

void CAniTool::OnCbnSelchangeEventcombobox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_eMeshType == Engine::MESH_STATIC)
		return;

	_int iIndex = -1;
	_int iEventIndex = -1;

	iIndex = m_AniListBox.GetCurSel();
	iEventIndex = m_EventComboBox.GetCurSel();

	if (iIndex < 0)
		return;
	if (iEventIndex < 0)
		return;

	MOTIONEVENTLIST::iterator iter = m_mapMotionEventInfo[iIndex].begin();

	for (_int i = 0; i < iEventIndex; i++)
		iter++;
	if (iter == m_mapMotionEventInfo[iIndex].end())
		return;
	m_EventCheck.SetCheck((*iter)->bUse);

	CString strData;
	_tchar szData[MAX_PATH] = L"";
	
	strData.Format(_T("%.7f"), (*iter)->dStartTime);
	lstrcpy(szData, strData.GetString());
	SetDlgItemText(IDC_EVENTTiMEEDIT, szData);

	UpdateData(FALSE);
}

void CAniTool::OnBnClickedEventsetbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_eMeshType == Engine::MESH_STATIC)
		return;

	_int iIndex = -1;
	_int iEventIndex = -1;

	iIndex = m_AniListBox.GetCurSel();
	iEventIndex = m_EventComboBox.GetCurSel();

	if (iIndex < 0)
		return;
	if (iEventIndex < 0)
		return;

	MOTIONEVENTLIST::iterator iter = m_mapMotionEventInfo[iIndex].begin();

	for (_int i = 0; i < iEventIndex; i++)
		iter++;
	if (iter == m_mapMotionEventInfo[iIndex].end())
		return;

	if (!m_EventCheck.GetCheck())
	{
		(*iter)->bUse = false;
		(*iter)->dStartTime = 0.0000000;
		SetDlgItemText(IDC_EVENTTiMEEDIT, L"0.0000000");
		return;
	}

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;

	_double dPosition = ((CDynamicObject*)pGameObject)->Get_OldTrackPos();
	CString strPos;
	strPos.Format(_T("%.10f"), dPosition);

	_tchar szPos[MAX_PATH] = L"";
	lstrcpy(szPos, strPos.GetString());
	SetDlgItemText(IDC_EVENTTiMEEDIT, szPos);

	(*iter)->bUse = true;
	(*iter)->dStartTime = dPosition;

	Engine::Safe_Release(pGameObject);
	UpdateData(FALSE);
}

void CAniTool::OnCbnSelchangeStatecombobox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;

	_int iStateIndex = -1;
	iStateIndex = m_StateComboBox.GetCurSel();
	if (iStateIndex < 0)
		return;

	MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[iStateIndex].find(iIndex);

	if (iter == m_pmapAnimatorInfo[iStateIndex].end())
		return;

	m_NextMotionList.ResetContent();

	ANIMATORLIST::iterator iter_list = iter->second.begin();
	ANIMATORLIST::iterator iter_list_end = iter->second.end();

	for (iter_list; iter_list != iter_list_end; iter_list++)
	{
		CString strAniName;
		m_AniListBox.GetText((*iter_list)->iNextIndex, strAniName);
		m_NextMotionList.AddString(strAniName.GetString());
	}

	LISTUSE::iterator iter_use = m_mapUse[iIndex].begin();
	
	for (_int i = 0; i < iStateIndex; i++)
		iter_use++;
	
	m_AnimationCheck.SetCheck(**iter_use);

	SetDlgItemText(IDC_MAGICNUMBEREDIT, L"0.0000000");
	SetDlgItemText(IDC_CANCELTIMEEDIT, L"0.0000000");

	UpdateData(FALSE);
}

void CAniTool::OnBnClickedAddmotionbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;
	_int iNextIndex = -1;
	iNextIndex = m_NextMotionComboBox.GetCurSel();
	if (iNextIndex < 0)
		return;

	_int iStateIndex = -1;
	iStateIndex = m_StateComboBox.GetCurSel();
	if (iStateIndex < 0)
		return;

	Engine::ANIMATORINFO* pAnimatorInfo = new Engine::ANIMATORINFO;
	pAnimatorInfo->dCancelTime = m_mapMotionInfo[iIndex]->dEndTime;
	pAnimatorInfo->dMagicNumber = 0.25;
	pAnimatorInfo->iNextIndex = iNextIndex;

	MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[iStateIndex].find(iIndex);

	iter->second.push_back(pAnimatorInfo);

	CString strAniName;
	m_AniListBox.GetText(iNextIndex, strAniName);

	m_NextMotionList.AddString(strAniName.GetString());

	UpdateData(FALSE);
}

void CAniTool::OnLbnSelchangeNextmotionlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;

	_int iStateIndex = -1;
	iStateIndex = m_StateComboBox.GetCurSel();
	if (iStateIndex < 0)
		return;

	_int iNextIndex = -1;
	iNextIndex = m_NextMotionList.GetCurSel();
	if (iNextIndex < 0)
		return;

	MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[iStateIndex].find(iIndex);
	ANIMATORLIST::iterator iter_list = iter->second.begin();

	for (_int i = 0; i < iNextIndex; i++)
		iter_list++;

	CString strData;
	_tchar szData[MAX_PATH] = L"";

	strData.Format(_T("%.7f"), (*iter_list)->dMagicNumber);
	lstrcpy(szData, strData.GetString());
	SetDlgItemText(IDC_MAGICNUMBEREDIT, szData);

	strData.Format(_T("%.7f"), (*iter_list)->dCancelTime);
	lstrcpy(szData, strData.GetString());
	SetDlgItemText(IDC_CANCELTIMEEDIT, szData);

	UpdateData(FALSE);
}

void CAniTool::OnBnClickedNextmotiondelbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;

	_int iStateIndex = -1;
	iStateIndex = m_StateComboBox.GetCurSel();
	if (iStateIndex < 0)
		return;

	_int iNextIndex = -1;
	iNextIndex = m_NextMotionList.GetCurSel();
	if (iNextIndex < 0)
		return;

	MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[iStateIndex].find(iIndex);
	ANIMATORLIST::iterator iter_list = iter->second.begin();

	for (_int i = 0; i < iNextIndex; i++)
		iter_list++;

	Engine::Safe_Delete(*iter_list);
	iter->second.erase(iter_list);
	m_NextMotionList.DeleteString(iNextIndex);

	SetDlgItemText(IDC_MAGICNUMBEREDIT, L"0.0000000");
	SetDlgItemText(IDC_CANCELTIMEEDIT, L"0.0000000");

	UpdateData(FALSE);
}

void CAniTool::OnBnClickedNextmotionset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;

	_int iStateIndex = -1;
	iStateIndex = m_StateComboBox.GetCurSel();
	if (iStateIndex < 0)
		return;

	_int iNextIndex = -1;
	iNextIndex = m_NextMotionList.GetCurSel();
	if (iNextIndex < 0)
		return;

	MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[iStateIndex].find(iIndex);
	ANIMATORLIST::iterator iter_list = iter->second.begin();

	for (_int i = 0; i < iNextIndex; i++)
		iter_list++;

	_double dMagicNumber = 0.0;
	_double dCancelTime = 0.0;
	CString strData;
	GetDlgItemText(IDC_MAGICNUMBEREDIT, strData);
	dMagicNumber = (_double)_tstof(strData.GetString());

	GetDlgItemText(IDC_CANCELTIMEEDIT, strData);
	dCancelTime = (_double)_tstof(strData.GetString());

	(*iter_list)->dCancelTime = dCancelTime;
	(*iter_list)->dMagicNumber = dMagicNumber;


	UpdateData(FALSE);
}

void CAniTool::OnBnClickedAnisavebtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog FileDlg(TRUE, NULL, NULL,
		OFN_EXPLORER | OFN_ALLOWMULTISELECT, NULL, NULL);
	TCHAR	szCurDir[MAX_PATH] = L"";

	//현재 경로 얻어오는 함수...
	GetCurrentDirectory(MAX_PATH - 1, szCurDir);
	PathRemoveFileSpec(szCurDir);

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;
	CString strPathName;
	CString strFileName;

	if (FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();
		CString strPath = FileDlg.GetPathName();

		DWORD dwByte = 0;
		HANDLE hFile = CreateFile(strPathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == NULL)
			return;

		WriteFile(hFile, &m_iAniCount, sizeof(_uint), &dwByte, NULL);
		WriteFile(hFile, &m_iNextMotionContainerSize, sizeof(_uint), &dwByte, NULL);

		for (_uint i = 0; i < m_iAniCount; i++)
		{
			//기본정보
			WriteFile(hFile, m_mapMotionInfo[i], sizeof(Engine::MOTIONINFO), &dwByte, NULL);
			//이벤트
			_uint iEventSize = EVENT_END;
			WriteFile(hFile, &iEventSize, sizeof(_uint), &dwByte, NULL);
			
			MOTIONEVENTLIST::iterator iter = m_mapMotionEventInfo[i].begin();
			MOTIONEVENTLIST::iterator iter_end = m_mapMotionEventInfo[i].end();

			for (iter; iter != iter_end; iter++)
			{
				WriteFile(hFile, (*iter), sizeof(Engine::MOTIONEVENTINFO), &dwByte, NULL);
			}
			
			//애니메이터
			for (_uint j = 0; j < m_iNextMotionContainerSize; j++)
			{
				MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[j].find(i);

				ANIMATORLIST::iterator iter_list = iter->second.begin();
				ANIMATORLIST::iterator iter_list_end = iter->second.end();

				_uint iNextMotionSize = iter->second.size();
				WriteFile(hFile, &iNextMotionSize, sizeof(_uint), &dwByte, NULL);

				for (iter_list; iter_list != iter_list_end; iter_list++)
				{
					WriteFile(hFile, (*iter_list), sizeof(Engine::ANIMATORINFO), &dwByte, NULL);
				}
			}
			//사용여부
			LISTUSE::iterator iter_use = m_mapUse[i].begin();
			LISTUSE::iterator iter_use_end = m_mapUse[i].end();
			for (iter_use; iter_use != iter_use_end; iter_use++)
			{
				WriteFile(hFile, (*iter_use), sizeof(_bool), &dwByte, NULL);
			}
		}


		CloseHandle(hFile);
	}
}

void CAniTool::OnBnClickedAniloadbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog FileDlg(TRUE, NULL, NULL,
		OFN_EXPLORER | OFN_ALLOWMULTISELECT, NULL, NULL);

	TCHAR	szCurDir[MAX_PATH] = L"";

	//현재 경로 얻어오는 함수...
	GetCurrentDirectory(MAX_PATH - 1, szCurDir);
	PathRemoveFileSpec(szCurDir);

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;
	CString strPathName;
	CString strFileName;


	if (FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();

		DWORD dwByte = 0;
		HANDLE hFile = CreateFile(strPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == NULL)
			return;

		Release_Dynamic();

		ReadFile(hFile, &m_iAniCount, sizeof(_uint), &dwByte, NULL);
		ReadFile(hFile, &m_iNextMotionContainerSize, sizeof(_uint), &dwByte, NULL);
		m_pmapAnimatorInfo = new MAPANIMATORINFO[m_iNextMotionContainerSize];

		for (_uint i = 0; i < m_iAniCount; i++)
		{
			//기본 동작 로드
			Engine::MOTIONINFO* pMotionInfo = new Engine::MOTIONINFO;
			ReadFile(hFile, pMotionInfo, sizeof(Engine::MOTIONINFO), &dwByte, NULL);
			m_mapMotionInfo.insert(MAPMOTIONINFO::value_type(i, pMotionInfo));
			//이벤트 로드
			_uint iEventSize = 0;
			ReadFile(hFile, &iEventSize, sizeof(_uint), &dwByte, NULL);
			for (_uint j = 0; j < iEventSize; j++)
			{
				Engine::MOTIONEVENTINFO* pMotionEventInfo = new Engine::MOTIONEVENTINFO;
				ReadFile(hFile, pMotionEventInfo, sizeof(Engine::MOTIONEVENTINFO), &dwByte, NULL);
				m_mapMotionEventInfo[i].push_back(pMotionEventInfo);
			}
			//애니메이터 로드
			for (_uint j = 0; j < m_iNextMotionContainerSize; j++)
			{
				ANIMATORLIST list;
				_uint iNextMotionSize = 0;
				ReadFile(hFile, &iNextMotionSize, sizeof(_uint), &dwByte, NULL);
				for (_uint k = 0; k < iNextMotionSize; k++)
				{
					Engine::ANIMATORINFO* pAnimatorInfo = new Engine::ANIMATORINFO;
					ReadFile(hFile, pAnimatorInfo, sizeof(Engine::ANIMATORINFO), &dwByte, NULL);
					list.push_back(pAnimatorInfo);
				}

				m_pmapAnimatorInfo[j].insert(MAPANIMATORINFO::value_type(i, list));
			}
			for (_uint j = 0; j < m_iNextMotionContainerSize; j++)
			{
				_bool* pUse = new _bool;
				ReadFile(hFile, pUse, sizeof(_bool), &dwByte, NULL);
				m_mapUse[i].push_back(pUse);
			}
		}

		CloseHandle(hFile);
	}
}

void CAniTool::OnLbnSelchangeBonelist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_eMeshType == Engine::MESH_DYNAMIC)
		return;

	list<Engine::CGameObject*>* pList = Engine::FindList(L"StaticObject");

	if (pList == NULL)
		return;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for (iter; iter != iter_end; iter++)
	{
		if (((CStaticObject*)(*iter))->IsSelected())
			break;
	}

	if (iter == iter_end)
		return;


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;

	_int iIndex = m_BoneList.GetCurSel();

	((CStaticObject*)(*iter))->SetBone(pGameObject, m_vecBone[iIndex]);

	wstring strName = (*iter)->GetName();
	MAPWEAPONINFO::iterator iter_wea = m_mapWeapon.find(strName);

	strcpy_s(iter_wea->second->szBoneName, sizeof(char) * 128, m_vecBone[iIndex]);


	Engine::Safe_Release(pGameObject);
}

void CAniTool::OnBnClickedStaticmeshinfosetbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_eMeshType == Engine::MESH_DYNAMIC)
		return;

	list<Engine::CGameObject*>* pList = Engine::FindList(L"StaticObject");

	if (pList == NULL)
		return;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for (iter; iter != iter_end; iter++)
	{
		if (((CStaticObject*)(*iter))->IsSelected())
			break;
	}

	if (iter == iter_end)
		return;



	_vec3 vPos;
	_vec3 vScale;
	_vec3 vRot;

	CString strData;
	GetDlgItemText(IDC_POSXEDIT, strData);
	vPos.x = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_POSYEDIT, strData);
	vPos.y = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_POSZEDIT, strData);
	vPos.z = (float)_tstof(strData.GetString());

	GetDlgItemText(IDC_SCALEXEDIT, strData);
	vScale.x = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_SCALEYEDIT, strData);
	vScale.y = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_SCALEZEDIT, strData);
	vScale.z = (float)_tstof(strData.GetString());

	GetDlgItemText(IDC_ROTXEDIT, strData);
	vRot.x = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_ROTYEDIT, strData);
	vRot.y = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_ROTZEDIT, strData);
	vRot.z = (float)_tstof(strData.GetString());

	((CStaticObject*)(*iter))->SetPos(vPos);
	((CStaticObject*)(*iter))->SetScale(vScale);
	((CStaticObject*)(*iter))->SetRot(vRot);

	_vec3 vAngle;
	((CStaticObject*)(*iter))->Get_Angle(&vAngle);

	wstring strName = (*iter)->GetName();
	MAPWEAPONINFO::iterator iter_wea = m_mapWeapon.find(strName);

	iter_wea->second->vPos = vPos;
	iter_wea->second->vScale = vScale;
	iter_wea->second->vRotate = vAngle;
	UpdateData(FALSE);
}

void CAniTool::OnBnClickedSavestaticbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_eMeshType == Engine::MESH_DYNAMIC)
		return;

	list<Engine::CGameObject*>* pList = Engine::FindList(L"StaticObject");

	if (pList == NULL)
		return;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for (iter; iter != iter_end; iter++)
	{
		if (((CStaticObject*)(*iter))->IsSelected())
			break;
	}

	if (iter == iter_end)
		return;

	wstring strName = (*iter)->GetName();

	MAPWEAPONINFO::iterator iter_wea = m_mapWeapon.find(strName);

	if (iter_wea == m_mapWeapon.end())
		return;

	CFileDialog FileDlg(TRUE, NULL, NULL,
		OFN_EXPLORER | OFN_ALLOWMULTISELECT, NULL, NULL);
	TCHAR	szCurDir[MAX_PATH] = L"";

	//현재 경로 얻어오는 함수...
	GetCurrentDirectory(MAX_PATH - 1, szCurDir);
	PathRemoveFileSpec(szCurDir);

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;
	CString strPathName;
	CString strFileName;

	if (FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();
		CString strPath = FileDlg.GetPathName();

		DWORD dwByte = 0;
		HANDLE hFile = CreateFile(strPathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == NULL)
			return;

		WriteFile(hFile, iter_wea->second, sizeof(WEAPONINFO), &dwByte, NULL);

		CloseHandle(hFile);
	}
}

void CAniTool::OnBnClickedLoadstaticbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_eMeshType == Engine::MESH_DYNAMIC)
		return;

	list<Engine::CGameObject*>* pList = Engine::FindList(L"StaticObject");

	if (pList == NULL)
		return;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for (iter; iter != iter_end; iter++)
	{
		if (((CStaticObject*)(*iter))->IsSelected())
			break;
	}

	if (iter == iter_end)
		return;

	wstring strName = (*iter)->GetName();

	MAPWEAPONINFO::iterator iter_wea = m_mapWeapon.find(strName);

	if (iter_wea == m_mapWeapon.end())
		return;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;

	CFileDialog FileDlg(TRUE, NULL, NULL,
		OFN_EXPLORER | OFN_ALLOWMULTISELECT, NULL, NULL);

	TCHAR	szCurDir[MAX_PATH] = L"";

	//현재 경로 얻어오는 함수...
	GetCurrentDirectory(MAX_PATH - 1, szCurDir);
	PathRemoveFileSpec(szCurDir);

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;
	CString strPathName;
	CString strFileName;


	if (FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();

		DWORD dwByte = 0;
		HANDLE hFile = CreateFile(strPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


		if (hFile == NULL)
			return;

		Engine::Safe_Delete(iter_wea->second);
		iter_wea = m_mapWeapon.erase(iter_wea);


		while (true)
		{
			WEAPONINFO* pWeaponInfo = new WEAPONINFO;

			ReadFile(hFile, pWeaponInfo, sizeof(WEAPONINFO), &dwByte, NULL);

			if (dwByte == 0)
			{
				Engine::Safe_Delete(pWeaponInfo);
				return;
			}

			m_mapWeapon.insert(MAPWEAPONINFO::value_type((*iter)->GetName(), pWeaponInfo));


			((CStaticObject*)(*iter))->SetBone(pGameObject, pWeaponInfo->szBoneName);
			((CStaticObject*)(*iter))->SetPos(pWeaponInfo->vPos);
			((CStaticObject*)(*iter))->SetScale(pWeaponInfo->vScale);
			((CStaticObject*)(*iter))->SetRot(pWeaponInfo->vRotate);
		}

		CloseHandle(hFile);
	}

	Engine::Safe_Release(pGameObject);
}

void CAniTool::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (m_eMeshType == Engine::MESH_STATIC)
		return;

	if (pScrollBar == NULL)
		return;

	if (pScrollBar->m_hWnd == m_TimeSlider.m_hWnd)
	{
		Engine::CGameObject* pGameObject = NULL;

		pGameObject = Engine::FindWithTag(L"DynamicObject");

		if (pGameObject == NULL)
			return;

		_int iPos = m_TimeSlider.GetPos();
		_double dPeriod = ((CDynamicObject*)pGameObject)->Get_Period();

		_double dPosition = (_double(iPos) / 100.0) * dPeriod;

		((CDynamicObject*)pGameObject)->Set_AniTrackPos(dPosition);

		CString strPos;
		strPos.Format(_T("%.7f"), dPosition);

		_tchar szPos[MAX_PATH] = L"";
		lstrcpy(szPos, strPos.GetString());
		SetDlgItemText(IDC_CURANITIMEEDIT, szPos);
		Engine::Safe_Release(pGameObject);
	}


	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CAniTool::OnBnClickedCanceltimebtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;

	_int iStateIndex = -1;
	iStateIndex = m_StateComboBox.GetCurSel();
	if (iStateIndex < 0)
		return;

	_int iNextIndex = -1;
	iNextIndex = m_NextMotionList.GetCurSel();
	if (iNextIndex < 0)
		return;

	MAPANIMATORINFO::iterator iter = m_pmapAnimatorInfo[iStateIndex].find(iIndex);
	ANIMATORLIST::iterator iter_list = iter->second.begin();

	for (_int i = 0; i < iNextIndex; i++)
		iter_list++;

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");

	if (pGameObject == NULL)
		return;

	_double dPosition = ((CDynamicObject*)pGameObject)->Get_OldTrackPos();
	CString strPos;
	strPos.Format(_T("%.10f"), dPosition);


	_tchar szPos[MAX_PATH] = L"";
	lstrcpy(szPos, strPos.GetString());
	SetDlgItemText(IDC_CANCELTIMEEDIT, szPos);

	(*iter_list)->dCancelTime = dPosition;

	Engine::Safe_Release(pGameObject);

	UpdateData(FALSE);
}


void CAniTool::OnBnClickedAnimationusesetbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iIndex = -1;
	iIndex = m_AniListBox.GetCurSel();
	if (iIndex < 0)
		return;

	_int iStateIndex = -1;
	iStateIndex = m_StateComboBox.GetCurSel();
	if (iStateIndex < 0)
		return;

	LISTUSE::iterator iter = m_mapUse[iIndex].begin();
	
	for (_int i = 0; i < iStateIndex; i++)
		iter++;

	if (m_AnimationCheck.GetCheck())
		*(*iter) = true;
	else
		*(*iter) = false;

	UpdateData(FALSE);
}
