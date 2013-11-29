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




enum	TP_VIDEO_STANDARD	// ������Ƶ��ʽ��׼ö�ٶ���
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
	TP_VIDEOSTANDARD_NTSC_2997_480		= 0x00400000,		//NTSC size:720*480  f/s : 29.97  //�����ţ���ֹ�Ժ���������ٸĳ�480
	TP_VIDEOSTANDARD_NTSC_30_480		= 0x00800000,		//NTSC size:720*480 f/s : 30 //�����ţ���ֹ�Ժ���������ٸĳ�480


	TP_VIDEOSTANDARD_1280_1080_50i		= 0x01000000,	    //HD  size:1280*1080 f/s : 25 interlaced
	TP_VIDEOSTANDARD_1280_1080_5994i	= 0x02000000,	    //HD  size:1280*1080 f/s : 29.97 interlaced
	TP_VIDEOSTANDARD_1280_1080_60i		= 0x04000000,	    //HD  size:1280*1080 f/s : 30 interlaced
};


#define TP_VIDEOSTANDARD_ALL 0x0fffffff   //ȱʡ֧��������ʽ hh



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

/************��Ƶ��ʽ�����ṹ***************/

typedef struct tag_TPTimeBase		//��Ƶ��ʽ����
{
	int		iSize;							//�ṹ��С
	DWORD	dwVersion;			//�汾��
	int		lTimeRate;			//15,24,25,2997,30
	int		lTimeScale;			//1����100
	long    lWidth;
	long    lHeight;
}TPTimeBase, *LPTPTimeBase;

/************��Ƶ��ʽ�����ṹ***************/

//�ļ�����
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
// ͼƬ���� alpha ��Ϣ
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


	PIX_FMT_SPLIT_ALPHA = 0x00040000 ///�Ƿ���ж�����alpha buffer
};


#define  PIX_FMT_RGB_TYPE (PIX_FMT_ARGB32 | PIX_FMT_RGBA32 | PIX_FMT_RGB24 | PIX_FMT_BGR24 | PIX_FMT_RGB565 | PIX_FMT_RGB555)
#define  PIX_FMT_YUV_TYPE (PIX_FMT_YUYV | PIX_FMT_YUAYVA | PIX_FMT_UYVY | PIX_FMT_UYAVYA | PIX_FMT_YUV422P | PIX_FMT_YUV444P | PIX_FMT_YUV410P | PIX_FMT_YUV411P | PIX_FMT_YUV420P | PIX_FMT_YUVA)
#define  PIX_FMT_A_TYPE (PIX_FMT_ARGB32 | PIX_FMT_RGBA32 | PIX_FMT_GRAY8 | PIX_FMT_YUAYVA | PIX_FMT_UYAVYA | PIX_FMT_YUVA)


/***********��Ƶ��ʽ�����ṹ*******************/


typedef struct tag_TPAudioFormat    //��Ƶ��ʽ(δѹ��)
{
	WORD					iSize;					//�ṹ��С
	DWORD					dwVersion;				//�汾��
	WORD					iChannels;				//ͨ����
	DWORD					iSamplesRate;			//������
	WORD					iBitsPerSample;			//����λ��

	//tnt add(tanting) - 10.11.9
	WORD                    iSeekCount;             //��֡seekʱ��¼seek��samplecount
	//tnt end
}TPAudioFormat,*LPTPAudioFormat;

typedef struct tag_TPAudioCompressPara		//��Ƶѹ������
{
	WORD	iSize;					//�ṹ��С
	DWORD	dwVersion;				//�汾��
	DWORD	iDataRate;				//�����ʣ���kbpsΪ��λ
	DWORD	iInterleave;			//����֡��
	DWORD   dwPrivateDataSize;
	void*	pPrivateData;			//˽������
}TPAudioCompressPara,*LPTPAudioCompressPara;

typedef struct tag_TPAudioFormatEx    //������Ƶѹ����ʽ�Ͳ���
{
	DWORD					iSize;							//�ṹ��С
	DWORD					dwVersion;			//�汾��
	TPAudioFormat			tpAudioFormat;
	DWORD					dwFOURCC;			//FOURCC Codes
	TPAudioCompressPara		tpAudioCompressPara;	//ѹ������
}TPAudioFormatEx,*LPTPAudioFormatEx;

/***********��Ƶ��ʽ�����ṹ*******************/


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

//�ֱ��ʽṹͳһ����
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
/***********��Ƶ��ͼ���ʽ�����ṹ*****************/
typedef struct tag_TPImageFormat	//ͼ����Ƶ֡��ʽ(δѹ��)
{
	DWORD			iSize;					//�ṹ��С
	DWORD			dwVersion;				//�汾��
	DWORD			iWidth;					//ͼ���
	DWORD			iHeight;			 	//ͼ���
	DWORD			iBitsPerPixel; 	   		//ÿ��������ռλ��
	DWORD			iRowbytes;				//ÿ����ռ�ֽ���
	DWORD			iPixFmt;				//���ظ�ʽ  ��ӦPixelFormat
	DWORD			dwFrameSize;			//ÿ֡ͼ��Ĵ�С
	DWORD			iAspectRatioNum;		//�ݺ�ȷ���
	DWORD			iAspectRatioDen;		//�ݺ�ȷ�ĸ
	BOOL			bStill;					//�Ƿ�Ϊ��̬ͼ��
	TP_SCANMODE     tpScanMode;             //ͼ��ɨ��ģʽ
	DWORD           dwPrivateDataSize;
	void*       	pPrivateData;			//˽������
}TPImageFormat, *LPTPImageFormat;


typedef struct tag_TPVideoCompressPara		//��Ƶѹ������
{
	DWORD			iSize;							//�ṹ��С
	DWORD			dwVersion;				//�汾��
	DWORD			iKeyFrame;				//�ؼ�֡
	DWORD			iDataRate;				//�����ʣ���kbpsΪ��λ
	DWORD           dwPrivateDataSize;
	DWORD           dwCompressType;
	void*			pPrivateData;			//˽������
}TPVideoCompressPara,*LPTPVideoCompressPara;


//scan mode ?????
typedef struct tag_TPVideoFormat			//��Ƶ��ʽ������ѹ����ʽ�Ͳ�����
{
	DWORD					iSize;							//�ṹ��С
	DWORD					dwVersion;		//�汾��
	TPTimeBase				tpTimeBase;		//֡���ʣ���������Ƶ��׼��
	TP_VIDEO_STANDARD       tpVideoStandard;
	TPImageFormat			tpImageFormat;
	DWORD					dwFOURCC;			//FOURCC Codes
	TPVideoCompressPara		tpVideoCompressPara;	//ѹ������
	DWORD                   dwSubMediaType;  //��ʱʹ��,һ�㲻Ҫ��
}TPVideoFormat,*LPTPVideoFormat;

/***********��Ƶ��ͼ���ʽ�����ṹ*****************/



/**********�ļ���ʽ�����ṹ*********************/

typedef struct tag_TPFileFormat					//�ļ���ʽ
{
	DWORD				iSize;					//�ṹ��С
	DWORD				dwVersion;				//�汾��
	BOOL 				bHasVideo;				//�Ƿ�����Ƶ
	BOOL 				bHasAudio;				//�Ƿ�����Ƶ
	UINT                uParam;                 //�м䴫�ݲ��� 
	GUID				guidFileType;			//�ļ�����GUID
	DWORD               dwFileHeadSize;			//�ļ�ͷ�ߴ�
	TPVideoFormat		tpVideoFormat;			//��Ƶ��ʽ
	TPAudioFormatEx		tpAudioFormatEx;			//��Ƶ��ʽ
}TPFileFormat,*LPTPFileFormat;


/**********�ļ���ʽ�����ṹ*********************/




/************����Ƶ���ݲ����ṹ��������BUFFER��**********/

//need change
typedef struct tag_TPImageData		//ĳһ֡ͼ���ͼ�����ݽṹ����ѹ����
{
	int				iSize;							//�ṹ��С
	DWORD			dwVersion;		//�汾��
	int             iOrgX;          //ԴBufferλ��Ŀ��Buffer����ʼ��X����   //ADD BY HAN
	int             iOrgY;          //ԴBufferλ��Ŀ��Buffer����ʼ��Y����   //ADD BY HAN
	int				iDesWidth;		//Ŀ��Buffer��
	int				iDesHeight;		//Ŀ��Buffer��
	int				iOffsetX;		//ͼ����ԴBuffer�е�ƫ��X����
	int				iOffsetY;		//ͼ����ԴBuffer�е�ƫ��Y����
	int				iWidth;			//ͼ���
	int				iHeight;		//ͼ���
	int				iSrcPitch;		//ԴBuffer Pitch
	int				iSrcHeight;		//ԴBuffer��
	int				iBitsPerPixel;	//������ռλ��
    DWORD           dwCurPos;		//ͼ������֡��
	BYTE*			pData;			//ͼ������
	PixelFormat		ePixFmt;		//ͼ���ʽ
	BYTE*			pExData;		//����ͼ������
	int				iExDataSize;	//����ͼ�����ݴ�С
	long			lTimeCode;		//ʱ��
}TPImageData, *LPTPImageData;

typedef struct tag_TPAudioData
{
	int				iSize;
	DWORD			dwVersion;
	WORD			iChannels;				//ͨ����
	DWORD			dwSamplesRate;			//������
	WORD			iBitsPerSample;			//����λ��
	TP_MEDIA_CLASS_TYPE lMediaClassType;	//��Ƶͨ����
	ULONGLONG		ulStartSample;			//��ʼ������λ��
	ULONGLONG		ulSampleCount;			//��������
	BYTE*			pData;					//��Ƶ����
}TPAudioData,*LPTPAudioData;

typedef struct tag_TPAudioFrameData		//ĳһ֡��Ƶ���ݽṹ����ѹ����
{
	int				iSize;			//�ṹ��С
	DWORD			dwVersion;		//�汾��
	TPAudioFormat	tpAudioFormat;	//��Ƶ��ʽ
	TPTimeBase		tpTimeBase;		//֡���ʣ���������Ƶ��׼��
	DWORD           dwCurPos;		//��Ƶ����֡��
	BYTE*			pData;			//��Ƶ����
}TPAudioFrameData, *LPTPAudioFrameData;

typedef struct tag_TPAVFrameData		//ĳһ֡����Ƶ���ݣ���ѹ����
{
	int				iSize;							//�ṹ��С
	DWORD			dwVersion;			//�汾��
	int				iWidth;				//ͼ���
	int				iHeight;			//ͼ���
	int				iBitPerPixel;		//������ռλ��
	DWORD			dwCurPos;			//����֡��
	BYTE*			pVideoData;			//ͼ������
	DWORD			dwAudioDataSize;	//��Ƶ���ݴ�С
	BYTE*			pAudioData;			//��Ƶ����
}TPAVFrameData,*LPTPAVFrameData;


typedef struct tag_TPAVStreamFrameData	//ĳһ֡����Ƶѹ������
{
	int				iSize;			//�ṹ��С
	DWORD			dwVersion;		//�汾��
	DWORD			dwCurPos;		//����֡��
	BYTE*			pData;			//ѹ������
	DWORD			dwSize;			//���ݳ���
}TPAVStreamFrameData,*LPTPAVStreamFrameData;


typedef struct tag_TPAudioSegmentData	//ĳ����Ƶ���ݵĽṹ
{
	int				iSize;							//�ṹ��С
	DWORD			dwVersion;			//�汾��
	DWORD			dwInPoint;			//���
	DWORD			dwOutPoint;			//����
	DWORD			dwSampleRate;		//����Ƶ��
	DWORD			dwSampleSize;		//������
	DWORD			dwValidBitsPerSample; //ÿ��sample����Чλ��
	DWORD			dwActualBitsPerSample; //ÿ��sample��ʵ��λ��������ռ�ڴ��λ��
	DWORD			dwDataSize;			//�������ݴ�С
	BYTE*			pData;				//��Ƶ����
}TPAudioSegmentData, *LPTPAudioSegmentData;

typedef struct tag_TPVideoSegmentData	//ĳ����Ƶ���ݽṹ
{
	int				iSize;							//�ṹ��С
	DWORD			dwVersion;			//�汾��
	int				iWidth;
	int				iHeight;
	int				iBitsPerPixel;
	TPTimeBase		tpTimeBase;			//֡��
	DWORD			dwInPoint;			//���
	DWORD			dwOutPoint;			//����
	DWORD			dwSize;				//���ݴ�С
	BYTE*			pData;				//��Ƶ����
}TPVideoSegmentData,*LPTPVideoSegmentData;

/************����Ƶ���ݲ����ṹ��������BUFFER��**********/







/************ϵͳ��֧�ֵ��ļ���ʽ��ض���**********/

typedef struct tag_TPImageSize 
{
	DWORD dwWidth;
	DWORD dwHeight;
}TPImageSize,*LPTPImageSize;



//ͼ����Ƶ֧֡�ָ�ʽ(δѹ��)
typedef struct tag_TPImageFormatSu	
{
	DWORD	dwSize;					//�ṹ��С
	DWORD	dwVersion;				//�汾��

	DWORD	dwMaxWidth;				//ͼ���
	DWORD	dwMaxHeight;			//ͼ���
	DWORD	dwMinWidth;				//ͼ���
	DWORD	dwMiniHeight;			//ͼ���
	DWORD	dwPrefWidth;			//ͼ���
	DWORD	dwPrefHeight;			//ͼ���

	BOOL	bStill;					//�Ƿ�Ϊ��̬ͼ��

	long    lPixFormat;             //����ͬһ��ѹ����ʽ��˵���п����в�ͬ�����ظ�ʽ,���Ը��ݲ�ͬ�����ظ�ʽ����ʵ�ʵ��ֽ���
	long    lAspectRatio;           //�ݺ��ѡ��
	long    lScanMode;              //��֧�ֵ�ͼ��ɨ��ģʽ

	long    lSuggestImageSizeNum;   //�ṩ�ɹ��û�ѡ��ļ��ֳߴ�����
	TPImageSize* pImageSizeSu;

	DWORD   dwVideoStandard;  //֧����ʽhh

}TPImageFormatSu, *LPTPImageFormatSu;


//ͼ����Ƶѹ������֧�ָ�ʽ(ѹ��)
typedef struct tag_TPVideoCompressParaSu
{
	DWORD   dwMinKeyFrame;
	DWORD   dwMaxKeyFrame;
	DWORD   dwPrefKeyFrame;				//�ؼ�֡

	DWORD   dwMinDataRate;				//�����ʣ���kbpsΪ��λ
	DWORD   dwMaxDataRate;				//�����ʣ���kbpsΪ��λ
	DWORD   dwPrefDataRate;				//�����ʣ���kbpsΪ��λ

	DWORD   dwPrivateDataSize;
	void*	pPrivateData;			//˽������
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



//��Ƶ֧�ָ�ʽ���壨����ѹ����ʽ�Ͳ�����
typedef struct tag_TPVideoFormatSu
{
	DWORD						dwSize;			//�ṹ��С
	DWORD						dwVersion;		//�汾��
	TCHAR				        ptszFormat[MAX_FILETYPE_NAME];		//��Ƶ��ʽ����
	DWORD						dwFOURCC;		//FOURCC Codes  //��Ӧ�ڱ������͵�����
	TP_ACCESS_MODE			    tpAccessType;  //�������Խ���ѹ�����ǽ�ѹ��
//	TPImageFormatSu				tpImageFormatSu;//hh
	BOOL                        bIsCompress;
	TPVideoCompressParaSu       tpVCParaSu;

	long						lVideoStandSuNum;         //��֧�ֵ���ʽ����hh
	TPImageFormatSu*            pImageFormatSu;
    
	DWORD                       dwVideoStandSu;           //��֧�ֵ���ʽhh
	BOOL                        bRealtime;               //�Ƿ�ʵʱ  
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



//��Ƶ֧�ֽṹ����
typedef struct tag_TPAudioFormatSu    //������Ƶѹ����ʽ�Ͳ���
{
	DWORD	   				dwSize;				//�ṹ��С
	DWORD	   				dwVersion;			//�汾��

	TCHAR				    ptszFormat[MAX_FILETYPE_NAME];	//��Ƶ��ʽ����
	DWORD	   				dwFOURCC;			//FOURCC Codes
	TP_ACCESS_MODE			tpAccessType;		//�������Խ���ѹ�����ǽ�ѹ��

	//��Ƶ������Ϣ
	long      				lChannelInfo;      //TP_AudioChannel
	long      				lSamplesInfo;	    //TPAudioSample
	long      				lBitsInfo;			//����λ�� : TP_AudioBits
	//��Ƶ������Ϣ

	//ѹ������
	BOOL       				bIsCompress;        //�Ƿ�ѹ��
	DWORD	   				dwMinCompDataRate;	//�����ʣ���kbpsΪ��λ
	DWORD	   				dwMaxCompDataRate;	//�����ʣ���kbpsΪ��λ
	DWORD	   				dwPrefCompDataRate;	//�����ʣ���kbpsΪ��λ
	//ѹ������

	DWORD      				dwPrivateSize;
	void*      				pPrivateData;
}TPAudioFormatSu,*LPTPAudioFormatSu;


typedef struct tab_TPExtNameTable			//��չ����
{
	DWORD	iSize;					//�ṹ��С
	DWORD	dwVersion;			//�汾��
	TCHAR	ptszExtName[MAX_EXT_NAME_SIZE];	//����֧�ֶ����ļ���չ������	
}TPExtNameTable,*LPTPExtNameTable;

//�ļ�֧�ָ�ʽ���壨����ѹ����ʽ�Ͳ�������ѹ���ļ�����͸�������Ҫ����������½ṹ���б�
typedef struct tag_TPFileFormatSu
{
	DWORD               dwSize;
	DWORD               dwVersion;

	TCHAR				ptszFileTypeGroup[MAX_FILETYPE_NAME];		//�ļ�����������
	//�������avi�ļ���ͬ����avi������

	long				lExtNameCount;							//֧���ļ���չ������
	LPTPExtNameTable	lpExtNameTable;							//��չ����(�����ڲ����)

	GUID                guidFileType;
	TCHAR				ptszFileTypeName[MAX_FILETYPE_NAME];		//�ļ���������
	TP_ACCESS_MODE      tpAccessMode;

	BOOL				bCanVideo;
	BOOL				bCanAudio;

	long                lVideoFormatSuNum;
	TPVideoFormatSu*    pVideoFormatSu;

	long                lAudioFormatSuNum;
	TPAudioFormatSu*    pAudioFormatSu;

	DWORD               dwExtraType;
	DWORD               dwVideoStandardSu;//��֧�ֵ���ʽ hh

}TPFileFormatSu,*LPTPFileFormatSu;




//�������ϵͳ�������ݽṹ
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
//�������ϵͳ�������ݽṹ


/******************��Ƶ��ʽGroup����******************
�����ַ������б�ʶ
******************��Ƶ��ʽGroup����******************/


//�����ı�ṹ����
typedef struct tag_TPParamChangeStu
{
	int iIndex;  
	int iSubIndexCount;    //mini  param adjʱ�����
	int* pIndexArray;  

	tag_TPParamChangeStu()
	{
		iIndex = -1;
		iSubIndexCount = -1;//���
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

typedef struct tag_Face_Area   //���� add 20110601
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
	double  dRatio;  //���ٷֱ�
	BOOL    bBorder;
	long    lSafeBoxX;//ȡ1������
	long    lSafeBoxY;
	BOOL	b169Border;
	BOOL    bCGBorder;
	long    lCGSafeBoxX;//ȡ1������
	long    lCGSafeBoxY;
	BOOL	bCG169Border;
	BOOL    bOnlyMove;
	BOOL    bOnlyZoom;
	POINT   ptOffset;
	BOOL    bDrawGrid;
	TPEffectSet_Grid tpGrid;
	TPTextOut tpTextOut;//�������
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
		lCGSafeBoxX = 0;//ȡ1������
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
	TPTextOut tpTextOut;//�������
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

//[[������¼��ǰ����ĺܶ�������Ϣ
struct  TPEngineConfigPara
{
	int		nCGProxyEditmode;//proxy mode cg Full edit or not
	int		nCGScrollMode;	//scroll mode 
	BOOL	nAudioMuteWhenNRealTime;//not realtime audio mute.
	unsigned int nCGMemoryLimit;	//cg use total mem.
	long    lRealSDlProxyRate;		
	long    lRealHDlProxyRate;
	BOOL	bField;//Frame or field ��Ҫ��720P��Proxy�༭��
	BOOL	bFieldTwiceRedner;
	long    lSoundCardDelayTime;  //�����ӳ�ʱ��/for sdi-����ͬ��

	// tnt add-09.08.10 --- ������¼��Ƶ֡���ָ�ķ�����Ϊ�˱�1920����������ӦΪ16�ı���
	int     nCutCount;
	// tnt end

	//tnt add(tanting) - 10.08.20 --- ������Ƶ�����̣�threadpool���ı�־, 0��ʾ�����ã�1��ʾ����
	int     nAudioThreadPool;
	//tnt end

	void Reset()
	{
		nCGProxyEditmode		= 1;//0ΪcgΪproxy�༭��1Ϊcg�������༭��engine�������š�
		nCGScrollMode			= 1;//0Ϊ��videort��1Ϊ��2048rt
		nAudioMuteWhenNRealTime = 1;//0 Ϊ���� 1Ϊ��������
		nCGMemoryLimit			= 100000000;	// 100MB
		lRealSDlProxyRate       = 2;
		lRealHDlProxyRate       = 4;
		bField					= TRUE;//Frame or field ��Ҫ��720P��Proxy�༭��
		bFieldTwiceRedner		= TRUE;

		// tnt add-09.08.11 --- ������¼��Ƶ֡���ָ�ķ�����Ϊ�˱�1920����������ӦΪ16�ı���
		nCutCount               = 16;
		lSoundCardDelayTime     = 40;
		// tnt end

		//tnt add(tanting) - 10.08.20 --- ������Ƶ�����̣�threadpool���ı�־, 0��ʾ�����ã�1��ʾ����
		nAudioThreadPool        = 0;
		//tnt end
	}
	BOOL copy(TPEngineConfigPara tpSourcePara)
	{
		nCGProxyEditmode		= tpSourcePara.nCGProxyEditmode;//0ΪcgΪproxy�༭��1Ϊcg�������༭��engine�������š�
		nCGScrollMode			= tpSourcePara.nCGScrollMode;
		nAudioMuteWhenNRealTime = tpSourcePara.nAudioMuteWhenNRealTime;
		nCGMemoryLimit			= tpSourcePara.nCGMemoryLimit;
		lRealSDlProxyRate       = tpSourcePara.lRealSDlProxyRate;
		lRealHDlProxyRate       = tpSourcePara.lRealHDlProxyRate;
		bField					= tpSourcePara.bField;
		bFieldTwiceRedner		= tpSourcePara.bFieldTwiceRedner;
		lSoundCardDelayTime     = tpSourcePara.lSoundCardDelayTime;

		// tnt add-09.08.11 --- ������¼��Ƶ֡���ָ�ķ�����Ϊ�˱�1920����������ӦΪ16�ı���
		nCutCount               = tpSourcePara.nCutCount;
		// tnt end

		//tnt add(tanting) - 10.08.20 --- ������Ƶ�����̣�threadpool���ı�־, 0��ʾ�����ã�1��ʾ����
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