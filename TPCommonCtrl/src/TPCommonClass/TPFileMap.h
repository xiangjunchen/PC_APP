
#pragma once

class AFX_EXT_CLASS CMapGuidToGuid:public CMapStringToString
{
public:
	CMapGuidToGuid();
	~CMapGuidToGuid();
public:
	BOOL             m_bLoadData;
	CCriticalSection m_csLock;
	CString          m_sFileName;
	BOOL             m_bDataChange;
public:
	void   AddItem(GUID guidKey,GUID guidValue);
	BOOL   GetItem(GUID guidKey,GUID &guidValue);
	void   DelItem(GUID guidKey);
	void   Clear();
	void   SaveFile(CString sFile,BOOL bClear = FALSE);
	BOOL   LoadFile(CString sFileName);
public:
	static CString	  GuidToString( UUID *pUuid);	 
	static GUID       GuidFromString( LPCTSTR lpszUuid);
};
class AFX_EXT_CLASS CTPFileMap  
{
public:
	CTPFileMap();
	virtual ~CTPFileMap();
public:
	static  void      SetPath(CString sPath);      //������ʱĿ¼
	void              ClearAll();                   //ɾ�������ļ�
public:
	static   CString   m_sPath;                      //��ʱĿ¼
	struct   FileHead
	{
		DWORD      dVersion;
		BOOL       bAudio;        //�Ƿ�Ϊ��Ƶ 

		__int64    iFrame;        //��Ƶ
		__int64    iBeginFrame;   //��ʼ
		__int64    iEndFrame;     //���� 
		__int64    iBufferLength; //��С
		long       iReadMode;     //ģʽ 
		int        iWidth;
		int        iHeight;


		double     dSizePerFrame; //ÿ֡Sample��
		short      shMaxValue;    //ƽ��ֵ	
		long       lTimeRate;
		FileHead()
		{
			dVersion  = 1;
			lTimeRate = 25;
		}
	}   m_stuTypedata;
	CString  m_sFileName;

	LPVOID   m_pMem;
private:

	HANDLE   m_hFileHandle;
	HANDLE   m_hHandle;
	__int64  m_iBufferSize;	
	BOOL     m_bIsRead;	
	CTime    m_tmCreate;
	CMutex   m_lock;
	
public:
	static void SetVideoStandTimeRate(long       lTimeRate);
	BOOL        SetVideoFileName(CString strFileName,__int64 iFrame,CString sNameEx);
	BOOL        SetAudioFileName(CString strName, int iFileIndex,__int64 iBeginFrame, __int64 iEndFrame,int iReadMode);

	LPVOID      GetBuffer(__int64 iSize);
	BOOL        Release();
	LPVOID      GetBuffer(CString sFileName,int iSize);
	BYTE *      GetBuffer();

	BOOL        LockMemory(BOOL bSet,BOOL bFlush = FALSE);	
	BOOL        GetStatus(LPCTSTR lpszFileName, CFileStatus& rStatus);
};
class AFX_EXT_CLASS CTPAutoFileMap  
{
public:
	CTPAutoFileMap(CTPFileMap *pFileMap,BOOL bFlush = FALSE)
	{
		m_pFileMap = pFileMap;
		m_bFlush   = bFlush;
		if(m_pFileMap) m_pFileMap ->LockMemory(TRUE,m_bFlush);
	}
	virtual ~CTPAutoFileMap()
	{
		if(m_pFileMap) m_pFileMap ->LockMemory(FALSE,m_bFlush);
		m_pFileMap = NULL;
	}
private:
	CTPFileMap *m_pFileMap;
	BOOL        m_bFlush;
};