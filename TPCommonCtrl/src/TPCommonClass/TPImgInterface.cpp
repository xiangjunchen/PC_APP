#include "StdAfx.h"

// �µ�ʹ�û����д�ļ��� begin
#define PNG_FILE_EXIT
#ifdef PNG_FILE_EXIT


#include "zlib.h"
#include "png.h"

#ifdef _M_X64
#pragma comment (lib,"zlib64.lib")
#pragma comment (lib,"png64.lib")
#else
#if _MSC_VER >= 1500
#pragma comment (lib,"zlib2008.lib")
#pragma comment (lib,"png2008.lib")
#else
#pragma comment (lib,"zlib2003.lib")
#pragma comment (lib,"png2003.lib")
#endif
#endif
#endif


#include "TPAVTgaFile.h"
#include "Jpegfile.h"
#include "gif.h"

extern BOOL SGBufferReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern BOOL _SGGetGifFrameNum(CSGBufferFile *pBufferFile, WORD *pwFirstFrameDelayTime, int *piFrameNum);
extern BYTE *SGGetGifImage(SGGIFFileData* pSGGifData, int nImageIndex, int *parWidth, int *parHeight, unsigned short *parDelayTime);


typedef struct tag_SGBMPInfo
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	HANDLE hBmpFile; // ��ʱ�򿪵� BMP �ļ���������ԭ���� lpResource->fp
	tag_SGBMPInfo()
	{
		TP_MemsetChar(&fileHeader, 0, sizeof(fileHeader));
		TP_MemsetChar(&infoHeader, 0, sizeof(infoHeader));
		hBmpFile = NULL;
	}
}SGBMPInfo;
typedef struct tag_PngFileType
{
	png_structp pPngStruct;
	png_infop pPngInfo;
	CSGBufferFile pngCNXBufferedFile;
	tag_PngFileType(){
		pPngInfo = NULL;
		pPngStruct = NULL;
	}
}PngFileType;
CSGBufferFile::CSGBufferFile(void)
{
	_Init();
}

inline void CSGBufferFile::_Init(void)
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_llFileSize.QuadPart = 0;
	m_llFileCursor.QuadPart = 0;
	m_bWriteFile = FALSE;
	m_pBufPool = NULL;
	m_pPoolCursor = NULL;
	m_pPoolRoof = NULL;
}

void CSGBufferFile::_Clear(void)
{
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	if(m_pBufPool != NULL)
	{
		delete []m_pBufPool;
		m_pBufPool = NULL;
		m_pPoolCursor = NULL;
		m_pPoolRoof = NULL;
	}
}

HANDLE CSGBufferFile::SGCreateFile(LPCTSTR lpFileName, DWORD dwDesireAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	try
	{
		_Clear();
		m_hFile = ::CreateFile(lpFileName, dwDesireAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		m_pBufPool = new BYTE[MAX_BUFFER_POOL_SIZE];
		m_pPoolCursor = m_pBufPool;
		m_pPoolRoof = m_pBufPool;
		m_llFileCursor.QuadPart = 0;

		if(dwDesireAccess & GENERIC_WRITE)
		{
			m_bWriteFile = TRUE;
		}
		else
		{
			m_bWriteFile = FALSE;
		}

		::GetFileSizeEx(m_hFile, &m_llFileSize);
		return m_hFile;
	}
	catch(...)
	{
		_Clear();
		return FALSE;
	}
}

BOOL CSGBufferFile::SGReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	if(m_llFileSize.QuadPart == 0 && m_pBufPool == m_pPoolRoof)
	{
		if(lpNumberOfBytesRead != NULL)
		{
			*lpNumberOfBytesRead = 0;
		}

		return FALSE;
	}

	if(m_pPoolRoof == m_pPoolCursor && dwNumberOfBytesToRead < MAX_BUFFER_POOL_SIZE)
	{
		// �����Ϊ�գ���Ҫ���ļ���ȡ�ֽڵ�������
		DWORD dwRealReadByte = 0;
		::ReadFile(m_hFile, m_pBufPool, MAX_BUFFER_POOL_SIZE, &dwRealReadByte, lpOverlapped);
		m_pPoolCursor = m_pBufPool;
		m_pPoolRoof = m_pBufPool + dwRealReadByte;
	}

	DWORD dwByteInPool = m_pPoolRoof - m_pPoolCursor; //����������ֽ���Ŀ
	DWORD dwCopyByte = dwByteInPool > dwNumberOfBytesToRead ? dwNumberOfBytesToRead : dwByteInPool;

	if(dwCopyByte > 0)
	{
		TP_MemcpyLine(lpBuffer, m_pPoolCursor, dwCopyByte);
		m_pPoolCursor += dwCopyByte;
		dwNumberOfBytesToRead -= dwCopyByte;
		m_llFileCursor.QuadPart += dwCopyByte;
	}

	if(lpNumberOfBytesRead != NULL)
	{
		*lpNumberOfBytesRead = dwCopyByte;
	}

	if(dwNumberOfBytesToRead == 0)
	{
		// ����ؿ�������Ҫ��
		return TRUE;
	}

	BYTE *pDestBuf = (BYTE *)lpBuffer + dwCopyByte;

	// ������Ѿ�ȡ�꣬��Ҫ���ļ���ȡ
	if(dwNumberOfBytesToRead > MAX_BUFFER_POOL_SIZE)
	{
		// ֱ�Ӵ��ļ���ȡ�����ٻ���ؿ����м价��
		DWORD dwRealReadByte = 0;
		BOOL bRet = ::ReadFile(m_hFile, pDestBuf, dwNumberOfBytesToRead, &dwRealReadByte, lpOverlapped);

		if(lpNumberOfBytesRead != NULL)
		{
			*lpNumberOfBytesRead += dwRealReadByte;
		}

		return bRet;
	}
	else
	{
		// �ȶ�ȡ�ļ����ݵ���������Ȼ���ٿ�����Ŀ�껺��
		DWORD dwRealReadByte = 0;
		BOOL bRet = ::ReadFile(m_hFile, m_pBufPool, MAX_BUFFER_POOL_SIZE, &dwRealReadByte, lpOverlapped);
		m_pPoolCursor = m_pBufPool;
		m_pPoolRoof = m_pBufPool + dwRealReadByte;

		DWORD dwRealCopyNum = dwRealReadByte > dwNumberOfBytesToRead ? dwNumberOfBytesToRead : dwRealReadByte;
		TP_MemcpyLine(pDestBuf, m_pPoolCursor, dwRealCopyNum);
		m_pPoolCursor += dwRealCopyNum;
		m_llFileCursor.QuadPart += dwRealCopyNum;

		if(lpNumberOfBytesRead != NULL)
		{
			*lpNumberOfBytesRead += dwRealCopyNum;
		}

		return bRet;
	}
}

BOOL CSGBufferFile::SGWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	DWORD dwWriteByte = (m_pPoolRoof - m_pBufPool) > (m_pPoolCursor - m_pBufPool) ? (m_pPoolRoof - m_pBufPool) : (m_pPoolCursor - m_pBufPool);
	DWORD dwWritableSpace = MAX_BUFFER_POOL_SIZE - dwWriteByte; // ����ؿ���д���ֽ���Ŀ

	if(dwWritableSpace <= dwNumberOfBytesToWrite)
	{
		// �Ƚ������д���ļ���Ȼ��ֱ�ӽ�������д���ļ�
		DWORD dwWillWriteByte = dwWriteByte;
		if(dwWillWriteByte > 0)
		{
			DWORD dwRealWriteByte = 0;
			::WriteFile(m_hFile, m_pBufPool, dwWillWriteByte, &dwRealWriteByte, lpOverlapped);
			if(m_pPoolRoof > m_pPoolCursor)
			{
				m_llFileCursor.QuadPart += (m_pPoolRoof - m_pPoolCursor);
			}
			m_pPoolRoof = m_pBufPool;
			m_pPoolCursor = m_pBufPool;
		}

		DWORD dwWriteFileByte = 0;
		BOOL bRet = ::WriteFile(m_hFile, lpBuffer, dwNumberOfBytesToWrite, &dwWriteFileByte, lpOverlapped);
		m_llFileCursor.QuadPart += dwWriteFileByte;
		m_llFileSize.QuadPart = m_llFileCursor.QuadPart;

		if(lpNumberOfBytesWritten != NULL)
		{
			*lpNumberOfBytesWritten = dwWriteFileByte;
		}

		return bRet;
	}
	else
	{
		// ֱ�ӽ�����д�뻺��������
		TP_MemcpyLine(m_pPoolCursor, (void *)lpBuffer, dwNumberOfBytesToWrite);
		m_llFileCursor.QuadPart += dwNumberOfBytesToWrite;
		if(m_llFileSize.QuadPart < m_llFileCursor.QuadPart)
		{
			m_llFileSize.QuadPart = m_llFileCursor.QuadPart;
			m_pPoolRoof += dwNumberOfBytesToWrite;
		}

		m_pPoolCursor += dwNumberOfBytesToWrite;

		if(m_pPoolRoof >= m_pBufPool + MAX_BUFFER_POOL_SIZE)
		{
			// ������Ѿ�д����������д���ļ�
			DWORD dwRealWriteByte = 0;
			::WriteFile(m_hFile, m_pBufPool, MAX_BUFFER_POOL_SIZE, &dwRealWriteByte, lpOverlapped);
			m_pPoolCursor = m_pBufPool;
			m_pPoolRoof = m_pBufPool;
		}

		if(lpNumberOfBytesWritten != NULL)
		{
			*lpNumberOfBytesWritten = dwNumberOfBytesToWrite;
		}

		return TRUE;
	}
}

BOOL CSGBufferFile::SGGetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize)
{
	if(lpFileSize != NULL)
	{
		lpFileSize->QuadPart = m_llFileSize.QuadPart;
	}

	return TRUE;
}

BOOL CSGBufferFile::SGSetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod)
{

	if(lpNewFilePointer != NULL)
	{
		lpNewFilePointer->QuadPart = 0;
	}

	LARGE_INTEGER llNewFilePos;

	switch(dwMoveMethod)
	{
	case FILE_BEGIN:
		llNewFilePos.QuadPart = liDistanceToMove.QuadPart;
		break;
	case FILE_END:
		llNewFilePos.QuadPart = m_llFileSize.QuadPart + liDistanceToMove.QuadPart;
		break;
	case FILE_CURRENT:
		llNewFilePos.QuadPart = m_llFileCursor.QuadPart + liDistanceToMove.QuadPart;
		break;
	}

	if(llNewFilePos.QuadPart < 0)
	{
		return FALSE;
	}

	// �ж����ļ�ָ���Ƿ��ڻ���ط�Χ��
	LARGE_INTEGER llBuffPoolPosBegin;
	llBuffPoolPosBegin.QuadPart = m_llFileCursor.QuadPart - (m_pPoolCursor - m_pBufPool);
	LARGE_INTEGER llBufPoolPosEnd;
	llBufPoolPosEnd.QuadPart = m_llFileCursor.QuadPart + (m_pPoolRoof - m_pBufPool) - (m_pPoolCursor - m_pBufPool);

	if(llNewFilePos.QuadPart >= llBuffPoolPosBegin.QuadPart && llNewFilePos.QuadPart <= llBufPoolPosEnd.QuadPart)
	{
		// �ڻ���ط�Χ�ڣ�ֱ�Ӽ��ƶ�ָ�뼴��
		m_pPoolCursor += (llNewFilePos.QuadPart - m_llFileCursor.QuadPart);
		m_llFileCursor.QuadPart = llNewFilePos.QuadPart;
	}
	else
	{
		if(m_bWriteFile)
		{
			// д�ļ�����Ҫ�����������д���ļ�
			DWORD dwWriteByte = (m_pPoolRoof - m_pBufPool) > (m_pPoolCursor - m_pBufPool) ? (m_pPoolRoof - m_pBufPool) : (m_pPoolCursor - m_pBufPool);
			if(dwWriteByte)
			{
				DWORD dwRealWriteFileByte = 0;
				::WriteFile(m_hFile, m_pBufPool, dwWriteByte, &dwRealWriteFileByte, NULL);
				//::FlushFileBuffers(m_hFile);
			}
		}
		else
		{
			// ���ļ�����ǰ��ȡ����������
		}

		::SetFilePointerEx(m_hFile, llNewFilePos, &m_llFileCursor, FILE_BEGIN);
		if(m_llFileSize.QuadPart < llNewFilePos.QuadPart)
		{
			m_llFileSize.QuadPart = llNewFilePos.QuadPart;
			::SetEndOfFile(m_hFile);
		}
		m_pPoolCursor = m_pBufPool;
		m_pPoolRoof = m_pBufPool;
	}

	if(lpNewFilePointer != NULL)
	{
		lpNewFilePointer->QuadPart = llNewFilePos.QuadPart;
	}

	return TRUE;
}

BOOL CSGBufferFile::SGCloseHandle(HANDLE hObject)
{
	if(m_bWriteFile)
	{
		// �������������д���ļ�
		DWORD dwWriteByte = (m_pPoolRoof - m_pBufPool) > (m_pPoolCursor - m_pBufPool) ? (m_pPoolRoof - m_pBufPool) : (m_pPoolCursor - m_pBufPool);

		if(dwWriteByte)
		{
			::GetFileSizeEx(m_hFile, &m_llFileSize);
			DWORD dwRealWriteByte = 0;
			::WriteFile(m_hFile, m_pBufPool, dwWriteByte, &dwRealWriteByte, NULL);
			m_pPoolCursor = m_pBufPool;
			m_pPoolRoof = m_pBufPool;
		}
	}

	_Clear();

	return TRUE;
}

CSGBufferFile::~CSGBufferFile(void)
{
	_Clear();
}
// �µ�ʹ�û����д�ļ��� end

DWORD g_SGMaxImageSize = 5000 * 4000; // 

CCriticalSection  g_CritSection;
HMODULE g_hFileSetModule=NULL;

void  GetALLScrTable( short xsize, short ysize, short scrx, short scry,	short *xtab, short *ytab )
{
	short i;
	double para, total;
	para = (double)xsize/(double)scrx;
	total=0;
	for(i=0;i<scrx;i++)
	{
		xtab[i]= (short) total;
		total+= para;
	}
	para = (double)ysize/(double)scry;
	total=0;
	for(i=0;i<scry;i++)
	{
		ytab[i]= (short) total;
		total+= para;
	}
}


void ConvertDataBlock( HANDLE hSourceFile, void *pp, BYTE *buf, void *platte, int sxsize,int sysize)
{
	short *xtable, *ytable;
	int   *plat32, *destbuf32;
	int chpt;

	int iReadSizeX=((sxsize+3)/4)*4;

	xtable = new short[sxsize];
	ytable = new short[sysize];
	DWORD k;

	TPImageProtectMode			protflag;
	protflag = READ_Pattern;
	GetALLScrTable( sxsize, sysize, sxsize, sysize, xtable, ytable );	
	plat32=(int *)platte;
	destbuf32=(int *)pp;
	int tt = 0;
	for(int i=0;i<sysize; i++)
	{
		chpt=ytable[i]*(sxsize);

		for(int j=0; j<sxsize; j++)
		{
			k=(DWORD)(buf[i*iReadSizeX+j]&0xff);
			destbuf32[j]  = ReadImageColorExchang32( plat32[k], destbuf32[j], protflag );
		}
		destbuf32+=sxsize; 
		tt += sxsize;
	}
	destbuf32 -= tt;

	delete []xtable;
	delete []ytable;

}

int GetBMP_8Data(HANDLE hSourceFile, BYTE *buf, void *plat,int iReadSize,int iDataOffset)	 
{
	int offset = iDataOffset -sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);//��ɫ���С
	BYTE * pTem = new BYTE[offset];
	TP_MemsetChar(pTem,0,offset);
	DWORD dwBytesRealRead = 0;
	ReadFile(hSourceFile, pTem, offset, &dwBytesRealRead, NULL);
	//fread(pTem,offset,1,(FILE*) hSourceFile);
	int k = offset/4 + 4;
	if(k >256)
		k = 256;
	//���ɵ�ɫ��
	for( int i =0; i< k ; i++)
		((int *)plat)[i]=pTem[i*4]+(pTem[i*4+1]<<8)+(pTem[i*4+2]<<16)+(pTem[i*4+3]<<24);

	ReadFile(hSourceFile, buf, iReadSize, &dwBytesRealRead, NULL);
	//fread(buf,1,iReadSize ,(FILE*)hSourceFile);

	delete []pTem;

	return 0;
}


CTPImgDecode::CTPImgDecode()
{
	m_pImgBase = NULL;
	m_lplpResource = NULL;	
	m_pSGImageDescriptor = NULL;	
}
CTPImgDecode::~CTPImgDecode()
{
	if(m_pImgBase)
	{
		m_pImgBase->Close(m_pSGImageDescriptor);
		delete m_pImgBase;
		m_pImgBase = NULL;
	}
	if(m_pSGImageDescriptor->bReadFile && NULL != m_pSGImageDescriptor->pImage)
		delete []m_pSGImageDescriptor->pImage;

	if(m_pSGImageDescriptor)
	{
		delete m_pSGImageDescriptor;
		m_pSGImageDescriptor = NULL;
	}
	if(m_lplpResource)
	{
		delete m_lplpResource;
		m_lplpResource = NULL;
	}
}
// ͨ����ȡ�ļ���Ϣ�ж��ļ�����
IMG_TYPE_ENUM CTPImgDecode::DistinguishImageFormat(LPTPResource lpResource)
{
	if(NULL == lpResource)
	{
		return IMG_UNKNOW;
	}

	HANDLE hImageHandle = CreateFile(lpResource->ptszfileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hImageHandle)
	{
		return IMG_UNKNOW;
	}
	BYTE buf[26] = {0};
	DWORD dwRealReadBytes = 0;
	if(ReadFile(hImageHandle, buf, 8, &dwRealReadBytes, NULL) == FALSE)
	{
		CloseHandle(hImageHandle);
		return IMG_UNKNOW;
	}
	BYTE strBmp[] = {0x42, 0x4d};
	BYTE strPcx[] = {0x0a};
	BYTE strJpg[] = {0xff, 0xd8};
	BYTE strTifIntel[] = {0x49, 0x49, 0x2a, 0x00};
	BYTE strTifMoto[] = {0x4d, 0x4d, 0x00, 0x2a};
	BYTE strGif89[] = {'G', 'I', 'F', '8', '9', 'a'};
	BYTE strGif87[] = {'G', 'I', 'F', '8', '7', 'a'};
	BYTE strPng[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
	BYTE strTga[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x54, 0x52, 0x55, 0x45, 0x56, 0x49, 0x53, 0x49, 
		0x4f, 0x4e, 0x2d, 0x58, 0x46, 0x49, 0x4c, 0x45, 
		0x2e, 0x00	};
	BYTE strTgaOld[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x2e, 0x00	};
	BYTE strTgaOldZero[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00	};

	IMG_TYPE_ENUM imgFileType = IMG_UNKNOW;
	if(memcmp(buf, strPcx, 1) == 0) // PCX
		imgFileType = IMG_PCX;
	else if(memcmp(buf, strBmp, 2) == 0) // BMP
		imgFileType = IMG_BMP;
	else if(memcmp(buf, strJpg, 2) == 0) // JPG
		imgFileType = IMG_JPG;
	else if(memcmp(buf, strTifIntel, 4) == 0 || memcmp(buf, strTifMoto, 4) == 0) // TIF
		imgFileType = IMG_TIF;
	else if(memcmp(buf, strGif89, 6) == 0 || memcmp(buf, strGif87, 6) == 0) // GIF
		imgFileType = IMG_GIF;
	else if(memcmp(buf, strPng, 8) == 0) // PNG
		imgFileType = IMG_PNG;
	else
	{
		// �����ж� TGA
		// SG: ���� TGA �ౣ���ͼ��ĩβ 26 �ֽڲ�ȷ�������Ӱ��� IMAGE_TYPE ���ж�
		if(buf[2] == 0x00 || buf[2] == 0x01 || buf[2] == 0x02 || buf[2] == 0x03 || buf[2] == 0x09 || buf[2] == 0x0a || buf[2] == 0x0b)
			imgFileType = IMG_TGA;
		else
		{
			long lHigh = 0;
			SetFilePointer(hImageHandle, -26, &lHigh, FILE_END);
			DWORD dwRealReadBytes = 0;
			if(ReadFile(hImageHandle, buf, 26, &dwRealReadBytes, NULL) == FALSE)
			{
				CloseHandle(hImageHandle);
				return IMG_UNKNOW;
			}
			if(memcmp(buf, strTga, 26) == 0 || memcmp(buf, strTgaOld, 26) == 0 || memcmp(buf, strTgaOldZero, 26) == 0) // TGA
				imgFileType = IMG_TGA;
		}
	}
	CloseHandle(hImageHandle);
	return imgFileType;
}
CTPImgBase::CTPImgBase(){}
CTPImgBase::~CTPImgBase(){}
// SG: �õ�ͼ��� �� & ��
CTPImgJPG::CTPImgJPG(){}
CTPImgJPG::~CTPImgJPG(){}
int CTPImgJPG::Open(LPTPResource lpResource)
{ 
	if(NULL == lpResource)
	{
		return FIO_OpenErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	HANDLE hJgpFile = CreateFile(lpResource->ptszfileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hJgpFile)
	{
		return FIO_OpenErr;
	}

	CSingleLock singleLock(&g_CritSection);
	singleLock.Lock();
	JpegFile::GetHeadSize(hJgpFile, (UINT *)&pSGImageDescriptor->iWidth, (UINT *)&pSGImageDescriptor->iHeight);
	if(singleLock.IsLocked())
		singleLock.Unlock();
	pSGImageDescriptor->pPrivateData = hJgpFile; // SG: ���� JPG ������ǵùر�
	return FIO_Succeed;
}

int CTPImgJPG::Close(SGImageDescriptor *pSGImageDescriptor)
{
	if(NULL != pSGImageDescriptor && NULL != pSGImageDescriptor->pPrivateData) // SG: ���洢�����дû��ʹ��
	{
		CloseHandle(pSGImageDescriptor->pPrivateData);
		pSGImageDescriptor->pPrivateData = NULL;
	}
	return FIO_Succeed; 
}

int CTPImgJPG::GetImage(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OpenErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	CSingleLock singleLock(&g_CritSection);
	singleLock.Lock();

	UINT uWidth =0;
	UINT uHeight =0;
	if(NULL != pSGImageDescriptor->pPrivateData){
		pSGImageDescriptor->pImage = JpegFile::JpegFileToBGRA(pSGImageDescriptor->pPrivateData,&uWidth,&uHeight); // SG: ֱ�ӵõ� BGRA ��ʽͼ��buf ��Ҫ�������ͷ�
		if(!pSGImageDescriptor->pImage) 
		{
			if(singleLock.IsLocked())
				singleLock.Unlock();
			return FIO_OtherErr;
		}
	}
	else{
		if(singleLock.IsLocked())
			singleLock.Unlock();
		return FIO_OtherErr;
	}
	return FIO_Succeed;
}
CTPImgBMP::CTPImgBMP(){}
CTPImgBMP::~CTPImgBMP(){}
int CTPImgBMP::Open(LPTPResource lpResource)
{
	if(NULL == lpResource)
		return FIO_OtherErr;

	SGBMPInfo *pSGBMPInfo = new SGBMPInfo;
	size_t iReadNum=0;

	pSGBMPInfo->hBmpFile = CreateFile(lpResource->ptszfileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == pSGBMPInfo->hBmpFile)
	{
		delete pSGBMPInfo;
		return FIO_OpenErr;
	}
	DWORD dwBytesRealRead = 0;
	if(ReadFile(pSGBMPInfo->hBmpFile, &pSGBMPInfo->fileHeader, sizeof(BITMAPFILEHEADER), &dwBytesRealRead, NULL) == FALSE)
	{
		CloseHandle(pSGBMPInfo->hBmpFile);
		delete pSGBMPInfo;
		return FIO_ErrIo;
	}
	if(dwBytesRealRead != sizeof(BITMAPFILEHEADER))
	{
		CloseHandle(pSGBMPInfo->hBmpFile);
		delete pSGBMPInfo;
		return FIO_ErrIo;
	}

	if(ReadFile(pSGBMPInfo->hBmpFile, &pSGBMPInfo->infoHeader, sizeof(BITMAPINFOHEADER), &dwBytesRealRead, NULL) == FALSE)
	{
		CloseHandle(pSGBMPInfo->hBmpFile);
		delete pSGBMPInfo;
		return FIO_ErrIo;
	}
	if(dwBytesRealRead != sizeof(BITMAPINFOHEADER))
	{
		CloseHandle(pSGBMPInfo->hBmpFile);
		delete pSGBMPInfo;
		return FIO_ErrIo;
	}

	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	pSGImageDescriptor->iWidth = pSGBMPInfo->infoHeader.biWidth;
	pSGImageDescriptor->iHeight = pSGBMPInfo->infoHeader.biHeight;
	pSGImageDescriptor->pPrivateData = pSGBMPInfo; // GetImage ʱʹ��

	// 32λ BMP �������� alpha ��Ϣ add by wgf@2010-5-31
	if(pSGBMPInfo->infoHeader.biBitCount == 32)
	{
		pSGImageDescriptor->bHaveAlpha = true;
	}
	else
	{
		pSGImageDescriptor->bHaveAlpha = false;
	}

	return FIO_Succeed;
}
int CTPImgBMP::Close(SGImageDescriptor *pSGImageDescriptor)
{
	if(NULL == pSGImageDescriptor)
	{
		return FIO_Succeed;
	}
	if(NULL != pSGImageDescriptor->pPrivateData)
	{
		SGBMPInfo *pSGBMPInfo = (SGBMPInfo *)pSGImageDescriptor->pPrivateData;
		if(NULL != pSGBMPInfo)
		{
			if(NULL != pSGBMPInfo->hBmpFile)
			{
				CloseHandle(pSGBMPInfo->hBmpFile);
				pSGBMPInfo->hBmpFile = NULL;
			}
			delete pSGBMPInfo;
		}
		pSGImageDescriptor->pPrivateData = NULL;
	}
	return FIO_Succeed;
}
int CTPImgBMP::GetImage(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	SGBMPInfo *pSGBMPInfo = (SGBMPInfo *)pSGImageDescriptor->pPrivateData;
	if(NULL == pSGBMPInfo)
	{
		return FIO_OtherErr;
	}
	pSGImageDescriptor->pImage = new BYTE[pSGBMPInfo->infoHeader.biWidth * pSGBMPInfo->infoHeader.biHeight * 4]; // SG: �洢 BGRA ͼ��
	TP_MemsetChar(pSGImageDescriptor->pImage, (char)0xff, pSGBMPInfo->infoHeader.biWidth * pSGBMPInfo->infoHeader.biHeight * 4);
	if (pSGBMPInfo->infoHeader.biBitCount == 24)
	{
		int iNum = (pSGBMPInfo->infoHeader.biWidth*3 ) %4;//ÿ�е�*�ֽ���*��4�ı�����������������
		iNum=((iNum==0)?4:iNum);
		int iRealWidthByte=pSGBMPInfo->infoHeader.biWidth*3+4-iNum;
		BYTE *tmp_buf = NULL;
		tmp_buf = new BYTE[iRealWidthByte * pSGBMPInfo->infoHeader.biHeight ];
		TP_MemsetChar(tmp_buf,0,iRealWidthByte * pSGBMPInfo->infoHeader.biHeight );

		DWORD dwBytesRealRead = 0;
		ReadFile(pSGBMPInfo->hBmpFile, tmp_buf, iRealWidthByte * pSGBMPInfo->infoHeader.biHeight, &dwBytesRealRead, NULL);
		TPCS_24_to_32_Frame(pSGImageDescriptor->pImage,pSGBMPInfo->infoHeader.biWidth * 4,
			tmp_buf,iRealWidthByte,NULL,pSGBMPInfo->infoHeader.biWidth,pSGBMPInfo->infoHeader.biWidth,
			pSGBMPInfo->infoHeader.biHeight,TRUE,FALSE);
		delete []tmp_buf;
	}
	else if(pSGBMPInfo->infoHeader.biBitCount == 8)
	{  
		int size=(((pSGBMPInfo->infoHeader.biWidth+3)/4)*4)*(pSGBMPInfo->infoHeader.biHeight);//���ٻ�������ÿ���ֽ�����4�ı���
		int dataoffset=pSGBMPInfo->fileHeader.bfOffBits;
		BYTE * tembuf = new BYTE [size];
		TP_MemsetChar(tembuf,0,size);
		UINT   platte32[256];
		void * plat = platte32;		
		GetBMP_8Data(pSGBMPInfo->hBmpFile, tembuf,(void*)plat,size,dataoffset);

		ConvertDataBlock(pSGBMPInfo->hBmpFile, pSGImageDescriptor->pImage,tembuf,platte32,pSGBMPInfo->infoHeader.biWidth,pSGBMPInfo->infoHeader.biHeight);
		delete []tembuf; 
		TP_OverTurnSelf (pSGImageDescriptor->pImage, pSGBMPInfo->infoHeader.biWidth * 4, pSGBMPInfo->infoHeader.biHeight);
	}
	else if(pSGBMPInfo->infoHeader.biBitCount == 4) // 16 ɫ
	{
		// ��ȡ��ɫ������
		DWORD dwPaletteData[16] = {0};
		DWORD dwBytesRealRead = 0;
		ReadFile(pSGBMPInfo->hBmpFile, dwPaletteData, 64, &dwBytesRealRead, NULL);
		// ��ȡͼ������
		int iBytesPerRow = (pSGBMPInfo->infoHeader.biWidth * 4 + 31) / 32 * 4; // BMP ͼ��һ���ֽ���Ŀ��Ϊ 4 �ı���
		int iImageBytes = iBytesPerRow * pSGBMPInfo->infoHeader.biHeight;
		BYTE *pImage = new BYTE[iImageBytes];
		TP_MemsetChar(pImage, 0, iImageBytes);
		ReadFile(pSGBMPInfo->hBmpFile, pImage, iImageBytes, &dwBytesRealRead, NULL);
		if(dwBytesRealRead != iImageBytes)
		{
			ASSERT(0);
			iImageBytes = dwBytesRealRead;
		}

		int iPixelNum = pSGBMPInfo->infoHeader.biWidth / 2;
		BYTE *pBytePalette = (BYTE *)dwPaletteData;
		BYTE *pDestTrv = pSGImageDescriptor->pImage;
		for(int iRow = 0; iRow < pSGBMPInfo->infoHeader.biHeight; ++iRow)
		{
			BYTE *pImageRow = pImage + iRow * iBytesPerRow;
			for(int i = 0; i < iPixelNum; ++i)
			{
				BYTE byteMask = 0xf0;
				for(int k = 0; k < 2; ++k)
				{
					BYTE bytePaletteIndex = byteMask & pImageRow[i];
					if(byteMask == 0xf0)
					{
						bytePaletteIndex >>= 4;
					}
					pDestTrv[0] = pBytePalette[(UINT)bytePaletteIndex * 4];
					pDestTrv[1] = pBytePalette[(UINT)bytePaletteIndex * 4 + 1];
					pDestTrv[2] = pBytePalette[(UINT)bytePaletteIndex * 4 + 2];
					pDestTrv += 4;
					byteMask = 0x0f;
				}
			}
			// ���������е����һ��ͼ������
			if(pSGImageDescriptor->iWidth % 2 == 1)
			{
				BYTE bytePaletteIndex = 0xf0 & pImageRow[pSGImageDescriptor->iWidth / 2 + 1];
				bytePaletteIndex >>= 4;
				pDestTrv[0] = pBytePalette[(UINT)bytePaletteIndex * 4];
				pDestTrv[1] = pBytePalette[(UINT)bytePaletteIndex * 4 + 1];
				pDestTrv[2] = pBytePalette[(UINT)bytePaletteIndex * 4 + 2];
				pDestTrv += 4;
			}
		}
		delete []pImage;
		TP_OverTurnSelf (pSGImageDescriptor->pImage, pSGBMPInfo->infoHeader.biWidth * 4, pSGBMPInfo->infoHeader.biHeight);
	}
	else if(pSGBMPInfo->infoHeader.biBitCount == 32)
	{
		DWORD dwBytesRealRead = 0;
		ReadFile(pSGBMPInfo->hBmpFile, pSGImageDescriptor->pImage, pSGBMPInfo->infoHeader.biWidth * pSGBMPInfo->infoHeader.biHeight * 4, 
			&dwBytesRealRead, NULL);
		TP_OverTurnSelf (pSGImageDescriptor->pImage, pSGBMPInfo->infoHeader.biWidth * 4, pSGBMPInfo->infoHeader.biHeight);
	}
	else if(pSGBMPInfo->infoHeader.biBitCount == 1){
		int nPaletteArray[2];
		DWORD dwBytesRealRead = 0;
		ReadFile(pSGBMPInfo->hBmpFile, nPaletteArray, 8, &dwBytesRealRead, NULL);

		int nBlack = nPaletteArray[0] ? 0 : 255;
		int nWhite = nBlack ? 0 : 255;
		int nRowByteNum=(pSGBMPInfo->infoHeader.biWidth * pSGBMPInfo->infoHeader.biBitCount+31)/32*4;
		char *pMid = new char[nRowByteNum];
		BYTE *pRow =pSGImageDescriptor->pImage + (pSGBMPInfo->infoHeader.biWidth * 4 * (pSGBMPInfo->infoHeader.biHeight - 1));

		for(int nRowNum=0; nRowNum < pSGBMPInfo->infoHeader.biHeight; ++nRowNum) // SG: ��ɫλͼ
		{
			ReadFile(pSGBMPInfo->hBmpFile, pMid, nRowByteNum, &dwBytesRealRead, NULL);
			if(dwBytesRealRead != nRowByteNum)
			{
				delete []pMid;
				return FIO_OtherErr;
			}

			int nNum=pSGBMPInfo->infoHeader.biWidth / 8;
			int i=0;
			while(i < nNum)
			{
				for(int j=0; j < 8; ++j){
					*pRow++ = (pMid[i] & 0x80) ? nBlack : nWhite;
					*pRow++ = (pMid[i] & 0x80) ? nBlack : nWhite;
					*pRow++ = (pMid[i] & 0x80) ? nBlack : nWhite;
					*pRow++ = 0xff;
					pMid[i] <<= 1;
				}
				++i;
			}
			// process the tail
			nNum=pSGBMPInfo->infoHeader.biWidth % 8;
			for(int j=0; j < nNum; ++j)
			{
				*pRow++ = (pMid[i] & 0x80) ? nBlack : nWhite;
				*pRow++ = (pMid[i] & 0x80) ? nBlack : nWhite;
				*pRow++ = (pMid[i] & 0x80) ? nBlack : nWhite;
				*pRow++ = 0xff;
				pMid[i] <<= 1;
			}
			pRow -= (pSGBMPInfo->infoHeader.biWidth * 8);
		}
		delete []pMid;
	}
	return FIO_Succeed;
}
CTPImgPNG::CTPImgPNG(){}
CTPImgPNG::~CTPImgPNG(){}
// lpResource �д洢������˳��Ϊ BGRA ���������ֱ��ʹ��
void NXReadPngCell(png_structp png_ptr, png_bytep pReadBuf, png_size_t uiBufSize)
{
	if(NULL == png_ptr || NULL == pReadBuf)
	{
		return ;
	}
	CSGBufferFile *pCNXBufferedFile = (CSGBufferFile *)png_ptr->io_ptr;
	if(NULL == pCNXBufferedFile)
		return ;
	DWORD dwRealReadBytes = 0;
	pCNXBufferedFile->SGReadFile(pCNXBufferedFile->m_hFile, pReadBuf, uiBufSize, &dwRealReadBytes, NULL);
}
PngFileType * readPng(LPTPResource lpResource)
{
	if(NULL == lpResource)
	{
		return NULL;
	}
	PngFileType *pPngFileType = new PngFileType;
	if(NULL == pPngFileType)
		return NULL;
	HANDLE hPngFile = pPngFileType->pngCNXBufferedFile.SGCreateFile(lpResource->ptszfileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hPngFile)
	{
		delete pPngFileType;
		return NULL;
	}

	char cLocalBuf[8];
	DWORD dwRealReadBytes = 0;
	pPngFileType->pngCNXBufferedFile.SGReadFile(hPngFile, cLocalBuf, 4, &dwRealReadBytes, NULL);

	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	pPngFileType->pPngStruct = png_create_read_struct(NULL, NULL, NULL);

	if (pPngFileType->pPngStruct == NULL)
	{
		delete pPngFileType;
		return NULL;
	}

	pPngFileType->pPngInfo = png_create_info_struct(pPngFileType->pPngStruct);
	if (pPngFileType->pPngInfo == NULL)
	{
		png_destroy_read_struct(&pPngFileType->pPngStruct, NULL, NULL);
		delete pPngFileType;
		return NULL;
	}

	if (setjmp(png_jmpbuf(pPngFileType->pPngStruct)))
	{
		png_destroy_read_struct(&pPngFileType->pPngStruct, &pPngFileType->pPngInfo, NULL);
	}

	//png_init_io(pPngFileType->pPngStruct, hPngFile);
	png_set_read_fn(pPngFileType->pPngStruct, &pPngFileType->pngCNXBufferedFile, NXReadPngCell);
	png_set_sig_bytes(pPngFileType->pPngStruct, 4); // less than 8

	png_read_info(pPngFileType->pPngStruct, pPngFileType->pPngInfo);
	png_get_IHDR(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);

	png_set_strip_16(pPngFileType->pPngStruct);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(pPngFileType->pPngStruct);

	if (png_get_valid(pPngFileType->pPngStruct, pPngFileType->pPngInfo, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(pPngFileType->pPngStruct);

	int intent;
	double screen_gamma = 2.2;
	if (png_get_sRGB(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &intent))
		png_set_gamma(pPngFileType->pPngStruct, screen_gamma, 0.45455);
	else
	{
		double image_gamma;
		if (png_get_gAMA(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &image_gamma))
			png_set_gamma(pPngFileType->pPngStruct, screen_gamma, image_gamma);
		else
			png_set_gamma(pPngFileType->pPngStruct, screen_gamma, 0.45455);
	}

	if (color_type & PNG_COLOR_MASK_COLOR)
	{
		int num_palette;
		png_colorp palette;

		if (png_get_PLTE(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &palette, &num_palette))
		{
			png_uint_16p histogram = NULL;

			png_get_hIST(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &histogram);

			png_set_dither(pPngFileType->pPngStruct, palette, num_palette,
				0x000000ff, histogram, 0);
		}
	}

	png_set_invert_mono(pPngFileType->pPngStruct);

	if (png_get_valid(pPngFileType->pPngStruct, pPngFileType->pPngInfo, PNG_INFO_sBIT))
	{
		png_color_8p sig_bit;

		png_get_sBIT(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &sig_bit);
		png_set_shift(pPngFileType->pPngStruct, sig_bit);
	}

	if (color_type & PNG_COLOR_MASK_COLOR)
		png_set_bgr(pPngFileType->pPngStruct); // ת��Ϊ BGRA | BGR ת��֮���������ֱ��ʹ��

	png_set_swap(pPngFileType->pPngStruct);

	png_set_filler(pPngFileType->pPngStruct, 0xff, PNG_FILLER_AFTER);

	int number_passes = png_set_interlace_handling(pPngFileType->pPngStruct);

	png_read_update_info(pPngFileType->pPngStruct, pPngFileType->pPngInfo);

	png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);

	for (png_uint_32 row = 0; row < height; row++)
	{
		row_pointers[row] = (png_bytep)png_malloc(pPngFileType->pPngStruct, png_get_rowbytes(pPngFileType->pPngStruct,
			pPngFileType->pPngInfo));
	}

	png_read_image(pPngFileType->pPngStruct, row_pointers);


	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	pSGImageDescriptor->pImage = new BYTE[width * height * 4]; // BGRA
	BYTE *pTrv = pSGImageDescriptor->pImage;
	for(png_uint_32 i = 0; i < height; ++i){
		TP_MemcpyLine(pTrv, row_pointers[i], width * 4);
		pTrv += (width * 4);
		free(row_pointers[i]);
	}
	free(row_pointers);
	return pPngFileType; // ����ڴ���Ҫ�������ͷ�
}
int CTPImgPNG::Open(LPTPResource lpResource)
{
	//if(NULL == lpResource)
	//{
	//	return FIO_OtherErr;
	//}
	//SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	//PngFileType * pPngFileType = readPng(lpResource); // SG: �ǵ��ͷ� pPngFileType

	//pSGImageDescriptor->iWidth = pPngFileType->pPngInfo->width;
	//pSGImageDescriptor->iHeight = pPngFileType->pPngInfo->height;
	//pSGImageDescriptor->pPrivateData = pPngFileType;
	//return FIO_Succeed;
	if(NULL == lpResource)
	{
		return FIO_OtherErr;
	}
	// ��ȡ PNG �ļ���Ϣ����Ҫ�ǵõ����
	PngFileType *pPngFileType = new PngFileType;
	if(NULL == pPngFileType)
		return NULL;
	HANDLE hPngFile = pPngFileType->pngCNXBufferedFile.SGCreateFile(lpResource->ptszfileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(NULL == hPngFile)
	{
		delete pPngFileType;
		return NULL;
	}

	char cLocalBuf[8];
	DWORD dwRealReadBytes = 0;
	pPngFileType->pngCNXBufferedFile.SGReadFile(hPngFile, cLocalBuf, 4, &dwRealReadBytes, NULL);

	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	pPngFileType->pPngStruct = png_create_read_struct(NULL, NULL, NULL);

	if (pPngFileType->pPngStruct == NULL)
	{
		delete pPngFileType;
		return NULL;
	}

	pPngFileType->pPngInfo = png_create_info_struct(pPngFileType->pPngStruct);
	if (pPngFileType->pPngInfo == NULL)
	{
		png_destroy_read_struct(&pPngFileType->pPngStruct, NULL, NULL);
		delete pPngFileType;
		return NULL;
	}

	if (setjmp(png_jmpbuf(pPngFileType->pPngStruct)))
	{
		png_destroy_read_struct(&pPngFileType->pPngStruct, &pPngFileType->pPngInfo, NULL);
	}

	//png_init_io(pPngFileType->pPngStruct, hPngFile);
	png_set_read_fn(pPngFileType->pPngStruct, &pPngFileType->pngCNXBufferedFile, NXReadPngCell);
	png_set_sig_bytes(pPngFileType->pPngStruct, 4); // less than 8

	png_read_info(pPngFileType->pPngStruct, pPngFileType->pPngInfo);
	png_get_IHDR(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);

	png_set_strip_16(pPngFileType->pPngStruct);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(pPngFileType->pPngStruct);

	if (png_get_valid(pPngFileType->pPngStruct, pPngFileType->pPngInfo, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(pPngFileType->pPngStruct);

	int intent;
	double screen_gamma = 2.2;
	if (png_get_sRGB(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &intent))
		png_set_gamma(pPngFileType->pPngStruct, screen_gamma, 0.45455);
	else
	{
		double image_gamma;
		if (png_get_gAMA(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &image_gamma))
			png_set_gamma(pPngFileType->pPngStruct, screen_gamma, image_gamma);
		else
			png_set_gamma(pPngFileType->pPngStruct, screen_gamma, 0.45455);
	}

	if (color_type & PNG_COLOR_MASK_COLOR)
	{
		int num_palette;
		png_colorp palette;

		if (png_get_PLTE(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &palette, &num_palette))
		{
			png_uint_16p histogram = NULL;

			png_get_hIST(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &histogram);

			png_set_dither(pPngFileType->pPngStruct, palette, num_palette,
				0x000000ff, histogram, 0);
		}
	}

	png_set_invert_mono(pPngFileType->pPngStruct);

	if (png_get_valid(pPngFileType->pPngStruct, pPngFileType->pPngInfo, PNG_INFO_sBIT))
	{
		png_color_8p sig_bit;

		png_get_sBIT(pPngFileType->pPngStruct, pPngFileType->pPngInfo, &sig_bit);
		png_set_shift(pPngFileType->pPngStruct, sig_bit);
	}

	if (color_type & PNG_COLOR_MASK_COLOR)
		png_set_bgr(pPngFileType->pPngStruct); // ת��Ϊ BGRA | BGR ת��֮���������ֱ��ʹ��

	png_set_swap(pPngFileType->pPngStruct);

	png_set_filler(pPngFileType->pPngStruct, 0xff, PNG_FILLER_AFTER);

	int number_passes = png_set_interlace_handling(pPngFileType->pPngStruct);

	png_read_update_info(pPngFileType->pPngStruct, pPngFileType->pPngInfo);

	// �����ߵ�˽������
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	pSGImageDescriptor->pPrivateData = pPngFileType;
	pSGImageDescriptor->iHeight = height;
	pSGImageDescriptor->iWidth = width;

	// ���� alpha ��Ϣ
	if(color_type == PNG_COLOR_TYPE_RGBA || color_type == PNG_COLOR_TYPE_GA)
	{
		// ���� alpha
		pSGImageDescriptor->bHaveAlpha = true;
	}
	else
	{
		pSGImageDescriptor->bHaveAlpha = false;
	}
	// ���� alpha ��Ϣ

	return FIO_Succeed;
}
int CTPImgPNG::Close(SGImageDescriptor *pSGImageDescriptor)
{
	if(NULL == pSGImageDescriptor || NULL == pSGImageDescriptor->pPrivateData) // SG: д�ļ��˺���ֱ�ӷ���
		return FIO_Succeed;
	PngFileType * pPngFileType=(PngFileType*)pSGImageDescriptor->pPrivateData;
	if(NULL != pPngFileType)
	{
		png_destroy_read_struct(&pPngFileType->pPngStruct, &pPngFileType->pPngInfo, NULL);
		delete pPngFileType; // �˽ṹ���ں��� PngOpenFileName ������
	}
	pSGImageDescriptor->pPrivateData = NULL; // pPngFileType �Ѿ��� PngCloseFile() �� delete ��

	return FIO_Succeed;

}
int CTPImgPNG::GetImage(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	PngFileType *pPngFileType = (PngFileType *)pSGImageDescriptor->pPrivateData;

	png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * pSGImageDescriptor->iHeight);

	pSGImageDescriptor->pImage = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 4];
	TP_MemsetChar(pSGImageDescriptor->pImage, (char)0xff, pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 4);
	for (png_uint_32 row = 0; row < (png_uint_32)pSGImageDescriptor->iHeight; row++)
	{
		row_pointers[row] = (png_bytep)png_malloc(pPngFileType->pPngStruct, png_get_rowbytes(pPngFileType->pPngStruct,
			pPngFileType->pPngInfo));
	}
	png_read_image(pPngFileType->pPngStruct, row_pointers);

	BYTE *pTrv = pSGImageDescriptor->pImage;
	for(int iBeginRow = 0; iBeginRow < pSGImageDescriptor->iHeight; ++iBeginRow)
	{
		TP_MemcpyLine(pTrv, row_pointers[iBeginRow], pSGImageDescriptor->iWidth * 4);
		pTrv += (pSGImageDescriptor->iWidth * 4);
		free(row_pointers[iBeginRow]);
	}
	free(row_pointers);
	return FIO_Succeed;
}
CTPImgTIF::CTPImgTIF(){}
CTPImgTIF::~CTPImgTIF(){}
int CTPImgTIF::Open(LPTPResource lpResource){ASSERT(0);return 0;}
int CTPImgTIF::Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
int CTPImgTIF::GetImage(LPTPResource lpResource){return 0;}
CTPImgPCX::CTPImgPCX(){}
CTPImgPCX::~CTPImgPCX(){}
int CTPImgPCX::Open(LPTPResource lpResource){ASSERT(0);return 0;}
int CTPImgPCX::Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
int CTPImgPCX::GetImage(LPTPResource lpResource){return 0;}
CTPImgTGA::CTPImgTGA(){}
CTPImgTGA::~CTPImgTGA(){}
int CTPImgTGA::Open(LPTPResource lpResource)
{
	if(NULL == lpResource)
		return FIO_OtherErr;
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;

	TGADATA* pTgaData=new TGADATA;
	TP_MemsetChar(pTgaData,0,sizeof(TGADATA));
	TPAVTgaFileOpen(pTgaData->hFile,lpResource->ptszfileName,TP_ACCESS_READ);
	int iDepth = 0;

	pTgaData->llFileSize = TPAVTgaFileGetImageInfo(pTgaData->hFile,pTgaData->iX,pTgaData->iY,
		pTgaData->iWidth,pTgaData->iHeight,pTgaData->iDepth,pTgaData->pCustomData,pTgaData->lCustomDataSize);
	if(pTgaData->llFileSize <= 0)
	{
		TPAVTgaFileClose(pTgaData->hFile);
		delete pTgaData;
		return FIO_OpenErr;
	}
	pTgaData->bExistFile = TRUE;

	wcscpy_s(pTgaData->file_name, MAX_PATH, lpResource->ptszfileName);

	pSGImageDescriptor->iWidth = pTgaData->iWidth;
	pSGImageDescriptor->iHeight = pTgaData->iHeight;
	pSGImageDescriptor->pPrivateData = pTgaData; // SG: tgaGetImg() ʹ��
	pSGImageDescriptor->bHaveAlpha = NXImageHaveAlpha(pTgaData->hFile); // �����ͼ���Ƿ��� alpha ��Ϣ

	pSGImageDescriptor->bBlockReadSupported = NXTgaSupportBlockRead(pTgaData->hFile);//ljj@2011-4-28:�Ƿ�֧�ַֿ��ȡ
	return FIO_Succeed;
}
int CTPImgTGA::Close(SGImageDescriptor *pSGImageDescriptor)
{
	TGADATA* pTgaData=(TGADATA*)pSGImageDescriptor->pPrivateData;
	if (NULL != pTgaData)
	{
		if(NULL != pTgaData->hFile)
		{
			TPAVTgaFileClose(pTgaData->hFile);
			pTgaData->hFile = NULL;
		}

		if (pTgaData->bNewCustomBuffer && NULL != pTgaData->pCustomData)
		{
			delete []pTgaData->pCustomData;
			pTgaData->pCustomData = NULL;
		}

		delete pTgaData;
		pTgaData = NULL;
	}
	pSGImageDescriptor->pPrivateData = NULL;
	return FIO_Succeed;
}
int CTPImgTGA::GetImage(LPTPResource lpResource)
{
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	TGADATA* pTgaData=(TGADATA*)pSGImageDescriptor->pPrivateData;
	pSGImageDescriptor->pImage = new BYTE[pSGImageDescriptor->iWidth * pSGImageDescriptor->iHeight * 4];
	if(0 == TPAVTgaFileGetImageData(pTgaData->hFile,pSGImageDescriptor->pImage)){
		delete []pSGImageDescriptor->pImage;
		pSGImageDescriptor->pImage = NULL;
		return FIO_OtherErr;
	}

	return FIO_Succeed;

}
CTPImgGIF::CTPImgGIF(){}
CTPImgGIF::~CTPImgGIF(){}
int CTPImgGIF::Open(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OpenErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;

	SGGIFFileData *pSGGifData = new SGGIFFileData;

	pSGGifData->bEncode = FALSE;

	if(pSGImageDescriptor->bKeepOdd == TRUE)
	{
		// showtime �汾
		pSGGifData->bUseGifGop = TRUE; // �� GetImage ʱ���һ�Ԥ��GOP֡������GOPģʽ
	}

	try
	{

		pSGGifData->pBufferFile = new CSGBufferFile;
		pSGGifData->hGifFile = pSGGifData->pBufferFile->SGCreateFile(lpResource->ptszfileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		pSGGifData-> pGifFile = DSGOpenFileHandle(pSGGifData->pBufferFile, SGBufferReadFile);
	}
	catch (...) 
	{
		if(pSGGifData->pGifFile)
		{
			//CloseHandle(pSGGifData->hGifFile);
			//pSGGifData->hGifFile = NULL;
			DSGCloseFile(pSGGifData->pGifFile);
			delete pSGGifData->pBufferFile;
			pSGGifData->pBufferFile = NULL;
		}
		delete pSGGifData;
		return FIO_OpenErr;
	}

	pSGImageDescriptor->iWidth = pSGGifData->pGifFile->SWidth;
	pSGImageDescriptor->iHeight = pSGGifData->pGifFile->SHeight;
	pSGImageDescriptor->pPrivateData = pSGGifData; // SG: 

	// �õ� GIF ͼ���ж���֡
	LARGE_INTEGER llCurrentFilePointer;
	LARGE_INTEGER llMoveDistance;
	llMoveDistance.QuadPart = 0;
	pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llMoveDistance, &llCurrentFilePointer, FILE_CURRENT); // ���浱ǰ�ļ�ָ��λ��
	pSGGifData->llNXImageBeginPos = llCurrentFilePointer;

	_SGGetGifFrameNum(pSGGifData->pBufferFile, &pSGGifData->usFrameDelayTime, &pSGGifData->iFrameNum);
	// ����ָ��
	pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llCurrentFilePointer, NULL, FILE_BEGIN); // �Ƶ��ն��� LSD λ�ã���Ӱ������� GetImage����
	return FIO_Succeed; // ʹ���µ�̽��֡��Ŀ�㷨

	SGRecordType eRecordType;
	bool bFirstFrame = true; // ��ȡ��һ֡ͼ���ͣ��ʱ�䣬��Ϊ����ͼ��Ĳ����ٶ�

	do {
		if (DSGGetRecordType((pSGGifData->pGifFile), &eRecordType) == GIF_ERROR) {
			return FIO_ErrIo;
		}
		switch (eRecordType) {
			case IMAGE_DESC_RECORD_TYPE: // ͼ��������
				{
					if (DSGGetImageDesc(pSGGifData->pGifFile) == GIF_ERROR) {
						return FIO_ErrIo;
					}
					++pSGGifData->iFrameNum; 
					// ������֡ͼ������
					BYTE *pRow = new BYTE[pSGGifData->pGifFile->Image.Width];
					for(int i = 0; i < pSGGifData->pGifFile->Image.Height; ++i)
					{
						DSGGetLine(pSGGifData->pGifFile, pRow, pSGGifData->pGifFile->Image.Width);
					}
					delete [] pRow;
					break;
				}
			case EXTENSION_RECORD_TYPE:
				{
					int ExtCode;
					SGByteType *Extension;
					if (DSGGetExtension((pSGGifData->pGifFile), &ExtCode, &Extension) == GIF_ERROR) {
						return FIO_ErrIo;
					}
					// Ѱ��ͼ����ƿ��ǩ 0xf9
					if(ExtCode == 0x000000f9)
					{
						// �˿��¼�����ŵ�ͼ����Ϣ�����ӳ٣�͸��ɫ������
						if(bFirstFrame == true)
						{
							pSGGifData->usFrameDelayTime = *(unsigned short *)(&Extension[2]); // ����ͼ���ӳ�ʱ�䣬��λΪ 1/100s
							bFirstFrame = false;
						}
					}
					while (Extension != NULL) {
						if (DSGGetExtensionNext((pSGGifData->pGifFile), &Extension) == GIF_ERROR) {
							return FIO_ErrIo;
						}	
					}
					break;
				}
			case TERMINATE_RECORD_TYPE:
				// ����ָ��
				pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llCurrentFilePointer, NULL, FILE_BEGIN); // �Ƶ��ն��� LSD λ�ã���Ӱ������� GetImage����
				return FIO_Succeed;
			default:		  
				// ����ָ��
				pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llCurrentFilePointer, NULL, FILE_BEGIN); // �Ƶ��ն��� LSD λ�ã���Ӱ������� GetImage����
				return FIO_Succeed;
				break;
		}
	} while (eRecordType != TERMINATE_RECORD_TYPE);
	// ����ָ��
	pSGGifData->pBufferFile->SGSetFilePointerEx(pSGGifData->hGifFile, llCurrentFilePointer, NULL, FILE_BEGIN); // �Ƶ��ն��� LSD λ�ã���Ӱ������� GetImage����
	return FIO_Succeed;
}
int CTPImgGIF::Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
int CTPImgGIF::GetImage(LPTPResource lpResource)
{
	if(NULL == lpResource || NULL == lpResource->pPrivateData)
	{
		return FIO_OtherErr;
	}
	SGImageDescriptor *pSGImageDescriptor = (SGImageDescriptor *)lpResource->pPrivateData;
	SGGIFFileData* pSGGifData=(SGGIFFileData*)pSGImageDescriptor->pPrivateData;
	if(NULL == pSGGifData->pGifFile)
		return FIO_OtherErr;

	int nImageWidth;
	int nImageHeight;
	unsigned short usDelayTime = 0; // ��֡ͼ����Ҫ���ֵ�ʱ�� ��λΪ 1/100s
	int iImageFrame = 0;
	if(iImageFrame < 0)
	{
		iImageFrame = 0;
	}

	// ʹ���µĶ�ȡ֡ͼ���� begin
	pSGImageDescriptor->pImage = SGGetGifImage(pSGGifData, iImageFrame, &nImageWidth, &nImageHeight, &usDelayTime);
	// ʹ���µĶ�ȡ֡ͼ���� end

	pSGGifData->usFrameDelayTime = usDelayTime; // ��֡ͼ��ͣ��ʱ��
	if(NULL == pSGImageDescriptor->pImage)
		return FIO_BadFile;

	return FIO_Succeed;
}
int CTPImgDecode::Open(TCHAR *cFileName, int &iWidth, int &iHeight)
{
	if(NULL == cFileName /*|| NULL == m_lplpResource*/)
	{
		return FIO_OtherErr;
	}
	m_lplpResource = new TPResource;
	if(NULL == m_lplpResource)
		return FIO_OtherErr;
	//TP_MemsetChar(lpResource, 0, sizeof(TPResource));
	memset(m_lplpResource, 0, sizeof(TPResource));
	m_lplpResource->iSize = sizeof(TPResource);
	_tcscpy_s(m_lplpResource->ptszfileName, _MAX_PATH, cFileName);
	m_lplpResource->fp	= NULL;
// 	*m_lplpResource = lpResource; // SG
	m_pSGImageDescriptor = new SGImageDescriptor;
	if(NULL == m_pSGImageDescriptor)
	{
		delete m_lplpResource;
// 		*m_lplpResource = NULL;
		return FIO_OtherErr;
	}
	//TP_MemsetChar(pSGImageDescriptor, 0, sizeof(SGImageDescriptor));
	memset(m_pSGImageDescriptor, 0, sizeof(SGImageDescriptor));
	m_lplpResource->pPrivateData = m_pSGImageDescriptor;
	m_pSGImageDescriptor->eImageType = DistinguishImageFormat(m_lplpResource);
	//pSGImageDescriptor->bKeepOdd = (iAccessMode & TP_ACCESS_KEEP_ODD) ? true : false; // �����Ƿ񱣳�������ͼ�� add by wgf@2010-5-18
	m_pSGImageDescriptor->bKeepOdd = false; // ĿǰE7��NS��ʹ��ż����ͼ�� add by wgf@2010-7-4
	if(IMG_UNKNOW == m_pSGImageDescriptor->eImageType)
	{
// 		delete m_pSGImageDescriptor;
// 		delete m_lplpResource;
// 		*m_lplpResource = NULL;
		return FIO_OtherErr;
	}
	m_pSGImageDescriptor->bReadFile = true; // SG: ����ͼ��������Ҫ�ͷ�

	// SG: ��ȡ��Ϣ����� lpResource �ṹ
	int nRet = FIO_Succeed; 
	switch(m_pSGImageDescriptor->eImageType)
	{
 	case IMG_BMP:
 		m_pImgBase = new CTPImgBMP();
 		//	nRet = bmpOpen(lpResource);
 		break;
 	case IMG_PNG:
 		m_pImgBase = new CTPImgPNG();
 		//	nRet = NXpngOpen(lpResource); // ���õ�ͼ���ߵ���Ϣ��������ȡͼ������
 		break;
	case IMG_JPG:
		m_pImgBase = new CTPImgJPG();
		//nRet = jpgOpen(lpResource);
		break;
 	case IMG_TIF:
 		m_pImgBase = new CTPImgTIF();
 		//	nRet = tifOpen(lpResource);
 		break;
 	case IMG_PCX:
 		m_pImgBase = new CTPImgPCX();
 		//	nRet = pcxOpen(lpResource);
 		break;
 	case IMG_TGA:
 		m_pImgBase = new CTPImgTGA();
 		//	nRet = tgaOpen(lpResource);
 		break;
 	case IMG_GIF:
 		m_pImgBase = new CTPImgGIF();
 		//	nRet = gifOpen(lpResource);
 		break;
	default:
	//	ASSERT(0);
		nRet = FIO_Unsupported;
		break;
	}
	if(nRet != FIO_Succeed)
	{
// 		delete m_lplpResource;
// 		m_lplpResource = NULL;
// 		*m_lplpResource = NULL;
		return nRet;
	}
	m_pImgBase->Open(m_lplpResource);
	if ((DWORD)m_pSGImageDescriptor->iWidth * (DWORD)m_pSGImageDescriptor->iHeight >= g_SGMaxImageSize)  // SG: ͼ�����
	{
		// �ϲ㲻����� imageFileClose(),������ã�����ײ��������Դ add by wgf@2010-9-1
		int iRet = m_pImgBase->Close(m_pSGImageDescriptor);
// 		*m_lplpResource = NULL;
		return FIO_TooLarge;
	}
	iWidth = m_pSGImageDescriptor->iWidth;
	iHeight = m_pSGImageDescriptor->iHeight;
	return nRet;
}
int CTPImgDecode::GetImage(void *pBuffer)
{
	if(m_pImgBase)
	{
		m_pImgBase->GetImage(m_lplpResource);
		memcpy(pBuffer, m_pSGImageDescriptor->pImage, m_pSGImageDescriptor->iWidth * m_pSGImageDescriptor->iHeight * 4);

	}
	return 1;
}
