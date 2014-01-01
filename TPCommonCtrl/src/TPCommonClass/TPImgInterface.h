#pragma once
// 减少 ReadFile & WriteFile 访问磁盘的次数，优化图像文件读写
#define MAX_BUFFER_POOL_SIZE 4096000 // 目前暂时设置为 4M

// 新的带缓冲读写图片类，已经和底层LIB整合 add by wgf@2010-11-14
class CSGBufferFile
{
public:
	CSGBufferFile();
	~CSGBufferFile();

	// 对外接口函数
	HANDLE SGCreateFile(LPCTSTR lpFileName, DWORD dwDesireAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	BOOL SGReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
	BOOL SGWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
	BOOL SGGetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize);
	BOOL SGSetFilePointerEx(HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod);
	BOOL SGCloseHandle(HANDLE hObject);

private:
	inline void _Init(void);
	void _Clear(void);
public:
	HANDLE m_hFile;
	LARGE_INTEGER m_llFileSize;
	LARGE_INTEGER m_llFileCursor;
	BOOL m_bWriteFile;

	BYTE *m_pBufPool;
	BYTE *m_pPoolCursor;
	BYTE *m_pPoolRoof; // 读取时，上次从文件中读取的字节数目；写入时，已经向缓冲池写入字节数目
};
typedef struct tag_TPJPGCustomStu 
{
	DWORD dwVersion;
	UINT  iQuality;

	tag_TPJPGCustomStu()
	{
		dwVersion = 1; 
		iQuality=75;
	}

	DWORD GetDataSize()
	{
		return sizeof(long)+sizeof(UINT) ;
	}

	LRESULT   Save(LPBYTE pBuffer,int& iBufLength)
	{
		if(!pBuffer) 
		{
			iBufLength = 0;
			return S_FALSE;
		}
		*((DWORD *)pBuffer) = dwVersion;
		iBufLength  += sizeof(DWORD);
		pBuffer  += sizeof(DWORD);
		*((UINT*) pBuffer)=iQuality;
		iBufLength  += sizeof(UINT);
		pBuffer  += sizeof(UINT);
		return S_OK;
	}

	LRESULT   Read(LPBYTE pBuffer,int& iBufLength)
	{
		if(!pBuffer)
		{
			iBufLength = 0;
			return S_FALSE;
		}
		dwVersion = (*(DWORD *)pBuffer);
		iBufLength  += sizeof(DWORD);
		pBuffer  += sizeof(DWORD);
		iQuality = (*(UINT *)pBuffer);
		iBufLength  += sizeof(UINT);
		pBuffer  += sizeof(UINT);

		return S_OK;
	}
}TPJPGCustomStu,*LPTPJPGCustomStu;
//////////////////////////////////////////////////////////////////////////bmp begin
//************************* 取图象文件的保护方式  **********************************
enum	TPImageProtectMode
{
	READ_NOChange,
	READ_Protect,			// 保护
	READ_UnProtect,			// 不保护
	READ_Pattern
};

#define ALPHA_ZERO			0x00	// 全透
#define ALPHA_FULL			0xFF	// 不透

/////////////////// ARGB 颜色结构(RGBQUAD) ///////////////////////////
union COLORARGB
{
	struct {
		BYTE	B;
		BYTE	G;
		BYTE	R;
		BYTE	A;
	};

	DWORD	ARGB;
};

typedef	COLORARGB	*LPCOLORARGB;
#define COLORARGB_A(color)	(((LPCOLORARGB)(&color))->A)
#define COLORARGB_R(color)	(((LPCOLORARGB)(&color))->R)
#define COLORARGB_G(color)	(((LPCOLORARGB)(&color))->G)
#define COLORARGB_B(color)	(((LPCOLORARGB)(&color))->B)

// 背景不透 Alpha 混和
#define BFMixAlpha(aW)				(ALPHA_FULL)
#define BFMixRGB(cB, cW, aW)		(((aW*(cW-cB))>>8)+cB)
// Alpha 混和
#define MixAlpha(aB, aW)			(~((BYTE)((((~((BYTE)aB))&0xFF)*((~((BYTE)aW))&0xFF))>>8)))
#define MixRGB(cB, cW, aB, aW)		((cB*aB+cW*aW)/(aB+aW))
#define MixKValue(aW, a)			((aW<<8)/a)
#define MixRGBK(cB, cW, k)			(((k*(cW-cB))>>8)+cB)

inline DWORD ColorMix(DWORD clB, DWORD clW)
{
	if(COLORARGB_A(clW) == ALPHA_ZERO)
		return	clB;
	if(COLORARGB_A(clW) == ALPHA_FULL)
		return	clW;
	if(COLORARGB_A(clB) == ALPHA_ZERO)
		return	clW;

	if(COLORARGB_A(clB) == ALPHA_FULL){
		COLORARGB_R(clB) = BFMixRGB(COLORARGB_R(clB), COLORARGB_R(clW), COLORARGB_A(clW));
		COLORARGB_G(clB) = BFMixRGB(COLORARGB_G(clB), COLORARGB_G(clW), COLORARGB_A(clW));
		COLORARGB_B(clB) = BFMixRGB(COLORARGB_B(clB), COLORARGB_B(clW), COLORARGB_A(clW));
		COLORARGB_A(clB) = BFMixAlpha(COLORARGB_A(clW));
	}else{
		COLORARGB_A(clB) = MixAlpha(COLORARGB_A(clB), COLORARGB_A(clW));
		register WORD K = MixKValue(COLORARGB_A(clW),COLORARGB_A(clB));
		COLORARGB_R(clB) = MixRGBK(COLORARGB_R(clB), COLORARGB_R(clW), K);
		COLORARGB_G(clB) = MixRGBK(COLORARGB_G(clB), COLORARGB_G(clW), K);
		COLORARGB_B(clB) = MixRGBK(COLORARGB_B(clB), COLORARGB_B(clW), K);
	}

	return	clB;
}

inline DWORD	ReadImageColorExchang32( DWORD SrcColor, DWORD DestColor, TPImageProtectMode protflag )
{
	if( protflag == READ_NOChange )
		return( SrcColor );

	DWORD	ReturnColor;
	DWORD	SAlpha,SRGB;

	SAlpha = (SrcColor&0xff000000);
	SRGB = (SrcColor&0xffffff);
	if( protflag == READ_UnProtect ) {
		if( SAlpha == 0x0 )
			ReturnColor = (SrcColor|0xff000000);
		else
			ReturnColor = ColorMix( DestColor, SrcColor );
	} else if( protflag == READ_Protect ) {
		if( SAlpha == 0x0 || SRGB == 0 )
			ReturnColor = 0x0;
		else
			ReturnColor = ColorMix( DestColor, SrcColor );
	} else if( protflag == READ_NOChange){
		ReturnColor = SrcColor;
	} else {
		if( SAlpha == 0x0 )
			ReturnColor = (SrcColor|0xff000000);
		else
			ReturnColor = SrcColor;
	}
	return( ReturnColor );
}

///bmp end
///gif begin


//////////////////////////////////////////////////////////////////////////gif end
typedef enum {
	IMG_UNKNOW,
	IMG_BMP,
	IMG_PNG,
	IMG_JPG,
	IMG_TIF,
	IMG_PCX,
	IMG_TGA,
	IMG_GIF
}IMG_TYPE_ENUM;

//返回值定义

enum FIO_Sign
{
	FIO_OtherErr=0,					//未知错误
	FIO_Succeed,						//成功
	FIO_OpenErr,						//打开错误
	FIO_TooLarge,						//文件太大
	FIO_BadFile,							//错误的文件格式
	FIO_Unsupported,					//损坏或者是不支持的格式
	FIO_MemErr,							//内存错误
	FIO_NoContent,					//没有音频或视频
	FIO_BadRate,						//错误的音频率
	FIO_BadCompression,			//错误的压缩方式
	FIO_BadCodec,						//编码器没找到
	FIO_FormatAccept,				//输出格式不支持
	FIO_OutOfDiskSpace,			//磁盘空间不足
	FIO_ErrFileNotFound,			//文件没找到
	FIO_ErrTooManyOpenFiles,	//太多的打开文件
	FIO_ErrPermErr,					//权限不够
	FIO_ErrInvalidDrive,				//驱动器无效
	FIO_ErrIo,								//文件输入输出错误
	FIO_ErrInUse,						//文件正在被使用
	FIO_LoadPlugInErr,				//加载插件错误
	FIO_NoInterface,					//没有接口
	FIO_NoPlugIn,						//没有此插件
	FIO_WriteErr,						//写入错误
	FIO_ErrDiffVideoStand,			//不一致的视频制式
	FIO_ErrNullFileName,			//送入的文件名为空
	FIO_ErrNoIdx,						//需要索引文件的没有找到索引文件
	FIO_BadParameter,				// 参数不正确，主要是空指针
	FIO_NetWorkDrop,				// 网络掉线
};

// SG: 抽象的图像描述，使用 LPTPResource->pPrivateData 来携带
typedef struct tag_SGImageDescriptor{
	void *pPrivateData; // 具体图像文件结构指针，避免多次打开文件
	BYTE *pImage; // 图像数据 BGRA
	int iWidth; // 图像 宽 & 高
	int iHeight;
	IMG_TYPE_ENUM eImageType; // 图像类型
	bool bReadFile; // 决定 imgClose() 时，图像数据是否释放
	bool bKeepOdd; // 是否保留奇数列图像的宽度 add by wgf@2010-5-18
	bool bHaveAlpha; // 图像是否含有 alpha 信息
	bool bPlayStatus; // true - 播放模式，顺序取帧，不能重复

	//[[ljj@2011-4-28:图像格式是否支持分块读取:压缩的TGA暂不做分块读取，实现起来太麻烦。
	bool bBlockReadSupported;
	//]]

	tag_SGImageDescriptor(){
		pPrivateData = NULL;
		pImage = NULL;
		iWidth = 0;
		iHeight = 0;
		eImageType = IMG_UNKNOW;
		bReadFile = true;
		bKeepOdd = false;
		bHaveAlpha = false;
		bPlayStatus = false;
		bBlockReadSupported = true;
	}
}SGImageDescriptor;

typedef struct tag_TPResource			//资源信息
{
	int		iSize;						//结构大小
	DWORD	dwVersion;			//版本号
	FILE*	fp;							//文件指针
	TCHAR	ptszfileName[MAX_PATH];		//文件路径名
	void*	pPrivateData;				//私有数据
	void*   hData;                      //时间线数据指针  
}TPResource,*LPTPResource;


class CTPImgBase
{
public:
	CTPImgBase();
	virtual ~CTPImgBase();


public:
	virtual int Open(LPTPResource lpResource){return 0;}
	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}

	virtual int GetImage(LPTPResource lpResource){return 0;}
};

class CTPImgJPG : public CTPImgBase
{
public:
	CTPImgJPG();
	virtual ~CTPImgJPG();

	virtual int Open(LPTPResource lpResource);
	virtual int Close(SGImageDescriptor *pSGImageDescriptor);

	virtual int GetImage(LPTPResource lpResource);
};
 class CTPImgBMP : public CTPImgBase
 {
 public:
 	CTPImgBMP();
 	virtual ~CTPImgBMP();
 
 	virtual int Open(LPTPResource lpResource);
 	virtual int Close(SGImageDescriptor *pSGImageDescriptor);
 
 	virtual int GetImage(LPTPResource lpResource);
 };
 class CTPImgPNG : public CTPImgBase
 {
 public:
 	CTPImgPNG();
 	virtual ~CTPImgPNG();
 
 	virtual int Open(LPTPResource lpResource);
 	virtual int Close(SGImageDescriptor *pSGImageDescriptor);
 
 	virtual int GetImage(LPTPResource lpResource);
 };

 class CTPImgTIF : public CTPImgBase
 {
 public:
 	CTPImgTIF();
 	virtual ~CTPImgTIF();
 
 	virtual int Open(LPTPResource lpResource);
 	virtual int Close(SGImageDescriptor *pSGImageDescriptor);
 
 	virtual int GetImage(LPTPResource lpResource);
 };
 class CTPImgPCX : public CTPImgBase
 {
 public:
 	CTPImgPCX();
 	virtual ~CTPImgPCX();
 
 	virtual int Open(LPTPResource lpResource);
 	virtual int Close(SGImageDescriptor *pSGImageDescriptor);
 
 	virtual int GetImage(LPTPResource lpResource);
 };
 class CTPImgTGA : public CTPImgBase
 {
 public:
 	CTPImgTGA();
 	virtual ~CTPImgTGA();
 
 	virtual int Open(LPTPResource lpResource);
 	virtual int Close(SGImageDescriptor *pSGImageDescriptor);
 
 	virtual int GetImage(LPTPResource lpResource);
 };
 class CTPImgGIF : public CTPImgBase
 {
 public:
 	CTPImgGIF();
 	virtual ~CTPImgGIF();
 
 	virtual int Open(LPTPResource lpResource);
 	virtual int Close(SGImageDescriptor *pSGImageDescriptor);
 
 	virtual int GetImage(LPTPResource lpResource);
 };
 
// CTPImgPreviewWnd message handlers
class AFX_EXT_CLASS CTPImgDecode
{
public:
	CTPImgDecode();
	~CTPImgDecode();

private:
	CTPImgBase		  *m_pImgBase ;

	TPResource		  *m_lplpResource;
	SGImageDescriptor *m_pSGImageDescriptor;
	IMG_TYPE_ENUM DistinguishImageFormat(LPTPResource lpResource);

	// JPG
public:
	int Open(TCHAR *cFileName, int &iWidth, int &iHeight);
	int GetImage(void *pBuffer);
	// 	int jpgOpen(LPTPResource lpResource);
	// 	int jpgClose(SGImageDescriptor *pSGImageDescriptor);
	// 	int jpgGetImage(LPTPResource lpResource);
	// 	int jpgPutImage(LPTPResource lpResource);
	// 
	// 	int jpgGetbNeedCustomSet(TPFileFormat* pFileFormat);  //
	// 	int jpgGetSimpleCustomWnd(TPFileFormat* pFileFormat,void* pParentWnd,void*& pWnd);
	// 	int jpgGetCustomWnd(TPFileFormat* pFileFormat,void* pParentWnd,void*& pWnd);
	// 	int jpgUpdateCustomData(TPFileFormat* pFileFormat,void* pWnd);
	// 	int jpgReleaseWnd(TPFileFormat* pFileFormat,void* pWnd);
	// 
	// 	// 分块读取图像数据 Begin
	// 	int NXjpegPrepareGetImage(LPTPResource lpResource, UINT *pGrayPlatte, void **ppPrivateInfo);
	// 	int NXjpegReadImageBlock(LPTPResource lpResource, int iBeginRow, int iEndRow, UINT *pGrayPlatte, void *pPrivateInfo);
	// 	// 分块读取图像数据 End

};
