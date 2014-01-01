#pragma once
// ���� ReadFile & WriteFile ���ʴ��̵Ĵ������Ż�ͼ���ļ���д
#define MAX_BUFFER_POOL_SIZE 4096000 // Ŀǰ��ʱ����Ϊ 4M

// �µĴ������дͼƬ�࣬�Ѿ��͵ײ�LIB���� add by wgf@2010-11-14
class CSGBufferFile
{
public:
	CSGBufferFile();
	~CSGBufferFile();

	// ����ӿں���
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
	BYTE *m_pPoolRoof; // ��ȡʱ���ϴδ��ļ��ж�ȡ���ֽ���Ŀ��д��ʱ���Ѿ��򻺳��д���ֽ���Ŀ
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
//************************* ȡͼ���ļ��ı�����ʽ  **********************************
enum	TPImageProtectMode
{
	READ_NOChange,
	READ_Protect,			// ����
	READ_UnProtect,			// ������
	READ_Pattern
};

#define ALPHA_ZERO			0x00	// ȫ͸
#define ALPHA_FULL			0xFF	// ��͸

/////////////////// ARGB ��ɫ�ṹ(RGBQUAD) ///////////////////////////
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

// ������͸ Alpha ���
#define BFMixAlpha(aW)				(ALPHA_FULL)
#define BFMixRGB(cB, cW, aW)		(((aW*(cW-cB))>>8)+cB)
// Alpha ���
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

//����ֵ����

enum FIO_Sign
{
	FIO_OtherErr=0,					//δ֪����
	FIO_Succeed,						//�ɹ�
	FIO_OpenErr,						//�򿪴���
	FIO_TooLarge,						//�ļ�̫��
	FIO_BadFile,							//������ļ���ʽ
	FIO_Unsupported,					//�𻵻����ǲ�֧�ֵĸ�ʽ
	FIO_MemErr,							//�ڴ����
	FIO_NoContent,					//û����Ƶ����Ƶ
	FIO_BadRate,						//�������Ƶ��
	FIO_BadCompression,			//�����ѹ����ʽ
	FIO_BadCodec,						//������û�ҵ�
	FIO_FormatAccept,				//�����ʽ��֧��
	FIO_OutOfDiskSpace,			//���̿ռ䲻��
	FIO_ErrFileNotFound,			//�ļ�û�ҵ�
	FIO_ErrTooManyOpenFiles,	//̫��Ĵ��ļ�
	FIO_ErrPermErr,					//Ȩ�޲���
	FIO_ErrInvalidDrive,				//��������Ч
	FIO_ErrIo,								//�ļ������������
	FIO_ErrInUse,						//�ļ����ڱ�ʹ��
	FIO_LoadPlugInErr,				//���ز������
	FIO_NoInterface,					//û�нӿ�
	FIO_NoPlugIn,						//û�д˲��
	FIO_WriteErr,						//д�����
	FIO_ErrDiffVideoStand,			//��һ�µ���Ƶ��ʽ
	FIO_ErrNullFileName,			//������ļ���Ϊ��
	FIO_ErrNoIdx,						//��Ҫ�����ļ���û���ҵ������ļ�
	FIO_BadParameter,				// ��������ȷ����Ҫ�ǿ�ָ��
	FIO_NetWorkDrop,				// �������
};

// SG: �����ͼ��������ʹ�� LPTPResource->pPrivateData ��Я��
typedef struct tag_SGImageDescriptor{
	void *pPrivateData; // ����ͼ���ļ��ṹָ�룬�����δ��ļ�
	BYTE *pImage; // ͼ������ BGRA
	int iWidth; // ͼ�� �� & ��
	int iHeight;
	IMG_TYPE_ENUM eImageType; // ͼ������
	bool bReadFile; // ���� imgClose() ʱ��ͼ�������Ƿ��ͷ�
	bool bKeepOdd; // �Ƿ���������ͼ��Ŀ�� add by wgf@2010-5-18
	bool bHaveAlpha; // ͼ���Ƿ��� alpha ��Ϣ
	bool bPlayStatus; // true - ����ģʽ��˳��ȡ֡�������ظ�

	//[[ljj@2011-4-28:ͼ���ʽ�Ƿ�֧�ַֿ��ȡ:ѹ����TGA�ݲ����ֿ��ȡ��ʵ������̫�鷳��
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

typedef struct tag_TPResource			//��Դ��Ϣ
{
	int		iSize;						//�ṹ��С
	DWORD	dwVersion;			//�汾��
	FILE*	fp;							//�ļ�ָ��
	TCHAR	ptszfileName[MAX_PATH];		//�ļ�·����
	void*	pPrivateData;				//˽������
	void*   hData;                      //ʱ��������ָ��  
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
	// 	// �ֿ��ȡͼ������ Begin
	// 	int NXjpegPrepareGetImage(LPTPResource lpResource, UINT *pGrayPlatte, void **ppPrivateInfo);
	// 	int NXjpegReadImageBlock(LPTPResource lpResource, int iBeginRow, int iEndRow, UINT *pGrayPlatte, void *pPrivateInfo);
	// 	// �ֿ��ȡͼ������ End

};
