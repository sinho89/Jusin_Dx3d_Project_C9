
// AnimationToolDoc.cpp : CAnimationToolDoc Ŭ������ ����
//

#include "stdafx.h"
#include "AnimationTool.h"

#include "AnimationToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimationToolDoc

IMPLEMENT_DYNCREATE(CAnimationToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnimationToolDoc, CDocument)
END_MESSAGE_MAP()


// CAnimationToolDoc ����/�Ҹ�

CAnimationToolDoc::CAnimationToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CAnimationToolDoc::~CAnimationToolDoc()
{
}

BOOL CAnimationToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CAnimationToolDoc serialization

void CAnimationToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CAnimationToolDoc ����

#ifdef _DEBUG
void CAnimationToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnimationToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAnimationToolDoc ���
