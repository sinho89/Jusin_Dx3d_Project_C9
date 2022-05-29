// EffectSprite.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "SpriteDIg.h"
#include "MainFrm.h"
#include "Export_Engine.h"
#include "Sprite.h"

// CEffectSprite ��ȭ �����Դϴ�.

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



// CEffectSprite �޽��� ó�����Դϴ�.

BOOL CEffectSprite::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitSpriteDIg();

	InitSpriteTexture();
	InitSettingNumber();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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




	SetDlgItemInt(IDC_MAXFRAME, SpriteInfo->m_tSpriteInfo.m_iMaxFrame); //��������Ʈ MaxFrame
	SetDlgItemInt(IDC_ROWFRAME, SpriteInfo->m_tSpriteInfo.m_iRow);		//��������Ʈ RowFrame
	SetDlgItemInt(IDC_COLUMNFRAME, SpriteInfo->m_tSpriteInfo.m_iColumn); //��������Ʈ ColumnFrame

	SetDlgItemInt(IDC_SPRITECOUNT, SpriteInfo->m_PrtN); //��������Ʈ ����
	SetDlgItemText(IDC_SpriteSPEED, Speed); //��������Ʈ���ǵ�
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



	
	//������ üũ
	if(SpriteInfo->m_bBillBoard)
		m_bButton.SetCheck(1); //1 : üũ�ڽ� ����
	else
	{
		m_bButton.SetCheck(0);
	}


	//���� üũ
	if(SpriteInfo->m_bRoop)
		m_bLoop.SetCheck(1); //1 : üũ�ڽ� ����
	else
	{
		m_bLoop.SetCheck(0);
	}



	Engine::Safe_Release(pGameObject);

	UpdateData(FALSE);
}

void CEffectSprite::OnCbnSelchangeCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_iDrawID = m_TextureComboBox.GetCurSel();

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetParticleTexuture(m_iDrawID);


	((CSprite*)pGameObject)->SetRoof(true);


	
	//���� üũ
	if(((CSprite*)pGameObject)->GetParticleInfo()->m_bRoop)
		m_bLoop.SetCheck(1); //1 : üũ�ڽ� ����
	else
	{
		m_bLoop.SetCheck(0);
	}

	Engine::Safe_Release(pGameObject);
}

//������
void CEffectSprite::OnBnClickedSpritebillboard()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//SpriteFrameMax 
void CEffectSprite::OnEnChangeMaxframe()
{
	UpdateData(TRUE);

	_int  MaxFrame;

	MaxFrame = GetDlgItemInt(IDC_MAXFRAME);


	if(MaxFrame < 1)
	{
		AfxMessageBox(L"�����ʰ�");
		return ;
	}



	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSpriteFrame(MaxFrame, 0);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//SpriteFrameRow
void CEffectSprite::OnEnChangeRowframe()
{
	UpdateData(TRUE);

	_int  RowFrame;

	RowFrame = GetDlgItemInt(IDC_ROWFRAME);


	if(RowFrame < 1)
	{
		AfxMessageBox(L"�����ʰ�");
		return ;
	}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSpriteFrame(RowFrame, 1);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//SpriteFrameColumn
void CEffectSprite::OnEnChangeColumnframe()
{
	UpdateData(TRUE);

	_int  ColumnFrame;

	ColumnFrame = GetDlgItemInt(IDC_COLUMNFRAME);

	if(ColumnFrame < 1)
	{
		AfxMessageBox(L"�����ʰ�");
		return ;
	}

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetSpriteFrame(ColumnFrame, 2);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	//	AfxMessageBox(L"�����ʰ�");
	//	return ;
	//}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetColor(ColorR, 0);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	//	AfxMessageBox(L"�����ʰ�");
	//	return ;
	//}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetColor(ColorG, 1);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	//	AfxMessageBox(L"�����ʰ�");
	//	return ;
	//}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetColor(ColorB, 2);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	//	AfxMessageBox(L"�����ʰ�");
	//	return ;
	//}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->SetColor(ColorA, 3);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//PathBtn
void CEffectSprite::OnBnClickedSpritepath()
{

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


std::wstring CEffectSprite::ConvertRelativePath(wstring strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szDirectoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);

	//�� ��ο��� �ٸ� ��η� ���� ��� ��θ� �����ϴ� ����...
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
		AfxMessageBox(L"��� ������ ���� �ؾ� �˴ϴ�.!");
		return;
	}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;


	//���̺�...
	DWORD	dwByte = 0;
	HANDLE	hFile = CreateFile(strPath, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		AfxMessageBox(L"���� ���� ����");
		return;
	}

	WriteFile(hFile, ((CSprite*)pGameObject)->GetParticleInfo(), sizeof(Engine::PARTICLEINFO), &dwByte, NULL);


	CloseHandle(hFile);
	Engine::Safe_Release(pGameObject);


	AfxMessageBox(L"Sprite Save OK!!");


	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



void CEffectSprite::OnBnClickedCheck1()
{


	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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



	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//�ִϸ��̼� ��ŸƮ
void CEffectSprite::OnBnClickedButton1()
{

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolSprite");

	if(pGameObject == NULL)
		return ;

	((CSprite*)pGameObject)->ResetAnimation();

	Engine::Safe_Release(pGameObject);


	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



//�н�����
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
