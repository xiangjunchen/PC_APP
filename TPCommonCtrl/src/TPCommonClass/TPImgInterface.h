#pragma once


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
// class CTPImgBMP : public CTPImgBase
// {
// public:
// 	CTPImgBMP();
// 	virtual ~CTPImgBMP();
// 
// 	virtual int Open(LPTPResource lpResource){return 0;}
// 	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
// 
// 	virtual int GetImage(LPTPResource lpResource){return 0;}
// };
// class CTPImgPNG : public CTPImgBase
// {
// public:
// 	CTPImgPNG();
// 	virtual ~CTPImgPNG();
// 
// 	virtual int Open(LPTPResource lpResource){return 0;}
// 	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
// 
// 	virtual int GetImage(LPTPResource lpResource){return 0;}
// };
// 
// class CTPImgTIF : public CTPImgBase
// {
// public:
// 	CTPImgTIF();
// 	virtual ~CTPImgTIF();
// 
// 	virtual int Open(LPTPResource lpResource){return 0;}
// 	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
// 
// 	virtual int GetImage(LPTPResource lpResource){return 0;}
// };
// class CTPImgPCX : public CTPImgBase
// {
// public:
// 	CTPImgPCX();
// 	virtual ~CTPImgPCX();
// 
// 	virtual int Open(LPTPResource lpResource){return 0;}
// 	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
// 
// 	virtual int GetImage(LPTPResource lpResource){return 0;}
// };
// class CTPImgTGA : public CTPImgBase
// {
// public:
// 	CTPImgTGA();
// 	virtual ~CTPImgTGA();
// 
// 	virtual int Open(LPTPResource lpResource){return 0;}
// 	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
// 
// 	virtual int GetImage(LPTPResource lpResource){return 0;}
// };
// class CTPImgGIF : public CTPImgBase
// {
// public:
// 	CTPImgGIF();
// 	virtual ~CTPImgGIF();
// 
// 	virtual int Open(LPTPResource lpResource){return 0;}
// 	virtual int Close(SGImageDescriptor *pSGImageDescriptor){return 0;}
// 
// 	virtual int GetImage(LPTPResource lpResource){return 0;}
// 
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
