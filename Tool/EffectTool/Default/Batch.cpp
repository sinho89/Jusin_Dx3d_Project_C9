// Batch.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Batch.h"


// CBatch ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CBatch, CDialog)

CBatch::CBatch(CWnd* pParent /*=NULL*/)
	: CDialog(CBatch::IDD, pParent)
{

}

CBatch::~CBatch()
{
}

void CBatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBatch, CDialog)
END_MESSAGE_MAP()


// CBatch �޽��� ó�����Դϴ�.
