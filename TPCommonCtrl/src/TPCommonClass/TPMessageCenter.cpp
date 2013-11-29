// TPMessageExchange.cpp : implementation file
//

#include "stdafx.h"
#include "TPCommonClass.h"
#include "TPMessageCenter.h"

//extern CTPNetInterface     *g_pNetInterface; 

// CTPMessageCenter
TPExchangeCallBack   CTPMessageCenter::m_funCallBack[9]    = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
LPVOID               CTPMessageCenter::m_pCallBackParam[9] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

CTPMessageCenter  *g_pMessageExchange  = NULL;

IMPLEMENT_DYNAMIC(CTPMessageCenter, CWnd)

CTPMessageCenter::CTPMessageCenter()
{
 }

CTPMessageCenter::~CTPMessageCenter()
{
}


BEGIN_MESSAGE_MAP(CTPMessageCenter, CWnd)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MESSAGEEXCHANGE1,OnMessageExchange1)
	ON_MESSAGE(WM_MESSAGEEXCHANGE2,OnMessageExchange2)
	ON_MESSAGE(WM_MESSAGEEXCHANGE3,OnMessageExchange3)
	ON_MESSAGE(WM_MESSAGEEXCHANGE4,OnMessageExchange4)
	ON_MESSAGE(WM_MESSAGEEXCHANGE5,OnMessageExchange5)
	ON_MESSAGE(WM_MESSAGEEXCHANGE6,OnMessageExchange6)
	ON_MESSAGE(WM_MESSAGEEXCHANGE7,OnMessageExchange7)
	ON_MESSAGE(WM_MESSAGEEXCHANGE8,OnMessageExchange8)
	ON_MESSAGE(WM_MESSAGEEXCHANGE9,OnMessageExchange9)
END_MESSAGE_MAP()

void CTPMessageCenter::TransMessage(UINT uMessage)
{
	if(uMessage == 0)
	{
		TP_TransMessage(WM_MESSAGEEXCHANGE1);
		TP_TransMessage(WM_MESSAGEEXCHANGE2);
		TP_TransMessage(WM_MESSAGEEXCHANGE3);
		TP_TransMessage(WM_MESSAGEEXCHANGE4);
		TP_TransMessage(WM_MESSAGEEXCHANGE5);
		TP_TransMessage(WM_MESSAGEEXCHANGE6);
		TP_TransMessage(WM_MESSAGEEXCHANGE7);
		TP_TransMessage(WM_MESSAGEEXCHANGE8);
		TP_TransMessage(WM_MESSAGEEXCHANGE9);
	}
	else
		TP_TransMessage(uMessage);
}
HWND CTPMessageCenter::GetMessageWnd()
{
	if(g_pMessageExchange != NULL)
		return g_pMessageExchange ->GetSafeHwnd();
	else 
		return NULL;
}
void CTPMessageCenter::DestroyWnd()
{
	if(g_pMessageExchange != NULL)
	{
		g_pMessageExchange ->DestroyWindow();
		delete g_pMessageExchange;
		g_pMessageExchange = NULL;
	}
}
void CTPMessageCenter::CreateWnd()
{
	UINT  uStyle = WS_POPUPWINDOW; uStyle &= ~WS_VISIBLE;
	g_pMessageExchange = new CTPMessageCenter();
	g_pMessageExchange ->CreateEx(0,TP_GetRegisterClass(),NULL,uStyle,CRect(0,0,0,0),NULL,0); 
}

void CTPMessageCenter::OnDestroy()
{
	TransMessage(0);
	CWnd::OnDestroy();
}
LRESULT CTPMessageCenter::SendMessageEx(UINT uMessage,WPARAM wp,LPARAM lp)
{
	ASSERT(g_pMessageExchange != NULL);
	return g_pMessageExchange ->SendMessage(uMessage,wp,lp);
 }
LRESULT CTPMessageCenter::PostMessageEx(UINT uMessage,WPARAM wp,LPARAM lp)
{
	ASSERT(g_pMessageExchange != NULL);
	g_pMessageExchange ->PostMessage(uMessage,wp,lp);
	return S_OK;
}
LRESULT CTPMessageCenter::OnMessageExchange1(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[0])
		return m_funCallBack[0](wp,lp,m_pCallBackParam[0]);
	else 
		return S_FALSE;
}
LRESULT CTPMessageCenter::OnMessageExchange2(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[1])
		return m_funCallBack[1](wp,lp,m_pCallBackParam[1]);
	else 
		return S_FALSE;
}
LRESULT CTPMessageCenter::OnMessageExchange3(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[2])
		return m_funCallBack[2](wp,lp,m_pCallBackParam[2]);
	else 
		return S_FALSE;
}
LRESULT CTPMessageCenter::OnMessageExchange4(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[3])
		return m_funCallBack[3](wp,lp,m_pCallBackParam[3]);
	else 
		return S_FALSE;
}
LRESULT CTPMessageCenter::OnMessageExchange5(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[4])
		return m_funCallBack[4](wp,lp,m_pCallBackParam[4]);
	else 
		return S_FALSE;
}
LRESULT CTPMessageCenter::OnMessageExchange6(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[5])
		return m_funCallBack[5](wp,lp,m_pCallBackParam[5]);
	else 
		return S_FALSE;
}
LRESULT CTPMessageCenter::OnMessageExchange7(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[6])
		return m_funCallBack[6](wp,lp,m_pCallBackParam[6]);
	else 
		return S_FALSE;
}

LRESULT CTPMessageCenter::OnMessageExchange8(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[7])
		return m_funCallBack[7](wp,lp,m_pCallBackParam[7]);
	else 
		return S_FALSE;
}
LRESULT CTPMessageCenter::OnMessageExchange9(WPARAM wp,LPARAM lp)
{
	if(m_funCallBack[8])
		return m_funCallBack[8](wp,lp,m_pCallBackParam[8]);
	else 
		return S_FALSE;
}
void  CTPMessageCenter::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);
	 

}
