#pragma once

#define   TP_COLOUM_NULL	           0x00000000
#define   TP_COLOUM_PROGRESS           0x00000001
#define   TP_COLOUM_EDIT               0x00000002
#define   TP_COLOUM_EDITEX             0x00000004
#define   TP_COLOUM_TIMECODEEDIT       0x00000008
#define   TP_COLOUM_COMBOBOX           0x00000010
#define   TP_COLOUM_CUSTOM             0x00000020
#define   TP_COLOUM_EDITPATH           0x00000040//compile mxf edit path jyn added 20081124  DQAdb00002810



#define   TP_BEGINLABELEDIT            0x00000001
#define   TP_ENDLABELEDIT              0x00000002
#define   TP_GETCUSTOMWND              0x00000004
#define   TP_DESTORYCUSTOMWND          0x00000008

#define   WM_EDITLISTCTRL              WM_USER+123
#define   WM_NM_CLICK			 	   WM_USER+126  //2007-05-29

// CTPEditListCtrl
#define   WC_EDITLISTCTRL  _L("TPEditListCtrl")
class AFX_EXT_CLASS  CTPEditListCtrl : public CTPListCtrl
{
	DECLARE_DYNAMIC(CTPEditListCtrl)

public:
	CTPEditListCtrl();
	virtual ~CTPEditListCtrl();
public:
	BOOL            m_bCanEdit;
protected:
	CWnd           *m_pMsgWnd;
private:
	DWORD           m_dwColEdit[100];
	TPCommonRes    *m_pThumbBmp;
	int             m_iItemSave;
	int             m_iItemSubSave;
	int             m_iItemLast;
	int             m_iItemSubLast;
	CRect           m_rtItemSubSave;

	CWnd                *m_pEditWnd;
	CTPEdit             *m_pEdit;
	CTPTimeCodeEditEx   *m_pTimeCodeEdit;	 
	CWnd                *m_pCustomWnd;

public:
	DWORD           GetColumnEditType(int iCol);  //zhaofei   XN00016577
	void            SetColumeEditType(int iCol,DWORD dEditType);
	void            SetItemPos(int iItem,int iSubItem,int iPos);
	void            GetEditItem(int &iItem,int &iSubItem,CWnd *&pWnd);	
	void            EditItem(int iItem,int iSubItem);
protected:
	virtual void    SubWindow(BOOL bSub);
	virtual BOOL    DrawReport(CDC *pDC,int iItem,int iSubItem,CRect &rtSub);

	void            EndEdit();
	void            BeginEdit();
	
	LRESULT         FunNotifyParent(UINT nNotify,int iItem,int iSubItem); 
	LRESULT         OnMessageListCtrl(WPARAM wp,LPARAM lp);	
	LRESULT         OnMessageReName(WPARAM wp,LPARAM lp);
public:
	static  void    RegisterClass(BOOL bRegister);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void     OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void     OnDestroy();
	afx_msg void     OnEnKillFocus();
	afx_msg void     OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void     OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT	 OnMessageTimeCode( WPARAM wp, LPARAM lp);
	virtual BOOL     PreTranslateMessage(MSG* pMsg);
	afx_msg		BOOL  OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

};


