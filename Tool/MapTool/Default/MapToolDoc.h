
// MapToolDoc.h : CMapToolDoc Ŭ������ �������̽�
//


#pragma once

class CMapToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMapToolDoc();
	DECLARE_DYNCREATE(CMapToolDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CMapToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


