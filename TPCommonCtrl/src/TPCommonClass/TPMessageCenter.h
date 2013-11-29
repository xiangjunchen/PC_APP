#pragma once


// CTPMessageCenter
#define  WM_MESSAGEEXCHANGE2		WM_USER + 2242
#define  WM_MESSAGEEXCHANGE3		WM_USER + 2243
#define  WM_MESSAGEEXCHANGE4		WM_USER + 2244
#define  WM_MESSAGEEXCHANGE5		WM_USER + 2245
#define  WM_MESSAGEEXCHANGE6		WM_USER + 2246
#define  WM_MESSAGEEXCHANGE7		WM_USER + 2247

#define  WM_MESSAGEEXCHANGE8		WM_USER + 2248
typedef LRESULT   (*TPExchangeCallBack)(WPARAM wp,LPARAM lp,LPVOID pUser);



class AFX_EXT_CLASS CTPMessageCenter : public CWnd
{
	DECLARE_DYNAMIC(CTPMessageCenter)

public:
	CTPMessageCenter();
	virtual ~CTPMessageCenter();

public:
	static TPExchangeCallBack   m_funCallBack[9];
	static LPVOID               m_pCallBackParam[9];

public:
	LRESULT      OnMessageExchange1(WPARAM wp,LPARAM lp);
	LRESULT      OnMessageExchange2(WPARAM wp,LPARAM lp);
	LRESULT      OnMessageExchange3(WPARAM wp,LPARAM lp);
	LRESULT      OnMessageExchange4(WPARAM wp,LPARAM lp);
	LRESULT      OnMessageExchange5(WPARAM wp,LPARAM lp);
	LRESULT      OnMessageExchange6(WPARAM wp,LPARAM lp);
	LRESULT      OnMessageExchange7(WPARAM wp,LPARAM lp);
	LRESULT      OnMessageExchange8(WPARAM wp,LPARAM lp);
	LRESULT      OnMessageExchange9(WPARAM wp,LPARAM lp);

	static LRESULT      SendMessageEx(UINT uMessage,WPARAM wp,LPARAM lp);
	static LRESULT      PostMessageEx(UINT uMessage,WPARAM wp,LPARAM lp);


	static void        TransMessage(UINT uMessage = 0);
	static HWND        GetMessageWnd();



public:
	static void        CreateWnd();
	static void        DestroyWnd();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


