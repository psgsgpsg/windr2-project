// 2DTransDoc.h : CMy2DTransDoc Ŭ������ �������̽�
//
#pragma once


class CMy2DTransDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMy2DTransDoc();
	DECLARE_DYNCREATE(CMy2DTransDoc)
	int m_flag;

// Ư���Դϴ�.
public:
	void GetMode(int);
	int PutMode();
	int count;
	double valTT_X, valTT_Y;//translation
	double valST_X, valST_Y;//SCALE
	double valROT_A, valROT_X, valROT_Y,valROT_Ori_X,valROT_Ori_Y;//ROTATION
	int wsx, wsy;
	int CenX, CenY;
	int width, height;
	double Scale;
	double valSingle_x, valSingle_y;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CMy2DTransDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


