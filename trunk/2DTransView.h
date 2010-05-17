// 2DTransView.h : CMy2DTransView 클래스의 인터페이스
//

#pragma once

#define MAXELEMENTS         100		// 최대 엘리먼트 갯수
#define MAXNODESPERELEMENT  4		// 엘리먼트당 최대 점 갯수

class CMy2DTransView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMy2DTransView();
	DECLARE_DYNCREATE(CMy2DTransView)

// 특성입니다.
public:
	CMy2DTransDoc* GetDocument() const;

// 직접 추가한 멤버 함수 및 변수 리스트
public:
	afx_msg void OnFileOpen();			// 파일을 열기 위한 함수 처리
	afx_msg void OnFileNew();
	void FileRead(CString);				// 파일 parsing을 위한 처리
	void DrawLines();					// 선을 그리는 함수

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMy2DTransView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 2DTransView.cpp의 디버그 버전
inline CMy2DTransDoc* CMy2DTransView::GetDocument() const
   { return reinterpret_cast<CMy2DTransDoc*>(m_pDocument); }
#endif

