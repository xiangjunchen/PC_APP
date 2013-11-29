// TPVUTableCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TPExtraCtrl.h"
#include ".\tpvutablectrl.h"


#define TABLE_EDGE 2
// CTPVUTableCtrl

namespace _tp_extractrl
{

int iVUTable[][5] = 
{
	{  0,   0x78D6,	0x7FFF,	0x78D6E,	0x7FFFF ,},	//1
	{ -1,	0x6BB1,	0x78D5,	0x6BB2C,	0x78D6D	,}, //2
	{ -2,	0x5FFB,	0x6BB0,	0x5FFC7,	0x6BB2B	,}, //3
	{ -3,	0x558B,	0x5FFA,	0x558C4,	0x5FFC6	,}, //4
	{ -4,	0x4C3E,	0x558A,	0x4C3E9,	0x558C3	,}, //5
	{ -5,	0x43F3,	0x4C3D,	0x43F3F,	0x4C3E8	,}, //6
	{ -6,	0x3C8F,	0x43F2,	0x3C903,	0x43F3E	,}, //7
	{ -7,	0x35F9,	0x3C8E,	0x35FA1,	0x3C902	,}, //8
	{ -8,	0x301B,	0x35F8,	0x301B6,	0x35FA0	,}, //9
	{ -9,	0x2ADF,	0x301A,	0x2AE02,	0x301B5	,}, //10
	{ -10,	0x220E,	0x2ADE,	0x220EA,	0x2AE01	,}, //11
	{ -12,	0x1B0D,	0x220D,	0x1B0D7,	0x220E9	,}, //12
	{ -14,	0x157C,	0x1B0C,	0x157D1,	0x1B0D6 ,}, //13
	{ -16,	0x1111,	0x157B,	0x1111A,	0x157D0 ,}, //14
	{ -18,	0xD8E,	0x1110,	0xD8EF, 	0x11119 ,}, //15
	{ -20,	0xAC4,	0xD8D,	0xAC51, 	0xD8EE  ,}, //16
	{ -22,	0x88D,	0xAC3,	0x88E0, 	0xAC50,},//17
	{ -24,	0x6CB,	0x88C,	0x6CB9, 	0x88DF,},//18
	{ -26,	0x565,	0x6CA,	0x565C, 	0x6CB8,},//19
	{ -28,	0x449,	0x564,	0x4499, 	0x565B,},//20
	{ -30,	0x309,	0x448,	0x3090, 	0x4498,},//21
	{ -33,	0x226,	0x308,	0x2261, 	0x308F,},//22
	{ -36,	0x15B,	0x225,	0x15B1, 	0x2260,},//23
	{ -40,	0xF5,	0x15A,	0xF5B,  	0x15B0,},//24
	{ -43,	0xAD,	0xF4,	0xADF,  	0xF5A, },//25
	{ -46,	0x6D,	0xAC,	0x6DC,  	0xADE, },//26
	{ -50,	0x4D,	0x6C,	0x4DB,  	0x6DB,}, //27
	{ -53,	0x37,	0x4C,	0x370,  	0x4DA,}, //28
	{ -56,	0x22,	0x36,	0x22B,  	0x36F,}, //29
	{ -60,	0xA,    0x21,	0xAF,   	0x22A,}, //30
	{ -120,	0x0,    0xA,	0x0,    	0xAF, }, //31
};
int TPX_DBToPos(double dDB)
{
	if(dDB >=0)        return 30;
	else if(dDB<=-120) return 0;
	for(INT l=0;l<30;l++)
	{
		if(dDB ==iVUTable[l][0]) return 30-l;
		if(dDB>iVUTable[l+1][0] && dDB <=iVUTable[l][0])
		{
			return 30-l;
		}
	}
	return 0;
}
int TPX_ValueToPos(int iValue,WORD wBitsPerSample)
{
	int iTabindex = 2;
	if(wBitsPerSample == 24 )     { iTabindex = 2;iValue = iValue/255;}
	else if(wBitsPerSample == 32 ){ iTabindex = 2;iValue = iValue/65535;}

	if(iValue >=iVUTable[0][iTabindex])         return 30;
	else if(iValue<=iVUTable[30][iTabindex])    return 0;
	for(INT l=0;l<30;l++)
	{
		if(iValue ==iVUTable[l][iTabindex]) return 30-l;
		if(iValue>iVUTable[l+1][iTabindex] && iValue <=iVUTable[l][iTabindex])
		{
			return 30-l;
		}
	}
	return 0;
}
double TPX_ValueToDB(double iValue,WORD wBitsPerSample)
{
	int iTabindex = 2;
	if(wBitsPerSample == 24 )     { iTabindex = 2;iValue = iValue/255;}
	else if(wBitsPerSample == 32 ){ iTabindex = 2;iValue = iValue/65535;}

	if(iValue >=iVUTable[0][iTabindex])         return 0;
	else if(iValue<=iVUTable[30][iTabindex])    return -120;
	for(INT l=0;l<30;l++)
	{
		if(iValue ==iVUTable[l][iTabindex]) return iVUTable[l][0];
		if(iValue>=iVUTable[l+1][iTabindex] && iValue <iVUTable[l][iTabindex])
			return iVUTable[l][0] + (iValue - iVUTable[l+1][iTabindex]) * (iVUTable[l+1][0] - iVUTable[l][0])/(iVUTable[l][iTabindex] - iVUTable[l+1][iTabindex]);
	}
	return -120;
}
}

TPCommonRes      *CTPVUTableCtrl::m_pLargIcon = NULL;
int               CTPVUTableCtrl::m_iRefCount = 0;
IMPLEMENT_DYNAMIC(CTPVUTableCtrl, CStatic)
CTPVUTableCtrl::CTPVUTableCtrl()
{
	m_dwWindowLevel = TPWNDLEVEL1; 
	m_bSubWindow    = FALSE;
	m_bCreate       = FALSE;
	m_uStyle        = 0;
	m_eBorderStyle  = TP_GetBorderStyle();

	m_iMin         = 0;
	m_iMax         = 100;
	m_iPos         = 0;

	m_iDivMin      = 65;
	m_iDivMax      = 30;

	m_iLastPos     = 0;
	m_iDelayPos    = 0;
	m_iDelayMis    = 150;

	m_iTickCount   = 0;
	m_iDelayCount  = 0;
	m_hDelayTimer  = NULL;

	m_uVUState     = VUTABLE_ENABLE |VUTABLE_VERT |VUTABLE_FLAT | VUTABLE_RES;

	for(INT l=0;l<VATABLE_MAX;l++) m_pTableRes[l] = NULL;
	for(INT l=0;l<3;l++) m_pPenRes[l] = NULL;

	m_iOffeset     = 0;
	m_uPeekHold    = 0;///0 自动下 1 保持 2 不显示

	m_iRefCount ++;
	if(m_iRefCount ==1)
	{
		m_pLargIcon = TP_LoadIcon(TP_GetSkinPath() + _T("\\CommonCtrl\\VUTable\\Larg.ico"));
	}
}

CTPVUTableCtrl::~CTPVUTableCtrl()
{
	for(INT l=0;l<VATABLE_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pTableRes[l]);
		m_pTableRes[l] = NULL;
	}
	for(INT l=0;l<3;l++)
	{
		TP_ReleaseCommonRes(m_pPenRes[l]);
		m_pPenRes[l] = NULL;
	}
	m_iRefCount --;
	if(m_iRefCount == 0)
	{
		TP_ReleaseCommonRes(m_pLargIcon);
		m_pLargIcon = NULL;
	}
}
void     CTPVUTableCtrl::RegisterClass(BOOL bRegister)
{
	if(bRegister)
	{
		WNDCLASS wndClass;
		::GetClassInfo(AfxGetInstanceHandle(),WC_STATIC,&wndClass);
		wndClass.lpszClassName = WC_VUTABLE;
		::RegisterClass(&wndClass);
	}
	else
		UnregisterClass(WC_VUTABLE,AfxGetInstanceHandle());
}
BEGIN_MESSAGE_MAP(CTPVUTableCtrl, CStatic)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_MESSAGE(WM_UNSUBCLASSWINDOW,OnMsgUnSubWindow)
END_MESSAGE_MAP()
void CTPVUTableCtrl::LodaRes()
{
	CString sSysFacePath = TP_GetSkinPath();
	for(INT l=0;l<VATABLE_MAX;l++)
	{
		TP_ReleaseCommonRes(m_pTableRes[l]);
		m_pTableRes[l] = NULL;
	}

#ifdef TP_MINI_EDIT
	if(VUTABLE_VERT & m_uVUState)
	{
		m_pTableRes[VATABLE_NORMAL]     = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\mini\\VUTable_v_Normal.Bmp")); 
		m_pTableRes[VATABLE_ALARM]      = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\mini\\VUTable_v_Alarm.Bmp")); 
		m_pTableRes[VATABLE_DANGER]     = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\mini\\VUTable_v_Danger.Bmp")); 
		m_pTableRes[VATABLE_DISABLE]    = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\mini\\VUTable_v_Disable.Bmp")); 
	}
	else  
	{
		m_pTableRes[VATABLE_NORMAL]     = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\mini\\VUTable_H_Normal.Bmp")); 
		m_pTableRes[VATABLE_ALARM]      = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\mini\\VUTable_H_Alarm.Bmp")); 
		m_pTableRes[VATABLE_DANGER]     = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\mini\\VUTable_H_Danger.Bmp")); 
		m_pTableRes[VATABLE_DISABLE]    = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\mini\\VUTable_H_Disable.Bmp")); 
	}
#else
	if(VUTABLE_VERT & m_uVUState)
	{
		m_pTableRes[VATABLE_NORMAL]     = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_v_Normal.Bmp"));
		m_pTableRes[VATABLE_ALARM]      = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_v_Alarm.Bmp"));
		m_pTableRes[VATABLE_DANGER]     = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_v_Danger.Bmp"));
		m_pTableRes[VATABLE_DISABLE]    = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_v_Disable.Bmp"));
		m_pTableRes[VATABLE_BACK]       = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_v_back.bmp"));
	}
	else
	{
		m_pTableRes[VATABLE_NORMAL]     = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_H_Normal.Bmp"));
		m_pTableRes[VATABLE_ALARM]      = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_H_Alarm.Bmp"));
		m_pTableRes[VATABLE_DANGER]     = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_H_Danger.Bmp"));
		m_pTableRes[VATABLE_DISABLE]    = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_H_Disable.Bmp"));
		m_pTableRes[VATABLE_BACK]       = TP_LoadBmpByte(sSysFacePath +  _L("\\CommonCtrl\\VUTable\\VUTable_v_back.bmp"));
	}

	m_pPenRes[0] = TP_LoadPen(COLOR_VU_NORMAL,PS_SOLID,1);
	m_pPenRes[1] = TP_LoadPen(COLOR_VU_ALARM,PS_SOLID,1);
	m_pPenRes[2] = TP_LoadPen(COLOR_VU_DANGER,PS_SOLID,1);
#endif

}
LRESULT  CTPVUTableCtrl::OnMsgUnSubWindow(WPARAM wp,LPARAM lp)
{
	SubWindow(FALSE);
	return S_OK;
}
void CTPVUTableCtrl::SubWindow(BOOL bSub)
{
	if(bSub)
	{
		if(m_bSubWindow) return;
		m_bSubWindow = TRUE;
		m_uStyle = CStatic::GetStyle();
		if(m_uVUState & VUTABLE_RES) LodaRes();
		m_dwWindowLevel = TPWNDLEVEL3;
	}
	else
	{
		if(!m_bSubWindow) return;
		m_bSubWindow = FALSE;
		if(m_hDelayTimer)
		{
			KillTimer(m_hDelayTimer);
			m_hDelayTimer = NULL;
		}
	}
}
BOOL CTPVUTableCtrl::Create(UINT uStyle,CRect rtWnd,CWnd *pParent,UINT nID)
{
	return CStatic::Create(_L(""),uStyle,rtWnd,pParent,nID);
}
int CTPVUTableCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	SubWindow(TRUE);
	return 0;
}
void CTPVUTableCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
	if(bEnable) m_uVUState |= VUTABLE_ENABLE;
	else        m_uVUState &= ~VUTABLE_ENABLE;
}

void CTPVUTableCtrl::OnDestroy()
{
	SubWindow(FALSE);
	CStatic::OnDestroy();
}
BOOL CTPVUTableCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bCreate = TRUE;
	return CStatic::PreCreateWindow(cs);
}
void CTPVUTableCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	if(!m_bCreate) SubWindow(TRUE);
}
void CTPVUTableCtrl::PaintDC(CDC *pDC)
{
	m_iLastPos  = m_iMin;

	CRect rtClient,rtItem;
	GetClientRect(&rtClient);
	if(rtClient.Width() <=1) return;

	pDC ->FillSolidRect(rtClient,RGB(53,53,53));
	int    iLineHeight = 4;
	int    iLineDis    = 2;
	int    iDivMin     = 20;
	int    iDivMax     = 30;
	int    iLine       = 31;
	int    iIndex      = 0;	

	pDC ->FillSolidRect(rtClient.left,rtClient.bottom -1- iDivMin *iLineHeight - (iDivMin - 1)*iLineDis,rtClient.Width(),iDivMin*iLineHeight +(iDivMin - 1)*iLineDis+1,RGB(40,58,47));//底
	pDC ->FillSolidRect(rtClient.left,rtClient.bottom -1- iDivMax *iLineHeight- (iDivMax -1) *iLineDis,rtClient.Width(),(iDivMax-iDivMin)*iLineHeight + (iDivMax-iDivMin)*iLineDis,RGB(68,64,45)); //中
	pDC ->FillSolidRect(rtClient.left,rtClient.bottom -2- iLine *iLineHeight - (iLine -1) *iLineDis,  rtClient.Width(),(iLine- iDivMax) *iLineHeight + (iLine- iDivMax) *iLineDis+1,RGB(63,56,47));  //上
	rtClient.DeflateRect(0,1);

	for(INT l = 0;l<iLine;l++)
	{
		rtItem        = rtClient;
		rtItem.top    = rtClient.bottom - (l+1) * iLineHeight - l*iLineDis;
		rtItem.bottom = rtItem.top + iLineHeight;

		rtItem.top    = max(rtItem.top,rtClient.top);
		if(rtItem.bottom <= rtClient.top) break;

		if(m_uVUState & VUTABLE_ADBE)
		{
			pDC ->FillSolidRect(rtItem,RGB(0,64,128));//jyn back
		}
		else
		{
			if(l>= iDivMax)      pDC ->FillSolidRect(rtItem,RGB(153,99,28)); 
			else if(l>= iDivMin) pDC ->FillSolidRect(rtItem,RGB(160,134,19));
			else                 pDC ->FillSolidRect(rtItem,RGB(0,87,35));
		}
	}
	PaintTableVerNS(pDC);

}
void CTPVUTableCtrl::OnPaint()
{
	CPaintDC dc(this); 
	PaintDC(&dc);
}
void CTPVUTableCtrl::SetSmooth(BOOL bSmooth)
{
	if(bSmooth)
	{
		m_uVUState |= VUTABLE_SMOOTH;
		m_uVUState &= ~VUTABLE_COMPART;
	}
	else
	{
		m_uVUState &= ~VUTABLE_SMOOTH;
		m_uVUState |=  VUTABLE_COMPART;
	}
}
void CTPVUTableCtrl::SetHorz(BOOL bHorz)
{
	if(bHorz)
	{
		m_uVUState &= ~VUTABLE_VERT;
		m_uVUState |=  VUTABLE_HORZ;
	}
	else
	{
		m_uVUState &= ~VUTABLE_HORZ;
		m_uVUState |=  VUTABLE_VERT;
	}
}
void CTPVUTableCtrl::SetFlat(BOOL bFlat)
{
	UINT uState = m_uVUState;
	if(bFlat)
	{
		m_uVUState &= ~VUTABLE_RES;
		m_uVUState |=  VUTABLE_FLAT;
	}
	else
	{
		m_uVUState &= ~VUTABLE_FLAT;
		m_uVUState |=  VUTABLE_RES;
	}
	if(uState != m_uVUState) LodaRes();
}
void CTPVUTableCtrl::SetDivision(int iMin,int iMax)
{
	m_iDivMin = iMin;
	m_iDivMax = iMax;
}
void CTPVUTableCtrl::SetRange(int iMin,int iMax)
{
	m_iMin      = iMin;
	m_iMax      = iMax;
	m_iPos      = max(iMin,m_iPos);
	m_iLastPos  = iMin;
	m_iDelayPos = iMin;
	Invalidate(FALSE);
}
void CTPVUTableCtrl::GetRange(int &iMin,int &iMax)
{
	iMin = m_iMin;
	iMax = m_iMax;
}
void CTPVUTableCtrl::SetPos(int iPos)
{
	if(iPos == m_iPos) return;
	m_iPos = iPos;
	m_iPos = min(m_iPos,m_iMax);
	m_iPos = max(m_iPos,m_iMin);
	if(m_iDelayMis>0 && (m_uVUState & VUTABLE_ENABLE) && !(m_uVUState &VUTABLE_PEEKHOLD)) //设置延迟标志
	{
		m_iTickCount = 0;
		if(m_iDelayPos <m_iPos)
		{
			m_iDelayCount  = 1;
			m_iDelayPos    = m_iPos;
		}
		m_iDelayPos = max(m_iDelayPos,m_iPos);
		if(m_hDelayTimer == NULL && m_uPeekHold == 0)
			m_hDelayTimer = SetTimer(1000,m_iDelayMis,NULL);
	}

	CDC *pDC = GetDC();
	PaintTableVerNS(pDC);
	ReleaseDC(pDC);
}
int CTPVUTableCtrl::GetPos()
{
	return m_iPos;
}
void CTPVUTableCtrl::SetDelayMis(int iMillSecond)
{
	m_iDelayMis = iMillSecond;
	if(m_hDelayTimer)
	{
		m_iTickCount = 0;
		KillTimer(m_hDelayTimer);
		m_hDelayTimer = NULL;
		if(iMillSecond >0)	m_hDelayTimer = SetTimer(100,m_iDelayMis,NULL);
	}
}
int CTPVUTableCtrl::GetDelayMis()
{
	return m_iDelayMis;
}
void CTPVUTableCtrl::OnNcPaint()
{
	//g_pRoundCorner ->PaintRect(m_eBorderStyle,this);
}
void CTPVUTableCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1000)
	{
		if(m_iDelayPos <= m_iPos)
		{
			m_iTickCount ++;
			if(m_iTickCount * m_iDelayMis > 100)
			{
				KillTimer(m_hDelayTimer);
				m_hDelayTimer = NULL;
				m_iTickCount  = 0;
				m_iDelayCount = 0;
				CDC *pDC = GetDC();
				CTPDCExchange stuDC(pDC);
				PaintDC(pDC);
				stuDC.ReleaseDC();
				ReleaseDC(pDC);
			}
		}
		else
		{
			m_iDelayPos -= m_iDelayCount;
			m_iDelayCount++;
			m_iTickCount = 0;
		}
	}
	CStatic::OnTimer(nIDEvent);
}
BOOL CTPVUTableCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CStatic::OnEraseBkgnd(pDC);
}
void CTPVUTableCtrl::PaintTableHorzSmooth(CDC *pDC)
{
	UINT uDrawFlag = 0;	
	int  iIndex    = 0;
	BOOL bEnable   = (m_uVUState & VUTABLE_ENABLE) == VUTABLE_ENABLE;
	CRect rtClient,rtNor,rtAlr,rtDang,rtBack,rtDelay;
	GetClientRect(&rtClient);
	rtClient.DeflateRect(TABLE_EDGE,TABLE_EDGE);
	if(m_iMax <= m_iMin)
	{
		pDC ->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW)); 
		return;
	}
	double dAmp = rtClient.Width() *1.0 / (m_iMax - m_iMin);

	rtNor   = rtClient;
	rtAlr   = rtClient;
	rtDang  = rtClient;
	rtBack  = rtClient;
	rtDelay = rtClient;
	if(m_iPos < m_iDivMin)
	{
		rtNor.right  = rtClient.left + TP_DoubleToInt(m_iPos *dAmp);
		uDrawFlag |= 0x00000001;
		rtBack.left = rtNor.right;
		uDrawFlag |= 0x00000008;
	}
	else if(m_iPos > m_iDivMax)
	{
		rtNor.right    = rtClient.left + TP_DoubleToInt(m_iDivMin *dAmp);
		uDrawFlag |= 0x00000001;
		rtAlr.left     = rtNor.right;
		rtAlr.right    = rtClient.left + TP_DoubleToInt(m_iDivMax *dAmp);
		uDrawFlag |= 0x00000002;
		rtDang.left    = rtAlr.right;
		rtDang.right   = rtClient.left + TP_DoubleToInt(m_iPos *dAmp);
		uDrawFlag |= 0x00000004;
		rtBack.left    = rtDang.right;
		uDrawFlag |= 0x00000008;
	}
	else 
	{
		rtNor.right    = rtClient.left + TP_DoubleToInt(m_iDivMin *dAmp);
		uDrawFlag |= 0x00000001;
		rtAlr.left     = rtNor.right;
		rtAlr.right    = rtClient.left + TP_DoubleToInt(m_iPos *dAmp);
		uDrawFlag |= 0x00000002;
		rtBack.left    = rtAlr.right;
		uDrawFlag |= 0x00000008;
	}
	if(m_iDelayPos > m_iPos)
	{
		rtDelay.right = rtClient.left + TP_DoubleToInt(m_iDelayPos *dAmp);
		rtDelay.left  = rtDelay.right - TABLE_EDGE;
		uDrawFlag |= 0x00000010;
	}
	if(uDrawFlag & 0x00000001)
	{
		if(VUTABLE_FLAT & m_uVUState)
		{			
			if(bEnable)	pDC ->FillSolidRect(rtNor,TP_GetSysColor(COLOR_VU_NORMAL));
			else    	pDC ->FillSolidRect(rtNor,TP_GetSysColor(COLOR_VU_DISABLE));
		}
		else
		{
			if(bEnable) iIndex = VATABLE_NORMAL;
			else    	iIndex = VATABLE_DISABLE;
			TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),0,0,rtNor,FALSE);
		}
	}
	if(uDrawFlag & 0x00000002)
	{
		if(VUTABLE_FLAT & m_uVUState)
		{
			if(bEnable)	pDC ->FillSolidRect(rtAlr,TP_GetSysColor(COLOR_VU_ALARM));
			else    	pDC ->FillSolidRect(rtAlr,TP_GetSysColor(COLOR_VU_DISABLE));
		}
		else
		{
			if(bEnable) iIndex = VATABLE_ALARM;
			else    	iIndex = VATABLE_DISABLE;
			TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),0,0,rtAlr,FALSE);
		}
	}
	if(uDrawFlag & 0x00000004)
	{
		if(VUTABLE_FLAT & m_uVUState)
		{
			if(bEnable)	pDC ->FillSolidRect(rtDang,TP_GetSysColor(COLOR_VU_DANGER));
			else    	pDC ->FillSolidRect(rtDang,TP_GetSysColor(COLOR_VU_DISABLE));
		}
		else
		{
			if(bEnable) iIndex = VATABLE_DANGER;
			else    	iIndex = VATABLE_DISABLE;
			TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),0,0,rtDang,FALSE);
		}
	}
	if(uDrawFlag & 0x00000008)
	{
		pDC ->FillSolidRect(rtBack,TP_GetSysColor(COLOR_BASE_WINDOW));
	}
	if(uDrawFlag & 0x00000010)
	{
		if(VUTABLE_FLAT & m_uVUState)
		{
			if(!bEnable)
			{
				pDC ->FillSolidRect(rtDelay,TP_GetSysColor(COLOR_VU_DISABLE));
			}
			else if(m_iDelayPos < m_iDivMin)
			{
				pDC ->FillSolidRect(rtDelay,TP_GetSysColor(COLOR_VU_NORMAL));
			}
			else if(m_iDelayPos > m_iDivMax)
			{
				pDC ->FillSolidRect(rtDelay,TP_GetSysColor(COLOR_VU_DANGER));
			}
			else
			{
				pDC ->FillSolidRect(rtDelay,TP_GetSysColor(COLOR_VU_ALARM));
			}
		}
		else
		{
			if(!bEnable)			    		iIndex = VATABLE_DISABLE;
			else if(m_iDelayPos < m_iDivMin)	iIndex = VATABLE_NORMAL;
			else if(m_iDelayPos > m_iDivMax)	iIndex = VATABLE_DANGER;
			else            					iIndex = VATABLE_ALARM;
			TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),0,0,rtDelay,FALSE);
		}
	}
}
void CTPVUTableCtrl::PaintTableVertSmooth(CDC *pDC)
{
	UINT uDrawFlag = 0;	
	int  iIndex    = 0;
	BOOL bEnable   = (m_uVUState  & VUTABLE_ENABLE) == VUTABLE_ENABLE;
	CRect rtClient,rtNor,rtAlr,rtDang,rtBack,rtDelay;
	GetClientRect(&rtClient);
	rtClient.DeflateRect(TABLE_EDGE,TABLE_EDGE);
	if(m_iMax <= m_iMin)
	{
		pDC ->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW)); 
		return;
	}
	double dAmp = rtClient.Height() *1.0 / (m_iMax - m_iMin);

	rtNor   = rtClient;
	rtAlr   = rtClient;
	rtDang  = rtClient;
	rtBack  = rtClient;
	rtDelay = rtClient;
	if(m_iPos < m_iDivMin)
	{
		rtNor.top = rtClient.bottom - TP_DoubleToInt(m_iPos *dAmp);
		uDrawFlag |= 0x00000001;
		rtBack.bottom = rtNor.top;
		uDrawFlag |= 0x00000008;
	}
	else if(m_iPos > m_iDivMax)
	{
		rtNor.top  = rtClient.bottom - TP_DoubleToInt(m_iDivMin *dAmp);
		uDrawFlag |= 0x00000001;
		rtAlr.bottom = rtNor.top;
		rtAlr.top  = rtClient.bottom - TP_DoubleToInt(m_iDivMax *dAmp);
		uDrawFlag |= 0x00000002;
		rtDang.bottom = rtAlr.top;
		rtDang.top = rtClient.bottom - TP_DoubleToInt(m_iPos *dAmp);
		uDrawFlag |= 0x00000004;
		rtBack.bottom = rtDang.top;
		uDrawFlag |= 0x00000008;
	}
	else 
	{
		rtNor.top  = rtClient.bottom - TP_DoubleToInt(m_iDivMin *dAmp);
		uDrawFlag |= 0x00000001;
		rtAlr.bottom = rtNor.top;
		rtAlr.top  = rtClient.bottom - TP_DoubleToInt(m_iPos *dAmp);
		uDrawFlag |= 0x00000002;
		rtBack.bottom = rtAlr.top;
		uDrawFlag |= 0x00000008;
	}
	if(m_iDelayPos > m_iPos)
	{
		rtDelay.top    = rtClient.bottom - TP_DoubleToInt(m_iDelayPos *dAmp);
		rtDelay.bottom = rtDelay.top + TABLE_EDGE;
		uDrawFlag |= 0x00000010;
	}

	if(uDrawFlag & 0x00000001)
	{
		if(VUTABLE_FLAT & m_uVUState)
		{
			if(bEnable)	pDC ->FillSolidRect(rtNor,TP_GetSysColor(COLOR_VU_NORMAL));
			else    	pDC ->FillSolidRect(rtNor,TP_GetSysColor(COLOR_VU_DISABLE));
		}
		else
		{
			if(bEnable) iIndex = VATABLE_NORMAL;
			else    	iIndex = VATABLE_DISABLE;
			TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),15,15,rtNor,TRUE);
		}
	}
	if(uDrawFlag & 0x00000002)
	{
		if(VUTABLE_FLAT & m_uVUState)
		{
			if(bEnable)	pDC ->FillSolidRect(rtAlr,TP_GetSysColor(COLOR_VU_ALARM));
			else    	pDC ->FillSolidRect(rtAlr,TP_GetSysColor(COLOR_VU_DISABLE));
		}
		else
		{
			if(bEnable) iIndex = VATABLE_ALARM;
			else    	iIndex = VATABLE_DISABLE;
			TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),15,15,rtAlr,TRUE);
		}
	}
	if(uDrawFlag & 0x00000004)
	{
		if(VUTABLE_FLAT & m_uVUState)
		{
			if(bEnable)	pDC ->FillSolidRect(rtDang,TP_GetSysColor(COLOR_VU_DANGER));
			else    	pDC ->FillSolidRect(rtDang,TP_GetSysColor(COLOR_VU_DISABLE));
		}
		else
		{
			if(bEnable) iIndex = VATABLE_DANGER;
			else    	iIndex = VATABLE_DISABLE;
			TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),15,15,rtDang,TRUE);
		}
	}
	if(uDrawFlag & 0x00000008)
	{
		pDC ->FillSolidRect(rtBack,TP_GetSysColor(COLOR_BASE_WINDOW));
	}
	if(uDrawFlag & 0x00000010)
	{
		if(VUTABLE_FLAT & m_uVUState)
		{
			if(!bEnable)
			{
				pDC ->FillSolidRect(rtDelay,TP_GetSysColor(COLOR_VU_DISABLE));
			}
			else if(m_iDelayPos < m_iDivMin)
			{
				pDC ->FillSolidRect(rtDelay,TP_GetSysColor(COLOR_VU_NORMAL));
			}
			else if(m_iDelayPos > m_iDivMax)
			{
				pDC ->FillSolidRect(rtDelay,TP_GetSysColor(COLOR_VU_DANGER));
			}
			else 
			{
				pDC ->FillSolidRect(rtDelay,TP_GetSysColor(COLOR_VU_ALARM));
			}
		}
		else
		{
			if(!bEnable)			    		iIndex = VATABLE_DISABLE;
			else if(m_iDelayPos < m_iDivMin)	iIndex = VATABLE_NORMAL;
			else if(m_iDelayPos > m_iDivMax)	iIndex = VATABLE_DANGER;
			else            					iIndex = VATABLE_ALARM;
			TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),15,15,rtDelay,TRUE);
		}
	}
}
int CTPVUTableCtrl::GetMiniWidth()
{
	return m_pTableRes[VATABLE_BACK] ->Width();
}
void CTPVUTableCtrl::PaintTableVerBack(CDC *pDC)
{
	CRect rtClient;
	GetClientRect(rtClient);
	CTPMemoDC dc(pDC,rtClient.Width(),rtClient.Height());
	dc.FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
	TP_StretchDIBRect(&dc,m_pTableRes[VATABLE_BACK] ->GetByte(),m_pTableRes[VATABLE_BACK]->Width(), m_pTableRes[VATABLE_BACK]->Height(),10,11,rtClient,TRUE,FALSE,2,1);
	int iLine = rtClient.Height()/2+1;
	int iPos        = TP_DoubleToInt(m_iPos * iLine *1.0/ (m_iMax - m_iMin));
	int iLastPos    = m_iLastPos * iLine / (m_iMax - m_iMin);
	int iDelayPos   = m_iDelayPos * iLine / (m_iMax - m_iMin);
	int iDivMin     = m_iDivMin * iLine / (m_iMax - m_iMin);
	int iDivMax     = m_iDivMax * iLine / (m_iMax - m_iMin);
	if(iPos >1)
	{
		int   iPenSave = 0,iPen = 0;
		HPEN  hPen     = (HPEN)::SelectObject(dc.m_hDC,m_pPenRes[0]->hPen);
		for(INT l=0;l<iLine;l++) 
		{
			if(l<=iPos ||  l == iDelayPos && l >iPos)
			{
				if(l < iDivMin)      iPen = 0;
				else if(l > iDivMax) iPen = 1;
				else        		 iPen = 2;
				if(iPen != iPenSave)
				{
					::SelectObject(dc.m_hDC,m_pPenRes[iPen]->hPen);
					iPenSave = iPen;
				}
				dc.MoveTo(0,rtClient.Height()-1-l*2);
				dc.LineTo(rtClient.Width(),rtClient.Height()-1-l*2);
			}
		}
		::SelectObject(dc.m_hDC,hPen);
	}
	pDC ->BitBlt(0,0,rtClient.Width(),rtClient.Height(),&dc,0,0,SRCCOPY);

}

void CTPVUTableCtrl::PaintTableHorzDivision(CDC *pDC)
{
	UINT uDrawFlag = 0;
	BOOL bEnable   = (m_uVUState & VUTABLE_ENABLE) == VUTABLE_ENABLE;
	CRect rtClient,rtItem;
	GetClientRect(&rtClient);
	rtClient.DeflateRect(TABLE_EDGE,TABLE_EDGE);
	rtItem = rtClient;
	if(m_iMax <= m_iMin)
	{
		pDC ->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
		return;
	}
	//计算小格
	int    iLineHeight = rtClient.Height() *2/4;
	int    iLine       = rtClient.Width()/iLineHeight + 1;
	int    iPos        = TP_DoubleToInt(m_iPos * iLine *1.0/ (m_iMax - m_iMin));
	int    iLastPos    = m_iLastPos * iLine / (m_iMax - m_iMin);
	int    iDelayPos   = m_iDelayPos * iLine / (m_iMax - m_iMin);
	int    iDivMin     = m_iDivMin * iLine / (m_iMax - m_iMin);
	int    iDivMax     = m_iDivMax * iLine / (m_iMax - m_iMin);
	int    iIndex      = 0;

	for(INT l = 1;l<=iLine;l++)
	{
		rtItem.right   = rtClient.left + l * iLineHeight - iLineHeight/4;
		rtItem.left    = rtItem.right  - iLineHeight*3/4;
		if(l==iLine) rtItem.right = min(rtClient.right,rtItem.right);

		if(l<=iPos ||  l == iDelayPos && l >iPos)
		{
			if(!bEnable)
			{
				if(VUTABLE_FLAT & m_uVUState)	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_DISABLE));
				else {iIndex = VATABLE_DISABLE ;TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),5,5,rtItem,FALSE);}
			}
			else if(l < iDivMin)
			{
				if(VUTABLE_FLAT & m_uVUState)   pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_NORMAL));
				else {iIndex = VATABLE_NORMAL ;TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),5,5,rtItem,FALSE);}
			}
			else if(l > iDivMax)
			{
				if(VUTABLE_FLAT & m_uVUState)   pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_DANGER));
				else {iIndex = VATABLE_ALARM ;TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),5,5,rtItem,FALSE);}
			}
			else 
			{
				if(VUTABLE_FLAT & m_uVUState)   pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_ALARM));
				else {iIndex = VATABLE_DISABLE ;TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),5,5,rtItem,FALSE);}
			}

			rtItem.left  = rtItem.right;
			rtItem.right = rtItem.left + iLineHeight/4;
			if(l<iLine) pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_DIVISION));
			else        pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
		}
		else
		{
			pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
			rtItem.left    = rtItem.right;
			rtItem.right   = rtItem.left + iLineHeight/4;
			if(l<iLine)  	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_DIVISION));
			else        	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
		}
	}
}

void CTPVUTableCtrl::PaintTableVertDivision(CDC *pDC)
{
	UINT uDrawFlag = 0;
	BOOL bEnable   = (m_uVUState  & VUTABLE_ENABLE) == VUTABLE_ENABLE;
	CRect rtClient,rtItem;
	GetClientRect(&rtClient);
	rtClient.DeflateRect(TABLE_EDGE,TABLE_EDGE,TABLE_EDGE,TABLE_EDGE);
	if(rtClient.Width() <=1) return;
	rtItem = rtClient;
	rtItem.right;
	if(m_iMax <= m_iMin)
	{
		pDC ->FillSolidRect(rtClient,TP_GetSysColor(COLOR_BASE_WINDOW));
		return;
	}
	//计算小格
	int    iLineHeight = rtClient.Width() /2+2;
	int    iLineSpace  = 2;
	int    iLine       = rtClient.Height()/iLineHeight +1;
	int    iPos        = TP_DoubleToInt(m_iPos * iLine *1.0/ (m_iMax - m_iMin));
	int    iLastPos    = m_iLastPos * iLine / (m_iMax - m_iMin);
	int    iDelayPos   = m_iDelayPos * iLine / (m_iMax - m_iMin);
	int    iDivMin     = m_iDivMin * iLine / (m_iMax - m_iMin);
	int    iDivMax     = m_iDivMax * iLine / (m_iMax - m_iMin);
	int    iIndex      = 0;

	pDC->FillSolidRect(1,rtClient.bottom -2,rtClient.Width()+2,2,TP_GetSysColor(COLOR_BASE_FRAME));	
	rtClient.bottom -=2;

	for(INT l = 1;l<=iLine;l++)
	{
		rtItem.top    = rtClient.bottom - l * iLineHeight + iLineSpace;
		rtItem.bottom = rtItem.top + iLineHeight-iLineSpace;
		if(l==iLine) rtItem.top = max(rtClient.top,rtItem.top);

		if(l<=iPos ||  l == iDelayPos && l >iPos)
		{
			if(!bEnable)
			{
				if(VUTABLE_FLAT & m_uVUState)	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_DISABLE));
				else {iIndex = VATABLE_DISABLE ;TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),5,5,rtItem,TRUE);}
			}
			else if(l < iDivMin)
			{
				if(VUTABLE_FLAT & m_uVUState)	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_NORMAL));
				else {iIndex = VATABLE_NORMAL ;TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),5,5,rtItem,TRUE);}
			}
			else if(l > iDivMax)
			{
				if(VUTABLE_FLAT & m_uVUState)	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_DANGER));
				else {iIndex = VATABLE_ALARM ;TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),5,5,rtItem,TRUE);}
			}
			else
			{
				if(VUTABLE_FLAT & m_uVUState)	pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_ALARM));
				else {iIndex = VATABLE_DANGER ;TP_StretchDIBRect(pDC,m_pTableRes[iIndex] ->GetByte(),m_pTableRes[iIndex]->Width(),m_pTableRes[iIndex]->Height(),5,5,rtItem,TRUE);}
			}
			rtItem.bottom = rtItem.top;
			rtItem.top    = rtItem.bottom - iLineSpace;
			if(l<iLine) pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_DIVISION));
			else        pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
		}
		else
		{
			pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
			rtItem.bottom = rtItem.top;
			rtItem.top    = rtItem.bottom - iLineSpace;
			if(l<iLine) pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_VU_DIVISION));
			else        pDC ->FillSolidRect(rtItem,TP_GetSysColor(COLOR_BASE_WINDOW));
		}
	}

}
void CTPVUTableCtrl::ReSetPeek()
{
	m_iDelayPos = m_iMin;
	Invalidate();
}
BOOL CTPVUTableCtrl::IsOverflow()
{
	return 	m_iPos >= m_iDivMax;
}
void CTPVUTableCtrl::PaintTableVerNS(CDC *pDC)
{
	CRect rtClient,rtItem;
	GetClientRect(&rtClient);

	int    iLineHeight = 4;
	int    iLineDis    = 2;
	int    iDivMin     = 20;
	int    iDivMax     = 30;
	int    iLine       = 31;
	int    iIndex      = 0;
	rtClient.DeflateRect(0,1);

	int    iPos        = m_iPos;
	int    iDelayPos   = m_iDelayPos;

	for(INT l = 0;l<iLine;l++)
	{
		rtItem        = rtClient;
		rtItem.top    = rtClient.bottom - (l+1) * iLineHeight - l * iLineDis;
		rtItem.bottom = rtItem.top + iLineHeight;

		rtItem.top    = max(rtItem.top,rtClient.top);
		if(rtItem.bottom <= rtClient.top) break;

		if(m_uVUState & VUTABLE_ADBE)
		{
			if(l<=iPos)
				pDC ->FillSolidRect(rtItem,RGB(0,128,255)); //jyn high
			else
				pDC ->FillSolidRect(rtItem,RGB(0,64,128)); //jyn low
		}
		else
		{
			if(l<=iPos || m_uPeekHold !=2 && l == iDelayPos && l >iPos)
			{
				if(l>= iDivMax)      pDC ->FillSolidRect(rtItem,RGB(255,109,0));
				else if(l>= iDivMin) pDC ->FillSolidRect(rtItem,RGB(243,246,0));
				else                 pDC ->FillSolidRect(rtItem,RGB(0,236,93));
			}
			else
			{
				if(l>= iDivMax)      pDC ->FillSolidRect(rtItem,RGB(153,99,28));
				else if(l>= iDivMin) pDC ->FillSolidRect(rtItem,RGB(160,134,19));
				else                 pDC ->FillSolidRect(rtItem,RGB(0,87,35));
			}
		}
	}
}

void CTPVUTableCtrl::PaintTicks(CDC *pDC,CRect rtDraw,BOOL bText,BOOL bLine)
{
	int iPos[] = {0,-2,-4,-6,-8,-10,-20,-30,-42,-51,-60,-120};
	CPen pPen(PS_SOLID,1,TP_GetSysColor(COLOR_BASE_TEXT));
	HPEN hPen    =	(HPEN) ::SelectObject(pDC->m_hDC,pPen.m_hObject);	
	int     iLineCY = 0,iCx = 0,iPosDB = 0;
	CRect   rtTemp  = CRect(0,0,0,0);
	TCHAR   sText[10];
	CSize   szExtend; 
	for(int i=0;i<12;i++)
	{
		iCx     = rtDraw.right;
		iPosDB  = TPX_DBToPos(iPos[i]);
		iLineCY = rtDraw.bottom - 1 - (iPosDB + 1) *4 - iPosDB * 2 +2;  
		if(bText)
		{
			if(iPos[i]<=-120) lstrcpy(sText,_T("-  "));// -00"));//∞"));
			else              wsprintf(sText,_L("%d"),iPos[i]);
			szExtend = pDC->GetTextExtent(sText);

			rtTemp =  CRect(rtDraw.left,iLineCY -szExtend.cy/2,rtDraw.right,iLineCY +szExtend.cy/2);
			if(iPos[i]<=-100) pDC->DrawIcon(rtTemp.right -15,rtTemp.top +4,m_pLargIcon->GetIcon());
			else pDC->DrawText(sText,rtTemp,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			iCx = rtDraw.right - szExtend.cx;
		}
		if(bLine)
		{
			pDC->MoveTo(iCx- 1, iLineCY);
			pDC->LineTo(rtDraw.left+1, iLineCY);
		}
	}
	::SelectObject(pDC->m_hDC,hPen);
}
void CTPVUTableCtrl::SetDBEState(BOOL bDbe)
{
	if(bDbe) m_uVUState |=  VUTABLE_ADBE;
	else     m_uVUState &= ~VUTABLE_ADBE;
}
void CTPVUTableCtrl::SetPeekHold(BOOL bSet)
{
	if(bSet) m_uVUState |= VUTABLE_PEEKHOLD;
	else     m_uVUState &= ~VUTABLE_PEEKHOLD;
}