// NaviTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "NaviTab.h"
#include "Map_Ice.h"
#include "Map_Store.h"
#include "NavigationMesh.h"
#include "Cell.h"
#include "Monster.h"
#include "Player.h"
#include "Camera_MapTool.h"


// CNaviTab 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNaviTab, CDialog)

CNaviTab::CNaviTab(CWnd* pParent /*=NULL*/)
: CDialog(CNaviTab::IDD, pParent)
, m_iPointCount(0)
, m_pMap(NULL)
, m_ctlDebugCheck(FALSE)
{

}

CNaviTab::~CNaviTab()
{
}

void CNaviTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COUNT_EDIT, m_iPointCount);

	DDX_Check(pDX, IDC_DEBUG_CHECK, m_ctlDebugCheck);
}


BEGIN_MESSAGE_MAP(CNaviTab, CDialog)
	ON_BN_CLICKED(IDC_MODIFY_RADIO, &CNaviTab::OnBnClickedModifyRadio)
	ON_BN_CLICKED(IDC_ADDCELL_RADIO, &CNaviTab::OnBnClickedAddcellRadio)
	ON_BN_CLICKED(IDC_CHECKCELL_RADIO, &CNaviTab::OnBnClickedCheckcellRadio)
	ON_BN_CLICKED(IDC_NORMAL_RADIO, &CNaviTab::OnBnClickedNormalRadio)
	ON_BN_CLICKED(IDC_FALL_RADIO, &CNaviTab::OnBnClickedFallRadio)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_REMOVE_BTN, &CNaviTab::OnBnClickedRemoveBtn)
	ON_BN_CLICKED(IDC_DELETEALL_BTN, &CNaviTab::OnBnClickedDeleteallBtn)
	ON_BN_CLICKED(IDC_DELETE_BTN, &CNaviTab::OnBnClickedDeleteBtn)
	ON_BN_CLICKED(IDC_NAVI_SAVE_BTN, &CNaviTab::OnBnClickedNaviSaveBtn)
	ON_BN_CLICKED(IDC_NAVI_LOAD_BTN, &CNaviTab::OnBnClickedNaviLoadBtn)
	ON_BN_CLICKED(IDC_DEBUG_CHECK, &CNaviTab::OnBnClickedDebugCheck)
	ON_BN_CLICKED(IDC_LINK_CELL_BTN, &CNaviTab::OnBnClickedLinkCellBtn)
END_MESSAGE_MAP()


// CNaviTab 메시지 처리기입니다.

void CNaviTab::OnBnClickedAddcellRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		((CMap_Ice*)(*iter))->m_eNaviMode = CMap_Ice::MODE_ADD;
	}

	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		((CMap_Store*)(*iter))->m_eNaviMode = CMap_Store::MODE_ADD;
	}

}

void CNaviTab::OnBnClickedModifyRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		((CMap_Ice*)(*iter))->m_eNaviMode = CMap_Ice::MODE_MODIFY;
	}

	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		((CMap_Store*)(*iter))->m_eNaviMode = CMap_Store::MODE_MODIFY;
	}

}

void CNaviTab::OnBnClickedCheckcellRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		((CMap_Ice*)(*iter))->m_eNaviMode = CMap_Ice::MODE_CHECK;
	}

	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		((CMap_Store*)(*iter))->m_eNaviMode = CMap_Store::MODE_CHECK;
	}
}



void CNaviTab::OnBnClickedNormalRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		((CMap_Ice*)(*iter))->m_eNaviType = Engine::TYPE_NORMAL;
	}

	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		((CMap_Store*)(*iter))->m_eNaviType = Engine::TYPE_NORMAL;
	}
}

void CNaviTab::OnBnClickedFallRadio()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		((CMap_Ice*)(*iter))->m_eNaviType = Engine::TYPE_FALL;
	}

	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		((CMap_Store*)(*iter))->m_eNaviType = Engine::TYPE_FALL;
	}
}

void CNaviTab::OnBnClickedRemoveBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		if(((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->end();
			vector<Engine::CCell*>::iterator iter_Cell_Delete = --iter_Cell_end;
			iter_Cell_end = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->end();
			for(iter_Cell; iter_Cell != iter_Cell_end;)
			{
				if(iter_Cell == iter_Cell_Delete)
				{
					Engine::Safe_Release(*iter_Cell);
					((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->erase(iter_Cell);
					break;
				}
				else
					++iter_Cell;
			}
		}
	}

	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		if(((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->end();
			vector<Engine::CCell*>::iterator iter_Cell_Delete = --iter_Cell_end;
			iter_Cell_end = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->end();
			for(iter_Cell; iter_Cell != iter_Cell_end;)
			{
				if(iter_Cell == iter_Cell_Delete)
				{
					Engine::Safe_Release(*iter_Cell);
					((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->erase(iter_Cell);
					break;
				}
				else
					++iter_Cell;
			}
		}

	}
}

void CNaviTab::OnBnClickedDeleteBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		if(((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->end();

			for(iter_Cell; iter_Cell != iter_Cell_end;)
			{
				if((*iter_Cell)->GetType() == Engine::TYPE_SELECT)
				{
					Engine::Safe_Release(*iter_Cell);
					((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->erase(iter_Cell);
					break;
				}
				else
					++iter_Cell;
			}
		}

	}

	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		if(((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->end();

			for(iter_Cell; iter_Cell != iter_Cell_end;)
			{
				if((*iter_Cell)->GetType() == Engine::TYPE_SELECT)
				{
					Engine::Safe_Release(*iter_Cell);
					((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->erase(iter_Cell);
					break;
				}
				else
					++iter_Cell;
			}
		}
	}

}

void CNaviTab::OnBnClickedDeleteallBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		if(((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->end();

			for(iter_Cell; iter_Cell != iter_Cell_end; ++iter_Cell)
			{
				Engine::Safe_Release(*iter_Cell);
			}

			((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->clear();
		}

	}

	if(NULL != Engine::FindList(L"Map_Store"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		if(((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->end();

			for(iter_Cell; iter_Cell != iter_Cell_end; ++iter_Cell)
			{
				Engine::Safe_Release(*iter_Cell);		
			}

			((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->clear();
		}
	}
}




BOOL CNaviTab::OnInitDialog()
{
	CDialog::OnInitDialog();

	CButton* pCheck = (CButton*)GetDlgItem(IDC_ADDCELL_RADIO);
	pCheck->SetCheck(1);

	pCheck = (CButton*)GetDlgItem(IDC_NORMAL_RADIO);
	pCheck->SetCheck(1);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNaviTab::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}





void CNaviTab::OnBnClickedNaviSaveBtn()
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
		return;
	}

	hFile = CreateFile(strPathName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(Engine::FindList(L"Map_Ice") != NULL)
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		if(((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->end();

			for(; iter_Cell != iter_Cell_end; ++iter_Cell)
			{
				WriteFile(hFile, ((Engine::CCell*)(*iter_Cell))->GetNaviInfo(), sizeof(Engine::NAVIINFO), &dwByte, NULL);
			}

		}
	}

	if(Engine::FindList(L"Map_Store") != NULL)
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
		if(((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->end();

			for(; iter_Cell != iter_Cell_end; ++iter_Cell)
			{
				WriteFile(hFile, ((Engine::CCell*)(*iter_Cell))->GetNaviInfo(), sizeof(Engine::NAVIINFO), &dwByte, NULL);
			}

		}
	}



	CloseHandle(hFile);


}

void CNaviTab::OnBnClickedNaviLoadBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

		if(NULL != Engine::FindList(L"Map_Ice"))
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
			if(((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
			{
				vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
				vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->end();

				for(; iter_Cell != iter_Cell_end; ++iter_Cell)
				{
					Engine::Safe_Release((*iter_Cell));
				}
				((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->clear();
			}


			while(true)
			{
				Engine::NAVIINFO*	pInfo = new Engine::NAVIINFO;
				ReadFile(hFile, pInfo, sizeof(Engine::NAVIINFO), &dwByte, NULL);
				Engine::CGameObject* pGameObject = NULL;

				if(dwByte == 0)
				{
					Engine::Safe_Delete(pInfo);
					break;
				}

				((CMap_Ice*)(*iter))->GetNaviCom()->Add_Cell(&pInfo->vPoint[0], 
					&pInfo->vPoint[1],
					&pInfo->vPoint[2],
					pInfo->m_eType);

				delete pInfo;
				pInfo = NULL;

			}
		}

		if(NULL != Engine::FindList(L"Map_Store"))
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Store")->begin();
			if(((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
			{
				vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
				vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->end();

				for(; iter_Cell != iter_Cell_end; ++iter_Cell)
				{
					Engine::Safe_Release((*iter_Cell));
				}
				((CMap_Store*)(*iter))->GetNaviCom()->GetVECCELL()->clear();
			}


			while(true)
			{
				Engine::NAVIINFO*	pInfo = new Engine::NAVIINFO;
				ReadFile(hFile, pInfo, sizeof(Engine::NAVIINFO), &dwByte, NULL);
				Engine::CGameObject* pGameObject = NULL;

				if(dwByte == 0)
				{
					Engine::Safe_Delete(pInfo);
					break;
				}

				((CMap_Store*)(*iter))->GetNaviCom()->Add_Cell(&pInfo->vPoint[0], 
					&pInfo->vPoint[1],
					&pInfo->vPoint[2],
					pInfo->m_eType);

				delete pInfo;
				pInfo = NULL;

			}
		}

		CloseHandle(hFile);
	}

}

void CNaviTab::OnBnClickedDebugCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);  //체크상자의 현재 상태 값을 m_ctlCheck 변수로 보냅니다.

	if(m_ctlDebugCheck == TRUE)
	{
		if( Engine::FindList(L"Player") != NULL )
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Player")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Player")->end();
			for(iter; iter != iter_end;iter++)
			{
				((CPlayer*)(*iter))->SetMode(MODE_DEBUG);
			}
		}

		if( Engine::FindList(L"Monster") != NULL )
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Monster")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Monster")->end();
			for(iter; iter != iter_end;iter++)
			{
				((CMonster*)(*iter))->SetMode(MODE_DEBUG);
			}
		}
		if( Engine::FindList(L"Camera") != NULL )
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Camera")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Camera")->end();
			for(iter; iter != iter_end;iter++)
			{
				((CCamera_MapTool*)(*iter))->SetCamMode(CCamera_MapTool::MODE_DEBUG);
			}
		}
	}
	else
	{
		if( Engine::FindList(L"Player") != NULL )
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Player")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Player")->end();
			for(iter; iter != iter_end;iter++)
			{
				((CPlayer*)(*iter))->SetMode(MODE_STOP);
			}
		}

		if( Engine::FindList(L"Monster") != NULL )
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Monster")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Monster")->end();
			for(iter; iter != iter_end;iter++)
			{
				((CMonster*)(*iter))->SetMode(MODE_STOP);
			}
		}
		if( Engine::FindList(L"Camera") != NULL )
		{
			list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Camera")->begin();
			list<Engine::CGameObject*>::iterator iter_end = Engine::FindList(L"Camera")->end();
			for(iter; iter != iter_end;iter++)
			{
				((CCamera_MapTool*)(*iter))->SetCamMode(CCamera_MapTool::MODE_NORMAL);
			}
		}
	}

	UpdateData(FALSE);  //에디터 상자에 변수 값을 보냅니다.


}

void CNaviTab::OnBnClickedLinkCellBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(NULL != Engine::FindList(L"Map_Ice"))
	{
		list<Engine::CGameObject*>::iterator iter = Engine::FindList(L"Map_Ice")->begin();
		if(((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->size() != 0)
		{
			vector<Engine::CCell*>::iterator iter_Cell = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->begin();
			vector<Engine::CCell*>::iterator iter_Cell_end = ((CMap_Ice*)(*iter))->GetNaviCom()->GetVECCELL()->end();

			for(; iter_Cell != iter_Cell_end; ++iter_Cell)
			{
				((CMap_Ice*)(*iter))->GetNaviCom()->Link_Cell();
			}
		}
	}

}
