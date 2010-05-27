// 2DTransDoc.cpp : CMy2DTransDoc 클래스의 구현
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


// CMy2DTransDoc 생성/소멸

CMy2DTransDoc::CMy2DTransDoc()
{
	// 여기에 일회성 생성 코드를 추가합니다.

}

CMy2DTransDoc::~CMy2DTransDoc()
{
}

BOOL CMy2DTransDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CMy2DTransDoc serialization

void CMy2DTransDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// 여기에 로딩 코드를 추가합니다.
	}
}


// CMy2DTransDoc 진단

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


// CMy2DTransDoc 명령

void CMy2DTransDoc::GetMode(int getflag)
{
	 m_flag=getflag;
}

int CMy2DTransDoc::PutMode()
{
	 return m_flag;
} 
