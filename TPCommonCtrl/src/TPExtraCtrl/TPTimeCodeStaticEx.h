#pragma once


// CTPTimeCodeStaticEx
//显示时码数据

#define WC_TPTIMECODESTATIC _L("TimeCodeStaticClassEx")

#define TP_TIMECODESTATICEX_SHORTCODE    0x00000001
class AFX_EXT_CLASS CTPTimeCodeStaticEx : public CTPWnd,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPTimeCodeStaticEx)

public:
	CTPTimeCodeStaticEx();
	virtual ~CTPTimeCodeStaticEx();
public:	
	VIDEO_TYPE              m_eVideoType;
	BOOL                    m_bSign; 
	UINT                    m_uTextAlign; 
	UINT                    m_uPrivateType;
	static BOOL             m_bInOutConcept;
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	TP_BORDER_STYLE         m_eBorderStyle;
	int                     m_iBackColor;
	int                     m_iTextColor;
	int                     m_iBaseTextWidth; 
	int                     m_iBaseTextSignWidth;

protected:
	CString                 m_sText;
	CString                 m_sTextSave;
	int                     m_iBmpLeft[11]; 
	__int64                 m_iFrame; 
	CFont                  *m_pFont;
	COLORREF                m_clText;

protected:
	TPCommonRes            *m_pResNumber[12];
	TPCommonRes            *m_pLeftLogo;
	TPCommonRes            *m_pBackRes;;
	int                     m_uLogoPos;
	int						GetNumberIndex( TCHAR ch);

public:
	BOOL            Create(UINT uStyle,CRect rectWnd,CWnd *pParent,UINT nID);

	static void     RegisterClass(BOOL bRegister);
	virtual void    SubWindow(BOOL bSub);
	void            SetResPath(CString sPath = _L(""));
	int             GetResHeight();
	void            SetLeftLogo(CString sLogo,UINT uYPos); 
	void            SetBackRes(CString sPath);

public:
	__int64         GetFrame();	
	void            SetFrame(__int64 iFrame,BOOL bPaint = TRUE);	
	void            SetString(CString strFrame,BOOL bPaint = TRUE);
	void            SetTextColor(TP_SYSTEM_COLOR eColorIndex);
	void            SetTextColor(COLORREF clText);
	void            SetBackColor(TP_SYSTEM_COLOR eColorIndex);
	void            SetFont(CFont &ftSet); 

	static  INT64   GetInOutLength(__int64 iIn,__int64 iOut);

protected:
	void            DrawDC(CDC *pDC);
	void            DrawResDC(CDC *pDC);
	void            UpdateResDC(CDC *pDC);		// FRAME改变时的刷新
	void            GetBaseTextWidth();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void    OnPaint();
	afx_msg void    OnNcPaint();
	afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
};



