// 2DTransView.cpp : CMy2DTransView Ŭ������ ����
#include "stdafx.h"
#include "2DTrans.h"

#include "MainFrm.h"
#include "2DTransDoc.h"
#include "2DTransView.h"

// �� �����͸� �����ϱ� ���� STL ���ø� ���̺귯�� ���
#include <vector>
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// �� �����͸� �����ϱ� ���� ����ü
// R, G, B : ���� ������
// nNodes : ������ ��� ����
// *Xpos, *Ypos : �������� x, y��ǥ
typedef struct DisplayList {
	int	R, G, B;                    // ���� ����
	int	nNodes;                     // ��� ����
	vector<double> XPos, YPos;      // X, Y ��� ��ǥ ����
} DisplayList;

// �� ������ ������ ���� ����ü�� ������
vector<DisplayList> DList, tempList;

// ���� ����
int cen_x, cen_y;                   // �� ������ �߽��� ��ǥ
double MaxX, MaxY, MinX, MinY;		// �����ͷκ��� �ִ� �ּҰ��� �о����
double ScaleX, ScaleY;				// X, Y ���������� scale factor
int nElements;                      // ��ü ������ ����
int nflag, m_option;
CPoint anchor, last;							// Mouse location points
double Scale, wsx, wsy, CenX, CenY;

// CMy2DTransView

IMPLEMENT_DYNCREATE(CMy2DTransView, CView)

BEGIN_MESSAGE_MAP(CMy2DTransView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CMy2DTransView::OnFileNew)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy2DTransView::OnFilePrintPreview)
	
	// ���콺 ��ɿ� ���� �޽��� ��
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMy2DTransView ����/�Ҹ�
CMy2DTransView::CMy2DTransView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	nflag = 0;
	nElements = 0;
}

CMy2DTransView::~CMy2DTransView()
{
}

BOOL CMy2DTransView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CMy2DTransView �׸���

void CMy2DTransView::OnDraw(CDC* pDC)
{
	CMy2DTransDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	// client ���� ����
    CRect rcClient;
	GetClientRect(rcClient);
 
	COLORREF crBack = RGB(255, 255, 255);	// ���� ȭ���� �� ����.
    CBrush jbrBack;
	jbrBack.CreateSolidBrush(crBack);		// ������ ������ �귯�� ����.
	pDC->FillRect(rcClient, &jbrBack);		// �귯�÷� Ŭ���̾�Ʈ ������ ä��.
    pDC->SetBkColor(crBack);				// ������ ����ȭ�� ������ ����.
	
	//**************************************************************************//
	// �� �������� �ִ� �ּҰ��� �о�鿩 �����ϸ��� �ϴ� �κ�                          //
	//**************************************************************************//

	// nElements�� 0�� �ƴ� ��쿡��
	if (nElements != 0) {
       	int WIDTH = rcClient.Width();				// Ŭ���̾�Ʈ ������ ���� �о��
	    int HEIGHT = rcClient.Height();				// Ŭ���̾�Ʈ ������ ���̸� �о��
	    cen_x = (int)(WIDTH/2);						// Ŭ���̾�Ʈ ������ �߾��� x ��ǥ
	    cen_y = (int)(HEIGHT/2);					// Ŭ���̾�Ʈ ������ �߾��� y ��ǥ

		/* �о���� �����ͷκ��� �߽����� ��� */
        wsx = (MaxX - MinX) / 2;
		wsy = (MaxY - MinY) / 2;
		CenX = MinX + wsx;
		CenY = MinY + wsy;

		// �������� �����. �������� �����ش� min(x, y)�� �����
		Scale = (double)(0.9 * min(WIDTH/(wsx*2), HEIGHT/(wsy*2)));

		/* ����� �߽����� �����Ͽ� ���߾� ��ȯ */
		CenX = CenX * Scale;
		CenY = CenY * Scale;

		//**************************************************************************//
        // DList�� tempList�� �����ϴ� �κ�
    	//**************************************************************************//
		// tempList�� �ʱ�ȭ�ϰ� DList�κ��� �����͸� ������
		tempList.clear();
		tempList.assign(DList.begin(), DList.end());


		// ���� �����Ϸ� �� �����͸� �ٽ� ���
		for(vector<DisplayList>::iterator j = tempList.begin(); j != tempList.end(); ++j) {
			// X ��ǥ ���
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


// CMy2DTransView �μ�
void CMy2DTransView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMy2DTransView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CMy2DTransView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CMy2DTransView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}

void CMy2DTransView::OnMouseMove(UINT nFlags, CPoint point) {
	// TODO: ���콺 �̵��� ���� ǥ���ٿ� ��ǥ�� ����մϴ�.
	CMainFrame* pmf = (CMainFrame*)AfxGetMainWnd();
	CString status;

	status.Format(_T("X ��ǥ : %ld / Y ��ǥ : %ld"), point.x, point.y);
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


// CMy2DTransView ����
#ifdef _DEBUG
void CMy2DTransView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2DTransView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2DTransDoc* CMy2DTransView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2DTransDoc)));
	return (CMy2DTransDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy2DTransView �޽��� ó����
// 1. ������ �� ��� �޼��� ó��
void CMy2DTransView::OnFileOpen() {
    // TODO: Add your command handler code here
	TCHAR szFilter[] = _T("DAT File (.dat)|*.dat|OUT File (.out)|*.out|All File (.*)|*.*||");

	CFileDialog m_FileOpenDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_EXPLORER, szFilter);
	m_FileOpenDialog.m_ofn.lpstrTitle = _T("������ ���� ����");

	if( m_FileOpenDialog.DoModal() == IDOK ) {
		CWnd* pWnd = AfxGetMainWnd();
		pWnd->SetWindowText( (LPCTSTR)m_FileOpenDialog.GetFileTitle() );
		FileRead( m_FileOpenDialog.GetPathName() );
	} 
	
	if( m_FileOpenDialog.GetFileName() == _T("\0") ) {
		AfxMessageBox( _T("������ �������� �����̽��ϴ�") );
	}
}

void CMy2DTransView::FileRead(CString FileName) {
    int i, cnt=0;
	TCHAR str[100] = {L'\0'};
    int red = 0, green = 0, blue = 0;       // �⺻ ������ ���������� ����
    MinX = 1.0E6;  MinY = 1.0E6;
    MaxX = 1.0E-6; MaxY = 1.0E-6;
    double x = 0., y = 0.;

    // DList ������ ������ 0�� �ƴ� ��� ��� ���Ҹ� �����ϵ��� �ؾ���
    if(!DList.empty()) { DList.clear(); }

    // ���� �����͸� ����
    FILE *fp;

    // ���Ͽ� ���� �б� ��尡 ������ ��� �޼��� ���
    if ( _tfopen_s(&fp, (LPCTSTR)FileName, _T("r")) != NULL) {
        AfxMessageBox(_T("������ ������ �б� ���� ���� �����ϴ�.\n�ٸ� ���α׷����� ��������� ������ Ȯ���Ͻñ� �ٶ��ϴ�."));
		return;
    }

    // ���� ������ parsing �ϴ� �κ�
	while (_fgetts(str, 100, fp) != NULL) {
		DisplayList tmp;                        // �ӽ� ����Ʈ
			
		// ù�ٿ� "COLOR"�� �ִٸ� R, G, B���� �о����
		if (str[0] == 'C') {
			_stscanf_s (_tcschr(str, '/')+1, _T("%d %d %d"), &red, &green, &blue);
		}
		// ���� COLOR�� ���ٸ� ������ �о���� ��(Ȥ�� �⺻���� ���)�� �Ҵ�ǰų�
		// �ι�° �ٷ� �Ѿ�鼭 ������ �ռ� �о���� �� ������ �Ҵ�ȴ�
		else if (str[0] == 'P') {
			tmp.R = red;
			tmp.G = green;
			tmp.B = blue;

			// "POLYGON /" ���Ŀ��� ����� ������ ���Եȴ�.
			_stscanf_s (_tcschr(str, '/')+1, _T("%d"), &tmp.nNodes);

			// �� �����͸� �о���̴� �κ�
			for (i = 0; i < tmp.nNodes; ++i) {
				_ftscanf_s(fp, _T("%lf %lf"), &x, &y);
				tmp.XPos.push_back(x);
				tmp.YPos.push_back(y);

				if (x < MinX) MinX = x;
				if (y < MinY) MinY = y;
				if (x > MaxX) MaxX = x;
				if (y > MaxY) MaxY = y;
			}
			// tmp�� DList�� ������ ���ҷ� ����
			DList.push_back(tmp);
			nElements++;
		}
	}
	
	// ���� element�� ������ DList�� �������� (size_type�� int�� ĳ����)
	nElements = (int)DList.size();

	// ������ ����
	fclose(fp);
    
	// �о���� �����ͷκ��� document�� ������ �ݿ�
	CWnd* pWnd = AfxGetMainWnd();
	pWnd->RedrawWindow();
}


// ������ �׸��� ���� ���� �κ�
void CMy2DTransView::DrawLines() {
	// nElements�� 0�� �ƴ� ��쿡��
	if (nElements != 0) {
		CClientDC dc(this);

		//dc.SetMapMode(MM_TEXT | MM_ISOTROPIC);
		// pDC->SetWindowExt(rcClient.Width(), rcClient.Height());
		// pDC->SetViewportExt(rcClient.Width(), rcClient.Height());
		//dc.SetViewportOrg( -int(MinX * Scale), -int(MinY * Scale) );

		for(vector<DisplayList>::iterator iterPos = tempList.begin(); iterPos != tempList.end(); ++iterPos) {
			// ���� �Ӽ� �� ���� ����
			CPen NewPen(PS_SOLID, 1, RGB(iterPos->R, iterPos->G, iterPos->B));
			dc.SelectObject(&NewPen);

			// i��° element�� �� �����͸� �̿��ؼ� ���� �׸�
			for(int i = 0; i < iterPos->nNodes - 1; i++) {
				dc.MoveTo( (int)(iterPos->XPos.at(i))  , (int)(iterPos->YPos.at(i))   );
				dc.LineTo( (int)(iterPos->XPos.at(i+1)), (int)(iterPos->YPos.at(i+1)) );
			}

			// nElements�� 2�� ��쿡�� �� ������ �ʿ� ����
			if (iterPos->nNodes != 2) {
				dc.MoveTo( (int)(iterPos->XPos.at(iterPos->nNodes - 1)),
					(int)(iterPos->YPos.at(iterPos->nNodes - 1)) );
				dc.LineTo( (int)(iterPos->XPos.at(0)), (int)(iterPos->YPos.at(0)) );
			}
		}
	}

}

// �� ������ ���� ���
void CMy2DTransView::OnFileNew() {
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CClientDC dc(this);						// Ŭ���̾�Ʈ ������ dc�� �о��

	CRect rcClient;							// Ŭ���̾�Ʈ ������ �о�� 
	GetClientRect(rcClient);

	COLORREF crBack;						// ����ȭ���� �� ���� (white)
	CBrush jbrBack; 
	crBack = RGB(255, 255, 255) ;

	jbrBack.CreateSolidBrush(crBack) ;		// ������ ������ ������ ä��
	dc.FillRect(rcClient, &jbrBack) ;
	dc.SetBkColor(crBack) ;

	nElements = 0;							// Element ������ 0���� �����Ͽ� draw ����
	tempList.clear();
	DList.clear();

	
	RedrawWindow();							// ������ �ٽ� �׸����� �� (OnDraw ȣ��)
}

