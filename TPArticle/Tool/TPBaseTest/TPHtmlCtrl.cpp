// TPHtmlCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TPHtmlCtrl.h"


// CTPHtmlCtrl

IMPLEMENT_DYNCREATE(CTPHtmlCtrl, CHtmlView)

CTPHtmlCtrl::CTPHtmlCtrl()
{

}

CTPHtmlCtrl::~CTPHtmlCtrl()
{
}

void CTPHtmlCtrl::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTPHtmlCtrl, CHtmlView)
END_MESSAGE_MAP()


// CTPHtmlCtrl diagnostics

#ifdef _DEBUG
void CTPHtmlCtrl::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CTPHtmlCtrl::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG


// CTPHtmlCtrl message handlers
