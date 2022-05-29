// EffectTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "EffectTool.h"
#include "DynamicObject.h"
#include "Export_Engine.h"
#include "Sprite.h"
#include "PickingRect.h"


// EffectTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEffectTool, CDialog)

CEffectTool::CEffectTool(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectTool::IDD, pParent)
{
	m_iDrawID = 0;
	EffectName = _T("");
	m_bRenderCheck = true;
}

CEffectTool::~CEffectTool()
{
}

void CEffectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECTBONELIST, m_BoneList);
	DDX_Control(pDX, IDC_LIST1, m_EffectList);
	DDX_Control(pDX, IDC_CHECK1, m_bBillBoardBtn);
}


BEGIN_MESSAGE_MAP(CEffectTool, CDialog)
	ON_LBN_SELCHANGE(IDC_EFFECTBONELIST, &CEffectTool::OnLbnSelchangeEffectbonelist)
	ON_BN_CLICKED(IDC_EFFECTCREATEBTN, &CEffectTool::OnBnClickedEffectcreatebtn)
	ON_BN_CLICKED(IDC_EFFECTSETBTN, &CEffectTool::OnBnClickedEffectsetbtn)
	ON_BN_CLICKED(IDC_DELETEBTN, &CEffectTool::OnBnClickedDeletebtn)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEffectTool::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_RECTRENDER, &CEffectTool::OnBnClickedRectrender)
	ON_BN_CLICKED(IDC_CHECK1, &CEffectTool::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_ANIMATIONSTART, &CEffectTool::OnBnClickedAnimationstart)
END_MESSAGE_MAP()


// EffectTool �޽��� ó�����Դϴ�.

BOOL CEffectTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	//=======Effect Info Setting===============//
	SetDlgItemText(IDC_EPOSX, L"0");
	SetDlgItemText(IDC_EPOSY, L"0");
	SetDlgItemText(IDC_EPOSZ, L"0");

	SetDlgItemText(IDC_ESCALEX, L"1");
	SetDlgItemText(IDC_ESCALEY, L"1");
	SetDlgItemText(IDC_ESCALEZ, L"1");

	SetDlgItemText(IDC_EROTX, L"0");
	SetDlgItemText(IDC_EROTY, L"0");
	SetDlgItemText(IDC_EROTZ, L"0");


	InitEffectStruct();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


//Player�� ����ġ�� �޾ƿ´�.

void CEffectTool::OnLbnSelchangeEffectbonelist()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_pvecBone == NULL)
		return ;

	char szBoneName[128];

	ZeroMemory(&szBoneName, sizeof(char) * 128);


	_int iIndex = m_BoneList.GetCurSel();


	strcpy_s(szBoneName, sizeof(char) * 128, (*m_pvecBone)[iIndex]);

	
	//========================Player===============================//
	Engine::CGameObject*	pGameObject = NULL;


	Engine::CGameObject*	pGameEffect = NULL;

	pGameObject = Engine::FindWithTag(L"DynamicObject");


	list<Engine::CGameObject*>* pList = Engine::FindList(L"Effect");

	if(pList == NULL)
		return ;


	Engine::CGameObject*  pEffect = NULL;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for(iter; iter != iter_end; ++iter)
	{

		if( ((CSprite*)(*iter))->IsSelected() == true )  
		{
			pGameEffect = (*iter);
			break;
		}
	}


	if(pGameEffect == NULL || pGameObject == NULL)
		return ;


	((CSprite*)pGameEffect)->SetBone(pGameObject, (*m_pvecBone)[iIndex]);


	Engine::Safe_Release(pGameObject);
}


//Effect Create ����
void CEffectTool::OnBnClickedEffectcreatebtn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LPDIRECT3DDEVICE9 pGraphicDev = NULL;
	Engine::Get_GraphicDev(&pGraphicDev);


	Engine::CGameObject* pGameObject = NULL;


	if(EffectName == _T(""))
		return ;


	pGameObject = CSprite::LoadCreate(pGraphicDev, (wstring)EffectName);

	((CSprite*)pGameObject)->m_pPickingRect->m_bRenderCheck = m_bRenderCheck;

	Engine::AddObject(Engine::LAYER_GAMELOGIC, L"Effect", pGameObject);

	Engine::Safe_Release(pGraphicDev);
}


//Transform ���� ��ȭ
void CEffectTool::OnBnClickedEffectsetbtn()
{
	_vec3 vPos;
	_vec3 vScale;
	_vec3 vRot;

	CString strData;



	list<Engine::CGameObject*>* pList = Engine::FindList(L"Effect");

	if(pList == NULL)
		return ;


	Engine::CGameObject*  pEffect = NULL;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for(iter; iter != iter_end; ++iter)
	{
		if( ((CSprite*)(*iter))->IsSelected() == true )  
		{
			pEffect = (*iter);
			break;
		}
	}

	if(pEffect == NULL)
		return ;




	GetDlgItemText(IDC_EPOSX, strData);
	vPos.x = (float)_tstof(strData.GetString());

	GetDlgItemText(IDC_EPOSY, strData);
	vPos.y = (float)_tstof(strData.GetString());

	GetDlgItemText(IDC_EPOSZ, strData);
	vPos.z = (float)_tstof(strData.GetString());

	GetDlgItemText(IDC_ESCALEX, strData);
	vScale.x = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_ESCALEY, strData);
	vScale.y = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_ESCALEZ, strData);
	vScale.z = (float)_tstof(strData.GetString());


	GetDlgItemText(IDC_EROTX, strData);
	vRot.x = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_EROTY, strData);
	vRot.y = (float)_tstof(strData.GetString());
	GetDlgItemText(IDC_EROTZ, strData);
	vRot.z = (float)_tstof(strData.GetString());



	((Engine::CEffect*)(pEffect))->GetParticleInfo()->m_vParentTranform = vPos;

	((Engine::CEffect*)(pEffect))->GetParticleInfo()->m_vParentRotate = vRot;


	((Engine::CEffect*)(pEffect))->GetParticleInfo()->m_vParentScale = vScale;



	((Engine::CEffect*)(pEffect))->SetTranscomInfo();


	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CEffectTool::InitSetting(void)
{
	list<Engine::CGameObject*>* pList = Engine::FindList(L"Effect");

	if(pList == NULL)
		return ;


	Engine::CGameObject*  pEffect = NULL;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for(iter; iter != iter_end; ++iter)
	{
		if( ((CSprite*)(*iter))->IsSelected() == true )  
		{
			pEffect = (*iter);
			break;
		}
	}

	if(pEffect == NULL)
		return ;



	Engine::CTransform* pTransform = (Engine::CTransform*)pEffect->GetComponent<Engine::CTransform>(Engine::COMID_DYNAMIC);


	CString PosX, PosY, PosZ, ScaleX, ScaleY, ScaleZ, RotX, RotY, RotZ;

	_vec3 Position,  Scale, Rot;
	
	Position = pTransform->Get_Now_Position();
	pTransform->Get_Scale(&Scale);
	pTransform->Get_Angle(&Rot);


	PosX.Format(_T("%.1f"), Position.x);
	PosY.Format(_T("%.1f"), Position.y);
	PosZ.Format(_T("%.1f"), Position.z);

	ScaleX.Format(_T("%.1f"), Scale.x);
	ScaleY.Format(_T("%.1f"), Scale.y);
	ScaleZ.Format(_T("%.1f"), Scale.z);

	RotX.Format(_T("%.1f"), Rot.x);
	RotY.Format(_T("%.1f"), Rot.y);
	RotZ.Format(_T("%.1f"), Rot.z);


	SetDlgItemText(IDC_EPOSX, PosX);
	SetDlgItemText(IDC_EPOSY, PosY);
	SetDlgItemText(IDC_EPOSZ, PosZ);

	SetDlgItemText(IDC_ESCALEX, ScaleX);
	SetDlgItemText(IDC_ESCALEY, ScaleY);
	SetDlgItemText(IDC_ESCALEZ, ScaleZ);

	SetDlgItemText(IDC_EROTX, RotX);
	SetDlgItemText(IDC_EROTY, RotY);
	SetDlgItemText(IDC_EROTZ, RotZ);


	//������ üũ
	if( ((Engine::CEffect*)(pEffect))->GetParticleInfo()->m_bBillBoard )
		m_bBillBoardBtn.SetCheck(1); //1 : üũ�ڽ� ����
	else
	{
		m_bBillBoardBtn.SetCheck(0);
	}



	Engine::Safe_Release(pTransform);
}


void CEffectTool::InitEffectStruct(void)
{
		if(m_EffectList.GetCount() == 0)
	{
		CFileFind finder;

		TCHAR* tPath = L"../../../Client/Bin/Data/EffectData/*.*";

		BOOL bFind  = finder.FindFile(tPath);

		int		iNumber = 0;

		while(bFind)
		{
			bFind = finder.FindNextFile();

			if(finder.GetFileName() == "."
				|| finder.GetFileName() == "..")
				continue;

			m_EffectList.AddString(finder.GetFileName());

		}

	}

	UpdateData(FALSE);
}

void CEffectTool::MouseClick(POINT ptMouse)
{
	map<float, Engine::CGameObject*>		m_mapEffect;


	list<Engine::CGameObject*>* pList = Engine::FindList(L"Effect");

	if(pList == NULL)
		return ;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for(iter; iter != iter_end; ++iter)
	{
		((CSprite*)(*iter))->SetSelected(false);
	}

	iter = pList->begin();

	for(iter; iter != iter_end; ++iter)
	{
		if( ((CSprite*)(*iter))->GetMouseCol(ptMouse))
		{

			float dist = ((CSprite*)(*iter))->m_pPickingRect->m_fDist;

			m_mapEffect.insert(map<float, Engine::CGameObject*>::value_type(dist, (*iter)) );

			((CSprite*)(*iter))->m_pPickingRect->InitDist();
		}
	}

	if(m_mapEffect.size() == 0)
	{
		return ;
	}
	else
	{
		//����Ʈ üũ
		((CSprite*)(m_mapEffect.begin()->second))->SetSelected(true);

		InitSetting();

		return ;
	}

}

//���õ� ����Ʈ ����
void CEffectTool::OnBnClickedDeletebtn()
{

	list<Engine::CGameObject*>* pList = Engine::FindList(L"Effect");

	if(pList == NULL)
		return ;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for(iter; iter != iter_end; ++iter)
	{
		if( ((CSprite*)(*iter))->IsSelected() == true)  
		{
			(*iter)->SetState(Engine::STATE_DESTROY);
			break;
		}
	}



	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Effect ����ü �ҷ�����
void CEffectTool::OnLbnSelchangeList1()
{
	UpdateData(TRUE);

	m_iDrawID = m_EffectList.GetCurSel();


	TCHAR szTmp[128];

	ZeroMemory(&szTmp, sizeof(TCHAR) * 128);


	m_EffectList.GetText(m_iDrawID, EffectName);


	StrCpy(szTmp, EffectName);


	PathRemoveExtension(szTmp); //Ȯ���� ����

	EffectName = szTmp;




	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

}


//��ŷ��Ʈ ���� ����
void CEffectTool::OnBnClickedRectrender()
{
	list<Engine::CGameObject*>* pList = Engine::FindList(L"Effect");

	if(pList == NULL)
		return ;

	if(m_bRenderCheck)
	{
		m_bRenderCheck = false;
	}
	else
	{
		m_bRenderCheck = true;
	}


	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for(iter; iter != iter_end; ++iter)
	{
		((CSprite*)(*iter))->m_pPickingRect->m_bRenderCheck = m_bRenderCheck;

	}



	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//������
void CEffectTool::OnBnClickedCheck1()
{
	UpdateData(TRUE);


	list<Engine::CGameObject*>* pList = Engine::FindList(L"Effect");

	if(pList == NULL)
		return ;


	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();

	for(iter; iter != iter_end; ++iter)
	{
		if( ((CSprite*)(*iter))->IsSelected() == true)  
		{
			if(m_bBillBoardBtn.GetCheck())
			{
				((Engine::CEffect*)(*iter))->GetParticleInfo()->m_bBillBoard = true;
			}
			else
			{
				((Engine::CEffect*)(*iter))->GetParticleInfo()->m_bBillBoard = false;
			}
				break;
		}
	}


	UpdateData(FALSE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


//Start
void CEffectTool::OnBnClickedAnimationstart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	list<Engine::CGameObject*>* pList = Engine::FindList(L"Effect");

	if(pList == NULL)
		return ;

	list<Engine::CGameObject*>::iterator iter = pList->begin();
	list<Engine::CGameObject*>::iterator iter_end = pList->end();



	for(iter; iter != iter_end; ++iter)
	{
		((CSprite*)(*iter))->ResetAnimation();
	}


	UpdateData(FALSE);

}
