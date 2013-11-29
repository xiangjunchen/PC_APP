#if !defined( _INC_TPBASEVADEF_H )
#define _INC_TPBASEVADEF_H

#include <list>

#ifdef TP_NS_W732
#define  TP_NS_W7
#elif  TP_NS_W764
#define  TP_NS_W7
#endif


#define TPBASEVADEF_VERSION_1       1  
#define TPBASEVADEF_VERSION_2       2  
#define TPBASEVADEF_VERSION_3       3  

#define TPBASEVADEF_VERSION  TPBASEVADEF_VERSION_1

#define MAX_COMPRESSION_NAME 100
#define MAX_COMPRESSION_COUNT 100

#define MAX_TYPE_NAME 32
#define MAX_EXT_NAME_SIZE 32
#define MAX_EXT_NAME_TYPE 32
#define MAX_FILETYPE_NAME 32


typedef long TP_ACCESS_MODE;

#define TP_ACCESS_NO			0x00000000
#define TP_ACCESS_READ			0x00000001
#define TP_ACCESS_WRITE			0x00000002
#define TP_ACCESS_FORCE			0x00000004//ljj @ 2006-12-28 for DVD open
#define TP_ACCESS_GETINFO		0x00000008//for TPAVFile: don't create cutlist ,ljj @ 2008-7-11
#define TP_ACCESS_GETIMAGE		0x00000010//for TPAVFile: don't prepare decoding.,ljj @ 2008-7-11
#define TP_ACCESS_PREPARE		0x00000020		
#define TP_ACCESS_GETAUDIO		0x00000040
#define TP_ACCESS_NOEXTINFO		0x00000080
#define TP_ACCESS_NOCHECKRATE   0x00000100
#define TP_ACCESS_NOPROXYCTRL	0x00001000//ljj @ 2008-12-2 for no proxy format limit access for LIHUILI's Import	
#define TP_ACCESS_DBEINFO   	0x00002000//swx 2009-03-14 for Doblly E track info
#define TP_ACCESS_STARTTC		0x00004000//ljj @2009-4-9
#define TP_ACCESS_EDI   		0x00008000//swx Edi

#define TP_VIDEO_STANDARD_NUM  19




enum	TP_VIDEO_STANDARD	// 桌面视频制式标准枚举定义
{
	TP_VIDEOSTANDARD_UNKNOW				= 0x00000000,		//Invalid

	TP_VIDEOSTANDARD_PAL				= 0x00000001,		//PAL size:720*576 f/s : 25
	TP_VIDEOSTANDARD_NTSC_2997			= 0x00000002,		//NTSC size:720*486  f/s : 29.97
	TP_VIDEOSTANDARD_NTSC_30			= 0x00000004,		//NTSC size:720*486 f/s : 30 
	TP_VIDEOSTANDARD_SECAM				= 0x00000008,		//SECAM

	TP_VIDEOSTANDARD_1920_1080_50i		= 0x00000010,		//HDTV size:1920*1080 f/s : 25  interlaced
	TP_VIDEOSTANDARD_1920_1080_5994i	= 0x00000020,		//HDTV size:1920*1080 f/s : 29.97 interlaced
	TP_VIDEOSTANDARD_1920_1080_60i		= 0x00000040,		//HDTV size:1920*1080 f/s : 30 interlaced

	TP_VIDEOSTANDARD_1920_1080_2398p	= 0x00000080,		//HDTV size:1920*1080 f/s : 23.98 progressive
	TP_VIDEOSTANDARD_1920_1080_24p		= 0x00000100,		//HDTV size:1920*1080 f/s : 24 progressive
	TP_VIDEOSTANDARD_1920_1080_25p		= 0x00000200,		//HDTV size:1920*1080 f/s : 25 progressive
	TP_VIDEOSTANDARD_1920_1080_2997p	= 0x00000400,		//HDTV size:1920*1080 f/s : 29.97 progressive
	TP_VIDEOSTANDARD_1920_1080_30p		= 0x00000800,		//HDTV size:1920*1080 f/s : 30 progressive

	TP_VIDEOSTANDARD_1280_720_2398p		= 0x00001000,	    //HDTV size:1280*720 f/s : 23.98 progressive
	TP_VIDEOSTANDARD_1280_720_24p		= 0x00002000,		//HDTV size:1280*720 f/s : 24 progressive
	TP_VIDEOSTANDARD_1280_720_50p		= 0x00004000,		//HDTV size:1280*720 f/s : 50 progressive
	TP_VIDEOSTANDARD_1280_720_5994p		= 0x00008000,		//HDTV size:1280*720 f/s : 59.94 progressive

	TP_VIDEOSTANDARD_1440_1080_50i		= 0x00010000,	    //HD  size:1440*1080 f/s : 25 interlaced
	TP_VIDEOSTANDARD_1440_1080_5994i	= 0x00020000,	    //HD  size:1440*1080 f/s : 29.97 interlaced
	TP_VIDEOSTANDARD_1440_1080_60i		= 0x00040000,	    //HD  size:1440*1080 f/s : 30 interlaced

	TP_VIDEOSTANDARD_PAL_16_9			= 0x00080000,		//PAL size:720*576 f/s : 25  
	TP_VIDEOSTANDARD_NTSC_2997_16_9		= 0x00100000,		//NTSC size:720*486  f/s : 29.97
	TP_VIDEOSTANDARD_NTSC_30_16_9		= 0x00200000,		//NTSC size:720*486 f/s : 30 
	TP_VIDEOSTANDARD_NTSC_2997_480		= 0x00400000,		//NTSC size:720*480  f/s : 29.97  //先留着，防止以后特殊情况再改成480
	TP_VIDEOSTANDARD_NTSC_30_480		= 0x00800000,		//NTSC size:720*480 f/s : 30 //先留着，防止以后特殊情况再改成480


	TP_VIDEOSTANDARD_1280_1080_50i		= 0x01000000,	    //HD  size:1280*1080 f/s : 25 interlaced
	TP_VIDEOSTANDARD_1280_1080_5994i	= 0x02000000,	    //HD  size:1280*1080 f/s : 29.97 interlaced
	TP_VIDEOSTANDARD_1280_1080_60i		= 0x04000000,	    //HD  size:1280*1080 f/s : 30 interlaced
};


#define TP_VIDEOSTANDARD_ALL 0x0fffffff   //缺省支持所有制式 hh



enum	TP_VIDEO_HS_CLASS	
{
	TP_VIDEO_HS_UNKNOWN			,
	TP_VIDEO_HS_SD				,
	TP_VIDEO_HS_HD720p			,
	TP_VIDEO_HS_HD1080i		    ,
	TP_VIDEO_HS_HD1080p		    ,
	TP_VIDEO_HS_HD1440i			,
	TP_VIDEO_HS_HD1280i			,

};

/************视频制式参数结构***************/

typedef struct tag_TPTimeBase		//视频制式参数
{
	int		iSize;							//结构大小
	DWORD	dwVersion;			//版本号
	int		lTimeRate;			//15,24,25,2997,30
	int		lTimeScale;			//1或者100
	long    lWidth;
	long    lHeight;
}TPTimeBase, *LPTPTimeBase;

/************视频制式参数结构***************/

//文件类型
typedef long  TP_MEDIA_CLASS_TYPE;

#define  TP_MEDIA_CLASS_UNKNOWN 0x00000000
#define  TP_MEDIA_CLASS_V      0x00000001

#define  TP_MEDIA_CLASS_A1     0x00000002
#define  TP_MEDIA_CLASS_A2     0x00000004
#define  TP_MEDIA_CLASS_A3     0x00000008
#define  TP_MEDIA_CLASS_A4     0x00000010
#define  TP_MEDIA_CLASS_A5     0x00000020
#define  TP_MEDIA_CLASS_A6     0x00000040
#define  TP_MEDIA_CLASS_A7     0x00000080
#define  TP_MEDIA_CLASS_A8     0x00000100

#define  TP_MEDIA_CLASS_A      (TP_MEDIA_CLASS_A1 | TP_MEDIA_CLASS_A2 | TP_MEDIA_CLASS_A3 | TP_MEDIA_CLASS_A4 | TP_MEDIA_CLASS_A5 | TP_MEDIA_CLASS_A6 | TP_MEDIA_CLASS_A7 | TP_MEDIA_CLASS_A8)


#define  TP_MEDIA_CLASS_V_G		   0x00001000
#define  TP_MEDIA_CLASS_V_P		   0x00002000
#define  TP_MEDIA_CLASS_V_V		   0x00004000
#define  TP_MEDIA_CLASS_V_SERIAL   0x00008000
#define  TP_MEDIA_CLASS_V_ICG      0x00010000
#define  TP_MEDIA_CLASS_V_ICGANI   0x00020000
#define  TP_MEDIA_CLASS_V_ICGCRAWL 0x00040000


#define  TP_MEDIA_CLASS_EFF		   0x00100000
#define  TP_MEDIA_CLASS_KEY		   0x00080000

#define TP_MEDIA_CLASS_ASPECTRATIO_4_3	0x01000000
#define TP_MEDIA_CLASS_ASPECTRATIO_16_9 0x02000000
#define TP_MEDIA_CLASS_DF				0x10000000
#define TP_MEDIA_CLASS_NDF				0x20000000

// add by wgf@2010-5-31
#define  NX_IMAGE_NO_ALPHA		0x00000000
// 图片含有 alpha 信息
#define  NX_IMAGE_HAVE_ALPHA	0x00000001



enum PixelFormat 
{
	PIX_FMT_UNKNOWN		= 0x00000000,

	PIX_FMT_ARGB32      = 0x00000001,///< Packed pixel, 4 bytes per pixel, ARGBARGB...
	PIX_FMT_RGBA32      = 0x00000002,///< Packed pixel, 4 bytes per pixel, BGRABGRA...
	PIX_FMT_RGB24       = 0x00000004,///< Packed pixel, 3 bytes per pixel, RGBRGB...
	PIX_FMT_BGR24       = 0x00000008,///< Packed pixel, 3 bytes per pixel, BGRBGR...
	PIX_FMT_GRAY8		= 0x00000010,  
	PIX_FMT_RGB565      = 0x00000020,///< always stored in cpu endianness   16 bpp
	PIX_FMT_RGB555      = 0x00000040,///< always stored in cpu endianness, most significant bit to 1  16 bpp

	PIX_FMT_YUYV        = 0x00000100,
	PIX_FMT_YUAYVA		= 0x00000200,	
	PIX_FMT_UYVY		= 0x00000400,
	PIX_FMT_UYAVYA		= 0x00000800,
	PIX_FMT_YUV422P     = 0x00001000,///< Planar YUV 4:2:2 (1 Cr & Cb sample per 2x1 Y samples)
	PIX_FMT_YUV444P     = 0x00002000,///< Planar YUV 4:4:4 (1 Cr & Cb sample per 1x1 Y samples)
	PIX_FMT_YUV410P     = 0x00004000,///< Planar YUV 4:1:0 (1 Cr & Cb sample per 4x4 Y samples)
	PIX_FMT_YUV411P     = 0x00008000,///< Planar YUV 4:1:1 (1 Cr & Cb sample per 4x1 Y samples)
	PIX_FMT_YUV420P     = 0x00010000,///< Planar YUV 4:2:0 (1 Cr & Cb sample per 2x2 Y samples)
    PIX_FMT_YUVA        = 0x00020000,


	PIX_FMT_SPLIT_ALPHA = 0x00040000 ///是否具有独立的alpha buffer
};


#define  PIX_FMT_RGB_TYPE (PIX_FMT_ARGB32 | PIX_FMT_RGBA32 | PIX_FMT_RGB24 | PIX_FMT_BGR24 | PIX_FMT_RGB565 | PIX_FMT_RGB555)
#define  PIX_FMT_YUV_TYPE (PIX_FMT_YUYV | PIX_FMT_YUAYVA | PIX_FMT_UYVY | PIX_FMT_UYAVYA | PIX_FMT_YUV422P | PIX_FMT_YUV444P | PIX_FMT_YUV410P | PIX_FMT_YUV411P | PIX_FMT_YUV420P | PIX_FMT_YUVA)
#define  PIX_FMT_A_TYPE (PIX_FMT_ARGB32 | PIX_FMT_RGBA32 | PIX_FMT_GRAY8 | PIX_FMT_YUAYVA | PIX_FMT_UYAVYA | PIX_FMT_YUVA)


/***********音频格式参数结构*******************/


typedef struct tag_TPAudioFormat    //音频格式(未压缩)
{
	WORD					iSize;					//结构大小
	DWORD					dwVersion;				//版本号
	WORD					iChannels;				//通道数
	DWORD					iSamplesRate;			//采样率
	WORD					iBitsPerSample;			//量化位数

	//tnt add(tanting) - 10.11.9
	WORD                    iSeekCount;             //多帧seek时记录seek的samplecount
	//tnt end
}TPAudioFormat,*LPTPAudioFormat;

typedef struct tag_TPAudioCompressPara		//音频压缩参数
{
	WORD	iSize;					//结构大小
	DWORD	dwVersion;				//版本号
	DWORD	iDataRate;				//数据率，以kbps为单位
	DWORD	iInterleave;			//交错帧数
	DWORD   dwPrivateDataSize;
	void*	pPrivateData;			//私有数据
}TPAudioCompressPara,*LPTPAudioCompressPara;

typedef struct tag_TPAudioFormatEx    //包括音频压缩格式和参数
{
	DWORD					iSize;							//结构大小
	DWORD					dwVersion;			//版本号
	TPAudioFormat			tpAudioFormat;
	DWORD					dwFOURCC;			//FOURCC Codes
	TPAudioCompressPara		tpAudioCompressPara;	//压缩参数
}TPAudioFormatEx,*LPTPAudioFormatEx;

/***********音频格式参数结构*******************/


enum TP_SCANMODE
{
	TP_ScanMode_Invalid 				= 0x00000000,
	TP_ScanMode_Interlaced				= 0x00000001,
	TP_ScanMode_Progressive				= 0x00000002,
};


enum TP_ASPECTRATIO
{
	TP_AspectRatio_Unknown = 0x00000000,
	TP_AspectRatio_4_3  = 0x00000001,
	TP_AspectRatio_16_9 = 0x00000002,
};

//分辨率结构统一定义
typedef struct  tag_TPResolutionInfo
{
	DWORD          dwSize;
	long           lWidth;
	long           lHeight;
	TP_ASPECTRATIO tpAspectRatio;
	TPTimeBase     tpTimeBase;
	TP_SCANMODE    tpScanMode;
}TPResolutionInfo, *LPTPResolutionInfo;

typedef struct  tag_MemBufCopyParam
{
	BOOL		bSwap;
	tag_MemBufCopyParam ()
	{
		bSwap = TRUE;
	}
	
}MemBufCopyParam, *LPMemBufCopyParam;
/***********视频和图象格式参数结构*****************/
typedef struct tag_TPImageFormat	//图象视频帧格式(未压缩)
{
	DWORD			iSize;					//结构大小
	DWORD			dwVersion;				//版本号
	DWORD			iWidth;					//图像宽
	DWORD			iHeight;			 	//图像高
	DWORD			iBitsPerPixel; 	   		//每个像素所占位数
	DWORD			iRowbytes;				//每行所占字节数
	DWORD			iPixFmt;				//像素格式  对应PixelFormat
	DWORD			dwFrameSize;			//每帧图像的大小
	DWORD			iAspectRatioNum;		//纵横比分子
	DWORD			iAspectRatioDen;		//纵横比分母
	BOOL			bStill;					//是否为静态图像
	TP_SCANMODE     tpScanMode;             //图像扫描模式
	DWORD           dwPrivateDataSize;
	void*       	pPrivateData;			//私有数据
}TPImageFormat, *LPTPImageFormat;


typedef struct tag_TPVideoCompressPara		//视频压缩参数
{
	DWORD			iSize;							//结构大小
	DWORD			dwVersion;				//版本号
	DWORD			iKeyFrame;				//关键帧
	DWORD			iDataRate;				//数据率，以kbps为单位
	DWORD           dwPrivateDataSize;
	DWORD           dwCompressType;
	void*			pPrivateData;			//私有数据
}TPVideoCompressPara,*LPTPVideoCompressPara;


//scan mode ?????
typedef struct tag_TPVideoFormat			//视频格式（包括压缩格式和参数）
{
	DWORD					iSize;							//结构大小
	DWORD					dwVersion;		//版本号
	TPTimeBase				tpTimeBase;		//帧速率（类似于视频标准）
	TP_VIDEO_STANDARD       tpVideoStandard;
	TPImageFormat			tpImageFormat;
	DWORD					dwFOURCC;			//FOURCC Codes
	TPVideoCompressPara		tpVideoCompressPara;	//压缩参数
	DWORD                   dwSubMediaType;  //临时使用,一般不要用
}TPVideoFormat,*LPTPVideoFormat;

/***********视频和图象格式参数结构*****************/



/**********文件格式参数结构*********************/

typedef struct tag_TPFileFormat					//文件格式
{
	DWORD				iSize;					//结构大小
	DWORD				dwVersion;				//版本号
	BOOL 				bHasVideo;				//是否含有视频
	BOOL 				bHasAudio;				//是否含有音频
	UINT                uParam;                 //中间传递参数 
	GUID				guidFileType;			//文件类型GUID
	DWORD               dwFileHeadSize;			//文件头尺寸
	TPVideoFormat		tpVideoFormat;			//视频格式
	TPAudioFormatEx		tpAudioFormatEx;			//音频格式
}TPFileFormat,*LPTPFileFormat;


/**********文件格式参数结构*********************/




/************视音频数据参数结构（含数据BUFFER）**********/

//need change
typedef struct tag_TPImageData		//某一帧图像和图象数据结构（非压缩）
{
	int				iSize;							//结构大小
	DWORD			dwVersion;		//版本号
	int             iOrgX;          //源Buffer位于目标Buffer的起始点X坐标   //ADD BY HAN
	int             iOrgY;          //源Buffer位于目标Buffer的起始点Y坐标   //ADD BY HAN
	int				iDesWidth;		//目标Buffer宽
	int				iDesHeight;		//目标Buffer高
	int				iOffsetX;		//图像在源Buffer中的偏移X坐标
	int				iOffsetY;		//图像在源Buffer中的偏移Y坐标
	int				iWidth;			//图像高
	int				iHeight;		//图像宽
	int				iSrcPitch;		//源Buffer Pitch
	int				iSrcHeight;		//源Buffer高
	int				iBitsPerPixel;	//像素所占位数
    DWORD           dwCurPos;		//图像所在帧号
	BYTE*			pData;			//图像数据
	PixelFormat		ePixFmt;		//图像格式
	BYTE*			pExData;		//附属图像数据
	int				iExDataSize;	//附属图像数据大小
	long			lTimeCode;		//时码
}TPImageData, *LPTPImageData;

typedef struct tag_TPAudioData
{
	int				iSize;
	DWORD			dwVersion;
	WORD			iChannels;				//通道数
	DWORD			dwSamplesRate;			//采样率
	WORD			iBitsPerSample;			//量化位数
	TP_MEDIA_CLASS_TYPE lMediaClassType;	//音频通道号
	ULONGLONG		ulStartSample;			//开始采样点位置
	ULONGLONG		ulSampleCount;			//采样数量
	BYTE*			pData;					//音频数据
}TPAudioData,*LPTPAudioData;

typedef struct tag_TPAudioFrameData		//某一帧音频数据结构（非压缩）
{
	int				iSize;			//结构大小
	DWORD			dwVersion;		//版本号
	TPAudioFormat	tpAudioFormat;	//音频格式
	TPTimeBase		tpTimeBase;		//帧速率（类似于视频标准）
	DWORD           dwCurPos;		//音频所在帧号
	BYTE*			pData;			//音频数据
}TPAudioFrameData, *LPTPAudioFrameData;

typedef struct tag_TPAVFrameData		//某一帧视音频数据（非压缩）
{
	int				iSize;							//结构大小
	DWORD			dwVersion;			//版本号
	int				iWidth;				//图像高
	int				iHeight;			//图像宽
	int				iBitPerPixel;		//像素所占位数
	DWORD			dwCurPos;			//所在帧号
	BYTE*			pVideoData;			//图像数据
	DWORD			dwAudioDataSize;	//音频数据大小
	BYTE*			pAudioData;			//音频数据
}TPAVFrameData,*LPTPAVFrameData;


typedef struct tag_TPAVStreamFrameData	//某一帧视音频压缩数据
{
	int				iSize;			//结构大小
	DWORD			dwVersion;		//版本号
	DWORD			dwCurPos;		//所在帧号
	BYTE*			pData;			//压缩数据
	DWORD			dwSize;			//数据长度
}TPAVStreamFrameData,*LPTPAVStreamFrameData;


typedef struct tag_TPAudioSegmentData	//某段音频数据的结构
{
	int				iSize;							//结构大小
	DWORD			dwVersion;			//版本号
	DWORD			dwInPoint;			//入点
	DWORD			dwOutPoint;			//出点
	DWORD			dwSampleRate;		//采样频率
	DWORD			dwSampleSize;		//采样数
	DWORD			dwValidBitsPerSample; //每个sample的有效位数
	DWORD			dwActualBitsPerSample; //每个sample的实际位数，即所占内存的位数
	DWORD			dwDataSize;			//采样数据大小
	BYTE*			pData;				//音频数据
}TPAudioSegmentData, *LPTPAudioSegmentData;

typedef struct tag_TPVideoSegmentData	//某段视频数据结构
{
	int				iSize;							//结构大小
	DWORD			dwVersion;			//版本号
	int				iWidth;
	int				iHeight;
	int				iBitsPerPixel;
	TPTimeBase		tpTimeBase;			//帧率
	DWORD			dwInPoint;			//入点
	DWORD			dwOutPoint;			//出点
	DWORD			dwSize;				//数据大小
	BYTE*			pData;				//视频数据
}TPVideoSegmentData,*LPTPVideoSegmentData;

/************视音频数据参数结构（含数据BUFFER）**********/







/************系统所支持的文件格式相关定义**********/

typedef struct tag_TPImageSize 
{
	DWORD dwWidth;
	DWORD dwHeight;
}TPImageSize,*LPTPImageSize;



//图象视频帧支持格式(未压缩)
typedef struct tag_TPImageFormatSu	
{
	DWORD	dwSize;					//结构大小
	DWORD	dwVersion;				//版本号

	DWORD	dwMaxWidth;				//图像宽
	DWORD	dwMaxHeight;			//图像高
	DWORD	dwMinWidth;				//图像宽
	DWORD	dwMiniHeight;			//图像高
	DWORD	dwPrefWidth;			//图像宽
	DWORD	dwPrefHeight;			//图像高

	BOOL	bStill;					//是否为静态图像

	long    lPixFormat;             //对于同一种压缩格式来说，有可能有不同的象素格式,可以根据不同的像素格式计算实际的字节数
	long    lAspectRatio;           //纵横比选项
	long    lScanMode;              //所支持的图像扫描模式

	long    lSuggestImageSizeNum;   //提供可供用户选择的几种尺寸设置
	TPImageSize* pImageSizeSu;

	DWORD   dwVideoStandard;  //支持制式hh

}TPImageFormatSu, *LPTPImageFormatSu;


//图象视频压缩参数支持格式(压缩)
typedef struct tag_TPVideoCompressParaSu
{
	DWORD   dwMinKeyFrame;
	DWORD   dwMaxKeyFrame;
	DWORD   dwPrefKeyFrame;				//关键帧

	DWORD   dwMinDataRate;				//数据率，以kbps为单位
	DWORD   dwMaxDataRate;				//数据率，以kbps为单位
	DWORD   dwPrefDataRate;				//数据率，以kbps为单位

	DWORD   dwPrivateDataSize;
	void*	pPrivateData;			//私有数据
}TPVideoCompressParaSu, *LPTPVideoCompressParaSu;


#define TP_ST_RATE_25				50			// PAL field frequency
#define	TP_ST_FRAME_RATE_25			25			// PAL frame  frequency
#define TP_ST_SCALE_25				1			// PAL scale

#define TP_ST_RATE_2997				5994		// NTSC field frequency
#define	TP_ST_FRAME_RATE_2997		2997		// NTSC frame  frequency
#define TP_ST_SCALE_2997			100		// NTSC scale

//#define TP_ST_RATE_2997				60000		// NTSC field frequency
//#define	TP_ST_FRAME_RATE_2997		30000		// NTSC frame  frequency
//#define TP_ST_SCALE_2997			1001		// NTSC scale

#define TP_ST_RATE_30				60			// 30-F field frequency
#define	TP_ST_FRAME_RATE_30			30			// 30-F frame frequency
#define TP_ST_SCALE_30				1			// 30-F scale

#define TP_ST_RATE_24				48			// 24-F field frequency
#define	TP_ST_FRAME_RATE_24			24			// 24-F field frequency
#define TP_ST_SCALE_24			    1			// 24-F scale

#define TP_ST_RATE_2398				48000		// 2398-F field frequency
#define	TP_ST_FRAME_RATE_2398		24000		// 2398-F frame frequency
#define TP_ST_SCALE_2398			1001        // 2398-F scale



#define FRAMES_SECOND_25		 25L      // 25 Frame: frames per second    
#define FRAMES_MINUTE_25		 1500L    // 25 Frame: frames per minute
#define FRAMES_HOUR_25			 90000L   // 25 Frame: frames per hour

#define FRAMES_SECOND_24		 24L      // 24 Frame: frames per second    
#define FRAMES_MINUTE_24		 1440L    // 24 Frame: frames per minute
#define FRAMES_HOUR_24			 86400L   // 24 Frame: frames per hour

#define FRAMES_SECOND_NODROP_30  30L      // 30 NO_DROP Frame: frames per second    
#define FRAMES_MINUTE_NODROP_30  1800L    // 30 NO_DROP Frame: frames per minute
#define FRAMES_HOUR_NODROP_30    108000L   // 30 NO_DROP Frame: frames per hour

#define FRAMES_MINUTE_30_DROP	 1798L    // 30 DROP Frame: frames per minute
#define FRAMES_10MINUTES_30_DROP 17982L   // 30 DROP Frame: frames per 10 minutes
#define FRAMES_HOUR_30_DROP		 107892L   // 30 DROP Frame: frames per hour



//视频支持格式定义（包括压缩格式和参数）
typedef struct tag_TPVideoFormatSu
{
	DWORD						dwSize;			//结构大小
	DWORD						dwVersion;		//版本号
	TCHAR				        ptszFormat[MAX_FILETYPE_NAME];		//视频格式名称
	DWORD						dwFOURCC;		//FOURCC Codes  //对应于编码类型等内容
	TP_ACCESS_MODE			    tpAccessType;  //表明可以进行压缩还是解压缩
//	TPImageFormatSu				tpImageFormatSu;//hh
	BOOL                        bIsCompress;
	TPVideoCompressParaSu       tpVCParaSu;

	long						lVideoStandSuNum;         //所支持的制式数量hh
	TPImageFormatSu*            pImageFormatSu;
    
	DWORD                       dwVideoStandSu;           //所支持的制式hh
	BOOL                        bRealtime;               //是否实时  
}TPVideoFormatSu,*LPTPVideoFormatSu;

// Audio support structure define
//	Audio bits support

#define TP_AudioBits_Bad	0x00000000

#define TP_AudioBits_8		0x000000001
#define	TP_AudioBits_16		0x000000002
#define TP_AudioBits_24	    0x000000004
#define TP_AudioBits_32	    0x000000008

#define TP_AudioBits_All	0xffffffff


//Audio Channel support

#define TP_AudioChannel_Bad     0x00000000

#define TP_AudioChannel_1	    0x00000001
#define TP_AudioChannel_2		0x00000002
#define TP_AudioChannel_4	    0x00000004
#define TP_AudioChannel_8	    0x00000008



#define TP_AudioChannel_All     0xffffffff



//Audio Sample support

#define TP_AudioSample_Bad   0x00000000

#define TP_AudioSample_8     0x00000001
#define TP_AudioSample_16    0x00000002
#define TP_AudioSample_22_05 0x00000004
#define TP_AudioSample_44_1  0x00000008
#define TP_AudioSample_48    0x00000010
#define TP_AudioSample_96    0x00000020
#define TP_AudioSample_192   0x00000040

#define TP_AudioSample_All   0xffffffff



//音频支持结构定义
typedef struct tag_TPAudioFormatSu    //包括音频压缩格式和参数
{
	DWORD	   				dwSize;				//结构大小
	DWORD	   				dwVersion;			//版本号

	TCHAR				    ptszFormat[MAX_FILETYPE_NAME];	//音频格式名称
	DWORD	   				dwFOURCC;			//FOURCC Codes
	TP_ACCESS_MODE			tpAccessType;		//表明可以进行压缩还是解压缩

	//音频基本信息
	long      				lChannelInfo;      //TP_AudioChannel
	long      				lSamplesInfo;	    //TPAudioSample
	long      				lBitsInfo;			//量化位数 : TP_AudioBits
	//音频基本信息

	//压缩参数
	BOOL       				bIsCompress;        //是否压缩
	DWORD	   				dwMinCompDataRate;	//数据率，以kbps为单位
	DWORD	   				dwMaxCompDataRate;	//数据率，以kbps为单位
	DWORD	   				dwPrefCompDataRate;	//数据率，以kbps为单位
	//压缩参数

	DWORD      				dwPrivateSize;
	void*      				pPrivateData;
}TPAudioFormatSu,*LPTPAudioFormatSu;


typedef struct tab_TPExtNameTable			//扩展名表
{
	DWORD	iSize;					//结构大小
	DWORD	dwVersion;			//版本号
	TCHAR	ptszExtName[MAX_EXT_NAME_SIZE];	//可能支持多种文件扩展名类型	
}TPExtNameTable,*LPTPExtNameTable;

//文件支持格式定义（包括压缩格式和参数），压缩文件插件和各引擎需要给出针对以下结构的列表
typedef struct tag_TPFileFormatSu
{
	DWORD               dwSize;
	DWORD               dwVersion;

	TCHAR				ptszFileTypeGroup[MAX_FILETYPE_NAME];		//文件类型所属组
	//例如多种avi文件可同属于avi大类内

	long				lExtNameCount;							//支持文件扩展名数量
	LPTPExtNameTable	lpExtNameTable;							//扩展名表(内置于插件里)

	GUID                guidFileType;
	TCHAR				ptszFileTypeName[MAX_FILETYPE_NAME];		//文件类型名称
	TP_ACCESS_MODE      tpAccessMode;

	BOOL				bCanVideo;
	BOOL				bCanAudio;

	long                lVideoFormatSuNum;
	TPVideoFormatSu*    pVideoFormatSu;

	long                lAudioFormatSuNum;
	TPAudioFormatSu*    pAudioFormatSu;

	DWORD               dwExtraType;
	DWORD               dwVideoStandardSu;//所支持的制式 hh

}TPFileFormatSu,*LPTPFileFormatSu;




//场景检测系统设置数据结构
typedef struct tag_TPDetectPrecisonStu
{
	DWORD dwVersion;
	long lSensitivity;
	long lMinLength;
	BOOL bDistrubClear;
	long lDisturbthreshold;
	BOOL bShotMerger;
	long lMerge;
	long lShotType;
}TPDetectPrecisonStu,*LPTPDetectPrecisonStu;
//场景检测系统设置数据结构


/******************视频格式Group类型******************
根据字符串进行标识
******************视频格式Group类型******************/


//参数改变结构定义
typedef struct tag_TPParamChangeStu
{
	int iIndex;  
	int iSubIndexCount;    //mini  param adj时候借用
	int* pIndexArray;  

	tag_TPParamChangeStu()
	{
		iIndex = -1;
		iSubIndexCount = -1;//勿改
		pIndexArray = NULL;
	};

}TPParamChangeStu, *LPTPParamChangeStu;



typedef struct tag_TPEffectLinkParaStu
{
	long lParamIndex;
	long lSubParamIndex;
}TPEffectLinkParaStu,*LPTPEffectLinkParaStu;


typedef struct tag_TPEffectLinkParaStuEx
{
	long lLinkNum;
	LPTPEffectLinkParaStu pLinkParaStu;
}TPEffectLinkParaStuEx,*LPTPEffectLinkParaStuEx;


typedef struct tag_TPEffectSet_Grid
{
	long lIntervalX;
	long lIntervalY;
	long lOffsetX;
	long lOffsetY;
	DWORD dwColor;

	void Reset()
	{
		lIntervalX = 50;
		lIntervalY = 50;
		lOffsetX = 0;
		lOffsetY = 0;
		//dwColor = RGB(0,0,0);
		//dwColor = RGBA(255,255,255,255);
		dwColor = 0xffffffff;
	}

	tag_TPEffectSet_Grid()
	{
		Reset();
	}
}TPEffectSet_Grid,*LPTPEffectSet_Grid;

typedef struct tag_Face_Area   //辛力 add 20110601
{
	int     nID;
	double  dX;
	double  dY;
	double  dWidth;
	double  dHeight;

	tag_Face_Area() : nID(-1), dX(0.0), dY(0.0), dWidth(0.0), dHeight(0.0)
	{
	}

	tag_Face_Area(const tag_Face_Area &SrcFace_area ) : nID(SrcFace_area.nID), 
		dX(SrcFace_area.dX),
		dY(SrcFace_area.dY),
		dWidth(SrcFace_area.dWidth),
		dHeight(SrcFace_area.dHeight)
	  {	
	  }
}FaceArea;
typedef std::list<FaceArea> FaceAreaList;
typedef struct tag_TPTextOut
{
	long lOffsetX;
	long lOffsetY;
	DWORD dwColor;
	TCHAR strTextout[512];
	void Reset()
	{
		lOffsetX = 10;
		lOffsetY = 10;
		dwColor = RGB(255,255,255);
		memset(strTextout,0,sizeof(strTextout));
	}

	tag_TPTextOut()
	{
		Reset();
	}
}TPTextOut,*LPTPTextOut;

enum TP_SPLITVIEW_RES
{
	TPEFF_SPLITVIEW_TRUEFX_BOTTOM = 0,  // 0
	TPEFF_SPLITVIEW_TRUEFX_LEFT,        // 1
	TPEFF_SPLITVIEW_TRUEFX_TOP,         // 2
	TPEFF_SPLITVIEW_TRUEFX_RIGHT,       // 3

	TPEFF_SPLITVIEW_FXTRUE_BOTTOM ,
	TPEFF_SPLITVIEW_FXTRUE_LEFT,
	TPEFF_SPLITVIEW_FXTRUE_TOP,
	TPEFF_SPLITVIEW_FXTRUE_RIGHT,

	TPSPLITVIEW_PAINTORRES_COUNT,
};
typedef struct tag_TPSplitMark
{
	BOOL			 bShowSplit;
	FLOAT			 rcLeft;
	FLOAT			 rcRight;
	FLOAT			 rcTop;
	FLOAT			 rcBottom;
	UINT			 type;

	tag_TPSplitMark()
		:bShowSplit(FALSE),
		type(TPEFF_SPLITVIEW_TRUEFX_BOTTOM),
		rcLeft(0.f),
		rcRight(0.f),
		rcTop(0.f),
		rcBottom(0.f)
	{
	}

}TPSplitMark, *LPTPSplitMark;

typedef struct  tag_TPVideoDispProperty
{
	double  dRatio;  //按百分比
	BOOL    bBorder;
	long    lSafeBoxX;//取1个即可
	long    lSafeBoxY;
	BOOL	b169Border;
	BOOL    bCGBorder;
	long    lCGSafeBoxX;//取1个即可
	long    lCGSafeBoxY;
	BOOL	bCG169Border;
	BOOL    bOnlyMove;
	BOOL    bOnlyZoom;
	POINT   ptOffset;
	BOOL    bDrawGrid;
	TPEffectSet_Grid tpGrid;
	TPTextOut tpTextOut;//文字输出
	BOOL	bShowZebra;
	TPSplitMark	tpSplitMark;
	BOOL    bDrawCenterXY;
	double  dCenterX;
	double  dCenterY;
	BOOL    bIsPlay;
	//[[add for display overlay timecode.
	BOOL	bOverlayDisplay;
	float	fOverlayPosX;
	float	fOverlayPosY;
	BYTE*	pOverlayOutBuffer;
	int		nOverlayBufWidth;
	int		nOverlayBufHeight;
	PixelFormat iOverlayPixelFormat;
	int		nOverlayDisplayID;
	//]]
	void Reset()
	{
		bIsPlay = FALSE;
		dRatio = 100;
		bBorder = FALSE;
		bOnlyMove = FALSE;
		bOnlyZoom = FALSE;
		bCGBorder = FALSE;
		lCGSafeBoxX = 0;//取1个即可
		lCGSafeBoxY = 0;
		ptOffset.x = 0;
		ptOffset.y = 0;
		lSafeBoxX = 0;
		lSafeBoxY = 0; //
		bDrawGrid = FALSE;
		tpGrid.Reset();
		tpTextOut.Reset();
		bShowZebra = FALSE;
		bDrawCenterXY = FALSE;
	    dCenterX = 0;
		dCenterY = 0;
		//[[overlay
		bOverlayDisplay			= FALSE;
		fOverlayPosX			= 0.5;
		fOverlayPosY			= 0.5;
		pOverlayOutBuffer		= NULL;
		nOverlayBufWidth		= 0;
		nOverlayBufHeight		= 0;
		iOverlayPixelFormat		= PIX_FMT_ARGB32;
		nOverlayDisplayID				= -1;
		//]]
	}

	tag_TPVideoDispProperty()
	{
		Reset();
	};

}TPVideoDispProperty,*LPTPVideoDispProperty;

typedef struct  tag_TPVideoTrimDisplayProperty
{
	TPTextOut tpTextOut;//文字输出
	BOOL bDrawMarkPt;
	BOOL bDrawTLIn;
	BOOL bDrawTLOut;
	BOOL bDrawTrimIn;
	BOOL bDrawTrimOut;
	BOOL bDrawSegIn;
	BOOL bDrawSegOut;
	BOOL bDrawEndPoint;
	int	 nSegType;
	float fSpeedShow;
	BOOL bShowSpeed;
	BOOL    bDrawFaceArea;
	FaceAreaList  sFaceAreaList;
	void Reset()
	{
		bDrawMarkPt	= FALSE;
		bDrawTLIn	= FALSE;
		bDrawTLOut	= FALSE;
		bDrawTrimIn	= FALSE;
		bDrawTrimOut= FALSE;
		bDrawSegIn  = FALSE;
		bDrawSegOut = FALSE;
		nSegType = 0;
		tpTextOut.Reset();
		bDrawEndPoint = FALSE;
		fSpeedShow = 0;
		bShowSpeed = FALSE;
		bDrawFaceArea = FALSE;
		sFaceAreaList.clear();
	}

	tag_TPVideoTrimDisplayProperty()
	{
		Reset();
	};
	 tag_TPVideoTrimDisplayProperty &operator = (tag_TPVideoTrimDisplayProperty& tpSrc)
	 {
		 tpTextOut    = tpSrc.tpTextOut;
		 bDrawMarkPt  = tpSrc.bDrawMarkPt;
		 bDrawTLIn    = tpSrc.bDrawTLIn;
		 bDrawTLOut	  = tpSrc.bDrawTLOut;
		 bDrawTrimIn  = tpSrc.bDrawTrimIn;
		 bDrawTrimOut = tpSrc.bDrawTrimOut;
		 bDrawSegIn   = tpSrc.bDrawSegIn;
		 bDrawSegOut  = tpSrc.bDrawSegOut;
		 nSegType     = tpSrc.nSegType;
		
		 bDrawEndPoint = tpSrc.bDrawEndPoint;
		 fSpeedShow    = tpSrc.fSpeedShow;
		 bShowSpeed    = tpSrc.bShowSpeed;
		 bDrawFaceArea = tpSrc.bDrawFaceArea;
		 sFaceAreaList = tpSrc.sFaceAreaList;

		 return *this;
	 }

}TPVideoTrimDisplayProperty,*LPTPVideoTrimDisplayProperty;

typedef struct tag_TPNSGeneralPropData
{
	BOOL        bDropFrame;
	BOOL        bCreateNewSequence;
	BOOL        bOpenLoggingTool;
    short       sShuttleMaxmag;
    BOOL        bEnableSlow;
    BOOL        bShuttleX1X0;
    short       sShuttleX1Angle;
    short       sShuttleX0Angle;
    short       sJogAcceleration;
    short       sShuttleLimitAngle;
    short       sDMCClickAngle;
    short       sDMCLimitAngle;
    short       sMaxHoldtime;
    short       sCenterHoldtime;
    short       sCenterIntensity;
    short       sInterMediateIntensity;
    BOOL        bEnableC5Beep;

    tag_TPNSGeneralPropData()
    {
		bDropFrame         = FALSE;
		bCreateNewSequence = FALSE;
		bOpenLoggingTool   = FALSE;
        sShuttleMaxmag     =  0;
        bEnableSlow        = FALSE;
        bShuttleX1X0       = FALSE;
        sShuttleX1Angle    =  0;
        sShuttleX0Angle    =  0;
        sJogAcceleration   =  0;
        sShuttleLimitAngle =  0;
        sDMCClickAngle     =  0;
        sDMCLimitAngle     =  0;
        sMaxHoldtime       =  0;
        sCenterHoldtime    =  0;
        sCenterIntensity   =  0;
        sInterMediateIntensity   =  0;
        bEnableC5Beep      = FALSE;
    }
    ~tag_TPNSGeneralPropData()
    {  
		bDropFrame         = FALSE;
		bCreateNewSequence = FALSE;
		bOpenLoggingTool   = FALSE;
        sShuttleMaxmag     =  0;
        bEnableSlow        = FALSE;
        bShuttleX1X0       = FALSE;
        sShuttleX1Angle    =  0;
        sShuttleX0Angle    =  0;
        sJogAcceleration   =  0;
        sShuttleLimitAngle =  0;
        sDMCClickAngle     =  0;
        sDMCLimitAngle     =  0;
        sMaxHoldtime       =  0;
        sCenterHoldtime    =  0;
        sCenterIntensity   =  0;
        sInterMediateIntensity   =  0;
        bEnableC5Beep      = FALSE;
    }


    tag_TPNSGeneralPropData &operator = (tag_TPNSGeneralPropData& tpStruct)
    {
		bDropFrame         = tpStruct.bDropFrame;
		bCreateNewSequence = tpStruct.bCreateNewSequence;
		bOpenLoggingTool   = tpStruct.bOpenLoggingTool;
        sShuttleMaxmag     =  tpStruct.sShuttleMaxmag;
        bEnableSlow        =  tpStruct.bEnableSlow;
        bShuttleX1X0       =  tpStruct.bShuttleX1X0;
        sShuttleX1Angle    =  tpStruct.sShuttleX1Angle;
        sShuttleX0Angle    =  tpStruct.sShuttleX0Angle;
        sJogAcceleration   =  tpStruct.sJogAcceleration;
        sShuttleLimitAngle =  tpStruct.sShuttleLimitAngle;
        sDMCClickAngle     =  tpStruct.sDMCClickAngle;
        sDMCLimitAngle     =  tpStruct.sDMCLimitAngle;
        sMaxHoldtime       =  tpStruct.sMaxHoldtime;
        sCenterHoldtime    =  tpStruct.sCenterHoldtime;
        sCenterIntensity   =  tpStruct.sCenterIntensity;
        sInterMediateIntensity   =  tpStruct.sInterMediateIntensity;
        bEnableC5Beep      = tpStruct.bEnableC5Beep;

        return *this;
    }

    DWORD GetDataSize()
    {
        DWORD dwDataSize = 0;
		dwDataSize += sizeof(BOOL);
		dwDataSize += sizeof(BOOL);
		dwDataSize += sizeof(BOOL);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(BOOL);
        dwDataSize += sizeof(BOOL);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(short);
        dwDataSize += sizeof(BOOL);
        return dwDataSize;
    }

    DWORD SaveToBuf(long& lSizeTemp,BYTE* pBuffer)
    {
		*((BOOL *)pBuffer) = bDropFrame;
		lSizeTemp  += sizeof(BOOL);
		pBuffer  += sizeof(BOOL);

		*((BOOL *)pBuffer) = bCreateNewSequence;
		lSizeTemp  += sizeof(BOOL);
		pBuffer  += sizeof(BOOL);

		*((BOOL *)pBuffer) = bOpenLoggingTool;
		lSizeTemp  += sizeof(BOOL);
		pBuffer  += sizeof(BOOL);

        *((short *)pBuffer) = sShuttleMaxmag;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((BOOL *)pBuffer) = bEnableSlow;
        lSizeTemp  += sizeof(BOOL);
        pBuffer  += sizeof(BOOL);

        *((BOOL *)pBuffer) = bShuttleX1X0;
        lSizeTemp  += sizeof(BOOL);
        pBuffer  += sizeof(BOOL);

        *((short *)pBuffer) = sShuttleX1Angle;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sShuttleX0Angle;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sJogAcceleration;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sShuttleLimitAngle;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sDMCClickAngle;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sDMCLimitAngle;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sMaxHoldtime;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sCenterHoldtime;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sCenterIntensity;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((short *)pBuffer) = sInterMediateIntensity;
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        *((BOOL *)pBuffer) = bEnableC5Beep;
        lSizeTemp  += sizeof(BOOL);
        pBuffer  += sizeof(BOOL);
        return 1;
    }

    DWORD RestoreFromBuf(long lDataSize,BYTE* pBuffer)
    {
        long lSizeTemp = 0;

		bDropFrame = *((BOOL *)pBuffer);
		lSizeTemp  += sizeof(BOOL);
		pBuffer  += sizeof(BOOL);
	
		bCreateNewSequence = *((BOOL *)pBuffer);
		lSizeTemp  += sizeof(BOOL);
		pBuffer  += sizeof(BOOL);

		bOpenLoggingTool = *((BOOL *)pBuffer);
		lSizeTemp  += sizeof(BOOL);
		pBuffer  += sizeof(BOOL);

        sShuttleMaxmag = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        bEnableSlow = *((BOOL *)pBuffer);
        lSizeTemp  += sizeof(BOOL);
        pBuffer  += sizeof(BOOL);

        bShuttleX1X0 = *((BOOL *)pBuffer);
        lSizeTemp  += sizeof(BOOL);
        pBuffer  += sizeof(BOOL);

        sShuttleX1Angle = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sShuttleX0Angle = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sJogAcceleration = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sShuttleLimitAngle = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sDMCClickAngle = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sDMCLimitAngle = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sMaxHoldtime = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sCenterHoldtime = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sCenterIntensity = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        sInterMediateIntensity = *((short *)pBuffer);
        lSizeTemp  += sizeof(short);
        pBuffer  += sizeof(short);

        bEnableC5Beep = *((BOOL *)pBuffer) ;
        lSizeTemp  += sizeof(BOOL);
        pBuffer  += sizeof(BOOL);

        return lSizeTemp;
    }

}TPNSGeneralPropData,*LPTPNSGeneralPropData;

//[[用来记录当前引擎的很多配置信息
struct  TPEngineConfigPara
{
	int		nCGProxyEditmode;//proxy mode cg Full edit or not
	int		nCGScrollMode;	//scroll mode 
	BOOL	nAudioMuteWhenNRealTime;//not realtime audio mute.
	unsigned int nCGMemoryLimit;	//cg use total mem.
	long    lRealSDlProxyRate;		
	long    lRealHDlProxyRate;
	BOOL	bField;//Frame or field 主要是720P和Proxy编辑。
	BOOL	bFieldTwiceRedner;
	long    lSoundCardDelayTime;  //声卡延迟时间/for sdi-声卡同步

	// tnt add-09.08.10 --- 用来记录音频帧被分割的份数，为了被1920整除，此数应为16的倍数
	int     nCutCount;
	// tnt end

	//tnt add(tanting) - 10.08.20 --- 增加音频新流程（threadpool）的标志, 0表示不采用，1表示采用
	int     nAudioThreadPool;
	//tnt end

	void Reset()
	{
		nCGProxyEditmode		= 1;//0为cg为proxy编辑。1为cg高质量编辑，engine进行缩放。
		nCGScrollMode			= 1;//0为用videort，1为用2048rt
		nAudioMuteWhenNRealTime = 1;//0 为静音 1为不静音。
		nCGMemoryLimit			= 100000000;	// 100MB
		lRealSDlProxyRate       = 2;
		lRealHDlProxyRate       = 4;
		bField					= TRUE;//Frame or field 主要是720P和Proxy编辑。
		bFieldTwiceRedner		= TRUE;

		// tnt add-09.08.11 --- 用来记录音频帧被分割的份数，为了被1920整除，此数应为16的倍数
		nCutCount               = 16;
		lSoundCardDelayTime     = 40;
		// tnt end

		//tnt add(tanting) - 10.08.20 --- 增加音频新流程（threadpool）的标志, 0表示不采用，1表示采用
		nAudioThreadPool        = 0;
		//tnt end
	}
	BOOL copy(TPEngineConfigPara tpSourcePara)
	{
		nCGProxyEditmode		= tpSourcePara.nCGProxyEditmode;//0为cg为proxy编辑。1为cg高质量编辑，engine进行缩放。
		nCGScrollMode			= tpSourcePara.nCGScrollMode;
		nAudioMuteWhenNRealTime = tpSourcePara.nAudioMuteWhenNRealTime;
		nCGMemoryLimit			= tpSourcePara.nCGMemoryLimit;
		lRealSDlProxyRate       = tpSourcePara.lRealSDlProxyRate;
		lRealHDlProxyRate       = tpSourcePara.lRealHDlProxyRate;
		bField					= tpSourcePara.bField;
		bFieldTwiceRedner		= tpSourcePara.bFieldTwiceRedner;
		lSoundCardDelayTime     = tpSourcePara.lSoundCardDelayTime;

		// tnt add-09.08.11 --- 用来记录音频帧被分割的份数，为了被1920整除，此数应为16的倍数
		nCutCount               = tpSourcePara.nCutCount;
		// tnt end

		//tnt add(tanting) - 10.08.20 --- 增加音频新流程（threadpool）的标志, 0表示不采用，1表示采用
		nAudioThreadPool        = tpSourcePara.nAudioThreadPool;
		//tnt end

		return TRUE;
	}

	TPEngineConfigPara()
	{
		Reset();
	};

};
typedef enum _Present_Window_type_
{
	PRESENT_WND_MV = 0,
	PRESENT_WND_SV,
	PRESENT_WND_EFFECT_EDIT,
	PRESENT_WND_CG_EDIT,
	PRESENT_WND_UPLOAD_SHOW,
	PRESENT_WND_CLIP_IMPORT,
	PRESENT_WND_ICON_PLAY,
	PRESENT_WND_MAX,
};
//]]
#endif// _INC_TPBASEVADEF_H