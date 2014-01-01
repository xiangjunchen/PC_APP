#pragma once
typedef void* HAVTGAFILE;

typedef struct tag_tagData
{
	HAVTGAFILE hFile;
	int			iX;
	int			iY;
	int        iWidth;
	int        iHeight;
	int        iDepth;
	BYTE       * pBuffer;
	TCHAR      file_name[MAX_PATH];
	BYTE*		pCustomData;//�������Ҫdelete��ֻҪhFile���ڴ򿪣����ɣ���
	long		lCustomDataSize;
	BOOL		bNewCustomBuffer;//�Ƿ�����New�Ŀռ�ljj
	LONGLONG	llFileSize;
	BOOL		bExistFile;//�Ƿ��Ǵ��Ѿ����ڵ�File��add by ljj @ 2009-3-4
	tag_tagData()
	{
		hFile=NULL;
		iX = 0;
		iY = 0;
		iWidth=0;
		iHeight=0;
		iDepth=0;
		pBuffer=NULL;
		ZeroMemory (file_name, sizeof (TCHAR) * MAX_PATH);
		pCustomData = NULL;
		lCustomDataSize = 0;
		bNewCustomBuffer = FALSE;
		llFileSize = 0;
		bExistFile = FALSE;
	}
}TGADATA;
#define TP_IMG_FILE_READ  0x00000001
#define TP_IMG_FILE_WRITE 0x00000002
#define TP_IMG_FILE_CREATE 0x00000004
//#define TP_TGA_MAX_CUSTOMDATASIZE 255


int TPAVTgaFileOpen(HAVTGAFILE& hFile,TCHAR* ptszFileName,DWORD dwOpenType );
int TPAVTgaFileOpen(HAVTGAFILE& hFile,TCHAR* ptszFileName );
int TPAVTgaFileClose(HAVTGAFILE hFile);
int TPAVTgaFileGetImageInfo(HAVTGAFILE hFile,int& iX, int& iY, int& iWidth,int& iHeight,int &iDepth,BYTE*& pBuf, long& lBufSize);//add X and Y location by ljj @ 2009-2-10 
int TPAVTgaFileGetImageInfo(HAVTGAFILE hFile,int& iWidth,int& iHeight,int &iDepth);//add X and Y location by ljj @ 2009-2-10 
int TPAVTgaFileGetImageData(HAVTGAFILE hFile,BYTE* pData);
int NXAVTgaFilePrepareGetImageData(HAVTGAFILE hFile,BYTE* pData, BOOL &bLeftBottom); // Ϊ�ֿ��ȡͼ��������׼��
int NXAVTgaFileGetImageData(HAVTGAFILE hFile, int iBeginRow, int iEndRow); // �ֿ��ȡͼ������
bool NXImageHaveAlpha(HAVTGAFILE hFile); // ���ش�ͼ���Ƿ��� alpha
int TPAVTgaFilePutImageData (HAVTGAFILE hFile, int iX, int iY, int iWidth, int iHeight, int iDepth, BYTE *pData);//add X and Y location by ljj @ 2009-2-10
int TPAVTgaFilePutImageData (HAVTGAFILE hFile, int iX, int iY, int iWidth, int iHeight, int iDepth, BYTE *pData, BYTE* pCustomData, long lCustomDataSize);//add X and Y location by ljj @ 2009-2-10
int TPAVTgaFileUpdateCustomData (HAVTGAFILE hFile,BYTE* pCustomData, long lCustomDataSize);//add by ljj @ 2009-3-4
bool NXTgaSupportBlockRead(HAVTGAFILE hFile); // ���ش�ͼ���Ƿ���Էֿ��ȡ by ljj@2011-4-28



// ����һ�� Buffer
LRESULT TP_MemcpyLine(OUT void *des,		// Ŀ��
					  IN void *src,		// Դ
					  IN int nlen);		// �ֽڳ���
// ���ֽ�Ϊ��λ����һ�� Buffer
LRESULT TP_MemsetChar(OUT void *des,		// Ŀ��
					  IN char value,		// �ֽ�ֵ
					  IN int nlen);		// �ֽڳ���

// һ�� Buffer ��˫����ֵ����
LRESULT TP_MemsetDWORD(OUT DWORD *des,		// Ŀ��
					   IN DWORD value,		// Դ
					   IN int nsize);		// ˫�ֵĸ���


void TPCS_24_to_32_Frame(OUT BYTE* pDes32,				// ������洢��ʽ X0 X1 X2 A X0 X1 X2 A ������
						 IN int nDesPitch,
						 IN BYTE* pSrc24,               // 24 λԴ���루�洢��ʽ X0 X1 X2 X0 X1 X2 ������
						 IN int nSrc24Pitch,            // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
						 IN BYTE* pSrcAlpha,            // Դ Alpha ���루�洢��ʽ A0 A1 A2 A3 ���������������Ϊ NULL ��ֱ���� 0xff��
						 IN int nSrcAlphaPitch,
						 IN int nWidth,                 // Դͼ��С
						 IN int nHeight,
						 IN BOOL bSrc24Revert,          // ͼ���Ƿ��з�ת��־
						 IN BOOL bSrcAlphaRevert);      // Alpha �Ƿ��з�ת��־
void TPCS_24_to_32_Split(OUT BYTE* pDes32First,         // ��һ��������洢��ʽ X0 X1 X2 A X0 X1 X2 A ������
						 IN int nFirstPitch,
						 OUT BYTE* pDes32Second,        // �ڶ���������洢��ʽ X0 X1 X2 A X0 X1 X2 A ������
						 IN int nSecondPitch,
						 IN BYTE* pSrc24,               // 24 λԴ���루�洢��ʽ X0 X1 X2 X0 X1 X2 ������
						 IN int nSrc24Pitch,            // Դһ�е��ֽڿ�ȣ�����Ϊ 4 �ı�����
						 IN BYTE* pSrcAlpha,            // Դ Alpha ���루�洢��ʽ A0 A1 A2 A3 ���������������Ϊ NULL ��ֱ���� 0xff��
						 IN int nSrcAlphaPitch,
						 IN int nWidth,                 // Դͼ��С
						 IN int nHeight,
						 IN BOOL bSrc24Revert,          // ͼ���Ƿ��з�ת��־
						 IN BOOL bSrcAlphaRevert);      // Alpha �Ƿ��з�ת��־
// ��ת
BOOL TP_OverTurnSelf(IN OUT void* pBuf,    // �ڴ�
					 IN int nPitch,        // һ���ֽڿ�ȣ����ֽڶ��룩
					 IN int nHeight)    ;   // �߶�

