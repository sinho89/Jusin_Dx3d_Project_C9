
// AnimationToolDoc.cpp : CAnimationToolDoc 클래스의 구현
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


// CAnimationToolDoc 생성/소멸

CAnimationToolDoc::CAnimationToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CAnimationToolDoc::~CAnimationToolDoc()
{
}

BOOL CAnimationToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CAnimationToolDoc serialization

void CAnimationToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CAnimationToolDoc 진단

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


// CAnimationToolDoc 명령
