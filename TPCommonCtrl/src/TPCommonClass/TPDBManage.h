#pragma once

class AFX_EXT_CLASS	CTPDBManage
{
public:
	CTPDBManage(void);
	~CTPDBManage(void);

	static CTPDBManage & instance();
	static void DestoryInstance();
	BOOL Initialize();

	// 获得站点数据库访问类对象
	CTPDBInterface * GetDBInterface(long lSiteID);
	CMap<long, long, CTPDBInterface *,CTPDBInterface *>	m_mapDBInterface; // 数据库访问map



private:
	static CTPDBManage * m_pSinstance;
	inline CTPDBInterface * CreateDBInterface(long lSiteID,ETVML_PLUGIN_TYPE lPlugInType);

	CTPDBInterface * m_pDefaultDBInterface;//缺省数据库访问接口
	CCriticalSection m_csDBInterface;

};
