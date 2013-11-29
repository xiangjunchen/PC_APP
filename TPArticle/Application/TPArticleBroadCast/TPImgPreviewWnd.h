#pragma once


// CTPImgPreviewWnd
enum TP_INFOLISTICON_INDEX
{
	TP_INFOLISTICON_UNKONW,
	TP_INFOLISTICON_FLODER,
	TP_INFOLISTICON_USERICON,
	TP_INFOLISTICON_WEIBOPIC,
	TP_INFOLISTICON_MAX,
};

enum TP_IMGPREVIEWICON_INDEX
{
	TP_IMGPREVIEWICON_UNKONW,
	TP_IMGPREVIEWICON_CLOSE,
	TP_IMGPREVIEWICON_MAX,
};

class CTPImgPreviewWnd : public CWnd
{
	DECLARE_DYNAMIC(CTPImgPreviewWnd)

public:
	CTPImgPreviewWnd();
	virtual ~CTPImgPreviewWnd();

	BOOL     Create(DWORD uStyle, CWnd *pWnd, UINT nID);
	BOOL     PreviewImg();
private:
	BOOL	GetMoveWindowRect(CRect &rtWnd);
	LRESULT OnMessagePreviewImg(WPARAM wp,LPARAM lp);

private:
	CWnd*	m_pMsgWnd ;
	CSize   m_szDesWndSize ;
	CRect   m_rtCloseBox;

	CPoint  m_ptStartPos;
	CPoint  m_ptCurPos;
	CSize   m_szSrcWndSize;
	CSize   m_szIncrementSize;

	CTPBaseThread m_stuUpdateThread;
	static  TPCommonRes  *m_pCommonRes[TP_IMGPREVIEWICON_MAX];
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


