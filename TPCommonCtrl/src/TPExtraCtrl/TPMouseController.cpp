// SeBaseWndController.cpp: CTPMouseDeviceImpl クラスのインプリメンテーション
//
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TPMouseController.h"
#include "TPMouseDeviceHooker.h"
#include "TPMouseCalc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTPMouseDeviceHooker*	CTPMouseDeviceImpl::m_pMouseDvHooker = NULL;
int						CTPMouseDeviceImpl::m_nOperateType = CTPMouseCalc::SE_SLT_INTELLIGENT;
CTPMouseCalc*			CTPMouseDeviceImpl::m_pDefCalculator = NULL;
INT						CTPMouseDeviceImpl::m_uCount		 =0;	//ﾋ�ﾓﾐﾆ萢�ｵﾄｴｰｿﾚｶｼﾊﾇﾎｨﾒｻｵﾄ｣ｬﾖｻﾓﾐaudio input toolｲｻﾊﾇ｣ｬﾋ�ﾒﾔﾕ篋�ﾒ�ﾓﾃｼﾆﾊ�ｸ�audio input tool
DWORD					CTPMouseDeviceImpl::m_dwWinAry       =0;


/////////////////////////////////////////////////////////////////////////////
// CTPMouseDeviceImpl


CTPMouseDeviceImpl::CTPMouseDeviceImpl()
{
}

CTPMouseDeviceImpl::~CTPMouseDeviceImpl()
{
	if(m_pDefCalculator)
	{
		delete m_pDefCalculator;
		m_pDefCalculator = NULL;
	}
	//UninstallMouseDeviceHooker();
}

/////////////////////////////////////////////////////////////////////////////
// マウスデバイスフック(DirectInput)機能をFxEditorに付ける

int CTPMouseDeviceImpl::GetSliderOperationType()
{
	int nType = m_nOperateType;

	/////////////////////////////////////////////////////////////
	// Hookerが無い場合は、Intelligent/Normal操作が不可能なので
	// 強制的にオリジナルモードにする
	/////////////////////////////////////////////////////////////
	switch(m_nOperateType)
	{
	case CTPMouseCalc::SE_SLT_INTELLIGENT:
	case CTPMouseCalc::SE_SLT_NORMAL:
		nType = (m_pMouseDvHooker != NULL)? m_nOperateType : CTPMouseCalc::SE_SLT_ORIGINAL;
		break;
	default:
		break;
	}
	return nType;	
}


bool CTPMouseDeviceImpl::InstallMouseDeviceHooker(CWnd* pHookOwner,DWORD dwWinType)
{
	if(CTPMouseDeviceImpl::m_dwWinAry & dwWinType)
		return TRUE;
	else 
		CTPMouseDeviceImpl::m_dwWinAry = CTPMouseDeviceImpl::m_dwWinAry |dwWinType;

	if(TP_SMARTSLIDER_WIN_AUDIOINPUT == dwWinType)
		CTPMouseDeviceImpl::m_uCount++;

	if(!pHookOwner || !::IsWindow(pHookOwner->GetSafeHwnd())){
		return false;
	}

	// 既に確保済みならば再確保しない
	if(m_pMouseDvHooker != NULL)
		return true;

	m_pMouseDvHooker = new CTPMouseDeviceHooker;
	if(!m_pMouseDvHooker->InstallMouseDeviceHook(pHookOwner->GetSafeHwnd()))
	{
		delete m_pMouseDvHooker;
		m_pMouseDvHooker = NULL;
	}
	else
	{	
		if(!m_pDefCalculator){
			m_pDefCalculator = new CTPMouseCalcIntelligent;
		}
	}

	return (m_pMouseDvHooker != NULL);
}

/////////////////////////////////////////////////////////////////////////////
// マウスデバイスフック(DirectInput)機能をFxEditorからはずす

void CTPMouseDeviceImpl::UninstallMouseDeviceHooker(DWORD dwWinType)
{
	if(CTPMouseDeviceImpl::m_dwWinAry & dwWinType)
	{
		if(TP_SMARTSLIDER_WIN_AUDIOINPUT == dwWinType)
		{
			CTPMouseDeviceImpl::m_uCount--;
			if(CTPMouseDeviceImpl::m_uCount<=0)
			{
				CTPMouseDeviceImpl::m_uCount =0;
				CTPMouseDeviceImpl::m_dwWinAry &=~dwWinType;
			}
		}
		else
			CTPMouseDeviceImpl::m_dwWinAry &=~dwWinType;
	}
	else 
		return;
	
	if(0 == CTPMouseDeviceImpl::m_dwWinAry )
	{
		//CTPMouseDeviceImpl::m_uCount =0;
		if(m_pMouseDvHooker)
		{
			m_pMouseDvHooker->UninstallMouseDeviceHook();
			delete m_pMouseDvHooker;
			m_pMouseDvHooker = NULL;
		}
		if(m_pDefCalculator)
		{
			delete m_pDefCalculator;
			m_pDefCalculator = NULL;
		}
	}
}

