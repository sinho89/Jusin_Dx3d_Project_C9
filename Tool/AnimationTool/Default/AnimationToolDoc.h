
// AnimationToolDoc.h : CAnimationToolDoc Ŭ������ �������̽�
//


#pragma once


class CAnimationToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CAnimationToolDoc();
	DECLARE_DYNCREATE(CAnimationToolDoc)

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
	virtual ~CAnimationToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


