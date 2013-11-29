// TPTimeCodeEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPTimeCodeEditNS.h"
#include ".\tptimecodeeditns.h"


//BOOL m_bCopyFlag  = FALSE;
extern int GetFrameFromFrameString(CString strName);

IMPLEMENT_DYNAMIC(CTPTimeCodeEditNS, CTPTimeCodeEditEx)
CTPTimeCodeEditNS::CTPTimeCodeEditNS()
{	
	m_ptDrag     = CPoint(-1,-1);
	m_pLogo      = TP_LoadImage(TP_GetSkinPath() + _T("\\CommonCtrl\\TimeCode\\TimeCodeNSLogo.bmp"),13,RGB(0,0,0));
	m_pLeftLogo  = NULL; 
	m_uLogoPos   = 0; 
	m_pOleDrag   = NULL;
	m_iLeftWidth = TP_TIMECODE_LOGO_WIDTH;
	m_uShowStyleTemp =-1;

}
CTPTimeCodeEditNS::~CTPTimeCodeEditNS()
{	
	TP_ReleaseCommonRes(m_pLogo);
	m_pLogo = NULL;
	TP_ReleaseCommonRes(m_pLeftLogo);
	m_pLeftLogo = NULL;
}
BEGIN_MESSAGE_MAP(CTPTimeCodeEditNS, CTPTimeCodeEditEx)	
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_OLEDROPTARGET_MSG,OnMsgOleDrag)	
END_MESSAGE_MAP()

void     CTPTimeCodeEditNS::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_TPTIMECODEEDITNS;		
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_TPTIMECODEEDITNS,AfxGetInstanceHandle());
}
void CTPTimeCodeEditNS::SubWindow(BOOL bSub)
{
	CTPTimeCodeEditEx::SubWindow(bSub);	
	if(bSub)
	{
		if(m_pOleDrag==NULL) 
		{
			m_pOleDrag = new CTPOleDropTarget();
			m_pOleDrag ->m_pMsgWnd = this;
			m_pOleDrag ->Register(this);
		}
	}
	else
	{
		if(m_pOleDrag)
		{		
			m_pOleDrag ->Revoke();
			delete m_pOleDrag;
			m_pOleDrag = NULL;
		}
	}
}
void CTPTimeCodeEditNS::SetMVMode()
{
	m_uEditStyle &= ~(TP_TIMECODE_NSEDITTYPE|TP_TIMECODE_NSCTLDISABLE|TP_TIMECODE_LOGO);
	m_uEditEnable =  (TP_TIMECODE_NSFTCTYPE|TP_TIMECODE_NSCTLTYPE);
	// liting 2009-10-27 XN00024050
	m_uEditStyle |= (/*TP_TIMECODE_NEGATIVE|*/TP_TIMECODE_EDITTYPEMV |TP_TIMECODE_NEGATIVE_POS|TP_TIMECODE_MENU  |TP_TIMECODE_POSITIVE_SIGN | TP_TIMECODE_CHANGEOPEN);
}
void CTPTimeCodeEditNS::SetTCMode()
{
	m_uEditStyle &= ~(TP_TIMECODE_NSEDITTYPE|TP_TIMECODE_NSCTLDISABLE|TP_TIMECODE_LOGO);
	m_uEditEnable =  (TP_TIMECODE_NSFTCTYPE|TP_TIMECODE_NSCTLTYPE);
	m_uEditStyle |= (TP_TIMECODE_NEGATIVE_POS|TP_TIMECODE_MENU  |TP_TIMECODE_POSITIVE_SIGN | TP_TIMECODE_EDITTYPTIMECODE);
}
void CTPTimeCodeEditNS::SetRecordMode()
{
	m_uEditStyle &= ~(TP_TIMECODE_NSEDITTYPE|TP_TIMECODE_NSCTLDISABLE|TP_TIMECODE_LOGO);
	m_uEditEnable =  (TP_TIMECODE_NSTCTYPE|TP_TIMECODE_NSCTLTYPE);
	m_uEditStyle |= TP_TIMECODE_EDITTYPEREC|TP_TIMECODE_POSITIVE_SIGN|TP_TIMECODE_MENU|TP_TIMECODE_LOGO;
	m_uEditStyle &= ~TP_TIMECODE_NEGATIVE;
}
void CTPTimeCodeEditNS::SetDigMode(BOOL bCTL)
{	  
	m_uEditStyle &= ~(TP_TIMECODE_NSEDITTYPE|TP_TIMECODE_NSCTLDISABLE|TP_TIMECODE_LOGO);

	m_uEditEnable =  (TP_TIMECODE_NSTCTYPE|TP_TIMECODE_NSCTLTYPE);
	m_uEditStyle |= TP_TIMECODE_EDITTYPEDIGITIZE|TP_TIMECODE_POSITIVE_SIGN|TP_TIMECODE_MENU;
	m_uEditStyle &= ~TP_TIMECODE_NEGATIVE;
	/*if(bCTL) m_uEditStyle &= ~TP_TIMECODE_NSCTLDISABLE;
	else     m_uEditStyle |=  TP_TIMECODE_NSCTLDISABLE;*/
}
void CTPTimeCodeEditNS::SetSVMode()
{	
	m_uEditStyle &= ~(TP_TIMECODE_NSEDITTYPE|TP_TIMECODE_NSCTLDISABLE);	
	m_uEditEnable =  (TP_TIMECODE_NSFTCTYPE|TP_TIMECODE_NSCTLTYPE);
	m_uEditStyle |= (TP_TIMECODE_EDITTYPESV | TP_TIMECODE_NEGATIVE_POS|TP_TIMECODE_MENU  | TP_TIMECODE_POSITIVE_SIGN | TP_TIMECODE_CHANGEOPEN);
}
BOOL CTPTimeCodeEditNS::ValidateClipboardData()
{
	if (!OpenClipboard()) 	return FALSE; 	
	CString sText;
	BOOL    ReturnVal = TRUE;
	BOOL    bUnicode  = TRUE;
	HGLOBAL hglbCopy  = GetClipboardData(CF_UNICODETEXT);
	if(hglbCopy == NULL)
	{
		hglbCopy  = GetClipboardData(CF_TEXT);
		bUnicode  = FALSE;
	}
	if(hglbCopy == NULL){CloseClipboard();return  FALSE;}
	LPVOID lptstrCopy = GlobalLock(hglbCopy); 
	if(bUnicode) sText = (LPTSTR)lptstrCopy;
	else         sText = (LPSTR)lptstrCopy;

	int flame=-0x7fffffff;
	int flag=sText.Find(':');
	if(flag==-1)
	{
		flame = GetFrameFromFrameString(sText);
	}
	else 
	{
		flame =TP_GetFrameFromString(m_eVideoType,sText);
	}

	if(flame>=0 && flame<=2159999 )
	{
	}
	else  ReturnVal = FALSE;
	
	GlobalUnlock(hglbCopy); 			
	CloseClipboard(); 
	return ReturnVal;
}
void CTPTimeCodeEditNS::OnRButtonDown(UINT nFlags, CPoint point)
{
	CTPTimeCodeEditEx::OnRButtonDown(nFlags, point);
	if(  m_uEditStyle&TP_TIMECODE_MENU)
	{
		CTPMenuEx menuLoad,*pSubMenu = NULL;
		CTPMenuEx *pSubsMenu=NULL;
		menuLoad.LoadMenu(IDR_EXTRAMENU);
		if((m_uEditStyle & TP_TIMECODE_EDITTYPESV)) 
		{
			pSubMenu  = menuLoad.GetSubMenu(6);
			pSubsMenu = pSubMenu->GetSubMenu(0);
		}
		else if((m_uEditStyle & TP_TIMECODE_EDITTYPEMV)) 		
		{
			pSubMenu = menuLoad.GetSubMenu(2);
			pSubsMenu = pSubMenu->GetSubMenu(0);
		}
		else if((m_uEditStyle & TP_TIMECODE_EDITTYPEDIGITIZE)) 		
		{
			pSubMenu = menuLoad.GetSubMenu(4);
			pSubsMenu=pSubMenu->GetSubMenu(0);
		}	
		else if (m_uEditStyle & TP_TIMECODE_EDITTYPEREC)//RecToVTR的时码弹出菜单，请不要更改。
		{			
			pSubMenu = menuLoad.GetSubMenu(5);	//RecToVTR不支持CTL时码。		
			pSubsMenu = pSubMenu->GetSubMenu(0);
		}
		else if(m_uEditStyle & TP_TIMECODE_EDITTYPEFFECT) 		
		{				
			pSubMenu = menuLoad.GetSubMenu(2);
			pSubsMenu = pSubMenu->GetSubMenu(0);   //xl changed for XN00028624 20100415
		}
		else if((m_uEditStyle & TP_TIMECODE_EDITTYPTIMECODE)) 		
			pSubMenu = menuLoad.GetSubMenu(7);		
		else if (m_uEditStyle & TP_TIMECODE_EDITTYPESPEED)
		{
			pSubMenu = menuLoad.GetSubMenu(10);
			pSubsMenu = pSubMenu->GetSubMenu(0);
		}
		else 
		{
			menuLoad.DestroyMenu();
			ASSERT(0);
			return;
		}

		CString strSkinPath=TP_GetSkinPath() + _L("\\CommonCtrl\\Menu\\MenuCheck2.bmp");
		pSubMenu->SetCheckImg(strSkinPath);
		if(pSubsMenu)pSubsMenu->SetCheckImg(strSkinPath);
		ClientToScreen(&point);

		BOOL  bCopyFlag = m_iCopyFrame >=0; 
		

		if(bCopyFlag == FALSE) pSubMenu ->EnableMenuItem(ID_TIMECODEEDIT_PASTE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
		if((m_uEditStyle & (TP_TIMECODE_EDITTYPESV|TP_TIMECODE_EDITTYPEMV|TP_TIMECODE_EDITTYPEDIGITIZE|TP_TIMECODE_EDITTYPEREC|TP_TIMECODE_EDITTYPESPEED))) 
		{
			if(m_uEditStyle & TP_TIMECODE_NSCTLDISABLE)
			{
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEVIEW_CTL,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_CTLRESET,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
			}		
			if(m_iEditMode == 0)      
				((CMenu*)pSubMenu)->CheckMenuItem(0,MF_BYPOSITION|MF_CHECKED);
			else				
				pSubMenu ->CheckMenuItem(ID_TIMECODEEDIT_FRAMEVIEW,MF_BYCOMMAND|MF_CHECKED);		

			if(( m_uEditStyle& TP_TIMECODE_NSCTLTYPE))
				pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_CTL,MF_BYCOMMAND|MF_CHECKED);
			else if(( m_uEditStyle& TP_TIMECODE_NSLTCTYPE))
				pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_LTC,MF_BYCOMMAND|MF_CHECKED);
			else if(( m_uEditStyle& TP_TIMECODE_NSVITCTYPE))
				pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_VITC,MF_BYCOMMAND|MF_CHECKED);				
			else if(( m_uEditStyle& TP_TIMECODE_NSFTCTYPE))
				pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_FTC,MF_BYCOMMAND|MF_CHECKED);
			else if(( m_uEditStyle& TP_TIMECODE_NSTCTYPE))
				pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_TC,MF_BYCOMMAND|MF_CHECKED);

		if(!(m_uEditEnable& TP_TIMECODE_NSCTLTYPE)) 
			pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_CTLRESET,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);

			if(pSubsMenu)
			{
				if(!(m_uEditEnable & TP_TIMECODE_NSCTLTYPE))
					pSubsMenu ->DeleteMenu(ID_TIMECODEVIEW_CTL,MF_BYCOMMAND);
				if(!(m_uEditEnable & TP_TIMECODE_NSLTCTYPE))
					pSubsMenu ->DeleteMenu(ID_TIMECODEVIEW_LTC,MF_BYCOMMAND);
				if(!(m_uEditEnable & TP_TIMECODE_NSVITCTYPE))
					pSubsMenu ->DeleteMenu(ID_TIMECODEVIEW_VITC,MF_BYCOMMAND);
				if(!(m_uEditEnable & TP_TIMECODE_NSFTCTYPE))
					pSubsMenu ->DeleteMenu(ID_TIMECODEVIEW_FTC,MF_BYCOMMAND);
				if(!(m_uEditEnable & TP_TIMECODE_NSTCTYPE))
					pSubsMenu ->DeleteMenu(ID_TIMECODEVIEW_TC,MF_BYCOMMAND);
			}
			if(m_iabsFrame<0)
			{
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_COPY,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_DELETE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
//				pSubMenu ->EnableMenuItemEx(ID_TIMECODEVIEW_CTL,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
//				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_CTLRESET,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
			}			
			if(m_uEditStyle & TP_TIMECODE_MENU_READ)
			{
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_PASTE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_UNDO,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_DELETE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
			}
		}
		else
		{	
			if(m_iEditMode == 0)      			
				pSubMenu ->CheckMenuItem(ID_TIMECODEEDIT_TIMECODEVIEW,MF_BYCOMMAND|MF_CHECKED);			
			else			
				pSubMenu ->CheckMenuItem(ID_TIMECODEEDIT_FRAMEVIEW,MF_BYCOMMAND|MF_CHECKED);		
			
			if(m_uEditStyle & TP_TIMECODE_EDITTYPTIMECODE)
			{
				//pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_DELETE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_UNDO,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				//pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_PASTE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
			}			
			if(m_iabsFrame<0)
			{
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_COPY,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_DELETE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEVIEW_CTL,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_CTLRESET,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
			}
			if(m_uEditStyle & TP_TIMECODE_MENU_READ)
			{
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_PASTE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_UNDO,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
				pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_DELETE,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
			}
		}

		// liting 2009-07-24 remove XN00022742 
//		if(m_iUnDoFrame <0)
//			pSubMenu ->EnableMenuItemEx(ID_TIMECODEEDIT_UNDO,MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);
		TPHotKeyData   stuTPHotKey_TimeCode[] =
		{
			{0 , ID_TIMECODEEDIT_UNDO  ,                 VK_Z,			TP_CTRL,				  _T("Undo")},
			{0 , ID_TIMECODEEDIT_COPY,                       VK_C,			TP_CTRL,		          _T("Copy")},
			{0 , ID_TIMECODEEDIT_PASTE ,                     VK_V,			TP_CTRL,		          _T("Paste")},
			{0 , ID_TIMECODEEDIT_DELETE ,                    VK_DELETE,			0,		              _T("Delete")},

		};

		TP_AssembledHotKeyControlData(0, NULL, sizeof(stuTPHotKey_TimeCode) / sizeof(TPHotKeyData), stuTPHotKey_TimeCode);
		pSubMenu->SetHotKeyData( stuTPHotKey_TimeCode, sizeof(stuTPHotKey_TimeCode) / sizeof(TPHotKeyData));

		TP_LoadMenuStr(pSubMenu);
		pSubMenu ->TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this);
		menuLoad.DestroyMenu();
	}
}
void CTPTimeCodeEditNS::OnNSMenuCmd(UINT nID)
{
	int tempUndoFrame=(int)m_iabsFrame;
	switch(nID)
	{
	case ID_TIMECODEEDIT_TIMECODEVIEW:
		//if(m_iFrameInPut<0) return;
		if(!(m_uEditStyle&TP_TIMECODE_SHOW_ONLY))if((CWnd*)GetFocus()!=(CWnd*)this) SetFocus();
		SetEditMode(0);
		if((m_uEditStyle&TP_TIMECODE_SHOW_ONLY)) DisableCover();
		GetParent()->SendMessage(WM_TIMECODE_THIS,TP_TIMECODE_FTCVIEW ,(LPARAM)this);
		return;
	case ID_TIMECODEEDIT_FRAMEVIEW:
		//if(m_iFrameInPut<0) return;
		if(!(m_uEditStyle&TP_TIMECODE_SHOW_ONLY))if((CWnd*)GetFocus()!=(CWnd*)this) SetFocus();
		SetEditMode(1);
		if((m_uEditStyle&TP_TIMECODE_SHOW_ONLY)) DisableCover();
		GetParent()->SendMessage(WM_TIMECODE_THIS,TP_TIMECODE_FRAMEVIEW ,(LPARAM)this);
		return;
	case ID_TIMECODEEDIT_UNDO:
		if(m_iUnDoFrame<0) return;
		SetFrame(m_iUnDoFrame);
		m_iUnDoFrame = tempUndoFrame;
		if(m_iUnDoFrame!=m_iabsFrame)
			GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_CHANGE,(LPARAM)this); 

		// liting 2009-07-24 remove XN00022742 
//		m_iUnDoFrame = -1;
		return;
	case ID_TIMECODEEDIT_COPY:
		//if(m_iFrameInPut<0) return;
		m_bCtlBtn   =TRUE;
		//m_bCopyFlag =TRUE;
		CTPTimeCodeEditEx::OnKeyDown(VK_C,NULL,NULL);
		return;
	case ID_TIMECODEEDIT_PASTE:
		//if(m_iFrameInPut<0) return;
		m_bCtlBtn=TRUE;
		CTPTimeCodeEditEx::OnKeyDown(VK_V,NULL,NULL);
		return;
	case ID_TIMECODEEDIT_DELETE:
		if(m_iFrameInPut<0) return;
		m_iUnDoFrame=tempUndoFrame;
		
		if(2 == m_uShowStyle && 0 == m_iEditMode)
		{
			PTSTR pstr=m_strInput.GetBuffer(10);
			pstr[m_iFocusPos]='0';
			pstr[m_iFocusPos-1]='0';
			m_strInput.ReleaseBuffer();
			FunCalcPos(m_iEditMode);
		}
		else
		{
			SetFrame(0);
		}
		TestTheString(m_strInput);
		Invalidate();
		if(m_iUnDoFrame!=m_iabsFrame) 
			GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_CHANGE,(LPARAM)this); 
		return;
	case ID_TIMECODEEDIT_CTLRESET:
		//if(m_iFrameInPut<0) return;
		ResetCTL();
		if(m_bSendChangeMsg)
			m_pMsgWnd->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_RESETCTL ,(LPARAM)this);

		break;
	case  ID_TIMECODEVIEW_CTL:
		//if(m_iFrameInPut<0) return;
		SetCTL(TRUE);
		m_uEditStyle &= ~TP_TIMECODE_NSALLTYPE;
		m_uEditStyle |=  TP_TIMECODE_NSCTLTYPE;
		if(m_bSendChangeMsg)
			m_pMsgWnd->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_CTLVIEW ,(LPARAM)this);
		break;
	case  ID_TIMECODEVIEW_FTC:
		m_uShowStyle = 0;
		SetCTL(FALSE);
		if ((m_uEditStyle & TP_TIMECODE_NSFTCTYPE) && !(m_uEditStyle & TP_TIMECODE_NSFTCTYPE))
			break;
		m_iUnDoFrame = -1;
		m_uEditStyle &= ~TP_TIMECODE_NSALLTYPE;
		m_uEditStyle |=  TP_TIMECODE_NSFTCTYPE;
		m_uPreEditStyle &= ~TP_TIMECODE_NSALLTYPE;
		m_uPreEditStyle |=  TP_TIMECODE_NSFTCTYPE;
		if(m_bSendChangeMsg)	
			m_pMsgWnd->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_FTCVIEW ,(LPARAM)this);
		break;
	case  ID_TIMECODEVIEW_LTC:
		m_uShowStyle = 0;
		SetCTL(FALSE);
		if (m_uEditStyle & TP_TIMECODE_NSLTCTYPE)
			break;
		m_iUnDoFrame = -1;
		m_uEditStyle &= ~TP_TIMECODE_NSALLTYPE;
		m_uEditStyle |=  TP_TIMECODE_NSLTCTYPE;
		m_uPreEditStyle &= ~TP_TIMECODE_NSALLTYPE;
		m_uPreEditStyle |=  TP_TIMECODE_NSLTCTYPE;
		if(m_bSendChangeMsg)
			m_pMsgWnd->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_LTCVIEW ,(LPARAM)this);
		break;
	case  ID_TIMECODEVIEW_VITC:
		m_uShowStyle = 0;
		SetCTL(FALSE);
		if (m_uEditStyle & TP_TIMECODE_NSVITCTYPE)
			break;
		m_iUnDoFrame = -1;
		m_uEditStyle &= ~TP_TIMECODE_NSALLTYPE;
		m_uEditStyle |=  TP_TIMECODE_NSVITCTYPE;
		m_uPreEditStyle &= ~TP_TIMECODE_NSALLTYPE;
		m_uPreEditStyle |=  TP_TIMECODE_NSVITCTYPE;
		if(m_bSendChangeMsg)
			m_pMsgWnd->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_VITCVIEW ,(LPARAM)this);
		break;
	case  ID_TIMECODEVIEW_TC:
		m_uShowStyle = 0;
		SetCTL(FALSE);
		m_uEditStyle &= ~TP_TIMECODE_NSALLTYPE;
		m_uEditStyle |=  TP_TIMECODE_NSTCTYPE;
		if(m_bSendChangeMsg)
			m_pMsgWnd->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_TCVIEW ,(LPARAM)this);
		break;
	default:
		return;
	}
}


void CTPTimeCodeEditNS::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTPTimeCodeEditEx::OnLButtonDown(nFlags, point);
	m_ptDrag = point;
}
void CTPTimeCodeEditNS::OnLButtonUp(UINT nFlags, CPoint point)
{
	CTPWnd::OnLButtonUp(nFlags, point);	
	m_ptDrag = CPoint(-1,-1);
}


void CTPTimeCodeEditNS::PaintDC(CDC *pDC)
{
	if((m_uEditStyle & TP_TIMECODE_NSEDITTYPE))
	{
		CTPTimeCodeEditEx::PaintDC(pDC);
	}
	else
	{	
		CRect rtClient,rtTimeCode;
		GetClientRect(&rtClient);
		GetTimeCodeRect(rtTimeCode);
		CTPTimeCodeEditEx::PaintDC(pDC);
		rtClient.right = rtTimeCode.left;
		pDC->FillSolidRect(rtClient,TP_GetSysColor(m_iBackColor));
		if(m_pLeftLogo &&(m_uEditStyle & TP_TIMECODE_LOGO))
		{
			CRect rtLogo = rtClient;
			rtLogo.left +=3;
			rtLogo.right = rtLogo.left + m_pLeftLogo->Width();
			if(m_uLogoPos == 0)     { rtLogo.top    = rtLogo.bottom - m_pLeftLogo->Height(); rtLogo.OffsetRect(0,-3);}
			else if(m_uLogoPos == 1){ rtLogo.bottom = rtLogo.top + m_pLeftLogo->Height();rtLogo.OffsetRect(0,3);}
			else {rtLogo.top = rtClient.top + (rtClient.Height() - m_pLeftLogo->Height())/2, rtLogo.bottom = rtLogo.top + m_pLeftLogo->Height();}
			TP_SetDIBitsToDevice(pDC,m_pLeftLogo->GetByte(),m_pLeftLogo->Width(),m_pLeftLogo->Height(),rtLogo);
			rtClient.left = rtLogo.right +3;
		}
		else if(m_pLogo &&(m_uEditStyle & TP_TIMECODE_LOGO))
		{
			ImageList_Draw(m_pLogo ->GetImage(),m_bFocus  ? 0:1 ,pDC ->m_hDC,2+(m_bFocus?0:1),(rtClient.Height() - m_pLogo->Height())/2,ILD_TRANSPARENT);
		}
	}
}
void CTPTimeCodeEditNS::OnMouseMove(UINT nFlags, CPoint point)
{
	CTPWnd::OnMouseMove(nFlags, point);	
	if(nFlags & MK_LBUTTON)
	{
		if(m_ptDrag != point && m_iabsFrame !=-1 && m_ptDrag != CPoint(-1,-1))
		{
			TestTheString(m_strInput);
			//if(m_iabsFrame <0) return;00:00:53:14
			m_ptDrag = point;
			HGLOBAL         hGlobal   = NULL;
			COleDataSource  dataDource;
			CRect           rtWnd;
			GetWindowRect(&rtWnd);
			hGlobal = GlobalAlloc(GHND|GMEM_SHARE,1000);
			if(NULL == hGlobal) 	return ;
			m_uEditStyle |= TP_TIMECODE_DRAG;			
			TCHAR * pData = (TCHAR *)GlobalLock(hGlobal);
			lstrcpy(pData,TP_StringFromFrame(m_eVideoType,m_iabsFrame));
			GlobalUnlock(hGlobal);
			dataDource.CacheGlobalData(CF_UNICODETEXT,hGlobal,NULL);
			dataDource.DoDragDrop(DROPEFFECT_COPY,&rtWnd,NULL);
			SetCursor(TP_GetCtrlCursor());		
			if(hGlobal) 
			GlobalFree(hGlobal);			
			dataDource.Empty();
			m_uEditStyle &= ~TP_TIMECODE_DRAG;

		}
	}
	else m_ptDrag = CPoint(-1,-1);
}
LRESULT  CTPTimeCodeEditNS::OnMsgOleDrag(WPARAM wp,LPARAM lp)
{
	TPOleDropData *pOleData = (TPOleDropData *)lp;
	if(wp == 0)
	{
		if((m_uEditStyle & TP_TIMECODE_DRAG))
		{
			if(m_iEditMode==0) 
			{
				m_uShowStyleTemp = m_uShowStyle;
				m_uShowStyle=1;
				FunCalcPos(m_iEditMode,TRUE);
			}
			else { m_iBeginSel=0;m_bDrawFocus=TRUE;}
			Invalidate();
			pOleData ->eRetrun = DROPEFFECT_NONE;
		}
		else
		{	
			if(m_iEditMode==0) 
			{
				m_uShowStyle=1;
				FunCalcPos(m_iEditMode,TRUE);
				m_bDrawFocus=TRUE;
			}
			else { m_iBeginSel=0;m_bDrawFocus=TRUE;FunCalcPos(m_iEditMode,TRUE);}
			if(!pOleData ->pDataObject->IsDataAvailable(CF_UNICODETEXT,NULL))	pOleData ->eRetrun = DROPEFFECT_NONE;
			else                                                                pOleData ->eRetrun = DROPEFFECT_COPY;
			Invalidate();
		}
	}
	else if(wp == 1)
	{
		if((m_uEditStyle & TP_TIMECODE_DRAG)) pOleData ->eRetrun = DROPEFFECT_NONE;
		else
		{		
			if(!pOleData ->pDataObject->IsDataAvailable(CF_UNICODETEXT,NULL))	pOleData ->eRetrun = DROPEFFECT_NONE;
			else                                                                pOleData ->eRetrun = DROPEFFECT_COPY;
		}
	}
	else if(wp == 2)
	{
		if((m_uEditStyle & TP_TIMECODE_DRAG))
		{
			pOleData ->eRetrun = DROPEFFECT_NONE;
			m_iBeginSel		   = 0;
			m_iEndSel		   = 10;
			m_bDrawFocus       = TRUE;
			Invalidate();
	    }
		else
		{		
			if(!pOleData ->pDataObject->IsDataAvailable(CF_UNICODETEXT,NULL))	
			{
				pOleData ->eRetrun = DROPEFFECT_NONE;
			}
			else                                                    
			{		
				HGLOBAL hGlobal   = pOleData ->pDataObject->GetGlobalData(CF_UNICODETEXT,NULL);;
				TCHAR  *lpData     =(TCHAR *)GlobalLock(hGlobal);	
				CString sText      = lpData; 
				GlobalUnlock(hGlobal);	
				int iFrame   = TP_GetFrameFromString(m_eVideoType,sText);
				if(iFrame>= m_lMin && iFrame<=m_lMax)
				{				
					m_iUnDoFrame = m_iabsFrame;
					SetFrame(TP_GetFrameFromString(m_eVideoType,sText));
					pOleData ->eRetrun = DROPEFFECT_COPY;
					if(m_iEditMode==0) 
					{
						m_uShowStyle=0;
						FunCalcPos(m_iEditMode,TRUE);
						m_bDrawFocus=FALSE;
					}

					else { m_iBeginSel= -1;FunCalcPos(m_iEditMode,TRUE);}
					if(m_iUnDoFrame   != m_iabsFrame)
						GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_CHANGE,(LPARAM)this); 
					Invalidate();
				}
				else 
					pOleData ->eRetrun = DROPEFFECT_NONE;
			}
		}
	}
	else if(wp == 3)
	{
		if((m_uEditStyle & TP_TIMECODE_DRAG))
		{
			pOleData ->eRetrun = DROPEFFECT_NONE;
			if(m_iEditMode==0)
			{
				m_uShowStyle = m_uShowStyleTemp;
				if(m_uShowStyle==2)FunCalcPos(m_iEditMode,FALSE);
			}
			else
			{
				m_bDrawFocus =FALSE;

			}
			
			Invalidate();
		}
		else
		{
			if(m_iEditMode==0) 
			{
				m_uShowStyle=0;
				FunCalcPos(m_iEditMode,TRUE);
				m_bDrawFocus=FALSE;
			}

			else { m_iBeginSel= -1;FunCalcPos(m_iEditMode,TRUE);}
			Invalidate();
		}

	}
	return S_OK;
}
void CTPTimeCodeEditNS::SetLeftLogo(CString sLogo,UINT uTPos)
{
	if(m_uEditStyle & TP_TIMECODE_LOGO)
	{
		if(m_pLeftLogo) TP_ReleaseCommonRes(m_pLeftLogo);
		if(sLogo.GetLength()<=0){ m_pLeftLogo = NULL;return;}
		m_pLeftLogo    = TP_LoadBmpByte(sLogo);
		m_uLogoPos     = uTPos;
	}
}

void CTPTimeCodeEditNS::SetLogo(CString sFileName,int iWidth,COLORREF clMask)
{
	if(m_uEditStyle & TP_TIMECODE_LOGO)
	{
		if(m_pLogo) TP_ReleaseCommonRes(m_pLogo);
		m_pLogo    = TP_LoadImage(sFileName,iWidth,clMask);
	}
}
BOOL CTPTimeCodeEditNS::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);
	if(nID>= ID_TIMECODEEDIT_TIMECODEVIEW && nID <=ID_TIMECODEVIEW_MAX){OnNSMenuCmd(nID);return TRUE;}
	else                return __super::OnCommand(wParam, lParam);
}
void CTPTimeCodeEditNS::GetTimeCodeRect(CRect &rtTimeCode)
{
	if((m_uEditStyle & TP_TIMECODE_NSEDITTYPE) ||!(m_uEditStyle & TP_TIMECODE_LOGO) )
	{
		CTPTimeCodeEditEx::GetTimeCodeRect(rtTimeCode);		
	}
	else
	{	
		GetClientRect(&rtTimeCode);
		rtTimeCode.left += m_iLeftWidth;
	}
}
void CTPTimeCodeEditNS::SelectNextType()
{
	UINT uType[5] = {TP_TIMECODE_NSFTCTYPE,TP_TIMECODE_NSLTCTYPE,TP_TIMECODE_NSVITCTYPE,TP_TIMECODE_NSCTLTYPE,TP_TIMECODE_NSTCTYPE};
	UINT uCmd[5]  = {ID_TIMECODEVIEW_FTC,ID_TIMECODEVIEW_LTC,ID_TIMECODEVIEW_VITC,ID_TIMECODEVIEW_CTL,ID_TIMECODEVIEW_TC};
	for(INT l=0;l<5;l++)
	{
        if(m_uEditStyle & uType[l])
		{
			for(INT k=l+1;k<l+5;k++)
			{
				if(m_uEditEnable & uType[k%5])
				{
					OnNSMenuCmd(uCmd[k%5]);
					return;
				}
			}
		}
	}
}
void CTPTimeCodeEditNS::SetTypeEnable(UINT uType,BOOL bMisMatchReset)//lzq:Reset FTC
{

	m_uEditEnable = uType;
	if(!(m_uEditEnable & m_uEditStyle) && bMisMatchReset)
		OnNSMenuCmd(ID_TIMECODEVIEW_FTC);
}
void CTPTimeCodeEditNS::SelectTypeMenu(CWnd *pWnd)
{
	if(pWnd == NULL) return ;
	CTPMenuEx menuLoad,*pSubMenu = NULL;
	CTPMenuEx *pSubsMenu=NULL;
	menuLoad.LoadMenu(IDR_EXTRAMENU);
	pSubMenu = menuLoad.GetSubMenu(6);
	pSubsMenu=pSubMenu->GetSubMenu(0);

	if(pSubsMenu)
	{

		if(( m_uEditStyle& TP_TIMECODE_NSCTLTYPE))
			pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_CTL,MF_BYCOMMAND|MF_CHECKED);
		else if(( m_uEditStyle& TP_TIMECODE_NSLTCTYPE))
			pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_LTC,MF_BYCOMMAND|MF_CHECKED);
		else if(( m_uEditStyle& TP_TIMECODE_NSVITCTYPE))
			pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_VITC,MF_BYCOMMAND|MF_CHECKED);				
		else if(( m_uEditStyle& TP_TIMECODE_NSFTCTYPE))
			pSubMenu ->CheckMenuItemEx(ID_TIMECODEVIEW_FTC,MF_BYCOMMAND|MF_CHECKED);

		if(!(m_uEditEnable & TP_TIMECODE_NSCTLTYPE))
			pSubMenu ->DeleteMenu(ID_TIMECODEVIEW_CTL,MF_BYCOMMAND);
		if(!(m_uEditEnable & TP_TIMECODE_NSLTCTYPE))
			pSubMenu ->DeleteMenu(ID_TIMECODEVIEW_LTC,MF_BYCOMMAND);
		if(!(m_uEditEnable & TP_TIMECODE_NSVITCTYPE))
			pSubMenu ->DeleteMenu(ID_TIMECODEVIEW_VITC,MF_BYCOMMAND);
		if(!(m_uEditEnable & TP_TIMECODE_NSFTCTYPE))
			pSubMenu ->DeleteMenu(ID_TIMECODEVIEW_FTC,MF_BYCOMMAND);

	}
	CPoint point;
	::GetCursorPos(&point);
	pSubsMenu ->TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this);
	menuLoad.DestroyMenu();
}
