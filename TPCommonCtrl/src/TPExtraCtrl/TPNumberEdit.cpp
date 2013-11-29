// TPNumberEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include "TPNumberEdit.h"
#include ".\tpnumberedit.h"
#include <math.h>

// CTPNumberEditExEx

extern TCHAR g_cDim; 


IMPLEMENT_DYNAMIC(CTPNumberEditExEx, CTPEditEx)
CTPNumberEditExEx::CTPNumberEditExEx()
{
	m_pSpinButton  = NULL;
	m_bCheckInfo   = TRUE;
	m_dMax         = 100; 
	m_dMin         = 0;
	m_lDecimals    = 0; 
	m_dDoubleSave  = 0.0;
	m_iAccel       = 1;
	m_bDouble      = FALSE; 
	m_iAccelCount  = 0;
	m_lStep        = 1;
	m_dStep        = 0.1;
	m_dwLastError  = 0;
}

CTPNumberEditExEx::~CTPNumberEditExEx()
{
	SubWindow(FALSE);
}


BEGIN_MESSAGE_MAP(CTPNumberEditExEx, CTPEditEx)
	ON_MESSAGE(WM_UDNDELTAPOS,OnSpinMessage)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnEnChange)
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void     CTPNumberEditExEx::RegisterClass(BOOL bRegister,HINSTANCE hInstance)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(hInstance,WC_EDIT,&wndClass);		
		wndClass.lpszClassName = WC_TPNUMBEREDIT;		
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_TPNUMBEREDIT,hInstance);
}

// CTPNumberEditExEx 消息处理程序
void     CTPNumberEditExEx::SubWindow(BOOL bSub)
{
	CTPEditEx::SubWindow(bSub);
	if(bSub)
	{
		UDACCEL  uSet;
	//	CTPEditEx::SetIncludeKey(_T("-01234567890"));
		m_pSpinButton = new CTPSpinButtonCtrl();
		m_pSpinButton ->m_bForceBud = FALSE;			 
		m_pSpinButton ->Create(UDS_ARROWKEYS|UDS_ALIGNRIGHT|WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),GetParent(),1000);	
		m_pSpinButton ->SetBuddy(this);
		m_pSpinButton ->SetRange32(0,100);
		m_pSpinButton ->GetAccel(1,&uSet);
		uSet.nSec = 600;uSet.nInc=1;
		m_pSpinButton ->SetAccel(1,&uSet);
		m_pSpinButton ->GetAccel(1,&uSet);
		m_pSpinButton ->m_pMsgDelPosWnd = this;
		SetWindowText(_T(""));
		SetRange(-0.01,100.0);
		SetDecimals(1);
	}
	else
	{
		if(m_pSpinButton) 
		{
			m_pSpinButton ->DestroyWindow();
			delete m_pSpinButton;
			m_pSpinButton = NULL;
		}
	}
}

double StrToDouble(CString sText)
{
	int    iDot   = 0;
	double dValue = 0; 
	if(sText.GetLength()<1) return 0;


	for(INT l= 0;l<sText.GetLength();l++)
	{
		if(sText[l] == g_cDim){iDot = l; break;}
	}
	if(iDot == 0) return StrToInt(sText);
	else 
	{
		long lPre   = StrToInt(sText.Left(iDot));
		long lNext  = StrToInt(sText.Mid(iDot+1));
		dValue = abs(lPre) + lNext *1.0/pow(10.0,(sText.GetLength() - iDot-1)); 
		dValue = sText[0] == '-'? -dValue : dValue;
	}
	return dValue;
}

CString DoubleToStr(double dValue,int iDec)
{
	if(dValue >0)      dValue += 0.0000000001;
	else if(dValue <0) dValue -= 0.0000000001;
	CString sText,sFormat;
	sFormat.Format(_T("%%.%df"),iDec);
	sText.Format(sFormat,dValue <= 0? -dValue : dValue);
	if(dValue <0) sText = _T("-") + sText;
	return sText;
}
LRESULT     CTPNumberEditExEx::OnSpinMessage(WPARAM wp,LPARAM lp)
{
	m_iAccelCount ++;
	if(m_iAccelCount <m_iAccel && lp == 2) return S_OK;
	if(lp == 1) return S_OK;
	m_iAccelCount = 0;
	m_bApply = FALSE;
	GetSel(m_lMix,m_lMax);
	if(m_bDouble)
	{
		double iPos = GetPos(),dPos;
		if(wp == 1)      dPos = iPos + m_dStep;
		else if(wp == 2) dPos = iPos - m_dStep;
		dPos = max(m_dMin,dPos);
		dPos = min(m_dMax,dPos);
		CString sText = DoubleToStr(dPos,m_lDecimals);		;
		SetWindowText(sText);	
		SetSel(m_lMix,m_lMax);
	}
	else
	{
		INT64 iPos = GetPos32();
		if(wp == 1)      iPos += m_lStep;
		else if(wp == 2) iPos -= m_lStep;
		iPos = max(m_lRangeMix,iPos);

		iPos = min(m_lRangeMax,iPos);
		CString sText;
		sText.Format(_T("%d"),int(iPos>=0? iPos:-iPos));
		if(iPos<0) sText = _T("-") + sText;
		SetWindowText(sText);	
		SetSel(m_lMix,m_lMax);
	}
	m_bApply = TRUE;	
	GetParent() ->PostMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_EDITCHANGE,(LPARAM)this);
	return S_OK;
}
void     CTPNumberEditExEx::SetRange(double dMin,double dMax)
{
	m_dMin = dMin;
	m_dMax = dMax;
	m_bDouble = TRUE;
	if(m_lDecimals <=0)
	{
		if(m_dMin>=0)		CTPEditEx::SetIncludeKey(_T("0123456789"));
		else        		CTPEditEx::SetIncludeKey(_T("-0123456789"));
	}
	else
	{
		if(m_dMin>=0)		CTPEditEx::SetIncludeKey(_T("0123456789.")  + (CString)g_cDim);
		else 		        CTPEditEx::SetIncludeKey(_T("-0123456789.") + (CString)g_cDim);
	}
	SetToolTips(m_sIncludeKey);
}

void     CTPNumberEditExEx::SetRange32(INT64 lMin,INT64 lMax)
{
	m_lRangeMix = lMin;
	m_lRangeMax = lMax;
	m_bDouble = FALSE;
	if(m_lRangeMix>=0)		CTPEditEx::SetIncludeKey(_T("0123456789")); // XN00028688 Wangjin 20100505
	else        	    	CTPEditEx::SetIncludeKey(_T("-0123456789"));
	SetToolTips(m_sIncludeKey);
}
void   CTPNumberEditExEx::SetPos(double dPos)
{
	dPos = min(dPos,m_dMax); 
	dPos = max(dPos,m_dMin);
	CString sText;
	m_dDoubleSave = 0.0;
	if( m_lDecimals <= 0) sText.Format(_T("%.0f"),dPos);
	else
	{
		CString sDisplayMode;
		sDisplayMode.Format(_T("%%.%df"), m_lDecimals);
		sText.Format( sDisplayMode,dPos);
		m_dDoubleSave = dPos - StrToDouble(sText);
	}
	long lLimitText = GetLimitText();
	sText  = sText.Left(lLimitText);
	m_bApply = FALSE;
	SetWindowText(sText);
	m_bApply = TRUE;

}
void   CTPNumberEditExEx::SetPos32(INT64 iPos)
{
	iPos = min(iPos,m_lRangeMax); 
	iPos = max(iPos,m_lRangeMix);
	CString sText;	
	sText.Format(_T("%d"),(int)iPos);
	m_bApply = FALSE;
	SetWindowText(sText);
	m_bApply = TRUE;
}
double   CTPNumberEditExEx::GetPosEx()
{
	if(m_sEditText.GetLength()<0)
		GetWindowText(m_sEditText);		
	return StrToDouble(m_sEditText);
}
double   CTPNumberEditExEx::GetPos()
{
	CString sText;	
	GetWindowText(sText);		
	return StrToDouble(sText) + m_dDoubleSave;
}
INT64    CTPNumberEditExEx::GetPos32()
{
	CString sText;
	LONGLONG lValue = 0;
	GetWindowText(sText);	
	StrToInt64Ex(sText,STIF_DEFAULT,&lValue);
	return lValue;
}

void     CTPNumberEditExEx::SetDecimals( int iDem)
{
	m_lDecimals = iDem;

	if( m_bDouble)
	{	
		if(m_lDecimals >= 0)
		{
			if(m_dMin>=0)		CTPEditEx::SetIncludeKey(_T("01234567890.")  + (CString)g_cDim);
			else        		CTPEditEx::SetIncludeKey(_T("-01234567890.") + (CString)g_cDim);
		}
		else
		{
			if(m_dMin >= 0)		CTPEditEx::SetIncludeKey(_T("01234567890"));
			else 		        CTPEditEx::SetIncludeKey(_T("-01234567890"));
		}
	}
	// Added by zhh on 2007-10-25
	m_dStep = 1.0;
	for( int i = 0; i < iDem; ++ i)
		m_dStep /= 10.0;
}
void     CTPNumberEditExEx::SetAccel(int iAccel)
{
	m_iAccel = iAccel;
}

BOOL CTPNumberEditExEx::OnEnChange()
{
	if(!m_bApply) return TRUE;
	BOOL bSetSave = TRUE;
	CString sText;
	GetWindowText(sText);	
	m_sEditText = sText;
	GetSel(m_lMix,m_lMax);
	if(!m_bDouble &&  m_lRangeMix<m_lRangeMax)
	{		
		BOOL bReset = FALSE;
		while(sText.GetLength()>1 && sText[0] == '0')
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		while(sText.GetLength()>1 && sText[1] == '-' && sText[0] == '-' )
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		for(INT l= (INT)sText.GetLength() -1;l>=1;l--)
		{
			if(sText[l] == '-') 
			{
				sText = sText.Left(l) + sText.Mid(l+1);
				bReset = TRUE;
			}
		}
		if(bReset)
		{
			m_bApply = FALSE;
			if((m_sTextSave.GetLength()>sText.GetLength() ||   m_sTextSave.GetLength()<=0 )&& sText.GetLength()>0)
				SetWindowText(sText);
			else 
			{
					SetWindowText(m_sTextSave);
			}
			
			SetSel(m_lMix,m_lMax);
			m_bApply = TRUE;			
			return FALSE;
		}
	/*	long lValue = StrToInt(sText);
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
		}	*/
	}
	else if(m_bDouble &&  m_dMin<m_dMax)
	{		
		BOOL bReset = FALSE;
		while(sText.GetLength()>1 && sText[0] == '0' && sText[1] == '0' )
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		while(sText.GetLength()>1 && sText[1] == '-')
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		for(INT l= (INT)sText.GetLength() -1;l>=1;l--)
		{
			if(sText[l] == '-') 
			{
				sText = sText.Left(l) + sText.Mid(l+1);
				bReset = TRUE;
			}
		}
		int iDot   = 0;
		int iIndex = sText.GetLength();
		for(INT l=0;l<sText.GetLength();l++)
		{
			if(sText[l] == g_cDim)
			{
				if(iDot == 0) {iDot ++;iIndex = l;}
				else {sText = sText.Left(l) + sText.Mid(l+1);bReset = TRUE;}
			}
		}
		if(sText.GetLength() - iIndex >m_lDecimals+1)
		{
			sText = sText.Left(iIndex +1+m_lDecimals);
			bReset = TRUE;
		}
		if(sText.GetLength() >=1 && sText[0] == g_cDim || sText.GetLength() >=2 && sText[1] == g_cDim && sText[0] == '-')
		{
			/*jyn changed 20090902
			*@brief:苏冠乔说“.”在第一位的时候也应该显示出来。
			*/
			/*sText = m_sTextSave;
			bReset = TRUE;*/
			bSetSave = FALSE;
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
			return FALSE;
		}
	/*	double dValue = StrToDouble(sText);
		if(dValue<m_dMin || dValue >m_dMax)
		{
			if(sText.GetLength()>0)
			{
				sText = sText.Left(sText.GetLength()-1);
				dValue = StrToDouble(sText);
				if(dValue<m_dMin || dValue >m_dMax)
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
		}	*/
	}
	if(m_sTextSave == sText)
		return TRUE;
	else if(bSetSave)
	{
		long lValue = StrToInt(sText);
		if(m_lRangeMix<=lValue && lValue<=m_lRangeMax)	m_sTextSave = sText; // XN00028898 Wangjin 20101104
		return FALSE;
	}

	return TRUE;
}

void CTPNumberEditExEx::OnDestroy()
{
	SubWindow(FALSE);
	CTPEditEx::OnDestroy();
}
BOOL CTPNumberEditExEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{		
	m_bApply = FALSE;
	GetSel(m_lMix,m_lMax);
	CString sTextEx;
	GetWindowText(sTextEx);	
	if(m_bDouble)
	{
		double iPos = GetPos(),dPos;
		if(zDelta>= 0)      dPos = iPos + m_dStep;
		else if(zDelta < 0) dPos = iPos - m_dStep;
		dPos = max(m_dMin,dPos);
		dPos = min(m_dMax,dPos);
		CString sText = DoubleToStr(dPos,m_lDecimals);		;
		SetWindowText(sText);	
		if(m_lMax >= sTextEx.GetLength()) m_lMax = sText.GetLength();
		SetSel(m_lMix,m_lMax);
	}
	else
	{
		INT64 iPos = GetPos32();
		if(zDelta>= 0)      iPos += m_lStep;
		else if(zDelta < 0) iPos -= m_lStep;
		iPos = max(m_lRangeMix,iPos);
		iPos = min(m_lRangeMax,iPos);
		CString sText;
		sText.Format(_T("%d"),int(iPos>=0? iPos:-iPos));
		if(iPos<0) sText = _T("-") + sText;
		SetWindowText(sText);	
		if(m_lMax >= sTextEx.GetLength()) m_lMax = sText.GetLength();
		SetSel(m_lMix,m_lMax);
	}
	m_bApply = TRUE;	
	return TRUE;
}
void CTPNumberEditExEx::OnEnKillFocus()
{
	if(!m_bApply) return;
	m_dwLastError = 0;
	CString sText;
	GetWindowText(sText);
	m_sEditText = sText;
	GetSel(m_lMix,m_lMax);
	if(!m_bDouble &&  m_lRangeMix<m_lRangeMax)
	{		
		BOOL bReset = FALSE;
		while(sText.GetLength()>1 && sText[0] == '0')
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		while(sText.GetLength()>1 && sText[1] == '-' && sText[0] == '-' )
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		for(INT l= (INT)sText.GetLength() -1;l>=1;l--)
		{
			if(sText[l] == '-') 
			{
				sText = sText.Left(l) + sText.Mid(l+1);
				bReset = TRUE;
			}
		}	
		if(sText.IsEmpty())
		{
			if(m_sTextSave.IsEmpty()) m_sTextSave=_T("0");
			SetWindowText(m_sTextSave);
			SetSel(0,-1);
			
			////jyn added 2007.10.31
			CString strWarn;
			strWarn.Format(_T("Please enter an integer."));
			AfxMessageBox(strWarn);

			CWnd * ptemWnd =GetFocus();
			if(ptemWnd!=(CWnd*)this) SetFocus();
			m_dwLastError = 1;

			return;
		}
		long lValue = StrToInt(sText);
		if(lValue<m_lRangeMix || lValue >m_lRangeMax)
		{		
			sText.Format(_T("%d"),int(lValue<m_lRangeMix ? m_lRangeMix : m_lRangeMax)); 
			m_bApply = FALSE;
			SetWindowText(sText);
			SetSel(0, -1);
			m_bApply = TRUE;
			if(m_pToolTips) m_pToolTips ->m_uState = 3;

			if(m_bCheckInfo)
			{			
				////  jyn added 2007.10.31
				CString strWarn;
				strWarn.Format(_T("Please enter an integer between %d and %d."),(int)m_lRangeMix,(int)m_lRangeMax);
				AfxMessageBox(strWarn);			
				SetFocus();
				SetSel(0,-1);
			}
			return;				
		}	
	}
	else if(m_bDouble &&  m_dMin<m_dMax)
	{		
		BOOL bReset = FALSE;
		while(sText.GetLength()>1 && sText[0] == '0' && sText[1] == '0' )
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		while(sText.GetLength()>1 && sText[1] == '-')
		{
			sText = sText.Mid(1);
			bReset = TRUE;
		}
		for(INT l= (INT)sText.GetLength() -1;l>=1;l--)
		{
			if(sText[l] == '-') 
			{
				sText = sText.Left(l) + sText.Mid(l+1);
				bReset = TRUE;
			}
		}
		int iDot   = 0;
		int iIndex = sText.GetLength();
		for(INT l=0;l<sText.GetLength();l++)
		{
			if(sText[l] == g_cDim)
			{
				if(iDot == 0) {iDot ++;iIndex = l;}
				else {sText = sText.Left(l) + sText.Mid(l+1);bReset = TRUE;}
			}
		}
		if(sText.GetLength() - iIndex >m_lDecimals+1)
		{
			sText = sText.Left(iIndex +1+m_lDecimals);
			bReset = TRUE;
		}
		if(sText.GetLength() >=1 && sText[0] == g_cDim || sText.GetLength() >=2 && sText[1] == g_cDim && sText[0] == '-')
		{
			sText = m_sTextSave;
			bReset = TRUE;
		}
		double dValue = StrToDouble(sText);
		if(bReset)
			SetWindowText(sText);
		if(dValue<m_dMin || dValue >m_dMax)
		{
			CString str;
			str.Format(_T("%%.%df"),m_lDecimals);
			sText.Format(str,(dValue<m_dMin ? m_dMin : m_dMax)); 
			m_bApply = FALSE;
			SetWindowText(sText);
			SetSel(m_lMix,m_lMax);
			m_bApply = TRUE;
			if(m_pToolTips) m_pToolTips ->m_uState = 3;
		}	
	}
	m_sTextSave = sText;	
}

void CTPNumberEditExEx::OnKillFocus(CWnd* pNewWnd)
{
	CTPEditEx::OnKillFocus(pNewWnd);
	OnEnKillFocus();
	GetParent() ->PostMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_EDITCHANGE,(LPARAM)this);

}

BOOL CTPNumberEditExEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg ->message == WM_CHAR && pMsg->hwnd == m_hWnd  )
	{
		if(pMsg->wParam== VK_RETURN )
		{
			OnEnKillFocus();
			return TRUE;
		}

	}

	/*if(pMsg ->message == WM_KEYDOWN || pMsg ->message == WM_KEYUP)
	{
		if(pMsg ->wParam == VK_UP)        PostMessage(WM_UDNDELTAPOS,1,NULL);
		else if(pMsg ->wParam == VK_DOWN) PostMessage(WM_UDNDELTAPOS,2,NULL);
	}*/

	//DQAdb00007205	LiangXuan		20090616
	if(pMsg ->message == WM_KEYDOWN)
	{
		if(pMsg ->wParam == VK_UP)        PostMessage(WM_UDNDELTAPOS,1,NULL);
		else if(pMsg ->wParam == VK_DOWN) PostMessage(WM_UDNDELTAPOS,2,NULL);

		if(pMsg->wParam== VK_RETURN && pMsg->hwnd == m_hWnd)  //xl 2011.06.14
		{
			OnEnKillFocus();
			return TRUE;
		}
	}

	return CTPEditEx::PreTranslateMessage(pMsg);
}
