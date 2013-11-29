#pragma once


// CTPNumberEditExEx
#define WC_TPNUMBEREDIT _L("NumberEditExEx")

class AFX_EXT_CLASS CTPNumberEditExEx : public CTPEditEx
{
	DECLARE_DYNAMIC(CTPNumberEditExEx)

public:
	CTPNumberEditExEx();
	virtual ~CTPNumberEditExEx();
public:	
	CTPSpinButtonCtrl *m_pSpinButton;
	BOOL               m_bCheckInfo;
	DWORD			   m_dwLastError;
private:
	double            m_dMax; 
	double            m_dMin;
	long              m_lDecimals; 
	double            m_dDoubleSave; 
	long              m_iAccel;
	long              m_iAccelCount;
	BOOL              m_bDouble;
	INT64             m_lStep;
	double            m_dStep;

public:
	void              SetRange(double dMin,double dMax); 
	void              SetRange32(INT64 lMin,INT64 lMax); 
	double            GetPosEx();
	double            GetPos();
	void              SetPos(double dPos);
	INT64             GetPos32();
	void              SetPos32(INT64 iPos);
	void              SetDecimals(int iDem);
	void              SetAccel(int iAccel);   //Base  100ms

protected:
	virtual void    SubWindow(BOOL bSub);
	void            OnEnKillFocus();

protected:
	LRESULT         OnSpinMessage(WPARAM wp,LPARAM lp);
protected:
	DECLARE_MESSAGE_MAP()

public:
	static void  RegisterClass(BOOL bRegister,HINSTANCE hInstance);
	afx_msg BOOL OnEnChange();
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); //by zhh
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


