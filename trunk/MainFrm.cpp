// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "2DTrans.h"

#include "MainFrm.h"
#include "2DTransDoc.h"
#include "2DTransView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)
	ON_COMMAND(ID_DIR_SIZE, &CMainFrame::OnDirSize)
	ON_COMMAND(ID_ROT_ORIGIN_X, &CMainFrame::OnRotOriginX)
	ON_COMMAND(ID_ROT_ORIGIN_Y, &CMainFrame::OnRotOriginY)
	ON_COMMAND(ID_ROT_ANGLE, &CMainFrame::OnRotAngle)
	ON_COMMAND(ID_SCALE_RATIO, &CMainFrame::OnScaleRatio)
	ON_COMMAND(ID_SCALE_DELTA, &CMainFrame::OnScaleDelta)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// 모든 사용자 인터페이스 요소를 그리는 데 사용하는 비주얼 관리자를 설정합니다.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	// 비주얼 관리자에서 사용하는 비주얼 스타일을 설정합니다.
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	CString strTitlePane1;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	//m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 탐색 창이 왼쪽에 만들어지므로 일시적으로 왼쪽에 도킹을 비활성화합니다.
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// "Outlook" 탐색 모음을 만들고 설치합니다.
	if (!CreateOutlookBar(m_wndNavigationBar, ID_VIEW_NAVIGATION, m_wndTree, 250))
	{
		TRACE0("탐색 창을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// Outlook 표시줄이 만들어지고 왼쪽에 도킹할 수 있도록 허용해야 합니다.
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// 직접 추가한 부분
	// 1. 상태 표시줄을 준비 상태로 전환함
	m_wndStatusBar.GetElement(0)->SetText(_T("준비됨"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	ASSERT(bNameValid);

	// 창 이미지를 로드합니다.
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);

	// 초기화 주 단추입니다.
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	bNameValid = strTemp.LoadString(IDS_RIBBON_NEW);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, strTemp, 0, 0));
	bNameValid = strTemp.LoadString(IDS_RIBBON_OPEN);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, strTemp, 1, 1));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE, strTemp, 2, 2));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SAVEAS);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, strTemp, 3, 3));

	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, strTemp, 6, 6);
	pBtnPrint->SetKeys(_T("p"), _T("w"));

	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_LABEL);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonLabel(strTemp));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_QUICK);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_DIRECT, strTemp, 7, 7, TRUE));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_PREVIEW);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, strTemp, 8, 8, TRUE));
	bNameValid = strTemp.LoadString(IDS_RIBBON_PRINT_SETUP);
	ASSERT(bNameValid);
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, strTemp, 11, 11, TRUE));
	pMainPanel->Add(pBtnPrint);
	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));

	bNameValid = strTemp.LoadString(IDS_RIBBON_CLOSE);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, strTemp, 9, 9));

	bNameValid = strTemp.LoadString(IDS_RIBBON_RECENT_DOCS);
	ASSERT(bNameValid);
	pMainPanel->AddRecentFilesList(strTemp);

	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));

// 메뉴 부분 시작

	// "클립보드" 패널을 사용하여 "홈" 범주를 추가합니다.
	bNameValid = strTemp.LoadString(IDS_RIBBON_HOME);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(strTemp, IDB_WRITESMALL, IDB_WRITELARGE);
	pCategoryHome->SetKeys(_T("H"));

	// "클립보드" 패널을 만듭니다.
	bNameValid = strTemp.LoadString(IDS_RIBBON_CLIPBOARD);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelClipboard = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon(27));

	bNameValid = strTemp.LoadString(IDS_RIBBON_PASTE);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnPaste = new CMFCRibbonButton(ID_EDIT_PASTE, strTemp, 0, 0);
	pPanelClipboard->Add(pBtnPaste);

	bNameValid = strTemp.LoadString(IDS_RIBBON_CUT);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_CUT, strTemp, 1));
	bNameValid = strTemp.LoadString(IDS_RIBBON_COPY);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_COPY, strTemp, 2));
	bNameValid = strTemp.LoadString(IDS_RIBBON_SELECTALL);
	ASSERT(bNameValid);
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_SELECT_ALL, strTemp, -1));

	// "보기" 패널을 만들고 추가합니다.
	bNameValid = strTemp.LoadString(IDS_RIBBON_VIEW);
	ASSERT(bNameValid);
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(strTemp, m_PanelImages.ExtractIcon (7));

	bNameValid = strTemp.LoadString(IDS_RIBBON_STATUSBAR);
	ASSERT(bNameValid);
	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, strTemp);
	pPanelView->Add(pBtnStatusBar);

//////////////////////////////
// 직접 추가 메뉴 부분 시작 //
//////////////////////////////
	// 1. 이동 관련 메뉴
	// "이동" 범주 추가
	bNameValid = strTemp.LoadString(IDS_RIBBON_CATEGORY_TRANSLATE);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryTranslate = m_wndRibbonBar.AddCategory(strTemp, IDB_TRANSMALL, IDB_TRANLARGE);
	pCategoryTranslate->SetKeys(_T("T"));

		// "직선 이동" 패널 추가
		bNameValid = strTemp.LoadString(IDS_RIBBON_PANEL_DIRECTION1);
		ASSERT(bNameValid);
		CMFCRibbonPanel* pPanelDirection1 = pCategoryTranslate->AddPanel(strTemp, m_PanelImages.ExtractIcon(27));

			// "상", "하", "좌", "우"에 해당하는 버튼을 추가
			// 상
			bNameValid = strTemp.LoadString(IDS_RIBBON_DIR_UP);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnDir1 = new CMFCRibbonButton(ID_DIR_UP, strTemp, 4, 4);
			pBtnDir1->SetKeys( _T("U") );
			pPanelDirection1->Add(pBtnDir1);

			// 하
			bNameValid = strTemp.LoadString(IDS_RIBBON_DIR_DOWN);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnDir2 = new CMFCRibbonButton(ID_DIR_DOWN, strTemp, 5, 5);
			pBtnDir2->SetKeys( _T("D") );
			pPanelDirection1->Add(pBtnDir2);

			// 좌
			bNameValid = strTemp.LoadString(IDS_RIBBON_DIR_LEFT);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnDir3 = new CMFCRibbonButton(ID_DIR_LEFT, strTemp, 3, 3);
			pBtnDir3->SetKeys( _T("L") );
			pPanelDirection1->Add(pBtnDir3);

			// 우
			bNameValid = strTemp.LoadString(IDS_RIBBON_DIR_RIGHT);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnDir4 = new CMFCRibbonButton(ID_DIR_RIGHT, strTemp, 2, 2);
			pBtnDir4->SetKeys( _T("R") );
			pPanelDirection1->Add(pBtnDir4);

		// "대각선 이동" 패널 추가
		bNameValid = strTemp.LoadString(IDS_RIBBON_PANEL_DIRECTION2);
		ASSERT(bNameValid);
		CMFCRibbonPanel* pPanelDirection2 = pCategoryTranslate->AddPanel(strTemp, m_PanelImages.ExtractIcon(27));

			// "우측 상향", "우측 하향", "좌측 상향", "좌측 하향"에 해당하는 버튼을 추가
			// 우측 상향
			bNameValid = strTemp.LoadString(IDS_RIBBON_DIR_RUP);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnDir5 = new CMFCRibbonButton(ID_DIR_RUP, strTemp, 8, 8);
			pBtnDir5->SetKeys( _T("Q") );
			pPanelDirection2->Add(pBtnDir5);

			// 우측 하향
			bNameValid = strTemp.LoadString(IDS_RIBBON_DIR_RDOWN);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnDir6 = new CMFCRibbonButton(ID_DIR_RDOWN, strTemp, 7, 7);
			pBtnDir6->SetKeys( _T("E") );
			pPanelDirection2->Add(pBtnDir6);

			// 좌측 상향
			bNameValid = strTemp.LoadString(IDS_RIBBON_DIR_LUP);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnDir7 = new CMFCRibbonButton(ID_DIR_LUP, strTemp, 9, 9);
			pBtnDir7->SetKeys( _T("Z") );
			pPanelDirection2->Add(pBtnDir7);

			// 좌측 하향
			bNameValid = strTemp.LoadString(IDS_RIBBON_DIR_LDOWN);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnDir8 = new CMFCRibbonButton(ID_DIR_LDOWN, strTemp, 6, 6);
			pBtnDir8->SetKeys( _T("C") );
			pPanelDirection2->Add(pBtnDir8);

		// "이동 크기 설정" 패널 추가
		bNameValid = strTemp.LoadString(IDS_RIBBON_PANEL_DIRECTION3);
		ASSERT(bNameValid);
		CMFCRibbonPanel* pPanelDirection3 = pCategoryTranslate->AddPanel(strTemp, m_PanelImages.ExtractIcon(28));

			// 이동 크기를 편집할 수 있는 에디트 콘트롤를 생성함
			CMFCRibbonEdit* pEdit = new CMFCRibbonEdit(ID_DIR_SIZE, 90, _T("Step"), 10);
			pEdit->EnableSpinButtons(0, 1000);			// 0 ~ 1000 사이의 값으로 스핀 버튼
			pEdit->SetEditText( _T("10") );
			pPanelDirection3->Add(pEdit);

	// 2. 회전 관련 메뉴
	// "회전 및 스케일링" 범주 추가
	bNameValid = strTemp.LoadString(IDS_RIBBON_CATEGORY_ROTATE);
	ASSERT(bNameValid);
	CMFCRibbonCategory* pCategoryRotScale = m_wndRibbonBar.AddCategory(strTemp, IDB_ROT_SCALE_SMALL, IDB_ROT_SCALE_LARGE);
	pCategoryRotScale->SetKeys(_T("R"));

		// "회전" 패널 추가
		bNameValid = strTemp.LoadString(IDS_RIBBON_PANEL_ROTATE1);
		ASSERT(bNameValid);
		CMFCRibbonPanel* pPanelRotate1 = pCategoryRotScale->AddPanel(strTemp, m_PanelImages.ExtractIcon(28));

			// 좌, 우로 회전 버튼을 추가
			// 좌로 회전
			bNameValid = strTemp.LoadString(IDS_RIBBON_ROT_LEFT);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnRotLeft = new CMFCRibbonButton(ID_ROT_LEFT, strTemp, 4, 4);
			pBtnRotLeft->SetKeys( _T("Q") );
			pPanelRotate1->Add(pBtnRotLeft);

			// 우로 회전
			bNameValid = strTemp.LoadString(IDS_RIBBON_ROT_RIGHT);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnRotRight = new CMFCRibbonButton(ID_ROT_RIGHT, strTemp, 3, 3);
			pBtnRotRight->SetKeys( _T("E") );
			pPanelRotate1->Add(pBtnRotRight);

		// "회전 관련 파라미터" 패널 추가
		bNameValid = strTemp.LoadString(IDS_RIBBON_PANEL_ROTATE2);
		ASSERT(bNameValid);
		CMFCRibbonPanel* pPanelRotate2 = pCategoryRotScale->AddPanel(strTemp, m_PanelImages.ExtractIcon(28));

			// 회전 중심의 X 좌표를 설정할 수 있는 에디트 콘트롤을 생성함
			CMFCRibbonEdit* pEditOriginX = new CMFCRibbonEdit(ID_ROT_ORIGIN_X, 65,  _T("X Coordinates"), 5);
			pEditOriginX->SetKeys( _T("Z") );
			pEditOriginX->SetEditText( _T("0") );
			pPanelRotate2->Add(pEditOriginX);

			// 회전 중심의 Y 좌표를 설정할 수 있는 에디트 콘트롤을 생성함
			CMFCRibbonEdit* pEditOriginY = new CMFCRibbonEdit(ID_ROT_ORIGIN_Y, 65,  _T("Y Coordinates"), 5);
			pEditOriginY->SetKeys( _T("X") );
			pEditOriginY->SetEditText( _T("0") );
			pPanelRotate2->Add(pEditOriginY);

			// 회전할 각도를 설정할 수 있는 에디트 콘트롤을 생성함
			CMFCRibbonEdit* pEditAngle = new CMFCRibbonEdit(ID_ROT_ANGLE, 65,  _T("Angle(degree)"), 5);
			pEditAngle->SetKeys( _T("R") );
			pEditAngle->SetEditText( _T("10") );
			pPanelRotate2->Add(pEditAngle);

		// "확대 축소" 패널 추가
		bNameValid = strTemp.LoadString(IDS_RIBBON_PANEL_ROTATE3);
		ASSERT(bNameValid);
		CMFCRibbonPanel* pPanelRotate3 = pCategoryRotScale->AddPanel(strTemp, m_PanelImages.ExtractIcon(28));

			// 확대
			bNameValid = strTemp.LoadString(IDS_RIBBON_MAGNIFY);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnMagnify = new CMFCRibbonButton(ID_SCALE_MAGNIFY, strTemp, 1, 1);
			pBtnMagnify->SetKeys( _T("A") );
			pPanelRotate3->Add(pBtnMagnify);

			// 원래 대로
			bNameValid = strTemp.LoadString(IDS_RIBBON_ORIGINAL);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnOriginal = new CMFCRibbonButton(ID_SCALE_ORIGINAL, strTemp, 0, 0);
			pBtnOriginal->SetKeys( _T("S") );
			pPanelRotate3->Add(pBtnOriginal);

			// 축소
			bNameValid = strTemp.LoadString(IDS_RIBBON_SHRINK);
			ASSERT(bNameValid);
			CMFCRibbonButton* pBtnShrink = new CMFCRibbonButton(ID_SCALE_SHRINK, strTemp, 2, 2);
			pBtnShrink->SetKeys( _T("D") );
			pPanelRotate3->Add(pBtnShrink);

		// "확대 및 축소 파라미터" 패널 추가
		bNameValid = strTemp.LoadString(IDS_RIBBON_PANEL_ROTATE4);
		ASSERT(bNameValid);
		CMFCRibbonPanel* pPanelRotate4 = pCategoryRotScale->AddPanel(strTemp, m_PanelImages.ExtractIcon(28));

			// 확대 및 축소 비율을 지정할 수 있는 에디트 콘트롤 추가
			CMFCRibbonEdit* pEditScaleDelta = new CMFCRibbonEdit(ID_SCALE_DELTA, 57,  _T("±Scale"), 5);
			pEditScaleDelta->SetKeys( _T("P") );
			pEditScaleDelta->SetEditText( _T("0.1") );
			pPanelRotate4->Add(pEditScaleDelta);

			// 확대 및 축소 비율을 증가할 값을 설정
			CMFCRibbonEdit* pEditScaleRatio = new CMFCRibbonEdit(ID_SCALE_RATIO, 65,  _T("Scale"), 5);
			pEditScaleRatio->SetKeys( _T("S") );
			pEditScaleRatio->SetEditText( _T("1.0") );
			pPanelRotate4->Add(pEditScaleRatio);

////////////////////////////
// 직접 추가 메뉴 부분 끝 //
////////////////////////////
			
	// 빠른 실행 도구 모음 명령을 추가합니다.
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);
	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("About\na"), m_PanelImages.ExtractIcon(0)));
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, int nInitialWidth)
{
	CWindowDC dc(NULL);

	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // 만들지 못했습니다.
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	DWORD dwPaneStyle = AFX_DEFAULT_TOOLBAR_STYLE | CBRS_FLOAT_MULTI;

	// 움직이거나, 자동으로 숨기거나, 크기를 조정할 수 있지만 닫을 수는 없습니다.
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	tree.Create(dwTreeStyle, rectDummy, &bar, 1200);
	bNameValid = strTemp.LoadString(IDS_FOLDERS);
	ASSERT(bNameValid);
	pOutlookBar->AddControl(&tree, strTemp, 2, TRUE, dwStyle);

	ASSERT(bNameValid);

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

// CMainFrame 진단
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 인쇄 미리 보기 모드를 닫습니다.
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}


// 형상을 이동(translation)하는 크기를 변경한 경우 이를 적용하는 부분
void CMainFrame::OnDirSize()
{
	// FindById 메소드로 해당 에디트 컨트롤의 포인터를 받아옴
	CMFCRibbonEdit* pEdit = (CMFCRibbonEdit*)(m_wndRibbonBar.FindByID(ID_DIR_SIZE));
	CString size = pEdit->GetEditText();
	size.Replace( _T(" "), _T("") );
	
	int count = size.GetLength();
	int i;

	// 형식 검사 int로 변경한 후 같지 않으면 오류 메시지 표시
	for(i = 0; i < count; i++) {
		TCHAR temp = size.GetAt(i);

		// 음수 처리. 
		if( i == 0 && temp == L'-' ) {
			continue;
		}

		// 입력된 키가 0 ~ 9 사이인가를 체크. 
		if( temp >= L'0' && temp <= L'9' ) {
			continue;
		}
		else{
			break;
		}
	} 

	// 만약 모두 수가 맞다면 해당 값을 전달
	if(i == count) {
		// 활성화된 현재 CView 객체에 접근
		CMy2DTransView *pView = (CMy2DTransView *)( this->GetActiveView() );
		
		// DirSize를 전달
		pView->DirSize = _tstoi(size);
		//pView->SetDirSize( _tstoi(size) );
	}
	// 그렇지 않다면 오류 메시지 표시
	else {
		AfxMessageBox( _T("이동 크기는 반드시 정수이어야 합니다.") );
	}	
}

// 회전 중심의 X좌표를 입력하는 콘트롤 Edit의 이벤트 핸들러
void CMainFrame::OnRotOriginX()
{
	// FindById 메소드로 해당 에디트 컨트롤의 포인터를 받아옴
	CMFCRibbonEdit* pEdit = (CMFCRibbonEdit*)(m_wndRibbonBar.FindByID(ID_ROT_ORIGIN_X));
	CString size = pEdit->GetEditText();
	size.Replace( _T(" "), _T("") );
	
	int count = size.GetLength();
	int i;

	// 형식 검사 int로 변경한 후 같지 않으면 오류 메시지 표시
	for(i = 0; i < count; i++) {
		TCHAR temp = size.GetAt(i);

		// 음수 및 소수점 처리 
		if( i == 0 && (temp == L'-' && temp == L'.') ) {
			continue;
		}

		// 입력된 키가 0 ~ 9 사이인가를 체크. 
		if( (temp >= L'0' && temp <= L'9') || (temp == L'.') ) {
			continue;
		}
		else{
			break;
		}
	} 

	// 만약 모두 수가 맞다면 해당 값을 전달
	if(i == count) {
		// 활성화된 현재 CView 객체에 접근
		CMy2DTransView *pView = (CMy2DTransView *)( this->GetActiveView() );
		
		// 회전 중심의 X좌표를 전달
		pView->rotCenterX = _tstof(size);

		// View를 다시 그려줌
		pView->SetCapture();
		pView->RedrawWindow();
		ReleaseCapture();
	}
	// 그렇지 않다면 오류 메시지 표시
	else {
		AfxMessageBox( _T("입력 값이 잘못되었습니다.") );
	}	
}

// 회전 중심의 Y좌표를 입력하는 콘트롤 Edit의 이벤트 핸들러
void CMainFrame::OnRotOriginY()
{
	// FindById 메소드로 해당 에디트 컨트롤의 포인터를 받아옴
	CMFCRibbonEdit* pEdit = (CMFCRibbonEdit*)(m_wndRibbonBar.FindByID(ID_ROT_ORIGIN_Y));
	CString size = pEdit->GetEditText();
	size.Replace( _T(" "), _T("") );
	
	int count = size.GetLength();
	int i;

	// 형식 검사 int로 변경한 후 같지 않으면 오류 메시지 표시
	for(i = 0; i < count; i++) {
		TCHAR temp = size.GetAt(i);

		// 음수 및 소수점 처리
		if( i == 0 && (temp == L'-' && temp == L'.') ) {
			continue;
		}

		// 입력된 키L가 0 ~ 9 사이인가를 체크. 
		if( (temp >= L'0' && temp <= L'9') || (temp == L'.') ) {
			continue;
		}
		else{
			break;
		}
	} 

	// 만약 모두 수가 맞다면 해당 값을 전달
	if(i == count) {
		// 활성화된 현재 CView 객체에 접근
		CMy2DTransView *pView = (CMy2DTransView *)( this->GetActiveView() );
		
		// 회전 중심의 Y좌표를 전달
		pView->rotCenterY = _tstof(size);

		// View를 다시 그려줌
		pView->SetCapture();
		pView->RedrawWindow();
		ReleaseCapture();
	}
	// 그렇지 않다면 오류 메시지 표시
	else {
		AfxMessageBox( _T("입력 값이 잘못되었습니다.") );
	}	
}

// 회전 각도를 입력하는 Edit 컨트롤 이벤트 핸들러
void CMainFrame::OnRotAngle()
{
	// FindById 메소드로 해당 에디트 컨트롤의 포인터를 받아옴
	CMFCRibbonEdit* pEdit = (CMFCRibbonEdit*)(m_wndRibbonBar.FindByID(ID_ROT_ANGLE));
	CString size = pEdit->GetEditText();
	size.Replace( _T(" "), _T("") );
	
	int count = size.GetLength();
	int i;

	// 형식 검사 int로 변경한 후 같지 않으면 오류 메시지 표시
	for(i = 0; i < count; i++) {
		TCHAR temp = size.GetAt(i);

		// 음수 및 소수점 처리
		if( i == 0 && (temp == L'-' && temp == L'.') ) {
			continue;
		}

		// 입력된 키가 0 ~ 9 사이인가를 체크. 
		if( (temp >= L'0' && temp <= L'9') || (temp == L'.') ) {
			continue;
		}
		else{
			break;
		}
	} 

	// 만약 모두 수가 맞다면 해당 값을 전달
	if(i == count) {
		// 활성화된 현재 CView 객체에 접근
		CMy2DTransView *pView = (CMy2DTransView *)( this->GetActiveView() );
		
		// 회전 각도를 전달
		pView->rotAngle = _tstof(size);
	}
	// 그렇지 않다면 오류 메시지 표시
	else {
		AfxMessageBox( _T("입력 값이 잘못되었습니다.") );
	}	
}

void CMainFrame::OnScaleRatio()
{
	// FindById 메소드로 해당 에디트 컨트롤의 포인터를 받아옴
	CMFCRibbonEdit* pEdit = (CMFCRibbonEdit*)(m_wndRibbonBar.FindByID(ID_SCALE_RATIO));
	CString size = pEdit->GetEditText();
	size.Replace( _T(" "), _T("") );
	
	int count = size.GetLength();
	int i;

	// 형식 검사 int로 변경한 후 같지 않으면 오류 메시지 표시
	for(i = 0; i < count; i++) {
		TCHAR temp = size.GetAt(i);

		// 음수 및 소수점 처리
		if( i == 0 && (temp == L'-' && temp == L'.') ) {
			continue;
		}

		// 입력된 키가 0 ~ 9 사이인가를 체크. 
		if( (temp >= L'0' && temp <= L'9') || (temp == L'.') ) {
			continue;
		}
		else{
			break;
		}
	} 

	// 만약 모두 수가 맞다면 해당 값을 전달
	if(i == count) {
		// 활성화된 현재 CView 객체에 접근
		CMy2DTransView *pView = (CMy2DTransView *)( this->GetActiveView() );
		
		// 축척 비율을 전달
		pView->Scale = _tstof(size);
		// 스케일이 변경되었음 플래그를 설정
		pView->isScaleRatioCustomized = true;
		// View를 다시 그려줌
		pView->SetCapture();
		pView->RedrawWindow();
		ReleaseCapture();
	}
	// 그렇지 않다면 오류 메시지 표시
	else {
		AfxMessageBox( _T("입력 값이 잘못되었습니다.") );
	}
}

void CMainFrame::OnScaleDelta()
{
	// FindById 메소드로 해당 에디트 컨트롤의 포인터를 받아옴
	CMFCRibbonEdit* pEdit = (CMFCRibbonEdit*)(m_wndRibbonBar.FindByID(ID_SCALE_DELTA));
	CString size = pEdit->GetEditText();
	size.Replace( _T(" "), _T("") );

	int count = size.GetLength();
	int i;

	// 형식 검사 int로 변경한 후 같지 않으면 오류 메시지 표시
	for(i = 0; i < count; i++) {
		TCHAR temp = size.GetAt(i);

		// 음수 및 소수점 처리
		if( i == 0 && (temp == L'-' && temp == L'.') ) {
			continue;
		}

		// 입력된 키가 0 ~ 9 사이인가를 체크. 
		if( (temp >= L'0' && temp <= L'9') || (temp == L'.') ) {
			continue;
		}
		else{
			break;
		}
	} 

	// 만약 모두 수가 맞다면 해당 값을 전달
	if(i == count) {
		// 활성화된 현재 CView 객체에 접근
		CMy2DTransView *pView = (CMy2DTransView *)( this->GetActiveView() );
		
		// 축척 비율을 전달
		pView->delScale = _tstof(size);
	}
	// 그렇지 않다면 오류 메시지 표시
	else {
		AfxMessageBox( _T("입력 값이 잘못되었습니다.") );
	}
}
