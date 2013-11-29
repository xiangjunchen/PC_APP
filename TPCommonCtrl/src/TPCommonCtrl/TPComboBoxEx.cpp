// TPComboBoxEx.cpp : 实现文件
//

#include "stdafx.h"
#include "TPCommonCtrl.h"
#include "TPComboBoxEx.h"
#include ".\tpcomboboxex.h"


// CTPComboBoxEx

IMPLEMENT_DYNAMIC(CTPComboBoxEx, CComboBoxEx)
CTPComboBoxEx::CTPComboBoxEx()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_pEdit         = NULL;
	m_pCombobox     = NULL;
}

CTPComboBoxEx::~CTPComboBoxEx()
{
	SubWindow(FALSE);
}


BEGIN_MESSAGE_MAP(CTPComboBoxEx, CComboBoxEx)
	ON_WM_CREATE()
	ON_WM_DESTROY()
//	ON_WM_CTLCOLOR()
//ON_WM_PAINT()
//ON_WM_NCPAINT()
//ON_WM_ERASEBKGND()
//ON_WM_DRAWITEM()
//ON_WM_PAINT()
ON_CONTROL_REFLECT(CBN_SELCHANGE, OnCbnSelchange)
//ON_CONTROL_REFLECT(CBN_DROPDOWN, OnCbnDropdown)
END_MESSAGE_MAP()



// CTPComboBoxEx 消息处理程序

BOOL CTPComboBoxEx::PreCreateWindow(CREATESTRUCT& cs)
{	
	m_bCreate = TRUE;
	return CComboBoxEx::PreCreateWindow(cs);
}
void CTPComboBoxEx::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CComboBoxEx::PreSubclassWindow();
	if(!m_bCreate)	SubWindow(TRUE);
}

int CTPComboBoxEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBoxEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	SubWindow(TRUE);

	return 0;
}

void CTPComboBoxEx::OnDestroy()
{
	SubWindow(FALSE);
	CComboBoxEx::OnDestroy();
}
void CTPComboBoxEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;		
		m_pCombobox = new CTPComboBox;
		m_pCombobox ->m_pComboBoxEx = this;
		m_pCombobox ->SubclassWindow(GetComboBoxCtrl() ->m_hWnd);
		m_pCombobox ->SetFont(TP_GetSysFont(m_iFontIndex));
		CEdit *pEdit = CComboBoxEx::GetEditCtrl();
		if(pEdit)
		{
			m_pEdit = new  CTPEdit();
			m_pEdit ->SubclassWindow(pEdit ->m_hWnd );
			m_pEdit ->SetFont(TP_GetSysFont(m_iFontIndex));
		}
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;
		m_pCombobox ->UnsubclassWindow();
		delete m_pCombobox;
		m_pCombobox = NULL;
		if(m_pEdit)
		{
			m_pEdit ->UnsubclassWindow();
			delete m_pEdit;
			m_pEdit = NULL;
		}
	}
}

void CTPComboBoxEx::PaintItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC        *pDC         = CDC::FromHandle(lpDrawItemStruct ->hDC);
	CRect       rtItem      = lpDrawItemStruct ->rcItem;
	CImageList *pImageList  = CComboBoxEx::GetImageList();
	int         iImageindex = -1;
	
	TCHAR cMax[100];
	COMBOBOXEXITEM itemAdd;
	itemAdd.mask       = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_INDENT;	
	itemAdd.iItem      = lpDrawItemStruct ->itemID;
	itemAdd.pszText    = cMax;
	itemAdd.cchTextMax = 100;
	if(!CComboBoxEx::GetItem(&itemAdd)) return;	
	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
	if(lpDrawItemStruct ->itemID == -1) return;

	if(rtItem.right > rtItem.left)
	{
		if(!IsWindowEnabled())	
		{
			pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS));			
		}
		else
		{		
			if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&	(lpDrawItemStruct->itemState & ODS_SELECTED))
			{
				pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS));
				if(!(lpDrawItemStruct ->itemData == 2 && m_pEdit))
					pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_HIGHLIGHT));
				iImageindex = itemAdd.iSelectedImage;
			}
			else
			{
				pDC ->SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT));				
				iImageindex = itemAdd.iImage;
			}
		}
		rtItem.DeflateRect(3,0);		
		rtItem.left += itemAdd.iIndent;
		if(pImageList)
		{
			IMAGEINFO imageInfo;
			if(pImageList ->GetImageInfo(iImageindex,&imageInfo) && rtItem.Width() > imageInfo.rcImage.right - imageInfo.rcImage.left)
			{
				pImageList ->Draw(pDC,iImageindex,CPoint(rtItem.left  ,rtItem.top + (rtItem.Height() - (imageInfo.rcImage.bottom - imageInfo.rcImage.top)) /2),ILD_TRANSPARENT);
				rtItem.left += imageInfo.rcImage.right - imageInfo.rcImage.left +3;
			}
			else rtItem.left += rtItem.Width() + 3;
		}
		pDC ->DrawText(itemAdd.pszText,rtItem,DT_LEFT | DT_SINGLELINE|DT_VCENTER|DT_WORD_ELLIPSIS);
	}

}

void CTPComboBoxEx::OnCbnSelchange()
{
	if(m_pEdit)
	{
		TCHAR cMax[100];
		COMBOBOXEXITEM itemAdd;
		itemAdd.mask       = CBEIF_TEXT;	
		itemAdd.iItem      = CComboBoxEx::GetCurSel();
		itemAdd.pszText    = cMax;
		itemAdd.cchTextMax = 100;
		if(!CComboBoxEx::GetItem(&itemAdd)) return;
		m_pEdit ->SetWindowText(itemAdd.pszText);
	}
}

//void CTPComboBoxEx::OnCbnDropdown()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


