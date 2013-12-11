//////////////////////////////////////////////////////////////////////////
// 内存操作优化接口
//////////////////////////////////////////////////////////////////////////
#ifndef __TP_TPBUFFEROPERATEAPI_INC__
#define __TP_TPBUFFEROPERATEAPI_INC__

#include "TPBufferOperateDef.h"
#include "..\..\Include\TPBaseVADef.h"
//#include "TPBaseVADef.h"

enum	_TP_PICTURE_POS
{
	_TP_PICTURE_UNKNOW               =   0x00000000,
	_TP_PICTURE_TOPLEFT              =   0x00000001,
	_TP_PICTURE_TOP                  =   0x00000002,
	_TP_PICTURE_TOPRIGHT             =   0x00000004,
	_TP_PICTURE_RIGHT                =   0x00000008,
	_TP_PICTURE_RIGHTBOTTOM          =   0x00000010,
	_TP_PICTURE_BOTTOM               =   0x00000020,
	_TP_PICTURE_LEFTBOTTOM           =   0x00000040,
	_TP_PICTURE_LEFT                 =   0x00000080,
	_TP_PICTURE_CENTER               =   0x00000100,
	_TP_PICTURE_TILE                 =   0x00000200,
	_TP_PICTURE_STRETCH              =   0x00000400,
	_TP_PICTURE_CUSTOM               =   0x00000800,
};

enum _TP_HDSD_MODE
{
	_TP_HDSD_None,
	_TP_HDSD_NoChange,     //不变化 
	_TP_HDSD_Stretch,      //填充
	_TP_HDSD_CutHorz,      //水平适配
	_TP_HDSD_EstopVert,    //垂直适配
	_TP_HDSD_Custom,       //垂直适配
};

// enum ADAPT_EXCEPTION
// {
// 	ADAPTE_NORMAL	= 0,
// 	ADAPTE_480_486,
// };

typedef long  _TP_MEDIA_CLASS_TYPE;

#define TP_ADAPTFALG_UNKNOW			0x0000000000
#define TP_ADAPTFLAG_480_486		0x0000000001		// N制,480的视频要扩展成为486，上面4行，下面2行，填充为黑色；
#define TP_ADAPTFLAG_1609			0x0000000002		// 传入的图片，要看作是16：9幅面；（宽度方向会被拉伸）

struct _TPTLClipHDSDChange
{
	BOOL						bHDSDFlag;			// NOT USED
	_TP_PICTURE_POS				ePicturePos;       // NOT USED, Save//图片位置
	POINT						ptPicturePos;      // NOT USED, Save//图片位置 
	TP_VIDEO_STANDARD			eClipVideoStand;	// 素材的制式
	TP_VIDEO_STANDARD         eTLVideoStand;		// 时间线/目标制式
	_TP_HDSD_MODE              eHDSDMode;			// 适配方式
	long                       lSpecify;			// custom adaption parameter
	double                     dCenterX;			// position after adaption, [0,1]
	double                     dCenterY;

	LPBYTE                     pSrcBuffer;		// 源buffer指针
	SIZE                      szSrc;				// 源buffer大小
	RECT						rcSrc_buffer;		// NOT USED, 在buffer上的有效区域
	POINT						posSrc;				// NOT USED, 有效区域在全屏下的位置

	LPBYTE                     pDestBuffer;		// 目标buffer指针
	SIZE                      szDest;				// 目标buffer大小

	_TP_MEDIA_CLASS_TYPE       eType;				// 源的类型，目前用于区分是图片还是视频

	BOOL						bYUV16;				// 源颜色格式是否为YUYV或UYUV，内部将会按照传入的buffer进行调整（宽度减半计算）
	//ADAPT_EXCEPTION			eAdaptException;		//buffer需要经过转换,这个变量将被废除，请使用lAdaptFlag
	INT64						lAdaptFlag;			// 适配附加标志，可支持的类型请参考TP_ADAPTFALG宏

	_TPTLClipHDSDChange()
		:bHDSDFlag(FALSE),
		ePicturePos(_TP_PICTURE_UNKNOW),
		eClipVideoStand(TP_VIDEOSTANDARD_UNKNOW),
		eTLVideoStand(TP_VIDEOSTANDARD_UNKNOW),
		eHDSDMode(_TP_HDSD_None),
		lSpecify(100),
		pSrcBuffer(NULL),
		pDestBuffer(NULL),
		eType(0),			
		bYUV16(FALSE),
		lAdaptFlag(TP_ADAPTFALG_UNKNOW)
	{
		ptPicturePos.x = ptPicturePos.y = 0;
		dCenterX = (0.5);
		dCenterY = (0.5);
		szSrc.cx = szSrc.cy = 0;
		rcSrc_buffer.left = rcSrc_buffer.top = rcSrc_buffer.right = rcSrc_buffer.bottom = 0;
		posSrc.x = posSrc.y = 0;
		szDest.cx = szDest.cy = 0;
	}
};

// 和时间线交互的结构，用于适配
typedef struct stu_TPTLTrimViewAdaptPara
{
	DWORD               dwVersion;
	_TP_HDSD_MODE         eMode;
	long                lSpecify;

	stu_TPTLTrimViewAdaptPara()
	{
		dwVersion    = 0;
		eMode      = _TP_HDSD_EstopVert; 
		lSpecify   = 222;
	}
}TPTLTrimViewAdaptPara;


//////////////////////////////////////////////////////////////////////////
// 导出函数

//////////////////////////////////////////////////////////////////////////
// 拷贝一行 Buffer
LRESULT CALLBACK TP_MemcpyLine(OUT void *des,		// 目标
							   IN void *src,		// 源
							   IN int nlen);		// 字节长度
// 以字节为单位设置一行 Buffer
LRESULT CALLBACK TP_MemsetChar(OUT void *des,		// 目标
							   IN char value,		// 字节值
							   IN int nlen);		// 字节长度
// 一行 Buffer 的双字设值操作
LRESULT CALLBACK TP_MemsetDWORD(OUT DWORD *des,		// 目标
								IN DWORD value,		// 源
								IN int nsize);		// 双字的个数
#define TP_DMemSet		TP_MemsetDWORD
// 翻转
BOOL CALLBACK TP_OverTurnSelf(IN OUT void* pBuf,    // 内存
							  IN int nPitch,        // 一行字节宽度（四字节对齐）
							  IN int nHeight);      // 高度
BOOL CALLBACK TP_OverTurnCopy(OUT void* pDes,       // 目标
							  IN int nDesPitch,     // 目标一行字节宽度（四字节对齐）
							  IN void* pSrc,        // 源
							  IN int nSrcPitch,     // 源一行字节宽度（四字节对齐）
							  IN int nHeight);      // 高度
// 将源 unit 中的某几个连续的 cell 分裂到连续目标中
// 一般 unit 的字节长为 32/16/8/4/2，cell 的位长为 8（也即一个字节）
// 例如将一个 BGRA 像素中的 A 通道取出，又如取 8 通道音频的第 1 通道
// 要求输入参数以及要拷贝的 cell 个数都是逻辑安全的
void CALLBACK TP_MemSplit(OUT void *des,			// 目标（分裂结果）
						  IN void *src,				// 源
						  IN int nUnitCount,		// 分裂 unit 的个数
						  IN int nUnitSize,			// unit 的字节长
						  IN int nCellStart,		// unit 中 cell 的分裂起点
						  IN int nCellCount);		// unit 中 cell 的分裂个数
// 将源中某几个连续的 cell 合成到的目标 unit 中
// 一般 unit 的字节长为 32/16/8/4/2，cell 的位长为 8（也即一个字节）
// 例如将一个 A 通道合成到一个 BGRA 像素中，又如将 1 通道的音频合成到 8 通道中
// 要求输入参数以及要拷贝的 cell 个数都是逻辑安全的
void CALLBACK TP_MemCompose(OUT void *des,			// 目标（合成结果）
							IN void *src,			// 源
							IN int nUnitCount,		// 合成 unit 的个数
							IN int nUnitSize,		// unit 的字节长
							IN int nCellStart,		// unit 中 cell 的合成起点
							IN int nCellCount);		// unit 中 cell 的合成个数
//////////////////////////////////////////////////////////////////////////
// RGB Mean 缩放
LRESULT CALLBACK TP_StretchBlt_Mean(IN OUT DWORD* pdwDestBuffer,
									IN int nDestX,
									IN int nDestY,
									IN int nDestWidth,
									IN int nDestHeight,
									IN int nDestBufferWidth,
									IN int nDestBufferHeight,
									IN DWORD* pdwSrcBuffer,
									IN int nSrcX,
									IN int nSrcY,
									IN int nSrcWidth,
									IN int nSrcHeight,
									IN int nSrcBufferWidth,
									IN int nSrcBufferHeight,
									IN int nMix,
									IN int nParity);
//////////////////////////////////////////////////////////////////////////

// 下面两种BitBlt算法不同
LRESULT CALLBACK TP_BitBlt(
						   IN OUT DWORD* pdwDestBuffer, 
						   IN int nDestX,
						   IN int nDestY,
						   IN int nDestWidth,
						   IN int nDestHeight,
						   IN int nDestBufferWidth,
						   IN int nDestBufferHeight,
						   IN DWORD* pdwSrcBuffer,
						   IN int nSrcX,
						   IN int nSrcY,
						   IN int nSrcWidth,
						   IN int nSrcHeight,
						   IN int nSrcBufferWidth,
						   IN int nSrcBufferHeight,
						   IN int nMix,
						   IN int nParity);

LRESULT CALLBACK TP_QuickBitBlt(
								IN OUT DWORD* pdwDestBuffer, 
								IN int nDestX,
								IN int nDestY,
								IN int nDestWidth,
								IN int nDestHeight,
								IN int nDestBufferWidth,
								IN int nDestBufferHeight,
								IN DWORD* pdwSrcBuffer,
								IN int nSrcX,
								IN int nSrcY,
								IN int nSrcWidth,
								IN int nSrcHeight,
								IN int nSrcBufferWidth,
								IN int nSrcBufferHeight,
								IN int nMix,
								IN int nParity);
LRESULT CALLBACK TP_StretchBlt(
							   IN OUT DWORD* pdwDestBuffer, 
							   IN int nDestX,
							   IN int nDestY,
							   IN int nDestWidth,
							   IN int nDestHeight,
							   IN int nDestBufferWidth,
							   IN int nDestBufferHeight,
							   IN DWORD* pdwSrcBuffer,
							   IN int nSrcX,
							   IN int nSrcY,
							   IN int nSrcWidth,
							   IN int nSrcHeight,
							   IN int nSrcBufferWidth,
							   IN int nSrcBufferHeight,
							   IN int nMix,
							   IN int nParity);
BOOL CALLBACK TP_ColorMixLine(
							  IN		DWORD* pDes,		//目标
							  IN		DWORD* pSrc,		//源
							  IN		int nWidth			//宽度
							  );
// 清理一个区域的值
void* CALLBACK TP_Clear(IN OUT DWORD *pdwBuffer,
						int nPitch,
						int nX,
						int nY,
						int nW,
						int nH,
						DWORD dwValue);
BOOL CALLBACK TP_FillMaskColor(
							   IN OUT DWORD* pDesBuffer, 
							   IN int nStartX,
							   IN int nStartY, 
							   IN int nWidth, 
							   IN int nHeight, 
							   IN BYTE* pSrcMask, 
							   IN int nMaskStartX, 
							   IN int nMaskStartY, 
							   IN int nMaskWidth, 
							   IN int nMaskHeight
							   );
void CALLBACK TP_CalMask(IN OUT BYTE *pbyMask,
						 IN int nStartX, 
						 IN int nStartY, 
						 IN int nMaskWidth,
						 IN int nMaskHeight,
						 IN BYTE *pnOperator,
						 IN int nOperatorWidth, 
						 IN int nOperatorHeight,
						 IN int nMainOperatorX,
						 IN int nMainOperatorY);
// 设置缓冲区的Alpha字节值
void* CALLBACK TP_AlphaMemSet(IN OUT DWORD *pdwBuffer, 
							  IN BYTE byValue, 
							  IN int nWidth,
							  IN int nHeight);

// 掩模混叠操作
LRESULT CALLBACK TP_MixMask(IN OUT BYTE *pbyDestMask,
							IN int nDestX, 
							IN int nDestY, 
							IN int nDestWidth,
							IN int nDestHeight,
							IN int nDestMaskWidth,
							IN int nDestMaskHeight,
							IN BYTE *pbySrcMask,
							IN int nSrcX, 
							IN int nSrcY,
							IN int nSrcWidth,
							IN int nSrcHeight,
							IN int nSrcMaskWidth,
							IN int nSrcMaskHeight);
// 源Buffer平铺拷贝到目标Buffer中
LRESULT CALLBACK TP_TileCopy(IN OUT DWORD* pdwDestBuf,
							 IN int nDestX,
							 IN int nDestY,
							 IN int nDestWidth,
							 IN int nDestHeight,
							 IN int nDestBufWidth,
							 IN int nDestBufHeight,
							 IN DWORD* pdwSrcBuf,
							 IN int nSrcX,
							 IN int nSrcY,
							 IN int nSrcWidth,
							 IN int nSrcHeight,
							 IN int nSrcBufWidth,
							 IN int nSrcBufHeight);

// 用双线性插值进行缓冲区的缩放
LRESULT CALLBACK TP_BufferZoom(IN OUT BYTE* pbyDestBuf,
							   IN int nDestWidth,
							   IN int nDestHeight,
							   IN BYTE* pbySrcBuf,
							   IN int nSrcWidth,
							   IN int nSrcHeight);
// 用双线性插值进行缓冲区的缩放
LRESULT CALLBACK TP_DwordBufferZoom(IN OUT DWORD* pbyDestBuf,
									IN int nDestX,
									IN int nDestY,
									IN int nDestWidth,
									IN int nDestHeight,
									IN int nDestBufferWidth,
									IN int nDestBufferHeight,
									IN DWORD* pbySrcBuf,
									IN int nSrcX,
									IN int nSrcY,
									IN int nSrcWidth,
									IN int nSrcHeight,
									IN int nSrcBufferWidth,
									IN int nSrcBufferHeight
									);

void CALLBACK TP_CreateTableFor_H1920To1440();
void CALLBACK TP_DeleteTableFor_H1920To1440();
void CALLBACK TP_LinearScale_H1920To1440(OUT BYTE* pDest,
										 IN int nDestWidth,
										 IN int nDestPitch,
										 IN BYTE* pSrc,
										 IN int nSrcWidth,
										 IN int nSrcPitch,
										 IN int nHeight
										 );
// YUYV拉伸拷贝
LRESULT CALLBACK TP_StretchBlt_YUYV(
									IN OUT BYTE* pbyDestBuffer, 
									IN int nDestPitch,
									IN int nDestBufferWidth,
									IN int nDestBufferHeight,
									IN BYTE* pbySrcBuffer,
									IN int nSrcPitch,
									IN int nSrcBufferWidth,		// 像素点数
									IN int nSrcBufferHeight);

/*
适配且拷贝
*/
LRESULT CALLBACK TP_VideoAdapte( _TPTLClipHDSDChange &adaptParam );

/*
适配，返回源和目标的矩形
*/
void CALLBACK TP_RectAdapt( _TPTLClipHDSDChange &adaptParam, RECT& rcSrc, RECT& rcDst, RECT* pRcDstFull = NULL );

LRESULT CALLBACK TP_BufferAdapte(
								 IN OUT BYTE* pbyDestBuffer, //目标buffer
								 IN int nDestBufferWidth,//目标buffer宽
								 IN int nDestBufferHeight,//目标buffer高
								 IN BYTE* pbySrcBuffer,//需要适配的源buffer
								 IN int nSrcBufferWidth,//需要适配的源buffer宽		
								 IN int nSrcBufferHeight,//需要适配的源buffer高
								 IN RECT rtAdapte,//源buffer中需要适配的矩形大小
								 IN _TP_HDSD_MODE eMode,//适配类型
								 IN long lSpecify,// custom adaption parameter
								 IN OUT RECT& rtDesAdapte);//适配后的矩形大小

BOOL yuv_yyuuvv2yuv(BYTE* pDesBuffer,BYTE* pSrcField,int nA16,int nMod16);
BOOL yuv_yuv2yyuuvv(BYTE* pDesBuffer,BYTE* pSrcField,int nA8,int nMod8);
void yuv_uyvy2yuv8(BYTE* puyvy,BYTE* py,BYTE* pu,BYTE* pv,int nA16,int nM16);
void yuv_yuyv2yuv8(BYTE* puyvy,BYTE* py,BYTE* pu,BYTE* pv,int nA16,int nM16);

DWORD* TP_VerticalBlur(DWORD* pSrcBuffer, int nWidth, int nHeight, int* pFactor);

// 获取CPU时钟周期计数器
unsigned _int64 TP_GetCycleCount();

// 生成交叉颜色块的透明色背景
void TP_MakeTranslucentBack(OUT DWORD* pBuf,			// 背景 Buffer
							IN int nBufWidth,			// Buffer 大小
							IN int nBufHeight,
							IN int nBlockSize,			// 相间颜色块大小
							IN DWORD crOddBlock,		// 奇数列颜色块颜色
							IN DWORD crEvenBlock);		// 偶数列颜色块颜色


#endif		// #ifndef __TP_TPBUFFEROPERATEAPI_INC__
//////////////////////////////////////////////////////////////////////////
// End of file.