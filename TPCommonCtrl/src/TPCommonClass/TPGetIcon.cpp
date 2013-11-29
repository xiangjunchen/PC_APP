
#include "stdafx.h"
#include "TPGetIcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define   ICON_WIDTH    352
#define   ICON_HEIGHT   288
#define   BUFFER_LENGTH ICON_WIDTH * ICON_HEIGHT *4

void      *CTPGetIcon::m_pFileInterface     = NULL;
void      *CTPGetIcon::m_pTimeLineInterface = NULL;
void      *CTPGetIcon::m_pEffectInterface   = NULL;

CString TP_GetIconScaleFileName(TPGetIconScale *pScale)
{
	CString sNameScale = _T("");
	if(pScale) sNameScale.Format(_T("%d-%d-%d-%d-%d-%d-%.2f-%.2f-%.2f-%.2f"),(int)pScale->b1609, (int)pScale->dwPicturePos,(int)pScale->ptChipTopLeft.x,(int)pScale->ptChipTopLeft.y,(int)pScale->ptChipRightBottom.x,(int)pScale->ptChipRightBottom.y,pScale->dScaleX,pScale->dScaleY, pScale->dCenterX, pScale->dCenterY);

	return sNameScale;
}

CString TP_GetIconMutexName(CString sName,__int64 iFrame,TPGetIconScale *pScale)
{
	// snp4100024169 li song ming 2011-02-28  两个P2盘上的重名素材路径有可能重复，造成Icon生成有错，所以加入了盘符判断
	int iDriveFind = sName.Find('\\');
	CString sDrive = sName.Left(iDriveFind -1);
	CString sGet;
	int iFind = sName.ReverseFind('\\');	
	sGet = sName.Mid(iFind+1);
	sName = sName.Left(iFind-1);	
	iFind = sName.ReverseFind('\\');
	if(iFind<0)  sName = sName+sGet;
	else 
	{
		sName = sName.Mid(iFind+1);
		sName = sName+sGet;
	}
	sName += _T("-");
	sName += sDrive;
	if(pScale)sGet.Format(_T("%s-%x-%s"),sName,(int)iFrame,TP_GetIconScaleFileName(pScale));
	else      sGet.Format(_T("%s-%x"),sName,iFrame,sDrive);	
	sGet.MakeUpper();	
	return sGet;
}
void TP_TransBufferEx(LPDWORD lpDestBuffer, UINT nDestWidth, UINT nDestHeight,LPDWORD lpSrcBuffer, UINT nSrcWidth, UINT nSrcHeight)
{
	//temp
}

void TP_ReadIcon(TPGetIconItem *pItem)
{
	/*if(pItem ->pMapFile == NULL)
		pItem ->pMapFile = new  CTPFileMap();   
 	CString sNameEx = _T("");
	if(pItem->pScale)sNameEx = TP_GetIconScaleFileName(pItem->pScale);
 	if(!PathFileExists(pItem->sName))
	{
		pItem->bFail = TRUE; 		
		return;
	}
	BOOL bNeadRead = !pItem ->pMapFile ->SetVideoFileName(pItem->sName,pItem->iFrame, sNameEx); 
	pItem ->pBuffer = (BYTE *)pItem ->pMapFile ->GetBuffer(BUFFER_LENGTH + 200);
	if(pItem ->pBuffer == NULL)
	{
		pItem->bFail = TRUE; 		
		pItem ->pMapFile->LockMemory(FALSE,TRUE);
		return;
	}
	if(bNeadRead && pItem ->pBuffer)
	{		
		if(0)
		{
			TPTimeLineDataEdit *pTimeLineInterface = (TPTimeLineDataEdit *)CTPGetIcon::m_pTimeLineInterface;
		}
		else
		{
			TPFileManageInterface *pFileManageInterface = (TPFileManageInterface *)CTPGetIcon::m_pFileInterface;
			if(pFileManageInterface == NULL || pFileManageInterface ->TPFile_OpenFile == NULL)		return;			
			HTPFILE  hFile     = NULL;
			try
			{
				hFile     = pFileManageInterface ->TPFile_OpenFile((LPTSTR)(LPCTSTR)pItem->sName,TP_ACCESS_READ);
			}
			catch(...)
			{
				hFile = NULL;
			}
			if(hFile == NULL)    
			{
				pItem ->pMapFile->LockMemory(FALSE,TRUE);
				pItem ->bFail = TRUE;				
				return;		
			}
			LPTPFileInfo  pFileInfo = pFileManageInterface ->TPFile_GetFileInfo(hFile);
			if(pFileInfo == NULL || !(pFileInfo->dwMediaType & TP_MEDIA_CLASS_V)) 
			{
				pFileManageInterface ->TPFile_CloseFile(hFile);	 
				pItem ->bFail = TRUE;			
				pItem ->pMapFile->LockMemory(FALSE,TRUE);
				return;
			}
			if(pItem ->pScale && CTPGetIcon::GetGraphScale)
			{
				INT iSrcWidth  = pFileInfo->tpFileFormat.tpVideoFormat.tpImageFormat.iWidth;
				INT iSrcHeight = pFileInfo->tpFileFormat.tpVideoFormat.tpImageFormat.iHeight;

				INT iEditWidth  = TP_GetEditVideoWidth((TP_VIDEO_STANDARD)pItem->pScale->eVideoStandard);
				INT iEditHeight = TP_GetEditVideoHeight((TP_VIDEO_STANDARD)pItem->pScale->eVideoStandard);

				LPBYTE pSrc      = (LPBYTE)VirtualAlloc(NULL, iSrcWidth * iSrcHeight * 4, MEM_COMMIT, PAGE_READWRITE);	
				LPBYTE pDest     = (LPBYTE)VirtualAlloc(NULL, iEditWidth * iEditHeight * 4, MEM_COMMIT, PAGE_READWRITE);	
				try
				{				
					pFileManageInterface->TPFile_GetImage(hFile, iSrcWidth, iSrcHeight, 0, 0, 32, (INT)pItem->iFrame, pSrc);
				}
				catch(...)
				{		
					pItem ->bFail = TRUE;					
				}

				CTPGetIcon::GetGraphScale(pSrc, CSize(iSrcWidth, iSrcHeight), pDest, pItem->pScale);
				TP_TransBufferEx((LPDWORD)pItem->pBuffer,ICON_WIDTH, ICON_HEIGHT, (LPDWORD)pDest,iEditWidth, iEditHeight);

				TP_BlendDIBBalck(pItem ->pBuffer,ICON_WIDTH , ICON_HEIGHT);

				VirtualFree(pDest, 0, MEM_RELEASE);
				VirtualFree(pSrc, 0, MEM_RELEASE);
			}
			else
			{			
				if(PathFileExists(pItem ->sKeyName))
				{
					HTPFILE  hFileKey     = NULL;
					try
					{
						hFileKey     = pFileManageInterface ->TPFile_OpenFile((LPTSTR)(LPCTSTR)pItem->sKeyName,TP_ACCESS_READ);
					}
					catch(...)
					{
						hFileKey = NULL;
					}
					try
					{	
						if(hFileKey)
						{
							pFileManageInterface ->TPFile_GetMixImageEx(hFile,hFileKey,ICON_WIDTH , ICON_HEIGHT,0,0,32,(int)pItem ->iFrame,pItem ->pBuffer);
							pFileManageInterface ->TPFile_CloseFile(hFileKey);
						}
						else
						{
							pFileManageInterface ->TPFile_GetImage(hFile,ICON_WIDTH , ICON_HEIGHT,0,0,32,(int)pItem ->iFrame,pItem ->pBuffer);
						}
					}
					catch(...)
					{		
						pItem ->bFail = TRUE;					
					}
				}
				else
				{
					try
					{	
						pFileManageInterface ->TPFile_GetImage(hFile,ICON_WIDTH , ICON_HEIGHT,0,0,32,(int)pItem ->iFrame,pItem ->pBuffer);
					}
					catch(...)
					{		
						pItem ->bFail = TRUE;					
					}
				}
				pItem ->pMapFile->m_stuTypedata.iWidth  = pFileInfo->tpFileFormat.tpVideoFormat.tpImageFormat.iWidth;
				pItem ->pMapFile->m_stuTypedata.iHeight = pFileInfo->tpFileFormat.tpVideoFormat.tpImageFormat.iHeight;

				if(pFileInfo ->dwSubMediaType & TP_MEDIA_CLASS_V_P)  
				{
					if(pItem && pItem->pBuffer)
						TP_BlendDIBBalck(pItem ->pBuffer,ICON_WIDTH , ICON_HEIGHT);
				}
				else                     
				{
					if(pItem && pItem->pBuffer)
						for(INT l=0;l<ICON_WIDTH * ICON_HEIGHT;l++) pItem ->pBuffer[l*4+3] = 0xff; 
				}

			}
			pFileManageInterface ->TPFile_CloseFile(hFile);
			pItem ->pMapFile->LockMemory(FALSE,TRUE);
		}
	}*/
}

BOOL TP_ReadIconEx(TPGetIconItem *pItem) // DQAdb00009457 wangjin 20100324
{
 	BOOL bRet = TRUE;
	pItem->bIsRead = TRUE;			
	try
	{			
		TP_ReadIcon(pItem);
	}
	catch (...)
	{
		bRet = FALSE;
		ASSERT(0);
	}
	pItem->bIsRead = FALSE;

	return bRet;
}

UINT  TP_GetIconThread(LPVOID pVoid)
{
	CTPGetIcon          *pGetIcon =(CTPGetIcon *)pVoid;		
	while ( ::WaitForSingleObject(pGetIcon ->m_hGetEvent,500) == WAIT_OBJECT_0) if(pGetIcon ->m_bCancel) {pGetIcon ->m_pThread = NULL;	return 1;}	


	TPGetIconItem       *pItem;
	TPGetIconItemArray   aItemData;

NextItem:

	aItemData.RemoveAll();
	pGetIcon ->m_hMutex.Lock();

	POSITION       posGet      = pGetIcon ->m_aData.GetStartPosition();
	CString        sKey        = _L("");
	TPGetIconItem *pGetStruct  = NULL;	
	while (posGet)
	{
		pGetIcon ->m_aData.GetNextAssoc(posGet,sKey,(void *&)pGetStruct);
		if(pGetStruct && pGetStruct->bNew && !pGetStruct ->bCancle && !pGetStruct ->bFail && !pGetStruct ->bDelete && pGetStruct->sName.GetLength()>0) 
			aItemData.Add(pGetStruct);		
	}	
	pGetIcon ->m_hMutex.Unlock();


	while(aItemData.GetSize()>0)
	{	
		for(int l= 0;l<(int)aItemData.GetSize();l++)
		{
			if(pGetIcon ->m_bCancel) break;	
			pItem = aItemData[l];		
			CTPGuidLock stuLock((DWORD_PTR)pItem);
			TP_ReadIconEx(pItem); // DQAdb00009457 wangjin 20100324	
			pItem->bNew    = FALSE;
			pItem->Invalidate();			
		}	
		aItemData.RemoveAll();	
	}
	if(pGetIcon->m_bConntinue) 
	{
		pGetIcon->m_bConntinue = FALSE;
		goto NextItem;
	}
	pGetIcon ->m_pThread    = NULL;
	pGetIcon ->m_bConntinue = FALSE;
	return TRUE;	
}
void       (*CTPGetIcon::GetGraphScale)(LPBYTE,CSize &,LPBYTE,TPGetIconScale *) = NULL;
CTPGetIcon::CTPGetIcon()
{
	CTPFileMap::SetPath(TP_GetTempPath());

	m_pThread       = NULL;
	m_bCancel       = FALSE;
	m_iArrayLength  = 1000;
	m_dwTickCount   = GetTickCount();
	m_hGetEvent     = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	m_bConntinue    = FALSE;
}
CTPGetIcon::~CTPGetIcon()
{
	ReleaseAll();
	::CloseHandle(m_hGetEvent);
	m_hGetEvent = NULL;
}
void CTPGetIcon::SetTimeLineInterface(void *pTimeLineInterface)
{
	m_pTimeLineInterface = pTimeLineInterface;
}
void CTPGetIcon::SetEffectInterface(void *pEffectInterface)
{
	m_pEffectInterface = pEffectInterface;
}
void CTPGetIcon::SetFileInterface(void *pFileInterface)
{
	m_pFileInterface = pFileInterface;
}
void CTPGetIcon::StopThread()
{
	m_bConntinue = FALSE;
	if(m_pThread && m_pThread->m_hThread)
	{
		m_bCancel = TRUE;
		::SetEvent(m_hGetEvent);
		::WaitForSingleObject(m_pThread->m_hThread,1000000);		
		m_pThread = NULL;
	}
}
void CTPGetIcon::ReleaseAll()
{
	StopThread();

	m_hMutex.Lock();

	POSITION       posGet      = m_aData.GetStartPosition();
	CString        sKey        = _L(""),sDel;
	TPGetIconItem *pGetStruct  = NULL;	
	while (posGet)
	{
		m_aData.GetNextAssoc(posGet,sKey,(void *&)pGetStruct);
		delete pGetStruct;
	}	
	m_aData.RemoveAll(); 	

	m_hMutex.Unlock();
} 
void   CTPGetIcon::ClearData()
{
	CTPSyncLock stuLock(&m_hMutex);
	if(m_aData.GetSize() <= m_iArrayLength || m_pThread)  return;
	DWORD dwTickCount = TP_GetMMTimeMinSecond();
	if(dwTickCount - m_dwTickCount >1000)
	{
		m_dwTickCount = dwTickCount;
		return;
	}
	m_dwTickCount = dwTickCount;	
	POSITION       posGet      = m_aData.GetStartPosition();
	CString            sKey        = _L("");
	TPGetIconItem *pGetStruct  = NULL;	
	CArray                 <TPGetIconItem *,TPGetIconItem * &> aDel;
	CStringArray        aKey;
	while (posGet)
	{
		if(pGetStruct && !pGetStruct->bIsRead)
		{
			aDel.Add(pGetStruct);
			aKey.Add(sKey);			 
		}	
		pGetStruct = NULL;
		m_aData.GetNextAssoc(posGet,sKey,(void *&)pGetStruct);
	}
	for(INT l= 0;l<aKey.GetSize();l++)  
		m_aData.RemoveKey(aKey[l]);	
	for(INT l= 0;l<aDel.GetSize();l++) 
	{
		CTPGuidLock stuLock((DWORD_PTR)aDel[l]);
		delete aDel[l];
	}
	aDel.RemoveAll();
	aKey.RemoveAll();
}
BOOL   CTPGetIcon::GetIcon(TPGetIconItem * pStruct,BOOL bForce)
{
	if(pStruct ->szBuffer.cx > 360 || pStruct ->szBuffer.cy > 288 || pStruct ->szBuffer.cx <= 0 || pStruct ->szBuffer.cy <=0) return FALSE;
	CString sName = TP_GetIconMutexName(pStruct->sName,pStruct->iFrame,pStruct->pScale);
	TPGetIconItem * pGetStruct = NULL;	
	m_aData.Lookup(sName,(void *&)pGetStruct);		
	if(pGetStruct == NULL) 
	{
		TPGetIconItem *pData =new TPGetIconItem();
		CTPGuidLock stuLock((DWORD_PTR)pData);

		pData->bNew            =  TRUE;
		pData->bFail           =  FALSE;
		pData->bCancle         =  FALSE;
		pData->bDelete         =  FALSE;
		pData ->bIsRead        =  FALSE;		
		pData ->iFrame         =  pStruct ->iFrame;
		pData ->sName          =  pStruct ->sName;
		pData ->sKeyName       =  pStruct ->sKeyName;

		pData ->bRedrawFlag    =  pStruct ->bRedrawFlag;
		pData ->rtDraw         =  pStruct ->rtDraw;
		pData ->rtValidateRect =  pStruct ->rtValidateRect;
		pData ->hWnd           =  pStruct->hWnd;		
		pData ->pScale         =  pStruct ->pScale;
		pData ->pFileInterface =  pStruct ->pFileInterface;

		pStruct ->pScale       = NULL;

		TPGetIconItem::_DrawWnd stuWnd;

		stuWnd.bRedrawFlag     = pStruct->bRedrawFlag;
		stuWnd.hWnd            = pStruct->hWnd;
		stuWnd.rtDraw          = pStruct->rtDraw;
		stuWnd.rtValidateRect  = pStruct->rtValidateRect;

		pData ->arrWnd.Add(stuWnd);

		if(!bForce)
		{			
			ClearData();			
			m_aData.SetAt(sName,pData);	
			if(m_pThread == NULL || m_pThread->m_hThread == NULL)
			{				
				DeleteData();
				m_bCancel    = FALSE;
				m_bConntinue = FALSE;
				m_pThread = AfxBeginThread((AFX_THREADPROC)TP_GetIconThread,(LPVOID)this,THREAD_PRIORITY_IDLE);
				if(m_pThread) m_pThread ->m_bAutoDelete = TRUE;
			}
			else m_bConntinue = TRUE;
			::SetEvent(m_hGetEvent);			
			return FALSE;
		}
		else
		{
			pData->bFail = pData->bFail || !TP_ReadIconEx(pData);
			pData->bNew  = FALSE;
			ClearData();		
			m_aData.SetAt(sName,pData);						
			GetSystemTime(&pData->timeUser);			
			if(pData ->pMapFile == NULL) return FALSE;
			CTPAutoFileMap stuMap(pData ->pMapFile,FALSE); 
			pData ->pBuffer  = pData ->pMapFile ->GetBuffer();
			if(pData ->pBuffer && !pData ->bFail  && pStruct ->rtDraw.Width()>4)
			{			
				TP_TransBufferEx((DWORD *)pStruct ->pBuffer, pStruct->szBuffer.cx,pStruct->szBuffer.cy,(DWORD *)pData->pBuffer,ICON_WIDTH,ICON_HEIGHT);  
				return TRUE;
			}
			else	return FALSE;
		}
	}
	else
	{		
		CTPGuidLock stuLock((DWORD_PTR)pGetStruct);
		GetSystemTime(&pGetStruct->timeUser);
		if(pGetStruct->bFail || pGetStruct->bIsRead || pGetStruct->bCancle || pGetStruct->bDelete) 
		{
			pGetStruct ->AddWnd(pStruct);
			if(pGetStruct->bCancle && m_pThread == NULL)
			{
				DeleteData();
				m_bCancel    = FALSE;
				m_bConntinue = FALSE;
				m_pThread = AfxBeginThread((AFX_THREADPROC)TP_GetIconThread,(LPVOID)this,THREAD_PRIORITY_IDLE);
				if(m_pThread)  m_pThread ->m_bAutoDelete = TRUE;
			}
			return FALSE;		
		}
		else
		{		
			CTPAutoFileMap stuMap(pGetStruct ->pMapFile,FALSE); 
			if(pGetStruct ->pMapFile == NULL) goto ErrorRead;		
			pGetStruct ->pBuffer  = pGetStruct ->pMapFile ->GetBuffer();
			pStruct->szFile       = CSize(pGetStruct ->pMapFile ->m_stuTypedata.iWidth,pGetStruct ->pMapFile ->m_stuTypedata.iHeight);
			if(pGetStruct ->pBuffer && pStruct ->rtDraw.Width() > 4) 
			{
				TP_TransBufferEx((DWORD *)pStruct ->pBuffer, pStruct->szBuffer.cx,pStruct->szBuffer.cy,(DWORD *)pGetStruct->pBuffer,ICON_WIDTH,ICON_HEIGHT);  
				return TRUE;
			}
			else 
			{
ErrorRead:
				pGetStruct ->AddWnd(pStruct);
				return FALSE;		
			}
		}
	}	
	return FALSE;

}
BOOL CTPGetIcon::GetDefaultIcon(int iState,TPGetIconItem * pStruct)
{
	if(iState == 0)
	{
		TPCommonRes *pAudio = TP_LoadBmpByte(TP_GetSkinPath() + _L("\\ClipExplorer\\DefaultIcon\\AClip.bmp"));
		if(pAudio)
		{
			TP_TransBufferEx((DWORD *)pStruct ->pBuffer, pStruct->szBuffer.cx,pStruct->szBuffer.cy,(DWORD *)pAudio ->GetByte(),pAudio ->Width(),pAudio ->Height());  
			TP_ReleaseCommonRes(pAudio);
			return S_OK;
		}
	}	
	else if(1 == iState)
	{
		TPCommonRes *pMediaOffline = TP_LoadBmpByte(TP_GetSkinPath() + _T("\\ClipExplorer\\DefaultIcon\\scr_media_off_1920.bmp"));
		if(pMediaOffline)
		{
			TP_TransBufferEx((DWORD *)pStruct ->pBuffer, pStruct->szBuffer.cx,pStruct->szBuffer.cy,(DWORD *)pMediaOffline ->GetByte(),pMediaOffline ->Width(),pMediaOffline ->Height());  
			TP_ReleaseCommonRes(pMediaOffline);
			return S_OK;
		}

	}
	return S_FALSE;
};
void CTPGetIcon::ReLoadIcon(CString sFileName,CString sKeyName,int iFrame)
{
	CString sName = TP_GetIconMutexName(sFileName,iFrame,NULL);
	TPGetIconItem * pGetStruct = NULL;
	m_aData.Lookup(sName,(void *&)pGetStruct);		
	if(pGetStruct) 
	{		
		CTPGuidLock stuLock((DWORD_PTR)pGetStruct); 
		pGetStruct ->bCancle = TRUE;
		pGetStruct ->bFail   = FALSE;
		pGetStruct ->bDelete = FALSE;			
		pGetStruct ->bNew    = FALSE;						
		pGetStruct ->bIsRead = FALSE;	
		stuLock.Unlock();
		if(m_pThread == NULL || m_pThread->m_hThread == NULL)
		{
			m_bConntinue = FALSE;
			DeleteData();
			m_bCancel  = FALSE;
			m_pThread = AfxBeginThread((AFX_THREADPROC)TP_GetIconThread,(LPVOID)this,THREAD_PRIORITY_IDLE);
			if(m_pThread)  m_pThread ->m_bAutoDelete = TRUE;
		}
		else 
		{
			CTPGuidLock stuLock((DWORD_PTR)pGetStruct); 
			if(pGetStruct ->pMapFile )//jyn added XN00016043 20081105
			{			
				pGetStruct ->pMapFile ->Release();
				DeleteFile(pGetStruct ->pMapFile ->m_sFileName);
				delete pGetStruct ->pMapFile;	
				pGetStruct ->pMapFile = NULL;
				pGetStruct ->pBuffer  = NULL;
			}
			pGetStruct ->bNew      = TRUE;
			pGetStruct ->bFail     = FALSE;			
			pGetStruct ->bCancle   = FALSE;			
			pGetStruct ->pMapFile  = NULL;
			pGetStruct ->pBuffer   = NULL;			
			m_bConntinue = TRUE;
		}

		::SetEvent(m_hGetEvent);
	}
}
void CTPGetIcon::DeleteData()
{
	CTPSyncLock stuLock(&m_hMutex);
	POSITION       posGet      = m_aData.GetStartPosition();
	CString        sKey        = _L("");
	TPGetIconItem *pGetStruct  = NULL;
	CArray<TPGetIconItem*,TPGetIconItem*&> arrDel;
	CStringArray                           arrKey;
	while (posGet)
	{
		m_aData.GetNextAssoc(posGet,sKey,(void *&)pGetStruct);
		if(pGetStruct == NULL) continue;
		CTPGuidLock stuLock((DWORD_PTR)pGetStruct); 
		if(!pGetStruct->bIsRead)
		{		
			if(pGetStruct->bDelete ) 
			{			
				if(pGetStruct ->pMapFile)
				{			
					pGetStruct ->pMapFile ->Release();
					DeleteFile(pGetStruct ->pMapFile ->m_sFileName);
					delete pGetStruct ->pMapFile;
					pGetStruct ->pMapFile = NULL;
					pGetStruct ->pBuffer  = NULL;
				}
				arrDel.Add(pGetStruct);
				arrKey.Add(sKey);			
			}
			else if(pGetStruct->bCancle)
			{
				if(pGetStruct ->pMapFile )
				{			
					pGetStruct ->pMapFile ->Release();
					DeleteFile(pGetStruct ->pMapFile ->m_sFileName);
					delete pGetStruct ->pMapFile;	
					pGetStruct ->pMapFile = NULL;
					pGetStruct ->pBuffer  = NULL;
				}
				pGetStruct ->bNew      = TRUE;
				pGetStruct ->bFail     = FALSE;			
				pGetStruct ->bCancle   = FALSE;			
				pGetStruct ->pMapFile  = NULL;
				pGetStruct ->pBuffer   = NULL;			
			}
		}
	}
	for(INT k=0;k<arrDel.GetSize();k++) 
	{
		m_aData.RemoveKey(arrKey[k]);
		delete arrDel[k];
	}
	arrDel.RemoveAll();
}
void CTPGetIcon::ReLoadAll()
{	
	POSITION       posGet      = m_aData.GetStartPosition();
	CString        sKey        = _L("");
	TPGetIconItem *pGetStruct  = NULL;

	m_aData.GetNextAssoc(posGet,sKey,(void *&)pGetStruct);
	while (pGetStruct)
	{		
		CTPGuidLock stuLock((DWORD_PTR)pGetStruct); 
		pGetStruct ->bCancle = TRUE;
		pGetStruct ->bFail   = FALSE;
		pGetStruct ->bDelete = FALSE;			
		pGetStruct ->bNew    = FALSE;						
		pGetStruct ->bIsRead = FALSE;			
		m_aData.GetNextAssoc(posGet,sKey,(void *&)pGetStruct);
	}
	if(m_pThread == NULL || m_pThread->m_hThread == NULL)
	{
		m_bConntinue = FALSE;
		DeleteData();
		m_bCancel  = FALSE;
		m_pThread = AfxBeginThread((AFX_THREADPROC)TP_GetIconThread,(LPVOID)this,THREAD_PRIORITY_IDLE);
		if(m_pThread) m_pThread ->m_bAutoDelete = TRUE;
	}
	else 
		m_bConntinue = TRUE;
	::SetEvent(m_hGetEvent);
}