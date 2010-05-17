// 2DTransView.cpp : CMy2DTransView 클래스의 구현
#include "stdafx.h"
#include "2DTrans.h"

#include "MainFrm.h"
#include "2DTransDoc.h"
#include "2DTransView.h"

// 점 데이터를 저장하기 위한 STL 템플릿 라이브러리 사용
#include <vector>
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 점 데이터를 저장하기 위한 구조체
// R, G, B : 색상 데이터
// nNodes : 형상의 노드 갯수
// *Xpos, *Ypos : 데이터의 x, y좌표
typedef struct DisplayList {
	int	R, G, B;                    // 색상 정보
	int	nNodes;                     // 노드 갯수
	vector<double> XPos, YPos;      // X, Y 노드 좌표 벡터
} DisplayList;

// 점 데이터 저장을 위한 구조체를 선언함
vector<DisplayList> DList, tempList;

// 변수 설정
int cen_x, cen_y;                   // 뷰 영역의 중심점 좌표
double MaxX, MaxY, MinX, MinY;		// 데이터로부터 최대 최소값을 읽어들임
double ScaleX, ScaleY;				// X, Y 방향으로의 scale factor
int nElements;                      // 전체 도형의 갯수
int nflag, m_option;
CPoint anchor, last;							// Mouse location points
double Scale, wsx, wsy, CenX, CenY;

// CMy2DTransView

IMPLEMENT_DYNCREATE(CMy2DTransView, CView)

BEGIN_MESSAGE_MAP(CMy2DTransView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CMy2DTransView::OnFileNew)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy2DTransView::OnFilePrintPreview)
	
	// 마우스 명령에 따른 메시지 맵
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMy2DTransView 생성/소멸
CMy2DTransView::CMy2DTransView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	nflag = 0;
	nElements = 0;
}

CMy2DTransView::~CMy2DTransView()
{
}

BOOL CMy2DTransView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMy2DTransView 그리기

void CMy2DTransView::OnDraw(CDC* pDC)
{
	CMy2DTransDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// client 영역 설정
    CRect rcClient;
	GetClientRect(rcClient);
 
	COLORREF crBack = RGB(255, 255, 255);	// 바탕 화면의 색 지정.
    CBrush jbrBack;
	jbrBack.CreateSolidBrush(crBack);		// 지정된 색으로 브러시 생성.
	pDC->FillRect(rcClient, &jbrBack);		// 브러시로 클라이언트 영역을 채움.
    pDC->SetBkColor(crBack);				// 지정된 바탕화면 색으로 덮음.
	
	//**************************************************************************//
	// 점 데이터의 최대 최소값을 읽어들여 스케일링을 하는 부분                          //
	//**************************************************************************//

	// nElements가 0이 아닐 경우에만
	if (nElements != 0) {
       	int WIDTH = rcClient.Width();				// 클라이언트 영역의 폭을 읽어옴
	    int HEIGHT = rcClient.Height();				// 클라이언트 영역의 높이를 읽어옴
	    cen_x = (int)(WIDTH/2);						// 클라이언트 영역의 중앙점 x 좌표
	    cen_y = (int)(HEIGHT/2);					// 클라이언트 영역의 중앙점 y 좌표

		/* 읽어들인 데이터로부터 중심점을 계산 */
        wsx = (MaxX - MinX) / 2;
		wsy = (MaxY - MinY) / 2;
		CenX = MinX + wsx;
		CenY = MinY + wsy;

		// 스케일을 계산함. 작은쪽을 취해준다 min(x, y)를 사용함
		Scale = (double)(0.9 * min(WIDTH/(wsx*2), HEIGHT/(wsy*2)));

		/* 계산한 중심점을 스케일에 맞추어 변환 */
		CenX = CenX * Scale;
		CenY = CenY * Scale;

		//**************************************************************************//
        // DList를 tempList로 복사하는 부분
    	//**************************************************************************//
		// tempList를 초기화하고 DList로부터 데이터를 복사함
		tempList.clear();
		tempList.assign(DList.begin(), DList.end());


		// 계산된 스케일로 점 데이터를 다시 계산
		for(vector<DisplayList>::iterator j = tempList.begin(); j != tempList.end(); ++j) {
			// X 좌표 계산
			for(int i = 0; i < j->nNodes; ++i) {
//				j->XPos.at(i) = WIDTH  - ( CenX - j->XPos.at(i) ) * 0.2 * Scale;
//				j->YPos.at(i) = HEIGHT - ( j->YPos.at(i) - CenY ) * 0.2 * Scale;
				j->XPos.at(i) =          ( Scale * ( j->XPos.at(i) - (double)MinX ) );
				j->YPos.at(i) = HEIGHT - ( Scale * ( j->YPos.at(i) - (double)MinY ) );

				// *(xpos+j) = WIDTH - (CenX - *(xpos+j)) * 0.2 * Scale;
				// *(ypos+j) = HEIGHT - (*(ypos+j) - CenY) * 0.2 * Scale;
				// *(xpos+j) = ( *(xpos+j)*(+Scale) ) - ( Scale * MinX ) + CenX;
				// *(ypos+j) = ( *(ypos+j)*(-Scale) ) - ( Scale * MinY ) + CenY;
			}
		}
	}
	
	//nflag = pDoc->PutMode();
	DrawLines();
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

void CMy2DTransView::OnMouseMove(UINT nFlags, CPoint point) {
	// TODO: 마우스 이동시 상태 표시줄에 좌표를 출력합니다.
	CMainFrame* pmf = (CMainFrame*)AfxGetMainWnd();
	CString status;

	status.Format(_T("X 좌표 : %ld / Y 좌표 : %ld"), point.x, point.y);
	pmf->m_wndStatusBar.GetElement(0)->SetText(status);
	pmf->m_wndStatusBar.RecalcLayout();	
	pmf->m_wndStatusBar.RedrawWindow();
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
    // TODO: Add your command handler code here
	TCHAR szFilter[] = _T("DAT File (.dat)|*.dat|OUT File (.out)|*.out|All File (.*)|*.*||");

	CFileDialog m_FileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_EXPLORER, szFilter);
	m_FileOpenDialog.m_ofn.lpstrTitle = _T("데이터 파일 열기");

	if( m_FileOpenDialog.DoModal() == IDOK ) {
		CWnd* pWnd = AfxGetMainWnd();
		pWnd->SetWindowText( (LPCTSTR)m_FileOpenDialog.GetFileTitle() );
		FileRead( m_FileOpenDialog.GetPathName() );
	} 
	
	if( m_FileOpenDialog.GetFileName() == _T("\0") ) {
		AfxMessageBox( _T("파일을 선택하지 않으셨습니다") );
	}
}

void CMy2DTransView::FileRead(CString FileName) {
    int i, cnt=0;
	TCHAR str[100] = {L'\0'};
    int red = 0, green = 0, blue = 0;       // 기본 색상은 검은색으로 지정
    MinX = 1.0E6;  MinY = 1.0E6;
    MaxX = 1.0E-6; MaxY = 1.0E-6;
    double x = 0., y = 0.;

    // DList 원소의 갯수가 0이 아닐 경우 모든 원소를 삭제하도록 해야함
    if(!DList.empty()) { DList.clear(); }

    // 파일 포인터를 연다
    FILE *fp;

    // 파일에 대한 읽기 모드가 실패할 경우 메세지 출력
    if ( _tfopen_s(&fp, (LPCTSTR)FileName, _T("r")) != NULL) {
        AfxMessageBox(_T("선택한 파일을 읽기 모드로 열수 없습니다.\n다른 프로그램에서 사용중이지 않은지 확인하시기 바랍니다."));
		return;
    }

    // 파일 구조를 parsing 하는 부분
	while (_fgetts(str, 100, fp) != NULL) {
		DisplayList tmp;                        // 임시 리스트
			
		// 첫줄에 "COLOR"가 있다면 R, G, B값을 읽어들임
		if (str[0] == 'C') {
			_stscanf_s (_tcschr(str, '/')+1, _T("%d %d %d"), &red, &green, &blue);
		}
		// 만약 COLOR가 없다면 이전에 읽어들인 색(혹은 기본색인 흑색)이 할당되거나
		// 두번째 줄로 넘어가면서 이전에 앞서 읽어들인 색 정보가 할당된다
		else if (str[0] == 'P') {
			tmp.R = red;
			tmp.G = green;
			tmp.B = blue;

			// "POLYGON /" 이후에는 노드의 갯수가 대입된다.
			_stscanf_s (_tcschr(str, '/')+1, _T("%d"), &tmp.nNodes);

			// 점 데이터를 읽어들이는 부분
			for (i = 0; i < tmp.nNodes; ++i) {
				_ftscanf_s(fp, _T("%lf %lf"), &x, &y);
				tmp.XPos.push_back(x);
				tmp.YPos.push_back(y);

				if (x < MinX) MinX = x;
				if (y < MinY) MinY = y;
				if (x > MaxX) MaxX = x;
				if (y > MaxY) MaxY = y;
			}
			// tmp를 DList의 마지막 원소로 삽입
			DList.push_back(tmp);
			nElements++;
		}
	}
	
	// 최종 element의 갯수는 DList의 사이즈임 (size_type을 int로 캐스팅)
	nElements = (int)DList.size();

	// 파일을 닫음
	fclose(fp);
    
	// 읽어들인 데이터로부터 document의 내용을 반영
	CWnd* pWnd = AfxGetMainWnd();
	pWnd->RedrawWindow();
}


// 펜으로 그리는 동작 구현 부분
void CMy2DTransView::DrawLines() {
	// nElements가 0이 아닐 경우에만
	if (nElements != 0) {
		CClientDC dc(this);

		//dc.SetMapMode(MM_TEXT | MM_ISOTROPIC);
		// pDC->SetWindowExt(rcClient.Width(), rcClient.Height());
		// pDC->SetViewportExt(rcClient.Width(), rcClient.Height());
		//dc.SetViewportOrg( -int(MinX * Scale), -int(MinY * Scale) );

		for(vector<DisplayList>::iterator iterPos = tempList.begin(); iterPos != tempList.end(); ++iterPos) {
			// 펜의 속성 및 색상 설정
			CPen NewPen(PS_SOLID, 1, RGB(iterPos->R, iterPos->G, iterPos->B));
			dc.SelectObject(&NewPen);

			// i번째 element의 점 데이터를 이용해서 선을 그림
			for(int i = 0; i < iterPos->nNodes - 1; i++) {
				dc.MoveTo( (int)(iterPos->XPos.at(i))  , (int)(iterPos->YPos.at(i))   );
				dc.LineTo( (int)(iterPos->XPos.at(i+1)), (int)(iterPos->YPos.at(i+1)) );
			}

			// nElements가 2인 경우에는 이 동작이 필요 없음
			if (iterPos->nNodes != 2) {
				dc.MoveTo( (int)(iterPos->XPos.at(iterPos->nNodes - 1)),
					(int)(iterPos->YPos.at(iterPos->nNodes - 1)) );
				dc.LineTo( (int)(iterPos->XPos.at(0)), (int)(iterPos->YPos.at(0)) );
			}
		}
	}

}

// 새 파일을 만들 경우
void CMy2DTransView::OnFileNew() {
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CClientDC dc(this);						// 클라이언트 영역의 dc를 읽어옴

	CRect rcClient;							// 클라이언트 영역을 읽어옴 
	GetClientRect(rcClient);

	COLORREF crBack;						// 바탕화면의 색 지정 (white)
	CBrush jbrBack; 
	crBack = RGB(255, 255, 255) ;

	jbrBack.CreateSolidBrush(crBack) ;		// 지정된 색으로 영역을 채움
	dc.FillRect(rcClient, &jbrBack) ;
	dc.SetBkColor(crBack) ;

	nElements = 0;							// Element 갯수를 0으로 리셋하여 draw 방지
	tempList.clear();
	DList.clear();

	
	RedrawWindow();							// 내용을 다시 그리도록 함 (OnDraw 호출)
}

