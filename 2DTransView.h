// 2DTransView.h : CMy2DTransView Ŭ������ �������̽�
//

#pragma once

#define MAXELEMENTS         100		// �ִ� ������Ʈ ����
#define MAXNODESPERELEMENT  4		// ������Ʈ�� �ִ� �� ����

class CMy2DTransView : public CView
{
protected: // serialization������ ��������ϴ�.
	CMy2DTransView();
	DECLARE_DYNCREATE(CMy2DTransView)

// Ư���Դϴ�.
public:
	CMy2DTransDoc* GetDocument() const;

// ���� �߰��� ��� �Լ� �� ���� ����Ʈ
public:
	afx_msg void OnFileOpen();			// ������ ���� ���� �Լ� ó��
	afx_msg void OnFileNew();
	void FileRead(CString);				// ���� parsing�� ���� ó��
	void DrawLines();					// ���� �׸��� �Լ�

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CMy2DTransView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 2DTransView.cpp�� ����� ����
inline CMy2DTransDoc* CMy2DTransView::GetDocument() const
   { return reinterpret_cast<CMy2DTransDoc*>(m_pDocument); }
#endif

