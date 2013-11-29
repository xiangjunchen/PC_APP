#pragma once

#define  WC_ICONLISTCTRLEX _L("IconListCtrl")

typedef struct _tagTPIconListItem
{
	LPBYTE  pIcon;
	CSize   szIcon;
	CString sText;
	BOOL    bAutoDelete;
	_tagTPIconListItem()
	{
		pIcon        = NULL;
		szIcon       = CSize(0,0);
		sText        = _L("");
		bAutoDelete  = TRUE;
	}
	~_tagTPIconListItem()
	{
		if(bAutoDelete && pIcon)
			delete pIcon;
		pIcon        = NULL;
		szIcon       = CSize(0,0);
	}

}TPIconListItem;

// CTPIconListCtrl
#include "TPListCtrlEx.h"
class AFX_EXT_CLASS CTPIconListCtrl : public CTPListCtrlEx
{
	DECLARE_DYNAMIC(CTPIconListCtrl)

public:
	CTPIconListCtrl();
	virtual ~CTPIconListCtrl();
public:
	BOOL          m_bHorz;
	CSize         m_szIcon;
	CSize         m_szDistance;
	BOOL          m_bText; 
public:
    int  InsertItem(int nItem, TPIconListItem &stuItem);
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();

protected:
	virtual void    SubWindow(BOOL bSub);
	virtual void    DrawIcon(CDC *pDC,int iItem);	

	virtual void    FunCalcItemSize();	
	virtual void    FunCalcScrollBar();
public:
	static  void    RegisterClass(BOOL bRegister);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


