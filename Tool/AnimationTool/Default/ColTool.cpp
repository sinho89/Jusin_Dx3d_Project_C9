// ColTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "ColTool.h"

#include "DynamicObject.h"

#include "Export_Engine.h"


// CColTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CColTool, CDialog)

CColTool::CColTool(CWnd* pParent /*=NULL*/)
	: CDialog(CColTool::IDD, pParent)
	, m_iColTypeRadio(0)
{

}

CColTool::~CColTool()
{
}

void CColTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_SPHERECOLRADIO, m_iColTypeRadio);
}


BEGIN_MESSAGE_MAP(CColTool, CDialog)
	ON_BN_CLICKED(IDC_COLCREATEBTN, &CColTool::OnBnClickedColcreatebtn)
	ON_BN_CLICKED(IDC_COLDELBTN, &CColTool::OnBnClickedColdelbtn)
	ON_BN_CLICKED(IDC_COLADJUSTBTN, &CColTool::OnBnClickedColadjustbtn)
	ON_BN_CLICKED(IDC_COLSAVEBTN, &CColTool::OnBnClickedColsavebtn)
	ON_BN_CLICKED(IDC_COLLOADBTN, &CColTool::OnBnClickedColloadbtn)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SPHERECOLRADIO, IDC_BOXCOLRADIO, CColTool::SetColliderTypeRadioStatus)
END_MESSAGE_MAP()


// CColTool 메시지 처리기입니다.

void CColTool::OnBnClickedColcreatebtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");
	if(pGameObject == NULL)
		return;

	((CDynamicObject*)pGameObject)->Set_Collider(m_eColType);

	Engine::Safe_Release(pGameObject);
}

void CColTool::OnBnClickedColdelbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");
	if(pGameObject == NULL)
		return;

	((CDynamicObject*)pGameObject)->Delete_Collider();

	Engine::Safe_Release(pGameObject);
}

void CColTool::OnBnClickedColadjustbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_vec3 vPos = _vec3(0.f, 0.f, 0.f);
	_vec3 vScale = _vec3(1.f, 1.f, 1.f);
	_float fRad = 0.f;

	CString strData;
	GetDlgItemText(IDC_COLPOSX, strData);
	vPos.x = (_float)_tstof(strData.GetString());

	GetDlgItemText(IDC_COLPOSY, strData);
	vPos.y = (_float)_tstof(strData.GetString());

	GetDlgItemText(IDC_COLPOSZ, strData);
	vPos.z = (_float)_tstof(strData.GetString());

	GetDlgItemText(IDC_COLSIZEX, strData);
	vScale.x = (_float)_tstof(strData.GetString());

	GetDlgItemText(IDC_COLSIZEY, strData);
	vScale.y = (_float)_tstof(strData.GetString());

	GetDlgItemText(IDC_COLSIZEZ, strData);
	vScale.z = (_float)_tstof(strData.GetString());

	GetDlgItemText(IDC_RADIONEDIT, strData);
	fRad = (_float)_tstof(strData.GetString());

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");
	if(pGameObject == NULL)
		return;

	((CDynamicObject*)pGameObject)->Set_ColliderPos(vPos);
	if(m_iColTypeRadio == 0)
		((CDynamicObject*)pGameObject)->Set_ColliderSize(fRad);
	if(m_iColTypeRadio == 1)
		((CDynamicObject*)pGameObject)->Set_ColliderSize(vScale);

	Engine::Safe_Release(pGameObject);
}

void CColTool::OnBnClickedColsavebtn()
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

	if(FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();
		CString strPath = FileDlg.GetPathName();

		DWORD dwByte = 0;
		HANDLE hFile = CreateFile(strPathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if(hFile == NULL)
			return;

		WriteFile(hFile, &m_eColType, sizeof(Engine::COL_TYPE), &dwByte, NULL);

		_vec3 vPos = _vec3(0.f, 0.f, 0.f);
		_vec3 vScale = _vec3(1.f, 1.f, 1.f);
		_float fRad = 0.f;

		CString strData;
		GetDlgItemText(IDC_COLPOSX, strData);
		vPos.x = (_float)_tstof(strData.GetString());

		GetDlgItemText(IDC_COLPOSY, strData);
		vPos.y = (_float)_tstof(strData.GetString());

		GetDlgItemText(IDC_COLPOSZ, strData);
		vPos.z = (_float)_tstof(strData.GetString());

		GetDlgItemText(IDC_COLSIZEX, strData);
		vScale.x = (_float)_tstof(strData.GetString());

		GetDlgItemText(IDC_COLSIZEY, strData);
		vScale.y = (_float)_tstof(strData.GetString());

		GetDlgItemText(IDC_COLSIZEZ, strData);
		vScale.z = (_float)_tstof(strData.GetString());

		GetDlgItemText(IDC_RADIONEDIT, strData);
		fRad = (_float)_tstof(strData.GetString());

		WriteFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);

		if(m_iColTypeRadio == 0)
			WriteFile(hFile, &fRad, sizeof(_float), &dwByte, NULL);
		if(m_iColTypeRadio == 1)
			WriteFile(hFile, &vScale, sizeof(_vec3), &dwByte, NULL);
	}
}

void CColTool::OnBnClickedColloadbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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


	if(FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();

		DWORD dwByte = 0;
		HANDLE hFile = CreateFile(strPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


		if(hFile == NULL)
			return;

		Engine::COL_TYPE eType;
		_vec3 vPos;
		_vec3 vScale;
		_float fRad;

		ReadFile(hFile, &eType, sizeof(Engine::COL_TYPE), &dwByte, NULL);
		ReadFile(hFile, &vPos, sizeof(_vec3), &dwByte, NULL);

		if(eType == Engine::COL_SPHERE)
			ReadFile(hFile, &fRad, sizeof(_float), &dwByte, NULL);
		if(eType == Engine::COL_BOX)
			ReadFile(hFile, &vScale, sizeof(_vec3), &dwByte, NULL);


		Engine::CGameObject* pGameObject = NULL;

		pGameObject = Engine::FindWithTag(L"DynamicObject");
		if(pGameObject == NULL)
			return;

		((CDynamicObject*)pGameObject)->Delete_Collider();
		((CDynamicObject*)pGameObject)->Set_Collider(eType);
		((CDynamicObject*)pGameObject)->Set_ColliderPos(vPos);

		if(eType == Engine::COL_SPHERE)
			((CDynamicObject*)pGameObject)->Set_ColliderSize(fRad);
		if(eType == Engine::COL_BOX)
			((CDynamicObject*)pGameObject)->Set_ColliderSize(vScale);

		Engine::Safe_Release(pGameObject);

		UpdateData(TRUE);
		CString strPos;
		strPos.Format(_T("%.2f"), vPos.x);

		_tchar szPos[MAX_PATH] = L"";
		lstrcpy(szPos, strPos.GetString());
		SetDlgItemText(IDC_COLPOSX, szPos);

		strPos.Format(_T("%.2f"), vPos.y);
		lstrcpy(szPos, strPos.GetString());
		SetDlgItemText(IDC_COLPOSY, szPos);

		strPos.Format(_T("%.2f"), vPos.z);
		lstrcpy(szPos, strPos.GetString());
		SetDlgItemText(IDC_COLPOSZ, szPos);

		if(eType == Engine::COL_BOX)
		{
			strPos.Format(_T("%.2f"), vScale.x);
			lstrcpy(szPos, strPos.GetString());
			SetDlgItemText(IDC_COLSIZEX, szPos);

			strPos.Format(_T("%.2f"), vScale.y);
			lstrcpy(szPos, strPos.GetString());
			SetDlgItemText(IDC_COLSIZEY, szPos);

			strPos.Format(_T("%.2f"), vScale.z);
			lstrcpy(szPos, strPos.GetString());
			SetDlgItemText(IDC_COLSIZEZ, szPos);

			SetDlgItemText(IDC_RADIONEDIT, L"1");
			m_iColTypeRadio = 1;
		}
		if(eType == Engine::COL_SPHERE)
		{
			strPos.Format(_T("%.2f"), fRad);
			lstrcpy(szPos, strPos.GetString());
			SetDlgItemText(IDC_RADIONEDIT, szPos);

			SetDlgItemText(IDC_COLSIZEX, L"1");
			SetDlgItemText(IDC_COLSIZEY, L"1");
			SetDlgItemText(IDC_COLSIZEZ, L"1");
			m_iColTypeRadio = 0;
		}
		

		UpdateData(FALSE);
	}
}

void CColTool::SetColliderTypeRadioStatus(UINT value)
{
	UpdateData(TRUE);

	switch(m_iColTypeRadio)
	{
	case 0:
		m_eColType = Engine::COL_SPHERE;
		break;

	case 1:
		m_eColType = Engine::COL_BOX;
		break;
	}

	UpdateData(FALSE);
}

BOOL CColTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetDlgItemText(IDC_COLPOSX, L"0");
	SetDlgItemText(IDC_COLPOSY, L"0");
	SetDlgItemText(IDC_COLPOSZ, L"0");
	SetDlgItemText(IDC_COLSIZEX, L"1");
	SetDlgItemText(IDC_COLSIZEY, L"1");
	SetDlgItemText(IDC_COLSIZEZ, L"1");
	SetDlgItemText(IDC_RADIONEDIT, L"1");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
