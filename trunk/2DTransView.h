// 2DTransView.h : CMy2DTransView 클래스의 인터페이스
//

#pragma once
// 표준 STL vector 컨테이너와, DisplayList 클래스
#include <vector>
#include "DisplayList.h"

class CMy2DTransView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMy2DTransView();
	DECLARE_DYNCREATE(CMy2DTransView)

// 특성입니다.
// 직접 추가한 멤버 변수 리스트
public:
	CMy2DTransDoc* GetDocument() const;
	double Scale, delScale;						// 스케일 및 스케일 증분 변수
	int DirSize;								// 형상 변경시 사용되는 변수
	vector<DisplayList> DList, tempList;		// 점 데이터 저장을 위한 구조체를 선언함
	//CBrush jbrBack;							// 바탕 배경 칠하기용 브러시
	CRect rcClient;								// 클라이언트 영역 저장용 구조체
	//COLORREF crBack;							// 배경 색을 저장하기 위한 구조체
	CPoint anchor, curPoint;					// Mouse location points
	CString status;								// 상태 표시줄용 CString 객체
	int cen_x, cen_y;							// 뷰 영역의 중심점 좌표
	double MaxX, MaxY, MinX, MinY;				// 데이터로부터 최대 최소값을 읽어들임
	int nElements;								// 전체 도형의 갯수
	double wsx, wsy, CenX, CenY;
	double moveSize, scaleSize;					// UI로부터의 입력 변수 저장용
	double originX, originY;					// 데이터의 원점이 View상의 좌표로 변환된 좌표
	double moveX, moveY;						// 마우스로 드래그시 이동한 총 변량을 저장하는 데이터
	int WIDTH, HEIGHT;							// View 영역의 폭과 높이를 저정하는 변수
	double rotCenterX, rotCenterY;				// 회전 중심 좌표점 (데이터 좌표)
	double rotCenterX_view, rotCenterY_view;	// 회전 중심 좌표점 (view에 매핑된 좌표)
	double rotAngle;							// 회전 각도 저장 변수
	double totalRot;							// 전체 회전 각도 저장 변수
// 플래그 관련
	bool isScaleRatioCustomized;				// 마우스 휠을 통해 스케일이 변경되었는지 여부를 저장
// 직접 추가한 멤버 함수 리스트
	bool FileRead(CString);						// 파일 parsing을 위한 처리
	void DrawLines();							// 선을 그리는 함수
	void DrawAxes(); // 좌표축을 그립니다.

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void AddToRecentFileList(LPCTSTR lpszPathName); // 파일을 MRU 목록에 추가합니다.
	CMainFrame* GetMainFrm(); // 주 윈도우 주소창의 주소를 반환합니다.
	static size_t round( double d ); // 반올림값을 반환합니다.

// 구현입니다.
public:
	virtual ~CMy2DTransView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

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
	afx_msg void OnScaleMagnify();
	afx_msg void recalcScale();			// 스케일을 원래대로 설정하고, 다시 그리는 함수
	afx_msg void OnScaleShrink();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 2DTransView.cpp의 디버그 버전
inline CMy2DTransDoc* CMy2DTransView::GetDocument() const
   { return reinterpret_cast<CMy2DTransDoc*>(m_pDocument); }
#endif

