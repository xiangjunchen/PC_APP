#include "StdAfx.h"

DWORD g_SGMaxImageSize = 5000 * 4000; // 

CCriticalSection  g_CritSection;

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
// 通过读取文件信息判断文件类型
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
		// 下面判断 TGA
		// SG: 由于 TGA 类保存的图像末尾 26 字节不确定，增加按照 IMAGE_TYPE 来判定
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
//class CTPImgBMP
//{
//public:
//	CTPImgBMP();
//	virtual ~CTPImgBMP();
//
//	virtual int Open(SGImageDescriptor *pSGImageDescriptor){return 0;}
//	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
//
//	virtual int GetImage(SGImageDescriptor *pSGImageDescriptor){return 0;}
//};
//class CTPImgPNG
//{
//public:
//	CTPImgPNG();
//	virtual ~CTPImgPNG();
//
//	virtual int Open(SGImageDescriptor *pSGImageDescriptor){return 0;}
//	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
//
//	virtual int GetImage(SGImageDescriptor *pSGImageDescriptor){return 0;}
//};
CTPImgBase::CTPImgBase()
{
	
}
CTPImgBase::~CTPImgBase()
{

}
CTPImgJPG::CTPImgJPG()
{

}
CTPImgJPG::~CTPImgJPG()
{

}
// SG: 得到图像的 宽 & 高
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
	pSGImageDescriptor->pPrivateData = hJgpFile; // SG: 保存 JPG 句柄，记得关闭
	return FIO_Succeed;
}

int CTPImgJPG::Close(SGImageDescriptor *pSGImageDescriptor)
{
	if(NULL != pSGImageDescriptor && NULL != pSGImageDescriptor->pPrivateData) // SG: 读存储句柄，写没有使用
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
		pSGImageDescriptor->pImage = JpegFile::JpegFileToBGRA(pSGImageDescriptor->pPrivateData,&uWidth,&uHeight); // SG: 直接得到 BGRA 格式图像，buf 需要调用者释放
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
// int CTPImgJPG::Open(SGImageDescriptor *pSGImageDescriptor)
// {
// 	//open
// 	HANDLE hJgpFile = CreateFile(sFileNmae, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
// 	if(NULL == hJgpFile)
// 	{
// 		return 0;
// 	}
// 
// 	JpegFile::GetHeadSize(hJgpFile, (UINT *)&(pSGImageDescriptor->iWidth), (UINT *)&(pSGImageDescriptor->iHeight));
// 
// 	//getimg
// 
// 	UINT uWidth =0;
// 	UINT uHeight =0;
// 	pBuffer = JpegFile::JpegFileToBGRA(hJgpFile,&uWidth,&uHeight); // SG: 直接得到 BGRA 格式图像，buf 需要调用者释放
// 
// 	//close
// 	CloseHandle(hJgpFile);
// 
// 	return 0;
// }


//class CTPImgBMP
//{
//public:
//	CTPImgBMP();
//	virtual ~CTPImgBMP();
//
//	virtual int Open(SGImageDescriptor *pSGImageDescriptor){return 0;}
//	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
//
//	virtual int GetImage(SGImageDescriptor *pSGImageDescriptor){return 0;}
//};
//class CTPImgTIF
//{
//public:
//	CTPImgTIF();
//	virtual ~CTPImgTIF();
//
//	virtual int Open(SGImageDescriptor *pSGImageDescriptor){return 0;}
//	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
//
//	virtual int GetImage(SGImageDescriptor *pSGImageDescriptor){return 0;}
//};
//class CTPImgPCX
//{
//public:
//	CTPImgPCX();
//	virtual ~CTPImgPCX();
//
//	virtual int Open(SGImageDescriptor *pSGImageDescriptor){return 0;}
//	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
//
//	virtual int GetImage(SGImageDescriptor *pSGImageDescriptor){return 0;}
//};
//class CTPImgTGA
//{
//public:
//	CTPImgTGA();
//	virtual ~CTPImgTGA();
//
//	virtual int Open(SGImageDescriptor *pSGImageDescriptor){return 0;}
//	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
//
//	virtual int GetImage(SGImageDescriptor *pSGImageDescriptor){return 0;}
//};
//class CTPImgGIF
//{
//public:
//	CTPImgGIF();
//	virtual ~CTPImgGIF();
//
//	virtual int Open(SGImageDescriptor *pSGImageDescriptor){return 0;}
//	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
//
//	virtual int GetImage(SGImageDescriptor *pSGImageDescriptor){return 0;}
//};

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
	//pSGImageDescriptor->bKeepOdd = (iAccessMode & TP_ACCESS_KEEP_ODD) ? true : false; // 设置是否保持奇数列图像 add by wgf@2010-5-18
	m_pSGImageDescriptor->bKeepOdd = false; // 目前E7和NS都使用偶数列图像 add by wgf@2010-7-4
	if(IMG_UNKNOW == m_pSGImageDescriptor->eImageType)
	{
		delete m_pSGImageDescriptor;
		delete m_lplpResource;
// 		*m_lplpResource = NULL;
		return FIO_OtherErr;
	}
	m_pSGImageDescriptor->bReadFile = true; // SG: 最后的图像数据需要释放

	// SG: 读取信息，填充 lpResource 结构
	int nRet = FIO_Succeed; 
	switch(m_pSGImageDescriptor->eImageType)
	{
// 	case IMG_BMP:
// 		pImgBase = new CTPImgBMP();
// 		//	nRet = bmpOpen(lpResource);
// 		break;
// 	case IMG_PNG:
// 		pImgBase = new CTPImgPNG();
// 		//	nRet = NXpngOpen(lpResource); // 仅得到图像宽高等信息，并不读取图像数据
// 		break;
	case IMG_JPG:
		m_pImgBase = new CTPImgJPG();
		//nRet = jpgOpen(lpResource);
		break;
// 	case IMG_TIF:
// 		pImgBase = new CTPImgTIF();
// 		//	nRet = tifOpen(lpResource);
// 		break;
// 	case IMG_PCX:
// 		pImgBase = new CTPImgPCX();
// 		//	nRet = pcxOpen(lpResource);
// 		break;
// 	case IMG_TGA:
// 		pImgBase = new CTPImgTGA();
// 		//	nRet = tgaOpen(lpResource);
// 		break;
// 	case IMG_GIF:
// 		pImgBase = new CTPImgGIF();
// 		//	nRet = gifOpen(lpResource);
// 		break;
	default:
	//	ASSERT(0);
		nRet = FIO_Unsupported;
		break;
	}
	if(nRet != FIO_Succeed)
	{
		delete m_lplpResource;
		m_lplpResource = NULL;
// 		*m_lplpResource = NULL;
		return nRet;
	}
	m_pImgBase->Open(m_lplpResource);
	if ((DWORD)m_pSGImageDescriptor->iWidth * (DWORD)m_pSGImageDescriptor->iHeight >= g_SGMaxImageSize)  // SG: 图像过大
	{
		// 上层不会调用 imageFileClose(),这里调用，清除底层申请的资源 add by wgf@2010-9-1
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