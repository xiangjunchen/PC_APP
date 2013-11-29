#pragma once


// CTPFrameWnd 框架

class AFX_EXT_CLASS CTPFrameWnd : public CWnd,public CTPBaseFrame
{
	DECLARE_DYNCREATE(CTPFrameWnd)
public:
	CTPFrameWnd();           // 动态创建所使用的受保护的构造函数
	virtual ~CTPFrameWnd();
public:
	CWnd     *m_pAttachWnd; 

public:
	BOOL     (*m_pTranslateMessage)(MSG* pMsg,LPARAM lParam);
	LPVOID     m_pTranslateMessageParam;

public:
	BOOL      Create(LPCTSTR lpClassName, LPCTSTR lpszWindowName,DWORD dwStyle, const RECT& rect, CWnd* pParentWnd = NULL, UINT nID = 0);
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};


