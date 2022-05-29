// NaviTab.cpp : ���� �����Դϴ�.
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


// CNaviTab ��ȭ �����Դϴ�.

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


// CNaviTab �޽��� ó�����Դϴ�.

void CNaviTab::OnBnClickedAddcellRadio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CNaviTab::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CDialog::OnLButtonDown(nFlags, point);
}





void CNaviTab::OnBnClickedNaviSaveBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);  //üũ������ ���� ���� ���� m_ctlCheck ������ �����ϴ�.

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

	UpdateData(FALSE);  //������ ���ڿ� ���� ���� �����ϴ�.


}

void CNaviTab::OnBnClickedLinkCellBtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

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
