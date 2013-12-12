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
	// 	// �ֿ��ȡͼ������ Begin
	// 	int NXjpegPrepareGetImage(LPTPResource lpResource, UINT *pGrayPlatte, void **ppPrivateInfo);
	// 	int NXjpegReadImageBlock(LPTPResource lpResource, int iBeginRow, int iEndRow, UINT *pGrayPlatte, void *pPrivateInfo);
	// 	// �ֿ��ȡͼ������ End

};
