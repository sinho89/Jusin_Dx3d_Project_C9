// ParticleDIg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "ParticleDIg.h"
#include "MainFrm.h"
#include "Particle.h"

// CParticleDIg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CParticleDIg, CDialog)

CParticleDIg::CParticleDIg(CWnd* pParent /*=NULL*/)
	: CDialog(CParticleDIg::IDD, pParent)
	, m_ParticleMove(0)
	, m_iParticlePathNumber(0)
{
	m_pView = NULL;
	m_iDrawID = 0;
	m_iParticleCount = 0;

	m_vCenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vSize= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

CParticleDIg::~CParticleDIg()
{
}

void CParticleDIg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTURECOMBO, m_TextureComboBox);
	DDX_Control(pDX, IDC_CHECK1, m_bButton);
	DDX_Radio(pDX, IDC_NORMAL, m_ParticleMove);
	DDX_Radio(pDX, IDC_NORMALPATH, m_iParticlePathNumber);
}


BEGIN_MESSAGE_MAP(CParticleDIg, CDialog)
	ON_CBN_SELCHANGE(IDC_TEXTURECOMBO, &CParticleDIg::OnCbnSelchangeTexturecombo)
	ON_EN_CHANGE(IDC_PARTICLECOUNT, &CParticleDIg::OnEnChangeParticlecount)
	ON_EN_CHANGE(IDC_CENTERX, &CParticleDIg::OnEnChangeCenterx)
	ON_EN_CHANGE(IDC_CENTERY, &CParticleDIg::OnEnChangeCentery)
	ON_EN_CHANGE(IDC_EDIT5, &CParticleDIg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_PARTICLESIZEX, &CParticleDIg::OnEnChangeParticlesizex)
	ON_EN_CHANGE(IDC_PARTICLESIZEY, &CParticleDIg::OnEnChangeParticlesizey)
	ON_EN_CHANGE(IDC_PARTICLESIZEZ, &CParticleDIg::OnEnChangeParticlesizez)
	ON_EN_CHANGE(IDC_PARTICLE_R, &CParticleDIg::OnEnChangeParticleR)
	ON_EN_CHANGE(IDC_PARTICLE_G, &CParticleDIg::OnEnChangeParticleG)
	ON_EN_CHANGE(IDC_PARTICLE_B, &CParticleDIg::OnEnChangeParticleB)
	ON_EN_CHANGE(IDC_PARTICLE_A, &CParticleDIg::OnEnChangeParticleA)
	ON_EN_CHANGE(IDC_WINDX, &CParticleDIg::OnEnChangeWindx)
	ON_EN_CHANGE(IDC_WINDY, &CParticleDIg::OnEnChangeWindy)
	ON_EN_CHANGE(IDC_WINDZ, &CParticleDIg::OnEnChangeWindz)
	ON_EN_CHANGE(IDC_WINDPOWER, &CParticleDIg::OnEnChangeWindpower)
	ON_EN_CHANGE(IDC_PARTICLESPEED, &CParticleDIg::OnEnChangeParticlespeed)
	ON_BN_CLICKED(IDC_CHECK1, &CParticleDIg::OnBnClickedCheck1)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_NORMAL, IDC_CYCLONE, &CParticleDIg::SetParticleMovement)
	ON_EN_CHANGE(IDC_ARRANGEX, &CParticleDIg::OnEnChangeArrangex)
	ON_EN_CHANGE(IDC_ARRANGEY, &CParticleDIg::OnEnChangeArrangey)
	ON_EN_CHANGE(IDC_ARRANGEZ, &CParticleDIg::OnEnChangeArrangez)
	ON_EN_CHANGE(IDC_ARRANGEFIRE, &CParticleDIg::OnEnChangeArrangefire)
	ON_EN_CHANGE(IDC_ParticleFade, &CParticleDIg::OnBnClickedChangingFade)
	ON_EN_CHANGE(IDC_ParticleFHeight, &CParticleDIg::OnEnChangeParticlefheight)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_NORMALPATH, IDC_PARTICLEPATHCOLOR, &CParticleDIg::SetPathNumber)
	ON_EN_CHANGE(IDC_RADIUSINTERVAL, &CParticleDIg::OnEnChangeRadiusinterval)
	ON_EN_CHANGE(IDC_ANGLEINTERVAL, &CParticleDIg::OnEnChangeAngleinterval)
	ON_EN_CHANGE(IDC_HEIGHTINTERVAL, &CParticleDIg::OnEnChangeHeightinterval)
	ON_BN_CLICKED(IDC_ParticlePathBtn, &CParticleDIg::OnBnClickedParticlepathbtn)
	ON_BN_CLICKED(IDC_ParticleSaveBtn, &CParticleDIg::OnBnClickedParticlesavebtn)
	ON_BN_CLICKED(IDC_LoadParticleBtn2, &CParticleDIg::OnBnClickedLoadparticlebtn2)
END_MESSAGE_MAP()


// CParticleDIg �޽��� ó�����Դϴ�.

BOOL CParticleDIg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitParticleDlg();
	InitParticleTexture();

	InitSettingNumber();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CParticleDIg::InitParticleDlg(void)
{
	m_pView = ((CMainFrame*)AfxGetMainWnd())->m_pMainView;

}

//��ƼŬ �ؽ����̸� ���
void CParticleDIg::OnCbnSelchangeTexturecombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	UpdateData(TRUE);


	m_iDrawID = m_TextureComboBox.GetCurSel();


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetParticleTexuture(m_iDrawID);

	Engine::Safe_Release(pGameObject);
}


//�ʱ⿡ �ؽ��� Init
void CParticleDIg::InitParticleTexture(void)
{
	if(m_TextureComboBox.GetCount() == 0)
	{
		CFileFind finder;

		TCHAR* tPath = L"../../../Client/Bin/Resource/Effect/Particle/*.*";

		BOOL bFind  = finder.FindFile(tPath);

		int		iNumber = 0;

		CString Name = L"Particle";

		CString Number = L"";

		CString Make = L".png";


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


//Particle ����
void CParticleDIg::OnEnChangeParticlecount()
{	
	m_iParticleCount = GetDlgItemInt(IDC_PARTICLECOUNT);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetParticleCount(m_iParticleCount);


	Engine::Safe_Release(pGameObject);



	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



//Center X�� ��
void CParticleDIg::OnEnChangeCenterx()
{
	UpdateData(TRUE);

	CString m_strX;


	GetDlgItemText(IDC_CENTERX, m_strX);

	m_vCenterPos.x = (_float)_tstof(m_strX);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->SetStandardPos(m_vCenterPos.x, 0);

	Engine::Safe_Release(pGameObject);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Center Y�� ��
void CParticleDIg::OnEnChangeCentery()
{
	UpdateData(TRUE);

	CString m_strY;

	GetDlgItemText(IDC_CENTERY, m_strY);

	m_vCenterPos.y = (_float)_tstof(m_strY);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->SetStandardPos(m_vCenterPos.y, 1);

	Engine::Safe_Release(pGameObject);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Center Z�� ��
void CParticleDIg::OnEnChangeEdit5()
{
	UpdateData(TRUE);

	CString m_strZ;

	GetDlgItemText(IDC_EDIT5, m_strZ);

	m_vCenterPos.z = (_float)_tstof(m_strZ);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetStandardPos(m_vCenterPos.z, 2);

	Engine::Safe_Release(pGameObject);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//��ƼŬ x ������
void CParticleDIg::OnEnChangeParticlesizex()
{
	UpdateData(TRUE);


	CString m_strX;

	GetDlgItemText(IDC_PARTICLESIZEX, m_strX);

	m_vSize.x = (_float)_tstof(m_strX);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetSize(m_vSize.x, 0);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//��ƼŬ y ������
void CParticleDIg::OnEnChangeParticlesizey()
{
	UpdateData(TRUE);


	CString m_strY;

	GetDlgItemText(IDC_PARTICLESIZEY, m_strY);

	m_vSize.y = (_float)_tstof(m_strY);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetSize(m_vSize.y, 1);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//��ƼŬ z ������
void CParticleDIg::OnEnChangeParticlesizez()
{
	UpdateData(TRUE);

	CString m_strZ;

	GetDlgItemText(IDC_PARTICLESIZEZ, m_strZ);

	m_vSize.z = (_float)_tstof(m_strZ);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetSize(m_vSize.z, 2);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Color R
void CParticleDIg::OnEnChangeParticleR()
{
	UpdateData(TRUE);

	CString m_strColor;

	GetDlgItemText(IDC_PARTICLE_R, m_strColor);

	_float	ColorR = 0.0f; 

	ColorR = (_float)_tstof(m_strColor);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetColor(ColorR, 0);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



//Color G
void CParticleDIg::OnEnChangeParticleG()
{
	UpdateData(TRUE);

	CString m_strColor;

	GetDlgItemText(IDC_PARTICLE_G, m_strColor);

	_float	ColorG = 0.0f; 

	ColorG = (_float)_tstof(m_strColor);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetColor(ColorG, 1);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Color B
void CParticleDIg::OnEnChangeParticleB()
{
	UpdateData(TRUE);

	CString m_strColor;

	GetDlgItemText(IDC_PARTICLE_B, m_strColor);

	_float	ColorB = 0.0f; 

	ColorB = (_float)_tstof(m_strColor);



	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetColor(ColorB, 2);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//Color A
void CParticleDIg::OnEnChangeParticleA()
{
	UpdateData(TRUE);

	CString m_strColor;

	GetDlgItemText(IDC_PARTICLE_A, m_strColor);

	_float	ColorA = 0.0f; 

	ColorA = (_float)_tstof(m_strColor);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetColor(ColorA, 3);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



//Wind X
void CParticleDIg::OnEnChangeWindx()
{
	UpdateData(TRUE);

	CString m_strX;
	_float   m_fWindX;

	GetDlgItemText(IDC_WINDX, m_strX);

	m_fWindX = (_float)_tstof(m_strX);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetWindControl(m_fWindX, 0);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//Wind Y
void CParticleDIg::OnEnChangeWindy()
{
	UpdateData(TRUE);

	CString m_strY;
	_float   m_fWindY;

	GetDlgItemText(IDC_WINDY, m_strY);

	m_fWindY = (_float)_tstof(m_strY);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetWindControl(m_fWindY, 1);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//Wind Z
void CParticleDIg::OnEnChangeWindz()
{
	UpdateData(TRUE);

	CString m_strZ;
	_float   m_fWindZ;

	GetDlgItemText(IDC_WINDZ, m_strZ);

	m_fWindZ = (_float)_tstof(m_strZ);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetWindControl(m_fWindZ, 2);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Wind Power
void CParticleDIg::OnEnChangeWindpower()
{
	UpdateData(TRUE);

	CString m_strPower;
	_float   m_fWindPower;

	GetDlgItemText(IDC_WINDPOWER, m_strPower);

	m_fWindPower = (_float)_tstof(m_strPower);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetWindControl(m_fWindPower, 3);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//��ƼŬ ���ǵ�
void CParticleDIg::OnEnChangeParticlespeed()
{
	UpdateData(TRUE);

	CString m_strSpeed;
	_float   m_fWindSpeed;

	GetDlgItemText(IDC_PARTICLESPEED, m_strSpeed);

	m_fWindSpeed = (_float)_tstof(m_strSpeed);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->SetAvgTime(m_fWindSpeed);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CParticleDIg::InitSettingNumber(void)
{
	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	Engine::PARTICLEINFO*  ParticleInfo =((CParticle*)pGameObject)->GetParticleInfo();

	CString CenterX, CenterY, CenterZ, ParticleSpeed, 
			ParticleSizeX, ParticleSizeY, ParticleSizeZ, 
			Color_R, Color_G, Color_B, Color_A,
			Wind_X, Wind_Y, Wind_Z, Wind_Power,
			ArrangeX, ArrangeY, ArrangeZ, FireArrange,
			RadiusInterval, AngleInterval, HeightInterval;

	CenterX.Format(_T("%.1f"), ParticleInfo->m_vStandard.x);
	CenterY.Format(_T("%.1f"), ParticleInfo->m_vStandard.y);
	CenterZ.Format(_T("%.1f"), ParticleInfo->m_vStandard.z);
	ParticleSpeed.Format(_T("%.1f"), ParticleInfo->m_fTimeAvg);
	ParticleSizeX.Format(_T("%.1f"), ParticleInfo->m_vScale.x);
	ParticleSizeY.Format(_T("%.1f"), ParticleInfo->m_vScale.y);
	ParticleSizeZ.Format(_T("%.1f"), ParticleInfo->m_vScale.z);
	Color_R.Format(_T("%.1f"), ParticleInfo->m_dColor.r);
	Color_G.Format(_T("%.1f"), ParticleInfo->m_dColor.g);
	Color_B.Format(_T("%.1f"), ParticleInfo->m_dColor.b);
	Color_A.Format(_T("%.1f"), ParticleInfo->m_dColor.a);
	Wind_X.Format(_T("%.1f"), ParticleInfo->m_vWindDir.x);
	Wind_Y.Format(_T("%.1f"), ParticleInfo->m_vWindDir.y);
	Wind_Z.Format(_T("%.1f"), ParticleInfo->m_vWindDir.z);
	Wind_Power.Format(_T("%.1f"), ParticleInfo->m_fWindPower);

	ArrangeX.Format(_T("%.0f"), ParticleInfo->m_vParticleArrange.x);
	ArrangeY.Format(_T("%.0f"), ParticleInfo->m_vParticleArrange.y);
	ArrangeZ.Format(_T("%.0f"), ParticleInfo->m_vParticleArrange.z);
	FireArrange.Format(_T("%.1f"), ParticleInfo->m_fFireArrange);

	RadiusInterval.Format(_T("%.1f"), ParticleInfo->m_fCycloneRadiusInterval);
	AngleInterval.Format(_T("%.1f"), ParticleInfo->m_fCycloneAngleInterval);
	HeightInterval.Format(_T("%.1f"), ParticleInfo->m_fCycloneHeightInterval);



	
	SetDlgItemInt(IDC_PARTICLECOUNT, ParticleInfo->m_PrtN); //��ƼŬ ����
	SetDlgItemText(IDC_CENTERX, CenterX); //���ؼ��������� X
	SetDlgItemText(IDC_CENTERY, CenterY); //���ؼ��������� Y
	SetDlgItemText(IDC_EDIT5, CenterZ); //���ؼ��������� Z
	SetDlgItemText(IDC_PARTICLESPEED, ParticleSpeed); //Speed
	SetDlgItemText(IDC_PARTICLESIZEX, ParticleSizeX); //SizeX
	SetDlgItemText(IDC_PARTICLESIZEY, ParticleSizeY); //SizeY
	SetDlgItemText(IDC_PARTICLESIZEZ, ParticleSizeZ); //SizeZ
	SetDlgItemText(IDC_PARTICLE_R, Color_R);  //ColorR
	SetDlgItemText(IDC_PARTICLE_G, Color_G);  //ColorG
	SetDlgItemText(IDC_PARTICLE_B, Color_B);  //ColorB
	SetDlgItemText(IDC_PARTICLE_A, Color_A);  //ColorA
	SetDlgItemText(IDC_WINDX, Wind_X);  //Wind X
	SetDlgItemText(IDC_WINDY, Wind_Y);  //Wind Y
	SetDlgItemText(IDC_WINDZ, Wind_Z);  //Wind Z
	SetDlgItemText(IDC_WINDPOWER, Wind_Power); //Wind Power
	SetDlgItemText(IDC_ARRANGEX, ArrangeX); //ParticleArrange
	SetDlgItemText(IDC_ARRANGEY, ArrangeY); //ParticleArrange
	SetDlgItemText(IDC_ARRANGEZ, ArrangeZ); //ParticleArrange
	SetDlgItemText(IDC_ARRANGEFIRE, FireArrange); //ParticleArrange
	SetDlgItemInt(IDC_ParticleFade, ParticleInfo->m_iFade);
	SetDlgItemInt(IDC_ParticleFHeight, ParticleInfo->m_iFHeight);

	//Cyclone
	SetDlgItemText(IDC_RADIUSINTERVAL, RadiusInterval);
	SetDlgItemText(IDC_ANGLEINTERVAL,  AngleInterval);
	SetDlgItemText(IDC_HEIGHTINTERVAL, HeightInterval);



	//�ؽ��� ��ȣ
	CString	strSelectName;

	m_TextureComboBox.SetCurSel(ParticleInfo->m_iTextureNumber);

	int iSelNum = m_TextureComboBox.GetCurSel();

	m_TextureComboBox.GetLBText(iSelNum, strSelectName);


	//������ üũ
	if(ParticleInfo->m_bBillBoard)
		m_bButton.SetCheck(1); //1 : üũ�ڽ� ����
	else
	{
		m_bButton.SetCheck(0);
	}


	//��ƼŬ �Ӽ�
	m_ParticleMove = (int)ParticleInfo->m_eMoveMent;



	Engine::Safe_Release(pGameObject);

	UpdateData(FALSE);
}


//������
void CParticleDIg::OnBnClickedCheck1()
{
	UpdateData(TRUE);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	
	if(m_bButton.GetCheck())
	{
		((CParticle*)pGameObject)->SetBillBoardCheck(true);
	}
	else
	{
		((CParticle*)pGameObject)->SetBillBoardCheck(false);
	}


	Engine::Safe_Release(pGameObject);

	UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//��ƼŬ ������
void CParticleDIg::SetParticleMovement(UINT value)
{
	UpdateData(TRUE);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;



	switch(m_ParticleMove)
	{
	case 0: //Normal
		((CParticle*)pGameObject)->ChangingMoveMent(Engine::MOVE_NORMAL);
		break;

	case 1: //fire
		((CParticle*)pGameObject)->ChangingMoveMent(Engine::MOVE_FIRE);
		break;

	case 2: //Cyclone
		((CParticle*)pGameObject)->ChangingMoveMent(Engine::MOVE_CYCLONE);
		break;
	}

	Engine::Safe_Release(pGameObject);
	UpdateData(FALSE);
}


//��ƼŬ ���� x
void CParticleDIg::OnEnChangeArrangex()
{
	UpdateData(TRUE);


	CString m_strArrangeX;
	_float   m_fArrangeX = 0.0f;

	GetDlgItemText(IDC_ARRANGEX, m_strArrangeX);

	m_fArrangeX = (_float)_tstof(m_strArrangeX);
	

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;

	((CParticle*)pGameObject)->ChangingArrange(m_fArrangeX, 0);


	Engine::Safe_Release(pGameObject);
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//��ƼŬ ���� y
void CParticleDIg::OnEnChangeArrangey()
{
	UpdateData(TRUE);


	CString m_strArrangeY;
	_float   m_fArrangeY;

	GetDlgItemText(IDC_ARRANGEY, m_strArrangeY);

	m_fArrangeY = (_float)_tstof(m_strArrangeY);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->ChangingArrange(m_fArrangeY, 1);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//��ƼŬ ���� z
void CParticleDIg::OnEnChangeArrangez()
{
	UpdateData(TRUE);


	CString m_strArrangeZ;
	_float   m_fArrangeZ;

	GetDlgItemText(IDC_ARRANGEZ, m_strArrangeZ);

	m_fArrangeZ = (_float)_tstof(m_strArrangeZ);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->ChangingArrange(m_fArrangeZ, 2);

	Engine::Safe_Release(pGameObject);
	
	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//��ƼŬ ���� arrange
void CParticleDIg::OnEnChangeArrangefire()
{
	UpdateData(TRUE);


	CString m_strArrange;
	_float   m_fArrange;

	GetDlgItemText(IDC_ARRANGEFIRE, m_strArrange);

	m_fArrange = (_float)_tstof(m_strArrange);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->ChangingArrange(m_fArrange, 3);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//��ƼŬ ���̵�
void CParticleDIg::OnBnClickedChangingFade()
{
	UpdateData(TRUE);

	int	m_iFade;

	m_iFade = GetDlgItemInt(IDC_ParticleFade);


	if(m_iFade < 1 )
	{
		AfxMessageBox(L"�������ʰ�");
		return ;
	}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->ChangingFadeArrange(m_iFade);

	Engine::Safe_Release(pGameObject);
}

void CParticleDIg::OnEnChangeParticlefheight()
{

	UpdateData(TRUE);

	int	m_iHeight;

	m_iHeight = GetDlgItemInt(IDC_ParticleFHeight);


	if(m_iHeight < 1 )
	{
		AfxMessageBox(L"�������ʰ�");
		return ;
	}


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->ChangingFHeight(m_iHeight);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//�н� ����
void CParticleDIg::SetPathNumber(UINT value)
{
	UpdateData(TRUE);

	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	switch(m_iParticlePathNumber)
	{
	case 0: //Normal
		((CParticle*)pGameObject)->SetPath(0);
		break;

	case 1: //Black
		((CParticle*)pGameObject)->SetPath(1);
		break;

	case 2: //colors
		((CParticle*)pGameObject)->SetPath(2);
		break;
	}

	Engine::Safe_Release(pGameObject);
	UpdateData(FALSE);
}




std::wstring CParticleDIg::ConvertRelativePath(wstring strFullPath)
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

//Radius
void CParticleDIg::OnEnChangeRadiusinterval()
{
	UpdateData(TRUE);


	CString m_strRadiusInterval;
	_float   RadiusInterval;

	GetDlgItemText(IDC_RADIUSINTERVAL, m_strRadiusInterval);

	RadiusInterval = (_float)_tstof(m_strRadiusInterval);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->SetCycloneInterval(RadiusInterval, 0);

	Engine::Safe_Release(pGameObject);

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Angle
void CParticleDIg::OnEnChangeAngleinterval()
{

	UpdateData(TRUE);


	CString m_strAngleInterval;
	_float   AngleInterval;

	GetDlgItemText(IDC_ANGLEINTERVAL, m_strAngleInterval);

	AngleInterval = (_float)_tstof(m_strAngleInterval);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->SetCycloneInterval(AngleInterval, 1);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Height
void CParticleDIg::OnEnChangeHeightinterval()
{

	UpdateData(TRUE);


	CString m_strHeightInterval;
	_float  HeightInterval;

	GetDlgItemText(IDC_HEIGHTINTERVAL, m_strHeightInterval);

	HeightInterval = (_float)_tstof(m_strHeightInterval);


	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if(pGameObject == NULL)
		return ;


	((CParticle*)pGameObject)->SetCycloneInterval(HeightInterval, 2);

	Engine::Safe_Release(pGameObject);


	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//Path
void CParticleDIg::OnBnClickedParticlepathbtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog	FileDlg(FALSE, L"dat", L"Particle.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"*.dat", this);

	TCHAR	szCurDir[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH - 1, szCurDir);


	PathRemoveFileSpec(szCurDir);
	PathRemoveFileSpec(szCurDir);

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;

	CString strPathName;

	if (FileDlg.DoModal() == IDOK)
	{
		strPathName = FileDlg.GetPathName();
	}

	CString strRelativePath = ConvertRelativePath((LPCWSTR)strPathName).c_str();

	SetDlgItemText(IDC_PATH, strRelativePath);

	UpdateData(TRUE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//Save
void CParticleDIg::OnBnClickedParticlesavebtn()
{
	CString strPath;
	GetDlgItemText(IDC_PATH, strPath);

	if (strPath == L"")
	{
		AfxMessageBox(L"��� ������ ���� �ؾ� �˴ϴ�.!");
		return;
	}



	Engine::CGameObject* pGameObject = NULL;

	pGameObject = Engine::Find(L"ToolParticle");

	if (pGameObject == NULL)
		return;


	//���̺�...
	DWORD	dwByte = 0;
	HANDLE	hFile = CreateFile(strPath, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL)
	{
		AfxMessageBox(L"���� ���� ����");
		return;
	}

	WriteFile(hFile, ((CParticle*)pGameObject)->GetParticleInfo(), sizeof(Engine::PARTICLEINFO), &dwByte, NULL);


	CloseHandle(hFile);
	Engine::Safe_Release(pGameObject);


	AfxMessageBox(L"Particle Save OK!!");


	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

//Load
void CParticleDIg::OnBnClickedLoadparticlebtn2()
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

	if (FileDlg.DoModal() == IDOK)
	{

		Engine::CGameObject* pGameObject = NULL;

		pGameObject = Engine::Find(L"ToolParticle");

		if (pGameObject == NULL)
			return;

		DWORD	dwByte = 0;

		strPathName = FileDlg.GetPathName();

		HANDLE hFile = CreateFile(strPathName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


		while (true)
		{
			Engine::PARTICLEINFO Particle;

			ReadFile(hFile, &Particle, sizeof(Engine::PARTICLEINFO), &dwByte, NULL);

			if (dwByte == 0)
			{
				Engine::Safe_Release(pGameObject);
				break;
			}

			((CParticle*)pGameObject)->LoadParticleInfo(&Particle);
			InitSettingNumber();
		}
		CloseHandle(hFile);
	}

	AfxMessageBox(L"Particle Load OK!!");
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
