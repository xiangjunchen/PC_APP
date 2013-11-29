// SeBaseWndController.cpp: CTPMouseDeviceImpl �N���X�̃C���v�������e�[�V����
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
INT						CTPMouseDeviceImpl::m_uCount		 =0;	//���������Ĵ��ڶ���Ψһ�ģ�ֻ��audio input tool���ǣ�����������ü�����audio input tool
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
// �}�E�X�f�o�C�X�t�b�N(DirectInput)�@�\��FxEditor�ɕt����

int CTPMouseDeviceImpl::GetSliderOperationType()
{
	int nType = m_nOperateType;

	/////////////////////////////////////////////////////////////
	// Hooker�������ꍇ�́AIntelligent/Normal���삪�s�\�Ȃ̂�
	// �����I�ɃI���W�i�����[�h�ɂ���
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

	// ���Ɋm�ۍς݂Ȃ�΍Ċm�ۂ��Ȃ�
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
// �}�E�X�f�o�C�X�t�b�N(DirectInput)�@�\��FxEditor����͂���

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

