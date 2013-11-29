#pragma once
#include <AfxTempl.h>

struct TPFileInterface;
typedef struct stu_TPGetIconScale
{		
	DWORD    eVideoStandard;
	DWORD	 dwPicturePos;		  //ͼƬ��λ��
	POINT    ptChipTopLeft;       //�������Ͻ�
	POINT    ptChipRightBottom;   //�������Ͻ�
	BOOL	 b1609;				  //�Ƿ�1609���� // LI HUI LI 2009-03-25

	double   dScaleX;             //X����
	double   dScaleY;             //Y����
	float    dCenterX;           //�������ĵ�
	float    dCenterY;           //�������ĵ�
}TPGetIconScale;
typedef struct tag_TPGetIconItem
{
	BOOL            bFail;        //shibai
	BOOL            bCancle;       //��ȡȡ��
	BOOL            bDelete;       //�˳���ʱ�� ��Ҫɾ�� 
	BOOL            bNew;          //�½�����Ŀ

	BOOL            bIsRead;       //��ǰ���ڶ�	
	CString         sName;         //�ļ���
	CString         sKeyName;
	BYTE           *pBuffer;       //�ڴ�
	CTPFileMap     *pMapFile;      //�ļ�
	SYSTEMTIME      timeUser;      //���ʹ��ʱ��
	__int64         iFrame;        //��ǰ֡��
	CSize           szBuffer;      //���ݴ�С

	TPGetIconScale  *pScale;
	struct _DrawWnd
	{	
		BOOL           *bRedrawFlag;   //�ػ���־
		HWND            hWnd;          //�ػ�����
		CRect           rtDraw;      //�ػ�����	
		CRect           rtValidateRect;
		_DrawWnd()
		{	
			bRedrawFlag    = NULL;
			hWnd           = NULL;
			rtDraw         = CRect(0,0,0,0);
			rtValidateRect = CRect(0,0,0,0);
		}		
	} ;
	CArray<_DrawWnd,_DrawWnd &> arrWnd;

	BOOL           *bRedrawFlag;   //�ػ���־
	HWND            hWnd;          //�ػ�����
	CRect           rtDraw;        //�ػ�����	
	CRect           rtValidateRect;        //�ػ�����	
	
	CSize             szFile;       //ԭ���ļ��Ĵ�С
	TPFileInterface  *pFileInterface; 

	void           *hTPEffect; 
	__int64         lClipInFrame;
	__int64         lClipOutFrame;

	tag_TPGetIconItem()
	{
		bFail        = FALSE;
		bCancle      = FALSE;
		bDelete      = FALSE;
		bNew         = TRUE;
		bIsRead      = TRUE;		
	    sName        = _L("");
		sKeyName     = _L("");
	    pBuffer      = NULL;
		pMapFile     = NULL;
		GetLocalTime(&timeUser);
	    iFrame       = 0;
	    szBuffer   = CSize(0,0);
	    bRedrawFlag  = NULL;
	    hWnd         = NULL;
		rtDraw       = CRect(0,0,0,0);
		rtValidateRect = CRect(0,0,0,0);

		szFile         = CSize(0,0);
		pFileInterface = NULL;

		hTPEffect      = NULL; 
		lClipInFrame   = -1;
		lClipOutFrame  = -1;
		pScale         = NULL;

	};
	void Invalidate()
	{
		CMutex stuLock(FALSE,_T("GetIcon"));
		stuLock.Lock();
		for(INT k=0;k<this->arrWnd.GetSize();k++)
		{
			if(this->arrWnd[k].hWnd!=NULL&&::IsWindow(this->arrWnd[k].hWnd)) 
			{				
				if(this->arrWnd[k].bRedrawFlag)   *(this->arrWnd[k].bRedrawFlag) +=1;
				if(this->arrWnd[k].rtValidateRect.Width()<=0) ::InvalidateRect(this->arrWnd[k].hWnd,NULL,TRUE);
				else                                          ::InvalidateRect(this->arrWnd[k].hWnd,&this->arrWnd[k].rtValidateRect,TRUE);
			}
		}
		this->arrWnd.RemoveAll();
		stuLock.Unlock();
	}
	BOOL IsDraw(int iWidth)
	{
		for(INT k=0;k<this->arrWnd.GetSize();k++) if(this->arrWnd[k].rtDraw.Width()>iWidth) return TRUE;
		return FALSE;		
	}
	void AddWnd(tag_TPGetIconItem *pStruct)
	{
		CMutex stuLock(FALSE,_T("GetIcon"));
		stuLock.Lock();
		if(pStruct ->hWnd)
		{
			INT k = 0;
			for(k=0;k<this->arrWnd.GetSize();k++)
			{
				if(this->arrWnd[k].hWnd == pStruct->hWnd)						
				{
					this->arrWnd[k].rtValidateRect |= pStruct->rtValidateRect;
					break;
				}
			}
			if(k>=this->arrWnd.GetSize())
			{
				TPGetIconItem::_DrawWnd stuWnd;
				stuWnd.bRedrawFlag          = pStruct->bRedrawFlag;
				stuWnd.hWnd                 = pStruct->hWnd;
				stuWnd.rtValidateRect       = pStruct->rtValidateRect;
				this->arrWnd.Add(stuWnd);
			}				
		}
		stuLock.Unlock();
	}
	~tag_TPGetIconItem()
	{
		if(pMapFile)
		{
			pMapFile->Release();
			delete pMapFile;
			pMapFile = NULL;
		}
		pBuffer    = NULL;
		if(pScale) delete pScale;		
	}
} TPGetIconItem;
typedef CArray <TPGetIconItem *,TPGetIconItem *&> TPGetIconItemArray;

class AFX_EXT_CLASS CTPGetIcon  
{
public:
	CTPGetIcon();
	virtual ~CTPGetIcon();
public:
	int               m_iArrayLength;
	static void      *m_pFileInterface;
	static void      *m_pTimeLineInterface;
	static void      *m_pEffectInterface;
public:
	CMapStringToPtr     m_aData;
	CWinThread         *m_pThread;
	BOOL                m_bCancel;
	DWORD               m_dwTickCount;
	HANDLE              m_hGetEvent;
	CMutex              m_hMutex; 
	BOOL                m_bConntinue;

	static void       (*GetGraphScale)(LPBYTE pSrc,CSize &szBuffer,LPBYTE pDest,TPGetIconScale *pScale);

public:
	static void         SetEffectInterface(void *pEffectInterface);
	static void         SetFileInterface(void *pFileInterface);
	static void         SetTimeLineInterface(void *pTimeLineInterface);	
	

	BOOL   GetIcon(TPGetIconItem * pStruct,BOOL bForce =FALSE);
	BOOL   GetDefaultIcon(int iState,TPGetIconItem * pStruct);

//	BOOL   GetIcon(CString sFileName,__int64 lPos,LPBYTE &pByte,CSize &szBuffer);
//	BOOL   GetIcon(CString sFileName,__int64 lPos,LPBYTE &pByte,CSize &szBuffer,DWORD &dwFileType);

	void   ReleaseAll(); 

	void   ReLoadIcon(CString sFileName,CString sKeyName,int iFrame);
	void   ReLoadAll();

	void   StopThread();
private:
	void   ClearData();
	void   DeleteData();
};
