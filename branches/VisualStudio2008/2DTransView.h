﻿// 2DTransView.h : CMy2DTransView 클래스의 인터페이스
//

#pragma once

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
	bool FileRead(CString);				// 파일 parsing을 위한 처리
	void DrawLines();					// 선을 그리는 함수
	
	int DirSize;						// 형상 변경시 사용되는 변수
	void SetDirSize(int);				// 이동 크기 변경시 적용하는 함수
	double GetDirSize();				// 이동 크기를 얻어내는 함수

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void AddToRecentFileList(LPCTSTR lpszPathName); // 파일을 MRU 목록에 추가합니다.
	virtual CMainFrame* GetMainFrm(); // 주 윈도우 주소창의 주소를 반환합니다.	

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
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
public:
	afx_msg void OnFileOpen();			// 파일을 열기 위한 함수 처리
	afx_msg void OnFileNew();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDirUp();
	afx_msg void OnDirDown();
	afx_msg void OnDirLeft();
	afx_msg void OnDirLup();
	afx_msg void OnDirLdown();
	afx_msg void OnDirRdown();
	afx_msg void OnDirRight();
	afx_msg void OnDirRup();
	afx_msg void OnRotateLeft();
	afx_msg void OnRotateRight();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 2DTransView.cpp의 디버그 버전
inline CMy2DTransDoc* CMy2DTransView::GetDocument() const
   { return reinterpret_cast<CMy2DTransDoc*>(m_pDocument); }
#endif
