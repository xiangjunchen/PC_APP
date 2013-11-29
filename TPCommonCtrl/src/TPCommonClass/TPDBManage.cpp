#include "StdAfx.h"


CTPDBManage * CTPDBManage::m_pSinstance = NULL;

CTPDBManage & CTPDBManage::instance()
{
	if(NULL == m_pSinstance)
	{
		m_pSinstance = new CTPDBManage;
		m_pSinstance->Initialize();
	}
	return *m_pSinstance;

}
void CTPDBManage::DestoryInstance()
{
	Sleep(10);
	if(m_pSinstance != NULL)
	{
		delete m_pSinstance;
		m_pSinstance = NULL;
	}
	//CETMediaProcess::DestoryInstance();
}


CTPDBManage::CTPDBManage(void)
{
}

CTPDBManage::~CTPDBManage(void)
{
	long lSiteID;
	POSITION pos;
	CTPDBInterface * pDBInterface;
	for(pos = m_mapDBInterface.GetStartPosition();pos;)
	{
		pDBInterface = NULL;
		m_mapDBInterface.GetNextAssoc(pos,lSiteID,pDBInterface);
		if(pDBInterface)
		{
			delete pDBInterface;
		}
	}
	m_mapDBInterface.RemoveAll();
}
BOOL CTPDBManage::Initialize()
{
	return TRUE;
}
inline CTPDBInterface * CTPDBManage::CreateDBInterface(long lSiteID,ETVML_PLUGIN_TYPE lPlugInType)
{
	CTPDBInterface * pDBInterface = NULL;
	if (lPlugInType == ETVML_PLUGIN_TYPE_LOCAL)
	{
		pDBInterface = new CTPAccessDBInterface();
		pDBInterface->SetDatabaseID(lSiteID);
	}
	else if (lPlugInType == ETVML_PLUGIN_TYPE_NETWORK)
	{
	}
	else if (lPlugInType == ETVML_PLUGIN_TYPE_REMOTE)
	{
	}
	else if (lPlugInType == ETVML_PLUGIN_TYPE_ARCHIVE)
	{
	}
	else if (lPlugInType == ETVML_PLUGIN_TYPE_REMOTEARCHIVE)
	{
	}
	else
	{
		CString strMsg;
		strMsg.Format(_T("ETBASICMGR_INVALID_SITE"),lPlugInType);
		//ETMessageBox(strMsg);
	}
	return pDBInterface;
}
CTPDBInterface * CTPDBManage::GetDBInterface(long lSiteID)
{
	if(lSiteID == 0)
		return m_pDefaultDBInterface;

	CTPDBInterface * pInterface = NULL;
	m_csDBInterface.Lock();
	try
	{

		m_mapDBInterface.Lookup(lSiteID,pInterface);
		if(pInterface)
		{
			m_csDBInterface.Unlock();
			return pInterface;
		}

		pInterface = CreateDBInterface(lSiteID,ETVML_PLUGIN_TYPE_LOCAL);
		m_mapDBInterface.SetAt(lSiteID,pInterface);
	}
	catch(_com_error)
	{
	}
	catch(...)
	{
	}
	m_csDBInterface.Unlock();
	return pInterface;
}

