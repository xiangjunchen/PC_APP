// TPEditEx.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPEditEx.h"
#include ".\tpeditex.h"

#include "shlwapi.h"

#undef SubclassWindow  //for dx

// CTPEditEx

extern TCHAR g_cDim;
IMPLEMENT_DYNAMIC(CTPEditEx, CTPEdit)
CTPEditEx::CTPEditEx()
{
	m_lRangeMix     = 0;
	m_lRangeMax     = 0;
	m_sExcludeKey   = _L("");
	m_bApply        = TRUE;
	m_bSetText      = FALSE;
	m_bCheckText    = FALSE;
	m_lMix          = 0;
	m_lMax          = 0;
	m_pToolTips     = NULL;
	m_hToopTip      = NULL;
	m_uEditStyle    = 0;
	m_lLimitByte    = 0;
	m_CodePage      = CP_ACP;
	m_bToolTip      = FALSE;
}

CTPEditEx::~CTPEditEx()
{
}
BEGIN_MESSAGE_MAP(CTPEditEx, CTPEdit)
ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0,0xffff, OnToolTipNotify)
ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
ON_WM_KEYDOWN()
ON_WM_TIMER()
END_MESSAGE_MAP()
void     CTPEditEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{	
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_EDIT,&wndClass);		
		wndClass.lpszClassName = WC_EDITEX;
		::RegisterClass(&wndClass);
	}
	else UnregisterClass(WC_EDITEX,AfxGetInstanceHandle());
}
void CTPEditEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		CTPEdit::SubWindow(bSub);
		m_pToolTips = new CTPToolTipCtrl();		
		m_pToolTips->Create(this,TTS_ALWAYSTIP); 
		m_pToolTips->EnableToolTips(FALSE); 
		m_pToolTips->AddTool(this,LPSTR_TEXTCALLBACK,CRect(0,0,0,0),20000);
		m_pToolTips->Activate(FALSE); 
		EnableToolTips(FALSE);
		if(m_uPrivateStyle & TPEDIT_PATHNAME) SetPathNameType();
	}
	else
	{
		CTPEdit::SubWindow(bSub);
		if(m_hToopTip) KillTimer(m_hToopTip);
		m_hToopTip = 0;
		if(m_pToolTips)
		{
			m_pToolTips->DestroyWindow();
			delete m_pToolTips;
			m_pToolTips = NULL;
		}
	}
}
HWND CTPEditEx::UnsubclassWindow()
{
	SubWindow(false);
	return CEdit::UnsubclassWindow();
}
BOOL CTPEditEx::ShowToolTip()
{
	if(m_pToolTips && m_pToolTips ->m_uState != 1)
	{
		//added by lizhiqiao, 在输入光标处显示提示tip 2008.09.23；
		POINT ptCarePos;
		::GetCaretPos(&ptCarePos);	//获得输入光标的位置。	
		ClientToScreen(&ptCarePos);

		CRect rtWnd;
		GetWindowRect(&rtWnd);
		m_pToolTips ->m_uState = 1;
		m_pToolTips ->m_sToolTips = m_sToolTips;

		//m_pToolTips ->SetWindowPos(NULL,ptCarePos.x,rtWnd.bottom,100,100,SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOACTIVATE);	
		//jyn changed  20080924 XN00015993
		m_pToolTips ->SetWindowPos(&wndTop,ptCarePos.x,rtWnd.bottom,100,100,SWP_SHOWWINDOW|SWP_NOACTIVATE);	

		::GetCursorPos(&m_ptToolTip);		
		if(m_hToopTip == NULL) m_hToopTip = SetTimer(1000,1000,NULL);
		//end of added by lizhiqiao, 2008.09.23；
	}		
	return TRUE;
}

BOOL CTPEditEx::PreTranslateMessage(MSG* pMsg)
{
	if(m_bToolTip && m_pToolTips) 
	{
		m_pToolTips ->RelayEvent(pMsg);
		return CTPEdit::PreTranslateMessage(pMsg);
	}		
	else
	{
		if(pMsg ->message == WM_KEYDOWN) 
			GetSel(m_lMix,m_lMax);
		if((pMsg->hwnd == m_hWnd && pMsg ->message == WM_KEYDOWN || pMsg ->message == WM_KEYUP)  && (m_uEditStyle& ES_RETURNNOSUND)  && pMsg ->wParam == VK_RETURN )
		{
			if(pMsg ->message == WM_KEYUP)
				GetParent()->PostMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_RETURN,(LPARAM)this); 	// TODO: Add your message handler code here and/or call default
			return TRUE;
		}
		else if(pMsg ->message == WM_CHAR && pMsg->hwnd == m_hWnd && m_sExcludeKey.Find((TCHAR)pMsg->wParam)>=0) 
		{
			ShowToolTip();
			return TRUE;	
		}
		else if(pMsg ->message == WM_CHAR && pMsg->hwnd == m_hWnd && m_sIncludeKey .GetLength() >0 && m_sIncludeKey.Find((TCHAR)pMsg->wParam)<0 && ((TCHAR)pMsg->wParam != VK_BACK && (TCHAR)pMsg->wParam != VK_RETURN))
		{
			if(TP_IsCtrlKeyDown())// lihuili 2009-03-17 如果是控制键，交系统处理
				return CEdit::PreTranslateMessage(pMsg);

			if(m_pToolTips) 
			{
				//zhaofei 
				POINT ptCarePos;
				::GetCaretPos(&ptCarePos);		
				ClientToScreen(&ptCarePos);
				CRect rtWnd;
				GetWindowRect(&rtWnd);
				m_pToolTips ->m_uState = 2;
				m_pToolTips ->m_sToolTips = m_sToolTips;
				m_pToolTips ->SetWindowPos(&wndTop,ptCarePos.x,rtWnd.bottom,100,100,SWP_SHOWWINDOW|SWP_NOACTIVATE);	
				::GetCursorPos(&m_ptToolTip);		
				if(m_hToopTip == NULL) m_hToopTip = SetTimer(1000,1000,NULL);
			}
			return TRUE;	
		}
		else if(pMsg ->message == WM_CHAR)
		{
			if(m_pToolTips ->m_uState != 0)
			{
				m_pToolTips ->m_uState = 0;	
				m_pToolTips ->ShowWindow(SW_HIDE);
				if(m_hToopTip) KillTimer(m_hToopTip);
				m_hToopTip = NULL;
			}
			GetSel(m_lMix,m_lMax);
			CString sText;
			GetWindowText(sText);

			int iText = GetLimitText();
			if((m_uPrivateStyle & TPEDIT_DECIMAL) && pMsg ->wParam == '.')  pMsg ->wParam  = g_cDim; 
			if(iText<=sText.GetLength() && pMsg->wParam != VK_BACK)  // modified by Jekkay Hu 2009-05-27 按下退格键不应该发出声音 fix bug XN00021334
			{
			//	TP_WarningMessageBeep(1);
			}
			return CEdit::PreTranslateMessage(pMsg);	
		}		
		else
		{
			//	if(m_pToolTips) m_pToolTips ->RelayEvent(pMsg);
			return CTPEdit::PreTranslateMessage(pMsg);
		}
	}
}
void CTPEditEx::SetIncludeKey(CString sKey)
{
	if(sKey.Find(_T("1234567890."))>=0) 
	{
		sKey +=g_cDim;
		m_uPrivateStyle |= TPEDIT_DECIMAL;
	}
	m_sIncludeKey = sKey;
}
void CTPEditEx::SetExcludeKey(CString sKey)
{
	m_sExcludeKey = sKey;
}
void CTPEditEx::SetToolTips(CString sToolTips)
{
	m_sToolTips = sToolTips;
}
void CTPEditEx::SetRange(INT64 lMix,INT64 lMax)
{
	m_lRangeMix     = lMix;
	m_lRangeMax     = lMax;
}
void CTPEditEx::OnEnChange()
{
	if(!m_bApply || m_bSetText) return;
	CString sText;
	GetWindowText(sText);	

	m_bCheckText = FALSE;
	m_sEditText  = sText;
	GetParent() ->PostMessage(WM_TPEDITEX,1,(LPARAM)m_hWnd);
	if((m_uPrivateStyle & TPEDIT_DECIMAL) &&  '.'!= g_cDim)  sText.Replace('.',g_cDim); 
	if(m_lLimitByte> 0 &&  GetByte(sText)>m_lLimitByte && GetByte(m_sTextSave)<=m_lLimitByte)
	{
		m_bApply = FALSE;
		SetWindowText(m_sTextSave);
		SetSel(m_lMix,m_lMax);
		m_bApply = TRUE;			
		return ;
	}
	if(m_sExcludeKey.GetLength()>0)
	{		
		for(INT l=0;l<sText.GetLength();l++)
		{
			for(int k=0;k<m_sExcludeKey.GetLength();k++)
			{
				if(m_sExcludeKey[k] == sText[l])
				{
					m_bApply = FALSE;
					SetWindowText(m_sTextSave);
					SetSel(m_lMix,m_lMax);
					m_bApply = TRUE;		
					m_bCheckText = TRUE;
					return ;
				}
			}
		}		
	}
	if(m_lRangeMix<m_lRangeMax)
	{		
		BOOL bReset = FALSE;
		while(sText.GetLength()>1 && sText[0] == '0' && sText[1]!='.')//added by lizhiqiao;
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		if(bReset)
		{
			m_bApply = FALSE;
			if(m_sTextSave.GetLength()>sText.GetLength() && sText.GetLength()>0)
				SetWindowText(sText);
			else 
				SetWindowText(m_sTextSave);
			SetSel(m_lMix,m_lMax);
			m_bApply = TRUE;			
			return;
		}
		long lValue = StrToInt(sText);
		if(lValue<m_lRangeMix || lValue >m_lRangeMax)
		{
			if(sText.GetLength()>0)
			{
				sText = sText.Left(sText.GetLength()-1);
				lValue = StrToInt(sText);
				if(lValue<m_lRangeMix || lValue >m_lRangeMax)
				{
					m_bApply = FALSE;
					SetWindowText(m_sTextSave);
					SetSel(m_lMix,m_lMax);
					m_bApply = TRUE;
					if(m_pToolTips) m_pToolTips ->m_uState = 3;
					return;
				}
				else
				{
					m_bApply = FALSE;
					SetWindowText(sText);
					SetSel(m_lMix,m_lMax);
					m_bApply = TRUE;
					return;
				}
			}
			else
			{			
				m_bApply = FALSE;
				SetWindowText(m_sTextSave);
				SetSel(m_lMix,m_lMax);
				m_bApply = TRUE;
				return;
			}
		}	
	}
	m_sTextSave = sText;
}

BOOL CTPEditEx::SubClassWindow(HWND hWnd)
{
	return CWnd::SubclassWindow(hWnd);
}
void CTPEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CTPEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CTPEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_RETURN  && (m_uPrivateStyle & ES_WANTRETURN) && ((m_uEditStyle& ES_RETURNDIRECT) || !TP_IsCtrlKeyDown() && !TP_IsShiftKeyDown() && !TP_IsAltKeyDown()))
		GetParent()->PostMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_RETURN,(LPARAM)this); 	// TODO: Add your message handler code here and/or call default

	CTPEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
long  CTPEditEx::GetByte(CString &sText)
{
	int iMultByteNum = 0;
	if(CP_NONASCII4 == m_CodePage)
	{
        #ifdef	_UNICODE
		for(int i=0;i<sText.GetLength();i++)
		{
			TCHAR tChar = sText.GetAt(i);
			if(tChar>=0 && tChar<=255) iMultByteNum +=1;
			else                       iMultByteNum +=4;
		}
        #else
		iMultByteNum = sText.GetLength();
        #endif
	}
	else
	{
		iMultByteNum = WideCharToMultiByte( m_CodePage, 0, (LPTSTR)(LPCTSTR)sText, -1,NULL, 0, NULL, NULL);
	}
	
	return (iMultByteNum-1);
}
void  CTPEditEx::SetLimitByte(long lByte,UINT CodePage)
{
	m_lLimitByte = lByte;
	m_CodePage   = CodePage;
}
BOOL  CTPEditEx::CheckString(CString sText)
{
	if(m_sExcludeKey.GetLength()>0)
	{		
		for(INT l=0;l<sText.GetLength();l++)
		{
			for(int k=0;k<m_sExcludeKey.GetLength();k++)
			{
				if(m_sExcludeKey[k] == sText[l])
				{
					return FALSE;
				}
			}
		}		
	}
	return TRUE;
}
void CTPEditEx::EnableToolTip(BOOL bTip)
{
	m_bToolTip = bTip;
	if(m_pToolTips)
	m_pToolTips->EnableToolTips(bTip); 	
	EnableToolTips(bTip);

}
void  CTPEditEx::SetPathNameType()
{
    CString sInvalidChar = WC_NAMEEXCLUDE;
 	SetExcludeKey(sInvalidChar);
	CString sKeyNoteTip = _T("Invalid Char    ");// // TP_LoadStr(_T("非法字符")) ;
	sKeyNoteTip += sInvalidChar;
	SetToolTips(sKeyNoteTip);
}
void CTPEditEx::OnTimer(UINT nIDEvent)
{
	if(m_hToopTip == nIDEvent)
	{
		CPoint ptToolTip;
		::GetCursorPos(&ptToolTip);
		if(GetFocus() != this || ptToolTip != m_ptToolTip)
		{
			KillTimer(m_hToopTip);
			m_hToopTip = NULL;
			if(m_pToolTips)
			{			
				m_pToolTips ->m_uState = 0;	
				m_pToolTips ->ShowWindow(SW_HIDE);
			}
		}
	}
	CTPEdit::OnTimer(nIDEvent);
}

LRESULT _tp_extractrl::CTPEditEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_SETTEXT)
	{
		m_bSetText = TRUE;
		LRESULT hResult =  CTPEdit::DefWindowProc(message, wParam, lParam);
		m_bSetText = FALSE;
		m_sTextSave = (LPCTSTR)lParam;
		return hResult;
	}	 	 
	else if(WM_PASTE ==message)
	{
		LRESULT hResult =  CTPEdit::DefWindowProc(message, wParam, lParam); 
		if(m_bCheckText)	ShowToolTip();			 
		return hResult;
	}
	else return CTPEdit::DefWindowProc(message, wParam, lParam);
}
int    _tp_extractrl::CTPEditEx::OnToolHitTest(CPoint point,TOOLINFO* pTI ) const
{
	if(m_bToolTip)
	{	
		pTI->hwnd     = m_hWnd;
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->uId      = (UINT)(UINT_PTR)m_hWnd;
		pTI->uFlags  |= TTF_IDISHWND;
		return  GetDlgCtrlID();
	}
	else
		return CEdit::OnToolHitTest(point,pTI);		 
}
BOOL _tp_extractrl::CTPEditEx::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	if(m_bToolTip)
	{
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
		lstrcpyn( pTTT->szText,m_sTextSave,80);
		m_pToolTips->m_sToolTips = pTTT->szText;
		return  TRUE;
	}	
	else 
		return FALSE;
}