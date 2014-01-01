#ifndef _TGA_FILE_H_
#define _TGA_FILE_H_
//压缩方式宏定义
#define MT_COMPRESS_UNCOMP	(WORD)0			//无压缩
#define MT_COMPRESS_RLE4	(WORD)1			//BMP16色位图压缩格式RLE4
#define MT_COMPRESS_RLE8	(WORD)2			//BMP256色位图压缩格式RLE8
#define MT_COMPRESS_PCXRLE	(WORD)3			//PCX压缩格式RLE
#define MT_COMPRESS_GIFLZW	(WORD)4			//GIF压缩格式LZW
#define MT_COMPRESS_TGARLE	(WORD)5			//TGA压缩格式RLE

#define MT_MASK_IMAGE		(WORD)1			//是掩模灰度图
#define MT_MASK_NOMAL		(WORD)0			//一般图形
#define MT_IMAGE_TL			0
#define MT_IMAGE_BL			1
#define TP_IMG_FILE_READ  0x00000001
#define TP_IMG_FILE_WRITE 0x00000002
#define TP_IMG_FILE_CREATE 0x00000004
#define TP_TGA_MAX_CUSTOMDATASIZE 255

/////////////////////////////////////////////////////////
//////////////////////////////////////////
typedef union tagRGBBIT { // rgbq 
	RGBQUAD RGBbits;
	DWORD	RGBdword;
} RGBBIT;
//////////////////////////////////////////

typedef struct
{
	DWORD	Xstart;				//实际起点X
	DWORD	Ystart;				//实际起点Y
	DWORD	Width;				//实际宽度
	DWORD	Height;				//实际高度
	BYTE	bits;				//位图颜色深度
	BOOL	fCompression;		//是否压缩
	DWORD	dwBmpType;			//数据类型
	LONGLONG llFileSize;		//文件长度 add by ljj @ 2009-3-4
}IMAGE_INFO;
#pragma pack(push, 1) //add by luotian 2009.02.18
typedef   struct
{
	BYTE	Identsize;
	BYTE	Colormaptype;
	BYTE	Imagetype;
	WORD	Colormapstart;
	WORD	Colormaplength;
	BYTE	Colormapbits;
	WORD    Xstart;
	WORD	Ystart;
	WORD    Width;
	WORD    Height;
	BYTE    Bits;
	BYTE    ImgDescriptor;
}TGA_HEAD;
#pragma pack(pop) //add by luotian 2009.02.18
class CTgaFile
{
public:
	CTgaFile();
	~CTgaFile();
public:

	BOOL				OpenTgaFile(LPCTSTR szFileName,DWORD dwOpenType);
	BOOL				CloseTgaFile();
	BOOL				LoadTGAImage(DWORD* dbuf);
	BOOL				NXPrepareLoadTGAImage(DWORD*	dbuf); // 为分块读取图像数据做准备
	BOOL				NXReadImage( int iBeginRow, int iEndRow); // 分块读取图像数据
	BOOL				_NXImageBeginLeftBottom(void); // 返回图象数据是否是从左下角开始的 add by wgf@2010-5-21
	BOOL				_NXHaveAlpha(void); // 图像是否含有 alpha 信息 add by wgf@2010-5-31
	BOOL				_NXSupportBlockRead(void); // 图像是否可以分块读取 by ljj@2011-4-28
	BOOL				WriteTGAImage(
							IN IMAGE_INFO	ImageInfo,
							IN DWORD*		dbuf,
							BYTE* pCustomData, long lCustomDataSize);
	BOOL				GetTGAInfo(IMAGE_INFO	&ImageInfo,BYTE*& pBuf, long& lBufSize);
	BOOL				UpdateCustomData(BYTE* pCustomData, long lCustomDataSize);

private:
	HANDLE			m_pTgaFileHandle; // 使用 ReadFile 等 API 读取图像数据句柄
	DWORD				m_dwAccessType;//File access type,Read/Write
	LARGE_INTEGER		m_llFilePos;//文件指针位置
	TGA_HEAD			m_cTGAHead;
	IMAGE_INFO			m_stImageInfo;
	BYTE*				m_pCustomData;
	long				m_lCustomDataSize;
	DWORD*				m_pImgBuf;//32位图像数据,外面给送，自己不存
	RGBQUAD*			m_pClrMap;//颜色表

	BYTE *				m_pNXRLCBuf; // 当解码 RLC 文件时，存储待解码数据 add by wgf @2010-4-27
	DWORD			m_dwNXRLCBufCapcity; // RLC缓冲池容量，m_dwNXRLCBufCapcity - m_dwNXRLCBufCursor == 剩余水量
	BOOL				ReadClrMap();//读取颜色表
	BOOL				GetTGAInfo();
	BOOL				ReadImage();
	
	BOOL				FillDstBufByClrMap(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, long lDstBits,BYTE* pSrc,long lSrcBits,long lWidth, long lHeight,BYTE byLocFlag);
	BOOL				FillDstBufByClrMap8(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag);
	BOOL				FillDstBufByClrMap16(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag);
	BOOL				FillDstBufByClrMap24(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag);
	BOOL				FillDstBufByClrMap32(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag);

	BOOL				Tga16To32(DWORD* pDst, BYTE* pSrc, long lWidth, long lHeight, BYTE byLocFlag);//把无压缩16位转换为32位
	BOOL				TGARLCDec(DWORD* pDst, BYTE* pSrc, LONGLONG llSrcSize,long lWidth,long lHeight,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount,BYTE byLocFlag);//RLC解码,目标为32位
	// 若RLC缓冲池空，而没有取到指定的宽高数据，则向缓冲池注水
	BOOL				NXTGARLCDec(DWORD* pDst, BYTE* pSrc, LONGLONG llSrcSize,long lWidth,long lHeight,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount,BYTE byLocFlag);//RLC解码,目标为32位
	int				TGARLCOneValue(RGBBIT& stDstClr,BYTE* pSrc, LONGLONG& llSrcByteIdx,LONGLONG llSrcSize,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount);

	//BOOL	CTgaFile::ReadImageDate(			//从文件当前位置读出未压缩图象数据
	//	OUT DWORD*	&pdbuf,		//图象缓冲区，注：只提供空指针
	//	IN	CFile*	loadFile,	//读出文件指针
	//	IN	RGBQUAD	inpal[256],	//调色板信息
	//	IN	BOOL	bHasPal,		//有无调色板
	//	IN	DWORD	Width,		//图象宽度
	//	IN	DWORD	Height,		//图象高度
	//	IN	WORD	ColorDepth,	//象素位数
	//	IN	WORD	CompressType,//压缩方式
	//	IN	BYTE	BeginFlag	//起始位置(3:2:1:0)//->TGA只有2，0.
	//	);
	//BOOL		ImageTo8Buf(			//把32位缓冲区转化为8位缓冲区
	//			IN   DWORD* ddbuf,		//32位缓冲区
	//			IN IMAGE_INFO ImageInfo,//32位缓冲区信息
	//			OUT  BYTE* &bbuf,		//8位缓冲区
	//			OUT  DWORD* oLength,	//8位缓冲区长度
	//			OUT  RGBQUAD pal[256],	//8位调色板
	//			IN   BOOL fCompression //是否压缩
	//						);

};
#endif