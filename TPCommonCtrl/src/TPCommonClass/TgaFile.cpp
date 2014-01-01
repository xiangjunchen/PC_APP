#include "stdafx.h"
#include "TgaFile.h"
#include "TPAVTgaFile.h"
///////////////////////////////////////////////////////////
/////////////////////TAG ��////////////////////////////////

// ����TGAͼ���RLC���ݣ���ʱ��Ϊ 400K add by wgf @2010-4-26
#define NX_TGA_RLC_BUF_SIZE 409600

//#define _PPPPP_   1//�Ż�����

extern void tga_cr16_to_cr32_line(OUT unsigned char *pucRGB32,		// 32 λ���
								  IN unsigned char *pucRGB16,		// 16 λ���� arrrrrgg gggbbbbb
								  IN int nLineCount);				// һ�д���ĸ���

extern unsigned char* tga_rlc_line(OUT unsigned char *pucDes,		// ��ѹ���Ŀ��
								   IN unsigned char *pucSrc,		// ѹ������Դ
								   IN int nCellBits,				// ���뵥Ԫλ��
								   IN int nLineCount);				// һ�е�Ԫ����

extern void tga_palette32_index8_line(OUT unsigned char *pucRGB32,		// 32 λ���
							   IN unsigned char *pucIndex8,		// 8 λ��ɫ������
							   IN int nLineCount,				// ��������
							   IN unsigned char *pucPalette);	// 32 λ��ɫ��


extern void tga_palette32_index16_line(OUT unsigned char *pucRGB32,	// 32 λ���
								IN unsigned char *pucIndex16,	// 16 λ��ɫ������
								IN int nLineCount,				// ��������
								IN unsigned char *pucPalette);	// 32 λ��ɫ��

extern void tga_palette32_line(OUT unsigned char *pucRGB32,		// 32 λ���
							   IN unsigned char *pucIndex,			// ��ɫ������
							   IN int nIndexBits,					// ����λ����8/16
							   IN int nLineCount,					// ��������
							   IN unsigned char *pucPalette);		// 32 λ��ɫ��


// ##### Start by wanggaofei @ 2010/04/28 18:41 #####
// Comments: ȥ�� CFile ����Ϊ ReadFile �� API �������ļ����������Ͷ�MFC����������߲����ļ�������ԡ�

// ###### End by wanggaofei @ 2010/04/28 18:41 ######
CTgaFile::CTgaFile()
:m_pImgBuf(NULL)
,m_pClrMap(NULL)
,m_pTgaFileHandle(NULL)
,m_pCustomData(NULL)
,m_lCustomDataSize(0)
,m_dwAccessType(0)
{
	memset(&m_cTGAHead,0,sizeof(TGA_HEAD));
	memset(&m_stImageInfo,0,sizeof(IMAGE_INFO));
	m_llFilePos.QuadPart = 0;
	m_pNXRLCBuf = NULL;
}

/********************************************************/
CTgaFile::~CTgaFile()
{
	CloseTgaFile();
}


BOOL	CTgaFile::OpenTgaFile(LPCTSTR szFileName,DWORD dwOpenType)
{
	if(NULL != m_pTgaFileHandle)
	{
		CloseHandle(m_pTgaFileHandle);
		m_pTgaFileHandle = NULL;
	}
	m_dwAccessType = dwOpenType;
	DWORD dwCreateFlag = GENERIC_READ;
	DWORD dwShareAccess = 0;
	DWORD dwCreateDescription = OPEN_EXISTING; // ���Դ��ļ�
	if (dwOpenType == TP_IMG_FILE_READ)
	{
		dwCreateFlag = GENERIC_READ;
		dwShareAccess = FILE_SHARE_READ;
		dwCreateDescription = OPEN_EXISTING;
	}
	else if (dwOpenType == TP_IMG_FILE_WRITE)
	{
		dwCreateFlag = GENERIC_WRITE;
		dwCreateDescription = CREATE_ALWAYS;
	}
	else if (dwOpenType == (TP_IMG_FILE_READ|TP_IMG_FILE_WRITE))
	{
		DWORD dwAttr = GetFileAttributes(szFileName);
		if (dwAttr & FILE_ATTRIBUTE_READONLY)
		{
			dwCreateFlag = GENERIC_READ;
			m_dwAccessType = TP_IMG_FILE_READ;
			dwShareAccess = FILE_SHARE_READ;
			dwCreateDescription = OPEN_EXISTING;
		}
		else
		{
			dwCreateFlag = GENERIC_WRITE;
			dwCreateDescription = CREATE_ALWAYS;
		}
	}
	else if (dwOpenType == TP_IMG_FILE_CREATE)
	{
		dwCreateFlag = GENERIC_ALL;
		dwCreateDescription = CREATE_ALWAYS;
	}
	m_pTgaFileHandle = CreateFile(szFileName, dwCreateFlag, dwShareAccess, NULL, dwCreateDescription, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == m_pTgaFileHandle)
	{
		m_pTgaFileHandle = NULL;
		return FALSE;
	}

	if (!(dwOpenType & TP_IMG_FILE_CREATE))//��������½��ļ������ļ�ͷ����
	{
		GetTGAInfo();
	}
	return TRUE;
}
BOOL	CTgaFile::CloseTgaFile()
{
	if(NULL != m_pTgaFileHandle)
	{
		CloseHandle(m_pTgaFileHandle);
		m_pTgaFileHandle = NULL;
	}
	if (NULL != m_pClrMap)
	{
		delete []m_pClrMap;
		m_pClrMap = NULL;
	}
	if (NULL != m_pCustomData)
	{
		delete []m_pCustomData;
		m_pCustomData = NULL;
	}
	if(NULL != m_pNXRLCBuf)
	{
		delete []m_pNXRLCBuf;
		m_pNXRLCBuf = NULL;
	}
	return TRUE;
}

/********************************************************/
//BOOL	CTgaFile::LoadTGAImage(DWORD*	dbuf)
//{
//	if (NULL == dbuf)
//	{
//		return FALSE;
//	}
//	m_pImgBuf = dbuf;
//	//if (NULL != m_pImgBuf)
//	//{
//	//	//�Ѿ����ˣ�ֱ�ӷ���
//	//	try
//	//	{
//	//		TP_MemcpyLine(dbuf,m_pImgBuf,m_cTGAHead.Width*m_cTGAHead.Height*4);
//	//	}
//	//	catch(...)
//	//	{
//	//		return FALSE;
//	//	}
//	//	return TRUE;
//	//}
//	if (NULL == m_pFile)
//	{
//		return FALSE;
//	}
//	if (m_dwAccessType & (TP_IMG_FILE_WRITE|TP_IMG_FILE_CREATE))
//	{
//		m_pFile->Flush();
//	}
//	if (m_llFilePos != sizeof(TGA_HEAD)+m_cTGAHead.Identsize)
//	{
//		m_pFile->Seek(sizeof(TGA_HEAD)+m_cTGAHead.Identsize, CFile::begin);
//		m_llFilePos = sizeof(TGA_HEAD)+m_cTGAHead.Identsize;
//	}
//
//	//�ж��Ƿ��е�ɫ��
//	if (m_cTGAHead.Colormaptype == 1)//�е�ɫ��
//	{
//		//��ȡ��ɫ��
//		ReadClrMap();
//	}
//
//	ReadImage();
//	//try
//	//{
//	//	TP_MemcpyLine(dbuf,m_pImgBuf,m_cTGAHead.Width*m_cTGAHead.Height*4);
//	//}
//	//catch(...)
//	//{
//	//	return FALSE;
//	//}
//	return TRUE;
//}

BOOL	CTgaFile::LoadTGAImage(DWORD*	dbuf)
{
	if (NULL == dbuf)
	{
		return FALSE;
	}
	m_pImgBuf = dbuf;
	
	if (NULL == m_pTgaFileHandle)
	{
		return FALSE;
	}
	if (m_dwAccessType & (TP_IMG_FILE_WRITE|TP_IMG_FILE_CREATE))
	{
		FlushFileBuffers(m_pTgaFileHandle);
	}
	if (m_llFilePos.QuadPart != sizeof(TGA_HEAD)+m_cTGAHead.Identsize)
	{
		LARGE_INTEGER llMoveDistance;
		llMoveDistance.QuadPart = sizeof(TGA_HEAD)+m_cTGAHead.Identsize;
		SetFilePointerEx(m_pTgaFileHandle, llMoveDistance, NULL, FILE_BEGIN);
		m_llFilePos.QuadPart = sizeof(TGA_HEAD)+m_cTGAHead.Identsize;
	}

	//�ж��Ƿ��е�ɫ��
	if (m_cTGAHead.Colormaptype == 1)//�е�ɫ��
	{
		//��ȡ��ɫ��
		ReadClrMap();
	}

	ReadImage();
	return TRUE;
}


// ����ͼ���Ƿ��� alpha ��Ϣ
BOOL	CTgaFile::_NXHaveAlpha(void)
{
	if(m_cTGAHead.Colormaptype != 0)
	{
		return FALSE; // ���е�ɫ�壬������ alpaha
	}

	if(m_cTGAHead.Imagetype != 2 && m_cTGAHead.Imagetype != 10)
	{
		// ͼ�����ݲ���δѹ����RLE���� true color
		return FALSE; // ������ alpaha
	}

	if(m_cTGAHead.Bits != 32) // 32 λͼ��ź��� alpha
	{
		return FALSE; // ������ alpaha
	}

	if((m_cTGAHead.ImgDescriptor & 0x0f) == 0)
	{
		return FALSE; // alpha λ��Ϊ 0
	}

	return TRUE;
}

BOOL CTgaFile::_NXSupportBlockRead(void) // ͼ���Ƿ���Էֿ��ȡ by ljj@2011-4-28
{
	//ֻ����ѹ���л�������ɫ��ͼ�����֧��.���಻֧��.
	if (m_cTGAHead.Imagetype == 1 || m_cTGAHead.Imagetype == 2)
		return TRUE;
	return FALSE;
}
// �õ�ͼ�������Ƿ�����½ǿ�ʼ
BOOL	CTgaFile::_NXImageBeginLeftBottom(void)
{
	BYTE byLocFlag = (m_cTGAHead.ImgDescriptor >> 4)&0x03;//ͼ��ԭ��λ�ñ�־��0�����½ǣ�2�����Ͻ�
	if(byLocFlag == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
// �ֿ��ȡͼ������
BOOL	CTgaFile::NXPrepareLoadTGAImage(DWORD*	dbuf)
{
	if (NULL == dbuf)
	{
		return FALSE;
	}
	m_pImgBuf = dbuf;
	if (NULL == m_pTgaFileHandle)
	{
		return FALSE;
	}
	if (m_dwAccessType & (TP_IMG_FILE_WRITE|TP_IMG_FILE_CREATE))
	{
		FlushFileBuffers(m_pTgaFileHandle);
	}
	if (m_llFilePos.QuadPart != sizeof(TGA_HEAD)+m_cTGAHead.Identsize)
	{
		LARGE_INTEGER llMoveDistance;
		llMoveDistance.QuadPart = sizeof(TGA_HEAD)+m_cTGAHead.Identsize;
		SetFilePointerEx(m_pTgaFileHandle, llMoveDistance, NULL, FILE_BEGIN);
		m_llFilePos.QuadPart = sizeof(TGA_HEAD)+m_cTGAHead.Identsize;
	}

	//�ж��Ƿ��е�ɫ��
	if (m_cTGAHead.Colormaptype == 1)//�е�ɫ��
	{
		//��ȡ��ɫ��
		ReadClrMap();
	}

	return TRUE;
}
// �ֿ��ȡͼ������
//��ȡ��ɫ��,ֻ��LoadImage������ã�ǰ��Ӧ�ñ�֤�ļ���Ч�����е�ɫ�壬���ļ�ָ���Ѿ��ƶ�����ɫ�崦
BOOL CTgaFile::ReadClrMap()
{
	//��ɫ�������
	if (NULL != m_pClrMap)
	{
		delete []m_pClrMap;
		m_pClrMap = NULL;
	}
	m_pClrMap = new RGBQUAD[m_cTGAHead.Colormaplength];
	TP_MemsetDWORD((DWORD*)m_pClrMap , 0, m_cTGAHead.Colormaplength);
	long lSrcClrSize = m_cTGAHead.Colormaplength* m_cTGAHead.Colormapbits / 8;
	BYTE* pTempBuf = new BYTE[lSrcClrSize];
	DWORD dwRealReadBytes = 0;
	ReadFile(m_pTgaFileHandle, pTempBuf, lSrcClrSize, &dwRealReadBytes, NULL);
	m_llFilePos.QuadPart += dwRealReadBytes;
	if ((DWORD)lSrcClrSize > dwRealReadBytes)
	{
		delete []m_pClrMap;
		m_pClrMap = NULL;
		delete []pTempBuf;
		return FALSE;
	}
	//ת����ɫ��32λ��ɫ��
	switch (m_cTGAHead.Colormapbits)
	{
	case 16://arrrrrgg gggbbbbb
		for (int i=0;i<m_cTGAHead.Colormaplength;i++)
		{
			m_pClrMap[i].rgbBlue = ((((WORD*)pTempBuf)[i]) & 0x001F)<<3;//ȡ����ɫֵ������ֱ�����ŵ�32λ
			m_pClrMap[i].rgbGreen = ((((WORD*)pTempBuf)[i]) & 0x03E0)>>2;
			m_pClrMap[i].rgbRed = ((((WORD*)pTempBuf)[i]) & 0x7C00)>>7;
			m_pClrMap[i].rgbReserved = 0xff;//((((WORD*)pTempBuf)[i]) & 0x8000) ? 0xFF : 0; // 16 λ������alpha��Ϣ
		}
		break;
	case 24://B��G��R
		for (int i=0;i<m_cTGAHead.Colormaplength;i++)
		{
			TP_MemcpyLine((BYTE*)(&(m_pClrMap[i])), pTempBuf + 3*i, 3);
			m_pClrMap[i].rgbReserved = 0xff;
		}
		break;
	case 32://B��G��R��A
		TP_MemcpyLine((BYTE*)m_pClrMap,pTempBuf,lSrcClrSize);
		break;
	default:
		delete []m_pClrMap;
		m_pClrMap = NULL;
		delete []pTempBuf;
		return FALSE;
		break;
	}
	delete []pTempBuf;
	return TRUE;
}
extern DWORD g_dwBlockSize;
//�ϲ���ã����뱣֤��ǰ�ļ�ָ����ļ���ȡλ�õ���ȷ��
BOOL	CTgaFile::ReadImage()
{
	//if (NULL != m_pImgBuf)
	//{
	//	delete []m_pImgBuf;
	//	m_pImgBuf = NULL;
	//}
	//m_pImgBuf = new DWORD [m_cTGAHead.Width*m_cTGAHead.Height];
	if (NULL == m_pImgBuf)
	{
		m_pImgBuf = NULL;
		return FALSE;
	}
	TP_MemsetDWORD(m_pImgBuf,0,m_cTGAHead.Width*m_cTGAHead.Height);
	unsigned long lImgDataSize = m_cTGAHead.Width*m_cTGAHead.Height * m_cTGAHead.Bits/8;
	unsigned long lImgDataSizeRLC = 0;
	BYTE* pTmpBuf = NULL;
	BOOL  bNewTmpBuf = FALSE;
	BYTE* pTmpBufRLC = NULL;
	BYTE byLocFlag = (m_cTGAHead.ImgDescriptor >> 4)&0x03;//ͼ��ԭ��λ�ñ�־��0�����½ǣ�2�����Ͻ�
	DWORD dwRealReadBytes = 0;
	switch (m_cTGAHead.Imagetype)
	{
	case 1://��ѹ����ɫ��ͼ��
		pTmpBuf = new BYTE[lImgDataSize];
		ReadFile(m_pTgaFileHandle, pTmpBuf, lImgDataSize, &dwRealReadBytes, NULL);
		if (lImgDataSize > dwRealReadBytes)
		{
			m_llFilePos.QuadPart += lImgDataSize;
			delete []pTmpBuf;
			m_pImgBuf = NULL;
			return FALSE;
		}
		m_llFilePos.QuadPart += lImgDataSize;
		//������ɫ�����Ŀ��Buffer
		if (!FillDstBufByClrMap(m_pClrMap,m_cTGAHead.Colormaplength,(BYTE*)m_pImgBuf,32,pTmpBuf,m_cTGAHead.Bits,m_cTGAHead.Width,m_cTGAHead.Height,byLocFlag))
		{
			delete []pTmpBuf;
//			delete []m_pImgBuf;
			m_pImgBuf = NULL;
			return FALSE;
		}
		//TP_OverTurnSelf (m_pImgBuf, m_cTGAHead.Width * 4, m_cTGAHead.Height);
		delete []pTmpBuf;
		break;
	case 2://��ѹ������ɫ��ͼ��
		switch (m_cTGAHead.Bits)
		{
		case 16://16λ: arrrrrgg gggbbbbb
			//����
			pTmpBuf = new BYTE[lImgDataSize];
			ReadFile(m_pTgaFileHandle, pTmpBuf, lImgDataSize, &dwRealReadBytes, NULL);
			if (lImgDataSize > dwRealReadBytes)
			{
				m_llFilePos.QuadPart += lImgDataSize;
				delete []pTmpBuf;
//				delete []m_pImgBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			m_llFilePos.QuadPart += lImgDataSize;

			if (!Tga16To32(m_pImgBuf,pTmpBuf,m_cTGAHead.Width,m_cTGAHead.Height,byLocFlag))//��16λת��Ϊ32λ
			{
				delete []pTmpBuf;
//				delete []m_pImgBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			delete []pTmpBuf;
			break;
		case 24:
			//ֱ�Ӷ��룬Alpha��Ϊ0xff
			pTmpBuf = new BYTE[lImgDataSize];
			ReadFile(m_pTgaFileHandle, pTmpBuf, lImgDataSize, &dwRealReadBytes, NULL);
			if (lImgDataSize > dwRealReadBytes)
			{
				m_llFilePos.QuadPart += lImgDataSize;
				delete []pTmpBuf;
//				delete []m_pImgBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			m_llFilePos.QuadPart += lImgDataSize;
			if (byLocFlag == 0)//���½�
			{
				TPCS_24_to_32_Frame((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,pTmpBuf,m_cTGAHead.Width*3,NULL,0,m_cTGAHead.Width,m_cTGAHead.Height,TRUE,TRUE);
			}
			else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
			{
				TPCS_24_to_32_Frame((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,pTmpBuf,m_cTGAHead.Width*3,NULL,0,m_cTGAHead.Width,m_cTGAHead.Height,FALSE,FALSE);
			}
			else 
			{
				//����δ֪
				delete []pTmpBuf;
//				delete []m_pImgBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}

			delete []pTmpBuf;
			break;
		case 32:
			
#ifdef TP_NS_NET
		{//CD2009.10.01
			DWORD dwBlockSize = g_dwBlockSize; // �����ִ�зֿ�д��
			DWORD dwLength = lImgDataSize;
			DWORD dwRead = 0L;
			DWORD dwToRead = dwRead;
			DWORD dwLeft = dwLength;
			LPBYTE pBufPos = (LPBYTE)m_pImgBuf;//CD2009.10.01
			while ( dwRead < dwLength )
			{
				dwLeft = dwLength - dwRead;
				dwToRead = min( dwLeft, dwBlockSize );

				if ( !ReadFile(m_pTgaFileHandle, pBufPos, dwToRead, &dwRealReadBytes, NULL) )
				{
					m_llFilePos.QuadPart = lImgDataSize;//CD2009.10.01
					//delete m_pImgBuf;
					m_pImgBuf = NULL;
					return FALSE;
				}
				pBufPos += dwToRead;
				dwRead += dwToRead;
			}
		}//CD2009.10.01
#else
			//ֱ�Ӷ���
			ReadFile(m_pTgaFileHandle, m_pImgBuf, lImgDataSize, &dwRealReadBytes, NULL);
			if (lImgDataSize > dwRealReadBytes)
			{
				m_llFilePos.QuadPart += lImgDataSize;
//				delete []m_pImgBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
#endif
			m_llFilePos.QuadPart += lImgDataSize;
			if (byLocFlag == 0)//���½ǣ���ת
			{
				TP_OverTurnSelf((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,m_cTGAHead.Height);
			}
			else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
			{
			}
			else//������������
			{
//				delete []m_pImgBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			break;
		default:
//			delete []m_pImgBuf;
			m_pImgBuf = NULL;
			return FALSE;
			break;
		}
		break;
	case 9://RLC,����ɫ��
	case 10://RLC��������ɫ��
		//�������ݣ���RLC���뺯��
		lImgDataSizeRLC = (unsigned long)(m_stImageInfo.llFileSize - sizeof(TGA_HEAD) - m_cTGAHead.Identsize - m_cTGAHead.Colormaplength*m_cTGAHead.Colormapbits/8);
		pTmpBufRLC = new BYTE[lImgDataSizeRLC ];
		ReadFile(m_pTgaFileHandle, pTmpBufRLC, lImgDataSizeRLC, &dwRealReadBytes, NULL);
		if (lImgDataSizeRLC > dwRealReadBytes)
		{
			m_llFilePos.QuadPart += lImgDataSizeRLC;
			delete []pTmpBufRLC;
//			delete []m_pImgBuf;
			m_pImgBuf = NULL;
			return FALSE;
		}
		m_llFilePos.QuadPart += lImgDataSizeRLC;
		//��RLC���뺯��
		if (!TGARLCDec(m_pImgBuf,pTmpBufRLC,lImgDataSizeRLC,m_cTGAHead.Width,m_cTGAHead.Height,m_cTGAHead.Bits,m_pClrMap,m_cTGAHead.Colormaplength,byLocFlag))
		{
			delete []pTmpBufRLC;
//			delete []m_pImgBuf;
			m_pImgBuf = NULL;
			return FALSE;
		}
		delete []pTmpBufRLC;
		break;
	default:
//		delete []m_pImgBuf;
		m_pImgBuf = NULL;
		return FALSE;
		break;
	}

	return TRUE;
}

// �ֿ��ȡͼ������
BOOL	CTgaFile::NXReadImage( int iBeginRow, int iEndRow)
{
	if (NULL == m_pImgBuf)
	{
		return FALSE;
	}
	int iRealReadImageHeight = iEndRow - iBeginRow + 1;
	TP_MemsetDWORD(m_pImgBuf, 0xff,m_cTGAHead.Width * iRealReadImageHeight); // BGRA
	unsigned long lImgDataSize = m_cTGAHead.Width * iRealReadImageHeight * m_cTGAHead.Bits/8;
	unsigned long lImgDataSizeRLC = 0;
	BYTE* pTmpBuf = NULL;
	BOOL  bNewTmpBuf = FALSE;
	BYTE* pTmpBufRLC = NULL;
	BYTE byLocFlag = (m_cTGAHead.ImgDescriptor >> 4)&0x03;//ͼ��ԭ��λ�ñ�־��0�����½ǣ�2�����Ͻ�
	DWORD dwRealReadBytes = 0;
#ifdef TP_NS_NET // �ֲ����������Ƴ�switch
	DWORD dwLength = lImgDataSize;
	DWORD dwRead = 0L;
	DWORD dwToRead = dwRead;
	DWORD dwLeft = dwLength;
	LPBYTE pBufPos = (LPBYTE)m_pImgBuf;
	HKEY hKey = NULL;
	DWORD dwBlockSize = 0L; // �����ִ�зֿ�д��
	DWORD dwValueSize = REG_DWORD;
	DWORD dwValueType = REG_DWORD;
#endif
	switch (m_cTGAHead.Imagetype)
	{
	case 1://��ѹ����ɫ��ͼ��
		pTmpBuf = new BYTE[lImgDataSize]; // ��Ҫ��ȡ��ͼ������
		ReadFile(m_pTgaFileHandle, pTmpBuf, lImgDataSize, &dwRealReadBytes, NULL);
		if (lImgDataSize > dwRealReadBytes)
		{
			m_llFilePos.QuadPart += lImgDataSize;
			delete []pTmpBuf;
			m_pImgBuf = NULL;
			return FALSE;
		}
		m_llFilePos.QuadPart += lImgDataSize;
		//������ɫ�����Ŀ��Buffer
		if (!FillDstBufByClrMap(m_pClrMap,m_cTGAHead.Colormaplength,(BYTE*)m_pImgBuf,32,pTmpBuf,m_cTGAHead.Bits,m_cTGAHead.Width,iRealReadImageHeight,byLocFlag))
		{
			delete []pTmpBuf;
			m_pImgBuf = NULL;
			return FALSE;
		}
		delete []pTmpBuf;
		break;
	case 2://��ѹ������ɫ��ͼ��
		switch (m_cTGAHead.Bits)
		{
		case 16://16λ: arrrrrgg gggbbbbb
			//����
			pTmpBuf = new BYTE[lImgDataSize];
			ReadFile(m_pTgaFileHandle, pTmpBuf, lImgDataSize, &dwRealReadBytes, NULL);
			if (lImgDataSize > dwRealReadBytes)
			{
				m_llFilePos.QuadPart += lImgDataSize;
				delete []pTmpBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			m_llFilePos.QuadPart += lImgDataSize;

			if (!Tga16To32(m_pImgBuf,pTmpBuf,m_cTGAHead.Width,iRealReadImageHeight,byLocFlag))//��16λת��Ϊ32λ
			{
				delete []pTmpBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			delete []pTmpBuf;
			break;
		case 24:
			//ֱ�Ӷ��룬Alpha��Ϊ0xff
			pTmpBuf = new BYTE[lImgDataSize];
			ReadFile(m_pTgaFileHandle, pTmpBuf, lImgDataSize, &dwRealReadBytes, NULL);
			if (lImgDataSize > dwRealReadBytes)
			{
				m_llFilePos.QuadPart += lImgDataSize;
				delete []pTmpBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			m_llFilePos.QuadPart += lImgDataSize;
			if (byLocFlag == 0)//���½�
			{
				TPCS_24_to_32_Frame((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,pTmpBuf,m_cTGAHead.Width*3,NULL,0,m_cTGAHead.Width,iRealReadImageHeight,TRUE,TRUE);
			}
			else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
			{
				TPCS_24_to_32_Frame((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,pTmpBuf,m_cTGAHead.Width*3,NULL,0,m_cTGAHead.Width,iRealReadImageHeight,FALSE,FALSE);
			}
			else 
			{
				//����δ֪
				delete []pTmpBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}

			delete []pTmpBuf;
			break;
		case 32:

#ifdef TP_NS_NET
			dwBlockSize = g_dwBlockSize;
			while ( dwRead < dwLength )
			{
				dwLeft = dwLength - dwRead;
				dwToRead = min( dwLeft, dwBlockSize );
				ReadFile(m_pTgaFileHandle, pBufPos, dwToRead, &dwRealReadBytes, NULL);
				if ( dwRealReadBytes != dwToRead )
				{
					m_llFilePos.QuadPart == lImgDataSize;
					//delete m_pImgBuf;
					m_pImgBuf = NULL;
					return FALSE;
				}
				pBufPos += dwToRead;
				dwRead += dwToRead;
			}
#else
			//ֱ�Ӷ���
			ReadFile(m_pTgaFileHandle, m_pImgBuf, lImgDataSize, &dwRealReadBytes, NULL);
			if (lImgDataSize > dwRealReadBytes)
			{
				m_llFilePos.QuadPart += lImgDataSize;
				m_pImgBuf = NULL;
				return FALSE;
			}
#endif
			m_llFilePos.QuadPart += lImgDataSize;
			if (byLocFlag == 0)//���½ǣ���ת
			{
				TP_OverTurnSelf((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,iRealReadImageHeight);
			}
			else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
			{
			}
			else//������������
			{
				m_pImgBuf = NULL;
				return FALSE;
			}
			break;
		default:
			m_pImgBuf = NULL;
			return FALSE;
			break;
		}
		break;
	case 9://RLC,����ɫ��
	case 10://RLC��������ɫ��
		//�������ݣ���RLC���뺯��
		if(NULL == m_pNXRLCBuf)
		{
			m_pNXRLCBuf = new BYTE[NX_TGA_RLC_BUF_SIZE ]; // ���뻺���
			TP_MemsetChar(m_pNXRLCBuf, 0, NX_TGA_RLC_BUF_SIZE);
		}
		ReadFile(m_pTgaFileHandle, m_pNXRLCBuf, NX_TGA_RLC_BUF_SIZE, &m_dwNXRLCBufCapcity, NULL);
		if(m_dwNXRLCBufCapcity == 0)
		{
			m_pImgBuf = NULL;
			return FALSE;
		}
		m_llFilePos.QuadPart += m_dwNXRLCBufCapcity;
		//��RLC���뺯��
		if (!NXTGARLCDec(m_pImgBuf,m_pNXRLCBuf,m_dwNXRLCBufCapcity,m_cTGAHead.Width,iRealReadImageHeight,m_cTGAHead.Bits,m_pClrMap,m_cTGAHead.Colormaplength,byLocFlag))
		{
			m_pImgBuf = NULL;
			return FALSE;
		}
		break;
	default:
		m_pImgBuf = NULL;
		return FALSE;
		break;
	}
	return TRUE;
}

BOOL	CTgaFile::Tga16To32(DWORD* pDst, BYTE* pSrc, long lWidth, long lHeight, BYTE byLocFlag)//����ѹ��16λת��Ϊ32λ
{
	if (byLocFlag == 0)//���½ǣ���Ҫ��ת
	{
		for (int y = lHeight - 1; y >= 0 ; y--)
		{
#if _PPPPP_ //����Ż����Ǻ���Ч�ģ�800*600�ģ����Ż���6.5ms���Ż���1.5ms
			tga_cr16_to_cr32_line((unsigned char*)(pDst + (lHeight - 1 - y) * lWidth) , (unsigned char*)(pSrc + y * lWidth * 2), lWidth);
#else	
			RGBBIT stTmpClr;
			for (int x = 0; x < lWidth; x++)
			{
				//��ÿ�����أ���ֵ�32λ
				long lIdx = y*lWidth + x;
				stTmpClr.RGBbits.rgbBlue = ((((WORD*)pSrc)[lIdx]) & 0x001F)<<3;//ȡ����ɫֵ������ֱ�����ŵ�32λ
				stTmpClr.RGBbits.rgbGreen = ((((WORD*)pSrc)[lIdx]) & 0x03E0)>>2;
				stTmpClr.RGBbits.rgbRed = ((((WORD*)pSrc)[lIdx]) & 0x7C00)>>7;
				stTmpClr.RGBbits.rgbReserved = 0xff;//((((WORD*)pSrc)[lIdx]) & 0x8000) ? 0xFF : 0;
				pDst[ (lHeight - 1 - y) * lWidth + x]=(DWORD)stTmpClr.RGBdword;
			}
#endif
		}
	}
	else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
	{

#if _PPPPP_
		
		for (int y = 0; y < lHeight ; y++)
		{
			tga_cr16_to_cr32_line((unsigned char*)(pDst + y * lWidth) , (unsigned char*)(pSrc + y * lWidth * 2), lWidth);
		}
#else	
		RGBBIT stTmpClr;
		for (int m = 0; m < lWidth*lHeight; m++)
		{
			//��ÿ�����أ���ֵ�32λ
			stTmpClr.RGBbits.rgbBlue = ((((WORD*)pSrc)[m]) & 0x001F)<<3;//ȡ����ɫֵ������ֱ�����ŵ�32λ
			stTmpClr.RGBbits.rgbGreen = ((((WORD*)pSrc)[m]) & 0x03E0)>>2;
			stTmpClr.RGBbits.rgbRed = ((((WORD*)pSrc)[m]) & 0x7C00)>>7;
			stTmpClr.RGBbits.rgbReserved = 0xff;//((((WORD*)pSrc)[m]) & 0x8000) ? 0xFF : 0;
			pDst[m]=(DWORD)stTmpClr.RGBdword;
		}
#endif
	}
	else 
	{
		//����δ֪
		return FALSE;
	}
	return TRUE;
}


//�ɵ����߱�֤��������ȷ�ͺ�����
BOOL	CTgaFile::FillDstBufByClrMap(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, long lDstBits,BYTE* pSrc,long lSrcBits,long lWidth, long lHeight,BYTE byLocFlag)
{
	ASSERT(lDstBits == 32);
	if (lDstBits != 32)
	{
		return FALSE;
	}
#if _PPPPP_
	if (byLocFlag == 0)//���½ǣ���Ҫ��ת
	{
		for (int y = lHeight - 1; y >= 0 ; y--)
		{
			tga_palette32_line((unsigned char*)(pDst + (lHeight - 1 - y)*lWidth * 4),(unsigned char*)(pSrc + y * lWidth * lSrcBits / 8),lSrcBits, lWidth ,(unsigned char*)pClrMap);
		}
	}
	else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
	{
		for (int y = 01; y < lHeight ; y++)
		{
			tga_palette32_line((unsigned char*)(pDst + y*lWidth * 4),(unsigned char*)(pSrc + y * lWidth * lSrcBits / 8),lSrcBits, lWidth  ,(unsigned char*)pClrMap);
		}
	}
	else//�������ͣ��������� 
	{
		return FALSE;
	}
#else

	switch (lSrcBits)
	{
	case 8:
		return FillDstBufByClrMap8(pClrMap,ulClrMapCount,pDst,pSrc,lWidth,lHeight,byLocFlag);
	case 16:
		return FillDstBufByClrMap16(pClrMap,ulClrMapCount,pDst,pSrc,lWidth,lHeight,byLocFlag);
	case 24:
		return FillDstBufByClrMap24(pClrMap,ulClrMapCount,pDst,pSrc,lWidth,lHeight,byLocFlag);
	case 32:
		return FillDstBufByClrMap32(pClrMap,ulClrMapCount,pDst,pSrc,lWidth,lHeight,byLocFlag);
	}
#endif
	return TRUE; // ��������
}

//Դ������8λ
BOOL	CTgaFile::FillDstBufByClrMap8(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag)
{
	long lIdx = 0;
	unsigned long lClrIdx = 0;
	if (byLocFlag == 0)//���½ǣ���Ҫ��ת
	{
		BYTE *pDstTrv = pDst;
		for (int y = m_cTGAHead.Height - 1; y >= 0 ; y--)
		{
			for (int x = 0; x < m_cTGAHead.Width; x++)
			{
				lIdx = y*m_cTGAHead.Width + x;
				lClrIdx = (unsigned long)(pSrc[lIdx]);
				if (lClrIdx < ulClrMapCount)
					TP_MemcpyLine(pDstTrv,(BYTE*)(&m_pClrMap[lClrIdx]),4);
				pDstTrv += 4;
			}
		}
	}
	else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
	{
		for (int m = 0; m < m_cTGAHead.Width*m_cTGAHead.Height; m++)
		{
			
			lClrIdx = (unsigned long)(pSrc[m]);
			if (lClrIdx < ulClrMapCount)
				TP_MemcpyLine((pDst+4*m),(BYTE*)(&m_pClrMap[lClrIdx]),4);
		}

	}
	else//�������ͣ��������� 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL	CTgaFile::FillDstBufByClrMap16(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag)
{
	long lIdx = 0;
	unsigned long lClrIdx = 0;
	if (byLocFlag == 0)//���½ǣ���Ҫ��ת
	{
		BYTE *pDstTrv = pDst;
		for (int y = m_cTGAHead.Height - 1; y >= 0 ; y--)
		{
			for (int x = 0; x < m_cTGAHead.Width; x++)
			{
				lIdx = y*m_cTGAHead.Width + x;
				lClrIdx = (unsigned long)(((WORD*)pSrc)[lIdx]);
				if (lClrIdx < ulClrMapCount)
					TP_MemcpyLine(pDstTrv,(BYTE*)(&m_pClrMap[lClrIdx]),4);
				pDstTrv += 4;
			}
		}
	}
	else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
	{
		for (int m = 0; m < m_cTGAHead.Width*m_cTGAHead.Height; m++)
		{

			lClrIdx = (unsigned long)(((WORD*)pSrc)[m]);
			if (lClrIdx < ulClrMapCount)
				TP_MemcpyLine((pDst+4*m),(BYTE*)(&m_pClrMap[lClrIdx]),4);
		}

	}
	else//�������ͣ��������� 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL	CTgaFile::FillDstBufByClrMap24(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag)
{
	long lIdx = 0;
	unsigned long lClrIdx = 0;
	if (byLocFlag == 0)//���½ǣ���Ҫ��ת
	{
		for (int y = m_cTGAHead.Height - 1; y >= 0 ; y--)
		{
			for (int x = 0; x < m_cTGAHead.Width; x++)
			{
				lIdx = y*m_cTGAHead.Width + x;
				lClrIdx = 0;
				((BYTE*)(&lClrIdx))[1] = pSrc[lIdx*3 + 0];
				((BYTE*)(&lClrIdx))[2] = pSrc[lIdx*3 + 1];
				((BYTE*)(&lClrIdx))[3] = pSrc[lIdx*3 + 2];
				lClrIdx = lClrIdx >> 8;

				if (lClrIdx < ulClrMapCount && lClrIdx >= 0)
					TP_MemcpyLine((pDst+4*lIdx),(BYTE*)(&m_pClrMap[lClrIdx]),4);
			}
		}
	}
	else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
	{
		for (int m = 0; m < m_cTGAHead.Width*m_cTGAHead.Height; m++)
		{
			lClrIdx = 0;
			((BYTE*)(&lClrIdx))[1] = pSrc[m*3 + 0];
			((BYTE*)(&lClrIdx))[2] = pSrc[m*3 + 1];
			((BYTE*)(&lClrIdx))[3] = pSrc[m*3 + 2];
			lClrIdx = lClrIdx >> 8;

			if (lClrIdx < ulClrMapCount && lClrIdx >= 0)
				TP_MemcpyLine((pDst+4*m),(BYTE*)(&m_pClrMap[lClrIdx]),4);
		}

	}
	else//�������ͣ��������� 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL	CTgaFile::FillDstBufByClrMap32(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag)
{
	long lIdx = 0;
	unsigned long lClrIdx = 0;
	if (byLocFlag == 0)//���½ǣ���Ҫ��ת
	{
		for (int y = m_cTGAHead.Height - 1; y >= 0 ; y--)
		{
			for (int x = 0; x < m_cTGAHead.Width; x++)
			{
				lIdx = y*m_cTGAHead.Width + x;
				lClrIdx = (unsigned long)(((DWORD*)pSrc)[lIdx]);
				if (lClrIdx < ulClrMapCount)
					TP_MemcpyLine((pDst+4*lIdx),(BYTE*)(&m_pClrMap[lClrIdx]),4);
			}
		}
	}
	else if (byLocFlag == 2)//���Ͻǣ�����Ҫ��ת
	{
		for (int m = 0; m < m_cTGAHead.Width*m_cTGAHead.Height; m++)
		{

			lClrIdx = (unsigned long)(((DWORD*)pSrc)[m]);
			if (lClrIdx < ulClrMapCount)
				TP_MemcpyLine((pDst+4*m),(BYTE*)(&m_pClrMap[lClrIdx]),4);
		}

	}
	else//�������ͣ��������� 
	{
		return FALSE;
	}
	return TRUE;
}

//������ɫ����ͬλ�������ɴ���
BOOL	CTgaFile::TGARLCDec(DWORD* pDst, BYTE* pSrc, LONGLONG llSrcSize,long lWidth,long lHeight,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount,BYTE byLocFlag)//RLC����,Ŀ��Ϊ32λ
{
#if 0 // ljj:�����粻�Ż�...���Ż���720*576��16ms���Ż���21ms��
	if (NULL == pClrMap && lSrcBits == 32) 
	{
		//������ɫ��,32λ������Ҫ��ת����ֱ�Ӱ�Dst���롣
		tga_rlc_line((unsigned char*)pDst,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (byLocFlag == 0) 
		{
			//��Ҫ��ת
			TP_OverTurnSelf((BYTE*)pDst,lWidth*4,lHeight);
		}
		return TRUE;
	}
	if (NULL == pClrMap) 
	{
		//������ɫ��
		BYTE* ptmp = new BYTE[lWidth*lHeight*lSrcBits/8];
		tga_rlc_line((unsigned char*)ptmp,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (lSrcBits == 16) 
		{
			if (byLocFlag == 0)//���½ǣ���Ҫ��ת
			{
				for (long ldx = lHeight -1; ldx >= 0; ldx --)
				{
					tga_cr16_to_cr32_line((unsigned char*)(pDst + (lHeight - 1 - ldx)*lWidth),(unsigned char*)(ptmp + ldx*lWidth*2),lWidth);
				}
			}
			else if (byLocFlag == 2)//���Ͻǣ����跴ת
			{
				for (long ldx = 0; ldx < lHeight; ldx ++)
				{
					tga_cr16_to_cr32_line((unsigned char*)(pDst + ldx*lWidth),(unsigned char*)(pSrc + ldx*lWidth*2),lWidth);
				}
			}
			else//����������������������
			{
				delete []ptmp;
				return FALSE;
			}
		}
		else if (lSrcBits == 24) 
		{
			//����24��32�ı任
			if (byLocFlag == 0)//���½ǣ���Ҫ��ת
			{
				TPCS_24_to_32_Frame((BYTE*)pDst,lWidth*4,ptmp,lWidth*3,NULL,0,lWidth,lHeight,TRUE,TRUE);
			}
			else if (byLocFlag == 2)//���Ͻǣ����跴ת
			{
				TPCS_24_to_32_Frame((BYTE*)pDst,lWidth*4,ptmp,lWidth*3,NULL,0,lWidth,lHeight,FALSE,FALSE);
			}
			else//����������������������
			{
				delete []ptmp;
				return FALSE;
			}
		}
		else
		{
			delete []ptmp;
			return FALSE;
		}
		delete []ptmp;
	}
	else
	{
		BYTE* ptmp = new BYTE[lWidth*lHeight*lSrcBits/8];
		tga_rlc_line((unsigned char*)ptmp,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (byLocFlag == 0)//���½ǣ���Ҫ��ת
		{
			for (long ldx = lHeight -1; ldx >= 0; ldx --)
			{
				tga_palette32_line((unsigned char*)(pDst + (lHeight - 1 - ldx)*lWidth),(unsigned char*)(ptmp + ldx*lWidth*lSrcBits/8),lSrcBits,lWidth*lSrcBits/8,(unsigned char*)pClrMap);
			}
		}
		else if (byLocFlag == 2)//���Ͻǣ����跴ת
		{
			for (long ldx = 0; ldx < lHeight; ldx ++)
			{
				tga_palette32_line((unsigned char*)(pDst + ldx*lWidth),(unsigned char*)(ptmp + ldx*lWidth*lSrcBits/8),lSrcBits,lWidth*lSrcBits/8,(unsigned char*)pClrMap);
			}
		}
		else//����������������������
		{
			delete []ptmp;
			return FALSE;
		}
		delete []ptmp;
	}

#else

	unsigned long	ulPixelIdx = 0;//Ŀ�������λ��
	LONGLONG	llSrcByteIdx = 0;//ѹ�������ֽ�λ��
	RGBBIT 	stDstClr;//�ӽ�ѹ��������еõ���Ŀ�����ɫֵ
	int iRes = 0;
	try
	{
		while((llSrcByteIdx < llSrcSize)&&(ulPixelIdx < (unsigned long)(lWidth*lHeight) ))//Դ��Ŀ�궼û�����
		{
			BYTE	byRepeat = 1;
			if((pSrc[llSrcByteIdx]&0x80)!=0) //RunLength���ݰ���ͷһ���ֽ���ѭ������N�������һ������ֵ������ɫֵ����Ϊ�������ֵ����ɫֵ�ظ�N�Ρ�
			{
				byRepeat=(pSrc[llSrcByteIdx]&0x7f)+1;//ѭ������N
				llSrcByteIdx += 1;//ָ����һ���ֽ�
				iRes = TGARLCOneValue(stDstClr,pSrc,llSrcByteIdx,llSrcSize,lSrcBits,pClrMap,lClrMapCount);
				if (0 == iRes)
				{
					return FALSE;
				}
				else if (-1 == iRes)
				{
					break; //�Ѿ�����Խ�磬ֹͣ���룬�������ѽ���Ĳ��֡�
				}
				if (byLocFlag == 0)//���½ǣ���Ҫ��ת
				{
					for(BYTE i=0;i<byRepeat;i++)
					{
						pDst[(lHeight - (ulPixelIdx / lWidth) -1) * lWidth + (ulPixelIdx % lWidth)] = (DWORD)stDstClr.RGBdword;
						ulPixelIdx += 1;
					}
				}
				else if (byLocFlag == 2)//���Ͻǣ����跴ת
				{
					for(BYTE i=0;i<byRepeat;i++)
					{
						pDst[ulPixelIdx] = (DWORD)stDstClr.RGBdword;
						ulPixelIdx += 1;
					}
				}
				else//����������������������
				{
					return FALSE;
				}
			}
			else //�޴������ݰ�����һ���ֽ��ǰ�ͷ������ѭ������N����ʾ����N��ֵ������ֵ����ɫֵ����N��ֵֻѭ��1�Ρ�
			{
				byRepeat=(pSrc[llSrcByteIdx]&0x7f)+1;
				llSrcByteIdx+=1;
				for(BYTE i=0;i<byRepeat;i++)
				{
					iRes = TGARLCOneValue(stDstClr,pSrc,llSrcByteIdx,llSrcSize,lSrcBits,pClrMap,lClrMapCount);
					if (0 == iRes)
				{
						return FALSE;
					}
					else if (-1 == iRes)
					{
						break; //�Ѿ�����Խ�磬ֹͣ���룬�������ѽ���Ĳ��֡�
					}
					if (byLocFlag == 0)//���½ǣ���Ҫ��ת
					{
						pDst[(lHeight - (ulPixelIdx / lWidth) -1) * lWidth + (ulPixelIdx % lWidth)] = (DWORD)stDstClr.RGBdword;
						ulPixelIdx += 1;
					}
					else if (byLocFlag == 2)//���Ͻǣ����跴ת
					{
						pDst[ulPixelIdx] = (DWORD)stDstClr.RGBdword;
						ulPixelIdx += 1;
					}
					else//����������������������
					{
						return FALSE;
					}
				}
			}
		}
		return TRUE;
	}
	catch(...)
	{
		return FALSE;
	}
#endif
	return TRUE;

}

BOOL	CTgaFile::NXTGARLCDec(DWORD* pDst, BYTE* pSrc, LONGLONG llSrcSize,long lWidth,long lHeight,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount,BYTE byLocFlag)//RLC����,Ŀ��Ϊ32λ
{
#if 0 // ljj:�����粻�Ż�...���Ż���720*576��16ms���Ż���21ms��
	if (NULL == pClrMap && lSrcBits == 32) 
	{
		//������ɫ��,32λ������Ҫ��ת����ֱ�Ӱ�Dst���롣
		tga_rlc_line((unsigned char*)pDst,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (byLocFlag == 0) 
		{
			//��Ҫ��ת
			TP_OverTurnSelf((BYTE*)pDst,lWidth*4,lHeight);
		}
		return TRUE;
	}
	if (NULL == pClrMap) 
	{
		//������ɫ��
		BYTE* ptmp = new BYTE[lWidth*lHeight*lSrcBits/8];
		tga_rlc_line((unsigned char*)ptmp,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (lSrcBits == 16) 
		{
			if (byLocFlag == 0)//���½ǣ���Ҫ��ת
			{
				for (long ldx = lHeight -1; ldx >= 0; ldx --)
				{
					tga_cr16_to_cr32_line((unsigned char*)(pDst + (lHeight - 1 - ldx)*lWidth),(unsigned char*)(ptmp + ldx*lWidth*2),lWidth);
				}
			}
			else if (byLocFlag == 2)//���Ͻǣ����跴ת
			{
				for (long ldx = 0; ldx < lHeight; ldx ++)
				{
					tga_cr16_to_cr32_line((unsigned char*)(pDst + ldx*lWidth),(unsigned char*)(pSrc + ldx*lWidth*2),lWidth);
				}
			}
			else//����������������������
			{
				delete []ptmp;
				return FALSE;
			}
		}
		else if (lSrcBits == 24) 
		{
			//����24��32�ı任
			if (byLocFlag == 0)//���½ǣ���Ҫ��ת
			{
				TPCS_24_to_32_Frame((BYTE*)pDst,lWidth*4,ptmp,lWidth*3,NULL,0,lWidth,lHeight,TRUE,TRUE);
			}
			else if (byLocFlag == 2)//���Ͻǣ����跴ת
			{
				TPCS_24_to_32_Frame((BYTE*)pDst,lWidth*4,ptmp,lWidth*3,NULL,0,lWidth,lHeight,FALSE,FALSE);
			}
			else//����������������������
			{
				delete []ptmp;
				return FALSE;
			}
		}
		else
		{
			delete []ptmp;
			return FALSE;
		}
		delete []ptmp;
	}
	else
	{
		BYTE* ptmp = new BYTE[lWidth*lHeight*lSrcBits/8];
		tga_rlc_line((unsigned char*)ptmp,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (byLocFlag == 0)//���½ǣ���Ҫ��ת
		{
			for (long ldx = lHeight -1; ldx >= 0; ldx --)
			{
				tga_palette32_line((unsigned char*)(pDst + (lHeight - 1 - ldx)*lWidth),(unsigned char*)(ptmp + ldx*lWidth*lSrcBits/8),lSrcBits,lWidth*lSrcBits/8,(unsigned char*)pClrMap);
			}
		}
		else if (byLocFlag == 2)//���Ͻǣ����跴ת
		{
			for (long ldx = 0; ldx < lHeight; ldx ++)
			{
				tga_palette32_line((unsigned char*)(pDst + ldx*lWidth),(unsigned char*)(ptmp + ldx*lWidth*lSrcBits/8),lSrcBits,lWidth*lSrcBits/8,(unsigned char*)pClrMap);
			}
		}
		else//����������������������
		{
			delete []ptmp;
			return FALSE;
		}
		delete []ptmp;
	}

#else

	unsigned long	ulPixelIdx = 0;//Ŀ�������λ��
	LONGLONG	llSrcByteIdx = 0;//ѹ�������ֽ�λ��
	RGBBIT 	stDstClr;//�ӽ�ѹ��������еõ���Ŀ�����ɫֵ
	int iRes = 0;
	while(ulPixelIdx < (unsigned long)(lWidth*lHeight) )//Ŀ��û�����
	{
		BYTE	byRepeat = 1;
		if(llSrcByteIdx >= llSrcSize)
		{
			// ������ѱ�ȡ�ɣ����¶�ȡ�ļ�
			ReadFile(m_pTgaFileHandle, pSrc, NX_TGA_RLC_BUF_SIZE, &m_dwNXRLCBufCapcity, NULL);
			if(m_dwNXRLCBufCapcity == 0)
			{
				return FALSE; // ��Ҫ���ݣ����ļ���û��
			}
			m_llFilePos.QuadPart += m_dwNXRLCBufCapcity;
			llSrcByteIdx = 0;
			if(llSrcSize > m_dwNXRLCBufCapcity)
			{
				llSrcSize = m_dwNXRLCBufCapcity;
			}
			//llSrcSize += m_dwNXRLCBufCapcity;
		}
		if((pSrc[llSrcByteIdx]&0x80)!=0) //RunLength���ݰ���ͷһ���ֽ���ѭ������N�������һ������ֵ������ɫֵ����Ϊ�������ֵ����ɫֵ�ظ�N�Ρ�
		{
			byRepeat=(pSrc[llSrcByteIdx]&0x7f)+1;//ѭ������N
			++llSrcByteIdx;//ָ����һ���ֽ�

			if(llSrcByteIdx + 4 > llSrcSize)
			{
				// TGARLCOneValue ������Ҫ������ֽڣ����ļ���ȡ add by wgf@2010-12-23
				DWORD dwUseableSize = (DWORD)( llSrcSize - llSrcByteIdx );
				BYTE *pMid = NULL;
				if(dwUseableSize > 0)
				{
					pMid = new BYTE[dwUseableSize];
					TP_MemcpyLine(pMid, pSrc + llSrcByteIdx, dwUseableSize);
				}
				
				ReadFile(m_pTgaFileHandle, pSrc + dwUseableSize, NX_TGA_RLC_BUF_SIZE - dwUseableSize, &m_dwNXRLCBufCapcity, NULL);
				TP_MemcpyLine(pSrc, pMid, dwUseableSize); // �ش�
				m_llFilePos.QuadPart += m_dwNXRLCBufCapcity;
				llSrcByteIdx = 0;
				llSrcSize = m_dwNXRLCBufCapcity + dwUseableSize;
			}
			iRes = TGARLCOneValue(stDstClr,pSrc,llSrcByteIdx,llSrcSize,lSrcBits,pClrMap,lClrMapCount);
			if (0 == iRes)
			{
				return FALSE;
			}
			else if (-1 == iRes)
			{
				break; //�Ѿ�����Խ�磬ֹͣ���룬�������ѽ���Ĳ��֡�
			}

			if (byLocFlag == 0)//���½ǣ���Ҫ��ת
			{
				for(BYTE i=0;i<byRepeat;i++)
				{
					pDst[(lHeight - (ulPixelIdx / lWidth) -1) * lWidth + (ulPixelIdx % lWidth)] = (DWORD)stDstClr.RGBdword;
					ulPixelIdx += 1;
				}
			}
			else if (byLocFlag == 2)//���Ͻǣ����跴ת
			{
				for(BYTE i=0;i<byRepeat;i++)
				{
					pDst[ulPixelIdx] = (DWORD)stDstClr.RGBdword;
					ulPixelIdx += 1;
				}
			}
			else//����������������������
			{
				return FALSE;
			}
		}
		else //�޴������ݰ�����һ���ֽ��ǰ�ͷ������ѭ������N����ʾ����N��ֵ������ֵ����ɫֵ����N��ֵֻѭ��1�Ρ�
		{
			byRepeat=(pSrc[llSrcByteIdx]&0x7f)+1;
			++llSrcByteIdx;

			for(BYTE i=0;i<byRepeat;i++)
			{
				if(llSrcByteIdx + 4 > llSrcSize)
				{
					// TGARLCOneValue ������Ҫ������ֽڣ����ļ���ȡ add by wgf@2010-12-23
					DWORD dwUseableSize = (DWORD)( llSrcSize - llSrcByteIdx );
					BYTE *pMid = NULL;
					if(dwUseableSize > 0)
					{
						pMid = new BYTE[dwUseableSize];
						TP_MemcpyLine(pMid, pSrc + llSrcByteIdx, dwUseableSize);
					}

					ReadFile(m_pTgaFileHandle, pSrc + dwUseableSize, NX_TGA_RLC_BUF_SIZE - dwUseableSize, &m_dwNXRLCBufCapcity, NULL);
					TP_MemcpyLine(pSrc, pMid, dwUseableSize); // �ش�
					m_llFilePos.QuadPart += m_dwNXRLCBufCapcity;
					llSrcByteIdx = 0;
					llSrcSize = m_dwNXRLCBufCapcity + dwUseableSize;
				}

				iRes = TGARLCOneValue(stDstClr,pSrc,llSrcByteIdx,llSrcSize,lSrcBits,pClrMap,lClrMapCount);
				if (0 == iRes)
				{
					return FALSE;
				}
				else if (-1 == iRes)
				{
					break; //�Ѿ�����Խ�磬ֹͣ���룬�������ѽ���Ĳ��֡�
				}

				if (byLocFlag == 0)//���½ǣ���Ҫ��ת
				{
					pDst[(lHeight - (ulPixelIdx / lWidth) -1) * lWidth + (ulPixelIdx % lWidth)] = (DWORD)stDstClr.RGBdword;
					ulPixelIdx += 1;
				}
				else if (byLocFlag == 2)//���Ͻǣ����跴ת
				{
					pDst[ulPixelIdx] = (DWORD)stDstClr.RGBdword;
					ulPixelIdx += 1;
				}
				else//����������������������
				{
					return FALSE;
				}
			}
		}
	}
	// ����������ˮ����������ļ�ָ��
	if(llSrcByteIdx < llSrcSize)
	{
		m_llFilePos.QuadPart -= (llSrcSize - llSrcByteIdx);
		LARGE_INTEGER llMoveDistance;
		llMoveDistance.QuadPart = llSrcByteIdx - llSrcSize;
		SetFilePointerEx(m_pTgaFileHandle, m_llFilePos, &llMoveDistance, FILE_BEGIN); // �˻ض�ȡ������
	}
	m_dwNXRLCBufCapcity = 0;
#endif
	return TRUE;

}

int	CTgaFile::TGARLCOneValue(RGBBIT& stDstClr,BYTE* pSrc, LONGLONG& llSrcByteIdx,LONGLONG llSrcSize,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount)
{
	unsigned long ulValue = 0;
	stDstClr.RGBdword = 0;
	switch (lSrcBits)
	{
	case 8:
		ulValue = (unsigned long)pSrc[llSrcByteIdx];
		llSrcByteIdx += 1;
		break;
	case 16:
		ulValue = (unsigned long) (*(WORD*)(pSrc + llSrcByteIdx));
		llSrcByteIdx += 2;
		break;
	case 24:
		((BYTE*)(&ulValue))[1] = pSrc[llSrcByteIdx + 0];
		((BYTE*)(&ulValue))[2] = pSrc[llSrcByteIdx + 1];
		((BYTE*)(&ulValue))[3] = pSrc[llSrcByteIdx + 2];
		ulValue = ulValue >> 8;
		llSrcByteIdx += 3;
		break;
	case 32:
		ulValue =  (unsigned long) (*(DWORD*)(pSrc + llSrcByteIdx));
		llSrcByteIdx += 4;
		break;
	default:
		return 0;
		break;
	}
	if (NULL != pClrMap)//����ɫ����Ҫ���
	{
		if (ulValue < (unsigned long)lClrMapCount)
		{
			stDstClr.RGBbits.rgbBlue = pClrMap[ulValue].rgbBlue;
			stDstClr.RGBbits.rgbGreen =pClrMap[ulValue].rgbGreen;
			stDstClr.RGBbits.rgbRed = pClrMap[ulValue].rgbRed;
			stDstClr.RGBbits.rgbReserved =pClrMap[ulValue].rgbReserved;
		}
	}
	else//����ɫ��
	{
		switch (lSrcBits)
		{
		case 8://û��8λ�����
			return FALSE;
			break;
		case 16:
			stDstClr.RGBbits.rgbBlue = (ulValue & 0x0000001F)<<3;//ȡ����ɫֵ������ֱ�����ŵ�32λ
			stDstClr.RGBbits.rgbGreen = (ulValue & 0x000003E0)>>2;
			stDstClr.RGBbits.rgbRed = (ulValue & 0x00007C00)>>7;
			stDstClr.RGBbits.rgbReserved = 0xff;//(ulValue & 0x00008000) ? 0xFF : 0; // 16 λʱ������alpha��Ϣ
			break;
		case 24:
			stDstClr.RGBdword = (DWORD)ulValue;
			stDstClr.RGBbits.rgbReserved = 0xff;
			break;
		case 32:
			stDstClr.RGBdword = (DWORD)ulValue;
			break;
		default:
			return 0;
			break;
		}
	}
	if (llSrcByteIdx >= llSrcSize)
	{
		return -1;//�Ѿ�Խ��
	}
	return 1;
}

BOOL	CTgaFile::WriteTGAImage(
							IN  IMAGE_INFO	ImageInfo,
							IN  DWORD*		dbuf,
							BYTE* pCustomData, long lCustomDataSize)
{
	if (NULL == m_pTgaFileHandle)
	{
		return FALSE;
	}

	if (m_llFilePos.QuadPart != 0)
	{
		LARGE_INTEGER llMoveDistance;
		llMoveDistance.QuadPart = 0;
		SetFilePointerEx(m_pTgaFileHandle, llMoveDistance, NULL, FILE_BEGIN);
		m_llFilePos.QuadPart = 0;
	}
	m_cTGAHead.Identsize=(BYTE)lCustomDataSize;
	m_cTGAHead.Xstart	=(WORD)ImageInfo.Xstart;
	m_cTGAHead.Ystart	=(WORD)ImageInfo.Ystart;
	m_cTGAHead.Width	=(WORD)ImageInfo.Width;
	m_cTGAHead.Height	=(WORD)ImageInfo.Height;

	switch(ImageInfo.bits)
	{
	case 16:
		return FALSE;
		break;
	case 24:
		return FALSE;
		break;
	case 32:
		m_cTGAHead.Colormaptype	=0;
		m_cTGAHead.Colormapstart	=0;
		m_cTGAHead.Colormaplength	=0;
		m_cTGAHead.Colormapbits	=0;
		m_cTGAHead.Bits			=32;
		m_cTGAHead.ImgDescriptor	=0x28;//���Ͻǣ�32λTGA
		if (ImageInfo.fCompression)//ѹ��
		{
		}
		else//��ѹ��
		{
			m_cTGAHead.Imagetype	=2;//��ѹ������ɫ��
			DWORD dwRealWriteBytes = 0;
			WriteFile(m_pTgaFileHandle, &m_cTGAHead,sizeof(TGA_HEAD), &dwRealWriteBytes, NULL);
			m_llFilePos.QuadPart += sizeof(TGA_HEAD);
			if (pCustomData && lCustomDataSize > 0)
			{
				WriteFile(m_pTgaFileHandle, pCustomData,lCustomDataSize, &dwRealWriteBytes, NULL);
				m_llFilePos.QuadPart += lCustomDataSize;
			}
			//For DDN written limitation
#ifdef TP_NS_NET
			DWORD dwBlockSize = g_dwBlockSize; // �����ִ�зֿ�д��

			DWORD Length = 4*m_cTGAHead.Width*m_cTGAHead.Height;
			DWORD dwWritten = 0L;
			DWORD dwToWrite = dwWritten;
			DWORD dwLeft = Length;
			LPDWORD pBufPos = dbuf;
			while ( dwWritten < Length )
			{
				dwLeft = Length - dwWritten;
				dwToWrite = min( dwLeft, dwBlockSize );

				WriteFile(m_pTgaFileHandle, pBufPos, dwToWrite, &dwRealWriteBytes, NULL);
				FlushFileBuffers(m_pTgaFileHandle);
				pBufPos += (dwToWrite >> 2); // convert BYTE to DWORD
				dwWritten += dwToWrite;
			}
#else
			WriteFile(m_pTgaFileHandle, dbuf, 4*m_cTGAHead.Width*m_cTGAHead.Height, &dwRealWriteBytes, NULL);
#endif
			m_llFilePos.QuadPart += 4*m_cTGAHead.Width*m_cTGAHead.Height;
		}
	}
	FlushFileBuffers(m_pTgaFileHandle);
	return TRUE;
}

//BOOL	CTgaFile::GetTGAInfo()
//{
//	if (NULL == m_pFile)
//	{
//		return FALSE;
//	}
//	if(m_llFilePos > 0)
//	{
//		m_pFile->SeekToBegin();
//		m_llFilePos = 0;
//	}
//	long lReadSize = m_pFile->Read(&m_cTGAHead,sizeof(TGA_HEAD));
//	if(sizeof(TGA_HEAD) > lReadSize)
//	{
//		return FALSE;
//	}
//	m_llFilePos += lReadSize;
//	m_stImageInfo.Xstart=m_cTGAHead.Xstart;
//	m_stImageInfo.Ystart=m_cTGAHead.Ystart;
//	m_stImageInfo.Width =m_cTGAHead.Width;
//	m_stImageInfo.Height=m_cTGAHead.Height;
//	m_stImageInfo.bits	=m_cTGAHead.Bits;
//	m_stImageInfo.llFileSize = m_pFile->GetLength();
//	if (NULL != m_pCustomData)
//	{
//		delete []m_pCustomData;
//		m_pCustomData = NULL;
//		m_lCustomDataSize = 0;
//	}
//	if (m_cTGAHead.Identsize > 0)
//	{
//		m_lCustomDataSize = m_cTGAHead.Identsize;
//		m_pCustomData = new BYTE[m_lCustomDataSize];
//		lReadSize = m_pFile->Read(m_pCustomData,m_lCustomDataSize);
//		m_llFilePos += lReadSize;
//		if(m_lCustomDataSize > lReadSize)
//		{
//			delete []m_pCustomData;
//			m_pCustomData = NULL;
//			m_lCustomDataSize = 0;
//			return FALSE;
//		}
//	}
//	return TRUE;
//
//}
BOOL	CTgaFile::GetTGAInfo()
{
	if (NULL == m_pTgaFileHandle)
	{
		return FALSE;
	}
	if(m_llFilePos.QuadPart > 0)
	{
		LARGE_INTEGER llMoveDistance;
		llMoveDistance.QuadPart = 0;
		SetFilePointerEx(m_pTgaFileHandle, llMoveDistance, &m_llFilePos, FILE_BEGIN);
	}
	DWORD dwRealReadBytes = 0;
	ReadFile(m_pTgaFileHandle, &m_cTGAHead,sizeof(TGA_HEAD), &dwRealReadBytes, NULL);
	if(sizeof(TGA_HEAD) > dwRealReadBytes)
	{
		return FALSE;
	}
	m_llFilePos.QuadPart += dwRealReadBytes;
	m_stImageInfo.Xstart=m_cTGAHead.Xstart;
	m_stImageInfo.Ystart=m_cTGAHead.Ystart;
	m_stImageInfo.Width =m_cTGAHead.Width;
	m_stImageInfo.Height=m_cTGAHead.Height;
	m_stImageInfo.bits	=m_cTGAHead.Bits;
	LARGE_INTEGER llTgaFileSize;
	GetFileSizeEx(m_pTgaFileHandle, &llTgaFileSize);
	m_stImageInfo.llFileSize = llTgaFileSize.QuadPart;
	if (NULL != m_pCustomData)
	{
		delete []m_pCustomData;
		m_pCustomData = NULL;
		m_lCustomDataSize = 0;
	}
	if (m_cTGAHead.Identsize > 0)
	{
		m_lCustomDataSize = m_cTGAHead.Identsize;
		m_pCustomData = new BYTE[m_lCustomDataSize];
		ReadFile(m_pTgaFileHandle, m_pCustomData,m_lCustomDataSize, &dwRealReadBytes, NULL);
		m_llFilePos.QuadPart += dwRealReadBytes;
		if(m_lCustomDataSize > (long)dwRealReadBytes)
		{
			delete []m_pCustomData;
			m_pCustomData = NULL;
			m_lCustomDataSize = 0;
			return FALSE;
		}
	}
	return TRUE;

}
BOOL	CTgaFile::GetTGAInfo(IMAGE_INFO	&ImageInfo, BYTE*& pBuf, long& lBufSize)
{
	//if (NULL == m_pFile)
	//{
	//	//�ļ���û�д�
	//	return FALSE;
	//}
	if (NULL == m_pTgaFileHandle)
	{
		//�ļ���û�д�
		return FALSE;
	}
	TP_MemcpyLine(&ImageInfo,&m_stImageInfo,sizeof(IMAGE_INFO));
	pBuf = m_pCustomData;
	lBufSize = m_lCustomDataSize;
	return TRUE;
}

BOOL	CTgaFile::UpdateCustomData(BYTE* pCustomData, long lCustomDataSize)
{
	if (NULL == m_pTgaFileHandle || NULL == pCustomData || lCustomDataSize <= 0 || lCustomDataSize > 256)
	{
		return FALSE;
	}
	LONGLONG lImgDataSize = m_stImageInfo.llFileSize - sizeof(TGA_HEAD) - m_cTGAHead.Identsize;
	ASSERT(lImgDataSize > 0);

	BYTE* pBufRead = new BYTE[(UINT)lImgDataSize];
	if (m_llFilePos.QuadPart != m_cTGAHead.Identsize + sizeof(TGA_HEAD))
	{
		LARGE_INTEGER llMoveDistance;
		llMoveDistance.QuadPart = m_cTGAHead.Identsize + sizeof(TGA_HEAD);
		SetFilePointerEx(m_pTgaFileHandle, llMoveDistance, NULL, FILE_BEGIN);
	}
	DWORD dwRealReadBytes = 0;
	ReadFile(m_pTgaFileHandle, pBufRead,(UINT)lImgDataSize, &dwRealReadBytes, NULL);
	m_cTGAHead.Identsize = (BYTE)lCustomDataSize;
	LARGE_INTEGER llMoveDistance;
	llMoveDistance.QuadPart = 0;
	SetFilePointerEx(m_pTgaFileHandle, llMoveDistance, NULL, FILE_BEGIN);
	DWORD dwRealWriteBytes = 0;
	WriteFile(m_pTgaFileHandle, &m_cTGAHead,sizeof(TGA_HEAD), &dwRealWriteBytes, NULL);
	WriteFile(m_pTgaFileHandle, pCustomData,lCustomDataSize, &dwRealWriteBytes, NULL);
	WriteFile(m_pTgaFileHandle, pBufRead,(UINT)lImgDataSize, &dwRealWriteBytes, NULL);
	m_stImageInfo.llFileSize = sizeof(TGA_HEAD) + m_cTGAHead.Identsize + lImgDataSize;
	m_llFilePos.QuadPart = m_stImageInfo.llFileSize;
	delete []pBufRead;
	return TRUE;
}