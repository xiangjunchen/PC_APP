#pragma once


// CTPVUStatic
#define WC_VUSTATIC  _L("VUStatic") 


class AFX_EXT_CLASS CTPVUStatic : public CStatic,public CTPBaseCtrl
{
	DECLARE_DYNAMIC(CTPVUStatic)

public:
	CTPVUStatic();
	virtual ~CTPVUStatic();
public:
	UINT                    m_iTextColor;
	UINT                    m_iBackColor;
	BOOL                    m_bBorder; 
	BOOL                    m_bPan;  
	CString                 m_sText;
	double                  m_dMinText;
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
	UINT                    m_uStyle; 
	TP_BORDER_STYLE         m_eBorderStyle;
private:
	double  m_dDB;
	double  m_dPan;
	BOOL    m_bEnable;
public:
	BOOL             Create(LPCTSTR sText,UINT uStyle,RECT &rtWnd,CWnd *pParentWnd,UINT nID = 0);
	void             SetDB(double dDB,BOOL bPaint = TRUE);
	void             GetDB(double& dDB);
	void             SetPan(double dPan,BOOL bPaint = TRUE);
	void             GetPan(double& dPan);
	void			 SetMinText(double dMinText);

	static  void    RegisterClass(BOOL bRegister = TRUE);
private:
	virtual void    SubWindow(BOOL bSub);
	void            PaintDC(CDC *pDC);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
};


