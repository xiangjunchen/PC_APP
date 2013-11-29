#pragma once

class AFX_EXT_CLASS	CTPDBManage
{
public:
	CTPDBManage(void);
	~CTPDBManage(void);

	static CTPDBManage & instance();
	static void DestoryInstance();
	BOOL Initialize();

	// ���վ�����ݿ���������
	CTPDBInterface * GetDBInterface(long lSiteID);
	CMap<long, long, CTPDBInterface *,CTPDBInterface *>	m_mapDBInterface; // ���ݿ����map



private:
	static CTPDBManage * m_pSinstance;
	inline CTPDBInterface * CreateDBInterface(long lSiteID,ETVML_PLUGIN_TYPE lPlugInType);

	CTPDBInterface * m_pDefaultDBInterface;//ȱʡ���ݿ���ʽӿ�
	CCriticalSection m_csDBInterface;

};
