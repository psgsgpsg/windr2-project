// 2DTransDoc.cpp : CMy2DTransDoc Ŭ������ ����
//

#include "stdafx.h"
#include "2DTrans.h"

#include "2DTransDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2DTransDoc

IMPLEMENT_DYNCREATE(CMy2DTransDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy2DTransDoc, CDocument)
END_MESSAGE_MAP()


// CMy2DTransDoc ����/�Ҹ�

CMy2DTransDoc::CMy2DTransDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CMy2DTransDoc::~CMy2DTransDoc()
{
}

BOOL CMy2DTransDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}

// CMy2DTransDoc serialization

void CMy2DTransDoc::Serialize(CArchive& ar)
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


// CMy2DTransDoc ����

#ifdef _DEBUG
void CMy2DTransDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy2DTransDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMy2DTransDoc ���

void CMy2DTransDoc::GetMode(int getflag)
{
	 m_flag=getflag;
}

int CMy2DTransDoc::PutMode()
{
	 return m_flag;
} 
