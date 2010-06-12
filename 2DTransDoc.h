// 2DTransDoc.h : CMy2DTransDoc 클래스의 인터페이스
//
#pragma once


class CMy2DTransDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CMy2DTransDoc();
	DECLARE_DYNCREATE(CMy2DTransDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현입니다.
public:
	virtual ~CMy2DTransDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};


