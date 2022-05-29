// Batch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EffectTool.h"
#include "Batch.h"


// CBatch 대화 상자입니다.

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


// CBatch 메시지 처리기입니다.
