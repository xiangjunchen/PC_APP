#pragma once

 
// CTPSliderCtrlExEx
#define WC_SLIDERCTRLEXEX _L("SliderCtrlExEx")

class AFX_EXT_CLASS   CTPSliderCtrlExEx : public CTPSliderCtrl 
{
	DECLARE_DYNAMIC(CTPSliderCtrlExEx)

public:
	CTPSliderCtrlExEx();
	virtual ~CTPSliderCtrlExEx();
public:
	CWnd                   *m_pMsgWnd ;
	int                     m_iChanneOffset;

 	BOOL                    m_iMicType;
	BOOL                    m_iDimLen;
	long                    m_dlMin;
	long                    m_dlMax;
	BOOL                    m_bReserce;
protected:
	BOOL                    m_bSubWindow;
	BOOL                    m_bCreate;
 	UINT                    m_uState;
 

    
	int                    m_iDragMode;          //拖拽模式
	CPoint                 m_ptDragSave;         //拖拽保存点
 
 public:

 
	 int                   GetPos();

 	 void                  SetPos(int nPos);
 
	 void                  SetRange(_In_ int nMin, _In_ int nMax, _In_ BOOL bRedraw = FALSE);
 
 

	static void     RegisterClass(BOOL bRegister = TRUE);

 
 protected:
	virtual void    SubWindow(BOOL bSub);
  
	virtual int     PixelToPos(int iPixel);   
	int             PosToPixel(int iPos);   
 	LRESULT         OnMsgUnSubWnd(WPARAM wp,LPARAM lp);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
 protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
 };


