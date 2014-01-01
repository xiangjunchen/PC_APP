#include "stdafx.h"
#include "TgaFile.h"
#include "TPAVTgaFile.h"
///////////////////////////////////////////////////////////
/////////////////////TAG 类////////////////////////////////

// 缓存TGA图像的RLC数据，暂时定为 400K add by wgf @2010-4-26
#define NX_TGA_RLC_BUF_SIZE 409600

//#define _PPPPP_   1//优化开关

extern void tga_cr16_to_cr32_line(OUT unsigned char *pucRGB32,		// 32 位输出
								  IN unsigned char *pucRGB16,		// 16 位输入 arrrrrgg gggbbbbb
								  IN int nLineCount);				// 一行处理的个数

extern unsigned char* tga_rlc_line(OUT unsigned char *pucDes,		// 解压输出目标
								   IN unsigned char *pucSrc,		// 压缩数据源
								   IN int nCellBits,				// 编码单元位数
								   IN int nLineCount);				// 一行单元个数

extern void tga_palette32_index8_line(OUT unsigned char *pucRGB32,		// 32 位输出
							   IN unsigned char *pucIndex8,		// 8 位颜色表索引
							   IN int nLineCount,				// 索引个数
							   IN unsigned char *pucPalette);	// 32 位颜色表


extern void tga_palette32_index16_line(OUT unsigned char *pucRGB32,	// 32 位输出
								IN unsigned char *pucIndex16,	// 16 位颜色表索引
								IN int nLineCount,				// 索引个数
								IN unsigned char *pucPalette);	// 32 位颜色表

extern void tga_palette32_line(OUT unsigned char *pucRGB32,		// 32 位输出
							   IN unsigned char *pucIndex,			// 颜色表索引
							   IN int nIndexBits,					// 索引位数，8/16
							   IN int nLineCount,					// 索引个数
							   IN unsigned char *pucPalette);		// 32 位颜色表


// ##### Start by wanggaofei @ 2010/04/28 18:41 #####
// Comments: 去掉 CFile ，改为 ReadFile 等 API 来进行文件操作，降低对MFC的依赖，提高操作文件的灵活性。

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
	DWORD dwCreateDescription = OPEN_EXISTING; // 尝试打开文件
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

	if (!(dwOpenType & TP_IMG_FILE_CREATE))//如果不是新建文件，把文件头读入
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
//	//	//已经有了，直接返回
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
//	//判断是否有调色板
//	if (m_cTGAHead.Colormaptype == 1)//有调色板
//	{
//		//读取调色板
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

	//判断是否有调色板
	if (m_cTGAHead.Colormaptype == 1)//有调色板
	{
		//读取调色板
		ReadClrMap();
	}

	ReadImage();
	return TRUE;
}


// 返回图像是否含有 alpha 信息
BOOL	CTgaFile::_NXHaveAlpha(void)
{
	if(m_cTGAHead.Colormaptype != 0)
	{
		return FALSE; // 含有调色板，不含有 alpaha
	}

	if(m_cTGAHead.Imagetype != 2 && m_cTGAHead.Imagetype != 10)
	{
		// 图像数据不是未压缩或RLE数据 true color
		return FALSE; // 不含有 alpaha
	}

	if(m_cTGAHead.Bits != 32) // 32 位图像才含有 alpha
	{
		return FALSE; // 不含有 alpaha
	}

	if((m_cTGAHead.ImgDescriptor & 0x0f) == 0)
	{
		return FALSE; // alpha 位数为 0
	}

	return TRUE;
}

BOOL CTgaFile::_NXSupportBlockRead(void) // 图像是否可以分块读取 by ljj@2011-4-28
{
	//只有无压缩有或者无颜色表图像可以支持.其余不支持.
	if (m_cTGAHead.Imagetype == 1 || m_cTGAHead.Imagetype == 2)
		return TRUE;
	return FALSE;
}
// 得到图象数据是否从左下角开始
BOOL	CTgaFile::_NXImageBeginLeftBottom(void)
{
	BYTE byLocFlag = (m_cTGAHead.ImgDescriptor >> 4)&0x03;//图像原点位置标志：0：左下角；2：左上角
	if(byLocFlag == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
// 分块读取图像数据
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

	//判断是否有调色板
	if (m_cTGAHead.Colormaptype == 1)//有调色板
	{
		//读取调色板
		ReadClrMap();
	}

	return TRUE;
}
// 分块读取图像数据
//读取颜色表,只在LoadImage里面调用，前面应该保证文件有效，及有调色板，及文件指针已经移动到调色板处
BOOL CTgaFile::ReadClrMap()
{
	//调色板个数：
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
	//转换颜色表到32位颜色表
	switch (m_cTGAHead.Colormapbits)
	{
	case 16://arrrrrgg gggbbbbb
		for (int i=0;i<m_cTGAHead.Colormaplength;i++)
		{
			m_pClrMap[i].rgbBlue = ((((WORD*)pTempBuf)[i]) & 0x001F)<<3;//取出颜色值，并且直接缩放到32位
			m_pClrMap[i].rgbGreen = ((((WORD*)pTempBuf)[i]) & 0x03E0)>>2;
			m_pClrMap[i].rgbRed = ((((WORD*)pTempBuf)[i]) & 0x7C00)>>7;
			m_pClrMap[i].rgbReserved = 0xff;//((((WORD*)pTempBuf)[i]) & 0x8000) ? 0xFF : 0; // 16 位不含有alpha信息
		}
		break;
	case 24://B，G，R
		for (int i=0;i<m_cTGAHead.Colormaplength;i++)
		{
			TP_MemcpyLine((BYTE*)(&(m_pClrMap[i])), pTempBuf + 3*i, 3);
			m_pClrMap[i].rgbReserved = 0xff;
		}
		break;
	case 32://B，G，R，A
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
//上层调用，必须保证当前文件指针和文件读取位置的正确性
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
	BYTE byLocFlag = (m_cTGAHead.ImgDescriptor >> 4)&0x03;//图像原点位置标志：0：左下角；2：左上角
	DWORD dwRealReadBytes = 0;
	switch (m_cTGAHead.Imagetype)
	{
	case 1://无压缩颜色表图像
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
		//根据颜色表，填充目标Buffer
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
	case 2://无压缩无颜色表图像
		switch (m_cTGAHead.Bits)
		{
		case 16://16位: arrrrrgg gggbbbbb
			//读入
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

			if (!Tga16To32(m_pImgBuf,pTmpBuf,m_cTGAHead.Width,m_cTGAHead.Height,byLocFlag))//把16位转换为32位
			{
				delete []pTmpBuf;
//				delete []m_pImgBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			delete []pTmpBuf;
			break;
		case 24:
			//直接读入，Alpha设为0xff
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
			if (byLocFlag == 0)//左下角
			{
				TPCS_24_to_32_Frame((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,pTmpBuf,m_cTGAHead.Width*3,NULL,0,m_cTGAHead.Width,m_cTGAHead.Height,TRUE,TRUE);
			}
			else if (byLocFlag == 2)//左上角，不需要反转
			{
				TPCS_24_to_32_Frame((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,pTmpBuf,m_cTGAHead.Width*3,NULL,0,m_cTGAHead.Width,m_cTGAHead.Height,FALSE,FALSE);
			}
			else 
			{
				//其他未知
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
			DWORD dwBlockSize = g_dwBlockSize; // 网络版执行分块写入
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
			//直接读入
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
			if (byLocFlag == 0)//左下角，反转
			{
				TP_OverTurnSelf((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,m_cTGAHead.Height);
			}
			else if (byLocFlag == 2)//左上角，不需要反转
			{
			}
			else//其他，不处理
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
	case 9://RLC,带颜色表
	case 10://RLC，不带颜色表
		//读出数据，送RLC解码函数
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
		//送RLC解码函数
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

// 分块读取图像数据
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
	BYTE byLocFlag = (m_cTGAHead.ImgDescriptor >> 4)&0x03;//图像原点位置标志：0：左下角；2：左上角
	DWORD dwRealReadBytes = 0;
#ifdef TP_NS_NET // 局部变量定义移出switch
	DWORD dwLength = lImgDataSize;
	DWORD dwRead = 0L;
	DWORD dwToRead = dwRead;
	DWORD dwLeft = dwLength;
	LPBYTE pBufPos = (LPBYTE)m_pImgBuf;
	HKEY hKey = NULL;
	DWORD dwBlockSize = 0L; // 网络版执行分块写入
	DWORD dwValueSize = REG_DWORD;
	DWORD dwValueType = REG_DWORD;
#endif
	switch (m_cTGAHead.Imagetype)
	{
	case 1://无压缩颜色表图像
		pTmpBuf = new BYTE[lImgDataSize]; // 需要读取的图像数据
		ReadFile(m_pTgaFileHandle, pTmpBuf, lImgDataSize, &dwRealReadBytes, NULL);
		if (lImgDataSize > dwRealReadBytes)
		{
			m_llFilePos.QuadPart += lImgDataSize;
			delete []pTmpBuf;
			m_pImgBuf = NULL;
			return FALSE;
		}
		m_llFilePos.QuadPart += lImgDataSize;
		//根据颜色表，填充目标Buffer
		if (!FillDstBufByClrMap(m_pClrMap,m_cTGAHead.Colormaplength,(BYTE*)m_pImgBuf,32,pTmpBuf,m_cTGAHead.Bits,m_cTGAHead.Width,iRealReadImageHeight,byLocFlag))
		{
			delete []pTmpBuf;
			m_pImgBuf = NULL;
			return FALSE;
		}
		delete []pTmpBuf;
		break;
	case 2://无压缩无颜色表图像
		switch (m_cTGAHead.Bits)
		{
		case 16://16位: arrrrrgg gggbbbbb
			//读入
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

			if (!Tga16To32(m_pImgBuf,pTmpBuf,m_cTGAHead.Width,iRealReadImageHeight,byLocFlag))//把16位转换为32位
			{
				delete []pTmpBuf;
				m_pImgBuf = NULL;
				return FALSE;
			}
			delete []pTmpBuf;
			break;
		case 24:
			//直接读入，Alpha设为0xff
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
			if (byLocFlag == 0)//左下角
			{
				TPCS_24_to_32_Frame((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,pTmpBuf,m_cTGAHead.Width*3,NULL,0,m_cTGAHead.Width,iRealReadImageHeight,TRUE,TRUE);
			}
			else if (byLocFlag == 2)//左上角，不需要反转
			{
				TPCS_24_to_32_Frame((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,pTmpBuf,m_cTGAHead.Width*3,NULL,0,m_cTGAHead.Width,iRealReadImageHeight,FALSE,FALSE);
			}
			else 
			{
				//其他未知
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
			//直接读入
			ReadFile(m_pTgaFileHandle, m_pImgBuf, lImgDataSize, &dwRealReadBytes, NULL);
			if (lImgDataSize > dwRealReadBytes)
			{
				m_llFilePos.QuadPart += lImgDataSize;
				m_pImgBuf = NULL;
				return FALSE;
			}
#endif
			m_llFilePos.QuadPart += lImgDataSize;
			if (byLocFlag == 0)//左下角，反转
			{
				TP_OverTurnSelf((BYTE*)m_pImgBuf,m_cTGAHead.Width*4,iRealReadImageHeight);
			}
			else if (byLocFlag == 2)//左上角，不需要反转
			{
			}
			else//其他，不处理
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
	case 9://RLC,带颜色表
	case 10://RLC，不带颜色表
		//读出数据，送RLC解码函数
		if(NULL == m_pNXRLCBuf)
		{
			m_pNXRLCBuf = new BYTE[NX_TGA_RLC_BUF_SIZE ]; // 申请缓冲池
			TP_MemsetChar(m_pNXRLCBuf, 0, NX_TGA_RLC_BUF_SIZE);
		}
		ReadFile(m_pTgaFileHandle, m_pNXRLCBuf, NX_TGA_RLC_BUF_SIZE, &m_dwNXRLCBufCapcity, NULL);
		if(m_dwNXRLCBufCapcity == 0)
		{
			m_pImgBuf = NULL;
			return FALSE;
		}
		m_llFilePos.QuadPart += m_dwNXRLCBufCapcity;
		//送RLC解码函数
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

BOOL	CTgaFile::Tga16To32(DWORD* pDst, BYTE* pSrc, long lWidth, long lHeight, BYTE byLocFlag)//把无压缩16位转换为32位
{
	if (byLocFlag == 0)//左下角，需要反转
	{
		for (int y = lHeight - 1; y >= 0 ; y--)
		{
#if _PPPPP_ //这个优化还是很有效的，800*600的：不优化：6.5ms，优化：1.5ms
			tga_cr16_to_cr32_line((unsigned char*)(pDst + (lHeight - 1 - y) * lWidth) , (unsigned char*)(pSrc + y * lWidth * 2), lWidth);
#else	
			RGBBIT stTmpClr;
			for (int x = 0; x < lWidth; x++)
			{
				//对每个象素，拆分到32位
				long lIdx = y*lWidth + x;
				stTmpClr.RGBbits.rgbBlue = ((((WORD*)pSrc)[lIdx]) & 0x001F)<<3;//取出颜色值，并且直接缩放到32位
				stTmpClr.RGBbits.rgbGreen = ((((WORD*)pSrc)[lIdx]) & 0x03E0)>>2;
				stTmpClr.RGBbits.rgbRed = ((((WORD*)pSrc)[lIdx]) & 0x7C00)>>7;
				stTmpClr.RGBbits.rgbReserved = 0xff;//((((WORD*)pSrc)[lIdx]) & 0x8000) ? 0xFF : 0;
				pDst[ (lHeight - 1 - y) * lWidth + x]=(DWORD)stTmpClr.RGBdword;
			}
#endif
		}
	}
	else if (byLocFlag == 2)//左上角，不需要反转
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
			//对每个象素，拆分到32位
			stTmpClr.RGBbits.rgbBlue = ((((WORD*)pSrc)[m]) & 0x001F)<<3;//取出颜色值，并且直接缩放到32位
			stTmpClr.RGBbits.rgbGreen = ((((WORD*)pSrc)[m]) & 0x03E0)>>2;
			stTmpClr.RGBbits.rgbRed = ((((WORD*)pSrc)[m]) & 0x7C00)>>7;
			stTmpClr.RGBbits.rgbReserved = 0xff;//((((WORD*)pSrc)[m]) & 0x8000) ? 0xFF : 0;
			pDst[m]=(DWORD)stTmpClr.RGBdword;
		}
#endif
	}
	else 
	{
		//其他未知
		return FALSE;
	}
	return TRUE;
}


//由调用者保证参数的正确和合理性
BOOL	CTgaFile::FillDstBufByClrMap(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, long lDstBits,BYTE* pSrc,long lSrcBits,long lWidth, long lHeight,BYTE byLocFlag)
{
	ASSERT(lDstBits == 32);
	if (lDstBits != 32)
	{
		return FALSE;
	}
#if _PPPPP_
	if (byLocFlag == 0)//左下角，需要反转
	{
		for (int y = lHeight - 1; y >= 0 ; y--)
		{
			tga_palette32_line((unsigned char*)(pDst + (lHeight - 1 - y)*lWidth * 4),(unsigned char*)(pSrc + y * lWidth * lSrcBits / 8),lSrcBits, lWidth ,(unsigned char*)pClrMap);
		}
	}
	else if (byLocFlag == 2)//左上角，不需要反转
	{
		for (int y = 01; y < lHeight ; y++)
		{
			tga_palette32_line((unsigned char*)(pDst + y*lWidth * 4),(unsigned char*)(pSrc + y * lWidth * lSrcBits / 8),lSrcBits, lWidth  ,(unsigned char*)pClrMap);
		}
	}
	else//其他类型，不做处理 
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
	return TRUE; // 正常返回
}

//源索引是8位
BOOL	CTgaFile::FillDstBufByClrMap8(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag)
{
	long lIdx = 0;
	unsigned long lClrIdx = 0;
	if (byLocFlag == 0)//左下角，需要反转
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
	else if (byLocFlag == 2)//左上角，不需要反转
	{
		for (int m = 0; m < m_cTGAHead.Width*m_cTGAHead.Height; m++)
		{
			
			lClrIdx = (unsigned long)(pSrc[m]);
			if (lClrIdx < ulClrMapCount)
				TP_MemcpyLine((pDst+4*m),(BYTE*)(&m_pClrMap[lClrIdx]),4);
		}

	}
	else//其他类型，不做处理 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL	CTgaFile::FillDstBufByClrMap16(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag)
{
	long lIdx = 0;
	unsigned long lClrIdx = 0;
	if (byLocFlag == 0)//左下角，需要反转
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
	else if (byLocFlag == 2)//左上角，不需要反转
	{
		for (int m = 0; m < m_cTGAHead.Width*m_cTGAHead.Height; m++)
		{

			lClrIdx = (unsigned long)(((WORD*)pSrc)[m]);
			if (lClrIdx < ulClrMapCount)
				TP_MemcpyLine((pDst+4*m),(BYTE*)(&m_pClrMap[lClrIdx]),4);
		}

	}
	else//其他类型，不做处理 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL	CTgaFile::FillDstBufByClrMap24(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag)
{
	long lIdx = 0;
	unsigned long lClrIdx = 0;
	if (byLocFlag == 0)//左下角，需要反转
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
	else if (byLocFlag == 2)//左上角，不需要反转
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
	else//其他类型，不做处理 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL	CTgaFile::FillDstBufByClrMap32(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag)
{
	long lIdx = 0;
	unsigned long lClrIdx = 0;
	if (byLocFlag == 0)//左下角，需要反转
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
	else if (byLocFlag == 2)//左上角，不需要反转
	{
		for (int m = 0; m < m_cTGAHead.Width*m_cTGAHead.Height; m++)
		{

			lClrIdx = (unsigned long)(((DWORD*)pSrc)[m]);
			if (lClrIdx < ulClrMapCount)
				TP_MemcpyLine((pDst+4*m),(BYTE*)(&m_pClrMap[lClrIdx]),4);
		}

	}
	else//其他类型，不做处理 
	{
		return FALSE;
	}
	return TRUE;
}

//有无颜色表；不同位数，都可处理
BOOL	CTgaFile::TGARLCDec(DWORD* pDst, BYTE* pSrc, LONGLONG llSrcSize,long lWidth,long lHeight,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount,BYTE byLocFlag)//RLC解码,目标为32位
{
#if 0 // ljj:还不如不优化...不优化：720*576的16ms，优化：21ms。
	if (NULL == pClrMap && lSrcBits == 32) 
	{
		//不带颜色版,32位，不需要反转，则直接把Dst送入。
		tga_rlc_line((unsigned char*)pDst,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (byLocFlag == 0) 
		{
			//需要反转
			TP_OverTurnSelf((BYTE*)pDst,lWidth*4,lHeight);
		}
		return TRUE;
	}
	if (NULL == pClrMap) 
	{
		//不带颜色板
		BYTE* ptmp = new BYTE[lWidth*lHeight*lSrcBits/8];
		tga_rlc_line((unsigned char*)ptmp,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (lSrcBits == 16) 
		{
			if (byLocFlag == 0)//左下角，需要反转
			{
				for (long ldx = lHeight -1; ldx >= 0; ldx --)
				{
					tga_cr16_to_cr32_line((unsigned char*)(pDst + (lHeight - 1 - ldx)*lWidth),(unsigned char*)(ptmp + ldx*lWidth*2),lWidth);
				}
			}
			else if (byLocFlag == 2)//左上角，无需反转
			{
				for (long ldx = 0; ldx < lHeight; ldx ++)
				{
					tga_cr16_to_cr32_line((unsigned char*)(pDst + ldx*lWidth),(unsigned char*)(pSrc + ldx*lWidth*2),lWidth);
				}
			}
			else//其他，不处理，不会有其他
			{
				delete []ptmp;
				return FALSE;
			}
		}
		else if (lSrcBits == 24) 
		{
			//调用24到32的变换
			if (byLocFlag == 0)//左下角，需要反转
			{
				TPCS_24_to_32_Frame((BYTE*)pDst,lWidth*4,ptmp,lWidth*3,NULL,0,lWidth,lHeight,TRUE,TRUE);
			}
			else if (byLocFlag == 2)//左上角，无需反转
			{
				TPCS_24_to_32_Frame((BYTE*)pDst,lWidth*4,ptmp,lWidth*3,NULL,0,lWidth,lHeight,FALSE,FALSE);
			}
			else//其他，不处理，不会有其他
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
		if (byLocFlag == 0)//左下角，需要反转
		{
			for (long ldx = lHeight -1; ldx >= 0; ldx --)
			{
				tga_palette32_line((unsigned char*)(pDst + (lHeight - 1 - ldx)*lWidth),(unsigned char*)(ptmp + ldx*lWidth*lSrcBits/8),lSrcBits,lWidth*lSrcBits/8,(unsigned char*)pClrMap);
			}
		}
		else if (byLocFlag == 2)//左上角，无需反转
		{
			for (long ldx = 0; ldx < lHeight; ldx ++)
			{
				tga_palette32_line((unsigned char*)(pDst + ldx*lWidth),(unsigned char*)(ptmp + ldx*lWidth*lSrcBits/8),lSrcBits,lWidth*lSrcBits/8,(unsigned char*)pClrMap);
			}
		}
		else//其他，不处理，不会有其他
		{
			delete []ptmp;
			return FALSE;
		}
		delete []ptmp;
	}

#else

	unsigned long	ulPixelIdx = 0;//目标的像素位置
	LONGLONG	llSrcByteIdx = 0;//压缩数据字节位置
	RGBBIT 	stDstClr;//从解压后的数据中得到的目标的颜色值
	int iRes = 0;
	try
	{
		while((llSrcByteIdx < llSrcSize)&&(ulPixelIdx < (unsigned long)(lWidth*lHeight) ))//源和目标都没有完成
		{
			BYTE	byRepeat = 1;
			if((pSrc[llSrcByteIdx]&0x80)!=0) //RunLength数据包，头一个字节是循环次数N，后面跟一个索引值或者颜色值，意为这个索引值或颜色值重复N次。
			{
				byRepeat=(pSrc[llSrcByteIdx]&0x7f)+1;//循环次数N
				llSrcByteIdx += 1;//指向下一个字节
				iRes = TGARLCOneValue(stDstClr,pSrc,llSrcByteIdx,llSrcSize,lSrcBits,pClrMap,lClrMapCount);
				if (0 == iRes)
				{
					return FALSE;
				}
				else if (-1 == iRes)
				{
					break; //已经解码越界，停止解码，并保留已解码的部分。
				}
				if (byLocFlag == 0)//左下角，需要反转
				{
					for(BYTE i=0;i<byRepeat;i++)
					{
						pDst[(lHeight - (ulPixelIdx / lWidth) -1) * lWidth + (ulPixelIdx % lWidth)] = (DWORD)stDstClr.RGBdword;
						ulPixelIdx += 1;
					}
				}
				else if (byLocFlag == 2)//左上角，无需反转
				{
					for(BYTE i=0;i<byRepeat;i++)
					{
						pDst[ulPixelIdx] = (DWORD)stDstClr.RGBdword;
						ulPixelIdx += 1;
					}
				}
				else//其他，不处理，不会有其他
				{
					return FALSE;
				}
			}
			else //无处理数据包，第一个字节是包头，含有循环次数N，表示后续N个值是索引值或颜色值。这N个值只循环1次。
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
						break; //已经解码越界，停止解码，并保留已解码的部分。
					}
					if (byLocFlag == 0)//左下角，需要反转
					{
						pDst[(lHeight - (ulPixelIdx / lWidth) -1) * lWidth + (ulPixelIdx % lWidth)] = (DWORD)stDstClr.RGBdword;
						ulPixelIdx += 1;
					}
					else if (byLocFlag == 2)//左上角，无需反转
					{
						pDst[ulPixelIdx] = (DWORD)stDstClr.RGBdword;
						ulPixelIdx += 1;
					}
					else//其他，不处理，不会有其他
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

BOOL	CTgaFile::NXTGARLCDec(DWORD* pDst, BYTE* pSrc, LONGLONG llSrcSize,long lWidth,long lHeight,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount,BYTE byLocFlag)//RLC解码,目标为32位
{
#if 0 // ljj:还不如不优化...不优化：720*576的16ms，优化：21ms。
	if (NULL == pClrMap && lSrcBits == 32) 
	{
		//不带颜色版,32位，不需要反转，则直接把Dst送入。
		tga_rlc_line((unsigned char*)pDst,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (byLocFlag == 0) 
		{
			//需要反转
			TP_OverTurnSelf((BYTE*)pDst,lWidth*4,lHeight);
		}
		return TRUE;
	}
	if (NULL == pClrMap) 
	{
		//不带颜色板
		BYTE* ptmp = new BYTE[lWidth*lHeight*lSrcBits/8];
		tga_rlc_line((unsigned char*)ptmp,(unsigned char*)pSrc, lSrcBits,lWidth*lHeight);
		if (lSrcBits == 16) 
		{
			if (byLocFlag == 0)//左下角，需要反转
			{
				for (long ldx = lHeight -1; ldx >= 0; ldx --)
				{
					tga_cr16_to_cr32_line((unsigned char*)(pDst + (lHeight - 1 - ldx)*lWidth),(unsigned char*)(ptmp + ldx*lWidth*2),lWidth);
				}
			}
			else if (byLocFlag == 2)//左上角，无需反转
			{
				for (long ldx = 0; ldx < lHeight; ldx ++)
				{
					tga_cr16_to_cr32_line((unsigned char*)(pDst + ldx*lWidth),(unsigned char*)(pSrc + ldx*lWidth*2),lWidth);
				}
			}
			else//其他，不处理，不会有其他
			{
				delete []ptmp;
				return FALSE;
			}
		}
		else if (lSrcBits == 24) 
		{
			//调用24到32的变换
			if (byLocFlag == 0)//左下角，需要反转
			{
				TPCS_24_to_32_Frame((BYTE*)pDst,lWidth*4,ptmp,lWidth*3,NULL,0,lWidth,lHeight,TRUE,TRUE);
			}
			else if (byLocFlag == 2)//左上角，无需反转
			{
				TPCS_24_to_32_Frame((BYTE*)pDst,lWidth*4,ptmp,lWidth*3,NULL,0,lWidth,lHeight,FALSE,FALSE);
			}
			else//其他，不处理，不会有其他
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
		if (byLocFlag == 0)//左下角，需要反转
		{
			for (long ldx = lHeight -1; ldx >= 0; ldx --)
			{
				tga_palette32_line((unsigned char*)(pDst + (lHeight - 1 - ldx)*lWidth),(unsigned char*)(ptmp + ldx*lWidth*lSrcBits/8),lSrcBits,lWidth*lSrcBits/8,(unsigned char*)pClrMap);
			}
		}
		else if (byLocFlag == 2)//左上角，无需反转
		{
			for (long ldx = 0; ldx < lHeight; ldx ++)
			{
				tga_palette32_line((unsigned char*)(pDst + ldx*lWidth),(unsigned char*)(ptmp + ldx*lWidth*lSrcBits/8),lSrcBits,lWidth*lSrcBits/8,(unsigned char*)pClrMap);
			}
		}
		else//其他，不处理，不会有其他
		{
			delete []ptmp;
			return FALSE;
		}
		delete []ptmp;
	}

#else

	unsigned long	ulPixelIdx = 0;//目标的像素位置
	LONGLONG	llSrcByteIdx = 0;//压缩数据字节位置
	RGBBIT 	stDstClr;//从解压后的数据中得到的目标的颜色值
	int iRes = 0;
	while(ulPixelIdx < (unsigned long)(lWidth*lHeight) )//目标没有完成
	{
		BYTE	byRepeat = 1;
		if(llSrcByteIdx >= llSrcSize)
		{
			// 缓冲池已被取干，重新读取文件
			ReadFile(m_pTgaFileHandle, pSrc, NX_TGA_RLC_BUF_SIZE, &m_dwNXRLCBufCapcity, NULL);
			if(m_dwNXRLCBufCapcity == 0)
			{
				return FALSE; // 需要数据，但文件中没有
			}
			m_llFilePos.QuadPart += m_dwNXRLCBufCapcity;
			llSrcByteIdx = 0;
			if(llSrcSize > m_dwNXRLCBufCapcity)
			{
				llSrcSize = m_dwNXRLCBufCapcity;
			}
			//llSrcSize += m_dwNXRLCBufCapcity;
		}
		if((pSrc[llSrcByteIdx]&0x80)!=0) //RunLength数据包，头一个字节是循环次数N，后面跟一个索引值或者颜色值，意为这个索引值或颜色值重复N次。
		{
			byRepeat=(pSrc[llSrcByteIdx]&0x7f)+1;//循环次数N
			++llSrcByteIdx;//指向下一个字节

			if(llSrcByteIdx + 4 > llSrcSize)
			{
				// TGARLCOneValue 可能需要更多的字节，从文件中取 add by wgf@2010-12-23
				DWORD dwUseableSize = (DWORD)( llSrcSize - llSrcByteIdx );
				BYTE *pMid = NULL;
				if(dwUseableSize > 0)
				{
					pMid = new BYTE[dwUseableSize];
					TP_MemcpyLine(pMid, pSrc + llSrcByteIdx, dwUseableSize);
				}
				
				ReadFile(m_pTgaFileHandle, pSrc + dwUseableSize, NX_TGA_RLC_BUF_SIZE - dwUseableSize, &m_dwNXRLCBufCapcity, NULL);
				TP_MemcpyLine(pSrc, pMid, dwUseableSize); // 回存
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
				break; //已经解码越界，停止解码，并保留已解码的部分。
			}

			if (byLocFlag == 0)//左下角，需要反转
			{
				for(BYTE i=0;i<byRepeat;i++)
				{
					pDst[(lHeight - (ulPixelIdx / lWidth) -1) * lWidth + (ulPixelIdx % lWidth)] = (DWORD)stDstClr.RGBdword;
					ulPixelIdx += 1;
				}
			}
			else if (byLocFlag == 2)//左上角，无需反转
			{
				for(BYTE i=0;i<byRepeat;i++)
				{
					pDst[ulPixelIdx] = (DWORD)stDstClr.RGBdword;
					ulPixelIdx += 1;
				}
			}
			else//其他，不处理，不会有其他
			{
				return FALSE;
			}
		}
		else //无处理数据包，第一个字节是包头，含有循环次数N，表示后续N个值是索引值或颜色值。这N个值只循环1次。
		{
			byRepeat=(pSrc[llSrcByteIdx]&0x7f)+1;
			++llSrcByteIdx;

			for(BYTE i=0;i<byRepeat;i++)
			{
				if(llSrcByteIdx + 4 > llSrcSize)
				{
					// TGARLCOneValue 可能需要更多的字节，从文件中取 add by wgf@2010-12-23
					DWORD dwUseableSize = (DWORD)( llSrcSize - llSrcByteIdx );
					BYTE *pMid = NULL;
					if(dwUseableSize > 0)
					{
						pMid = new BYTE[dwUseableSize];
						TP_MemcpyLine(pMid, pSrc + llSrcByteIdx, dwUseableSize);
					}

					ReadFile(m_pTgaFileHandle, pSrc + dwUseableSize, NX_TGA_RLC_BUF_SIZE - dwUseableSize, &m_dwNXRLCBufCapcity, NULL);
					TP_MemcpyLine(pSrc, pMid, dwUseableSize); // 回存
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
					break; //已经解码越界，停止解码，并保留已解码的部分。
				}

				if (byLocFlag == 0)//左下角，需要反转
				{
					pDst[(lHeight - (ulPixelIdx / lWidth) -1) * lWidth + (ulPixelIdx % lWidth)] = (DWORD)stDstClr.RGBdword;
					ulPixelIdx += 1;
				}
				else if (byLocFlag == 2)//左上角，无需反转
				{
					pDst[ulPixelIdx] = (DWORD)stDstClr.RGBdword;
					ulPixelIdx += 1;
				}
				else//其他，不处理，不会有其他
				{
					return FALSE;
				}
			}
		}
	}
	// 如果缓冲池有水量，则回移文件指针
	if(llSrcByteIdx < llSrcSize)
	{
		m_llFilePos.QuadPart -= (llSrcSize - llSrcByteIdx);
		LARGE_INTEGER llMoveDistance;
		llMoveDistance.QuadPart = llSrcByteIdx - llSrcSize;
		SetFilePointerEx(m_pTgaFileHandle, m_llFilePos, &llMoveDistance, FILE_BEGIN); // 退回多取的数据
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
	if (NULL != pClrMap)//有颜色表，需要查表
	{
		if (ulValue < (unsigned long)lClrMapCount)
		{
			stDstClr.RGBbits.rgbBlue = pClrMap[ulValue].rgbBlue;
			stDstClr.RGBbits.rgbGreen =pClrMap[ulValue].rgbGreen;
			stDstClr.RGBbits.rgbRed = pClrMap[ulValue].rgbRed;
			stDstClr.RGBbits.rgbReserved =pClrMap[ulValue].rgbReserved;
		}
	}
	else//无颜色表
	{
		switch (lSrcBits)
		{
		case 8://没有8位的情况
			return FALSE;
			break;
		case 16:
			stDstClr.RGBbits.rgbBlue = (ulValue & 0x0000001F)<<3;//取出颜色值，并且直接缩放到32位
			stDstClr.RGBbits.rgbGreen = (ulValue & 0x000003E0)>>2;
			stDstClr.RGBbits.rgbRed = (ulValue & 0x00007C00)>>7;
			stDstClr.RGBbits.rgbReserved = 0xff;//(ulValue & 0x00008000) ? 0xFF : 0; // 16 位时不含有alpha信息
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
		return -1;//已经越界
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
		m_cTGAHead.ImgDescriptor	=0x28;//左上角，32位TGA
		if (ImageInfo.fCompression)//压缩
		{
		}
		else//无压缩
		{
			m_cTGAHead.Imagetype	=2;//无压缩无颜色表
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
			DWORD dwBlockSize = g_dwBlockSize; // 网络版执行分块写入

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
	//	//文件还没有打开
	//	return FALSE;
	//}
	if (NULL == m_pTgaFileHandle)
	{
		//文件还没有打开
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