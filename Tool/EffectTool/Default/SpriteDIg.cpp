// EffectSprite.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "SpriteDIg.h"
#include "MainFrm.h"
#include "Export_Engine.h"
#include "Sprite.h"

// CEffectSprite 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEffectSprite, CDialog)

CEffectSprite::CEffectSprite(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectSprite::IDD, pParent)
	, m_iPathNumber(0)
{
	m_pView = NULL;
	m_iDrawID = 0;

}

CEffectSprite::~CEffectSprite()
{
}

void CEffectSprite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_TextureComboBox);
	DDX_Control(pDX, IDC_SPRITEBILLBOARD, m_bButton);
	DDX_Control(pDX, IDC_CHECK1, m_bLoop);
	DDX_Radio(pDX, IDC_SPRITENORMAL, m_iPathNumber);
}


BEGIN_MESSAGE_MAP(CEffectSprite, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CEffectSprite::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_SPRITEBILLBOARD, &CEffectSprite::OnBnClickedSpritebillboard)
	ON_EN_CHANGE(IDC_SPRITECOUNT, &CEffectSprite::OnEnChangeSpritecount)
	ON_EN_CHANGE(IDC_SpriteSPEED, &CEffectSprite::OnEnChangeSpritespeed)
	ON_EN_CHANGE(IDC_MAXFRAME, &CEffectSprite::OnEnChangeMaxframe)
	ON_EN_CHANGE(IDC_ROWFRAME, &CEffectSprite::OnEnChangeRowframe)
	ON_EN_CHANGE(IDC_COLUMNFRAME, &CEffectSprite::OnEnChangeColumnframe)
	ON_EN_CHANGE(IDC_FRAMECENTERX, &CEffectSprite::OnEnChangeFramecenterx)
	ON_EN_CHANGE(IDC_FRAMECENTERY, &CEffectSprite::OnEnChangeFramecentery)
	ON_EN_CHANGE(IDC_FRAMECENTERZ, &CEffectSprite::OnEnChangeFramecenterz)
	ON_EN_CHANGE(IDC_SPRITESIZEX, &CEffectSprite::OnEnChangeSpritesizex)
	ON_EN_CHANGE(IDC_SPRITESIZEY, &CEffectSprite::OnEnChangeSpritesizey)
	ON_EN_CHANGE(IDC_SPRITESIZEZ, &CEffectSprite::OnEnChangeSpritesizez)
	ON_EN_CHANGE(IDC_SPRITECOLORR, &CEffectSprite::OnEnChangeSpritecolorr)
	ON_EN_CHANGE(IDC_SPRITECOLORG, &CEffectSprite::OnEnChangeSpritecolorg)
	ON_EN_CHANGE(IDC_SPRITECOLORB, &CEffectSprite::OnEnChangeSpritecolorb)
	ON_EN_CHANGE(IDC_SPRITECOLORA, &CEffectSprite::OnEnChangeSpritecolora)
	ON_BN_CLICKED(IDC_SPRITEPATH, &CEffectSprite::OnBnClickedSpritepath)
	ON_BN_CLICKED(IDC_SPRITESAVE, &CEffectSprite::OnBnClickedSpritesave)
	ON_BN_CLICKED(IDC_SPRITELOAD, &CEffectSprite::OnBnClickedSpriteload)
	ON_BN_CLICKED(IDC_CHECK1, &CEffectSprite::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CEffectSprite::OnBnClickedButton1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SPRITENORMAL, IDC_SPRITECOLOR, &CEffectSprite::SetSpritePathNumber)

END_MESSAGE_MAP()



// CEffectSprite 메시지 처리기입니다.

BOOL CEffectSprite::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitSpriteDIg();

	InitSpriteTexture();
	InitSettingNumber();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEffectSprite::InitSpriteDIg(void)
{
	m_pView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;
}

void CEffectSprite::InitSpriteTexture(void)
{
	if(m_TextureComboBox.GetCount() == 0)
	{
		CFileFind finder;

		TCHAR* tPath = L"../../../Client/Bin/Resource/Effect/Sprite/*.*";

		BOOL bFind  = finder.FindFile(tPath);

		int		iNumber = 0;

		CString Name = L"Sprite";

		CString Number = L"";

		CString Make = L".dds";

		while(bFind)
		{
			bFind = finder.FindNextFile();

			if(finder.GetFileName() == "."
				|| finder.GetFileName() == "..")
				continue;

			CString Result;

			
			Number.Format(_T("%d"), iNumber);


			Result = Name + Number + Make;


			m_TextureComboBox.AddString(Result);

			++iNumber;
		}
	}

	CString	strSelectName;

	m_TextureComboBox.SetCurSel(m_iDrawID);

	int iSelNum = m_TextureComboBox.GetCurSel();

	m_TextureComboBox.GetLBText(iSelNum, strSelectName);

	UpdateData(FALSE);
}

void CEffectSprite::InitSettingNumber(void)
{
	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	Engine::PARTICLEINFO*  SpriteInfo =((CSprite*)pGameObject)->GetParticleInfo();


	CString Speed, SenterX, SenterY, SenterZ, SizeX, SizeY, SizeZ, ColorR, ColorG, ColorB, ColorA;


	Speed.Format(_T("%.1f"), SpriteInfo->m_tSpriteInfo.m_fSpriteSpeed);
	SenterX.Format(_T("%.1f"), SpriteInfo->m_vStandard.x);
	SenterY.Format(_T("%.1f"), SpriteInfo->m_vStandard.y);
	SenterZ.Format(_T("%.1f"), SpriteInfo->m_vStandard.z);
	SizeX.Format(_T("%.1f"), SpriteInfo->m_vScale.x);
	SizeY.Format(_T("%.1f"), SpriteInfo->m_vScale.y);
	SizeZ.Format(_T("%.1f"), SpriteInfo->m_vScale.z);
	ColorR.Format(_T("%.1f"), SpriteInfo->m_dColor.r);
	ColorG.Format(_T("%.1f"), SpriteInfo->m_dColor.g);
	ColorB.Format(_T("%.1f"), SpriteInfo->m_dColor.b);
	ColorA.Format(_T("%.1f"), SpriteInfo->m_dColor.a);




	SetDlgItemInt(IDC_MAXFRAME, SpriteInfo->m_tSpriteInfo.m_iMaxFrame); //스프라이트 MaxFrame
	SetDlgItemInt(IDC_ROWFRAME, SpriteInfo->m_tSpriteInfo.m_iRow);		//스프라이트 RowFrame
	SetDlgItemInt(IDC_COLUMNFRAME, SpriteInfo->m_tSpriteInfo.m_iColumn); //스프라이트 ColumnFrame

	SetDlgItemInt(IDC_SPRITECOUNT, SpriteInfo->m_PrtN); //스프라이트 개수
	SetDlgItemText(IDC_SpriteSPEED, Speed); //스프라이트스피드
	SetDlgItemText(IDC_FRAMECENTERX, SenterX); //CenterX
	SetDlgItemText(IDC_FRAMECENTERY, SenterY); //CenterY
	SetDlgItemText(IDC_FRAMECENTERZ, SenterZ); //CenterZ

	SetDlgItemText(IDC_SPRITESIZEX, SizeX); //SizeX
	SetDlgItemText(IDC_SPRITESIZEY, SizeY); //SizeY
	SetDlgItemText(IDC_SPRITESIZEZ, SizeZ); //SizeY

	//Color
	SetDlgItemText(IDC_SPRITECOLORR, ColorR);
	SetDlgItemText(IDC_SPRITECOLORG, ColorG);
	SetDlgItemText(IDC_SPRITECOLORB, ColorB);
	SetDlgItemText(IDC_SPRITECOLORA, ColorA);



	
	//빌보드 체크
	if(SpriteInfo->m_bBillBoard)
		m_bButton.SetCheck(1); //1 : 체크박스 선택
	else
	{
		m_bButton.SetCheck(0);
	}


	//루프 체크
	if(SpriteInfo->m_bRoop)
		m_bLoop.SetCheck(1); //1 : 체크박스 선택
	else
	{
		m_bLoop.SetCheck(0);
	}



	Engine::Safe_Release(pGameObject);

	UpdateData(FALSE);
}

void CEffectSprite::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_iDrawID = m_TextureComboBox.GetCurSel();

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetParticleTexuture(m_iDrawID);


	((CSprite*)pGameObject)->SetRoof(true);


	
	//루프 체크
	if(((CSprite*)pGameObject)->GetParticleInfo()->m_bRoop)
		m_bLoop.SetCheck(1); //1 : 체크박스 선택
	else
	{
		m_bLoop.SetCheck(0);
	}

	Engine::Safe_Release(pGameObject);
}

//빌보드
void CEffectSprite::OnBnClickedSpritebillboard()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	if(m_bButton.GetCheck())
	{
		((CSprite*)pGameObject)->SetBillBoardCheck(true);
	}
	else
	{
		((CSprite*)pGameObject)->SetBillBoardCheck(false);
	}

	Engine::Safe_Release(pGameObject);

	UpdateData(FALSE);
}

void CEffectSprite::OnEnChangeSpritecount()
{
	_int m_iParticleCount = GetDlgItemInt(IDC_SPRITECOUNT);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ; 

	((CSprite*)pGameObject)->SetParticleCount(m_iParticleCount);


	Engine::Safe_Release(pGameObject);


	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CEffectSprite::OnEnChangeSpritespeed()
{
	UpdateData(TRUE);

	CString m_strSpeed;
	_float   m_fSpeed;


	GetDlgItemText(IDC_SpriteSPEED, m_strSpeed);

	m_fSpeed = (_float)_tstof(m_strSpeed);


	Engine::CGameObject* pGameObject = NULL;


	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSpriteSpeed(m_fSpeed);


	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//SpriteFrameMax 
void CEffectSprite::OnEnChangeMaxframe()
{
	UpdateData(TRUE);

	_int  MaxFrame;

	MaxFrame = GetDlgItemInt(IDC_MAXFRAME);


	if(MaxFrame < 1)
	{
		AfxMessageBox(L"범위초과");
		return ;
	}



	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSpriteFrame(MaxFrame, 0);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//SpriteFrameRow
void CEffectSprite::OnEnChangeRowframe()
{
	UpdateData(TRUE);

	_int  RowFrame;

	RowFrame = GetDlgItemInt(IDC_ROWFRAME);


	if(RowFrame < 1)
	{
		AfxMessageBox(L"범위초과");
		return ;
	}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSpriteFrame(RowFrame, 1);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//SpriteFrameColumn
void CEffectSprite::OnEnChangeColumnframe()
{
	UpdateData(TRUE);

	_int  ColumnFrame;

	ColumnFrame = GetDlgItemInt(IDC_COLUMNFRAME);

	if(ColumnFrame < 1)
	{
		AfxMessageBox(L"범위초과");
		return ;
	}

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSpriteFrame(ColumnFrame, 2);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



//Center Pos X
void CEffectSprite::OnEnChangeFramecenterx()
{
	UpdateData(TRUE);

	CString m_strX;

	float CenterX;

	GetDlgItemText(IDC_FRAMECENTERX, m_strX);


	CenterX = (_float)_tstof(m_strX);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;


	((CSprite*)pGameObject)->SetStandardPos(CenterX, 0);

	Engine::Safe_Release(pGameObject);


	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//Center Pos Y
void CEffectSprite::OnEnChangeFramecentery()
{
	UpdateData(TRUE);

	CString m_strY;

	float CenterY;

	GetDlgItemText(IDC_FRAMECENTERY, m_strY);


	CenterY = (_float)_tstof(m_strY);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;


	((CSprite*)pGameObject)->SetStandardPos(CenterY, 1);

	Engine::Safe_Release(pGameObject);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//Center Pos Z
void CEffectSprite::OnEnChangeFramecenterz()
{
	UpdateData(TRUE);

	CString m_strZ;

	_float CenterZ;

	GetDlgItemText(IDC_FRAMECENTERZ, m_strZ);


	CenterZ = (_float)_tstof(m_strZ);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;


	((CSprite*)pGameObject)->SetStandardPos(CenterZ, 2);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//CenterSizeX
void CEffectSprite::OnEnChangeSpritesizex()
{
	UpdateData(TRUE);


	CString m_strX;

	GetDlgItemText(IDC_SPRITESIZEX, m_strX);

	_float SizeX = (_float)_tstof(m_strX);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSize(SizeX, 0);

	Engine::Safe_Release(pGameObject);


	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//CenterSizeY
void CEffectSprite::OnEnChangeSpritesizey()
{

	UpdateData(TRUE);


	CString m_strY;

	GetDlgItemText(IDC_SPRITESIZEY, m_strY);

	_float SizeY = (_float)_tstof(m_strY);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSize(SizeY, 1);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//CenterSizeZ
void CEffectSprite::OnEnChangeSpritesizez()
{
	UpdateData(TRUE);


	CString m_strZ;

	GetDlgItemText(IDC_SPRITESIZEZ, m_strZ);

	_float SizeZ = (_float)_tstof(m_strZ);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSize(SizeZ, 2);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CEffectSprite::OnEnChangeSpritecolorr()
{
	UpdateData(TRUE);

	CString m_strColor;

	GetDlgItemText(IDC_SPRITECOLORR, m_strColor);

	_float	ColorR = 0.0f; 

	ColorR = (_float)_tstof(m_strColor);


	//if(ColorR < 0.0f || ColorR > 1.0f)
	//{
	//	AfxMessageBox(L"범위초과");
	//	return ;
	//}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetColor(ColorR, 0);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void CEffectSprite::OnEnChangeSpritecolorg()
{
	UpdateData(TRUE);

	CString m_strColor;

	GetDlgItemText(IDC_SPRITECOLORG, m_strColor);

	_float	ColorG = 0.0f; 

	ColorG = (_float)_tstof(m_strColor);


	//if(ColorG < 0.0f || ColorG > 1.0f)
	//{
	//	AfxMessageBox(L"범위초과");
	//	return ;
	//}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetColor(ColorG, 1);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CEffectSprite::OnEnChangeSpritecolorb()
{
	UpdateData(TRUE);

	CString m_strColor;

	GetDlgItemText(IDC_SPRITECOLORB, m_strColor);

	_float	ColorB = 0.0f; 

	ColorB = (_float)_tstof(m_strColor);


	//if(ColorB < 0.0f || ColorB > 1.0f)
	//{
	//	AfxMessageBox(L"범위초과");
	//	return ;
	//}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetColor(ColorB, 2);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CEffectSprite::OnEnChangeSpritecolora()
{

	UpdateData(TRUE);

	CString m_strColor;

	GetDlgItemText(IDC_SPRITECOLORA, m_strColor);

	_float	ColorA = 0.0f; 

	ColorA = (_float)_tstof(m_strColor);


	//if(ColorA < 0.0f || ColorA > 1.0f)
	//{
	//	AfxMessageBox(L"범위초과");
	//	return ;
	//}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetColor(ColorA, 3);

	Engine::Safe_Release(pGameObject);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//PathBtn
void CEffectSprite::OnBnClickedSpritepath()
{

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog	FileDlg(FALSE, L"dat", L"Sprite.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"*.dat", this);

	TCHAR	szCurDir[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH - 1, szCurDir);


	PathRemoveFileSpec(szCurDir);
	PathRemoveFileSpec(szCurDir);

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;

	CString strPathName;

	if(FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();
	}

	CString strRelativePath = ConvertRelativePath((LPCWSTR)strPathName).c_str();

	SetDlgItemText(IDC_SPRITETOPATH, strRelativePath);

	UpdateData(TRUE);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


std::wstring CEffectSprite::ConvertRelativePath(wstring strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szDirectoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);

	//한 경로에서 다른 경로로 가는 상대 경로를 추출하는 것임...
	PathRelativePathTo((LPWSTR)szRelativePath, szDirectoryPath,
		FILE_ATTRIBUTE_DIRECTORY, strFullPath.c_str(),
		FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}

//Save
void CEffectSprite::OnBnClickedSpritesave()
{
	CString strPath;
	GetDlgItemText(IDC_SPRITETOPATH, strPath);

	if(strPath == L"")
	{
		AfxMessageBox(L"경로 지정을 먼저 해야 됩니다.!");
		return;
	}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;


	//세이브...
	DWORD	dwByte = 0;
	HANDLE	hFile = CreateFile(strPath, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		AfxMessageBox(L"파일 개방 실패");
		return;
	}

	WriteFile(hFile, ((CSprite*)pGameObject)->GetParticleInfo(), sizeof(Engine::PARTICLEINFO), &dwByte, NULL);


	CloseHandle(hFile);
	Engine::Safe_Release(pGameObject);


	AfxMessageBox(L"Sprite Save OK!!");


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//Load
void CEffectSprite::OnBnClickedSpriteload()
{
	CFileDialog	FileDlg(TRUE, NULL, NULL,
		OFN_EXPLORER, NULL, NULL);

	TCHAR	szCurDir[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH - 1, szCurDir);
	PathRemoveFileSpec(szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\dat");
	FileDlg.m_ofn.lpstrInitialDir = szCurDir;

	CString strPathName;

	if(FileDlg.DoModal() == IDOK)
	{

		Engine::CGameObject* pGameObject = NULL;

		pGameObject = Engine::Find(L"ToolSprite");

		if(pGameObject == NULL)
			return ;

		DWORD	dwByte = 0;

		strPathName = FileDlg.GetPathName();

		HANDLE hFile = CreateFile(strPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


		while(true)
		{
			Engine::PARTICLEINFO Particle;

			ReadFile(hFile, &Particle, sizeof(Engine::PARTICLEINFO), &dwByte, NULL);

			if (dwByte == 0)
			{
				Engine::Safe_Release(pGameObject);
				break;
			}

			((CSprite*)pGameObject)->LoadSpriteInfo(&Particle);
			InitSettingNumber();
		}
		CloseHandle(hFile);
	}

	AfxMessageBox(L"Sprite Load OK!!");

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CEffectSprite::OnBnClickedCheck1()
{


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	if(m_bLoop.GetCheck())
	{
		((CSprite*)pGameObject)->SetRoof(true);
	}
	else
	{
		((CSprite*)pGameObject)->SetRoof(false);
	}

	Engine::Safe_Release(pGameObject);

	UpdateData(FALSE);



	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


//애니메이션 스타트
void CEffectSprite::OnBnClickedButton1()
{

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->ResetAnimation();

	Engine::Safe_Release(pGameObject);


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



//패스지정
void CEffectSprite::SetSpritePathNumber(UINT value)
{
	UpdateData(TRUE);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;


	switch(m_iPathNumber)
	{
	case 0: //Normal
		((CSprite*)pGameObject)->SetPath(0);
		break;

	case 1: //Black
		((CSprite*)pGameObject)->SetPath(1);
		break;

	case 2: //Color
		((CSprite*)pGameObject)->SetPath(2);
		break;
	}

	Engine::Safe_Release(pGameObject);
	UpdateData(FALSE);
}
