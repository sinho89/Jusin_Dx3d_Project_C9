
// MapToolDoc.cpp : CMapToolDoc 클래스의 구현
//

#include "stdafx.h"
#include "MapTool.h"

#include "MapToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolDoc

IMPLEMENT_DYNCREATE(CMapToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapToolDoc, CDocument)
END_MESSAGE_MAP()


// CMapToolDoc 생성/소멸

CMapToolDoc::CMapToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMapToolDoc::~CMapToolDoc()
{
}

BOOL CMapToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMapToolDoc serialization

void CMapToolDoc::Serialize(CArchive& ar)
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


// CMapToolDoc 진단

#ifdef _DEBUG
void CMapToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMapToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMapToolDoc 명령
