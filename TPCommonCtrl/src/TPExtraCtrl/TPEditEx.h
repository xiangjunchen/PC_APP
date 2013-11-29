#pragma once

#define ES_RETURNDIRECT 0x00000001
#define ES_RETURNNOSUND 0x00000002

#define CP_NONASCII4    65009
// CTPEditEx
#define WC_EDITEX          _L("TPEditEx")
class AFX_EXT_CLASS CTPEditEx : public CTPEdit
{
	DECLARE_DYNAMIC(CTPEditEx)

public:
	CTPEditEx();
	virtual ~CTPEditEx();



public:
	UINT                    m_uEditStyle;  
protected:
	CString                 m_sExcludeKey;
	CString                 m_sIncludeKey;
	INT64                   m_lRangeMix;
	INT64                   m_lRangeMax;
	CString                 m_sTextSave; 
	BYTE                    m_bApply;
	BYTE                    m_bSetText;
	BYTE                    m_bCheckText;
	int                     m_lMix;
	int                     m_lMax;
	CTPToolTipCtrl         *m_pToolTips;
	CString                 m_sToolTips;
	UINT_PTR                m_hToopTip;
	CPoint                  m_ptToolTip; 
	CString                 m_sEditText;
	long                    m_lLimitByte;
	UINT                    m_CodePage;
	BOOL                    m_bToolTip;


public:
	void                    SetExcludeKey(CString sKey);  
	void                    SetIncludeKey(CString sKey);  
	void                    SetRange(INT64 lMix,INT64 lMax);
	void                    SetToolTips(CString sToolTips);
	void                    SetPathNameType();
	void                    EnableToolTip(BOOL bTip);
	void                    SetLimitByte(long lByte,UINT CodePage = CP_ACP);
    
	HWND                    UnsubclassWindow();

	BOOL                    SubClassWindow(HWND hWnd);
	BOOL                    CheckString(CString sText);

	static void             RegisterClass(BOOL bRegister);

private:
	long                    GetByte(CString &sText);
	BOOL                    ShowToolTip();
protected:
	virtual void    SubWindow(BOOL bSub);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg int  OnToolHitTest(CPoint point,TOOLINFO* pTI ) const;
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


