// 2DTransView.cpp : CMy2DTransView 클래스의 구현
#include "stdafx.h"
#include "2DTrans.h"

#include "MainFrm.h"
#include "2DTransDoc.h"
#include "2DTransView.h"
//#include "windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CMy2DTransView

IMPLEMENT_DYNCREATE(CMy2DTransView, CView)

BEGIN_MESSAGE_MAP(CMy2DTransView, CView)
	// 표준 메뉴 명령입니다.
	ON_COMMAND(ID_FILE_OPEN, &CMy2DTransView::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CMy2DTransView::OnFileNew)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy2DTransView::OnFilePrintPreview)

	// 마우스 명령에 따른 메시지 맵
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()

	// 메뉴 코맨드 메시지 맵
	// 이동 범주 메뉴 메시지
	ON_COMMAND(ID_DIR_DOWN, &CMy2DTransView::OnDirDown)
	ON_COMMAND(ID_DIR_LEFT, &CMy2DTransView::OnDirLeft)
	ON_COMMAND(ID_DIR_LUP, &CMy2DTransView::OnDirLup)
	ON_COMMAND(ID_DIR_LDOWN, &CMy2DTransView::OnDirLdown)
	ON_COMMAND(ID_DIR_RDOWN, &CMy2DTransView::OnDirRdown)
	ON_COMMAND(ID_DIR_RIGHT, &CMy2DTransView::OnDirRight)
	ON_COMMAND(ID_DIR_RUP, &CMy2DTransView::OnDirRup)
	ON_COMMAND(ID_DIR_UP, &CMy2DTransView::OnDirUp)

	// 회전 및 크기 변경 메뉴 메시지
	ON_COMMAND(ID_ROT_LEFT, &CMy2DTransView::OnRotateLeft)
	ON_COMMAND(ID_ROT_RIGHT, &CMy2DTransView::OnRotateRight)
END_MESSAGE_MAP()

// CMy2DTransView 생성/소멸
CMy2DTransView::CMy2DTransView()
{
	// 여기에 생성 코드를 추가합니다.
	nElements = 0;
	DirSize = 10;
	Scale = 1.0;
	moveX = 0.0;
	moveY = 0.0;
	delScale = 0.1;

	jbrBack.CreateSolidBrush(crBack);		// 지정된 색으로 브러시 생성.
	crBack = RGB(255, 255, 255);			// 배경 색을 흰색으로 설정.
}

CMy2DTransView::~CMy2DTransView()
{
}

BOOL CMy2DTransView::PreCreateWindow(CREATESTRUCT& cs)
{
	// CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMy2DTransView 그리기

void CMy2DTransView::OnDraw(CDC* pDC)
{
	// 2DTransDoc 클래스의 구현에 문제를 확인하는 부분으로
	// 이 부분은 특별한 문제가 없을 경우 삭제해도 무방함
	// 예외처리가 필요한 경우에는 반드시 이 부분이 존재해야 함
	// CMy2DTransDoc* pDoc = GetDocument();
	// ASSERT_VALID(pDoc);
	// if (!pDoc) return;

	GetClientRect(rcClient);				// client 영역 설정 
	//pDC->FillRect(rcClient, &jbrBack);		// 브러시로 클라이언트 영역을 채움.
    //pDC->SetBkColor(crBack);					// 지정된 바탕화면 색으로 덮음.
	
	/* 점 데이터의 최대 최소값을 읽어들여 스케일링을 하는 부분
	 1. 마우스 조작이 가해진 경우 GetCapture()로 플래그를 읽어 들여 동작 여부를 확인함
	 2. 도형 요소의 수가 0일 경우에는 동작하지 않음 */

	// nElements가 0이 아닐 경우에만
	if (nElements != 0) {
       	WIDTH = rcClient.Width();					// 클라이언트 영역의 폭을 읽어옴
	    HEIGHT = rcClient.Height();					// 클라이언트 영역의 높이를 읽어옴
	    cen_x = (int)(WIDTH/2);						// 클라이언트 영역의 중앙점 x 좌표
	    cen_y = (int)(HEIGHT/2);					// 클라이언트 영역의 중앙점 y 좌표

		/* 읽어들인 데이터로부터 중심점을 계산 */
        wsx = (MaxX - MinX) / 2;
		wsy = (MaxY - MinY) / 2;
		CenX = MinX + wsx;
		CenY = MinY + wsy;

		// 스케일을 계산함. 작은쪽을 취해준다 min(x, y)를 사용함
		// 마우스 휠 조작으로 스케일이 변경된 경우에는 계산하지 않음
		if ( !isScaleRatioCustomized ) {
			Scale = (double)(0.9 * min(WIDTH/(wsx*2), HEIGHT/(wsy*2)));
		}
		
		/* 계산한 중심점을 스케일에 맞추어 변환하고, 클라이언트 영역의 중심점과의 차이를 저장 */
		CenX = cen_x - Scale * ( CenX - MinX );
		CenY = cen_y - ( HEIGHT - ( Scale * ( CenY - MinY) ) );

		/* 데이터의 원점을 스케일에 맞추어 변환하고, 클라이언트 영역의 중심점과의 차이를 저장 */
		originX = ( Scale * -MinX ) + CenX + moveX;
		originY = ( HEIGHT + (Scale * MinY) ) + CenY + moveY;

		// 계산된 스케일로 점 데이터를 다시 계산
		if ( GetCapture() != this ) {		// 마우스 조작이 걸려있는 경우, 이 부분은 넘어감
			int k = 0;

			for(vector<DisplayList>::iterator j = tempList.begin(); j != tempList.end(); ++j) {
				for(unsigned int i = 0; i < j->GetNodes(); ++i) {
					j->setXPos( i, Scale * ( DList[k].getXPos(i) - MinX ) + CenX + moveX);
					j->setYPos( i, (HEIGHT - ( Scale * ( DList[k].getYPos(i) - MinY ) )) + CenY + moveY);
				}
				++k;
			}
		}
		DrawLines();		// 계산된 tempList를 비트맵으로 저장
	}
}

// CMy2DTransView 인쇄
void CMy2DTransView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMy2DTransView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMy2DTransView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMy2DTransView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMy2DTransView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy2DTransView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

void CMy2DTransView::AddToRecentFileList(LPCTSTR lpszPathName) // 최근 파일 열기 목록에 추가하는 명령입니다.
{
    ((CMy2DTransApp*)AfxGetApp())->AddToRecentFileList(lpszPathName);
}

CMainFrame* CMy2DTransView::GetMainFrm() // 주 윈도우 주소창의 주소를 반환합니다.
{
	return ( (CMainFrame*)AfxGetMainWnd() );
}

// CMy2DTransView 진단
#ifdef _DEBUG
void CMy2DTransView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2DTransView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2DTransDoc* CMy2DTransView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2DTransDoc)));
	return (CMy2DTransDoc*)m_pDocument;
}
#endif //_DEBUG

// CMy2DTransView 메시지 처리기
// 1. 파일을 열 경우 메세지 처리
void CMy2DTransView::OnFileOpen() {
	TCHAR szFilter[] = _T("DAT File (.dat)|*.dat|OUT File (.out)|*.out|All File (.*)|*.*||");

	CFileDialog m_FileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_EXPLORER, szFilter);
	m_FileOpenDialog.m_ofn.lpstrTitle = _T("데이터 파일 열기");			// 파일 열기 대화 상자의 제목을 설정

	// 현재 프로그램 실행 경로를 기본 경로로 설정
	TCHAR currentPath[250];
	GetCurrentDirectory(255, currentPath);
	m_FileOpenDialog.m_ofn.lpstrInitialDir = (LPCTSTR)currentPath;

	if( m_FileOpenDialog.DoModal() == IDOK ) {
		AddToRecentFileList( (LPCTSTR)m_FileOpenDialog.GetPathName() ); // MRU 목록에 해당 파일을 추가
		
		if( !FileRead( m_FileOpenDialog.GetPathName() ) ) {
			AfxMessageBox( _T("파일 읽기가 제대로 수행되지 않았습니다.") );
			return;
		}// 파일 열기 함수 호출
	} 
	
	if( m_FileOpenDialog.GetFileName() == _T("\0") ) {
		AfxMessageBox( _T("파일을 선택하지 않으셨습니다") );
	}
}

bool CMy2DTransView::FileRead(CString FileName) {
	TCHAR str[100] = {L'\0'};
    bool isColorSelected; // 색이 지정되었는지 여부를 확인하는 변수
    MinX = 1.0E6;  MinY = 1.0E6;
    MaxX = 1.0E-6; MaxY = 1.0E-6;
    double x = 0., y = 0.;
	int red, green, blue;
	unsigned int nNodes;

    // DList 원소의 갯수가 0이 아닐 경우 모든 원소를 삭제하도록 해야함
    if( !DList.empty() ) { DList.clear(); }
	if( !tempList.empty() ) { tempList.clear(); }

	// 마우스 움직임 변량값을 초기화
	moveX = 0;
	moveY = 0;

	// 휠을 통한 스케일 변경 플래그를 초기화
	isScaleRatioCustomized = false;

    // 파일 포인터를 설정/
    FILE *fp;

    // 파일에 대한 읽기 모드가 실패할 경우 메세지 출력
    if ( _tfopen_s(&fp, (LPCTSTR)FileName, _T("r")) != NULL) {
        AfxMessageBox(_T("선택한 파일을 읽기 모드로 열수 없습니다.\n다른 프로그램에서 사용중이지 않은지 확인하시기 바랍니다."));
		return false;
    }

	DisplayList tmp;                        // 임시 리스트

	// 파일 구조를 parsing 하는 부분
	while (_fgetts(str, 100, fp) != NULL) {
		// 첫줄에 "COLOR"가 있다면 R, G, B값을 읽어들임
		if (str[0] == 'C') {
			_stscanf_s ( _tcschr(str, '/')+1, _T("%d %d %d"), &red, &green, &blue );

			// R, G, B 색상은 모두 0-255사이의 값이어야 하므로
			if ( red < 0 || red > 255 || green <0 || green > 255 || blue < 0 || blue > 255 ) {
				// 오류 메시지 출력
				AfxMessageBox( _T("색상 데이터는 0~255 사이어야 합니다.\n데이터 파일이 잘못되지 않았는지 확인하십시오.") );
				return false;
			} else
			{
				// 정상적으로 지정된 경우 색상을 저장
				tmp.SetRGB(red, green, blue);
				isColorSelected = true; // 색이 지정되었음을 지정함
			}
		}
		else if (str[0] == 'P') {
			// 만약 COLOR가 없다면 기본색인 흑색이 할당된다
			if (!isColorSelected) {
				tmp.SetRGB(0, 0, 0);
			}

			// "POLYGON /" 이후에는 노드의 갯수가 대입된다
			_stscanf_s (_tcschr(str, '/')+1, _T("%d"), &nNodes);

			// 노드의 갯수는 최소 2개이상이어야 하므로
			if ( nNodes < 2) {
				// 오류 메시지 출력
				AfxMessageBox(_T("노드 갯수는 최소한 2개 이상이어야 합니다.\n데이터 파일이 잘못되지 않았는지 확인하십시오."));
				return false;
			} else {
				// 2개 이상으로 지정된 경우 vector 컨테이너의 공간을 미리 할당함
				tmp.setNodes(nNodes);
			}

			// 점 데이터를 읽어들이는 부분
			for (unsigned int i = 0; i < nNodes; ++i) {
				_ftscanf_s(fp, _T("%lf %lf"), &x, &y);
				tmp.addXPos(x);
				tmp.addYPos(y);

				if (x < MinX) MinX = x;
				if (y < MinY) MinY = y;
				if (x > MaxX) MaxX = x;
				if (y > MaxY) MaxY = y;
			}
			DList.push_back(tmp);		// tmp를 DList의 마지막 원소로 삽입
			tmp.reset();				// tmp를 초기화 함
			isColorSelected = false;	// 색 지정 여부 플래그를 초기화 함
		}
	}
	
	// 최종 element의 갯수는 DList의 사이즈임 (size_type을 int로 캐스팅)
	nElements = (int)DList.size();

	// tempList에 읽어들인 원본 데이터 DList를 복사
	// 미리 공간을 확보
	tempList.assign(DList.begin(), DList.end());

	// 파일을 닫음
	fclose(fp);

	// 읽어들인 데이터로부터 document의 내용을 반영
	GetMainFrm()->RedrawWindow();
	status.Format( _T("%s - 2DTrans"), FileName );
	GetMainFrm()->SetWindowText( status ); // 윈도우 제목창을 다시 설정함

	// 파일 읽기 성공 여부 반환
	return true;
}

// 스케일을 다시 계산하고, 다시 그리는 함수
void CMy2DTransView::recalcScale() {
	// 스케일 변경 플래그를 해제
	isScaleRatioCustomized = false;

	// 다시 그리기
	GetMainFrm()->RedrawWindow();

	// Status Bar에 현재 Scale을 반영
	status.Format(_T("X 좌표 : %ld / Y 좌표 : %ld / 현재 배율 : %8.6lf"), curPoint.x, curPoint.y, Scale);
	GetMainFrm()->m_wndStatusBar.GetElement(0)->SetText(status);
}

// 펜으로 그리는 동작 구현 부분
void CMy2DTransView::DrawLines() {
	// nElements가 0이 아닐 경우에만
	if (nElements != 0) {
		CClientDC dc(this);

		for(vector<DisplayList>::iterator iterPos = tempList.begin(); iterPos != tempList.end(); ++iterPos) {
			// 펜의 속성 및 색상 설정
			CPen NewPen( PS_SOLID, 1, RGB( iterPos->getR(), iterPos->getG(), iterPos->getB() ) );
			dc.SelectObject(&NewPen);

			// i번째 element의 점 데이터를 이용해서 선을 그림
			for(unsigned int i = 0; i < iterPos->GetNodes() - 1; i++) {
				dc.MoveTo( (int)iterPos->getXPos(i)  , (int)iterPos->getYPos(i)   );
				dc.LineTo( (int)iterPos->getXPos(i+1), (int)iterPos->getYPos(i+1) );
			}

			// 마지막 노드와 처음 노드를 잇는 부분
			// 노드 갯수가 2개인 경우에는 이 동작이 필요하지 않음
			if (iterPos->GetNodes() != 2) {
				dc.MoveTo( (int)iterPos->getXPos(iterPos->GetNodes() - 1), (int)iterPos->getYPos(iterPos->GetNodes() - 1) );
				dc.LineTo( (int)iterPos->getXPos(0)                      , (int)iterPos->getYPos(0) );
			}
		}
	}
}

// 새 파일을 만들 경우
void CMy2DTransView::OnFileNew() {
	CClientDC dc(this);						// 클라이언트 영역의 dc를 읽어옴
	CString str;
	str.Format( _T("제목 없음 - 2DTrans") );

	GetClientRect(rcClient);

	jbrBack.CreateSolidBrush(crBack) ;		// 지정된 색으로 영역을 채움
	dc.FillRect(rcClient, &jbrBack) ;
	dc.SetBkColor(crBack) ;

	nElements = 0;							// Element 갯수를 0으로 리셋하여 draw 방지
	tempList.clear();						// DisplayList 데이터를 모두 초기화함
	DList.clear();

	Scale = 0.0;							// Scale을 0.0으로 리셋하고
	moveX = 0.0;  moveY = 0.0;				// 이동 변량 변수 모두 리셋	
	isScaleRatioCustomized = false;					// 스케일 변경 여부 리셋

	GetMainFrm()->m_wndStatusBar.GetElement(0)->SetText( _T("준비됨") );	// 상태 표시줄의 메시지를 초기화
	GetMainFrm()->SetWindowText( str );										// 제목 표시줄을 초기화
	GetMainFrm()->RedrawWindow();											// 내용을 다시 그리도록 함 (OnDraw 호출)
}

BOOL CMy2DTransView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 마우스 조작이 가해졌음을 플래그에 설정
		isScaleRatioCustomized = true;
		
		if( (nFlags & MK_CONTROL) != MK_CONTROL ) {
			return CView::OnMouseWheel(nFlags, zDelta, point);
		}
		
		if ( zDelta > 0 ) {
			Scale += 0.1;

			if ( Scale > 100 ) {
				Scale = 100;
			}
		}
		else {
			Scale -= 0.1;
			
			if (Scale < 1E-6) {
				Scale = 1E-6;
			}
		}
			
		status.Format(_T("X 좌표 : %ld / Y 좌표 : %ld / 현재 배율 : %8.6lf"), point.x, point.y, Scale);

		GetMainFrm()->m_wndStatusBar.GetElement(0)->SetText(status);
		//pmf->m_wndStatusBar.RecalcLayout();	
		//pmf->m_wndStatusBar.RedrawWindow();

		// view에 내용을 반영
		GetMainFrm()->RedrawWindow();
	}

	return CView::OnMouseWheel(nFlags, zDelta, point);
}

void CMy2DTransView::OnMouseMove(UINT nFlags, CPoint point) {
	// 마우스 이동시 상태 표시줄에 좌표를 출력합니다.
	// 만약 드래그를 한다면 점을 이동합니다.
	CPoint realPos(0, 0);

	if( GetCapture() == this ) {
		// tempList의 사이즈가 0이 아닐 경우에만
		if ( tempList.size() > 0 ) {
			realPos += (point - anchor);
			anchor = point;

			// 계산된 좌표만큼 형상을 이동시킨다.
			if ( nElements != 0) {
				for( vector<DisplayList>::iterator j = tempList.begin(); j != tempList.end(); ++j) {
					j->Translate( (double)realPos.x, (double)realPos.y );
				}
			}

			// 마우스로 이동한 변량을 변수에 계속 더한다.
			moveX += (double)realPos.x;
			moveY += (double)realPos.y;
			
			// view에 내용을 반영
			GetMainFrm()->RedrawWindow();
		}
	}

	// 위치를 변수에 저장
	curPoint = point;

	// 상태 표시줄 업데이트
	status.Format(_T("X 좌표 : %ld / Y 좌표 : %ld / 현재 배율 : %8.6lf"), point.x, point.y, Scale);
	GetMainFrm()->m_wndStatusBar.GetElement(0)->SetText(status);
	//GetMainFrm()->m_wndStatusBar.RecalcLayout();	
	//GetMainFrm()->m_wndStatusBar.RedrawWindow();
}

void CMy2DTransView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	anchor = point;

	CView::OnLButtonDown(nFlags, point);
}

void CMy2DTransView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}

// 위로가기 버튼을 누른 경우
void CMy2DTransView::OnDirUp()
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 모든 DisplayList의 좌표를 Size만큼 위로 이동시킴 (방향이 반대이므로 y를 감소시킴)
		for( vector<DisplayList>::iterator i = tempList.begin(); i != tempList.end(); ++i ) {
			for( unsigned int j = 0; j < i->GetNodes(); ++j ) {
				i->Translate( 0.0, DirSize );
			}
		}

		// 이동 변량을 더해준다.
		moveY -= DirSize;

		SetCapture();
		RedrawWindow();
		ReleaseCapture();
	}
}

void CMy2DTransView::OnDirDown()
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 모든 DisplayList의 좌표를 Size만큼 아래로 이동시킴 (방향이 반대이므로 y를 증가시킴)
		for( vector<DisplayList>::iterator i = tempList.begin(); i != tempList.end(); ++i ) {
			for( unsigned int j = 0; j < i->GetNodes(); ++j ) {
				i->Translate( 0.0, DirSize );
			}
		}

		// 이동 변량을 더해준다.
		moveY += DirSize;

		SetCapture();
		RedrawWindow();
		ReleaseCapture();
	}
}

void CMy2DTransView::OnDirLeft()
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 모든 DisplayList의 좌표를 Size만큼 좌로로 이동시킴 (x를 감소시킴)
		for( vector<DisplayList>::iterator i = tempList.begin(); i != tempList.end(); ++i ) {
			for( unsigned int j = 0; j < i->GetNodes(); ++j ) {
				i->Translate( -DirSize, 0.0 );
			}
		}

		// 이동 변량을 더해준다.
		moveX -= DirSize;

		SetCapture();
		RedrawWindow();
		ReleaseCapture();
	}
}

void CMy2DTransView::OnDirRight()
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 모든 DisplayList의 좌표를 Size만큼 우측으로 이동시킴 (x를 증가)
		for( vector<DisplayList>::iterator i = tempList.begin(); i != tempList.end(); ++i ) {
			for( unsigned int j = 0; j < i->GetNodes(); ++j ) {
				i->Translate( DirSize, 0.0 );
			}
		}

		// 이동 변량을 더해준다.
		moveY += DirSize;

		SetCapture();
		RedrawWindow();
		ReleaseCapture();
	}
}

void CMy2DTransView::OnDirLup()
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 모든 DisplayList의 좌표를 Size만큼 좌측위로 이동 (x를 감소, y를 감소)
		for( vector<DisplayList>::iterator i = tempList.begin(); i != tempList.end(); ++i ) {
			for( unsigned int j = 0; j < i->GetNodes(); ++j ) {
				i->Translate( -DirSize, -DirSize );
			}
		}

		// 이동 변량을 더해준다.
		moveX -= DirSize;
		moveY -= DirSize;

		SetCapture();
		RedrawWindow();
		ReleaseCapture();
	}
}

void CMy2DTransView::OnDirLdown()
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 모든 DisplayList의 좌표를 Size만큼 좌측 아래로 이동 (x를 감소, y를 증가)
		for( vector<DisplayList>::iterator i = tempList.begin(); i != tempList.end(); ++i ) {
			for( unsigned int j = 0; j < i->GetNodes(); ++j ) {
				i->Translate( -DirSize, DirSize );
			}
		}

		// 이동 변량을 더해준다.
		moveX -= DirSize;
		moveY += DirSize;

		SetCapture();
		RedrawWindow();
		ReleaseCapture();
	}
}

void CMy2DTransView::OnDirRdown()
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 모든 DisplayList의 좌표를 Size만큼 우측 아래로 이동시킴 (x를 증가, y를 증가)
		for( vector<DisplayList>::iterator i = tempList.begin(); i != tempList.end(); ++i ) {
			for( unsigned int j = 0; j < i->GetNodes(); ++j ) {
				i->Translate( DirSize, DirSize );
			}
		}

		// 이동 변량을 더해준다.
		moveX += DirSize;
		moveY += DirSize;

		SetCapture();
		RedrawWindow();
		ReleaseCapture();
	}
}

void CMy2DTransView::OnDirRup()
{
	// tempList의 사이즈가 0이 아닐 경우에만
	if ( tempList.size() > 0 ) {
		// 모든 DisplayList의 좌표를 Size만큼 우측 위로 이동시킴 (x를 증가, y를 감소)
		for( vector<DisplayList>::iterator i = tempList.begin(); i != tempList.end(); ++i ) {
			for( unsigned int j = 0; j < i->GetNodes(); ++j ) {
				i->Translate( DirSize, -DirSize );
			}
		}

		// 이동 변량을 더해준다.
		moveX += DirSize;
		moveY -= DirSize;

		SetCapture();
		RedrawWindow();
		ReleaseCapture();
	}
}

void CMy2DTransView::OnRotateLeft()
{
	tempList.begin()->rot(45, originX, originY);
	// 모든 DisplayList의 좌표를 정해진 각도만큼 반시계방향으로 회전시킴
	SetCapture();
	RedrawWindow();
	ReleaseCapture();


}

void CMy2DTransView::OnRotateRight()
{
	tempList.begin()->rot(-45, originX, originY);
	// 모든 DisplayList의 좌표를 정해진 각도만큼 반시계방향으로 회전시킴
	SetCapture();
	RedrawWindow();
	ReleaseCapture();

}