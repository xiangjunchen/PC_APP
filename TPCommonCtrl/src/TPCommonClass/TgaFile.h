#ifndef _TGA_FILE_H_
#define _TGA_FILE_H_
//ѹ����ʽ�궨��
#define MT_COMPRESS_UNCOMP	(WORD)0			//��ѹ��
#define MT_COMPRESS_RLE4	(WORD)1			//BMP16ɫλͼѹ����ʽRLE4
#define MT_COMPRESS_RLE8	(WORD)2			//BMP256ɫλͼѹ����ʽRLE8
#define MT_COMPRESS_PCXRLE	(WORD)3			//PCXѹ����ʽRLE
#define MT_COMPRESS_GIFLZW	(WORD)4			//GIFѹ����ʽLZW
#define MT_COMPRESS_TGARLE	(WORD)5			//TGAѹ����ʽRLE

#define MT_MASK_IMAGE		(WORD)1			//����ģ�Ҷ�ͼ
#define MT_MASK_NOMAL		(WORD)0			//һ��ͼ��
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
	DWORD	Xstart;				//ʵ�����X
	DWORD	Ystart;				//ʵ�����Y
	DWORD	Width;				//ʵ�ʿ��
	DWORD	Height;				//ʵ�ʸ߶�
	BYTE	bits;				//λͼ��ɫ���
	BOOL	fCompression;		//�Ƿ�ѹ��
	DWORD	dwBmpType;			//��������
	LONGLONG llFileSize;		//�ļ����� add by ljj @ 2009-3-4
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
	BOOL				NXPrepareLoadTGAImage(DWORD*	dbuf); // Ϊ�ֿ��ȡͼ��������׼��
	BOOL				NXReadImage( int iBeginRow, int iEndRow); // �ֿ��ȡͼ������
	BOOL				_NXImageBeginLeftBottom(void); // ����ͼ�������Ƿ��Ǵ����½ǿ�ʼ�� add by wgf@2010-5-21
	BOOL				_NXHaveAlpha(void); // ͼ���Ƿ��� alpha ��Ϣ add by wgf@2010-5-31
	BOOL				_NXSupportBlockRead(void); // ͼ���Ƿ���Էֿ��ȡ by ljj@2011-4-28
	BOOL				WriteTGAImage(
							IN IMAGE_INFO	ImageInfo,
							IN DWORD*		dbuf,
							BYTE* pCustomData, long lCustomDataSize);
	BOOL				GetTGAInfo(IMAGE_INFO	&ImageInfo,BYTE*& pBuf, long& lBufSize);
	BOOL				UpdateCustomData(BYTE* pCustomData, long lCustomDataSize);

private:
	HANDLE			m_pTgaFileHandle; // ʹ�� ReadFile �� API ��ȡͼ�����ݾ��
	DWORD				m_dwAccessType;//File access type,Read/Write
	LARGE_INTEGER		m_llFilePos;//�ļ�ָ��λ��
	TGA_HEAD			m_cTGAHead;
	IMAGE_INFO			m_stImageInfo;
	BYTE*				m_pCustomData;
	long				m_lCustomDataSize;
	DWORD*				m_pImgBuf;//32λͼ������,������ͣ��Լ�����
	RGBQUAD*			m_pClrMap;//��ɫ��

	BYTE *				m_pNXRLCBuf; // ������ RLC �ļ�ʱ���洢���������� add by wgf @2010-4-27
	DWORD			m_dwNXRLCBufCapcity; // RLC�����������m_dwNXRLCBufCapcity - m_dwNXRLCBufCursor == ʣ��ˮ��
	BOOL				ReadClrMap();//��ȡ��ɫ��
	BOOL				GetTGAInfo();
	BOOL				ReadImage();
	
	BOOL				FillDstBufByClrMap(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, long lDstBits,BYTE* pSrc,long lSrcBits,long lWidth, long lHeight,BYTE byLocFlag);
	BOOL				FillDstBufByClrMap8(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag);
	BOOL				FillDstBufByClrMap16(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag);
	BOOL				FillDstBufByClrMap24(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag);
	BOOL				FillDstBufByClrMap32(RGBQUAD*	pClrMap,unsigned long ulClrMapCount,BYTE* pDst, BYTE* pSrc,long lWidth, long lHeight,BYTE byLocFlag);

	BOOL				Tga16To32(DWORD* pDst, BYTE* pSrc, long lWidth, long lHeight, BYTE byLocFlag);//����ѹ��16λת��Ϊ32λ
	BOOL				TGARLCDec(DWORD* pDst, BYTE* pSrc, LONGLONG llSrcSize,long lWidth,long lHeight,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount,BYTE byLocFlag);//RLC����,Ŀ��Ϊ32λ
	// ��RLC����ؿգ���û��ȡ��ָ���Ŀ�����ݣ����򻺳��עˮ
	BOOL				NXTGARLCDec(DWORD* pDst, BYTE* pSrc, LONGLONG llSrcSize,long lWidth,long lHeight,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount,BYTE byLocFlag);//RLC����,Ŀ��Ϊ32λ
	int				TGARLCOneValue(RGBBIT& stDstClr,BYTE* pSrc, LONGLONG& llSrcByteIdx,LONGLONG llSrcSize,long lSrcBits,RGBQUAD* pClrMap,long lClrMapCount);

	//BOOL	CTgaFile::ReadImageDate(			//���ļ���ǰλ�ö���δѹ��ͼ������
	//	OUT DWORD*	&pdbuf,		//ͼ�󻺳�����ע��ֻ�ṩ��ָ��
	//	IN	CFile*	loadFile,	//�����ļ�ָ��
	//	IN	RGBQUAD	inpal[256],	//��ɫ����Ϣ
	//	IN	BOOL	bHasPal,		//���޵�ɫ��
	//	IN	DWORD	Width,		//ͼ����
	//	IN	DWORD	Height,		//ͼ��߶�
	//	IN	WORD	ColorDepth,	//����λ��
	//	IN	WORD	CompressType,//ѹ����ʽ
	//	IN	BYTE	BeginFlag	//��ʼλ��(3:2:1:0)//->TGAֻ��2��0.
	//	);
	//BOOL		ImageTo8Buf(			//��32λ������ת��Ϊ8λ������
	//			IN   DWORD* ddbuf,		//32λ������
	//			IN IMAGE_INFO ImageInfo,//32λ��������Ϣ
	//			OUT  BYTE* &bbuf,		//8λ������
	//			OUT  DWORD* oLength,	//8λ����������
	//			OUT  RGBQUAD pal[256],	//8λ��ɫ��
	//			IN   BOOL fCompression //�Ƿ�ѹ��
	//						);

};
#endif