
#include "stdafx.h"

#include "TPExtraCtrl.h"
#include "TPTimeCodeEditEx.h"
#include ".\tptimecodeeditex.h"
#include "shlwapi.h"




#define TIMECODE_SETFRAME    0x00000001
#define TIMECODE_LBUTTONDOWN 0x00000002

__int64  CTPTimeCodeEditEx::m_iCopyFrame = -1;


CString  GetNormalStringFromFrame(int fType,__int64  iFrame)
{
	CString strTemp;
	strTemp.Format(_T("%d"),iFrame);
	return strTemp;
}


int  GetFrameFromNormalString(int fType,CString  strFrame)
{
	int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0;
	CString   strTemp;
	strFrame   = _T("0000000000") + strFrame;
	strFrame   = strFrame.Right(8);
	dwHour     = (strFrame[0]-'0')*10 + (strFrame[1]-'0') ;
	dwMinute   = (strFrame[2]-'0')*10 + (strFrame[3]-'0') ;
	dwSecond   = (strFrame[4]-'0')*10 + (strFrame[5]-'0') ;
	dwFrame    = (strFrame[6]-'0')*10 + (strFrame[7]-'0') ;

	return TP_GetFrameFormTime(dwHour,dwMinute,dwSecond,dwFrame,fType);
}

int GetFrameFromFrameString(CString strName)
{
	int len = strName.GetLength();
	int frame = 0 ;
	int i=0;
	while(i<len)
	{
		frame*=10;
		frame=frame+(strName[i]-'0');
		i++;
	}
	return frame;
} 

int CTPTimeCodeEditEx::AddFrameFromString(int iPos,int iValue)   
{
	int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0,iReturn=0,lFrame = m_eVideoType == VIDEO_25?25:30;
	CString strFrame = m_strInput;

	if(strFrame.GetLength()<11)    return 0;

	dwHour     = (strFrame[0]-'0')*10 + (strFrame[1]-'0');
	dwMinute   = (strFrame[3]-'0')*10 + (strFrame[4]-'0');
	dwSecond   = (strFrame[6]-'0')*10 + (strFrame[7]-'0');
	dwFrame    = (strFrame[9]-'0')*10 + (strFrame[10]-'0');

	switch(iPos)
	{
	case  0: dwHour   += iValue *10; break;
	case  1: dwHour   += iValue;     break;
	case  3: dwMinute += iValue *10; break;
	case  4: dwMinute += iValue;     break;
	case  6: dwSecond += iValue *10; break;
	case  7: dwSecond += iValue;     break;
	case  9: dwFrame  += iValue *10; break;
	case 10: dwFrame  += iValue;     break;
	}
	if(iValue>0)
	{

		if(dwFrame>=lFrame)  { dwSecond ++;dwFrame-=lFrame;}
		if(dwSecond>59)      { dwMinute ++;dwSecond-=60;}
		if(dwMinute>59)      { dwHour ++;dwMinute-=60;}
	}
	else
	{

		if(dwFrame<0)  { dwSecond --;dwFrame+=lFrame;}
		if(dwSecond<0) { dwMinute --;dwSecond+=60;}
		if(dwMinute<0) { dwHour --;  dwMinute+=60;}
		if(dwHour<0)   { dwHour+=24;}
	}		 

	iReturn = TP_GetFrameFormTime(dwHour,dwMinute,dwSecond,dwFrame,m_eVideoType);	

	if((m_uEditStyle & TP_TIMECODE_NEGATIVE))
	{
		return iReturn;
	}
	else return max(0,iReturn);
	
}

LRESULT CTPTimeCodeEditEx::OnTimeCodeThis(WPARAM wp,LPARAM lp)
{
	if(lp) ((CWnd *)lp) ->SetFocus();
	return S_OK;
}
int CTPTimeCodeEditEx::AddFrameFromNormalString(int iPos,int iValue)  
{
	int dwHour=0,dwMinute=0,dwSecond=0,dwFrame=0,iReturn =0;
	CString   strTemp;
	m_strInput   = m_strInput.Right(m_lMaxLength);
	int lFrame = StrToInt(m_strInput),lAdd =iValue,lMax = 9;
	for(INT l=0;l< m_lMaxLength-1;l++) lMax = lMax*10+9;

	if((m_uEditStyle & TP_TIMECODE_NEGATIVE) && m_bNegative)
	{
		lFrame = -lFrame;	
		lMax   = -lMax;
	}
	for(INT l=iPos;l<m_strInput.GetLength()-1;l++) lAdd*=10;
	lFrame += lAdd;
	iReturn = lFrame;	

	if((m_uEditStyle & TP_TIMECODE_NEGATIVE) && m_bNegative)
		iReturn = max(lMax,iReturn);
	else
		iReturn = min(lMax,iReturn);


	if(m_uEditStyle & TP_TIMECODE_NEGATIVE)
	{
		if(iReturn>0) m_bNegative = FALSE;
		else  m_bNegative = TRUE;
		return abs(iReturn);
	}
	else return max(0,iReturn);
}

IMPLEMENT_DYNAMIC(CTPTimeCodeEditEx, CTPWnd)
CTPTimeCodeEditEx::CTPTimeCodeEditEx()
{
	m_bCtlBtn                = FALSE;
	m_iDrawNegOffetVal       = 0;
	m_iDrawTimeCodeOffsetVal = 0;
	m_boffsetvalueshow       =TRUE;
	m_boffsetNeg             =FALSE;
	m_shadow                 =0;
	m_offsetView             =0;
	m_bTimeView              =FALSE;
	m_boffSet                =TRUE;
	m_b2BitFlag              =FALSE;
	m_bSubWindow             = FALSE;
	m_bCreate                = FALSE;
	m_eBorderStyle           = TP_GetBorderStyle();
	m_dwWindowLevel          = TPWNDLEVEL1;	
	m_iBackColor             = COLOR_BASE_WINDOW;
	m_bViewExchange          = FALSE;
	m_uShowStyle             =0; 
	m_bBeep				     =FALSE;
	m_eVideoType             = VIDEO_25;
	//m_eVideoType           = VIDEO_30M;//测试用代码
	if(m_eVideoType == VIDEO_30M)  m_strInput       = _T("00:00.00:00");
	else                           m_strInput = _T("00:00:00:00");

	m_iFrameInPut            = 0;
	m_iUnDoFrame             = -1;
	m_iFocusPos              = 10; 	
	m_iFocusUndoPos          = 0;
	m_iSetFocusFrame         = -1;
	m_iBeginSel              = 0;
	m_iEndSel                = 10;
	m_bNegative              = FALSE;
	m_bLable                 = TRUE; 	
	m_iEditMode              = 0;
	m_bDrawFocus             = FALSE;
	m_bSkipTimer             = TRUE;
	m_ptSave                 = CPoint(0,0);
	m_bInsert                = FALSE;
	m_lMaxLength             = 8;
	m_bFocus                 = FALSE;
	m_uState                 = 0;
	m_uEditStyle             =0;
	
    m_uEditStyle            |=(TP_TIMECODE_LOGO | TP_TIMECODE_NSFTCTYPE);
	m_uEditEnable            = TP_TIMECODE_NSLTCTYPE|TP_TIMECODE_NSVITCTYPE|TP_TIMECODE_NSFTCTYPE|TP_TIMECODE_NSCTLTYPE;
	m_uPreEditStyle			 = TP_TIMECODE_NSFTCTYPE;
	m_lMin                   = -MAX_INT64/3;
	m_lMax                   = MAX_INT64/3;
	m_uState                 = 0;
	m_pFont                  = TP_GetSysFont(m_iFontIndex);
	m_pFontNeg               =NULL;
	m_iCTLFrame              = 0;
	m_iabsFrame              = 0;
	m_bInvalid               = TRUE; 
	m_pShowTips              = NULL;
	m_sToolTips              = _T("Set m_sToolTips Variable");
	m_pMsgWnd                = NULL;
	
	m_bSelCalc				 = FALSE;
	m_bFrameToTC			 = FALSE;

	m_iLastInput			 = -1;
	m_bSendChangeMsg         = TRUE;
	m_iBaseTextWidth         = 10000;
	//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
	m_bNotifyMode			 = FALSE;
	m_iBackColor_Notify		 = COLOR_TL_TIMECODE; // 默认橙色
	m_iTextColor_Notify		 = COLOR_BASE_BALCK; // 默认黑色
#endif
	//}
}


inline void	CTPTimeCodeEditEx::SetTCClipBoardData(__int64 i64CopyData)
{
	CTPTimeCodeEditEx::m_iCopyFrame = i64CopyData;
}
inline __int64& CTPTimeCodeEditEx::GetTCClipBoardData()
{
	return CTPTimeCodeEditEx::m_iCopyFrame;
}


CTPTimeCodeEditEx::~CTPTimeCodeEditEx()
{
	if(m_uEditStyle & TP_TIMECODE_CHANGEOPEN)
	{
		if(m_pFontNeg ) {m_pFontNeg->DeleteObject();delete m_pFontNeg;m_pFontNeg=NULL;}
	}
	if(m_pFont && m_pFont != TP_GetSysFont(m_iFontIndex))
	{
		m_pFont ->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}
	
}


BEGIN_MESSAGE_MAP(CTPTimeCodeEditEx, CTPWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONUP()

	ON_MESSAGE(WM_TIMECODE_THIS,OnTimeCodeThis)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_EX_RANGE( TTN_NEEDTEXT, 0,0xffff, OnToolTipNotify)
	ON_MESSAGE(WM_UNSUBCLASSWINDOW,OnUnsubWindow)
END_MESSAGE_MAP()

void     CTPTimeCodeEditEx::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_WND,&wndClass);		
		wndClass.lpszClassName = WC_TPTIMECODEEDITEX;		
		::RegisterClass(&wndClass);		
	}
	else UnregisterClass(WC_TPTIMECODEEDITEX,AfxGetInstanceHandle());
}

void CTPTimeCodeEditEx::SubWindow(BOOL bSub)
{
	if(bSub)
	{	
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		EnableToolTips();

		if(m_dwWindowLevel == TPWNDLEVEL1)	m_iBackColor   = COLOR_BASE_FRAME;
		else	                        	m_iBackColor   = COLOR_BASE_FRAME2;

		SaveFont(m_hWnd);

		CTPWnd::SetFont(TP_GetSysFont(m_iFontIndex));
		FunCalcPos(m_iEditMode);
		m_bInsert = GetKeyState(VK_INSERT) & 0x00000001;		
#ifdef TP_NS_EDIT
		m_iTextColor =  COLOR_BASE_TIMECODE;
		m_iBackColor =  COLOR_BASE_BALCK;
#endif
		m_pMsgWnd = GetParent();
		GetBaseTextWidth();
	}
	else
	{
		if(!m_bSubWindow) return;
		RestoreFont(m_hWnd);
		m_bSubWindow = FALSE;		
		EnableToolTips(FALSE);
	}
}
BOOL CTPTimeCodeEditEx::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CTPWnd::PreCreateWindow(cs);
}

void CTPTimeCodeEditEx::PreSubclassWindow()
{
	CTPWnd::PreSubclassWindow();
 	if(!m_bCreate) SubWindow(TRUE);
}
void CTPTimeCodeEditEx::SetToolTips(CString sText)
{
	m_sToolTips = sText;
   
	if(m_sToolTips.GetLength() >0 && m_pShowTips == NULL)
	{
		m_pShowTips = new CTPToolTipCtrl();
		ASSERT(m_pShowTips->Create(this,TTS_ALWAYSTIP)); 
		m_pShowTips->EnableToolTips(TRUE); 
		m_pShowTips->Activate(TRUE); 
		m_pShowTips->AddTool(this,LPSTR_TEXTCALLBACK,CRect(0,0,500,200),GetDlgCtrlID());
	}
}

BOOL CTPTimeCodeEditEx::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	if((m_uEditStyle & TP_TIMECODE_TOOLTIPS))
	{
		TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;	
		lstrcpy( pTTT->szText , m_sToolTips);
		return  TRUE;		
	}
	return  FALSE;		

}


int   CTPTimeCodeEditEx::OnToolHitTest(CPoint point,TOOLINFO* pTI ) const
{	
	if(m_sToolTips.GetLength()>0 && (m_uEditStyle & TP_TIMECODE_TOOLTIPS))
	{		
		pTI ->hwnd     = m_hWnd;	
		pTI ->lpszText = LPSTR_TEXTCALLBACK;
		pTI ->uId      = (UINT)(UINT_PTR)m_hWnd;
		pTI ->uFlags  |= TTF_IDISHWND;

		return  GetDlgCtrlID();			
	}
	else return -1;
}

int CTPTimeCodeEditEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTPWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
LRESULT CTPTimeCodeEditEx::OnUnsubWindow(WPARAM wp,LPARAM lp)
{
	SubWindow(FALSE);
	return S_OK;
}
void CTPTimeCodeEditEx::OnDestroy()
{
	SubWindow(FALSE);
	CTPWnd::OnDestroy();
}

void CTPTimeCodeEditEx::OnPaint()
{
	CPaintDC dc(this); 	
	PaintDC(&dc);
}
void CTPTimeCodeEditEx::DrawNeg(CDC *pDC ,CRect rtText)
{
	if((m_uEditStyle & TP_TIMECODE_DISABLE_ZERO_NEG) && m_iFrameInPut==0) return;
	if(!(m_uEditStyle & TP_TIMECODE_SHOWNEG))
	{
		
		if((m_uEditStyle & TP_TIMECODE_NEGATIVE_POS))
		{
			if(m_bLable==TRUE && !m_bNegative) return;
			if(m_bNegative)
				pDC->DrawText(_L("-"),rtText,DT_CENTER|DT_TOP); 
			else
				pDC->DrawText(_L("+"),rtText,DT_CENTER|DT_TOP); 


		}
		else
		{

			if(m_bLable==TRUE && !m_bNegative)return;
			if(m_bNegative)
				pDC->DrawText(_L("-"),m_rtNegative,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 
			else 
				pDC->DrawText(_L("+"),m_rtNegative,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 


		}	
	}
	else
	{

		if((m_uEditStyle & TP_TIMECODE_NEGATIVE_POS))
		{

			if(m_bNegative)
				pDC->DrawText(_L("-"),rtText,DT_CENTER|DT_TOP); 

			else if(!m_bLable)
				pDC->DrawText(_L("+"),rtText,DT_CENTER|DT_TOP); 

		}
		else
		{

			if(m_bNegative)
				pDC->DrawText(_L("-"),m_rtNegative,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 
			else if(!m_bLable) 
				pDC->DrawText(_L("+"),m_rtNegative,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 

		}


	}
}
#ifdef TP_NS_EDIT
void CTPTimeCodeEditEx::PaintDC(CDC *pDC)
{
	CRect     rtClient,rtText;

	GetTimeCodeRect(rtClient);		
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());		
	CFont* pOldFont = cdcMemo.SelectObject(m_pFont);
	cdcMemo.SetBkMode(TRANSPARENT); 	

	if((m_uEditStyle & TP_TIMECODE_DISABLECOLOR) && !IsWindowEnabled()) cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS)); 
	//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
	else if ( m_iTextColor > 0 )
	{
		if ( m_bNotifyMode && !(m_uEditStyle & TP_TIMECODE_NSCTLTYPE) ) // 设置为醒目颜色，CTL仍然显示绿色
		{
			cdcMemo.SetTextColor( TP_GetSysColor( m_iTextColor_Notify ) );
		}
		else // 原始颜色
		{
			cdcMemo.SetTextColor( TP_GetSysColor( m_iTextColor ) );
		}
	}
#else
	else if(m_iTextColor>0)                                             cdcMemo.SetTextColor(TP_GetSysColor(m_iTextColor)); 
#endif
	//}
	else if(!IsWindowEnabled())                                         cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS)); 
	else if(GetFocus() == this)                                         cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS)); 
	else                                                                cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT)); 

	//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
	int iBackColorBak = m_iBackColor;
	if ( m_bNotifyMode ) // 使用醒目的背景色
	{
		m_iBackColor = m_iBackColor_Notify;
	}
#endif
	//}

	if(m_iEditMode == 1)
	{

		if(m_bFocus) cdcMemo.FillSolidRect(&rtClient,TP_GetSysColor(m_iBackColor));
		else         cdcMemo.FillSolidRect(&rtClient,TP_GetSysColor(m_iBackColor));  
	}
	else cdcMemo.FillSolidRect(&rtClient,TP_GetSysColor(m_iBackColor)); 

	//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
	m_iBackColor = iBackColorBak; // 还原背景色
#endif
	//}

	{	
		int iColotIndex = COLOR_BASE_WINDOW;
		if(m_iEditMode == 1)
		{
			
			if(m_bFocus)    iColotIndex = m_iBackColor;
			else        	iColotIndex = COLOR_BASE_WINDOW;
		}		
		if(m_rtSelText.Width()>0&&(m_bDrawFocus==TRUE)&&m_rtSelText.right>m_rtText.left&&m_rtSelText.left<m_rtText.right && m_iEditMode==1)
			cdcMemo.FillSolidRect(m_rtSelText,RGB(90,90,90));
		if(m_bDrawFocus)
		{
			if(m_iEditMode==0)
			{
				if(m_uShowStyle==1)
				{
					m_rtFocus.left=m_rtFocus.left-m_shadow;
					cdcMemo.FillSolidRect(m_rtFocus,RGB(90,90,90));
					m_rtFocus.left=m_rtFocus.left+m_shadow;
				}
				else cdcMemo.FillSolidRect(m_rtFocus,RGB(90,90,90));
		
			}
		}
	}	

	if(!(m_uEditStyle & TP_TIMECODE_NEGATIVE) && (-1 !=m_strInput.FindOneOf(_T("--")) )&& m_iEditMode == 0)  
	{
		CRect szContent1 = CRect(0,0,0,0), szContent2 = CRect(0,0,0,0),rtText;
		cdcMemo.DrawText(_T("9"),szContent1,DT_CALCRECT);
		cdcMemo.DrawText(_T(":"),szContent2,DT_CALCRECT);
		rtText= m_rtText;
		for(INT l=0;l<m_strInput.GetLength();l++)
		{
			if(m_strInput[l] == '-') rtText.right = rtText.left + szContent1.Width();
			else                     rtText.right = rtText.left + szContent2.Width();
			cdcMemo.DrawText((CString)m_strInput[l],rtText,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 
			rtText.OffsetRect(rtText.Width(),0);
		}
	}
	else
	{
		CRect rtText = m_rtText;
		rtText.DeflateRect((rtText.Width() - m_iBaseTextWidth)/2-1,0);		
		cdcMemo.DrawText(m_strInput,rtText,DT_LEFT|DT_VCENTER|DT_SINGLELINE); 
	}
	
	//显示当前绝对值的代码,不要删
	/*CString str;
	str.Format(_T("%d"),m_iabsFrame);
	CRect rt;
	rt=m_rtText;
	rt.right=rt.left;
	rt.left =rt.right-50; 
	cdcMemo.DrawText(str,rt,DT_CENTER|DT_VCENTER|DT_SINGLELINE); */    
	

	if(m_uEditStyle &TP_TIMECODE_CHANGEOPEN)
	{
		CFont* pold =cdcMemo.SelectObject(m_pFontNeg);
		DrawNeg(&cdcMemo ,m_rtNegative);	
		cdcMemo.SelectObject(pold);
	}
	else DrawNeg(&cdcMemo ,m_rtNegative);
	cdcMemo.SelectObject(pOldFont);	
	pDC ->BitBlt(rtClient.left,rtClient.top,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	

}
#else
void CTPTimeCodeEditEx::PaintDC(CDC *pDC)
{
	CRect     rtClient;
	GetTimeCodeRect(rtClient);	
	CTPMemoDC cdcMemo(pDC,rtClient.Width(),rtClient.Height());		
	CFont* pOldFont = cdcMemo.SelectObject(m_pFont);

	
	cdcMemo.SetBkMode(TRANSPARENT); 	
	if(m_iTextColor>0)          cdcMemo.SetTextColor(TP_GetSysColor(m_iTextColor)); 
	else if(GetFocus() == this) cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTFOCUS)); 
	else if(!IsWindowEnabled()) cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXTDIS)); 
	else                        cdcMemo.SetTextColor(TP_GetSysColor(COLOR_BASE_TEXT)); 

	cdcMemo.FillSolidRect(&rtClient,TP_GetSysColor(m_iBackColor)); 

	if(!(m_uEditStyle & TP_TIMECODE_NEGATIVE) && m_iFrameInPut <0 && m_iEditMode == 0)  
	{
		CRect szContent1 = CRect(0,0,0,0), szContent2 = CRect(0,0,0,0),rtText;
		cdcMemo.DrawText(_T("9"),szContent1,DT_CALCRECT);
		cdcMemo.DrawText(_T(":"),szContent2,DT_CALCRECT);
		rtText= m_rtText;
		for(INT l=0;l<m_strInput.GetLength();l++)
		{
			if(m_strInput[l] == '-') rtText.right = rtText.left + szContent1.Width();
			else                     rtText.right = rtText.left + szContent2.Width();
			cdcMemo.DrawText((CString)m_strInput[l],rtText,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 
			rtText.OffsetRect(rtText.Width(),0);
		}
	}
	else cdcMemo.DrawText(m_strInput,m_rtText,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 
	if(m_bFocus)
	{	
		if(m_rtSelText.Width()>0&&m_rtSelText.right>m_rtText.left&&m_rtSelText.left<m_rtText.right)
			cdcMemo.InvertRect(m_rtSelText); 	
		if(m_bDrawFocus)
			cdcMemo.InvertRect(m_rtFocus); 
	}		
	if(( TP_TIMECODE_NEGATIVE & m_uEditStyle) && m_bNegative && m_iFrameInPut>0)
		cdcMemo.DrawText(_L("-"),m_rtNegative,DT_CENTER|DT_VCENTER|DT_SINGLELINE); 

	cdcMemo.SelectObject(pOldFont);	
	pDC ->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&cdcMemo,0,0,SRCCOPY);	
	
}
#endif


BOOL CTPTimeCodeEditEx::OnEraseBkgnd(CDC* pDC)
{
	return TRUE ;
}
BOOL  CTPTimeCodeEditEx::Create(UINT uStyle,CRect rectWnd,CWnd *pParent,UINT nID)
{
	return  CWnd::Create(NULL,_T(""),uStyle ,rectWnd,pParent,nID);	
}

__int64     CTPTimeCodeEditEx::GetFrame()
{
	if(m_uEditStyle&TP_TIMECODE_POPINPUT)
	{
		if(m_iFrameInPut==-1) 
			return -1;
		else 
		{
			if(!TestTheString(m_strInput))
			{
				return -1;
			}
			else
				return m_bNegative ? - m_iFrameInPut : m_iFrameInPut;

		}
	}
	else
	{
		TestTheString(m_strInput);
		if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE&& !OffSet())
		{
			m_iFrameInPut=m_iFrameInPut+m_iCTLFrame;
			m_iFrameInPut%=(GetMaxFrame(m_eVideoType));
		}
		return m_bNegative ? - m_iFrameInPut : m_iFrameInPut;

	}
}
CString   CTPTimeCodeEditEx::GetString()
{
	return m_strInput;
}

void      CTPTimeCodeEditEx::SetFrame(__int64 iFrame,BOOL bPaint , BOOL ResetNeg)
{
	if(ResetNeg){m_bLable=TRUE;m_bNegative=FALSE;}
	m_iabsFrame = iFrame;

	BOOL  bReset = !(TIMECODE_SETFRAME & m_uState);
	m_uState |= TIMECODE_SETFRAME;
	iFrame = min(iFrame,m_lMax);

	iFrame = max(iFrame,m_lMin);
	if((m_uEditStyle & TP_TIMECODE_NEGATIVE))
	{	
		if(m_uEditStyle & TP_TIMECODE_DISPLAYNEG)
		{
			if(iFrame>0){	m_iFrameInPut =  iFrame;m_bLable = FALSE;m_bNegative = FALSE;}
			else if(iFrame<0) {	m_iFrameInPut = -iFrame;m_bNegative = TRUE;m_bLable = TRUE;}	
			else {	m_iFrameInPut = 0;m_bNegative = FALSE;m_bLable = TRUE;}	
		}
		else
		{
			if(iFrame>=0){	m_iFrameInPut =  iFrame;m_bNegative = FALSE;}
			else if(iFrame<0) {	m_iFrameInPut = -iFrame;m_bNegative = TRUE;}	
		}
	}
	else 
	{
		if(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)
		{
			m_iFrameInPut=(GetMaxFrame(m_eVideoType)+iFrame)%GetMaxFrame(m_eVideoType);
			m_strInput=StringFromFrame(m_eVideoType,m_iFrameInPut);

		}
		else
		{
			if(iFrame<0) 
			{
				if(m_uEditStyle &TP_TIMECODE_OldNew || m_eVideoType==VIDEO_30M)
				{
					if(m_iEditMode == 0)
						m_strInput=_T("--:--.--:--");
					else
						m_strInput=_T("- - - - - - - -");
				}
				else
				{
					if(m_iEditMode == 0)
						m_strInput=_T("--:--:--:--");
					else
						m_strInput=_T("- - - - - - - -");
				}
				m_iCTLFrame = iFrame;
			}
			else
			{
				if(m_iCTLFrame < 0) m_iCTLFrame= 0;
			}
			m_iFrameInPut = iFrame;

		}
	}	
	if(m_iFrameInPut>=0)
	{

		if(m_iEditMode == 0)
		{
			if(m_uEditStyle&TP_TIMECODE_NSCTLTYPE)
			{
				INT64 tempFrame=m_iFrameInPut-m_iCTLFrame;
				if(tempFrame<0)tempFrame+=GetMaxFrame(m_eVideoType);
				m_strInput=StringFromFrame(m_eVideoType,tempFrame);
			}
			else m_strInput  = StringFromFrame(m_eVideoType,m_iFrameInPut);
			if(bReset)	m_iFocusPos = 10;
		}
		else                   
		{
			/*jyn added 20091102
			*@brief:帧显示时码时，需要取天的余数
			*/
			INT64 i64TemFrame = m_iFrameInPut%GetMaxFrame(m_eVideoType);
			/*jyn added end*/
	     	m_strInput  = GetNormalStringFromFrame(m_eVideoType,i64TemFrame);
			if(bReset)
			{
				m_iFocusPos = min(m_lMaxLength, m_strInput.GetLength()+1);
				m_iBeginSel = 0;
				m_iEndSel   = m_iFocusPos-1;
			}
		}
	}
	if(bPaint) 
	{
		if(m_uEditStyle &TP_TIMECODE_POPINPUT)FunCalcPos(m_iEditMode,TRUE);
		else FunCalcPos(m_iEditMode,FALSE);
		Invalidate();
	}
}
void CTPTimeCodeEditEx::GetBaseTextWidth()
{
	CDC   *pDC       = GetDC();
	CFont *pFont     = pDC ->SelectObject(m_pFont);
	CSize  szText    = pDC->GetTextExtent(_T("99:99:99:99"));
	m_iBaseTextWidth = szText.cx;
	pDC ->SelectObject(pFont);
	ReleaseDC(pDC);
}
void CTPTimeCodeEditEx::SetFont(CFont &ftSet)
{
	if(m_pFont && m_pFont != TP_GetSysFont(m_iFontIndex))
	{
		m_pFont ->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}
	m_pFont = new CFont();
	LOGFONT lgFont;
	ftSet.GetLogFont(&lgFont);
	m_pFont ->CreateFontIndirect(&lgFont);

	GetBaseTextWidth();
}
void  CTPTimeCodeEditEx::SetNegFont(CFont &pfont)
{
	if(m_pFontNeg==NULL){}
	else 
	{
		m_pFontNeg->DeleteObject();
		delete m_pFontNeg;
		m_pFontNeg = NULL;
	}
	m_pFontNeg = new CFont();
	LOGFONT lgFont;
	pfont.GetLogFont(&lgFont);
	m_pFontNeg ->CreateFontIndirect(&lgFont);

}
void       CTPTimeCodeEditEx::SetString(CString strFrame,BOOL bPaint)
{
	if(m_iEditMode == 0)
	{
		m_iFrameInPut  = FrameFromString(m_eVideoType,strFrame);
		m_strInput     = StringFromFrame(m_eVideoType,m_iFrameInPut);
	}
	else
	{
		m_iFrameInPut  = GetFrameFromNormalString(m_eVideoType,strFrame);
		m_strInput     = GetNormalStringFromFrame(m_eVideoType,m_iFrameInPut);
	}
	if(bPaint) Invalidate();	
}


void       CTPTimeCodeEditEx::SetVideoType(VIDEO_TYPE iVideoType,BOOL bReCalc,BOOL bFrameBase)
{
	__int64 nMinNew =  ((-MAX_INT64/3<m_lMin) && (m_lMin<MAX_INT64/3)) ? FrameFromString(iVideoType, StringFromFrame(m_eVideoType, m_lMin)) : m_lMin;
	__int64 nMaxNew =  ((-MAX_INT64/3<m_lMax) && (m_lMax<MAX_INT64/3)) ? FrameFromString(iVideoType, StringFromFrame(m_eVideoType, m_lMax)) : m_lMax;
	m_eVideoType = iVideoType;
	if(bReCalc)
	{
		SetRange(nMinNew, nMaxNew);
		if(bFrameBase)//基于frame的
		{
			SetFrame(m_iFrameInPut);
		}
		else//基于String的
		{
			m_iFrameInPut= FrameFromString(m_eVideoType,m_strInput);
			SetFrame(m_iFrameInPut);
		}
	}
}

 BOOL CTPTimeCodeEditEx::FunKeyDown(UINT nChar) 
 {   
	 m_bInvalid=TRUE;

	 if(m_iEditMode==0 && m_uShowStyle==0 && nChar != VK_TAB/*jyn added 20090707 for XN00022330*/) return TRUE;
	 if(!((nChar >= VK_NUMPAD0 && nChar <=VK_NUMPAD9) ||(nChar>='0' && nChar<='9') ) && m_b2BitFlag==TRUE) m_b2BitFlag=FALSE;
	 if(nChar==VK_DECIMAL )
	 {
		 if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
			 && (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT))
			 && m_iFrameInPut<0 
			 && FrameFromString(m_eVideoType,m_strInput)<0  )//add by lzq:当已经输入数字后再输入"."/"00",不要清零，而应该在后面补00.bug:XN00012276,XN00022518，2009.08.06
		 {
			 if(m_eVideoType == VIDEO_30M)m_strInput       = _T("00:00.00:00");
			 else m_strInput       = _T("00:00:00:00");

			 FunCalcPos(m_iEditMode,1);
			 Invalidate();
		 }
		 if(m_iEditMode==0 && m_uShowStyle==1)
		 {
			 if(m_bViewExchange==FALSE)
			 {
				 m_iFrameInPut=0;
				 m_strInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
				 Invalidate();
			 }
			 else
			 {
				 MoveTheTimeCode(m_strInput,'0');
			     MoveTheTimeCode(m_strInput,'0');
			     m_iFocusPos=10;
			     Invalidate();
			 }
		 }
		 if(m_iEditMode==0 && m_uShowStyle==2)  
		 {
			 m_iFocusPos =m_iFocusPos+3 ;
			 if(m_iFocusPos >=11) m_iFocusPos =1;
			 else  if(m_iFocusPos ==2 || m_iFocusPos == 5 ||m_iFocusPos ==8)  m_iFocusPos ++;
			 FunCalcPos(m_iEditMode);
			 Invalidate();

		 }

		 return TRUE;
	 }
	 else if(nChar==VK_DELETE)
	 {
		 if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
			 && (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)) 
			 && m_iFrameInPut<0
			 && FrameFromString(m_eVideoType,m_strInput)<0 )
		 {
			 return TRUE;
		 }

		 if(m_iEditMode==0 && m_uShowStyle==1)
		 {
			 if(m_eVideoType == VIDEO_30M)m_strInput       = _T("00:00.00:00");
			 else m_strInput       = _T("00:00:00:00");
		 }
		 else if (m_iEditMode==0 && m_uShowStyle==2)
		 {
			 PTSTR pstr=m_strInput.GetBuffer(10);
			 pstr[m_iFocusPos]='0';
			 pstr[m_iFocusPos-1]='0';
			 m_strInput.ReleaseBuffer();
			 FunCalcPos(m_iEditMode);
		 }
		 else
		 {
			 m_strInput=_T("0");
			 FunCalcPos(1);

		 }
		 m_iFrameInPut=0;

		 Invalidate();	
		 return TRUE;

	 }
	 else if(nChar == VK_ESCAPE) 
	 {
		 if(m_strInput == _T("--:--:--:--") || m_strInput == _T("--:--.--:--") || m_strInput == _T("- - - - - - - -")) return TRUE;
		 m_bViewExchange =FALSE;	
	
		 if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE)
		 {
		      m_iFrameInPut=(m_iabsFrame-m_iCTLFrame);
			  if(m_iFrameInPut<0) m_iFrameInPut+=GetMaxFrame(m_eVideoType);
			  m_strInput  = StringFromFrame(m_eVideoType,m_iFrameInPut);
			
		 }
		 else
		 {
			 if(m_iEditMode == 0)	m_strInput  = StringFromFrame(m_eVideoType,m_iFrameInPut);
			 else                    m_strInput  = GetNormalStringFromFrame(m_eVideoType,m_iFrameInPut);
		 }

		 //{{add by lzq: 2009.08.28,ESC退出，恢复以前的值。
		 m_boffSet=m_bLable;
		 m_boffsetNeg=m_bNegative;
		 if(m_boffsetNeg || !m_boffSet)//+ - 
		 {
			 m_iFrameInPut = m_iLastInput;
			 m_strInput  = StringFromFrame(m_eVideoType,m_iFrameInPut);		 
		 }
		 //}}

		 m_bNegative     =FALSE;
		 m_bLable        =TRUE;
		 m_boffSet=m_bLable;
		 m_boffsetNeg=m_bNegative;		 

		 m_iFocusPos            = 10;
		 //m_uShowStyle=0; DQAdb00008473 linhao 2009-12-30
		 m_iBeginSel  = 0;
		 m_iEndSel    = 10;		
		 m_rtSelText  = CRect(0,0,0,0);
		 m_rtFocus    = CRect(0,0,0,0);
		 m_bDrawFocus = FALSE;
		 GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_ESC,(LPARAM)this);
		 if(m_uEditStyle & TP_TIMECODE_WANTRETURN)
		 {
			 CWnd *pNext = GetParent()->GetNextDlgTabItem(this,TP_IsShiftKeyDown());
			 if(pNext && pNext != this) 		PostMessage(WM_TIMECODE_THIS,0,(LPARAM)pNext); 
			 else                            PostMessage(WM_TIMECODE_THIS,0,(LPARAM)GetParent());
			 return TRUE;
		 }
		 Invalidate();
		 return TRUE;
	 }
	 else if(nChar == VK_RETURN) 
	 {
		 if(m_uEditStyle & TP_TIMECODE_WANTRETURN)
		 {
			 CWnd *pNext = GetParent()->GetNextDlgTabItem(this,TP_IsShiftKeyDown());
			 if(pNext && pNext != this) 		PostMessage(WM_TIMECODE_THIS,0,(LPARAM)pNext); 
			 else                            PostMessage(WM_TIMECODE_THIS,0,(LPARAM)GetParent());
			 return TRUE;
		 }

		 //lzq:帧模式转换为时码模式。2009.09.28
		 if (m_bFrameToTC && m_bTimeView) 
		 {
			 if(m_iFrameInPut<0) m_iFrameInPut = 0;//lzq:当m_iFrameInPut<0时，不让转换。所以这里修正一下。2009.09.29
			 SetEditMode(0);
		 }
		 
         //////////////判断当前输入的字符串是否合法,如果合法,将其转化成m_iframeinput,m_bInvalid为TRUE.
		 //////////////反之,m_iframeinput无效,m_bInvalid为false.
		 __int64 tempUndoFrame=m_iabsFrame;
		 if(TestTheString(m_strInput)&&tempUndoFrame!=m_iabsFrame)
		 {
			 m_boffSet=m_bLable;
			 m_boffsetNeg=m_bNegative;

			 if(OffsetType4()!=1 && OffsetType4()!=2)
				 m_iUnDoFrame=tempUndoFrame;
		 }

		 else if(m_uEditStyle&TP_TIMECODE_POPINPUT) 
		 {
			 //m_iFrameInPut=-1; jyn changed 7.18
		 }
		 

		 //////////////////////符号存档,并将符号显示标志置零
	
		 m_boffSet=m_bLable;
		 m_boffsetNeg=m_bNegative;
		 if((m_boffsetNeg || !m_boffSet) && m_bInvalid)
		 {
			 m_offsetView=m_iFrameInPut;
		 }

		 if(!(m_uEditStyle & TP_TIMECODE_NEGATIVE))
		 {
			 m_bLable    =TRUE;
			 m_bNegative =FALSE;

		 }

		 //lzq: 控件自己计算‘+/-’，并把计算后的结果设为当前值。2009.08.28
		 if(m_bSelCalc && m_bInvalid)  FunCalcFrame();		


		 //////////////////////
         ///////////////////// 其他归位标志
		 m_iFocusUndoPos       = m_iFocusPos; 	

		 m_bViewExchange       =FALSE;
		 m_bDrawFocus          = FALSE;
		 m_bSkipTimer          = TRUE;
		 m_boffsetvalueshow    = TRUE;
		 m_iFocusPos           =10;

		 m_iBeginSel  = 0;
		 m_iEndSel    = 10;		

		 m_rtSelText  = CRect(0,0,0,0);
		 m_uShowStyle=0;

		 FunCalcPos(m_iEditMode,TRUE);

		 Invalidate();		

		 //if(m_bInvalid)			TFS3004
			 GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_RETURN,(LPARAM)this); 

		 return TRUE;
	 }
	 else if(nChar == VK_OEM_PLUS || nChar == VK_ADD)
	 {
    	 if(!(m_uEditStyle & TP_TIMECODE_NEGATIVE) && !(m_uEditStyle & TP_TIMECODE_POSITIVE_SIGN) ) return TRUE;
		 if(!(m_uEditStyle & TP_TIMECODE_SHOWNEG))
		 {	
			 m_bNegative           = FALSE;	
			 m_bLable=false;	
			 m_bDrawFocus          = TRUE;
			 m_bSkipTimer          = TRUE;
		 }
		 else
		 {
			 m_bNegative           = FALSE;
			 m_bLable=!m_bLable;
		 }

		 m_iLastInput = m_iFrameInPut;	//add by lzq;2009.08.28;	

		 if(!(m_uEditStyle&TP_TIMECODE_NEGATIVE)  && m_boffsetvalueshow && m_iFrameInPut>=0 )
		 {
			 //add by lzq:如果先输入数值后按“+/-”则，直接在当前输入数值前 设+/-号，2009.09.28 
			 BOOL bHasInput = FALSE;
			 CString strLastInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
			 if (strLastInput.CompareNoCase(m_strInput)!=0)	 bHasInput = TRUE;

			 if(m_iEditMode==0)
			 {
				 m_boffsetvalueshow=FALSE;
				 if (!bHasInput)
				 {
					 m_iFrameInPut=m_offsetView;
					 m_strInput=StringFromFrame(m_eVideoType,m_offsetView);
				 }
				 FunCalcPos(0,TRUE);
			 }
			 else
			 {
				 m_boffsetvalueshow=FALSE;
				 if (!bHasInput)
				 {
					 m_iFrameInPut=m_offsetView;
					 m_strInput=GetNormalStringFromFrame(m_eVideoType,m_offsetView);
				 }
				 FunCalcPos(1);
			 }			 
		 }
		
		 Invalidate();	
		 return TRUE;
	 }
	 else if(nChar == VK_OEM_MINUS  || nChar == VK_SUBTRACT)
	 {
		
		 if(!(TP_TIMECODE_NEGATIVE & m_uEditStyle) && !(m_uEditStyle & TP_TIMECODE_POSITIVE_SIGN)) return FALSE;
		 if(!(m_uEditStyle & TP_TIMECODE_SHOWNEG))
		 {

			 m_bNegative           = TRUE;	
			 m_bDrawFocus          = TRUE;
			 m_bSkipTimer          = TRUE;
		 }
		 else
		 {
			 m_bNegative       = !m_bNegative;
			 m_bLable=TRUE;
		 } 

		 m_iLastInput = m_iFrameInPut;	//add by lzq;2009.08.28;		

		 if(!(m_uEditStyle&TP_TIMECODE_NEGATIVE) && m_boffsetvalueshow && m_iFrameInPut>=0 )
		 {
			 //add by lzq:如果先输入数值后按“+/-”则，直接在当前输入数值前 设+/-号，2009.09.28 
			 BOOL bHasInput = FALSE;
			 CString strLastInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
			 if (strLastInput.CompareNoCase(m_strInput)!=0)	 bHasInput = TRUE;			

			 if(m_iEditMode==0)
			 {
				 m_boffsetvalueshow=FALSE;
				 if (!bHasInput)
				 {
					 m_iFrameInPut=m_offsetView;
					 m_strInput=StringFromFrame(m_eVideoType,m_offsetView);
				 }
				 FunCalcPos(0,TRUE);
			 }
			 else
			 {
				 m_boffsetvalueshow=FALSE;
				 if (!bHasInput)
				 {
					 m_iFrameInPut=m_offsetView;
					 m_strInput=GetNormalStringFromFrame(m_eVideoType,m_offsetView);
				 }
				 FunCalcPos(1);
			 }
		 }
		 Invalidate();	
		 return TRUE;
	 }	
	 if(nChar==VK_MULTIPLY )
	 {
		 //*号准备切换视图
		 if(m_bViewExchange && !(m_uEditStyle & TP_TIMECODE_NSCTLTYPE))
		 {
			 BOOL m_bView=!m_bTimeView;
			 SetEditModeNoChange(m_bView);
			 if(m_bView)
				 GetParent()->SendMessage(WM_TIMECODE_THIS,TP_TIMECODE_FRAMEVIEW,(LPARAM)this);
			 else
				 GetParent()->SendMessage(WM_TIMECODE_THIS,TP_TIMECODE_FTCVIEW,(LPARAM)this);
		 }
		 return TRUE;
	 }
	 else
	 {
		 __int64 iFrame     = m_iFrameInPut;
		 CString sFrame     = m_strInput;
		 int     iFocusPos  = m_iFocusPos;     
		 if(nChar >= VK_NUMPAD0 && nChar <=VK_NUMPAD9)   nChar = nChar - VK_NUMPAD0 + '0';
		 if(m_iEditMode ==0)
		 {
			 if(nChar >='0' && nChar <='9')
			 {
				 if(m_iFocusPos != 2 && m_iFocusPos != 5 && m_iFocusPos != 8)
				 {
					 if(m_uShowStyle==1)
					 {
						 if(m_bViewExchange==FALSE )
						 {
							 if(m_eVideoType == VIDEO_30M)m_strInput       = _T("00:00.00:00");
							 else m_strInput                               = _T("00:00:00:00");
						 }
						 MoveTheTimeCode(m_strInput,nChar);
						 Invalidate();
						 m_bViewExchange=TRUE;
						 return TRUE;
					 }
					 else if(m_uShowStyle==2)	MoveTheTimeCodex(m_strInput,nChar);					
				 }
				 else 
					 return FALSE;
			 }
			 else if(nChar == VK_HOME)
			 {
				 if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
					 && (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)) 
					 && m_iFrameInPut<0
					 && FrameFromString(m_eVideoType,m_strInput)<0 )
				 {
					 return TRUE;
				 }
				 if(m_uShowStyle==1||m_uShowStyle==0) 
				 {
					 m_uShowStyle= 2;
					 m_iFocusPos = 1;

				 }
				 else m_iFocusPos=1;
			 }
			 else if(nChar == VK_END)
			 {
				 if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
					 && (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)) 
					 && m_iFrameInPut<0
					 && FrameFromString(m_eVideoType,m_strInput)<0 )
				 {
					 return TRUE;
				 }

				 if(m_uShowStyle==1||m_uShowStyle==0) 
				 {
					 m_uShowStyle  = 2;
					 m_iFocusPos   = 10;

				 }
				 else m_iFocusPos  =10;
			 }
			 else if(nChar == VK_PRIOR)
			 {
				 return TRUE;
			 }
			 else if(nChar == VK_NEXT)
			 {
				 return TRUE;
			 }
			 else if(nChar == VK_DELETE)
			 {
				 if(m_iFocusPos<0 || m_iFocusPos >=11 || m_iFocusPos ==2 || m_iFocusPos == 5 ||m_iFocusPos ==8) return TRUE;
				 else m_strInput.SetAt(m_iFocusPos,'0');
			 }
			 else  if(nChar == VK_LEFT  )
			 {
				 if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
					 && (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)) 
					 && m_iFrameInPut<0
					 && FrameFromString(m_eVideoType,m_strInput)<0  )
				 {
					 return TRUE;
				 }
				 if(m_uShowStyle==1||m_uShowStyle==0) 
				 {
					 m_iFocusPos=1;
					 m_uShowStyle=2;

				 }
				 else m_iFocusPos =m_iFocusPos-2 ;
				 if(m_iFocusPos <0) m_iFocusPos =10;
				 else  if(m_iFocusPos ==2 || m_iFocusPos == 5 ||m_iFocusPos ==8)  m_iFocusPos --;
			 }
			 else  if(nChar == VK_RIGHT)
			 {
				 if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
					 && (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)) 
					 && m_iFrameInPut<0 
					 && FrameFromString(m_eVideoType,m_strInput)<0 )
				 {
					 return TRUE;
				 }
				 if(m_uShowStyle==1||m_uShowStyle==0)  
				 {
					 m_iFocusPos=10;
					 m_uShowStyle=2;

				 }
				 else m_iFocusPos =m_iFocusPos+3 ;
				 if(m_iFocusPos >=11) m_iFocusPos =1;
				 else  if(m_iFocusPos ==2 || m_iFocusPos == 5 ||m_iFocusPos ==8)  m_iFocusPos ++;
			 }
			 else if(nChar == VK_UP ||  nChar == VK_DOWN)
			 {		
				 if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
					 && (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)) 
					 && m_iFrameInPut<0
					 && FrameFromString(m_eVideoType,m_strInput)<0 )
				 {
					 return TRUE;
				 }
				 if(m_iFocusPos ==2 || m_iFocusPos == 5 ||m_iFocusPos ==8 || m_iFocusPos <0 || m_iFocusPos >=11)  return TRUE;
				 if(m_uShowStyle==1)
				 {
					 m_iFocusPos=10;
					 if(nChar == VK_UP) 	    
					 {
						 m_strInput = TP_StringFromFrame(m_eVideoType,AddFrameFromString(m_iFocusPos,1));
						 m_bViewExchange=TRUE;
						 Invalidate();
						 return TRUE;

					 }
					 else if(nChar == VK_DOWN)
					 {
						 if(m_eVideoType == VIDEO_30M)
						 {
							 if(VIDEO_30MReduce())
							 {
								 //m_iFrameInPut=FrameFromString(m_eVideoType,m_strInput);//del lzq:中间状态不要更新数值，失去焦点或回车会更新数值；2010.09.02 NLEbug:1784:
								 Invalidate();
								 return TRUE;
							 }
						 }

						 if(m_strInput=="00:00:00:00" || m_strInput=="00:00.00:00")
						 {
							 int frame=GetMaxFrame(m_eVideoType)-1;
							 m_strInput = TP_StringFromFrame(m_eVideoType,frame);
							 m_bViewExchange=TRUE;
							 Invalidate();
							 return TRUE;
						 }
						 int frame=AddFrameFromString(m_iFocusPos,-1);
						 frame=frame+GetMaxFrame(m_eVideoType);
						 m_strInput = TP_StringFromFrame(m_eVideoType,frame);
						 m_bViewExchange=TRUE;
						 Invalidate();
						 return TRUE;
					 }
				 }
				 if(m_uShowStyle==2)
				 {
					 if(nChar==VK_UP)
					 {
						 AddFrameTo2Bite(m_strInput,1);
					 }
					 if(nChar==VK_DOWN)
					 {
						 AddFrameTo2Bite(m_strInput,-1);
					 }
				 }
				 Invalidate();
				 m_bViewExchange=TRUE;
				 return TRUE;
			 }
			 else if(nChar == 187 ||  nChar == 189)
			 {		
				 m_iFrameInPut = FrameFromString(m_eVideoType,m_strInput);
				 if(nChar == 187) 	  m_strInput = StringFromFrame(m_eVideoType,m_iFrameInPut +1);
				 else if(nChar == 189) m_strInput = StringFromFrame(m_eVideoType,max(m_iFrameInPut-1,0));
			 }
			 else if(nChar == VK_BACK)
			 {
				 if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
					 && (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)) 
					 && m_iFrameInPut<0
					 && FrameFromString(m_eVideoType,m_strInput)<0 )
				 {
					 return TRUE;
				 }
				 if(m_uShowStyle==1 )
				 {
					 if(m_strInput==_T("00:00:00:00") || m_strInput==_T("00:00.00:00") ) return TRUE;
					 CString strTemp;
					 LPTSTR  pstr = m_strInput.GetBuffer(11);
					 pstr[10]  = pstr[9];
					 pstr[9]   = pstr[7];
					 pstr[7]   = pstr[6];
					 pstr[6]   = pstr[4];
					 pstr[4]   = pstr[3];
					 pstr[3]   = pstr[1];
					 pstr[1]   = pstr[0];
					 pstr[0]   ='0';
					 m_strInput.ReleaseBuffer();
					 FunCalcPos(m_iEditMode,TRUE);
					 Invalidate();
					 m_iFocusPos=10;
					 return TRUE;
				 }
				 else if(m_uShowStyle==2)
				 {
					 CString strtemp=m_strInput.Mid(m_iFocusPos-1,1);
					 if(strtemp!=_T("0"))m_strInput.SetAt(m_iFocusPos-1,'0');
					 else m_strInput.SetAt(m_iFocusPos,'0');
				 }
			 }
			 else	return FALSE;
			 FunCalcPos(m_iEditMode);
			 m_bDrawFocus = TRUE;
			 m_bSkipTimer = TRUE;
			 Invalidate();
			 m_bViewExchange=TRUE;
			 return TRUE;
		 }
		 if(m_iEditMode ==1)
		 {
			 if(nChar >='0' && nChar <='9')
			 {   
				 if(m_bViewExchange==FALSE)m_strInput=_T("0");
				 AddCharToFrame(nChar);
				 m_bViewExchange=TRUE;	
				 m_iBeginSel=0;
				 m_iEndSel=10;
				 FunCalcPos(m_iEditMode);
				 m_bDrawFocus          = TRUE;
				 m_bSkipTimer          = TRUE;
				 Invalidate();	
				 return TRUE;

			 }
			 else if(nChar == VK_HOME)
			 {
			 }
			 else if(nChar == VK_END)
			 {
			 }
			 else if(nChar == VK_INSERT)
			 {
				 m_bInsert = GetKeyState(VK_INSERT) & 0x00000001;				
			 }
			 else if(nChar == VK_DELETE)
			 {

				 if(m_iBeginSel !=m_iEndSel  &&  m_iBeginSel>=0 &&  m_iEndSel>=0) 
				 {
					 m_strInput.Delete(min(m_iBeginSel,m_iEndSel),abs(m_iEndSel - m_iBeginSel));
					 m_iFocusPos = min(m_iBeginSel,m_iEndSel);
				 }
				 else 
				 {
					 if(m_iFocusPos <0 || m_iFocusPos >= m_strInput.GetLength()) return TRUE;
					 m_strInput.Delete(m_iFocusPos); 
					 m_iFocusPos = min(m_iFocusPos,m_strInput.GetLength());
					 while(m_strInput.GetLength()>1 && m_strInput[0] =='0') m_strInput = m_strInput.Mid(1);  
				 }
			 }
			 else if(nChar == VK_BACK)
			 {
				 int i=m_strInput.GetLength();
				 m_strInput=m_strInput.Left(i-1);

			 }
			 else  if(nChar == VK_LEFT  )
			 {
			 }
			 else if(nChar == VK_UP ||  nChar == VK_DOWN)
			 {		
				 int num=m_strInput.GetLength();
				 m_iFocusPos=num;
				 if(nChar == VK_UP) 	      
				 {
					 m_strInput.Format(_L("%d"),AddFrameFromNormalString(m_iFocusPos,1));
					 if(StrToInt(m_strInput)==m_lMax+1)m_strInput=_T("0");
				 }
				 else if(nChar == VK_DOWN) 
				 {
					 m_strInput   = m_strInput.Right(m_lMaxLength);
					 __int64 lFrame = StrToInt(m_strInput),ladd=-1;
					 lFrame=lFrame+ladd;
					 if(lFrame<0)				 
						 lFrame=lFrame + TP_GetMaxFrameOfDay(m_eVideoType);				
					 m_strInput.Format(_L("%d"),lFrame);

					  m_strInput   = m_strInput.Right(m_lMaxLength);
					  __int64 lMax = StrToInt(m_strInput);
					  if(lMax < lFrame)
						 m_strInput.Format(_L("%d"),lMax);
					 
				 }
			 }		
			 else if(!TP_IsShiftKeyDown())
			 {
				 m_iBeginSel  = 0;
				 m_iEndSel    = 10;		
				 return FALSE;
			 }

			 if(!TP_IsShiftKeyDown())
			 {
				 m_iBeginSel  = 0;
				 m_iEndSel    = 10;		
			 }
			 m_rtSelText  = CRect(0,0,0,0);

			 m_iFrameInPut	=	 StrToInt(m_strInput);
			 if(!FunVertiftPos(m_iFrameInPut))
			 {
				 m_iFrameInPut =  iFrame;
				 m_strInput    =  sFrame;
				 m_iFocusPos   =  iFocusPos; 
				 return FALSE;
			 }
			 m_iBeginSel=0;
			 m_iEndSel=10;
			 FunCalcPos(m_iEditMode);
			 m_bDrawFocus          = TRUE;
			 m_bSkipTimer          = TRUE;
			 Invalidate();	


			 return TRUE;
		 }
	 }
	 return FALSE;    
 }

 BOOL CTPTimeCodeEditEx::VIDEO_30MReduce()
 {
	 PTSTR pstr=m_strInput.GetBuffer(10);
	 if((/*pstr[3]!='0' ||*/ pstr[4]!='0') && pstr[6]=='0' &&pstr[7]=='0' &&pstr[9]=='0'&& (pstr[10]=='2'|| pstr[10]=='1'))
	 {
		 int num=(pstr[3]-'0')*10+(pstr[4]-'0')-1;
		 pstr[3]=num/10+'0';
		 pstr[4]=num%10+'0'; 
		 pstr[6]='5';
		 pstr[7]='9';
		 pstr[9]='2';
		 pstr[10]='9';
		 m_strInput.ReleaseBuffer();
		 return TRUE;
	 }
	 m_strInput.ReleaseBuffer();
	 return FALSE;
 }
 void CTPTimeCodeEditEx::MoveTheTimeCode(CString &strTimeCode,UINT nChar)
 {
	 LPTSTR str=strTimeCode.GetBuffer(10);
	 int j=0;
	 int k=1;
	 for( ;j<10;)
	 {  
		 if(str[j]!=':'&&str[k]!=':'&& str[j]!='.'&&str[k]!='.')

		 {
			 str[j]=str[k];
			 j++;
			 k++;
		 }
		 else
		 {
			 if(str[j]==':'||str[j]=='.')j++;
			 if(str[k]==':'||str[k]=='.' )k++;
		 }
	 }
	 str[m_iFocusPos]=nChar;
	 strTimeCode.ReleaseBuffer();
 }
 void CTPTimeCodeEditEx::MoveTheTimeCodex(CString &strInput,UINT nChar)
 {
	 LPTSTR str=strInput.GetBuffer(10);
	 if(m_b2BitFlag==FALSE)
	 {
		 str[m_iFocusPos-1]='0';
		 str[m_iFocusPos]=nChar;
		 m_b2BitFlag=TRUE;
	 }
	 else
	 {
		 str[m_iFocusPos-1]=str[m_iFocusPos];
		 str[m_iFocusPos]=nChar;
		 m_iFocusPos=m_iFocusPos+3;
		 if(m_iFocusPos>10)m_iFocusPos=1;
		 m_b2BitFlag=FALSE;

	 }
	 strInput.ReleaseBuffer();


 }
 INT64    CTPTimeCodeEditEx::FrameFromString(VIDEO_TYPE eType,CString strFrame,BOOL bIsAddRemove)    
 {
	 if (strFrame.CompareNoCase(_T("--:--:--:--"))==0
		 ||strFrame.CompareNoCase(_T("--:--.--:--"))==0
		 ||strFrame.CompareNoCase(_T("- - - - - - - -"))==0)
	 {
		 return -1;
	 }
	 
	 return TP_GetFrameFromString(eType,strFrame,bIsAddRemove);
 }
 CString   CTPTimeCodeEditEx::StringFromFrame(VIDEO_TYPE enVideoType,__int64 iFrame)
 {
	 return TP_StringFromFrame(enVideoType,iFrame);
 }
 bool CTPTimeCodeEditEx::FunVertiftPos(__int64 iFrame)
 {
	 m_iFrameInPut          = max(m_iFrameInPut,m_lMin); 
	 m_iFrameInPut          = min(m_iFrameInPut,m_lMax); 
	 if(m_iEditMode == 0)	m_strInput  = StringFromFrame(m_eVideoType,m_iFrameInPut);
	 else                    m_strInput  = GetNormalStringFromFrame(m_eVideoType,m_iFrameInPut);
	 return TRUE;
 }


 int CTPTimeCodeEditEx::GetOffSet()
 {
	 return (int)m_offsetView;
 }

 void CTPTimeCodeEditEx::FunCalcPos(int iMode,BOOL show)
 {

	 CDC      *pDC    = GetDC();
	 CFont *ftOld = pDC->SelectObject(m_pFont); 

	 if(iMode ==0)
	 {
		 CRect     rtClient;
		 CString   csTemp = _T("00:00:00:00");
		 CSize     szText = pDC->GetTextExtent(csTemp); //取得字体的长度

		 GetTimeCodeRect(rtClient);
		 CSize     szTemp = pDC->GetTextExtent(_T("+"));
		 m_shadow         = szTemp.cx;

	//	 if(-1 !=m_strInput.FindOneOf(_T("--")))
		 {
			 m_rtText.left   =  rtClient.Width()-szText.cx;
			 m_rtText.right  = rtClient.Width();
		 }
		// else
		 {
	//		 m_rtText.left   = rtClient.left +(rtClient.Width()-szText.cx)/2;
	//		 m_rtText.right  = m_rtText.left  + szText.cx;
		 }
		 m_rtText.top    = rtClient.top;
		 m_rtText.bottom = rtClient.bottom;
	


		 m_rtFocus       =  m_rtText;

		 if(m_iFocusPos<0 || m_iFocusPos >=11)
		 {
			 m_rtFocus =CRect(0,0,0,0);
		 }
		 else
		 {
			 if(show==FALSE && m_uShowStyle==2)
			 {
				 m_rtFocus=m_rtText;
				 if(m_iFocusPos >=1)
				 {
					 szText = pDC->GetTextExtent(csTemp.Left(m_iFocusPos-1)); 
					 m_rtFocus.OffsetRect(szText.cx,0); 
				 }

				 szText = pDC->GetTextExtent(_T("99")); 

				 m_rtFocus.right =  m_rtFocus.left + szText.cx ;
				 m_rtFocus.InflateRect(1,2,0,2);
			 }
		 }		
		 szText = pDC->GetTextExtent(_T("+")); 
		 if(m_uEditStyle & TP_TIMECODE_NEGATIVE_POS)
		 {
			 m_rtNegative = m_rtText;
			 m_rtNegative.right = m_rtNegative.left + szText.cx;
			 m_rtNegative.OffsetRect(-(szText.cx ),int(-m_iDrawNegOffetVal));
		 }
		 else
		 {
			 m_rtNegative = m_rtText;
			 m_rtNegative.right = m_rtNegative.left + szText.cx;
			 if(m_uEditStyle == TP_TIMECODE_NEGATIVE)// (jyn hujiankang )preroll 20081023 只做显示，不做编辑，所以只有一个宏
				 m_rtNegative.OffsetRect((szText.cx ),int(-m_iDrawNegOffetVal));
			 else
				 m_rtNegative.OffsetRect(-2*(szText.cx ),int(-m_iDrawNegOffetVal));
		 }
	 }
	 else if(iMode ==1)
	 {
		 CRect     rtClient;
		 CString   csTemp = _T("00:00:00:00");

		 GetTimeCodeRect(rtClient);

		 CSize     szText  = pDC->GetTextExtent(csTemp); 
		 CSize     szText1 = pDC->GetTextExtent(m_strInput); 
		 CSize     szText2 = pDC->GetTextExtent(_T("9")); 


		 m_rtText.left   = rtClient.right - szText1.cx  - rtClient.left;
		 m_rtText.top    = rtClient.top;
		 m_rtText.bottom = rtClient.bottom;
		 m_rtText.right  = m_rtText.left +  szText1.cx;

		 m_rtFocus       =  m_rtText;




		 if(m_iFocusPos<0 || m_iFocusPos > m_lMaxLength) 
		 {
			 m_rtFocus =CRect(0,0,0,0);
		 }

		 if(m_iBeginSel != m_iEndSel && m_iEndSel >=0  && m_iBeginSel >=0)
		 {
			 CString   csTemp = _T("00:00:00:00");
			 CSize     szText  = pDC->GetTextExtent(csTemp); 
			 m_rtSelText =m_rtText;
			 m_rtSelText.left=m_rtText.right-szText.cx;
			 m_rtSelText.left=m_rtSelText.left-m_shadow;


			 if(m_rtSelText.right < m_rtSelText.left)
			 {
				 int iTemp         = m_rtSelText.left;
				 m_rtSelText.left  = m_rtSelText.right;
				 m_rtSelText.right = iTemp;
			 }
		 }
		 else 
		 {
			 m_rtSelText = CRect(0,0,0,0);
		 }
		

		 szText = pDC->GetTextExtent(_T("+")); 
		 m_rtNegative = m_rtSelText;
		 m_rtNegative.right = m_rtNegative.left + szText.cx;
		 if(m_uEditStyle & TP_TIMECODE_NEGATIVE_POS ){m_rtNegative.OffsetRect(0,int(-m_iDrawNegOffetVal));}
		 else m_rtNegative.OffsetRect(-(szText.cx ),int(-m_iDrawNegOffetVal));
	 }


	 pDC->SelectObject(ftOld); 
	 ReleaseDC(pDC);
	 if(m_iDrawTimeCodeOffsetVal!=0 &&!(m_uEditStyle & TP_TIMECODE_SHOW_MID) )//如果有TP_TIMECODE_SHOW_MID宏就忽略此参数
	 {
		 m_rtSelText.OffsetRect(int(-m_iDrawTimeCodeOffsetVal),0);
		 m_rtNegative.OffsetRect(int(-m_iDrawTimeCodeOffsetVal),0);
		 m_rtFocus.OffsetRect(int(-m_iDrawTimeCodeOffsetVal),0);
		 m_rtText.OffsetRect(int(-m_iDrawTimeCodeOffsetVal),0);
	 }
	 if(m_uEditStyle & TP_TIMECODE_SHOW_MID)//时码永远显示在窗口中间
	 {
		 CRect rect;
		 int ioffsetVaule = 0,temp;
		 GetClientRect(&rect);
		 if(m_uEditStyle & TP_TIMECODE_LOGO)
		 {
           rect.left+=TP_TIMECODE_LOGO_WIDTH;
		 }
		 ioffsetVaule = (rect.right - rect.left)/2;
		 temp = (m_rtText.right - m_rtText.left)/2;
		 ioffsetVaule -=temp;

		 m_rtSelText.OffsetRect(int(-ioffsetVaule),0);
		 m_rtNegative.OffsetRect(int(-ioffsetVaule),0);
		 m_rtFocus.OffsetRect(int(-ioffsetVaule),0);
		 m_rtText.OffsetRect(int(-ioffsetVaule),0);
	 }
 }
void CTPTimeCodeEditEx::OnLButtonDown(UINT nFlags, CPoint point)
{	
	if(m_b2BitFlag==TRUE)m_b2BitFlag=FALSE;
	if( m_hWnd!=::GetFocus() ) 
	{
		m_iFocusPos=10;
		m_uShowStyle=1;
		m_iBeginSel  = 0;
		m_iEndSel    = 10;
		m_bFocus=TRUE;
		if(m_uEditStyle & TP_TIMECODE_POPINPUT) 
		{
			goto BBQ;
		}

		SetFocus();
		if(m_iEditMode==0)m_rtFocus=m_rtText;
		else  FunCalcPos(m_iEditMode);
		Invalidate();
	}
	else
	{
		if((!(m_uEditStyle & TP_TIMECODE_NEGATIVE)) 
			&& (!(m_uEditStyle & TP_TIMECODE_POSITIVE_TEXT)) 
			&& m_iFrameInPut<0
			&& FrameFromString(m_eVideoType,m_strInput)<0 )
		{
			if(m_strInput!=_T("--:--:--:--") && m_strInput!=_T("--:--.--:--") && m_strInput!=_T("- - - - - - - -"))
			{
				m_iFrameInPut =0;
				goto BBQ;

			}

	


			m_iFocusPos=10;
			m_iBeginSel  = 0;
			m_iEndSel    = 10;
			m_bFocus=TRUE;
			m_bDrawFocus=TRUE;
			if(m_iEditMode==0)
			{
				m_rtFocus=m_rtText;
				m_uShowStyle=1;
			}
			FunCalcPos(m_iEditMode,TRUE);
			Invalidate();
			return ;
		}
BBQ:

		if(m_iEditMode==0) m_uShowStyle = 2;
		else               m_uShowStyle = 1;
		CRect  rtClient;
		CDC   *pDC   = GetDC();

		CFont *ftOld  = pDC->SelectObject(m_pFont); 
		CSize  size1  = pDC->GetTextExtent(_T("9"));
		CSize  size2  = pDC->GetTextExtent(_T(":"));
		pDC->SelectObject(ftOld); 
		ReleaseDC(pDC);

		GetTimeCodeRect(rtClient);

		point.x -= rtClient.left;

		if(m_iEditMode == 0)
		{
			if(point.x <=   m_rtText.left)  m_iFocusPos = 1;
			else if(point.x >=   m_rtText.right )  m_iFocusPos = 10;
			else
			{
				int icx = m_rtText.left;
				for(int i=0;i<=11;i++)
				{
					if(point.x<icx) 
					{
						m_iFocusPos = i-1;
						break;
					}
					else if(i==2 || i==5 ||i==8) icx +=size2.cx;
					else icx +=size1.cx;
				}
				if(m_iFocusPos==2 || m_iFocusPos==5 ||m_iFocusPos==8) m_iFocusPos ++;

				switch(m_iFocusPos) {
			case 0:
			case 3:
			case 6:
			case 9:
				m_iFocusPos=m_iFocusPos+1; break;
			default:break;
				}
			}
		}
		else
		{

		}
		m_bDrawFocus          = TRUE;
		m_bSkipTimer          = TRUE;
		m_bFocus=TRUE;
		m_iBeginSel  = 0;
		m_iEndSel    = 10;	

		FunCalcPos(m_iEditMode);
		Invalidate();
		m_uState |= TIMECODE_LBUTTONDOWN;

	}
	CTPWnd::OnLButtonDown(nFlags, point);
}

BOOL CTPTimeCodeEditEx::PreTranslateMessage(MSG* pMsg)
{
	
	if(m_uEditStyle & TP_TIMECODE_DISABLE && (pMsg->message!=WM_PAINT &&pMsg->message!=WM_MOUSEMOVE))
	{
		return TRUE;
	}
	if(m_uEditStyle & TP_TIMECODE_SHOW_ONLY && (pMsg->message!=WM_PAINT && pMsg->message!=WM_RBUTTONDOWN &&  pMsg->message!=WM_COMMAND))
	{
		return TRUE;
	}

	if( pMsg->message == WM_KEYDOWN /*|| pMsg->message == WM_CHAR*/)
	{
		if(VK_C == pMsg->wParam)
		{
			int j = 0;
			j = 1;
		}
		if(TP_IsCtrlKeyDown() || TP_IsShiftKeyDown()) return FALSE;//Ctrl and Shift 按下的时候不处理
		BOOL bReturn = FunKeyDown((UINT)pMsg->wParam);
		if(bReturn) return TRUE;
		else if(pMsg->hwnd != NULL) return CTPWnd::PreTranslateMessage(pMsg);		
		else return FALSE;
	}
	else if(pMsg->hwnd != NULL) return CTPWnd::PreTranslateMessage(pMsg);
	else return FALSE;
}

void CTPTimeCodeEditEx::OnSize(UINT nType, int cx, int cy)
{
	CTPWnd::OnSize(nType, cx, cy);
	FunCalcPos(m_iEditMode);	
}

void CTPTimeCodeEditEx::OnTimer(UINT nIDEvent)
{
	if(m_bSkipTimer)
	{
		m_bSkipTimer =FALSE;
	}
	else
	{
		m_bDrawFocus = !m_bDrawFocus;
		if(GetFocus() != this)
		{
			m_bDrawFocus = FALSE;
			KillTimer(10);
			Invalidate();
		}

		if(m_bFocus)
		{	
			CDC *pDC = GetDC();
			if(m_rtSelText.Width()>0&&m_rtSelText.right>m_rtText.left&&m_rtSelText.left<m_rtText.right)
				pDC->InvertRect(m_rtSelText); 	
			if(m_bDrawFocus)
				pDC->InvertRect(m_rtFocus); 
			ReleaseDC(pDC);
		}
	}
	CTPWnd::OnTimer(nIDEvent);
}

void CTPTimeCodeEditEx::OnSetFocus(CWnd* pOldWnd)
{

	CTPWnd::OnSetFocus(pOldWnd);
	m_bDrawFocus = TRUE;
	m_bInvalid=TRUE;

	if(m_uEditStyle&TP_TIMECODE_POPINPUT && m_iFrameInPut<0) m_iFrameInPut=0;


	if(m_iFocusPos <0||m_iFocusPos>m_strInput.GetLength())
		m_iFocusPos           = m_strInput.GetLength()-1;

	m_uShowStyle=1;
	m_iSetFocusFrame      = m_iFrameInPut;
	m_iFocusUndoPos       = m_iFocusPos; 	
	m_rtFocus=m_rtText;
	Invalidate();
	m_bFocus = TRUE;
    m_bDrawFocus = TRUE;

	GetOwner()->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),EN_SETFOCUS ),LPARAM(m_hWnd));
}

void CTPTimeCodeEditEx::OnKillFocus(CWnd* pNewWnd)
{
	CTPWnd::OnKillFocus(pNewWnd);

    //////////////判断当前输入的字符串是否合法,如果合法,将其转化成m_iframeinput,m_bInvalid为TRUE.
    //////////////反之,m_iframeinput无效,m_bInvalid为false.
	__int64 tempUndoFrame=m_iabsFrame;
	if(TestTheString(m_strInput)&&tempUndoFrame!=m_iabsFrame)
	{
		m_iUnDoFrame=tempUndoFrame;
	}
	else if(m_uEditStyle&TP_TIMECODE_POPINPUT  )  m_iFrameInPut=-1;

	//////////////////////符号存档,并将符号显示标志置零
	m_boffSet=m_bLable;
	m_boffsetNeg=m_bNegative;
	if((m_boffsetNeg || !m_boffSet) && m_bInvalid)
	{
		m_offsetView=m_iFrameInPut;
	}

	if(!(m_uEditStyle & TP_TIMECODE_NEGATIVE))
	{
		m_bLable    =TRUE;
		m_bNegative =FALSE;
	}

	//lzq:帧模式转换为时码模式。2009.09.28
	if (m_bFrameToTC && m_bTimeView) 
	{
		if(m_iFrameInPut<0) m_iFrameInPut = 0;//lzq:当m_iFrameInPut<0时，不让转换。所以这里修正一下。2009.09.29
		SetEditMode(0);
	}

	//lzq: 控件自己计算‘+/-’，并把计算后的结果设为当前值。2009.08.28
	if(m_bSelCalc && m_bInvalid)  FunCalcFrame(); 	

	//////////////////////
    //////////////////// 其他归位标志
	m_uShowStyle=0;

	KillTimer(10);
	m_boffsetvalueshow    =TRUE;
	m_bViewExchange       =FALSE;
	m_bDrawFocus          = FALSE;
	m_bSkipTimer          = TRUE;
	m_iFocusPos           =10;
	m_iBeginSel           = 0;
	m_iEndSel             = 10;	
	FunCalcPos(m_iEditMode,TRUE);
	m_rtSelText           = CRect(0,0,0,0);
	m_bFocus              = FALSE; 
	Invalidate();
	if(m_iSetFocusFrame!= m_iFrameInPut || OffSet()==1 )
	{
		GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_EDITEND,(LPARAM)this); 
	}
		
	GetOwner()->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),EN_KILLFOCUS ),LPARAM(m_hWnd));
}
void CTPTimeCodeEditEx::SetEditStyle(UINT uStyle,BOOL flag)
{
	if(flag)m_uEditStyle |= uStyle;
	else    m_uEditStyle = uStyle;
}

BOOL CTPTimeCodeEditEx::TestTwoBitString()
{
	int num =m_strInput.GetLength();
	PTSTR pstr=m_strInput.GetBuffer(10);
	for(int i=0;i<num-2;)
	{
		if(pstr[i]=='0' || pstr[i]==':' || pstr[i]=='.') i++;
		else
		{
			m_strInput.ReleaseBuffer();
			return FALSE;
		}
	}
	int sum=(pstr[9]-'0')*10+(pstr[10]-'0');
	//wxd  snp4100032796 只对蓝光窗口的TimeCode做判断
	if(m_bSelCalc)
	{
		m_boffSet=m_bLable;
		m_boffsetNeg=m_bNegative;
		if(m_boffsetNeg || !m_boffSet)
		{
			if(sum*(m_boffsetNeg?-1:1)+m_iLastInput>m_lMax || sum*(m_boffsetNeg?-1:1)+m_iLastInput<m_lMin)
				return FALSE;
		}
		else
		{
			if(sum>m_lMax || sum<m_lMin)
				return FALSE;
		}
	}
	
	m_iFrameInPut=sum;
	if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE) 
	{
		m_iabsFrame   =(sum+m_iCTLFrame)%GetMaxFrame(m_eVideoType);
	}
	else m_iabsFrame  =sum;
	m_strInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
	m_strInput.ReleaseBuffer();
	return TRUE;
}


BOOL CTPTimeCodeEditEx::TestTheString(CString &strtest)
{
	
	if(m_strInput == _T("--:--:--:--") || m_strInput == _T("--:--.--:--") || m_strInput == _T("- - - - - - - -")) return TRUE;
	int flag=strtest.Find(':');

	if(flag!=-1)
	{
		__int64 Frame= FrameFromString(m_eVideoType,strtest);
		if(m_uEditStyle &TP_TIMECODE_OldNew || m_eVideoType==VIDEO_30M)
		{
			PTSTR pstr=m_strInput.GetBuffer(10);
			if(m_lMin<=Frame && Frame<=m_lMax && (pstr[4]!='0') && ((pstr[6]=='0' &&pstr[7]=='0' &&pstr[9]=='0'&& pstr[10]=='1')||(pstr[6]=='0' &&pstr[7]=='0' &&pstr[9]=='0'&& pstr[10]=='0'))) // snp4100032792 Wangjin 20110412
			{
				if(m_uEditStyle&TP_TIMECODE_30M_DURATION)
				{
					pstr[4]=pstr[4]-1;
					pstr[6]='5';
					pstr[7]='9';
					pstr[9]='2';
					if(pstr[10]=='1')	pstr[10]='9';
					else				pstr[10]='8';
				}
				m_strInput.ReleaseBuffer();
				//BOOL bIsAddRemove = FALSE;
				//if( m_bSelCalc && m_bInvalid) bIsAddRemove = TRUE;
				m_iFrameInPut=FrameFromString(m_eVideoType,strtest);//,bIsAddRemove);
				m_strInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
				return TRUE;
			}
		}

		if(TestTwoBitString()) return TRUE;
		CString strtemp=StringFromFrame(m_eVideoType,Frame);
		if(strtemp==strtest && m_lMin<=Frame && Frame<=m_lMax)
		{
			m_iFrameInPut=Frame;
			if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE)
			{
				m_iabsFrame=(Frame+m_iCTLFrame)%GetMaxFrame(m_eVideoType);
			}
			else{m_iabsFrame=Frame;}
			return TRUE;
		}
		else 
		{
			if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE)
			{
				m_iFrameInPut=(Frame+m_iFrameInPut)%GetMaxFrame(m_eVideoType);
			}
			m_strInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
			m_bInvalid=FALSE;
			//added by chenxiaolong 2009-08-24
			if(m_bBeep)
			{
			//	TP_WarningMessageBeep(1);
			}	
			return FALSE;
		}
	}
	else
	{
		int i=strtest.GetLength();
		LPTSTR pstr=strtest.GetBuffer(10);
		int frame=0;
		for (int j=0;j<i;j++)
		{
			frame=(frame*10+(pstr[j]-'0'));
		}
        strtest.ReleaseBuffer();
		if(frame<=m_lMax && frame>=m_lMin)
		{
			m_iFrameInPut= frame;
			m_iabsFrame  = frame;
			return TRUE;
		}

		else{
			m_strInput=GetNormalStringFromFrame(m_eVideoType,m_iFrameInPut);
			m_bInvalid=FALSE;
			//added by chenxiaolong 2009-08-24
			if(m_bBeep)
			{
			//	TP_WarningMessageBeep(1);
			}	
			return  FALSE;
		}

	}
}

void CTPTimeCodeEditEx::SetEditMode(int iMode)
{
	m_iEditMode = iMode;
	if(m_iEditMode==1)  
	{	
		if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE)SetCTL(FALSE);
        
		BOOL testFrame =TRUE;
		if(m_strInput==_T("--:--:--:--") || m_strInput==_T("--:--.--:--") || m_strInput==_T("- - - - - - - -"))
		{
			testFrame=FALSE;
		}

		if(m_iFrameInPut>=0 || testFrame)
		{
			__int64 tempUndoFrame=m_iabsFrame;
			if(TestTheString(m_strInput)&&tempUndoFrame!=m_iabsFrame)
			{
				m_iUnDoFrame=tempUndoFrame;
			}
			m_bViewExchange=FALSE;
			m_uShowStyle=1;
			/*jyn added 20091102
			*@brief:帧显示时码时，需要取天的余数
			*/
			INT64 i64TemFrame = m_iFrameInPut%GetMaxFrame(m_eVideoType);
			/*jyn added end*/
			m_strInput = GetNormalStringFromFrame(m_eVideoType,i64TemFrame);
		}

		else  m_strInput=_T("- - - - - - - -");



		if(m_iFocusPos <0 || m_iFocusPos >= m_strInput.GetLength()) 
		m_iFocusPos=1;
		m_iBeginSel  = 0;
		m_iEndSel    = m_strInput.GetLength();		


		m_bTimeView=TRUE;


	}
	else if(m_iEditMode ==0)
	{   
		if(m_iFrameInPut>=0)
		{
			__int64 tempUndoFrame=m_iabsFrame;
			if(TestTheString(m_strInput)&&tempUndoFrame!=m_iabsFrame)
			{
				m_iUnDoFrame=tempUndoFrame;
			}

			m_strInput = StringFromFrame(m_eVideoType,m_iFrameInPut);
		}   
		else
		{
			m_strInput=_T("--:--:--:--");
		}
		m_bViewExchange=FALSE;
		m_uShowStyle=1;
		m_iFocusPos =   m_strInput.GetLength()-1;
		if(m_iFocusPos ==2 || m_iFocusPos == 5 ||m_iFocusPos ==8)  m_iFocusPos ++;
		m_iBeginSel  = 0;
		m_iEndSel    = 10;		
		m_bTimeView=FALSE;

	}  
	if(this == GetFocus())
		m_bDrawFocus          = TRUE;

	//{{ lzq:如果不是当前焦点，则把符号清空,bug:XN00026042,2009.12.04
	if (!m_bDrawFocus)
	{
		m_uShowStyle		= 0;
		m_boffsetvalueshow  = TRUE;
		m_bViewExchange     = FALSE;		
		m_bSkipTimer        = TRUE;//在OnTimer中不执行，跳过一次OnTimer,第二次再执行。
		m_iFocusPos         = 10;
		m_iBeginSel         = 0;
		m_iEndSel           = 10;			
		m_rtSelText         = CRect(0,0,0,0);
		m_bFocus            = FALSE; 
	}
	//}}

	m_bSkipTimer          = TRUE;
	FunCalcPos(m_iEditMode,TRUE);
	Invalidate();
}

void CTPTimeCodeEditEx::SetEditModeNoChange(int iMode)
{


	m_iEditMode = iMode;

	if(m_iEditMode==1)    
	{		
		SwitchTestShow(m_strInput,0);
		if(m_iFocusPos <0 || m_iFocusPos >= m_strInput.GetLength()) 
			m_iFocusPos =   m_strInput.GetLength()-1;
		m_iBeginSel  = 0;
		m_iEndSel    = 10;		
		m_bTimeView=TRUE;


	}
	else if(m_iEditMode ==0)
	{   		
		SwitchTestShow(m_strInput,1);
		m_iFocusPos =   m_strInput.GetLength()-1;
		if(m_iFocusPos ==2 || m_iFocusPos == 5 ||m_iFocusPos ==8)  m_iFocusPos ++;
		m_iBeginSel  = 0;
		m_iEndSel    = 10;		
		m_bTimeView=FALSE;
	}  
	m_bDrawFocus          = TRUE;
	m_bSkipTimer          = TRUE;
	FunCalcPos(m_iEditMode,TRUE);
	Invalidate();
}

void CTPTimeCodeEditEx::OnMouseMove(UINT nFlags, CPoint point)
{
	CTPWnd::OnMouseMove(nFlags, point);

	if((m_uEditStyle & TP_TIMECODE_DRAG) && m_iEditMode == 0 )
	{
		TestTheString(m_strInput);
		if(m_ptSave.y  - point.y >0)      FunKeyDown(VK_UP);
		else if(m_ptSave.y  - point.y <0) FunKeyDown(VK_DOWN);
		m_ptSave  = point;
		GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_CHANGE,(LPARAM)this); 
	}
	else
	{
		if(!(nFlags & MK_LBUTTON)) m_uState &=~TIMECODE_LBUTTONDOWN;
		if(m_iEditMode == 1 && (m_uState&TIMECODE_LBUTTONDOWN) && m_ptSave.x !=point.x)
		{		
			CRect rtClient;  
			CDC   *pDC   = GetDC();
			CFont *ftOld = pDC->SelectObject(m_pFont); 
			CSize  size1  = pDC->GetTextExtent(_T("9"));
			CSize  size2  = pDC->GetTextExtent(_T(":"));
			pDC->SelectObject(ftOld); 
			ReleaseDC(pDC);
			GetTimeCodeRect(rtClient);


			int iFocusPos = 0;

			point.x -= rtClient.left;

			if(point.x <=   m_rtText.left)         iFocusPos = 0;
			else if(point.x >=   m_rtText.right )  iFocusPos = m_strInput.GetLength();
			else                                   iFocusPos = (point.x - m_rtText.left)/size1.cx; 

			if(iFocusPos !=m_iFocusPos)
			{
				m_iFocusPos  = iFocusPos;

				if(m_iEndSel == m_iBeginSel)
				{
					if(m_iFocusPos <m_iEndSel) m_bFirstBegin = TRUE;
					else m_bFirstBegin = FALSE;
				}
				if(m_bFirstBegin) m_iBeginSel = m_iFocusPos;
				else m_iEndSel = m_iFocusPos;

				m_bDrawFocus          = TRUE;
				m_bSkipTimer          = TRUE;
				FunCalcPos(m_iEditMode);
				Invalidate();
			}

			point.x += rtClient.left;
		}

		if(m_iEditMode == 0 && (m_uState&TIMECODE_LBUTTONDOWN) && m_ptSave.y !=point.y)
		{		
			m_uEditStyle |= TP_TIMECODE_DRAG;
			SetCapture();
		}
		m_ptSave  = point;
	}
}
BOOL CTPTimeCodeEditEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	__int64 iFrame = m_iFrameInPut;
	if(zDelta <0)      FunKeyDown(VK_UP);	
	else if(zDelta > 0) FunKeyDown(VK_DOWN);	

	return TRUE;
	
}

void CTPTimeCodeEditEx::DisableCover()
{
	m_bDrawFocus = FALSE;
	m_rtSelText = CRect(0,0,0,0);
	m_rtFocus	= CRect(0,0,0,0);
	return;
}

void CTPTimeCodeEditEx::OnNcPaint()
{
	g_pRoundCorner ->m_dwWindowLevel = m_dwWindowLevel;
	g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}
void CTPTimeCodeEditEx::SetTextColor(TP_SYSTEM_COLOR eColorIndex)
{
	m_iTextColor = eColorIndex;
}
void CTPTimeCodeEditEx::SetBackColor(TP_SYSTEM_COLOR eColorIndex)
{
	m_iBackColor = eColorIndex;

}
void CTPTimeCodeEditEx::SetRange(__int64 lMin,__int64 lMax)
{
	m_lMin = lMin;
	m_lMax = lMax;
}

void CTPTimeCodeEditEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	

	if((m_uEditStyle & TP_TIMECODE_DRAG)) 	ReleaseCapture();
	m_uState     &= ~TIMECODE_LBUTTONDOWN;
	m_uEditStyle &= ~TP_TIMECODE_DRAG;
	CTPWnd::OnLButtonUp(nFlags, point);
}

void CTPTimeCodeEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(GetKeyState(VK_CONTROL) <0 || m_bCtlBtn==TRUE)
	{
		m_bCtlBtn=FALSE;
		if(nChar == VK_C)
		{
			m_iCopyFrame = GetFrame();//m_iabsFrame;///XN00027210
		}
		if(nChar == VK_V)
		{
			if(m_iCopyFrame <0) return;	  			
			if(m_iCopyFrame>= m_lMin && m_iCopyFrame<=m_lMax)
			{				
				m_iUnDoFrame = m_iabsFrame;
				SetFrame(m_iCopyFrame);		
				if(m_iUnDoFrame    != m_iabsFrame)
				{
					GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_CHANGE,(LPARAM)this); 
				}
			}			
		}
		if(nChar == VK_X)
		{
			m_iUnDoFrame = m_iabsFrame;
			m_iCopyFrame = GetFrame();//m_iabsFrame;
			SetFrame(0);
			if(m_iUnDoFrame   != m_iabsFrame)
				GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_CHANGE,(LPARAM)this); 
		}
		if(nChar == VK_Z)
		{
			if(m_iUnDoFrame<0) return ;
			int tempUndoFrame=(int)m_iabsFrame;
			SetFrame(m_iUnDoFrame);
			//m_iUnDoFrame = -1;	//XN00018442
			m_iUnDoFrame = tempUndoFrame;
			if(tempUndoFrame   != m_iabsFrame)
				GetParent()->SendMessage(WM_TIMECODE_CHANGE,TP_TIMECODE_CHANGE,(LPARAM)this); 
		}
	
	}
	if(nChar==VK_MULTIPLY )
	{
		//*号准备切换视图
		if(m_bViewExchange && !(m_uEditStyle & TP_TIMECODE_NSCTLTYPE))
		{
			BOOL m_bView=!m_bTimeView;
			SetEditModeNoChange(m_bView);
			if(m_bView)
				GetParent()->SendMessage(WM_TIMECODE_THIS,TP_TIMECODE_FRAMEVIEW,(LPARAM)this);
			else
				GetParent()->SendMessage(WM_TIMECODE_THIS,TP_TIMECODE_FTCVIEW,(LPARAM)this);
		}
	}

	CTPWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CTPTimeCodeEditEx::SwitchTestShow(CString &strShow,UINT flag)
{
  if(flag==0)
  {
	  int len=0;
	  strShow.Remove(':');
	  if(m_eVideoType == VIDEO_30M)strShow.Remove('.');
	  int pos=strShow.Find('0');
	  while(pos==0)
		{
			len=strShow.GetLength();
			if(len==1) return;
			strShow=strShow.Right(len-1);
			pos=strShow.Find('0');
		} 

  }
  else if(flag==1)
  {
	  while(strShow.GetLength()!=8)
	  {
		  strShow.Insert(0,'0');
	  }
	  strShow.Insert(2,':');
	  if(m_eVideoType == VIDEO_30M)strShow.Insert(5,'.');
	  else strShow.Insert(5,':');
 
	  strShow.Insert(8,':');

  }
  else return;
  
}
void CTPTimeCodeEditEx::GetTimeCodeRect(CRect &rtTimeCode)
{
	GetClientRect(&rtTimeCode);
}

void CTPTimeCodeEditEx::SetCTL(BOOL bSet)
{
	m_boffSet=m_bLable;
	m_boffsetNeg=m_bNegative;
  if(bSet)
  {
	  if(!(m_uEditEnable& TP_TIMECODE_NSCTLTYPE))  return;
	  if(!(m_uEditStyle & TP_TIMECODE_NSCTLTYPE))
	  {	  
		  if(m_iEditMode==0)
		  {
			  if(OffSet())
			  {
				  m_uEditStyle |=  TP_TIMECODE_NSCTLTYPE;

				  if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE) m_iTextColor = COLOR_TL_TIMECODECTL;
				  else                                     m_iTextColor = COLOR_TL_TIMECODE;

				  FunCalcPos(m_iEditMode,TRUE);
				  Invalidate();
				  return;
			  }
			  if(m_iFrameInPut > 0)
			  {
				  m_iFrameInPut=m_iFrameInPut-m_iCTLFrame;
				  if(m_iFrameInPut<0)m_iFrameInPut=GetMaxFrame(m_eVideoType)+m_iFrameInPut;
				  m_strInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
			  }
		  }
		  else
		  {
			  if(OffSet())
			  {

				  SetEditMode(0);
				  m_uEditStyle |=  TP_TIMECODE_NSCTLTYPE;

				 
				  if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE) m_iTextColor = COLOR_TL_TIMECODECTL;
				  else                                     m_iTextColor = COLOR_TL_TIMECODE;

				  FunCalcPos(m_iEditMode,TRUE);
				  Invalidate();
				  return;
			  }
			  SetEditMode(0);
			  if(m_iFrameInPut > 0)
			  {
				  m_iFrameInPut=m_iFrameInPut-m_iCTLFrame;
				  if(m_iFrameInPut<0)m_iFrameInPut=GetMaxFrame(m_eVideoType)+m_iFrameInPut;
				  m_strInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
			  }
		  }
		  m_uEditStyle |=  TP_TIMECODE_NSCTLTYPE;
		  m_iUnDoFrame = -1;
	  }

  }
  else
  {
	 
	  if(!(m_uEditStyle & TP_TIMECODE_NSCTLTYPE)) 
	  {
		  if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE) m_iTextColor = COLOR_TL_TIMECODECTL;
		  else                                     m_iTextColor = COLOR_TL_TIMECODE;

		  if(m_iEditMode==1)
		  {
			  SetEditMode(0);
			  return;
		  }
		  else return;
	  }
	  m_uEditStyle &= ~TP_TIMECODE_NSCTLTYPE;
	  if(OffSet())
	  {
		  
		  if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE) m_iTextColor = COLOR_TL_TIMECODECTL;
		  else                                     m_iTextColor = COLOR_TL_TIMECODE;

		  FunCalcPos(m_iEditMode,TRUE);
		  Invalidate();
		  return;
	  }
	  m_iFrameInPut = m_iCTLFrame+max(FrameFromString(m_eVideoType,m_strInput),0);
	  m_strInput    = StringFromFrame(m_eVideoType,m_iFrameInPut); 
  }
  if(m_uEditStyle & TP_TIMECODE_NSCTLTYPE) m_iTextColor = COLOR_TL_TIMECODECTL;
  else                                     m_iTextColor = COLOR_TL_TIMECODE;

  FunCalcPos(m_iEditMode);
  Invalidate();

}



void CTPTimeCodeEditEx::ResetCTL()
{
	
	if(!(m_uEditEnable& TP_TIMECODE_NSCTLTYPE))  return;

	m_boffSet=m_bLable;
	m_boffsetNeg=m_bNegative;


	if(m_uEditStyle&TP_TIMECODE_EDITTYPEDIGITIZE)
	{
		m_iCTLFrame=m_iFrameInPut=0;
		if(m_eVideoType == VIDEO_30M)
			m_strInput       = _T("00:00.00:00");
		else
			m_strInput       = _T("00:00:00:00");
	}
	else
	{	
		if( m_iEditMode==0)
		{

			if(!OffSet()) 
			{
				if(m_iabsFrame >= 0)
				{
					if(!(m_uEditStyle&TP_TIMECODE_NSCTLTYPE))
					{
						TestTheString(m_strInput); 
						if(m_eVideoType == VIDEO_30M)
							m_strInput       = _T("00:00.00:00");
						else
							m_strInput       = _T("00:00:00:00");
						m_iCTLFrame=m_iFrameInPut;
					}
					else
					{              
						TestTheString(m_strInput);
						m_iCTLFrame+=m_iFrameInPut;
						m_iCTLFrame%=(GetMaxFrame(m_eVideoType));
						if(m_eVideoType == VIDEO_30M)
							m_strInput       = _T("00:00.00:00");
						else
							m_strInput       = _T("00:00:00:00");
					}
				}
			}
			else
			{}

		}
		else 
		{
			if(!OffSet())
			{
				SetEditMode(0);
				if(m_iabsFrame >= 0)
				{
					if(m_eVideoType == VIDEO_30M)
						m_strInput=_T("00:00.00:00");
					else
						m_strInput=_T("00:00.00:00");
				}
			}
			else
			{
				__int64 tempframeInput=m_iFrameInPut;
				SetEditMode(0);
				m_iFrameInPut=tempframeInput;

			}
			m_iCTLFrame=m_iFrameInPut;
		}
	}
	m_uEditStyle &= ~TP_TIMECODE_NSALLTYPE;
	m_uEditStyle |=  TP_TIMECODE_NSCTLTYPE;
	m_iTextColor = COLOR_TL_TIMECODECTL;
	m_iUnDoFrame = -1;

	FunCalcPos(m_iEditMode,TRUE);
	Invalidate();

}


void CTPTimeCodeEditEx::SetTimeCodeStyle(INT64 uStyle,BOOL bSendChangeMsg)
{
	if(!(uStyle & m_uEditEnable)) return;
	INT64 uEditStyle = m_uEditStyle;
	m_uEditStyle &= ~TP_TIMECODE_NSALLTYPE;
	m_uEditStyle    |= uStyle;
	m_bSendChangeMsg = FALSE;
	if((uStyle & TP_TIMECODE_NSFTCTYPE)       && !(uEditStyle & TP_TIMECODE_NSFTCTYPE)) 
	{		
		if(bSendChangeMsg)	PostMessage(WM_COMMAND, ID_TIMECODEVIEW_FTC,NULL);
		else                SendMessage(WM_COMMAND, ID_TIMECODEVIEW_FTC,NULL);
	}
	else if((uStyle & TP_TIMECODE_NSCTLTYPE)  && !(uEditStyle & TP_TIMECODE_NSCTLTYPE))  
	{
		if(bSendChangeMsg)	PostMessage(WM_COMMAND, ID_TIMECODEVIEW_CTL,NULL);
		else                SendMessage(WM_COMMAND, ID_TIMECODEVIEW_CTL,NULL);
	}
	else if((uStyle & TP_TIMECODE_NSLTCTYPE)  && !(uEditStyle & TP_TIMECODE_NSLTCTYPE)) 
	{
		if(bSendChangeMsg)	PostMessage(WM_COMMAND, ID_TIMECODEVIEW_LTC,NULL);
		else                SendMessage(WM_COMMAND, ID_TIMECODEVIEW_LTC,NULL);
	}
	else if((uStyle & TP_TIMECODE_NSVITCTYPE) && !(uEditStyle & TP_TIMECODE_NSVITCTYPE)) 
	{
		if(bSendChangeMsg)	PostMessage(WM_COMMAND, ID_TIMECODEVIEW_VITC,NULL);
		else                SendMessage(WM_COMMAND, ID_TIMECODEVIEW_VITC,NULL);
	}
	else if((uStyle & TP_TIMECODE_NSTCTYPE)   && !(uEditStyle & TP_TIMECODE_NSTCTYPE)) 
	{
		if(bSendChangeMsg)	PostMessage(WM_COMMAND, ID_TIMECODEVIEW_TC,NULL);
		else                SendMessage(WM_COMMAND, ID_TIMECODEVIEW_TC,NULL);		
	}
	m_bSendChangeMsg = TRUE;
}
void CTPTimeCodeEditEx::AjustFocus(int &focus)
{
	switch(focus)
	{
	case 1:focus=0;
		break;
	case 4:focus=3;
		break;
	case 7:focus=6;
	break;
	case 10:focus=9;
		break;
	default:
		break;
	}
}

BOOL CTPTimeCodeEditEx::OffSet( )
{
  if(m_boffsetNeg || !m_boffSet)
  {
	  return TRUE;
  }
  else return FALSE;
}

long CTPTimeCodeEditEx::OffsetType4()
{
	if(m_boffsetNeg || !m_boffSet)
	{
		if(m_boffsetNeg) return 2;
		else          return 1;
	}
	else return 0;
}


void CTPTimeCodeEditEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	m_iFocusPos=10;
	m_uShowStyle=1;
	m_rtFocus=m_rtText;
	Invalidate();
	__super::OnLButtonDblClk(nFlags, point);
}
int CTPTimeCodeEditEx::GetMaxFrame(VIDEO_TYPE Video_Type)
{

	int nf=0;
	switch(Video_Type)
	{
	case VIDEO_30M:
	case VIDEO_30: nf=30;break;
	case VIDEO_25:
	case VIDEO_SECAM: nf=25;break;
	}
	return (TP_GetFrameFormTime(23,59,59,nf,Video_Type));

}
void CTPTimeCodeEditEx::AddFrameTo2Bite(CString &str,int num)
{
   int maxnum=0;

   int result=GetFrame2Bite(m_iFocusPos,maxnum);
   result=result+num;
   if(result>=0)result=result%maxnum;
   else{
	   result=result+maxnum;
   }
   LPTSTR pstr=str.GetBuffer(10);
   pstr[m_iFocusPos-1]='0'+result/10;
   pstr[m_iFocusPos]='0'+result%10;
   str.ReleaseBuffer();


}
int CTPTimeCodeEditEx::GetFrame2Bite(int m_iFocusPos,int &maxnum)
{
   
   int num=(m_strInput[m_iFocusPos-1]-'0')*10+(m_strInput[m_iFocusPos]-'0');
   int numvideo=0;
   switch(m_eVideoType)
   {
   case VIDEO_30M:
   case VIDEO_30: numvideo=30;break;
   case VIDEO_25:
   case VIDEO_SECAM: numvideo=25;break;
   }
   switch(m_iFocusPos)
   {
	 case 1:maxnum=24;
		  break;
	 case 4:maxnum=60;
		 break;
	 case 7:maxnum=60;
		  break;
	 case 10:maxnum=numvideo;
		  break;
	 default: break;
   }
    if(num>maxnum)num=maxnum;
   return num;
}

void CTPTimeCodeEditEx::AddCharToFrame(char nChar)
{
   CString str=_T("0");
   PTSTR pstr=str.GetBuffer(10);
   pstr[0]=nChar;
   str.ReleaseBuffer();
   if(m_strInput==_T("0")) m_strInput=str;
   else m_strInput=m_strInput+str;
   if(m_strInput.GetLength()>m_lMaxLength)
   {
	   m_strInput=m_strInput.Right(m_lMaxLength);
   }
}
HWND CTPTimeCodeEditEx::UnsubclassWindow()
{
	SubWindow(FALSE);
	return CTPWnd::UnsubclassWindow();
}

//add by lzq:计算+/-的结果，并把结果设为当前值。2009.08.28
void CTPTimeCodeEditEx::FunCalcFrame()
{
	//自己计算
	if(m_boffsetNeg || !m_boffSet)//+ - 
	{
		if (m_iFrameInPut >= 0)//输入有效增量值，如果原值无效，则把增量做当前值。
		{
			m_iLastInput = max(0,m_iLastInput);

			if(m_boffsetNeg)//-					
			{
				m_iFrameInPut = m_iLastInput-m_iFrameInPut;

				if (m_iLastInput>=0 && m_iFrameInPut<0)
				{
					m_iFrameInPut += GetMaxFrame(m_eVideoType);
				}
			}
			else //+
			{
				m_iFrameInPut += m_iLastInput;					
			}	
		}
		else//输入无效增量值，则恢复原来的值。
		{
			m_iFrameInPut = m_iLastInput;	
		}

		m_strInput=StringFromFrame(m_eVideoType,m_iFrameInPut);
	}		
}

void CTPTimeCodeEditEx::SetTimeCodeStyleEx(UINT uStyleEx)
{
	if(uStyleEx & TP_TIMECODE_STYLE_EX_CALCULATE) 		m_bSelCalc   = TRUE;	
	if(uStyleEx & TP_TIMECODE_STYLE_EX_FRAMETOTC)      m_bFrameToTC = TRUE; 
}

//{ 4.6Feature - 1269. Identification for Ingesting Add by CZ.Lee : CD2009.10.01
#ifdef TP_NS_NET
void CTPTimeCodeEditEx::SetNotifyMode( BOOL bSet /*= TRUE*/) // 设置提醒模式
{
	if ( m_bNotifyMode != bSet )
	{
		m_bNotifyMode = bSet;

		Invalidate( FALSE );
		UpdateWindow();
	}
}
#endif
//}
