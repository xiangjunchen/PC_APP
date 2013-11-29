// SeBaseWndController.cpp: CTPMouseDeviceImpl 僋儔僗偺僀儞僾儕儊儞僥乕僔儑儞
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
INT						CTPMouseDeviceImpl::m_uCount		 =0;	//所有其他的窗口都是唯一的，只有audio input tool不是，所以这个引用计数给audio input tool
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
// 儅僂僗僨僶僀僗僼僢僋(DirectInput)婡擻傪FxEditor偵晅偗傞

int CTPMouseDeviceImpl::GetSliderOperationType()
{
	int nType = m_nOperateType;

	/////////////////////////////////////////////////////////////
	// Hooker偑柍偄応崌偼丄Intelligent/Normal憖嶌偑晄壜擻側偺偱
	// 嫮惂揑偵僆儕僕僫儖儌乕僪偵偡傞
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

	// 婛偵妋曐嵪傒側傜偽嵞妋曐偟側偄
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
// 儅僂僗僨僶僀僗僼僢僋(DirectInput)婡擻傪FxEditor偐傜偼偢偡

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

