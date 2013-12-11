//////////////////////////////////////////////////////////////////////////
// �ڴ�����Ż��ӿ�
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
	_TP_HDSD_NoChange,     //���仯 
	_TP_HDSD_Stretch,      //���
	_TP_HDSD_CutHorz,      //ˮƽ����
	_TP_HDSD_EstopVert,    //��ֱ����
	_TP_HDSD_Custom,       //��ֱ����
};

// enum ADAPT_EXCEPTION
// {
// 	ADAPTE_NORMAL	= 0,
// 	ADAPTE_480_486,
// };

typedef long  _TP_MEDIA_CLASS_TYPE;

#define TP_ADAPTFALG_UNKNOW			0x0000000000
#define TP_ADAPTFLAG_480_486		0x0000000001		// N��,480����ƵҪ��չ��Ϊ486������4�У�����2�У����Ϊ��ɫ��
#define TP_ADAPTFLAG_1609			0x0000000002		// �����ͼƬ��Ҫ������16��9���棻����ȷ���ᱻ���죩

struct _TPTLClipHDSDChange
{
	BOOL						bHDSDFlag;			// NOT USED
	_TP_PICTURE_POS				ePicturePos;       // NOT USED, Save//ͼƬλ��
	POINT						ptPicturePos;      // NOT USED, Save//ͼƬλ�� 
	TP_VIDEO_STANDARD			eClipVideoStand;	// �زĵ���ʽ
	TP_VIDEO_STANDARD         eTLVideoStand;		// ʱ����/Ŀ����ʽ
	_TP_HDSD_MODE              eHDSDMode;			// ���䷽ʽ
	long                       lSpecify;			// custom adaption parameter
	double                     dCenterX;			// position after adaption, [0,1]
	double                     dCenterY;

	LPBYTE                     pSrcBuffer;		// Դbufferָ��
	SIZE                      szSrc;				// Դbuffer��С
	RECT						rcSrc_buffer;		// NOT USED, ��buffer�ϵ���Ч����
	POINT						posSrc;				// NOT USED, ��Ч������ȫ���µ�λ��

	LPBYTE                     pDestBuffer;		// Ŀ��bufferָ��
	SIZE                      szDest;				// Ŀ��buffer��С

	_TP_MEDIA_CLASS_TYPE       eType;				// Դ�����ͣ�Ŀǰ����������ͼƬ������Ƶ

	BOOL						bYUV16;				// Դ��ɫ��ʽ�Ƿ�ΪYUYV��UYUV���ڲ����ᰴ�մ����buffer���е�������ȼ�����㣩
	//ADAPT_EXCEPTION			eAdaptException;		//buffer��Ҫ����ת��,������������ϳ�����ʹ��lAdaptFlag
	INT64						lAdaptFlag;			// ���丽�ӱ�־����֧�ֵ�������ο�TP_ADAPTFALG��

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

// ��ʱ���߽����Ľṹ����������
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
// ��������

//////////////////////////////////////////////////////////////////////////
// ����һ�� Buffer
LRESULT CALLBACK TP_MemcpyLine(OUT void *des,		// Ŀ��
							   IN void *src,		// Դ
							   IN int nlen);		// �ֽڳ���
// ���ֽ�Ϊ��λ����һ�� Buffer
LRESULT CALLBACK TP_MemsetChar(OUT void *des,		// Ŀ��
							   IN char value,		// �ֽ�ֵ
							   IN int nlen);		// �ֽڳ���
// һ�� Buffer ��˫����ֵ����
LRESULT CALLBACK TP_MemsetDWORD(OUT DWORD *des,		// Ŀ��
								IN DWORD value,		// Դ
								IN int nsize);		// ˫�ֵĸ���
#define TP_DMemSet		TP_MemsetDWORD
// ��ת
BOOL CALLBACK TP_OverTurnSelf(IN OUT void* pBuf,    // �ڴ�
							  IN int nPitch,        // һ���ֽڿ�ȣ����ֽڶ��룩
							  IN int nHeight);      // �߶�
BOOL CALLBACK TP_OverTurnCopy(OUT void* pDes,       // Ŀ��
							  IN int nDesPitch,     // Ŀ��һ���ֽڿ�ȣ����ֽڶ��룩
							  IN void* pSrc,        // Դ
							  IN int nSrcPitch,     // Դһ���ֽڿ�ȣ����ֽڶ��룩
							  IN int nHeight);      // �߶�
// ��Դ unit �е�ĳ���������� cell ���ѵ�����Ŀ����
// һ�� unit ���ֽڳ�Ϊ 32/16/8/4/2��cell ��λ��Ϊ 8��Ҳ��һ���ֽڣ�
// ���罫һ�� BGRA �����е� A ͨ��ȡ��������ȡ 8 ͨ����Ƶ�ĵ� 1 ͨ��
// Ҫ����������Լ�Ҫ������ cell ���������߼���ȫ��
void CALLBACK TP_MemSplit(OUT void *des,			// Ŀ�꣨���ѽ����
						  IN void *src,				// Դ
						  IN int nUnitCount,		// ���� unit �ĸ���
						  IN int nUnitSize,			// unit ���ֽڳ�
						  IN int nCellStart,		// unit �� cell �ķ������
						  IN int nCellCount);		// unit �� cell �ķ��Ѹ���
// ��Դ��ĳ���������� cell �ϳɵ���Ŀ�� unit ��
// һ�� unit ���ֽڳ�Ϊ 32/16/8/4/2��cell ��λ��Ϊ 8��Ҳ��һ���ֽڣ�
// ���罫һ�� A ͨ���ϳɵ�һ�� BGRA �����У����罫 1 ͨ������Ƶ�ϳɵ� 8 ͨ����
// Ҫ����������Լ�Ҫ������ cell ���������߼���ȫ��
void CALLBACK TP_MemCompose(OUT void *des,			// Ŀ�꣨�ϳɽ����
							IN void *src,			// Դ
							IN int nUnitCount,		// �ϳ� unit �ĸ���
							IN int nUnitSize,		// unit ���ֽڳ�
							IN int nCellStart,		// unit �� cell �ĺϳ����
							IN int nCellCount);		// unit �� cell �ĺϳɸ���
//////////////////////////////////////////////////////////////////////////
// RGB Mean ����
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

// ��������BitBlt�㷨��ͬ
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
							  IN		DWORD* pDes,		//Ŀ��
							  IN		DWORD* pSrc,		//Դ
							  IN		int nWidth			//���
							  );
// ����һ�������ֵ
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
// ���û�������Alpha�ֽ�ֵ
void* CALLBACK TP_AlphaMemSet(IN OUT DWORD *pdwBuffer, 
							  IN BYTE byValue, 
							  IN int nWidth,
							  IN int nHeight);

// ��ģ�������
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
// ԴBufferƽ�̿�����Ŀ��Buffer��
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

// ��˫���Բ�ֵ���л�����������
LRESULT CALLBACK TP_BufferZoom(IN OUT BYTE* pbyDestBuf,
							   IN int nDestWidth,
							   IN int nDestHeight,
							   IN BYTE* pbySrcBuf,
							   IN int nSrcWidth,
							   IN int nSrcHeight);
// ��˫���Բ�ֵ���л�����������
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
// YUYV���쿽��
LRESULT CALLBACK TP_StretchBlt_YUYV(
									IN OUT BYTE* pbyDestBuffer, 
									IN int nDestPitch,
									IN int nDestBufferWidth,
									IN int nDestBufferHeight,
									IN BYTE* pbySrcBuffer,
									IN int nSrcPitch,
									IN int nSrcBufferWidth,		// ���ص���
									IN int nSrcBufferHeight);

/*
�����ҿ���
*/
LRESULT CALLBACK TP_VideoAdapte( _TPTLClipHDSDChange &adaptParam );

/*
���䣬����Դ��Ŀ��ľ���
*/
void CALLBACK TP_RectAdapt( _TPTLClipHDSDChange &adaptParam, RECT& rcSrc, RECT& rcDst, RECT* pRcDstFull = NULL );

LRESULT CALLBACK TP_BufferAdapte(
								 IN OUT BYTE* pbyDestBuffer, //Ŀ��buffer
								 IN int nDestBufferWidth,//Ŀ��buffer��
								 IN int nDestBufferHeight,//Ŀ��buffer��
								 IN BYTE* pbySrcBuffer,//��Ҫ�����Դbuffer
								 IN int nSrcBufferWidth,//��Ҫ�����Դbuffer��		
								 IN int nSrcBufferHeight,//��Ҫ�����Դbuffer��
								 IN RECT rtAdapte,//Դbuffer����Ҫ����ľ��δ�С
								 IN _TP_HDSD_MODE eMode,//��������
								 IN long lSpecify,// custom adaption parameter
								 IN OUT RECT& rtDesAdapte);//�����ľ��δ�С

BOOL yuv_yyuuvv2yuv(BYTE* pDesBuffer,BYTE* pSrcField,int nA16,int nMod16);
BOOL yuv_yuv2yyuuvv(BYTE* pDesBuffer,BYTE* pSrcField,int nA8,int nMod8);
void yuv_uyvy2yuv8(BYTE* puyvy,BYTE* py,BYTE* pu,BYTE* pv,int nA16,int nM16);
void yuv_yuyv2yuv8(BYTE* puyvy,BYTE* py,BYTE* pu,BYTE* pv,int nA16,int nM16);

DWORD* TP_VerticalBlur(DWORD* pSrcBuffer, int nWidth, int nHeight, int* pFactor);

// ��ȡCPUʱ�����ڼ�����
unsigned _int64 TP_GetCycleCount();

// ���ɽ�����ɫ���͸��ɫ����
void TP_MakeTranslucentBack(OUT DWORD* pBuf,			// ���� Buffer
							IN int nBufWidth,			// Buffer ��С
							IN int nBufHeight,
							IN int nBlockSize,			// �����ɫ���С
							IN DWORD crOddBlock,		// ��������ɫ����ɫ
							IN DWORD crEvenBlock);		// ż������ɫ����ɫ


#endif		// #ifndef __TP_TPBUFFEROPERATEAPI_INC__
//////////////////////////////////////////////////////////////////////////
// End of file.