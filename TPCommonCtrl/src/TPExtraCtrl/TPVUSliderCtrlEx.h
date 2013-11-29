#pragma once
#include "tpvusliderctrl.h"

#define WC_VUSLIDERCTRLEX  _L("VUSliderCtrlEx") 

#define TP_TPVUSLIDERCTRLEX_LDB     0x00000001
#define TP_TPVUSLIDERCTRLEX_LDOWN   0x00000002
#define TP_TPVUSLIDERCTRLEX_RDOWN   0x00000004

#define WM_TPVUSLIDERCTRLEX  WM_USER +1209
class CTPEditEx;

class AFX_EXT_CLASS CTPVUSliderCtrlEx :public CTPVUSliderCtrl
{
public:
	CTPVUSliderCtrlEx(void);
	~CTPVUSliderCtrlEx(void);
public:
	int             m_iType; 		
	int             m_iTextColor; 
	CRect           m_rtBtnPos;
private:
	CRect           m_rtBackChannle;
	CRect           m_rtClient;
	CRect           m_rtDBText; 
	int             m_iPosSave;
	double          m_dDBSave;	
	int             m_iTickWidth;
	CTPEditEx      *m_pEditName;
	
	double          m_dDBSave_SliderStop;//保存滑块停止时的增益值，区别于m_dDBSave； added by lizhiqiao，on2008.05.21
public:
	double          GetDB();  
	void            SetDB(double dDB,BOOL bComplex = FALSE);

protected:
	virtual void    OnLButtonDB(CPoint pt);
	LRESULT         OnEnEditEnter(WPARAM wp,LPARAM lp);
public:
	static void     RegisterClass(BOOL bRegister = TRUE);
protected:
	virtual void    SubWindow(BOOL bSub);
	virtual void    LoadThumb();
	virtual void    GetChannelRect(CRect &rtChannel);
	virtual void    PaintCtrl(CDC *pDC);
    virtual void    PaintTicks(CDC *pDC);
	void            OnEnEdit();
public:
	DECLARE_MESSAGE_MAP()
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
